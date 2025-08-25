#include "pakpatch.h"

#include <windows.h>

#include <string>

#include "detours.h"

#include "pakfile.h"
#include "utils.h"
#include "version.h"
#include "config.h"

namespace {

static DWORD resources_pak_size = 0;
static HANDLE resources_pak_map = nullptr;
static HANDLE resources_pak_file = nullptr;

static auto RawCreateFile = CreateFileW;
static auto RawCreateFileMapping = CreateFileMappingW;
static auto RawMapViewOfFile = MapViewOfFile;

HANDLE WINAPI MyMapViewOfFile(_In_ HANDLE hFileMappingObject,
                              _In_ DWORD dwDesiredAccess,
                              _In_ DWORD dwFileOffsetHigh,
                              _In_ DWORD dwFileOffsetLow,
                              _In_ SIZE_T dwNumberOfBytesToMap) {
  if (hFileMappingObject == resources_pak_map) {
    // Modify it to be modifiable.
    LPVOID buffer =
        RawMapViewOfFile(hFileMappingObject, FILE_MAP_COPY, dwFileOffsetHigh,
                         dwFileOffsetLow, dwNumberOfBytesToMap);

    // No more hook needed.
    resources_pak_map = nullptr;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(reinterpret_cast<LPVOID*>(&RawMapViewOfFile),
                 reinterpret_cast<void*>(MyMapViewOfFile));
    auto status = DetourTransactionCommit();
    if (status != NO_ERROR) {
      DebugLog(L"Unhook RawMapViewOfFile failed {}", status);
    }

    if (buffer && config.GetDisableUpdates()) {
      // Traverse the gzip file.
      TraversalGZIPFile(static_cast<BYTE*>(buffer), [=](uint8_t* begin,
                                                        uint32_t size,
                                                        size_t& new_len) {
        bool changed = false;

        BYTE search_start[] = R"(</settings-about-page>)";
        uint8_t* pos =
            memmem(begin, size, search_start, sizeof(search_start) - 1);
        if (pos) {
          // Compress the HTML for writing patch information.
          std::string html(reinterpret_cast<char*>(begin), size);
          compression_html(html);

          // Disable updates by redirecting the update URL to a non-existent address.
          std::string status_str = " [已停用更新";
          if (ReplaceStringInPlace(html, R"(hidden="[[!showUpdateStatus_]]")", R"(hidden="true")")) {
            status_str += " showUpdateStatus";
          }
          if (ReplaceStringInPlace(html, R"(hidden="[[!shouldShowIcons_(showUpdateStatus_)]]")", R"(hidden="true")")) {
            status_str += " shouldShowIcons";
          }
          if (ReplaceStringInPlace(html, R"(update-failure)", R"(update-failure-hidden)", true)) {
            status_str += " hideUpdateFailure";
          } else if (ReplaceStringInPlace(html, R"(update-notification)", R"(update-notification-hidden)", true)) {
            status_str += " hideUpdateNotification";
          } else if (ReplaceStringInPlace(html, R"(notification-area)", R"(notification-area-hidden)", true)) {
            status_str += " hideNotificationArea";
          }
          if (config.GetDisableUpdates()) {
            status_str += " GoogleUpdate";
          }
          status_str += "]</div>";


          std::string prouct_title =
              R"({aboutBrowserVersion}</div><div class="secondary"><a target="_blank" href="https://github.com/mitis1233/chrome_plus">Chrome++</a> )" RELEASE_VER_STR
              " modified version" + status_str;
          ReplaceStringInPlace(html, R"({aboutBrowserVersion}</div>)",
                               prouct_title);

          if (html.length() <= size) {
            // Write modifications.
            memcpy(begin, html.c_str(), html.length());

            // Modify length.
            new_len = static_cast<uint32_t>(html.length());
            changed = true;
          }
        }

        return changed;
      });
    }

    return buffer;
  }

  return RawMapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh,
                          dwFileOffsetLow, dwNumberOfBytesToMap);
}

HANDLE WINAPI MyCreateFileMapping(_In_ HANDLE hFile,
                                  _In_opt_ LPSECURITY_ATTRIBUTES lpAttributes,
                                  _In_ DWORD flProtect,
                                  _In_ DWORD dwMaximumSizeHigh,
                                  _In_ DWORD dwMaximumSizeLow,
                                  _In_opt_ LPCTSTR lpName) {
  if (hFile == resources_pak_file) {
    // Modify it to be modifiable.
    resources_pak_map =
        RawCreateFileMapping(hFile, lpAttributes, PAGE_WRITECOPY,
                             dwMaximumSizeHigh, dwMaximumSizeLow, lpName);

    // No more hook needed.
    resources_pak_file = nullptr;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(reinterpret_cast<LPVOID*>(&RawCreateFileMapping),
                 reinterpret_cast<void*>(MyCreateFileMapping));
    auto status = DetourTransactionCommit();
    if (status != NO_ERROR) {
      DebugLog(L"Unhook RawCreateFileMapping failed {}", status);
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<LPVOID*>(&RawMapViewOfFile),
                 reinterpret_cast<void*>(MyMapViewOfFile));
    status = DetourTransactionCommit();
    if (status != NO_ERROR) {
      DebugLog(L"Hook RawMapViewOfFile failed {}", status);
    }

    return resources_pak_map;
  }
  return RawCreateFileMapping(hFile, lpAttributes, flProtect, dwMaximumSizeHigh,
                              dwMaximumSizeLow, lpName);
}

HANDLE WINAPI MyCreateFile(_In_ LPCTSTR lpFileName,
                           _In_ DWORD dwDesiredAccess,
                           _In_ DWORD dwShareMode,
                           _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                           _In_ DWORD dwCreationDisposition,
                           _In_ DWORD dwFlagsAndAttributes,
                           _In_opt_ HANDLE hTemplateFile) {
  HANDLE file = RawCreateFile(lpFileName, dwDesiredAccess, dwShareMode,
                              lpSecurityAttributes, dwCreationDisposition,
                              dwFlagsAndAttributes, hTemplateFile);

  if (std::wstring(lpFileName).ends_with(L"resources.pak")) {
    resources_pak_file = file;
    resources_pak_size = GetFileSize(resources_pak_file, nullptr);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<LPVOID*>(&RawCreateFileMapping),
                 reinterpret_cast<void*>(MyCreateFileMapping));
    auto status = DetourTransactionCommit();
    if (status != NO_ERROR) {
      DebugLog(L"Hook RawCreateFileMapping failed {}", status);
    }

    // No more hook needed.
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(reinterpret_cast<LPVOID*>(&RawCreateFile),
                 reinterpret_cast<void*>(MyCreateFile));
    status = DetourTransactionCommit();
    if (status != NO_ERROR) {
      DebugLog(L"Unhook RawCreateFile failed {}", status);
    }
  }

  return file;
}

}  // namespace

void PakPatch() {
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(reinterpret_cast<LPVOID*>(&RawCreateFile),
               reinterpret_cast<void*>(MyCreateFile));
  auto status = DetourTransactionCommit();
  if (status != NO_ERROR) {
    DebugLog(L"Hook RawCreateFile failed {}", status);
  }
}

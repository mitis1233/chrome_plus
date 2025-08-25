#include "updateblocker.h"

#include <windows.h>
#include <string>

#include "detours.h"
#include "utils.h"
#include "config.h"

namespace chrome_plus {

namespace {

static decltype(&CreateProcessW) RawCreateProcessW = CreateProcessW;

BOOL WINAPI MyCreateProcessW(_In_opt_ LPCWSTR lpApplicationName,
                             _Inout_opt_ LPWSTR lpCommandLine,
                             _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
                             _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
                             _In_ BOOL bInheritHandles,
                             _In_ DWORD dwCreationFlags,
                             _In_opt_ LPVOID lpEnvironment,
                             _In_opt_ LPCWSTR lpCurrentDirectory,
                             _In_ LPSTARTUPINFOW lpStartupInfo,
                             _Out_ LPPROCESS_INFORMATION lpProcessInformation) {
  std::wstring appName = lpApplicationName ? lpApplicationName : L"";
  std::wstring cmdLine = lpCommandLine ? lpCommandLine : L"";

  // Check if the process being created is GoogleUpdate.exe
  if (appName.find(L"GoogleUpdate.exe") != std::wstring::npos ||
      cmdLine.find(L"GoogleUpdate.exe") != std::wstring::npos) {
    DebugLog(L"Blocked attempt to launch GoogleUpdate.exe: AppName=%s, CmdLine=%s", appName.c_str(), cmdLine.c_str());
    SetLastError(ERROR_ACCESS_DENIED);
    return FALSE;
  }

  return RawCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes,
                           lpThreadAttributes, bInheritHandles, dwCreationFlags,
                           lpEnvironment, lpCurrentDirectory, lpStartupInfo,
                           lpProcessInformation);
}

}  // namespace

void UpdateBlockerInit() {
  if (!config.GetDisableUpdates()) {
    DebugLog(L"Update blocking is disabled in configuration");
    return;
  }

  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(reinterpret_cast<LPVOID*>(&RawCreateProcessW),
               reinterpret_cast<void*>(MyCreateProcessW));
  auto status = DetourTransactionCommit();
  if (status != NO_ERROR) {
    DebugLog(L"Hook CreateProcessW failed: %d", status);
  } else {
    DebugLog(L"Successfully hooked CreateProcessW to block GoogleUpdate.exe");
  }
}

}  // namespace chrome_plus

#ifndef PORTABLE_H_
#define PORTABLE_H_

std::wstring QuoteSpaceIfNeeded(const std::wstring& str) {
  if (str.find(L' ') == std::wstring::npos)
    return std::move(str);

  std::wstring escaped(L"\"");
  for (auto c : str) {
    if (c == L'"')
      escaped += L'"';
    escaped += c;
  }
  escaped += L'"';
  return std::move(escaped);
}

std::wstring JoinArgsString(std::vector<std::wstring> lines,
                            const std::wstring& delimiter) {
  std::wstring text;
  bool first = true;
  for (auto& line : lines) {
    if (!first)
      text += delimiter;
    else
      first = false;
    text += QuoteSpaceIfNeeded(line);
  }
  return text;
}

// 构造新命令行
std::wstring GetCommand(LPWSTR param) {
  std::vector<std::wstring> args;

  int argc;
  LPWSTR* argv = CommandLineToArgvW(param, &argc);

  int insert_pos = 0;
  for (int i = 0; i < argc; i++) {
    if (std::wstring(argv[i]).find(L"--") != std::wstring::npos ||
        std::wstring(argv[i]).find(L"--single-argument") !=
            std::wstring::npos) {
      break;
    }
    insert_pos = i;
  }
  for (int i = 0; i < argc; i++) {
    // 保留原来参数
    if (i)
      args.push_back(argv[i]);

    // 追加参数
    if (i == insert_pos) {
      args.push_back(L"--portable");

      {
        auto userdata = GetUserDataDir();
        args.push_back(L"--user-data-dir=" + userdata);
      }

      {
        auto diskcache = GetDiskCacheDir();
        args.push_back(L"--disk-cache-dir=" + diskcache);
      }

      {
        if (IsNewTabDisableFun()) {
          args.push_back(L"--force-renderer-accessibility");
        }
      }

      // 获取命令行，然后追加参数
      // 截取拆分每个--开头的参数，然后多次 args.push_back
      // 重复上述过程，直到字串中不再存在 -- 号
      // 这样就可以保证每个参数生效
      {
        auto cr_command_line = GetCrCommandLine();
        std::wstring temp = cr_command_line;
        while (true) {
          auto pos = temp.find(L"--");
          if (pos == std::wstring::npos) {
            break;
          } else {
            auto pos2 = temp.find(L" --", pos);
            if (pos2 == std::wstring::npos) {
              args.push_back(temp);
              break;
            } else {
              args.push_back(temp.substr(pos, pos2 - pos));
              temp = temp.substr(0, pos) + temp.substr(pos2 + 1);
            }
          }
        }
      }
    }
  }
  LocalFree(argv);

  return JoinArgsString(args, L" ");
}

void Portable(LPWSTR param) {
  wchar_t path[MAX_PATH];
  ::GetModuleFileName(NULL, path, MAX_PATH);

  std::wstring args = GetCommand(param);

  SHELLEXECUTEINFO sei = {0};
  sei.cbSize = sizeof(SHELLEXECUTEINFO);
  sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
  sei.lpVerb = L"open";
  sei.lpFile = path;
  sei.nShow = SW_SHOWNORMAL;

  sei.lpParameters = args.c_str();
  if (ShellExecuteEx(&sei)) {
    ExitProcess(0);
  }
}

#endif  // PORTABLE_H_

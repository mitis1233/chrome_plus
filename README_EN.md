# Chrome++
[![LICENSE](https://img.shields.io/badge/License-GPL--3.0--only-blue.svg?style=for-the-badge&logo=github "LICENSE")](https://github.com/mitis1233/chrome_plus/blob/main/LICENSE) [![LAST COMMIT](https://img.shields.io/github/last-commit/mitis1233/chrome_plus?color=blue&logo=github&style=for-the-badge "LAST COMMIT")](https://github.com/mitis1233/chrome_plus/commits/main)  [![STARS](https://img.shields.io/github/stars/mitis1233/chrome_plus?color=brightgreen&logo=github&style=for-the-badge "STARS")](https://github.com/mitis1233/chrome_plus/stargazers) ![SIZES](https://img.shields.io/github/languages/code-size/mitis1233/chrome_plus?color=brightgreen&logo=github&style=for-the-badge "SIZES")

[正體中文](README.md) | [简体中文](README_CN.md) | [English](README_EN.md)

## Features
- Double-click to close tabs.
- Right-click to close tabs (hold Shift to show the original menu).
- Keep the last tab (prevent the browser from closing when the last tab is closed, except when clicking the close button).
- Use the mouse wheel to switch tabs when hovering over the tab bar.
- Use the mouse wheel to switch tabs while holding the right mouse button.
- Open address bar input in a new tab (configurable to open in foreground or background).
- Open bookmarks in a new tab (configurable to open in foreground or background).
- Disable the above two features when the current tab is a new tab.
- Custom shortcut key to quickly hide the browser window (boss key).
- Custom shortcut key for webpage translation.
- Portable mode (not compatible with original data, retains data after system reinstall or computer change).
- Customizable Chromium command-line switches.
- More features can be found in the [INI configuration file](src/chrome++.ini).

## Download
This project uses GitHub Actions for automatic build and release. Download from: [https://github.com/mitis1233/chrome_plus/releases](https://github.com/mitis1233/chrome_plus/releases).

## Installation
Make sure to place `version.dll` in the same directory as `chrome.exe`. It is recommended to download the [Chrome offline installer](https://github.com/mitis1233/chrome_installer), extract it twice to get the Chrome program files, and place them in the [App](https://github.com/mitis1233/chrome_plus/releases/latest) folder.

## Compatibility
* Theoretically supports all browsers based on the latest stable branch of Chromium.
* Only tested on the latest stable version of Chrome, maintenance is not guaranteed.
* If you encounter issues with DLL not loading properly, try [forcing DLL injection](https://github.com/mitis1233/setdll/).

## License
* Versions 1.5.4 and earlier use the MIT License, copyrighted by [Shuax](https://github.com/shuax/).
* Versions 1.5.5 - 1.5.9 use the MIT License, modified by contributors of this repository based on Shuax's version.
* Versions 1.6.0 and later use the [GPL-3.0 License](LICENSE).

## Acknowledgments
* All [contributors](https://github.com/mitis1233/chrome_plus/graphs/contributors)
* Original author [Shuax](https://github.com/shuax/)
* Provider of modified code for 1.5.5 [provider](https://forum.ru-board.com/topic.cgi?forum=5&topic=51073&start=620&limit=1&m=1#1)
* [面向大海](https://github.com/mxdh/)
* [Ho Cheung](https://github.com/gz83/)

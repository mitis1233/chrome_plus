# Chrome++
[![LICENSE](https://img.shields.io/badge/License-GPL--3.0--only-blue.svg?style=for-the-badge&logo=github "LICENSE")](https://github.com/Bush2021/chrome_plus/blob/main/LICENSE) [![LAST COMMIT](https://img.shields.io/github/last-commit/Bush2021/chrome_plus?color=blue&logo=github&style=for-the-badge "LAST COMMIT")](https://github.com/Bush2021/chrome_plus/commits/main)  [![STARS](https://img.shields.io/github/stars/Bush2021/chrome_plus?color=brightgreen&logo=github&style=for-the-badge "STARS")](https://github.com/Bush2021/chrome_plus/stargazers) ![SIZES](https://img.shields.io/github/languages/code-size/Bush2021/chrome_plus?color=brightgreen&logo=github&style=for-the-badge "SIZES")

[正體中文](README_TW.md) | [简体中文](README_CN.md) | [English](README.md)

## 功能
- 雙擊關閉分頁。
- 右鍵關閉分頁（按住 Shift 鍵可彈出原始選單）。
- 保留最後一個分頁（防止關閉最後一個分頁時關閉瀏覽器，點擊關閉按鈕除外）。
- 滑鼠懸停於分頁列時，使用滾輪切換分頁。
- 按住右鍵時，使用滾輪切換分頁。
- 在新分頁中開啟網址列輸入的內容（可設定於前台或背景開啟）。
- 在新分頁中開啟書籤（可設定於前台或背景開啟）。
- 當前為新分頁時，可以停用上述兩項功能。
- 自訂快捷鍵以快速隱藏瀏覽器視窗（老闆鍵）。
- 自訂快捷鍵以進行網頁翻譯。
- 便攜化（與原版資料不相容，可在重灌系統或更換電腦後保留資料）。
- 可自訂 Chromium 命令列參數。
- 更多功能請參閱 [INI 設定檔](src/chrome++.ini)。

## 取得
本專案採用 GitHub Actions 自動編譯發佈，下載位址：[https://github.com/Bush2021/chrome_plus/releases](https://github.com/Bush2021/chrome_plus/releases)。

## 安裝
請確保將 `version.dll` 放入 `chrome.exe` 的相同目錄中。建議下載 [Chrome 離線安裝包](https://github.com/Bush2021/chrome_installer)，解壓縮兩次以取得 Chrome 程式檔案，並將其放入 [App](https://github.com/Bush2021/chrome_plus/releases/latest) 資料夾即可。

## 相容性
* 理論上支援所有基於 Chromium 最新穩定分支的瀏覽器。
* 僅針對 Chrome 最新穩定版進行測試，不保證維護。
* 如果遇到 DLL 未正確載入的問題，可嘗試[強制注入 DLL](https://github.com/Bush2021/setdll/)。

## 授權條款
* 1.5.4 及更早版本使用 MIT 授權，版權所有者為 [Shuax](https://github.com/shuax/)。
* 1.5.5 - 1.5.9 版本使用 MIT 授權，由本倉儲貢獻者在 Shuax 版本上進行修改。
* 1.6.0 及之後的版本使用 [GPL-3.0 授權](LICENSE)。

## 致謝
* 所有[貢獻者](https://github.com/Bush2021/chrome_plus/graphs/contributors)
* 原作者 [Shuax](https://github.com/shuax/)
* 1.5.5 修改程式碼[提供者](https://forum.ru-board.com/topic.cgi?forum=5&topic=51073&start=620&limit=1&m=1#1)
* [面向大海](https://github.com/mxdh/)
* [Ho Cheung](https://github.com/gz83/)

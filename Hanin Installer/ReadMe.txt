解壓縮hanin_win7.zip
在setup_win7.bat檔案，點選滑鼠右鍵，用系統管理者執行
PS:在64位元的程式上漢音會沒有作用，所以安裝office或是chrome請安裝32位元的版本。

----------------------------------------------------------------------------

在Windows7/10 64-bit手動解決無法使用漢音的方法:

1. copy hanin95.ime 到
   c:\windows\system32
   c:\windows\syswow64
2. 確認 whanin.ini 在 c:\windows 的目錄下
3. 確認 hanin 應用程式和辭典在 c:\hanin 目錄下
4. 編輯 hanin.reg (存檔時, 編碼選擇unicode格式)
   
   Windows Registry Editor Version 5.00
   [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts\E0200404]
   "IME File"="hanin95.ime"
   "Layout File"="KBDUS.DLL"
   "Layout Text"="中文(繁體) - 漢音輸入法"
5. 匯入 hanin.reg (點兩下hanin.reg)
6. 到[控制台/地區與語言]中, 把漢音輸入法叫進來就可以用了
7. 或許, 要重新開機才能正常使用

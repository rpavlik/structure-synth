rmdir content
mkdir content
xcopy ..\..\Examples\*.* content\Examples\  /E
xcopy ..\..\Misc\*.* content\Misc\  /E
xcopy Microsoft.VC80.CRT\*.* content\Microsoft.VC80.CRT\  /E
copy ..\..\Release\structuresynth.exe content\  
copy c:\windows\system32\QtOpenGl4.dll content\  
copy c:\windows\system32\QtGUI4.dll content\ 
copy c:\windows\system32\QtCore4.dll content\  
copy c:\windows\system32\QtCore4.dll content\  
rem copy *.manifest content\ 
copy EULA.txt content\  
copy License.txt content\  
copy c:\windows\system32\QtCore4.dll content\  
rem copy C:\WINDOWS\WinSxS\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_6b128700\msvcr80.dll content\ 
rem copy C:\WINDOWS\WinSxS\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_6b128700\msvcp80.dll content\ 


::@echo off 
goto start
以内为注释内容
,settings会删除工程中对IAR软件的设置
:start
@for /d /r %%c in (Flash_Debug,RAM_Debug,Debug,Release) do @if exist %%c ( rd /s /q "%%c" & echo     删除目录%%c) 

@for /r  %%c in (*.tmp,*.bak ,*.dep,*.sfr,Backup*,*.tmp.c ) do del "%%c"

pause
::@echo off 
goto start
����Ϊע������
,settings��ɾ�������ж�IAR���������
:start
@for /d /r %%c in (Flash_Debug,RAM_Debug,Debug,Release) do @if exist %%c ( rd /s /q "%%c" & echo     ɾ��Ŀ¼%%c) 

@for /r  %%c in (*.tmp,*.bak ,*.dep,*.sfr,Backup*,*.tmp.c ) do del "%%c"

pause
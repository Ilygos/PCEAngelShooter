cls
@echo off

SET EXTENSION=%~x1
SET NAME=%1
SET PROJECT_NAME=%2
SET CHEMIN=%~dp0
SET PCE_INCLUDE=%CHEMIN%include\pce

If "%EXTENSION%"==".c" goto Compilc
If "%EXTENSION%"==".C" goto Compilc
:Compilasm
bin\pceas.exe %1
goto Fin

:Compilc
bin\huc.exe %1
bin\pceas.exe -s %NAME:~0,-2%.s  
del %PROJECT_NAME%.pce
ren *.pce %PROJECT_NAME%.pce
:Fin     
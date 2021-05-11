cls
@echo off

SET EXTENSION=%~x1
SET NAME=%1
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
del AngelShooter.pce
ren *.pce AngelShooter.pce
:Fin     
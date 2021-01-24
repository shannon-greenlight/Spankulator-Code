@echo off

set file_path=.\releases\spankulator_updater_%SPANK_RELEASE_NUM%.zip

echo This creates zip file: %file_path%
echo.

REM copy C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.7.0-arduino3/bossac.exe updater
copy C:\Users\Shannon\Dropbox\moog\spankulator\Spankulator-Code\out\Spankulator.ino.bin updater

"C:\Program Files\7-Zip\7z.exe" a -r .\releases\spankulator_updater_%SPANK_RELEASE_NUM%.zip updater 

pause
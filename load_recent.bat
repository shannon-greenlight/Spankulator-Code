@echo off
echo.
echo.
echo Available COM ports
echo -------------------
powershell.exe [System.IO.Ports.SerialPort]::getportnames()
echo.
echo.
set port=%SPANK_PORT%
set /p port=What's the COM port? (please enter a number or press ENTER to use com%SPANK_PORT%) 'x' to exit 

if %port% == x exit

setx SPANK_PORT %port%

@mode com%port% baud=1200
pause

.\updater\bossac.exe -d -U true -i -e -w -v -b .\out\Spankulator.ino.bin -R
pause
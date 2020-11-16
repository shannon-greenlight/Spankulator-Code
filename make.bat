@echo off

set build_path=.\out
set verbose=
rem set verbose=-verbose

rem "C:\Program Files (x86)\Arduino\arduino-builder" -dump-prefs -logger=machine -hardware "C:\Program Files (x86)\Arduino\hardware" -hardware C:\Users\Shannon\AppData\Local\Arduino15\packages -tools "C:\Program Files (x86)\Arduino\tools-builder" -tools "C:\Program Files (x86)\Arduino\hardware\tools\avr" -tools C:\Users\Shannon\AppData\Local\Arduino15\packages -built-in-libraries "C:\Program Files (x86)\Arduino\libraries" -libraries C:\Users\Shannon\Documents\Arduino\libraries -fqbn=arduino:samd:nano_33_iot -vid-pid=0X2341_0X8057 -ide-version=10810 -build-path C:\Users\Shannon\Dropbox\moog\spankulator\code\out -warnings=default -build-cache C:\Users\Shannon\AppData\Local\Temp\arduino_cache_217360 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.openocd.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\openocd\0.10.0-arduino7 -prefs=runtime.tools.openocd-0.10.0-arduino7.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\openocd\0.10.0-arduino7 -prefs=runtime.tools.CMSIS-Atmel.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS-Atmel\1.2.0 -prefs=runtime.tools.CMSIS-Atmel-1.2.0.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS-Atmel\1.2.0 -prefs=runtime.tools.arm-none-eabi-gcc.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\7-2017q4 -prefs=runtime.tools.arm-none-eabi-gcc-7-2017q4.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\7-2017q4 -prefs=runtime.tools.bossac.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.7.0-arduino3 -prefs=runtime.tools.bossac-1.7.0-arduino3.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.7.0-arduino3 -prefs=runtime.tools.arduinoOTA.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arduinoOTA\1.2.1 -prefs=runtime.tools.arduinoOTA-1.2.1.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arduinoOTA\1.2.1 -prefs=runtime.tools.CMSIS.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS\4.5.0 -prefs=runtime.tools.CMSIS-4.5.0.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS\4.5.0 -verbose C:\Users\Shannon\Dropbox\moog\spankulator\code\Spankulator\Spankulator.ino

"C:\Program Files (x86)\Arduino\arduino-builder" -compile -logger=machine -hardware "C:\Program Files (x86)\Arduino\hardware" -hardware C:\Users\Shannon\AppData\Local\Arduino15\packages -tools "C:\Program Files (x86)\Arduino\tools-builder" -tools "C:\Program Files (x86)\Arduino\hardware\tools\avr" -tools C:\Users\Shannon\AppData\Local\Arduino15\packages -built-in-libraries "C:\Program Files (x86)\Arduino\libraries" -libraries .\Spankulator\greenface_libs -libraries C:\Users\Shannon\Documents\Arduino\libraries -fqbn=arduino:samd:nano_33_iot -vid-pid=0X2341_0X8057 -ide-version=10810 -build-path %build_path% -warnings=default -build-cache C:\Users\Shannon\AppData\Local\Temp\arduino_cache_217360 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.openocd.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\openocd\0.10.0-arduino7 -prefs=runtime.tools.openocd-0.10.0-arduino7.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\openocd\0.10.0-arduino7 -prefs=runtime.tools.CMSIS-Atmel.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS-Atmel\1.2.0 -prefs=runtime.tools.CMSIS-Atmel-1.2.0.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS-Atmel\1.2.0 -prefs=runtime.tools.arm-none-eabi-gcc.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\7-2017q4 -prefs=runtime.tools.arm-none-eabi-gcc-7-2017q4.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\7-2017q4 -prefs=runtime.tools.bossac.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.7.0-arduino3 -prefs=runtime.tools.bossac-1.7.0-arduino3.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.7.0-arduino3 -prefs=runtime.tools.arduinoOTA.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arduinoOTA\1.2.1 -prefs=runtime.tools.arduinoOTA-1.2.1.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\arduinoOTA\1.2.1 -prefs=runtime.tools.CMSIS.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS\4.5.0 -prefs=runtime.tools.CMSIS-4.5.0.path=C:\Users\Shannon\AppData\Local\Arduino15\packages\arduino\tools\CMSIS\4.5.0 %verbose% Spankulator\Spankulator.ino
if errorlevel 1 (
   echo Compiler Error: %errorlevel%
   exit /b %errorlevel%
)
echo.
echo.
echo Available COM ports
echo -------------------
powershell.exe [System.IO.Ports.SerialPort]::getportnames()
echo.
echo.
echo Using: %SPANK_PORT%

set port=%SPANK_PORT%
set /p port=What's the COM port? (please enter a number or press ENTER to use com%SPANK_PORT%) 'x' to exit 

if %port% == x exit

setx SPANK_PORT %port%

@mode com%port% baud=1200
pause

.\updater\bossac.exe -d -U true -i -e -w -v -b %build_path%\Spankulator.ino.bin -R
pause
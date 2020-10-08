@echo off

echo This will compare a backup to the Spankulator folder

if defined SPANK_BACKUP_DRIVE goto ask_num

:ask
set /p drive=What's the drive letter?
setx SPANK_BACKUP_DRIVE %drive%
setx SPANK_BACKUP_NUMBER ""
goto ask_num2

:ask_num
set drive=%SPANK_BACKUP_DRIVE%

:ask_num2
if not defined SPANK_BACKUP_NUMBER ( 
	set /p backup_number=What's the backup number?
) else (
	set backup_number=%SPANK_BACKUP_NUMBER%
	set /a backup_number -= 1
)

rem "C:\Program Files\Beyond Compare 4\BComp.com" %drive%:\back\Moog\Spankulator\code\Spankulator-%backup_number%\Spankulator Spankulator
"C:\Program Files\Beyond Compare 4\BComp.com" %drive%:\back\Spankulator\Spankulator-%backup_number% .


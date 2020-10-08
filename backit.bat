@echo off

rem set backup_folder=\back\Moog\Spankulator\code\Spankulator
set backup_folder=\back\Spankulator\

echo This will backup the Spankulator folder and related libraries

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
	set /p backup_number=What's the backup number for %backup_folder%?
) else (
	set backup_number=%SPANK_BACKUP_NUMBER%
)

set backup_path=%drive%:%backup_folder%Spankulator-%backup_number%

set /p ans=That's %backup_path%, eh? (y/n)

if %ans% == n goto ask

xcopy /i /exclude:excluded_from_backup.txt Spankulator %backup_path%\Spankulator /e
xcopy /i /exclude:excluded_from_backup.txt Updater %backup_path%\updater /e

copy *.bat %backup_path%

set /A backup_number=backup_number+1

setx SPANK_BACKUP_NUMBER %backup_number%

pause
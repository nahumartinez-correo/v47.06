@echo off
goto start
rem $Name: $
rem $Revision: 1.1 $
rem $Date: 1998/10/01 22:36:57 $
rem Nightly server tasks - backup.bat 
rem ! ! ! Remember to update the version number REV 
rem backup.cmd  V1 19980116x keith.s - new - created from bu.cmd v17
                 2 19980421x keith.s - chg - to fap3
                 3 19980520x keith.s - chg - new environment variables

:start
set REV=3
rem production settings 1 = 240, 2 = 120
set Wait1=30
set Wait2=60

if not exist c:\dlld\nul mkdir c:\dlld

:Step1
echo 1. * * Nightly -%MOACURPROJ_MAINT%- server tasks - Program is backup(%REV%) * *
cmd /C stall.cmd 1
echo .. For Domain %USERDOMAIN% at Server %COMPUTERNAME% User %USERNAME%
echo .. Wait timers are set to 1 = %Wait1% and 2 = %Wait2% seconds
echo .. 1. %MOADIR_MAINT% 2. %MOAPROJ_MAINT% 3. %MOACURPROJ_MAINT% 4. %TZ%

:Step2
echo.
echo 2. * * * * * Mosaic OA ShutDown to disconnect workstations
cdinstsv -n%MOACURPROJ_MAINT% stop > c:\dlld\cdstop.out 2>&1
if errorlevel 1 echo BackUp(%REV%) ERROR: MOA Shutdown 
cmd /C saveat -c:\dlld\cdstop.out
del c:\dlld\cdstop.out

echo 2a.* * * * * Wait %Wait1% seconds for CDS shutdown  
cmd /C stall.cmd %Wait1%

:Step3
echo.
echo 3. * * * * * Backup logs  
set ntbackup flag file
if exist c:\dlld\ntbu.up del c:\dlld\ntbu.up
echo Backup started > c:\dlld\ntbu.up

if exist c:\dlld\bkup.2 del c:\dlld\bkup.2
if not exist c:\dlld\bkup.1 goto Next3a
copy c:\dlld\bkup.1 c:\dlld\bkup.2

:Next3a
if not exist c:\dlld\bkup.log goto Next3b
copy c:\dlld\bkup.log c:\dlld\bkup.1 
del c:\dlld\bkup.log

:Next3b
echo.
echo 3a.* * * * * Backup Procedure (Project files) 
ntbackup backup %MOADIR_MAINT%\%MOACURPROJ_MAINT%\ /tape:0 /t normal /l "c:\dlld\bkup.log"

echo.
echo 3b.* * * * * Backup Procedure (Append Mosaic) 
ntbackup backup %MOADIR_MAINT%\bin\ /a /tape:0 /t normal /l "c:\dlld\bkup.log"

rem unset ntbackup flag file
rem if exist c:\dlld\NTbkup.up del c:\dlld\NTbkup.up

:Step4
echo 4. * * * * * ONA Mosaic ReStart   
cdinstsv -n%MOACURPROJ_MAINT% start > c:\dlld\cdstart.out 2>&1
if errorlevel 1 echo BackUp(%REV%) ERROR: MOA Restart 
cmd /C saveat -c:\dlld\cdstart.out
del c:\dlld\cdstart.out

:Step4a
echo 4a.* * * * * Test for CDS restart   
echo.

set KLS=
:Step4b
set KLS=%KLS%*
echo 4b.* * * * * CDS restart wait loop (2 min) %KLS%
cmd /C stall.cmd 120
if "%KLS%" == "***************" goto Step4c
cdsstat -n%MOACURPROJ_MAINT% > c:\dlld\cdsstat.out 2>&1
if errorlevel 1 goto Step4b 
:Step4c

if exist c:\dlld\cdsstat.out del c:\dlld\cdsstat.out

:Step5
echo.
echo 5.  * * * * * CDS health check
cdsevent -n%MOACURPROJ_MAINT% > c:\dlld\cdsevent.out 2>&1
if errorlevel 1 echo BackUp(%REV%) ERROR: CDS health check
cmd /C saveat -c:\dlld\cdsevent.out
del c:\dlld\cdsevent.out

:Step6
echo.
echo 6. * * * * * Time stamp end of backup(%REV%)
cmd /C stall.cmd 2


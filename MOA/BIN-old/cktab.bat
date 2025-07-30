@echo off
goto start
weekly server tasks - cktab.bat 
! ! ! Remember to update the version number REV 
rem $Name: $
rem $Revision: 1.1 $
rem $Date: 1998/10/01 22:37:20 $
V1 960217x keith.s - new
V4 970406x <kls    - add - CDS restart loop
V5 970520x <kls    - chg - adjust for project
V6 970624x <kls    - add - more saveat with file name
V7 970708x <kls    - chg - c:\apps\~
V8 970924x <kls    - chg - c:\moa\bin
V9 980421x <kls    - chg - to fap3
10 980520x <kls    - chg - new environment variables
:start

set REV=10
set Wait1=30
set Wait2=60

if not exist c:\dlld\nul mkdir c:\dlld

:Step1
echo 1. * * Weekly %MOACURPROJ_MAINT% server tasks - Program is cktab(%REV%) * * 
cmd /C stall.cmd 1
echo .. For Domain %USERDOMAIN% at Server %COMPUTERNAME% User %USERNAME%
echo .. Wait1 set to %Wait1% and Wait2 set to %Wait2%
echo .. 1. %MOADIR_MAINT% 2. %MOAPROJ_MAINT% 3. %MOACURPROJ_MAINT% 4. %TZ%

:Step2
echo.
echo 2. * * * * * Mosaic OA ShutDown to disconnect workstations
cdinstsv -n%MOACURPROJ_MAINT% stop > c:\dlld\cdstop.out 2>&1
if errorlevel 1 echo cktab(%REV%) ERROR: MOA Shutdown 
cmd /C saveat -c:\dlld\cdstop.out
if exist c:\dlld\cdstop.out del c:\dlld\cdstop.out

echo 2a.* * * * * Wait %Wait2% seconds for CDS shutdown  
cmd /C stall.cmd %Wait2%

echo 2b.* * * * * Mosaic OA Restart 
cdinstsv -n%MOACURPROJ_MAINT% start > c:\dlld\cdstart.out 2>&1
if errorlevel 1 echo cktab(%REV%) ERROR: MOA Restart for table check
cmd /C saveat -c:\dlld\cdstart.out
if exist c:\dlld\cdstart.out del c:\dlld\cdstart.out

:Step2c
echo 2c.* * * * * Test for CDS restart   

set KLS=
:Again
set KLS=%KLS%*
echo 2d.* * * * * CDS restart wait loop (2 min) %KLS%
cmd /C stall.cmd 120
if "%KLS%" == "***************" goto Out
cdsstat -n%MOACURPROJ_MAINT% > c:\dlld\cdsstat.out 2>&1
if errorlevel 1 goto Again 
:Out

if exist c:\dlld\cdsstat.out del c:\dlld\cdsstat.out

:Step3
echo.
echo 3. * * * * * CDS table check
cdsstat -T -n%MOACURPROJ_MAINT% > c:\dlld\cdsstat.out 2>&1
if errorlevel 1 echo cktab(%REV%) ERROR: CDS table check

echo 3a.* * * * * Wait %Wait2% seconds for CDS table check 
cmd /C stall.cmd %Wait2%

cmd /C saveat -c:\dlld\cdsstat.out
if exist c:\dlld\cdsstat.out del c:\dlld\cdsstat.out

:Step4
echo.
echo 4. * * * * * CDS health check
cdsevent -d 1 -n%MOACURPROJ_MAINT% > c:\dlld\cdsevent.out 2>&1
if errorlevel 1 echo cktab(%REV%) ERROR: CDS health check
cmd /C saveat -c:\dlld\cdsevent.out
if exist c:\dlld\cdsevent.out del c:\dlld\cdsevent.out

echo 5. * * * * * Time stamp end of cktab(%REV%) 
cmd /C stall.cmd 2

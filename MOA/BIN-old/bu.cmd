@echo off
goto start
rem $Name: $
rem $Revision: 1.1 $
rem $Date: 1998/10/01 22:38:53 $
rem CDS nightly server tasks - bu.cmd 
rem ! ! ! Remember to update the version number REV 
rem bu.cmd V3 960503x keith.s - new
rem        V4 960611x keith.s - CDSkill before compression
rem        V5 960612x keith.s - add tran attribute table put records
rem        V5 960612x keith.s - add better error checking
rem        V6 960613x keith.s - add windows dir to path
rem        V7 960613x keith.s - add cpjnl.cmd
rem        V8 960620x keith.s - add test for c:\dlld and create
rem                           - add version number to the first echo
rem        V9 960621x keith.s - add date an time stamp at top of batch
rem       V10 960624x keith.s - add REV to error echoes
rem       V11 970221x keith.s - chg adjusted for sigb
rem       V12 970304x keith.s - chg sigb
rem       V13 970306x keith.s - chg make generic
rem       V14 970609x keith.s - add 2nd CDS stop and restart (signal 21)
rem                             chg wait timers 240/360 to 60/120
rem       V15 970624x keith.s - add move saveat.cmd with -file     
rem       V16 970708x keith.s - chg - c:\apps\~  
rem       V17 970924x keith.s - chg - c:\moa\bin  
rem                           - chg - remove backup from this file
rem       V18 980421x keith.s - chg - to fap3  
rem       V19 980520x keith.s - chg - new environment variables  

:start
set REV=19
rem production settings 1 = 30, 2 = 60
set Wait1=30
set Wait2=60

if not exist c:\dlld\nul mkdir c:\dlld

:Step1
echo 1. * * Nightly -%MOACURPROJ_MAINT%- server tasks - Program is bu(%REV%) * *
cmd /C stall.cmd 1
echo .. For Domain %USERDOMAIN% at Server %COMPUTERNAME% User %USERNAME%
echo .. Wait timers are set to 1 = %Wait1% and 2 = %Wait2% seconds
echo .. 1. %MOADIR_MAINT% 2. %MOAPROJ_MAINT% 3. %MOACURPROJ_MAINT% 4. %TZ%

:Step2
echo.
echo 2. * * * * * Mosaic OA ShutDown to disconnect workstations
cdinstsv -n%MOACURPROJ_MAINT% stop > c:\dlld\cdstop.out 2>&1
if errorlevel 1 echo bu(%REV%) ERROR: MOA Shutdown 
cmd /C saveat -c:\dlld\cdstop.out
del c:\dlld\cdstop.out

echo 2a.* * * * * Wait %Wait1% seconds for CDS shutdown  
cmd /C stall.cmd %Wait1%

:Step3
echo.
echo 3. * * * * * Copy CDS and QL directories off
cmd /C "cpjnl.cmd -x"
if errorlevel 1 echo bu(%REV%) ERROR: Copy CDS and QL

:Step4
rem echo.
rem echo 4. * * * * * Backup logs  
rem set ntbackup flag file
rem if exist c:\dlld\ntbu.up del c:\dlld\ntbu.up
rem echo Backup started > c:\dlld\ntbu.up

rem if exist c:\dlld\NTbkup.2 del c:\dlld\NTbkup.2
rem if not exist c:\dlld\NTbkup.1 goto Next4a
rem copy c:\dlld\NTbkup.1 c:\dlld\NTbkup.2
:Next4a
rem if not exist c:\dlld\NTbkup.log goto Next4
rem copy c:\dlld\NTbkup.log c:\dlld\NTbkup.1 
rem del c:\dlld\NTbkup.log
:Next4
rem if exist c:\dlld\ntbu.up del c:\dlld\ntbu.up

echo.
echo 4a.* * * * * Backup Procedure (No longer HERE) 
rem ntbackup backup c:\moaproj\%MOACURPROJ_MAINT%\ /v /d "Daily Backup" /b /hc:on /t normal /l "c:\dlld\bkup.log"
rem cmd /C c:\bkupexec\nt\bewinnt.exe /J:"Nightly Backup"

rem unset ntbackup flag file
rem if exist c:\dlld\NTbkup.up del c:\dlld\NTbkup.up

:Step5
echo.
echo 5. * * * * * Copy the CDS recovery log  
if exist c:\moaproj\%MOACURPROJ_MAINT%\cds\cds_log- del c:\moaproj\%MOACURPROJ_MAINT%\cds\cds_log-
copy c:\moaproj\%MOACURPROJ_MAINT%\cds\cds_log c:\moaproj\%MOACURPROJ_MAINT%\cds\cds_log- > c:\dlld\cdslog.out 2>&1
cmd /C saveat -c:\dlld\cdslog.out
del c:\dlld\cdslog.out

:Step6
echo 6. * * * * * ONA Mosaic ReStart   
cdinstsv -n%MOACURPROJ_MAINT% start > c:\dlld\cdstart.out 2>&1
if errorlevel 1 echo bu(%REV%) ERROR: MOA Restart 
cmd /C saveat -c:\dlld\cdstart.out
del c:\dlld\cdstart.out

:Step6a
echo 6a.* * * * * Test for CDS restart   
echo.

set KLS=
:Again
set KLS=%KLS%*
echo 6b.* * * * * CDS restart wait loop (2 min) %KLS%
cmd /C stall.cmd 120
if "%KLS%" == "***************" goto Out
cdsstat -n%MOACURPROJ_MAINT% > c:\dlld\cdsstat.out 2>&1
if errorlevel 1 goto Again 
:Out

if exist c:\dlld\cdsstat.out del c:\dlld\cdsstat.out

echo 6c.* * * * * Clear the CDS recovery log  
cdsclrlg -n%MOACURPROJ_MAINT% > c:\dlld\cdsclrlg.out 2>&1
if errorlevel 1 echo bu(%REV%) ERROR: CDS clear log 
cmd /C saveat -c:\dlld\cdsclrlg.out
if exist c:\dlld\cdsclrlg.out del c:\dlld\cdsclrlg.out

echo 6d.* * * * * Wait %Wait2% seconds for CDS to clear the log  
cmd /C stall.cmd %Wait2%

:Step7
echo 7. * * * * * Journal Compression 
cmd /C jnlpr.cmd
if errorlevel 1 echo bu(%REV%) ERROR: Journal Compression 

echo.
echo 7a.* * * * * Wait %Wait1% seconds for Compression of tables  
cmd /C stall.cmd %Wait1%
if exist c:\dlld\jnlpr.ql del c:\dlld\jnlpr.ql
if exist c:\dlld\jnlpr.cfb del c:\dlld\jnlpr.cfb

echo.
echo 7b.* * * * * Copy Journal 
cmd /C cpjnl.cmd
if errorlevel 1 echo bu(%REV%) ERROR: Copy Journal 

rem Executing ADT perl scripts
:Step8
echo.
echo 8. * * * * * Process CDS ADT files

cmd /C "putadt.cmd"
if errorlevel 1 echo bu(%REV%) ERROR: PutADT records 

echo.
echo 8a.* * * * * Wait %Wait2% seconds for ADT updates  
cmd /C stall.cmd %Wait2%

:Step9
echo.
echo 9.  * * * * * CDS health check
cdsevent -n%MOACURPROJ_MAINT% > c:\dlld\cdsevent.out 2>&1
if errorlevel 1 echo bu(%REV%) ERROR: CDS health check
cmd /C saveat -c:\dlld\cdsevent.out
del c:\dlld\cdsevent.out

:Step10
echo.
echo 10. * * * * * Time stamp end of bu(%REV%)
cmd /C stall.cmd 2

:Step11
echo.
echo 11.* * * * * Mosaic OA ShutDown to reset CDS for problem with Sig 21
cdinstsv -n%MOACURPROJ_MAINT% stop > c:\dlld\cdstop.out 2>&1
if errorlevel 1 echo bu(%REV%) ERROR: MOA Shutdown 
cmd /C saveat -c:\dlld\cdstop.out
del c:\dlld\cdstop.out

echo 11a* * * * * Wait 120 seconds for CDS shutdown  
cmd /C stall.cmd 120

:Step11b
echo 11b* * * * * ONA Mosaic ReStart   
cdinstsv -n%MOACURPROJ_MAINT% start > c:\dlld\cdstart.out 2>&1
if errorlevel 1 echo bu(%REV%) ERROR: MOA Restart 
cmd /C saveat -c:\dlld\cdstart.out
del c:\dlld\cdstart.out

:Step11c
echo 11c* * * * * Test for CDS restart   

set KLS=
:Again11
set KLS=%KLS%*
echo 11d* * * * * CDS restart wait loop (1 min) %KLS%
cmd /C stall.cmd 60
if "%KLS%" == "***************" goto Out11
cdsstat -n%MOACURPROJ_MAINT% > c:\dlld\cdsstat.out 2>&1
if errorlevel 1 goto Again11
:Out11

if exist c:\dlld\cdsstat.out del c:\dlld\cdsstat.out

:Step12
echo.
echo 12. * * * * * Time stamp end of bu(%REV%)
cmd /C stall.cmd 2


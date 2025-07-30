@echo off
goto start
rem $Name: $
rem $Revision: 1.1 $
rem $Date: 1998/10/01 22:36:15 $
rem V1 970217x keith.s - atsave.bat created 
rem V2 970708x keith.s - chg - c:\apps\~
rem V3 970924x keith.s - chg - change back to c:\moa\bin
rem V4 980421x keith.s - chg - to fap3 
rem V5 980520x keith.s - chg - new environment variables

:start

set REV=5

rem save off at1.out to at_1.log
cmd /C saveat

rem check for errors in at1.out and send notification
cmd /C ck4err
 
if exist c:\dlld\ck4err.out del c:\dlld\ck4err.out

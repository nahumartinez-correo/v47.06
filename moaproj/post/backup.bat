::backup.bat 1.0
::Sebastián Toledo, 22/10/2018

@echo off

:: make sure to change the settings from line 4-9
set dbUser=root
set dbPassword="rootAdmin125"
set backupDir="D:\BCK_Sopti"
set backupDir1="\\B1842ZACW0597\c$\BaseSopti"
::set backupDir2="\\sopti\D$\Backup_BaseSopti"
set rutalog="C:\Backup_sopti"
set mysqldump="C:\xampp\mysql\bin\mysqldump.exe"
set mysqlDataDir="C:\xampp\mysql\data"
set zip="C:\Program Files\7-Zip\7z.exe"

echo Inicio ejecución: %date%%time%> %rutalog%\log_backup.txt

:: get date
for /F "tokens=1,2,3 delims=/ " %%i in ('date /t') do (
	set dd=%%i
	set mm=%%j
	set yyyy=%%k
)

set dirName=%yyyy%%mm%%dd%
set fileSuffix=%yyyy%%mm%%dd%

:: remove echo here if you like
echo "dirName"="%dirName%"

:: switch to the "data" folder
pushd "%mysqlDataDir%"

:: create backup folder if it doesn't exist
if not exist %backupDir%\%dirName%\   mkdir %backupDir%\%dirName%
if not exist %backupDir1%\%dirName%\   mkdir %backupDir1%\%dirName%
::if not exist %backupDir2%\%dirName%\   mkdir %backupDir2%\%dirName%

:: iterate over the folder structure in the "data" folder to get the databases
for /d %%f in (*) do (
	:: remove echo here if you like
	echo processing folder "%%f"

	%mysqldump% --host="localhost" --user=%dbUser% --password=%dbPassword% --single-transaction --add-drop-table --databases %%f > %backupDir%\%dirName%\%%f.sql
	%zip% a -tgzip %backupDir%\%dirName%\%fileSuffix%_%%f.sql.gz %backupDir%\%dirName%\%%f.sql
	copy %backupDir%\%dirName%\%fileSuffix%_%%f.sql.gz %backupDir1%\%dirName%
	::copy %backupDir%\%dirName%\%fileSuffix%_%%f.sql.gz %backupDir2%\%dirName%
	del %backupDir%\%dirName%\%%f.sql
)

%zip% a %backupDir%\%dirName%\%fileSuffix%_ranashack_web.zip C:\xampp\htdocs\ranashack_web
%zip% a %backupDir%\%dirName%\%fileSuffix%_archivos_web.zip C:\xampp\htdocs\archivos_web

copy %backupDir%\%dirName%\%fileSuffix%_ranashack_web.zip %backupDir1%\%dirName%
copy %backupDir%\%dirName%\%fileSuffix%_archivos_web.zip %backupDir1%\%dirName%

copy %backupDir%\%dirName%\%fileSuffix%_ranashack_web.zip %backupDir2%\%dirName%
copy %backupDir%\%dirName%\%fileSuffix%_archivos_web.zip %backupDir2%\%dirName%

::Depuración
forfiles -p %backupDir% -d -10 -c "cmd /c IF @isdir == TRUE rd /S /Q @path"
psexec \\B1842ZACW0597 "C:\BaseSopti\depura.bat"
psexec \\sopti "D:\Backup_Basesopti\depura.bat"

popd

echo Fin ejecución:    %date%%time%>> %rutalog%\log_backup.txt
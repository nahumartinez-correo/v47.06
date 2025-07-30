# ENV_NOVTT.PL - Transmisión de Novedades de Track & Trace
# Se Agrego la Version para Poder Identificar la Version desde el Log
#$VersionScript = "7.4";
# Autor: Sebastian Cazeaux
# Modificaciones: Gabriel Falciola
# Fecha Creacion : 31/03/2000
#$VersionScript = "7.5";
# Modificaciones: German Bazante
# Fecha Modificacion : 15/12/2016
# Se comenta rutina MantenerLOG 
# Se agrego el tratamiento para separar los logs (YYYYMMDD_ENV_NOVTT.LOG - YYYYMMDD_ENV_NOVTT_OL.LOG)
$VersionScript = "7.6";
# Modificaciones: German Bazante
# Fecha Modificacion : 19/01/2017
# Se agrega funcionalidad de backup de archivos .GZ que no fueron enviados al FTP
# Se modifica tratamiento de eliminacion de archivo .VER del FTP

local($PROY) = @ARGV[0];
local($FILE_PROC_1) = @ARGV[1];
local($FILE_PROC_2) = @ARGV[2];
local($FILE_TAR) = @ARGV[3];
local($ENVIO_EXCEP) = @ARGV[4];
local($PROGRAMA) = @ARGV[5];
local($COMANDO_EXCEP) = @ARGV[6];
local($PROCESO_OL) = @ARGV[7];

if ($PROCESO_OL eq "") {
		$PROCESO_OL = "NO";
}

$NIS = substr($FILE_TAR,1,5);

#Determino en donde estoy ejecutando, para obtener el archivo INI
#$0, contiene el PATH completo del programa
$ProgramPath = substr($0,0,rindex($0,"\\")+1);

&FechayHora;

#Archivo con Configuraciones
$INI_FILE = $ProgramPath."NOVTT.ini";

#Seteo proyecto y nombre de equipo
$WORKSTA = $ENV{"COMPUTERNAME"};
if ($PROY eq "") { $PROY = "post"; }

#Si no se especifica el envío de Excepciones, por default seteo que no lo haga
if ($ENVIO_EXCEP eq "") {
	$ENVIO_EXCEP = "NO";
}

#SETEO valores default si no estan definidos en el archivo INI
$MAXLINLOG=500;
$FILE_DIR="";
$FILE_DIR_LOG="";
$LOG_FILE=$FILE_DIR_LOG."ENV_NOVTT.LOG";
$LOG_FILE_OL=$FILE_DIR_LOG."ENV_NOVTT_OL.LOG";
$DEBUG=0;
$CANTINTENTOSFTP=3;
$SLEEP_TIME=10;
$SLEEP_TIME_RECEP=80;

#Leo archivo INI con configuracion
open(INI,"<$INI_FILE");
        while ( $l = <INI>) {
            chomp($l);
            (@Campos) = split(/=/,$l);

            if (@Campos[0] eq "FILEDIR") {
                $FILE_DIR = @Campos[1];
            }
            if (@Campos[0] eq "LOGDIR") {
                $FILE_DIR_LOG = @Campos[1];
            }
            if (@Campos[0] eq "LOGFILE_ENV") {
                $LOG_FILE = @Campos[1];
            }
            if (@Campos[0] eq "MAXLINLOG") {
                $MAXLINLOG = @Campos[1];
            }
            if (@Campos[0] eq "INTENTOS_FTP") {
                $CANTINTENTOSFTP = @Campos[1];
            }
            if (@Campos[0] eq "SLEEP_TIME") {
                $SLEEP_TIME = @Campos[1];
            }
            if (@Campos[0] eq "SLEEP_TIME_RECEP") {
                $SLEEP_TIME_RECEP = @Campos[1];
            }
            if (@Campos[0] eq "DEBUG_LEVEL") {
                $DEBUG = @Campos[1];
            }
            if (@Campos[0] eq "IP_SNMP") {
                $ipsnmp = @Campos[1];
            }
            if (@Campos[0] eq "IP_UNIX") {
                $ipunix = @Campos[1];
            }
            if (@Campos[0] eq "FTP_CUE") {
                $FTP_CUE = @Campos[1];
            }
            if (@Campos[0] eq "FTP_PAS") {
                $FTP_PAS = @Campos[1];
            }
        }
close(INI);

#Si no es un proceso OL
if ($PROCESO_OL eq "NO") {
	$LOG_FILE=$FILE_DIR_LOG.$fecha."_".$LOG_FILE;
	#Directorios backup de archivos .gz
	$GZ_BCK=$FILE_DIR."\GZ_BCK";
} else {
	$LOG_FILE=$FILE_DIR_LOG.$fecha."_".$LOG_FILE_OL;
	#Directorios backup de archivos .gz
	$GZ_BCK=$FILE_DIR."\GZ_OL_BCK";
}

$FILE_DIRGZ_LOG=$FILE_DIR_LOG."dir_gz.log";

#Estos directorios deben estar fijos.
$FILE_LOCK="d:\\moaproj\\" . $PROY ."\\envio_novtt.lock";

#Verifico si existe otro proceso corriendo.
#Chequeo si el archivo de lockeo se encuentra creado, si no lo creo.
if (open (AUXFILE, "<$FILE_LOCK") != 0 ) {
	close (AUXFILE);
} else {
	if (open(AUXFILE, ">$FILE_LOCK") != 0){
		close (AUXFILE);
	} else {
    	&SendTrap("NO SE PUEDE CREAR EL ARCHIVO DE LOCKEO ($FILE_LOCK)\n");
		exit;
	}	
}
$Pruebas_lockeo = 0;
$Tiempo_Lockeo = 300;
while ($Pruebas_lockeo < 50) {
	if (open(LOCK, "+<$FILE_LOCK") != 0) {
		#Lockeo el archivo, si ya está lockeado espero ...
		if (flock(LOCK, 2 | 4) != 0) {
			last;
		} else {
			#Espero!!
			sleep $Tiempo_Lockeo;
			$Pruebas_lockeo ++;
		}
	} else {
   		&SendTrap("NO SE PUEDE ABRIR EL ARCHIVO DE LOCKEO ($FILE_LOCK)\n");
		exit;
	}
}
if ($Pruebas_lockeo == 50) {
	exit;
}

#Limpio el Archivo de Log, Dejando solo las ultimas n lineas
#&MantenerLOG;

#Testeo integridad/Permisos de Directorio de Archivos LOG
$TestFileDir = $FILE_DIR_LOG."test.aux";
if (open(AUXFILE,">$TestFileDir") != 0) {
    close(AUXFILE);
    unlink($TestFileDir);
} else { 
    PrintLog( "NO SE PUEDE ACCEDER AL DIRECTORIO DE LOGS ($FILE_DIR_LOG)\n");
    &SendTrap("NO SE PUEDE ACCEDER AL DIRECTORIO DE LOGS ($FILE_DIR_LOG)\n");
    &AbortEnvioTT(1,1);
}

PrintLog("<=========== Version del Env_NovTT : $VersionScript - Proceso OL :[$PROCESO_OL] ==========>\n");
PrintLog("<=========== Comienzo Ejecucion ($fecha_disp) ==========>\n");

#Testeo integridad/Permisos de Directorio de Archivos Auxiliares
$TestFileDir = $FILE_DIR."test.aux";
if (open(AUXFILE,">$TestFileDir") != 0) {
    close(AUXFILE);
    unlink($TestFileDir);
} else { 
    PrintLog( "NO SE PUEDE ACCEDER AL DIRECTORIO DE ARCHIVOS ($FILE_DIR)\n");
    &SendTrap("NO SE PUEDE ACCEDER AL DIRECTORIO DE ARCHIVOS ($FILE_DIR)\n");    
    &AbortEnvioTT(2,1);
}

$FILE_TAR_DIR=$FILE_DIR.$FILE_TAR;

$FILE_DUMP_CRC=$FILE_TAR.".crc";
$FILE_DUMP_CRC_DIR=$FILE_DIR.$FILE_TAR.".crc";
$FILE_CRC_LOG_DIR=$FILE_DIR_LOG.$FILE_TAR."crc.log";

$FILE_DUMP_TAR=$FILE_TAR.".tar";
$FILE_DUMP_TAR_DIR=$FILE_TAR_DIR.".tar";

$FILE_DUMP_VER=$FILE_TAR.".ver";

$FILE_COMP_AUX = $FILE_DIR.$FILE_TAR."gz_tar.bat";
$FILE_FIND_AUX = $FILE_DIR.$FILE_TAR."find.txt";
$FILE_COMP_LOG = $FILE_DIR_LOG.$FILE_TAR."gz_tar.log";

$FILE_FTP_DUMP=$FILE_DIR.$FILE_TAR."ftp.aux";
$FILE_FTP_VER=$FILE_DIR.$FILE_TAR."ver.aux";
$FILE_FTP_LOG=$FILE_DIR_LOG.$FILE_TAR."ftp.log";
$FILE_FTP_VER_LOG=$FILE_DIR_LOG.$FILE_TAR."ver.log";

#Archivos a Enviar, separados por espacio
$FILES_TAR="$NIS\p*.gz";

PrintLog("GENERACION DE ARCHIVOS DE COMANDOS.\n");
#Comandos para el FTP
open(AUXFILE,">$FILE_FTP_DUMP");

print AUXFILE "user $FTP_CUE\n$FTP_PAS\nprompt off\nbi\nmput $FILE_DUMP_TAR_DIR\nmput $FILE_DUMP_CRC_DIR\nrename $FILE_DUMP_TAR received/$FILE_DUMP_TAR\nbye\n";
close(AUXFILE);

#-----------------------------------------------------------------------------------
#---------------------- Comienzo Armado de Archivos (Compresion y Tar)--------------
#-----------------------------------------------------------------------------------
PrintLog("COMPRIMO ARCHIVOS A ENVIAR ($FILE_PROC_1 y $FILE_PROC_2).\n");
open (AUXFILE,">$FILE_COMP_AUX");
print AUXFILE "\@ECHO OFF\n";
#Si es otro Disco
if (substr($FILE_DIR,1,1) eq ":") {
    print AUXFILE substr($FILE_DIR,0,2)."\n";
}
#Cambio de Directorio
print AUXFILE "cd $FILE_DIR\n";
#Ejecuto Comandos (Compress y Tar)
#print AUXFILE "c:\\tools\\gzip -f $FILE_PROC_1 >$FILE_COMP_LOG 2>\&1 \n";
#print AUXFILE "c:\\tools\\gzip -f $FILE_PROC_2 >>$FILE_COMP_LOG 2>\&1 \n";
#print AUXFILE "c:\\tools\\tar -c -f$FILE_DUMP_TAR $FILES_TAR >$FILE_COMP_LOG 2>\&1\n";
print AUXFILE "c:\\tools\\gzip -f $FILE_PROC_1 >>$LOG_FILE 2>\&1 \n";
print AUXFILE "c:\\tools\\gzip -f $FILE_PROC_2 >>$LOG_FILE 2>\&1 \n";
close (AUXFILE);
$LRetVal = system("$FILE_COMP_AUX");
if ($retval != 0) {
	PrintLog("NO SE PUDO COMPRIMIR LA INFORMACION A ENVIAR.\n");
	&AbortEnvioTT(12,1);
}

$ENCONTRO = "N";
PrintLog("COMPRIMO ARCHIVOS PENDIENTES DE ENVIO Y ARMO TAR PARA TRANSMITIR.\n");
$NOMBRE_ARCH = $FILE_DIR . $NIS . "p*.1";
$retval = system("dir $NOMBRE_ARCH /OD /B >$FILE_FIND_AUX 2>>$LOG_FILE");
if ($retval == 0) {
	$ENCONTRO = "S";
} else {
	PrintLog("NO SE ENCONTRARON ARCHIVOS PENDIENTES CON EXTENSION 1.\n");
}
$NOMBRE_ARCH = $FILE_DIR . $NIS . "p*.2";
$retval = system("dir $NOMBRE_ARCH /OD /B >>$FILE_FIND_AUX 2>>$LOG_FILE");
if ($retval == 0) {
	$ENCONTRO = "S";
} else {
	PrintLog("NO SE ENCONTRARON ARCHIVOS PENDIENTES CON EXTENSION 2.\n");
}
if ($ENCONTRO eq "N") {
	PrintLog("NO HAY ARCHIVOS PENDIENTE DE ENVIO.\n");
}

open (AUXFILE,">$FILE_COMP_AUX");
print AUXFILE "\@ECHO OFF\n";

#Si es otro Disco
if (substr($FILE_DIR,1,1) eq ":") {
    print AUXFILE substr($FILE_DIR,0,2)."\n";
}
#Cambio de Directorio
print AUXFILE "cd $FILE_DIR\n";

if (open(AUXFILE2,"<$FILE_FIND_AUX") != 0) {
	while ($Linea = <AUXFILE2>) {
		chomp($Linea);
		#Verifico que el secuenciador no sea mayor a lo que originalmente estoy enviando, porque
		#eso quiere decir que lo está armando un proceso posterior.
		$Secuen1 = substr($Linea,6,6);
		$Secuen2 = substr($FILE_PROC_1,6,6);
		if ($Secuen1 < $Secuen2) {
			PrintLog("ARCHIVO $Linea\n");
			print AUXFILE "c:\\tools\\gzip -f $Linea >>$LOG_FILE 2>\&1 \n";
		}
	}
	print AUXFILE "c:\\tools\\tar -c -f$FILE_DUMP_TAR $FILES_TAR >>$LOG_FILE 2>\&1\n";
	close (AUXFILE);
} else {
	PrintLog("NO SE PUDO ABRIR EL ARCHIVO $FILE_FIND_AUX.\n");
	&AbortEnvioTT(12,1);
}	
$retval = system("$FILE_COMP_AUX");
if ($retval != 0) {
	PrintLog("NO SE PUDO COMPRIMIR LA INFORMACION A ENVIAR.\n");
	&AbortEnvioTT(12,1);
}

if (open (AUXFILE, "<$FILE_DUMP_TAR_DIR") <= 0) {
    PrintLog( "NO SE PUDO GENERAR ARCHIVO TAR ($FILE_DUMP_TAR_DIR). ABORTANDO TRANSFER\n");
    &SendTrap("NO SE PUDO GENERAR ARCHIVO TAR ($FILE_DUMP_TAR_DIR). ABORTANDO TRANSFER\n");
    &AbortEnvioTT(12,1);
}
close (AUXFILE);

#Define las variables con los tamaños del archivo Tar
($size_tar) = (stat($FILE_DUMP_TAR_DIR))[7];
if ($size_tar <= 0) {
    PrintLog("ERROR EN GENERACION ARCHIVO TAR ($FILE_DUMP_TAR_DIR). ABORTANDO TRANSFER\n");
    &SendTrap("ERROR EN GENERACION ARCHIVO TAR ($FILE_DUMP_TAR_DIR). ABORTANDO TRANSFER\n");
    &AbortEnvioTT(14,1);
}

PrintLog("CALCULO CRC DE ARCHIVOS A ENVIAR.\n");
#Genera el CRC del archivo .tar
system("c:\\tools\\crc32 $FILE_DUMP_TAR_DIR > $FILE_DUMP_CRC_DIR 2>$FILE_CRC_LOG_DIR");
#Define las variables con los tamaños del archivo crc
($size_crc) = (stat($FILE_DUMP_CRC_DIR))[7];
if ($size_crc <= 0) {
    PrintLog("ERROR EN GENERACION ARCHIVO CRC ($FILE_DUMP_CRC_DIR). ABORTANDO TRANSFER\n");
    &SendTrap("ERROR EN GENERACION ARCHIVO CRC ($FILE_DUMP_CRC_DIR). ABORTANDO TRANSFER\n");
    &AbortEnvioTT(16,1);
}

#Obtengo la lista de archivos que envío para después eliminarlos si todo sale bien
$NOMBRE_GZ = $FILE_DIR . $FILES_TAR;
system("dir $NOMBRE_GZ /OD /B > $FILE_DIRGZ_LOG 2>&1");

#Realizo el backup de los archivos .gz
&backupGZ;

#-----------------------------------------------------------------------------------
#---------------------- Comienzo intentos de envios via FTP ------------------------
#-----------------------------------------------------------------------------------
#Mientras no haya completado los 2 envios (tar y crc) sigo hasta
#que la cantidad de intentos sea mayor que los intentos maximos
#Si CANTINTENTOSFTP < 0, se intenta continuamente hasta que se consiga el envio
$IntentoFTP=0;
while (($ArchivosEnviadosFTP != 2) & (($IntentoFTP < $CANTINTENTOSFTP) | ($CANTINTENTOSFTP < 0))) {

    $IntentoFTP++;
    PrintLog("INICIANDO FTP DE ARCHIVOS (TAR y CRC).INTENTO ($IntentoFTP de $CANTINTENTOSFTP)\n");
    system("ftp -n -s:$FILE_FTP_DUMP $ipunix > $FILE_FTP_LOG 2>&1");

    $ArchivosEnviadosFTP = 0;
    #Chequea que la transferencia este completa
    open (AUXFILE,"<$FILE_FTP_LOG");
    while ($linea = <AUXFILE>) {
          chomp($linea);
          if ($linea eq "226 Transfer complete.") {
              (@Campos) = split(/ /,$linea_bck);
              PrintLog("FTP ".@Campos[7]." OK.\n");
              &SendTrap("FTP DEL ARCHIVO ".@Campos[7]." COMPLETO.\n");
              if ((@Campos[7] eq $FILE_DUMP_TAR.".") | (@Campos[7] eq $FILE_DUMP_CRC.".")) {
                  $ArchivosEnviadosFTP++;
              }
          }
          $linea_bck = $linea
    }
    close (AUXFILE);

    #Si se enviaron los dos archivos, traigo el archivos VER
    #Sino, reintento
    #Espero $SLEEP_TIME segundos
    if ($ArchivosEnviadosFTP != 2) {
        sleep $SLEEP_TIME;
    }

}

if ($ArchivosEnviadosFTP != 2) {
    PrintLog("FALLA EN ENVIO DE ARCHIVOS (TAR y CRC) VIA FTP.SE ABORTA TRANSMISION\n");
    &SendTrap("FALLA EN ENVIO DE ARCHIVOS (TAR y CRC) VIA FTP.SE ABORTA TRANSMISION\n");
    &AbortEnvioTT(18,0);
}

#Si se enviaron los dos archivos, traigo el archivos VER
#Mientras no haya completado el envios (ver) sigo hasta
#que la cantidad de intentos sea mayor que los intentos maximos
#Si CANTINTENTOSFTP < 0, se intenta continuamente hasta que se consiga el envio

#Espero $SLEEP_TIME_RECEP segundos
PrintLog("ESPERANDO GENERACION DE ARCHIVO DE RESPUESTA ($FILE_DUMP_VER)\n");
sleep $SLEEP_TIME_RECEP;

#Comandos para el FTP
open(AUXFILE,">$FILE_FTP_VER");
$FILE_DIR_TAR = $FILE_DIR;
chop($FILE_DIR_TAR);
#print AUXFILE "user $FTP_CUE\n$FTP_PAS\nprompt off\nbi\nlcd $FILE_DIR_TAR\nmget $FILE_DUMP_VER\ndel $FILE_DUMP_VER\nbye\n";
print AUXFILE "user $FTP_CUE\n$FTP_PAS\nprompt off\nbi\nlcd $FILE_DIR_TAR\nmget $FILE_DUMP_VER\nbye\n";
close(AUXFILE);
$FILE_VER = $FILE_DIR.$FILE_DUMP_VER;
$ArchivosEnviadosFTP = 0;
$IntentoFTP=0;
while (($ArchivosEnviadosFTP != 1) & (($IntentoFTP < $CANTINTENTOSFTP) | ($CANTINTENTOSFTP < 0))) {

    $IntentoFTP++;
    PrintLog("INICIANDO FTP DE ARCHIVO (VER).INTENTO ($IntentoFTP de $CANTINTENTOSFTP)\n");

    $ArchivosEnviadosFTP = 0;
    #Borro el archivo VER para asegurarme que despues del FTP leo el que se envio.
    unlink ($FILE_VER);

    if ($DEBUG > 0) {
       PrintLog("OBTENIENDO ARCHIVOS DE RESULTADO VIA FTP ($FILE_DUMP_VER)\n");
    }
    system("ftp -n -s:$FILE_FTP_VER $ipunix > $FILE_FTP_VER_LOG 2>&1");

    #Define las variables con los tamaños del archivo ver
    ($size_ver) = (stat($FILE_VER))[7];
    #Si el archivo esta en cero, hubo error en la TRANSMISION del VER
    if ($size_ver == 0) {
        PrintLog("ERROR EN TRANSMISION DE ARCHIVO ($FILE_VER = $size_ver bytes).\n");
        #Seteo la cantidad de archivos enviados en 0 para volver a reintentar
        #Espero
        #sleep $SLEEP_TIME;
    } else {
        #Leo Archivo VER para confirmar el envio de los archivos via FTP
        #si tiene OK, salio todo bien
        #si tiene FAILED (u otra cosa que no sea OK), hubo error y vuelvo a intentar
        open(AUXFILE,"<$FILE_VER");
        $Linea = <AUXFILE>;
        close(AUXFILE);
    
        #Saco caracteres de mas como <CR> y <LF>
        chomp($Linea);
        if ($Linea ne "OK") {
            PrintLog("ERROR EN TRANSMISION SEGUN ARCHIVO ($FILE_VER = $Linea).\n");
            &SendTrap("ERROR EN TRANSMISION SEGUN ARCHIVO ($FILE_VER = $Linea).\n");
            &AbortEnvioTT(19,0);
        } else {
            #Seteo cantidad de archivos enviados para 
            $ArchivosEnviadosFTP = 1;
            PrintLog("TRANSMISION EXITOSA ($FILE_VER = $Linea).\n");
        }
    } #if ($size_ver == 0) {

    #Si NO se pudo obtener el archivo VER archivos, 
    #espero y reintento
    if ($ArchivosEnviadosFTP != 1) {
        #Espero $SLEEP_TIME_RECEP segundos
       sleep $SLEEP_TIME_RECEP;
    }

}

if ($ArchivosEnviadosFTP != 1) {
    PrintLog("FALLA EN RECEPCION DE ARCHIVO (VER). SE ABORTA TRANSMISION\n");
    &SendTrap("FALLA EN RECEPCION DE ARCHIVO (VER). SE ABORTA TRANSMISION\n");
    &AbortEnvioTT(20,0);
} else {
		#Comandos para el FTP
		open(AUXFILE,">$FILE_FTP_VER");
		$FILE_DIR_TAR = $FILE_DIR;
		chop($FILE_DIR_TAR);
		print AUXFILE "user $FTP_CUE\n$FTP_PAS\nprompt off\ndel $FILE_DUMP_VER\nbye\n";
		close(AUXFILE);
		
		PrintLog("EJECUTANDO COMANDO PARA ELIMINACION DE ARCHIVOS FTP ($FILE_DUMP_VER)\n");
    system("ftp -n -s:$FILE_FTP_VER $ipunix > $FILE_FTP_VER_LOG 2>&1");
    PrintLog("ELIMINADOS ARCHIVOS DE RESULTADO VIA FTP ($FILE_DUMP_VER)\n");
}
#-----------------------------------------------------------------------------------
#---------------------- FIN de envios via FTP ------------------------
#-----------------------------------------------------------------------------------


# Dependiendo de qué transacción ejecutó al script lanzo el proceso de envío de
# Excepciones de T&T
if ($ENVIO_EXCEP eq "SI") {
	PrintLog("SE LANZA EL PROCESO DE ENVIO DE EXCEPCIONES DE TT.\n");
	if ($COMANDO_EXCEP ne "") {
		open(AUXFILE,"<$COMANDO_EXCEP") or $ENCONTRO = "NO";
		close(AUXFILE);
		if ($ENCONTRO ne "NO") {
			$COMANDO_EXCEP = "execute.exe " . $PROGRAMA . " " . $COMANDO_EXCEP . " " . $PROY;
			print $COMANDO_EXCEP . "\n";
			system($COMANDO_EXCEP);
		} else {
			PrintLog("NO SE ENCUENTRA EL ARCHIVO $COMANDO_EXCEP.\n");
			PrintLog("IMPOSIBLE LANZAR EL PROCESO DE EXCEPCIONES.\n");
		}
	} else {
		PrintLog("FALTA PARAMETRIA PARA EL ENVIO DE EXCEPCIONES TT\n( ITEM ScriptExcep DEL ARCHIVO DDPROJ.INI )\n");
	}
}


#FIN DEL PROGRAMA
#Borro Archivos comprimidos, para que no sean enviados nuevamente
$FILE_GZ_BAT=$FILE_DIR."del_gz.bat";
open (AUXFILE,">$FILE_GZ_BAT");
open (AUXFILE2,"<$FILE_DIRGZ_LOG");
print AUXFILE "\@ECHO OFF\n";

#Si es otro Disco
if (substr($FILE_DIR,1,1) eq ":") {
    print AUXFILE substr($FILE_DIR,0,2)."\n";
}
#Cambio de Directorio
print AUXFILE "cd $FILE_DIR\n";
#Recorro la lista de GZ que se mandan y los borro.
while ($NOMBRE_GZ = <AUXFILE2>) {
	chomp($NOMBRE_GZ);
	print AUXFILE "del $NOMBRE_GZ\n";
}
close(AUXFILE);
close(AUXFILE2);

system ($FILE_GZ_BAT);
unlink $FILE_GZ_BAT;

#Elimino archivos .gz del backup 
&eliminoGZ;

&SendTrap("ENVIO NOVEDADES TT COMPLETO.\n");
&AbortEnvioTT(0,0);
exit 0;

sub FechayHora{
    ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime(time);
    $mon=$mon+1;
    $year = $year + 1900;

    if (length($mon)<2) { $mon="0".$mon };
    if (length($mday)<2) { $mday="0".$mday };
    if (length($hour)<2) { $hour="0".$hour };
    if (length($min)<2) { $min="0".$min };
    if (length($sec)<2) { $sec="0".$sec };

    $fecha = $year.$mon.$mday;
    $fecha_disp = $mday."/".$mon."/".$year;
    $hora = $hour.$min;
    $hora_disp = $hour.":".$min.":".$sec;
}

sub MantenerLOG {
    local ($n) = 0;
    local ($MaxLin) = 0;

    if ($MAXLINLOG > 0) {

    	open (FH, "< $LOG_FILE") or return;
    	while (<FH>) { $MaxLin++; }
    	close (FH)			     or die "Closing: $!";

        if ($MaxLin > $MAXLINLOG) {
        	open (FH, "+< $LOG_FILE") or die "Opening: $!";
        	@ARRAY = <FH>;
        	seek (FH, 0, 0)			 or die "Seeking: $!";

            $n = $MaxLin - $MAXLINLOG;

            while ($n <= $MAXLINLOG) {
                print FH @ARRAY[$n] or die "Printing: $!";
                $n++;
            }
        	truncate (FH, tell (FH)) or die "Truncating: $!";
        	close (FH)			     or die "Closing: $!";
        }
    }
}

#Rutina para enviar un mensaje a la direccion TCP apuntada por la variable $ipsnmp
sub SendTrap {
    local($Mensaje) = @_;
    &FechayHora;
    local($Comando) = "c:\\tools\\sendtrap $ipsnmp -e1.4 -s3 -o1 -mo\"\" -o2 -mo\"$WORKSTA: ($fecha_disp-$hora_disp). $Mensaje.\"";
    system("$Comando > SendTrap.log 2>&1\n");
}


#Rutina para Grabar un Mensaje en el Archivo LOG
sub PrintLog {
	local($Mensaje) = @_;
	
	&FechayHora;
	
	open(LOG,">>$LOG_FILE");
	my $ofh = select LOG;
	$| = 1;
	select $ofh;
	print LOG "[$hora_disp] ".$Mensaje;
	close(LOG);
}

sub AbortEnvioTT {

   local($LRetVal) = @_[0];
   #Obtengo el nivel de error que voy a devolver al sistema operativo.
   #Si es 0, Salio todo bien
   #Si es mayor que cero, Dependiendo del numero es la gravedad del error
   local($EjecutoLimpieza) = @_[1];
   #Indicador para ejecutar limpieza si algo sale mal al teminal ejecucion
   #Si es 1, Ejecuto, 0 no Ejecuto.
   #Por Default, NO Ejecuto.


   # Si hubo Error, y la Opcion de Debug esta deshabilitada
   # Seteo el debug en 1 para que deje los LOGS
   if (($LRetVal != 0) & ($DEBUG == 0)) {
      $DEBUG = 1;
   }

   if ($LRetVal == 0) {
    #Por ahora, no se hace nada
   } else {
        if ($EjecutoLimpieza == 1) {
        } else { 
			if ($DEBUG > 1) {
                PrintLog( "  >>CODIGO DE EXIT ($LRetVal), SIN LIMPIAR TABLAS MOSAIC (NOVTT)<<\n");
            }
        }
   } #if ($LRetVal == 0) {

   #Limpio Archivos Intermedios usados en el proceso
   #De acuerdo al nivel de DEBUG, dejo o limpio los archivos

    #Archivos Auxiliares
    if ($DEBUG < 2) {
        unlink $FILE_COMP_AUX;
        unlink $FILE_FTP_DUMP;
        unlink $FILE_FTP_VER;
        unlink $FILE_FIND_AUX;
    }

    #Archivos LOG
    if ($DEBUG < 1) {
        unlink $FILE_CRC_LOG_DIR;
        unlink $FILE_COMP_LOG;
        unlink $FILE_FTP_LOG;
        unlink $FILE_FTP_VER_LOG;
				unlink $FILE_DIRGZ_LOG;
    }

		&FechayHora;
		PrintLog("<=========== FIN Ejecucion ($fecha_disp) ====================>\n");

   #Devuelvo el Numero de Error
   exit $LRetVal;
}

#Realizo el backup de los archivos .gz
sub backupGZ {
	my $files_gz = $FILE_DIR.$NIS."p*.gz";
	
	PrintLog("REALIZANDO BACKUP [$NOMBRE_GZ] EN [$GZ_BCK]\n");
	system ("copy $files_gz $GZ_BCK >> $LOG_FILE 2>&1");
	PrintLog(".....LISTO\n");
}

#Elimo archivos .gz del backup
sub eliminoGZ {
	my $files_gz = $GZ_BCK."\\".$NIS."p*.gz";

	PrintLog("ELIMINANDO ARCHIVO (S) .GZ  DE \[$GZ_BCK\]\n");
	system("del $files_gz >> $LOG_FILE 2>&1");
	PrintLog("....LISTO\n");
}	
	
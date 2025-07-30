# NOVTT.PL - Generación de Novedades de Track & Trace
# Se Agrego la Version para Poder Identificar la Version desde el Log
# Autor: Sebastian Cazeaux
#$VersionScript = "7.6";
# Modificaciones: Gabriel Falciola
# Fecha Creacion : 31/03/2000
#$VersionScript = "7.7";
# Modificaciones: German Bazante
# Fecha Modificacion : 15/05/2014
# Se agrego el tratamiento para la eliminacion de los archivos .gz
#$VersionScript = "7.8";
# Modificaciones: German Bazante
# Fecha Modificacion : 11/07/2016
# Se agrego el tratamiento para el procesamiento de piezas OnLine
#$VersionScript = "7.9";
# Modificaciones: German Bazante
# Fecha Modificacion : 16/09/2016
# Se agrego el tratamiento para separar los logs NOVTT.LOG Y NOVTT_OL.LOG
#$VersionScript = "8.0";
# Modificaciones: German Bazante
# Fecha Modificacion : 21/09/2016
# Se parametriza el tiempo que se le resta al sysdate (MIN_REV_OL en minutos)
# para que se pueda reversar una pieza con Envio OL
#$VersionScript = "8.1";
# Modificaciones: German Bazante
# Fecha Modificacion : 18/11/2016
# Se comentan rutinas MantenerLOG y MantenerLOG_OL 
# Se agrega fecha al nombre del log
#$VersionScript = "8.2";
# Modificaciones: German Bazante
# Fecha Modificacion : 15/12/2016
# Se agrega argumento $PROCESO_OL al llamado de ENV_NOVTT.pl
#$VersionScript = "8.3";
# Modificaciones: German Bazante
# Fecha Modificacion : 19/01/2017
# Se modifica rutina eliminoGZ, se agrega mas detalle de log
# Se agrega funciones para la creacion de carpetas backup GZ_BCK y GZ_OL_BCK
# Se agrega funcionalidad de reenivo de archivos .GZ que no fueron enviados al FTP
$VersionScript = "8.4";
# Modificaciones: German Bazante
# Fecha Modificacion : 20/02/2017
# Se agrega rutina para el control de archivos p*.1 y p*.2 que no fueron transmitidos

local($PROY) = @ARGV[0];
local($BLOCK) = @ARGV[1];
local($ENVIO_EXCEP) = @ARGV[2];
local($PROGRAMA) = @ARGV[3];
local($COMANDO_EXCEP) = @ARGV[4];
local($PROCESO_OL) = @ARGV[5];

if ($PROCESO_OL eq "") {
		$PROCESO_OL = "NO";
}

$FLAG_SLEEP_OL=0;

StartOL:

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
$LOG_FILE=$FILE_DIR_LOG."NOVTT.LOG";
$LOG_FILE_OL=$FILE_DIR_LOG."NOVTT_OL.LOG";

$SECUENCIADOR="NOVTT";
$DEBUG=0;
$SLEEP_OL=600;
$MIN_REV_OL=30;

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
		    if (@Campos[0] eq "LOGFILE") {
		        $LOG_FILE = @Campos[1];
		    }
		    if (@Campos[0] eq "MAXLINLOG") {
		        $MAXLINLOG = @Campos[1];
		    }
		    if (@Campos[0] eq "SECUENCIADOR") {
		        $SECUENCIADOR = @Campos[1];
		    }
		    if (@Campos[0] eq "DEBUG_LEVEL") {
		        $DEBUG = @Campos[1];
		    }
		    if (@Campos[0] eq "IP_SNMP") {
		        $ipsnmp = @Campos[1];
		    }
		    if (@Campos[0] eq "SLEEP_OL") {
		        $SLEEP_OL = @Campos[1];
		    }
		    if (@Campos[0] eq "LOGFILE_OL") {
		        $LOG_FILE_OL = @Campos[1];
		    }
		    if (@Campos[0] eq "MIN_REV_OL") {
		        $MIN_REV_OL = @Campos[1];
		    }
		}
close(INI);

#Directorios backup de archivos .gz
$GZ_BCK=$FILE_DIR."GZ_BCK";
$GZ_OL_BCK=$FILE_DIR."GZ_OL_BCK";

$LOG_FILE=$FILE_DIR_LOG.$fecha."_".$LOG_FILE;
$LOG_FILE_OL=$FILE_DIR_LOG.$fecha."_".$LOG_FILE_OL;

$FILE_SEC_AUX=$FILE_DIR."sec.aux";
$FILE_SEC_DMP=$FILE_DIR."sec.dmp";
$FILE_SEC_LOG=$FILE_DIR_LOG."sec.log";

$FILE_NIS_AUX=$FILE_DIR."nis.aux";
$FILE_NIS_DMP=$FILE_DIR."nis.dmp";
$FILE_NIS_LOG=$FILE_DIR_LOG."nis.log";

$FILE_CIERRE_AUX=$FILE_DIR."cierre.aux";
$FILE_CIERRE_DMP=$FILE_DIR."cierre.dmp";
$FILE_CIERRE_LOG=$FILE_DIR_LOG."cierre.log";

$FILE_ESCANEOS_OL_AUX=$FILE_DIR."escaneos_ol.aux";
$FILE_ESCANEOS_OL_DMP=$FILE_DIR."escaneos_ol.dmp";
$FILE_ESCANEOS_OL_LOG=$FILE_DIR_LOG."escaneos_ol.log";

$FILE_CIERRE_DIA_AUX=$FILE_DIR."cierre_dia.aux";
$FILE_CIERRE_DIA_DMP=$FILE_DIR."cierre_dia.dmp";
$FILE_CIERRE_DIA_LOG=$FILE_DIR_LOG."cierre_dia.log";

$FILE_CIERRE_SUC_AUX=$FILE_DIR."cierre_suc.aux";
$FILE_CIERRE_SUC_DMP=$FILE_DIR."cierre_suc.dmp";
$FILE_CIERRE_SUC_LOG=$FILE_DIR_LOG."cierre_suc.log";

$FILE_ENVIO_OL_AUX=$FILE_DIR."envio_ol.aux";
$FILE_ENVIO_OL_DMP=$FILE_DIR."envio_ol.dmp";
$FILE_ENVIO_OL_LOG=$FILE_DIR_LOG."envio_ol.log";

$FILE_TRANSTAT_AUX=$FILE_DIR."transtat.aux";
$FILE_TRANSTAT_DMP=$FILE_DIR."transtat.dmp";
$FILE_TRANSTAT_LOG=$FILE_DIR_LOG."transtat.log";

$FILE_MARCA_OL_AUX=$FILE_DIR."marca_ol.aux";
$FILE_MARCA_OL_DMP=$FILE_DIR."marca_ol.dmp";
$FILE_MARCA_OL_LOG=$FILE_DIR_LOG."marca_ol.log";

$FILE_SEC_OL_AUX=$FILE_DIR."sec_ol.aux";
$FILE_SEC_OL_DMP=$FILE_DIR."sec_ol.dmp";
$FILE_SEC_OL_LOG=$FILE_DIR_LOG."sec_ol.log";

$FILE_UNEG_CTP_AUX=$FILE_DIR."uneg_ctp.aux";
$FILE_UNEG_CTP_DMP=$FILE_DIR."uneg_ctp.dmp";
$FILE_UNEG_CTP_LOG=$FILE_DIR_LOG."uneg_ctp.log";

$FILE_DESP_AUX=$FILE_DIR."desp.aux";
$FILE_DESP_LOG=$FILE_DIR_LOG."desp.log";

#Estos directorios deben estar fijos.
$FILE_LOCK_LOG="d:\\moaproj\\" . $PROY . "\\novtt_lock.log";
$FILE_LOCK="d:\\moaproj\\" . $PROY ."\\novtt.lock";

#Archivo que carga los .gz a eliminar
$FILE_DIRGZ_AUX=$FILE_DIR."del_gz.aux";

#Archivo que guarda detalla error al querer eliminar un archivo
$INFO_FILE=$FILE_DIR."delete.info";

#Verifico si existe otro proceso corriendo.
#Elimino el LOG que genera si está lockeado el proceso por las dudas.
unlink ("$FILE_LOCK_LOG");
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

if (open(LOCK, "+<$FILE_LOCK") != 0) {
		#Lockeo el archivo, si ya está lockeado genero el LOG
		if (flock(LOCK, 2 | 4) != 0) {
		#if (flock(LOCK, 2) != 0) {	
		} else {
				if (open (LOCK_LOG, ">$FILE_LOCK_LOG") != 0) {
					close (LOCK_LOG);
					exit;
				} else {
		    		&SendTrap("NO SE PUEDE CREAR EL ARCHIVO LOG DE LOCKEO ($FILE_LOCK_LOG)\n");
		    		exit;			
		    }
		}
} else {
   	&SendTrap("NO SE PUEDE ABRIR EL ARCHIVO DE LOCKEO ($FILE_LOCK)\n");
		exit;
}

if ($BLOCK eq "SI") {
		exit;
}

#if ($PROCESO_OL eq "NO") {
#	#Limpio el Archivo de Log, Dejando solo las ultimas n lineas
#	&MantenerLOG;
#} else {
#	#Limpio el Archivo de Log OL, Dejando solo las ultimas n lineas
#	&MantenerLOG_OL;
#}

#Testeo integridad/Permisos de Directorio de Archivos LOG
$TestFileDir = $FILE_DIR_LOG."test.aux";
if (open(AUXFILE,">$TestFileDir") != 0) {
    close(AUXFILE);
    unlink($TestFileDir);
} else { 
    print "NO SE PUEDE ACCEDER AL DIRECTORIO DE LOGS ($FILE_DIR_LOG)\n";
    &SendTrap("NO SE PUEDE ACCEDER AL DIRECTORIO DE LOGS ($FILE_DIR_LOG)\n");
    &AbortEnvioTT(1,1);
}

if ($FLAG_SLEEP_OL eq 0) {
	PrintLog("<=========== Version del NovTT : $VersionScript - Proceso OL :[$PROCESO_OL] ==========>\n");
	PrintLog("<=========== Comienzo Ejecucion ($fecha_disp) ==========>\n");
}

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

#Checkeo estado del CDSERVER
if (&CheckStatusCDS != 0) {
    PrintLog( "CDSERVER ($PROY) FUERA DE LINEA.IMPOSIBLE REALIZAR EL PROCESO\n");
    &SendTrap("CDSERVER ($PROY) FUERA DE LINEA.IMPOSIBLE REALIZAR EL PROCESO\n");
    &AbortEnvioTT(3,0);
}
PrintLog("CDSERVER ($PROY) EN LINEA.\n");

#Obtengo NIS
$NIS = &obtengoNIS;

#Obtengo secuenciador de la tabla CodSeq
$SECUENCIADOR_FORMATEADO = &obtengoSecuenciador;

#Si no es un Proceso OL
if ($PROCESO_OL eq "NO") {
	$FILE_TAR="P".$NIS.$fecha.$hora;
} else {
	#Resto N minutos a la fecha actual
	$FECHA_HORA_REV = &subTime($MIN_REV_OL);	
	$FILE_TAR="P".$NIS.$FECHA_HORA_REV;
}

$FILE_TAR_DIR=$FILE_DIR.$FILE_TAR;

$FILE_DUMP=$FILE_TAR;
$FILE_DUMP_DIR=$FILE_DIR.$FILE_TAR;
$FILE_DUMP_LOG=$FILE_DIR_LOG.$FILE_DUMP.".log";

$FILE_DUMP_DMP=$FILE_DUMP_DIR.".dmp";
$FILE_DUMP_AUX=$FILE_DIR.$FILE_TAR.".aux";

$FILE_FIN_AUX = $FILE_TAR_DIR."fin.aux";
$FILE_FIN_LOG = $FILE_DIR_LOG.$FILE_TAR."fin.log";

$FILE_PROC=$NIS."p".$SECUENCIADOR_FORMATEADO;

$FILE_COUNT=$FILE_TAR."Count";
$FILE_COUNT_AUX=$FILE_DIR.$FILE_COUNT.".aux";
$FILE_COUNT_DMP=$FILE_DIR.$FILE_COUNT.".dmp";
$FILE_COUNT_LOG=$FILE_DIR_LOG.$FILE_COUNT.".log";

$FILE_PROC_1=$FILE_PROC.".1";
$FILE_PROC_1_DIR=$FILE_DIR.$FILE_PROC.".1";

$FILE_PROC_2=$FILE_PROC.".2";
$FILE_PROC_2_DIR=$FILE_DIR.$FILE_PROC.".2";

$FILE_FIND_AUX = $FILE_DIR."find.txt";

##Verifico directorios de backup de archivos GZ
#PrintLog("VERIFICANDO EXISTENCIA DE DIRECTORIO \[$FILE_DIR\GZ_BCK\]\n");
#if (-d $FILE_DIR."\GZ_BCK") {
#   # El directorio existe
#   PrintLog(".....EXISTE\n");
#} else {
#   # El directorio NO existe
#   PrintLog("CREANDO DIRECTORIO \[$FILE_DIR\GZ_BCK\]\n");
#   system("mkdir $FILE_DIR\GZ_BCK");
#   PrintLog(".....CREADO\n");
#}

PrintLog("GENERACION DE ARCHIVOS DE COMANDOS.\n");

#Realizo la validacion si es un Envio OnLine
if ($PROCESO_OL eq "SI") {
		#Obtengo el secuenciador para generar el header 54
		$SECUENCIADOR_OL = "ESC54";
		$SECUENCIADOR_OL_FORMATEADO = &obtengoSecuenciadorOL;
		
		#Obtengo UNEG y CTP de la sucursal
		&obtengoUNEG_CTP;
	
		#Valido que no se haya efectuado el Cierre del Dia
		$CIERRE_DIA = &CierreDia;
		if ($CIERRE_DIA eq "SI"){
				PrintLog("SE ABORTA EJECUCION DE ENVIO OL\n");
				&SendTrap("SE ABORTA EJECUCION DE ENVIO OL\n");
				&AbortEnvioTT(0,1);
		}
		
		##Valido que no se haya efectuado el Cierre de Sucursal
		#$CIERRE_SUC = &CierreSucursal;
		#if ($CIERRE_SUC eq "SI"){
		#		PrintLog("SE ABORTA EJECUCION DE ENVIO OL\n");
		#		&SendTrap("SE ABORTA EJECUCION DE ENVIO OL\n");
		#		&AbortEnvioTT(0,1);
		#}
		
		#Valido que no haya otro proceso efectuando el Envio OL
		$ENVIO_OL = &Envio_OL;
		if ($ENVIO_OL eq "SI"){
				PrintLog("SE ABORTA EJECUCION DE ENVIO OL\n");
				&SendTrap("SE ABORTA EJECUCION DE ENVIO OL\n");
				&AbortEnvioTT(0,1);
		}
		
		#Valido que la tabla TranStat no este lockeada
		$TRANSTAT_LOCKED = &tranStat(99050);
		if ($TRANSTAT_LOCKED eq "SI") {
			PrintLog("ESPERO $SLEEP_OL SEGUNDOS...\n");
			sleep($SLEEP_OL);
			$FLAG_SLEEP_OL=1;
			goto StartOL;
		}
		$TRANSTAT_LOCKED = &tranStat(9560);
		if ($TRANSTAT_LOCKED eq "SI") {
			&FechayHora;
			PrintLog("ESPERO $SLEEP_OL SEGUNDOS...\n");
			sleep($SLEEP_OL);
			goto StartOL;
		}
		$TRANSTAT_LOCKED = &tranStat(6000);
		if ($TRANSTAT_LOCKED eq "SI") {
			&FechayHora;
			PrintLog("ESPERO $SLEEP_OL SEGUNDOS...\n");
			sleep($SLEEP_OL);
			goto StartOL;
		}		
		
		#Valido que haya realizado la marca de Envio OL
		$MARCA_ENVIO_OL = &Marca_Envio_OL("X");
		if ($MARCA_ENVIO_OL eq "NO"){
				PrintLog("SE ABORTA EJECUCION DE ENVIO OL\n");
				&SendTrap("SE ABORTA EJECUCION DE ENVIO OL\n");
				&AbortEnvioTT(0,1);		
		}

		#Conteo de Registros de la Tabla NovTT, todavia no genere el Header 54
		$nCountReg = &Cant_Escaneos_OL;
		
		#Cantidad de registros del Dump de la tabla NovTT
		$nDumpReg = &Cant_Escaneos_OL_Dmp;
		
		#Agrego el HeaderOL(54) al dump
		&agregoHeaderOL;		
} else {
		#Conteo de Registros de la Tabla NovTT
		$nCountReg = &Cant_Escaneos;
		
		#Cantidad de registros del Dump de la tabla NovTT
		$nDumpReg = &Cant_Escaneos_Dmp;
}

#Obtengo la lista de archivos .gz a eliminar
&eliminoGZ;

if ($PROCESO_OL eq "SI") {
	#Creo el directiorio GZ_OL_BCK
	&createGZ_BCK($GZ_OL_BCK);
	#Verfico archivos FILE_PROC con extension p*.1 y p*.2
	&verifFileProc($GZ_OL_BCK,$LOG_FILE_OL);
	#Muevo archivos gz de la carpeta backup
	&moveGZ_BCK($GZ_OL_BCK,$LOG_FILE_OL);
} else {
	#Creo el directiorio GZ_BCK
	&createGZ_BCK($GZ_BCK);
	#Verfico archivos FILE_PROC con extension p*.1 y p*.2
	&verifFileProc($GZ_BCK,$LOG_FILE);
	#Muevo archivos gz de la carpeta backup
	&moveGZ_BCK($GZ_BCK,$LOG_FILE);
}	

#Checkeo estado del CDSERVER
if (&CheckStatusCDS != 0) {
    PrintLog("CDSERVER ($PROY) FUERA DE LINEA.ABORTANDO PROCESO\n");
    &SendTrap("CDSERVER ($PROY) FUERA DE LINEA.ABORTANDO PROCESO\n");
    &AbortEnvioTT(8,0);
}

#Checkeo por Diferencias entre cuenta de Registros en Tablas y
#Cantidad de Registros en el dump
if ($nCountReg != $nDumpReg) {
    PrintLog( "CUENTA DE REGISTROS ($nCountReg) DIFIERE CON REGISTROS EN DUMP ($nDumpReg)\n");
    &SendTrap("CUENTA DE REGISTROS ($nCountReg) DIFIERE CON REGISTROS EN DUMP ($nDumpReg)\n");
    &AbortEnvioTT(9,1);
}

#Define las variables con los tamaños del archivo dump
($size_dmp) = (stat($FILE_DUMP_DMP))[7];

if ($size_dmp eq "0 bytes") {
   	&SendTrap("EL ARCHIVO $FILE_DUMP_DUMP SE GENERO CON 0 BYTES\n");
    PrintLog("EL ARCHIVO $FILE_DUMP_DUMP SE GENERO CON 0 BYTES\n");
    &AbortEnvioTT(10,1);
}

#-----------------------------------------------------------------------------------
#---------------------- Comienzo Proceso del Dump ----------------------------------
#-----------------------------------------------------------------------------------
PrintLog("PROCESANDO $nDumpReg REGISTROS DEL ARCHIVO $FILE_DUMP_DMP.\n");
open (AUXFILE,"<$FILE_DUMP_DMP") or die "No puedo abrir archivo $FILE_DUMP_DMP\n";

#Busco Cabecera y cuento registros
undef $Cabecera;
undef $nCountRegAux1;
undef $nCountRegAux1cab;
undef $nCountRegAux2;
undef $nCountCabecera;

while ($Linea = <AUXFILE>) {
    chomp($Linea);
    undef $ArrayCampos;
    undef @ArrayCampos;
    undef %ArrayCampos;        
    (@Campos) = split(/\|/,$Linea);
    while ($campo=shift(@Campos)){
      $valor = shift(@Campos);
      $ArrayCampos{"$campo"} = "$valor";
    }

    $Tipo = $ArrayCampos{"post:adt_novtt_tipo"};

    #Si es la Cabecera
    if (&TipoRegistro($Tipo) == 0) {
        $Cabecera = $Linea;
		$nCountCabecera++;
    }
    
    #Tipo 1
    if (&TipoRegistro($Tipo) == 1) {
        $nCountRegAux1++; 
        if (($ArrayCampos{"post:adt_novtt_reenc"} ne "S") || ($ArrayCampos{"post:adt_novtt_tipo"} == 16 && $ArrayCampos{"post:adt_novtt_reenc"} eq "S" && $ArrayCampos{"post:adt_novtt_estado_ent"} != 9)) {
			if (substr($ArrayCampos{"post:adt_novtt_firma"},0,1) ne "*") {
        		$nCountRegAux1cab++;
        	}
        }	
    }
    
    #Tipo 2
    if (&TipoRegistro($Tipo) == 2) {
        $nCountRegAux2++; 
    }
    
}

  undef $ArrayCampos;
  undef @ArrayCampos;
  undef %ArrayCampos;        

  (@Campos) = split(/\|/,$Cabecera);
  while ($campo=shift(@Campos)){
      $valor = shift(@Campos);
      $ArrayCampos{"$campo"} = "$valor";
  }

  #Armo la Cabecera para Cada archivo. Repetir para cada nueva cabecera (archivo)
  #que haga falta
  #Relleno con ceros adelante;
  #Lo hago asi, porque en la V4 de Perl no funciona bien el sprintf
  $Ceros = substr("000000",0,6 - length($nCountRegAux1cab));
  $sCountReg = $Ceros.$nCountRegAux1cab;
  $ArrayCampos{"post:adt_novtt_cant"} = $sCountReg;
        
  $RegCabecera1 = $ArrayCampos{"post:adt_novtt_tipo"}.
                  $ArrayCampos{"post:adt_novtt_uneg"}.
                  $ArrayCampos{"post:adt_novtt_ctp"}.
                  $ArrayCampos{"post:adt_novtt_ws"}.
                  $ArrayCampos{"post:adt_novtt_cant"}.
                  $ArrayCampos{"post:adt_novtt_fecha"}."\n";

  $Ceros = substr("000000",0,6 - length($nCountRegAux2));
  $sCountReg = $Ceros.$nCountRegAux2;
  $ArrayCampos{"post:adt_novtt_cant"} = $sCountReg;
  $FechaCabe2    = substr($ArrayCampos{"post:adt_novtt_fecha"},0,8);
  $HoraCabe      = substr($ArrayCampos{"post:adt_novtt_fecha"},8,2);
  $MinutoCabe    = substr($ArrayCampos{"post:adt_novtt_fecha"},10,2);
  $SegundoCabe   = substr($ArrayCampos{"post:adt_novtt_fecha"},12,2);
  #Pedido por Nina por problemas que tenian en la interfaz
  &AumentoUnSegundo();
  $HoraCabe2 =  $HoraCabe . $MinutoCabe . $SegundoCabe;
  $RegCabecera2 = $ArrayCampos{"post:adt_novtt_tipo"}.
                  $ArrayCampos{"post:adt_novtt_uneg"}.
                  $ArrayCampos{"post:adt_novtt_ctp"}.
                  $ArrayCampos{"post:adt_novtt_ws"}.
                  $ArrayCampos{"post:adt_novtt_cant"}.
                  $FechaCabe2 . $HoraCabe2."\n";

close (AUXFILE);

if (!(defined $Cabecera)) {
    PrintLog("NO SE PUDO ENCONTRAR REGISTRO CABECERA. ABORTANDO TRANSMISION\n");
    &SendTrap("NO SE PUDO ENCONTRAR REGISTRO CABECERA. ABORTANDO TRANSMISION\n");
    &AbortEnvioTT(11,1);
}

undef $ProcLinea;
open (PROCFILE1,">$FILE_PROC_1_DIR");
binmode(PROCFILE1);

open (PROCFILE2,">$FILE_PROC_2_DIR");
binmode(PROCFILE2);

    #Grabo Registros Cabecera
    print PROCFILE1 $RegCabecera1;
    print PROCFILE2 $RegCabecera2;    

    open (AUXFILE,"<$FILE_DUMP_DMP");
    #Busco Cabecera
    while ($Linea = <AUXFILE>) {
        chomp($Linea);
        (@Campos) = split(/\|/,$Linea);
        while ($campo=shift(@Campos)){
          $valor = shift(@Campos);
          $ArrayCampos{"$campo"} = "$valor";
        }

        undef $ProcLinea;
        $Tipo = $ArrayCampos{"post:adt_novtt_tipo"};
        if ($Tipo == 1) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_pieza"}.
                         $ArrayCampos{"post:adt_novtt_caratula"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        if ($Tipo == 2) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_caratula"}.
                         $ArrayCampos{"post:adt_novtt_prodcont"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        if ($Tipo == 3) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.            
                         $ArrayCampos{"post:adt_novtt_caratula"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        if ($Tipo == 9) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.            
                         $ArrayCampos{"post:adt_novtt_pieza"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        if ($Tipo == 10) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_transp"}.
                         $ArrayCampos{"post:adt_novtt_sal_lleg"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        if ($Tipo == 11) {
            $Item = $ArrayCampos{"post:adt_novtt_item"};
            if ($Item eq "I") { 
                $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                             $ArrayCampos{"post:adt_novtt_subtipo"}.
                             $ArrayCampos{"post:adt_novtt_item"}.
                             $ArrayCampos{"post:adt_novtt_fecha"}.
                             $ArrayCampos{"post:adt_novtt_pieza"}.
                             " ".
                             $ArrayCampos{"post:adt_novtt_transf"}.
                             $ArrayCampos{"post:adt_novtt_optt"}."\n";
            }
            if ($Item eq "C") { 
                $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                             $ArrayCampos{"post:adt_novtt_subtipo"}.
                             $ArrayCampos{"post:adt_novtt_item"}.
                             $ArrayCampos{"post:adt_novtt_fecha"}.
                             $ArrayCampos{"post:adt_novtt_caratula"}.
                             $ArrayCampos{"post:adt_novtt_transf"}.
                             $ArrayCampos{"post:adt_novtt_optt"}."\n";
            }

        }

        if ($Tipo == 12) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_pieza"}.                         
                         $ArrayCampos{"post:adt_novtt_caratula"}.
						 $ArrayCampos{"post:adt_novtt_resultado"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        if ($Tipo == 15) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_subtipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_uneg"}.
                         $ArrayCampos{"post:adt_novtt_ruta"}.
                         $ArrayCampos{"post:adt_novtt_cliente"}.
                         #5 Espacios Fijos.
                         "     " .
                         $ArrayCampos{"post:adt_novtt_sap"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_pieza"}.
                         $ArrayCampos{"post:adt_novtt_promesa"}.
                         $ArrayCampos{"post:adt_novtt_opcional"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";

        }

        if ($Tipo == 16 && (($ArrayCampos{"post:adt_novtt_reenc"} ne "S") || ($ArrayCampos{"post:adt_novtt_reenc"} eq "S") && ($ArrayCampos{"post:adt_novtt_estado_ent"} != 9))) {
        	if (substr($ArrayCampos{"post:adt_novtt_firma"},0,1) ne "*") {
            	$ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
               		         $ArrayCampos{"post:adt_novtt_subtipo"}.
                   		     $ArrayCampos{"post:adt_novtt_fecha_obs"}.
                       		 $ArrayCampos{"post:adt_novtt_uneg"}.
	                       	 $ArrayCampos{"post:adt_novtt_ruta"}.
    	                   	 $ArrayCampos{"post:adt_novtt_estado_ent"}.
        	               	 $ArrayCampos{"post:adt_novtt_firma"}.
            	           	 $ArrayCampos{"post:adt_novtt_fecha"}.
                	       	 $ArrayCampos{"post:adt_novtt_pieza"}.
                    	   	 $ArrayCampos{"post:adt_novtt_optt"}."\n";
           }
        }

        if ($Tipo == 24) {
            $ProcLinea = $ArrayCampos{"post:adt_novtt_tipo"}.
                         $ArrayCampos{"post:adt_novtt_fecha"}.
                         $ArrayCampos{"post:adt_novtt_caratula"}.
                         $ArrayCampos{"post:adt_novtt_transp"}.
                         $ArrayCampos{"post:adt_novtt_peso"}.
                         $ArrayCampos{"post:adt_novtt_optt"}."\n";
        }

        #Si es la cabecera, no proceso
        if (&TipoRegistro($Tipo) == 0) {
            $ProcLinea = "";
        }

        if (!(defined $ProcLinea)) {
           if ($Tipo != 16) {
                PrintLog("REGISTRO INVALIDO TIPO ($Tipo) EN LINEA $. ($Linea)\n");
           }
        } else {
            if (&TipoRegistro($Tipo) == 1) {
                print PROCFILE1 $ProcLinea;
            }
            if (&TipoRegistro($Tipo) == 2) {
                print PROCFILE2 $ProcLinea;
            }
        } #if (!(defined $ProcLinea)) 

    }
    close (AUXFILE);

close (PROCFILE1);
close (PROCFILE2);

#Checkeo archivo Cantidad de Registros Procesados
#La cantidad Total de Registros Procesados debe ser
#La suma de los registros de cada archivo - 1 (por la cabecera)
$nProcReg = ($nCountRegAux1 + $nCountRegAux2) + $nCountCabecera;

if ($nProcReg != $nDumpReg) {
    PrintLog( "REGISTROS PROCESADOS ($nProcReg) DIFIERE CON REGISTROS EN DUMP ($nDumpReg)\n");
    &SendTrap("REGISTROS PROCESADOS ($nProcReg) DIFIERE CON REGISTROS EN DUMP ($nDumpReg)\n");
    &AbortEnvioTT(12,1);
}


#Le sumo 0 a la variable, para que la defina y aparezca en la impresion con valor,
#sino aparecian vacia.
$nCountRegAux1cab = $nCountRegAux1cab + 0;
$nCountRegAux2 = $nCountRegAux2 + 0;
$nCountCabecera = $nCountCabecera+ 0; 

#Si los archivos no tienen registros, los borro
if ($nCountRegAux1cab == 0) {
    unlink ($FILE_PROC_1_DIR);
}

if ($nCountRegAux2 == 0) {
    unlink ($FILE_PROC_2_DIR);
}

PrintLog("  $nCountRegAux1cab registros para el archivo $FILE_PROC_1.\n");
PrintLog("  $nCountRegAux2 registros para el archivo $FILE_PROC_2.\n");

if (($nCountRegAux1cab == 0) & ($nCountRegAux2 == 0)) {
    PrintLog("NO HAY REGISTROS PARA ENVIAR\n");
    &SendTrap("NO HAY REGISTROS PARA ENVIAR\n");
    &AbortEnvioTT(13,1);
}

#Checkeo estado del CDSERVER
if (&CheckStatusCDS != 0) {
    PrintLog("CDSERVER ($PROY) FUERA DE LINEA. NO SE PUEDE ACTUALIZAR BASE DE DATOS MOSAIC\n");
    &SendTrap("CDSERVER ($PROY) FUERA DE LINEA. NO SE PUEDE ACTUALIZAR BASE DE DATOS MOSAIC\n");
    &AbortEnvioTT(17,0);
}

PrintLog("CHECKEO ESTADO DE LA SUCURSAL.\n");
open(AUXFILE,">$FILE_CIERRE_AUX");
print AUXFILE "select siaf:adt_sucur_cierre_dia from sucur_act dump into $FILE_CIERRE_DMP;\n";
close(AUXFILE);
system("ql -Smain -n$PROY < $FILE_CIERRE_AUX > $FILE_CIERRE_LOG 2>&1");

open(AUXFILE,"<$FILE_CIERRE_DMP");
    #Leo Encabezado CDS_FIXED_FORMAT
    $Linea = <AUXFILE>;
    #Leo Encabezado (Nombre del campo)
    $Linea = <AUXFILE>;
    #Leo Valor
    $Linea = <AUXFILE>;
    chomp $Linea;
    if ($Linea eq "S") {
        $CIERRESUC = "S";
    } else {
        $CIERRESUC = "N";
    }
close(AUXFILE);
PrintLog("CIERRE = $CIERRESUC.\n");

PrintLog("ACTUALIZANDO BASES MOSAIC PARA REFLEJAR GENERACION EXITOSA DE NOVEDADES TT\n");
if ($PROCESO_OL eq "NO") {
		open(AUXFILE,">$FILE_FIN_AUX");
		print AUXFILE "update CodSeq set post:adt_codseq_nro = $SECUENCIADOR_NRO where post:adt_codseq_cod = \"$SECUENCIADOR\";\n";
		print AUXFILE "delete from NovTT where post:adt_novtt_listo = \"S\" && post:adt_novtt_tipo != 15 && post:adt_novtt_tipo != 16 && post:adt_novtt_tipo != 24 && post:adt_novtt_tipo != 10;\n";
		print AUXFILE "update NovTT set post:adt_novtt_trans = \"T\" where post:adt_novtt_trans != \"T\" && post:adt_novtt_listo = \"S\" && ( post:adt_novtt_tipo == 15 || post:adt_novtt_tipo == 24 || post:adt_novtt_tipo == 10);\n";
		print AUXFILE "update NovTT set post:adt_novtt_trans = \"T\" where post:adt_novtt_trans != \"T\" && post:adt_novtt_listo = \"S\" && post:adt_novtt_tipo == 16 && post:adt_novtt_firma != \"*Interdicta\";\n";
		print AUXFILE "update NovTT set post:adt_novtt_listo = \"N\" where post:adt_novtt_tipo == 16 && post:adt_novtt_firma == \"*Interdicta\";\n";
		close(AUXFILE);
} else {
	#Cuando es un proceso de Envio OL
	#El archivo $FILE_FIN_AUX es creado en la rutina Cant_Escaneos_OL_Dmp
}
system("ql -Smain -n$PROY < $FILE_FIN_AUX > $FILE_FIN_LOG 2>&1");

# Lanzo el proceso que comprime y envía por FTP las Novedades
PrintLog("SE LANZA EL PROCESO DE COMPRESION Y ENVIO DE NOVEDADES DE TT.\n");
#Obtengo el Path del script ENV_NOVTT.PL
&ActDDproj;
if ($PATH_ENVIO_NOVTT ne "") {
	($Programa,$Archivo) = split(/ /,$PATH_ENVIO_NOVTT);
	open(AUXFILE,"<$Archivo") or $ENCONTRO = "NO";
	close(AUXFILE);
	if ($ENCONTRO ne "NO") {
		$Comando = "execute.exe " . $PATH_ENVIO_NOVTT . " " . $PROY . " " . $FILE_PROC_1 . " " . $FILE_PROC_2 . " " . $FILE_TAR;
		if ($ENVIO_EXCEP eq "SI") {
			$Comando = $Comando . " " . $ENVIO_EXCEP . " " . $PROGRAMA . " " . $COMANDO_EXCEP;
		} else {
			$Comando = $Comando . " " . $ENVIO_EXCEP . " " . "NO" . " " . "NO";
		}
		$Comando = $Comando . " " . $PROCESO_OL;
		
		PrintLog("\[$Comando\]\n");
		system($Comando);
	} else {
		PrintLog("NO SE ENCUENTRA EL ARCHIVO $PATH_ENVIO_NOVTT.\n");
		PrintLog("IMPOSIBLE LANZAR EL PROCESO DE COMPRESION Y ENVIO DE NOVEDADES DE TT.\n");
	}
} else {
	PrintLog("FALTA PARAMETRIA PARA EL PROCESO DE COMPRESION Y ENVIO DE NOVEDADES DE TT\n( ITEM Envio_NovTT DEL ARCHIVO DDPROJ.INI )\n");
}

#FIN DEL PROGRAMA
&SendTrap("GENERACION DE NOVEDADES TT COMPLETO.\n");
&AbortEnvioTT(0,0);
exit 0;

#Esta rutina me dice que tipo de registro es (En que archivo va)
#recibe como parametro el tipo de registro
#devuelve: 0, si es la cabecera
#          1, si es de tipo 1
#          2, si es de tipo 2

sub TipoRegistro {
    local($TipoReg) = @_[0];
    
    if (($Tipo == 1) | ($Tipo == 2) | ($Tipo == 3) | ($Tipo == 9) | ($Tipo == 10) |
        ($Tipo == 11) | ($Tipo == 12) | ($Tipo == 24)) {
        return (2);            
    }
    if (($TipoReg == 15) | ($TipoReg == 16)) {
        return (1);
    }
    if ($Tipo == 54) {
        return (0);
    }

}

#Por ahora se usa esta rutina para aumentar un segundo
sub AumentoUnSegundo{
local($seg) = $HoraCabe * 3600 + $MinutoCabe * 60 + $SegundoCabe + 1;
local($tiempo) = time;
local($Segundo,$Minuto,$Hora) = localtime($tiempo);
local($t) = $tiempo - ( $Segundo + 60 * $Minuto + 3600 * $Hora)+ $seg;
($SegundoCabe,$MinutoCabe,$HoraCabe) = localtime($t);
if (length($HoraCabe)<2) { $HoraCabe="0".$HoraCabe };
if (length($MinutoCabe)<2) { $MinutoCabe="0".$MinutoCabe };
if (length($SegundoCabe)<2) {$SegundoCabe="0".$SegundoCabe };
#$SegundoCabe++; 
#if ($SegundoCabe > 59 ){
#	$SegundoCabe=0;
#	$MinutoCabe++; 
#	if ($MinutoCabe > 59 ){
#		$HoraCabe++;
#		$MinutoCabe=0;
#	   if ($HoraCabe > 23 ){
# 			$HoraCabe=0;
#		}
#	}
}

#Calcula Fecha y Hora
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
    $hora = $hour.$min.$sec;
    $hora_disp = $hour.":".$min.":".$sec;
}

#Rutina para mantener el log OL
sub MantenerLOG_OL {
    local ($n) = 0;
    local ($MaxLin) = 0;

    if ($MAXLINLOG > 0) {
	    	open (FH, "< $LOG_FILE_OL") or return;
	    	while (<FH>) { $MaxLin++; }
	    	close (FH)			     or die "Closing: $!";
	
	      if ($MaxLin > $MAXLINLOG) {
	      	open (FH, "+< $LOG_FILE_OL") or die "Opening: $!";
	      	@ARRAY = <FH>;
	      	seek (FH, 0, 0)			 or die "Seeking: $!";
	
	        $n = $MaxLin - $MAXLINLOG;
	        
	      	while ($n <= $MaxLin) {
	            print FH @ARRAY[$n] or die "Printing: $!";
	            $n++;
	        }
	      	truncate (FH, tell (FH)) or die "Truncating: $!";
	      	close (FH)			     or die "Closing: $!";
	      }
    }
}

#Rutina para mantener el log
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

        	while ($n <= $MaxLin) {
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

#Rutina para checkear por el estado del CDSSERVER
#Devuelve 0, si el CDSSERVER esta funcionando
sub CheckStatusCDS {
    local($LRetVal);
    local($CDSLOG) = $FILE_DIR_LOG."cdserver.log";
    
    $LRetVal = system("cdsstat -Smain -n$PROY > $CDSLOG 2>&1");
    if ($DEBUG < 1) {
        unlink($CDSLOG);
    }
    return($LRetVal);
}

#Rutina para Grabar un Mensaje en el Archivo LOG
sub PrintLog {
	local($Mensaje) = @_;
	
	&FechayHora;
	
	if ($PROCESO_OL eq "NO") {
		open(LOG,">>$LOG_FILE");
	} else {
		open(LOG,">>$LOG_FILE_OL");
	}
	
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
            # Modifico en tabla de DespPend los registros que tienen >> por --
            #Checkeo estado del CDSERVER
            if (&CheckStatusCDS != 0) {
                PrintLog( "CDSERVER ($PROY) FUERA DE LINEA.IMPOSIBLE ACTUALIZAR TABLAS PARA REFLEJAR ENVIO FALLIDO DE NOVEDADES TT\n");
            } else {
                PrintLog( "ACTUALIZANDO TABLAS PARA REFLEJAR ENVIO FALLIDO DE NOVEDADES TT\n");
                open(AUXFILE,">$FILE_DESP_AUX");
                #print AUXFILE "update DespPend set post:adt_desppend_marca = \"--\" where post:adt_desppend_marca = \">>\";\n";
                #print AUXFILE "update NovTT set post:adt_novtt_listo = \"N\" where post:adt_novtt_listo = \"S\";\n";
                #print AUXFILE "delete from DespPend where post:adt_desppend_marca == \">>\";\n";
                #print AUXFILE "delete from DespPend where post:adt_desppend_fecha = 99999999;\n";
 								#print AUXFILE "delete from DespPend where post:adt_desppend_fecha = 88888888;\n";
								print AUXFILE "delete from NovTT where (post:adt_novtt_tipo == 15 or post:adt_novtt_tipo == 16 or post:adt_novtt_tipo == 24) and post:adt_novtt_trans == \"T\" and post:adt_novtt_linea > 0;\n";
                close(AUXFILE);
                system("ql -Smain -n$PROY < $FILE_DESP_AUX > $FILE_DESP_LOG 2>&1");
                if ($DEBUG > 1) {
                    PrintLog( "  >>CODIGO DE EXIT ($LRetVal), LIMPIANDO TABLAS MOSAIC (NOVTT)<<\n");
                }
            } #if (&CheckStatusCDS != 0) {
        } else { #if ($EjecutoLimpieza == 1) {
            if ($DEBUG > 1) {
                PrintLog( "  >>CODIGO DE EXIT ($LRetVal), SIN LIMPIAR TABLAS MOSAIC (NOVTT)<<\n");
            }
        }
   } #if ($LRetVal == 0) {

   #Limpio Archivos Intermedios usados en el proceso
   #De acuerdo al nivel de DEBUG, dejo o limpio los archivos

   #Archivos DUMP
    if ($DEBUG < 3) {
        unlink $FILE_CIERRE_DMP;
        unlink $FILE_DUMP_DMP;
        unlink $FILE_NIS_DMP;
        unlink $FILE_SEC_DMP;
        unlink $FILE_SEC_OL_DMP;
        unlink $FILE_COUNT_DMP;
				unlink $FILE_ESCANEOS_OL_DMP;
				unlink $FILE_CIERRE_DIA_DMP;
				unlink $FILE_CIERRE_SUC_DMP;
				unlink $FILE_ENVIO_OL_DMP;
				unlink $FILE_UNEG_CTP_DMP;
				unlink $FILE_TRANSTAT_DMP;
				unlink $FILE_MARCA_OL_DMP;
    }

    #Archivos Auxiliares
    if ($DEBUG < 2) {
				unlink $FILE_CIERRE_AUX;        
				unlink $FILE_NIS_AUX;
				unlink $FILE_SEC_AUX;
				unlink $FILE_SEC_OL_AUX;
				unlink $FILE_DUMP_AUX;
				unlink $FILE_COUNT_AUX;
				unlink $FILE_FIN_AUX;
				unlink $FILE_DESP_AUX;
				unlink $FILE_ESCANEOS_OL_AUX;
				unlink $FILE_CIERRE_DIA_AUX;
				unlink $FILE_CIERRE_SUC_AUX;
				unlink $FILE_ENVIO_OL_AUX;
				unlink $FILE_UNEG_CTP_AUX;
				unlink $FILE_TRANSTAT_AUX;
				unlink $FILE_MARCA_OL_AUX;
    }

    #Archivos LOG
    if ($DEBUG < 1) {
    		unlink $FILE_CIERRE_LOG;  
    		unlink $FILE_NIS_LOG;
        unlink $FILE_SEC_LOG;
        unlink $FILE_SEC_OL_LOG;
        unlink $FILE_DUMP_LOG;
        unlink $FILE_COUNT_LOG;
        unlink $FILE_FIN_LOG;
        unlink $FILE_DESP_LOG;
				unlink $FILE_ESCANEOS_OL_LOG;
				unlink $FILE_CIERRE_DIA_LOG;
				unlink $FILE_CIERRE_SUC_LOG;
				unlink $FILE_ENVIO_OL_LOG;
				unlink $FILE_UNEG_CTP_LOG;
				unlink $FILE_TRANSTAT_LOG;
				unlink $FILE_MARCA_OL_LOG;
    }

		#Si es un proceso de Envio OL, limpio la marca en la sucur_act
		#if ($PROCESO_OL eq "SI") {
				$MARCA_ENVIO_OL = &Marca_Envio_OL("");
		#}	
		
		&FechayHora;
		PrintLog("<=========== FIN Ejecucion ($fecha_disp) ====================>\n");
		
		#Devuelvo el Numero de Error
		exit $LRetVal;
}

#Rutina que busca el path del script ENV_NOTT.pl en el archivo DDproj.ini
##########################################################################################
sub ActDDproj {
	my ($Archivo,$SeccionProject,$linea_ori,$Termino);

	$Archivo = "d:\\moaproj\\" . $PROY . "\\DDproj.ini";
	open (ORIG, "<$Archivo") || PrintLog ("ERROR (No puedo abrir el archivo $Archivo)\n");
	$SeccionProject = "N";
	while ($linea_ori = <ORIG>) {
		chomp($linea_ori);
		#$linea_ori =~ s/[ ]*//og;
		if ($linea_ori =~ /\[project\]/i) {
			$SeccionProject = "S";
		}
		if ($SeccionProject eq "S") {
			(@Campos) = split(/=/,$linea_ori);
			if (@Campos[0] =~ /Envio_NovTT/i) {
				$PATH_ENVIO_NOVTT = @Campos[1];
				last;
			}
		}
	}
	close (ORIG);
	if ($PATH_ENVIO_NOVTT =~ /perl/i) {
	} else {
		$PATH_ENVIO_NOVTT = "perl " . $PATH_ENVIO_NOVTT;
	}
}

sub obtengoNIS {
		PrintLog("OBTENGO NIS DE LA SUCURSAL.\n");
		open(AUXFILE,">$FILE_NIS_AUX");
		print AUXFILE "select siaf:adt_sucur_codigo from sucur_act dump into $FILE_NIS_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_NIS_AUX > $FILE_NIS_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_NIS_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    if ($Linea eq "") {
		        PrintLog( "NO SE PUDO OBTENER NIS DE LA SUCURSAL ($WORKSTA).\n");
		        &SendTrap("NO SE PUDO OBTENER NIS DE LA SUCURSAL ($WORKSTA).\n");
		        &AbortEnvioTT(4,1);
		    }
		    $NIS = $Linea;
		close(AUXFILE);
		PrintLog("SERVIDOR:($WORKSTA), SUCURSAL:($NIS)\n");	
		return $NIS;
}

sub obtengoSecuenciador {
		PrintLog("OBTENGO SECUENCIADOR ($SECUENCIADOR).\n");
		open(AUXFILE,">$FILE_SEC_AUX");
		print AUXFILE "select post:adt_codseq_nro from CodSeq where post:adt_codseq_cod = \"$SECUENCIADOR\" dump into $FILE_SEC_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_SEC_AUX > $FILE_SEC_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_SEC_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    if ($Linea eq "") {
		        PrintLog( "SECUENCIADOR ($SECUENCIADOR) NO EXISTE.\n");
		        &SendTrap("SECUENCIADOR ($SECUENCIADOR) NO EXISTE.\n");
		        &AbortEnvioTT(5,1);
		    }
		    #Aumento en 1. Esto tendra efecto (se grabara en MOSAIC) recien cuando se halla completado
		    #el FTP.
		    $SECUENCIADOR_NRO = $Linea + 1;
		    #Relleno con ceros adelante;
		    #Lo hago asi, porque en la V4 de Perl no funciona bien el sprintf
		    $Ceros = substr("000000",0,6 - length($SECUENCIADOR_NRO));
		    $SECUENCIADOR_FORMATEADO = $Ceros.$SECUENCIADOR_NRO;
		
		close(AUXFILE);
		PrintLog("SECUENCIADOR ($SECUENCIADOR) = $SECUENCIADOR_NRO\n");	
		return $SECUENCIADOR_FORMATEADO;
}

#Devuelve la cantidad de registros a procesar de la tabla NovTT que no son OL
sub Cant_Escaneos {
		$nCountReg = 0;
		#Comandos para el Conteo de Registros de la Tabla NovTT
		open(AUXFILE,">$FILE_COUNT_AUX");
		print AUXFILE "select count(post:adt_novtt_op) from NovTT where post:adt_novtt_listo = \"S\" && post:adt_novtt_trans != \"T\" dump into $FILE_COUNT_DMP;\n";
		close(AUXFILE);
		
		PrintLog("CUENTO CANTIDAD DE REGISTROS A ENVIAR....\n");
		system("ql -Smain -n$PROY < $FILE_COUNT_AUX > $FILE_COUNT_LOG 2>&1");
		open(AUXFILE,"<$FILE_COUNT_DMP");
		$trash=<AUXFILE>;
		$trash=<AUXFILE>;
		$nCountReg=<AUXFILE>;
		chomp($nCountReg);
    if ($nCountReg eq "") {
				$nCountReg = 0;
		}	
		close(AUXFILE);
		PrintLog("($nCountReg) reg.\n");
		if ($nCountReg <= 1) {
		    PrintLog("NO HAY REGISTROS PARA ENVIAR\n");
		    #&SendTrap("NO HAY REGISTROS PARA ENVIAR\n");
		    &AbortEnvioTT(6,1);
		}
		return $nCountReg;
}

sub Cant_Escaneos_Dmp {
		$nDumpReg = 0;
		#Comandos para el Dump de la Tabla NovTT
		open(AUXFILE,">$FILE_DUMP_AUX");
		print AUXFILE "select from NovTT where post:adt_novtt_listo = \"S\" && post:adt_novtt_trans != \"T\" dump into $FILE_DUMP_DMP;\n";
		close(AUXFILE);
	
		PrintLog("EJECUTO DUMP DE REGISTROS NOVEDADES TT....\n");
		system("ql -Smain -n$PROY  < $FILE_DUMP_AUX > $FILE_DUMP_LOG 2>&1");
		open(AUXFILE,"<$FILE_DUMP_LOG") or die "No encontre archivo";
		while ($Linea = <AUXFILE>) {
		    chomp($Linea);
		    if ($Linea eq "Redirection completed") {
		        $Linea = <AUXFILE>;
		        (@Campos) = split(/ /,$Linea);
		        $nDumpReg = @Campos[0];
		        if ($nDumpReg eq "") {
								$nDumpReg = 0;
						}
		        break;
		    }
		}
		close(AUXFILE);
		PrintLog("($nDumpReg) reg.\n");
		if ($nDumpReg <= 1) {
		    PrintLog("NO HAY REGISTROS PARA ENVIAR\n");
		    #&SendTrap("NO HAY REGISTROS PARA ENVIAR\n");
		    &AbortEnvioTT(7,1);
		}
		return $nDumpReg;
}

sub Cant_Escaneos_OL {
		PrintLog("CUENTO CANTIDAD DE REGISTROS OL A ENVIAR....\n");
		$nCountReg = 0;
		open(AUXFILE,">$FILE_ESCANEOS_OL_AUX");
		print AUXFILE "select post:scanOL_tipo post:scanOL_estado_ent from Escaneos_OL dump into $FILE_ESCANEOS_OL_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_ESCANEOS_OL_AUX > $FILE_ESCANEOS_OL_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_ESCANEOS_OL_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;		
		    while ($Linea = <AUXFILE>) {
		        chomp($Linea);
		        (@Campos) = split(/\|/,$Linea);

						#Comandos para el Conteo de Registros de la Tabla NovTT
						open(AUXFILE_AUX,">$FILE_COUNT_AUX");
						print AUXFILE_AUX "select count(post:adt_novtt_op) from NovTT where post:adt_novtt_en_cond = \"S\" and post:adt_novtt_trans != \"T\" and post:adt_novtt_tipo = @Campos[0] and post:adt_novtt_estado_ent = \"@Campos[1]\" and post:adt_novtt_fecha <= $FECHA_HORA_REV dump into $FILE_COUNT_DMP;\n";
						close(AUXFILE_AUX);
						
						$cantidad = 0;
						system("ql -Smain -n$PROY < $FILE_COUNT_AUX > $FILE_COUNT_LOG 2>&1");
						open(AUXFILE_AUX,"<$FILE_COUNT_DMP");
						$trash=<AUXFILE_AUX>;
						$trash=<AUXFILE_AUX>;
						$cantidad=<AUXFILE_AUX>;
						chomp($cantidad);
						if ($cantidad eq "") {
								$cantidad = 0;
						}
						$nCountReg = $nCountReg + $cantidad;
						close(AUXFILE_AUX);		
				}
		close(AUXFILE);		
		PrintLog("($nCountReg) reg.\n");
		if ($nCountReg < 1) {
			PrintLog("NO HAY REGISTROS OL PARA ENVIAR\n");
			#&SendTrap("NO HAY REGISTROS OL PARA ENVIAR\n");
			&AbortEnvioTT(6,1);
		}
		return $nCountReg;	
}

sub Cant_Escaneos_OL_Dmp {
		PrintLog("CUENTO CANTIDAD DE REGISTROS DUMP OL A ENVIAR....\n");
		$nDumpReg = 0;
		my $flag = 0;
		open(AUXFILE,"<$FILE_ESCANEOS_OL_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;		
		    while ($Linea = <AUXFILE>) {
		        chomp($Linea);
		        (@Campos) = split(/\|/,$Linea);
		        #Comandos para el Dump de la Tabla NovTT
						if ($flag eq 0) {
								$flag = 1;
								open(AUXFILE_AUX,">$FILE_DUMP_AUX");
								print AUXFILE_AUX "select from NovTT where post:adt_novtt_en_cond = \"S\" and post:adt_novtt_trans != \"T\" and post:adt_novtt_tipo = @Campos[0] and post:adt_novtt_estado_ent = \"@Campos[1]\" and post:adt_novtt_fecha <= $FECHA_HORA_REV dump into $FILE_DUMP_DMP;\n";
								#creo el archivo de fin de ejecucion
								open(AUXFILE_FIN_AUX,">$FILE_FIN_AUX");
								print AUXFILE_FIN_AUX "update CodSeq set post:adt_codseq_nro = $SECUENCIADOR_NRO where post:adt_codseq_cod = \"$SECUENCIADOR\";\n";
								print AUXFILE_FIN_AUX "update CodSeq set post:adt_codseq_nro = $SECUENCIADOR_OL_NRO where post:adt_codseq_cod = \"$SECUENCIADOR_OL\";\n";
								print AUXFILE_FIN_AUX "update NovTT set post:adt_novtt_trans = \"T\" where post:adt_novtt_en_cond = \"S\" and post:adt_novtt_trans != \"T\" and post:adt_novtt_tipo = @Campos[0] and post:adt_novtt_estado_ent = \"@Campos[1]\" and post:adt_novtt_fecha <= $FECHA_HORA_REV;\n";
						} else {
								open(AUXFILE_AUX,">>$FILE_DUMP_AUX");
								print AUXFILE_AUX "select from NovTT where post:adt_novtt_en_cond = \"S\" and post:adt_novtt_trans != \"T\" and post:adt_novtt_tipo = @Campos[0] and post:adt_novtt_estado_ent = \"@Campos[1]\" and post:adt_novtt_fecha <= $FECHA_HORA_REV dump onto $FILE_DUMP_DMP;\n";
								#creo el archivo de fin de ejecucion
								open(AUXFILE_FIN_AUX,">>$FILE_FIN_AUX");
								print AUXFILE_FIN_AUX "update NovTT set post:adt_novtt_trans = \"T\" where post:adt_novtt_en_cond = \"S\" and post:adt_novtt_trans != \"T\" and post:adt_novtt_tipo = @Campos[0] and post:adt_novtt_estado_ent = \"@Campos[1]\" and post:adt_novtt_fecha <= $FECHA_HORA_REV;\n";
						}
						close(AUXFILE_AUX);	
						close(AUXFILE_FIN_AUX);
				}
		close(AUXFILE);
	
		PrintLog("EJECUTO DUMP DE REGISTROS OL NOVEDADES TT....\n");
		system("ql -Smain -n$PROY  < $FILE_DUMP_AUX > $FILE_DUMP_LOG 2>&1");

		open(AUXFILE,"<$FILE_DUMP_DMP") or die "No encontre archivo";
		while ($Linea = <AUXFILE>) {
		        $nDumpReg = $nDumpReg + 1;
		}
		close(AUXFILE);
		
		PrintLog("($nDumpReg) reg.\n");
		if ($nDumpReg < 1) {
		    PrintLog("NO HAY REGISTROS OL PARA ENVIAR\n");
		    #&SendTrap("NO HAY REGISTROS OL PARA ENVIAR\n");
		    &AbortEnvioTT(7,1);
		}
		return $nDumpReg;
}

#Rutina que resta N minutos a la fecha actual
sub subTime {
		local ($Minutos) = @_;

		my $my_time = time;
		my $my_time = $my_time - ($Minutos * 60);
	
		my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime($my_time);
		$mon = $mon + 1;
		$year = $year + 1900;
	
		if (length($mon)<2) { $mon="0".$mon };
		if (length($mday)<2) { $mday="0".$mday };
		if (length($hour)<2) { $hour="0".$hour };
		if (length($min)<2) { $min="0".$min };
		if (length($sec)<2) { $sec="0".$sec };
	
		$myfecha = $year.$mon.$mday;
		$myhora = $hour.$min.$sec;
		$FECHA_HORA_REV = $myfecha.$myhora;
		
		PrintLog("MIN_REV_OL \[$MIN_REV_OL\] - FECHA_HORA_REV \[$FECHA_HORA_REV\]\n");
		
		return $FECHA_HORA_REV;
}

#Rutina que verifica si la Sucursal realizo el cierre del dia
#Devuelve "N", si no realizo el cierre del dia
#Devuelve "S", si realizo el cierre del dia
sub CierreDia {
		PrintLog("CONTROLO CIERRE DEL DIA....\n");
		open(AUXFILE,">$FILE_CIERRE_DIA_AUX");
		print AUXFILE "select siaf:adt_sucur_cierre_dia from sucur_act dump into $FILE_CIERRE_DIA_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_CIERRE_DIA_AUX > $FILE_CIERRE_DIA_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_CIERRE_DIA_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    if ($Linea eq "S") {
		        $CIERRE_DIA = "SI";
		    } else {
		        $CIERRE_DIA = "NO";
		    }
		close(AUXFILE);
		PrintLog("CIERRE DIA ($CIERRE_DIA).\n");
	  return $CIERRE_DIA;
}

#Rutina que verifica si la Sucursal esta cerrada
sub CierreSucursal {
		PrintLog("CONTROLO CIERRE DE LA SUCURSAL....\n");
		open(AUXFILE,">$FILE_CIERRE_SUC_AUX");
		print AUXFILE "select tes:suc_marca_cierre from DiaSuc dump into $FILE_CIERRE_SUC_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_CIERRE_SUC_AUX > $FILE_CIERRE_SUC_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_CIERRE_SUC_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    $CIERRE_SUC = $Linea;
		    if ($CIERRE_SUC eq "X") {
		    	 $CIERRE_SUC = "SI";
		  	} else {
		  		$CIERRE_SUC = "NO";
				}
		close(AUXFILE);
		PrintLog("CIERRE SUCURSAL ($CIERRE_SUC).\n");
		return $CIERRE_SUC;
}

#Rutina que verfica la marca de Envio OL en la tabla sucur_act
sub Envio_OL {
		PrintLog("CONTROLO ENVIO OL....\n");
		open(AUXFILE,">$FILE_ENVIO_OL_AUX");
		print AUXFILE "select post:marca_env_novtt_ol from sucur_act dump into $FILE_ENVIO_OL_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_ENVIO_OL_AUX > $FILE_ENVIO_OL_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_ENVIO_OL_DMP");
		$trash=<AUXFILE>;
		$trash=<AUXFILE>;
		$ENVIO_OL=<AUXFILE>;
		chomp($ENVIO_OL);
    if ($ENVIO_OL eq "X") {
    	 $ENVIO_OL = "SI";
  	} else {
  		$ENVIO_OL = "NO";
		}
		close(AUXFILE);
		PrintLog("EXISTE OTRO PROCESO EJECUTANDO EL ENVIO OL ($ENVIO_OL)\n");
		return $ENVIO_OL;
}

#Rutina que verifica si esta lockedo un registro en la tabla TranStat
sub tranStat {
		my($codigo_siaf) = @_;
		
		PrintLog("CONTROLO ESTADO DE LA TABLA TRANSTAT PARA CODIGO_SIAF ($codigo_siaf)....\n");
		open(AUXFILE,">$FILE_TRANSTAT_AUX");
		print AUXFILE "update TranStat set siaf:marca_transtat = \"X\" where siaf:codigo_siaf = \"$codigo_siaf\";\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_TRANSTAT_AUX > $FILE_TRANSTAT_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_TRANSTAT_LOG");
		    #Leo 1er Linea y 2da Linea vacias
		    $Linea = <AUXFILE>;
		    $Linea = <AUXFILE>;
		    #Leo 3er Linea Connecting to Data Server
		    $Linea = <AUXFILE>;
		    #Leo 4ta Linea vacia
		    $Linea = <AUXFILE>;
		    #Leo 5ta Linea MOA CDS Query Language Version 1.0
		    $Linea = <AUXFILE>;		    
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    if (substr($Linea,6,26) eq "Update_All failed: cds 803") {
		        $TRANSTAT_LOCKED = "SI";
		    } else {
		        $TRANSTAT_LOCKED = "NO";
		    }
		close(AUXFILE);
		PrintLog("LOCKEADO ($TRANSTAT_LOCKED).\n");
	  return $TRANSTAT_LOCKED;
}

#Actualiza la tabla sucur_act con la marca de Envio OL
sub Marca_Envio_OL {
		my ($marca) = @_;	
	
		PrintLog("ACTUALIZANDO MARCA ENVIO OL ($marca)....\n");
		open(AUXFILE,">$FILE_MARCA_OL_AUX");
		print AUXFILE "update sucur_act set post:marca_env_novtt_ol = \"$marca\";\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_MARCA_OL_AUX > $FILE_MARCA_OL_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_MARCA_OL_LOG");
		    #Leo 1er Linea y 2da Linea vacias
		    $Linea = <AUXFILE>;
		    $Linea = <AUXFILE>;
		    #Leo 3er Linea Connecting to Data Server
		    $Linea = <AUXFILE>;
		    #Leo 4ta Linea vacia
		    $Linea = <AUXFILE>;
		    #Leo 5ta Linea MOA CDS Query Language Version 1.0
		    $Linea = <AUXFILE>;		    
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    if (substr($Linea,6,17) ne "1 records updated" && substr($Linea,6,17) ne "0 records updated" ) {
					$MARCA_ENVIO_OL = "NO";
				} else {
		    	$MARCA_ENVIO_OL = "SI";
		    }
		close(AUXFILE);
		PrintLog("ACTUALIZADO ($MARCA_ENVIO_OL).\n");	
		return $MARCA_ENVIO_OL;
}

#Elimino archivo .gz
sub eliminoGZ {
	#Contadores de control archivos .GZ a eliminar
	$CANT_GZ = 0;
	$DELETED_GZ = 0;

	#Obtengo la lista de archivos .gz a eliminar
	system("dir $FILE_DIR\\*.gz /OD /B > $FILE_DIRGZ_AUX 2>&1");
	print "ELIMINACION DE ARCHIVO(S) .GZ\n";
	PrintLog("ELIMINACION DE ARCHIVO(S) .GZ\n");
	#Recorro la lista de .gz y los borro.
	open(FILE_GZ_DEL,"<$FILE_DIRGZ_AUX");
	while ( $l = <FILE_GZ_DEL>) {
		chomp($l);
    if ($l ne "No se encuentra el archivo") {
			PrintLog(".....($l) .....");
			system("del $FILE_DIR$l > $INFO_FILE 2>&1");
			#Si genero archivo
			if (-e $INFO_FILE) {
				my $size = -s $INFO_FILE;
				if ($size > 0) {
			 		#Logeo el error
			 		open(FILE_DELETE,"<$INFO_FILE");
			 		while ( $i = <FILE_DELETE>) {
						chomp($i);
						PrintLog("[$i]\n");
					}
			 		close(FILE_DELETE);
			 	} else {
					PrintLog("[SI]\n");
					$DELETED_GZ = $DELETED_GZ + 1;
				}
				$CANT_GZ = $CANT_GZ + 1;
			}
    } else {
			print "NO SE ENCONTRARON ARCHIVOS .GZ PARA ELIMINAR\n";
			PrintLog("NO SE ENCONTRARON ARCHIVOS .GZ PARA ELIMINAR\n");
    }		
	}
	close(FILE_GZ_DEL);
	#Elimino el LOG que genera con los .gz a eliminar
	unlink ("$FILE_DIRGZ_AUX");	
	
	if ($CANT_GZ > 0) {
		if ($CANT_GZ eq $DELETED_GZ) {
			print "SE HAN ELIMINADO \[$DELETED_GZ\] ARCHIVO(S) .GZ DE \[$CANT_GZ\]\n";
			PrintLog("SE HAN ELIMINADO \[$DELETED_GZ\] ARCHIVO(S) .GZ DE \[$CANT_GZ\]\n");
		} else {
			print "ERROR AL ELIMINAR \[$DELETED_GZ\] ARCHIVO(S) .GZ DE \[$CANT_GZ\]\n";
			PrintLog("ERROR AL ELIMINAR \[$DELETED_GZ\] ARCHIVO(S) .GZ DE \[$CANT_GZ\]\n");
		}
	}
}

sub obtengoSecuenciadorOL {
		PrintLog("OBTENGO SECUENCIADOR OL ($SECUENCIADOR_OL).\n");
		open(AUXFILE,">$FILE_SEC_OL_AUX");
		print AUXFILE "select post:adt_codseq_nro from CodSeq where post:adt_codseq_cod = \"$SECUENCIADOR_OL\" dump into $FILE_SEC_OL_DMP;\n";
		close(AUXFILE);
		system("ql -Smain -n$PROY < $FILE_SEC_OL_AUX > $FILE_SEC_OL_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_SEC_OL_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;
		    #Leo Valor
		    $Linea = <AUXFILE>;
		    chomp $Linea;
		    if ($Linea eq "") {
		        PrintLog( "SECUENCIADOR ($SECUENCIADOR_OL) NO EXISTE.\n");
		        &SendTrap("SECUENCIADOR ($SECUENCIADOR_OL) NO EXISTE.\n");
		        &AbortEnvioTT(5,1);
		    }
		    #Aumento en 1. Esto tendra efecto (se grabara en MOSAIC) recien cuando se halla completado
		    #el FTP.
		    $SECUENCIADOR_OL_NRO = $Linea + 1;
		    #Relleno con ceros adelante;
		    #Lo hago asi, porque en la V4 de Perl no funciona bien el sprintf
		    $Ceros = substr("000000",0,6 - length($SECUENCIADOR_OL_NRO));
		    $SECUENCIADOR_OL_FORMATEADO = $Ceros.$SECUENCIADOR_OL_NRO;
		
		close(AUXFILE);
		PrintLog("SECUENCIADOR ($SECUENCIADOR_OL) = $SECUENCIADOR_OL_NRO\n");	
		return $SECUENCIADOR_OL_FORMATEADO;
}

sub obtengoUNEG_CTP {
		PrintLog("OBTENGO UNEG Y CTP\n");
		
		open(AUXFILE,">$FILE_UNEG_CTP_AUX");
		print AUXFILE "select admin:adt_br_CUN admin:adt_br_ctpTT from BR_adt_br where admin:adt_br_branch = \"$NIS\" dump into $FILE_UNEG_CTP_DMP;\n";
		close(AUXFILE);
		system("ql -Sgene -n$PROY < $FILE_UNEG_CTP_AUX > $FILE_UNEG_CTP_LOG 2>&1");
		
		open(AUXFILE,"<$FILE_UNEG_CTP_DMP");
		    #Leo Encabezado CDS_FIXED_FORMAT
		    $Linea = <AUXFILE>;
		    #Leo Encabezado (Nombre del campo)
		    $Linea = <AUXFILE>;
		    chomp($Linea);
				(@Campo) = split(/\|/,$Linea);
		    #Leo Valor
		    $Linea = <AUXFILE>;
				chomp($Linea);
				(@Valor) = split(/\|/,$Linea);
				if (@Valor[0] eq "") {
		        PrintLog("NO SE PUDO OBTENER VALOR DEL CAMPO (@Campo[0]).\n");
		        &SendTrap("NO SE PUDO OBTENER VALOR DEL CAMPO (@Campo[0]).\n");
		        &AbortEnvioTT(5,1);
		    }
				if (@Valor[1] eq "") {
		        PrintLog("NO SE PUDO OBTENER VALOR DEL CAMPO (@Campo[1]).\n");
		        &SendTrap("NO SE PUDO OBTENER VALOR DEL CAMPO (@Campo[1]).\n");
		        &AbortEnvioTT(5,1);
		    }				
				$UNEG=@Valor[0];
				$CTP=@Valor[1];
		close(AUXFILE);
		PrintLog("UNEG = ($UNEG), CTP ($CTP)\n");	
}

sub agregoHeaderOL {
		PrintLog("AGREGANDO HEADER OL (54) EN DUMP\n");
		
		open(AUXFILE_AUX,">>$FILE_DUMP_DMP");
		print AUXFILE_AUX "post:adt_novtt_op|99999|post:adt_novtt_sequence|$SECUENCIADOR_OL_FORMATEADO|post:adt_novtt_tipo|54|post:adt_novtt_fecha|$fecha$hora|post:adt_novtt_uneg|$UNEG|post:adt_novtt_ctp|$CTP|post:adt_novtt_ws|01|post:adt_novtt_listo|S|post:adt_novtt_trans|F\n";
		close(AUXFILE_AUX);	
		
		$nDumpReg  = $nDumpReg  + 1;
	  $nCountReg = $nCountReg + 1;
		
		PrintLog("CANTIDAD DE REGISTROS OL A ENVIAR....($nCountReg) reg.\n");
		PrintLog("CANTIDAD DE REGISTROS DUMP OL A ENVIAR....($nDumpReg) reg.\n");	
}

sub createGZ_BCK {
	my ($gz_bck) = @_;

	#Verifico directorios de backup de archivos GZ
	PrintLog("VERIFICANDO EXISTENCIA DE DIRECTORIO \[$gz_bck\]\n");
	if (-d $gz_bck) {
	   # El directorio existe
	   PrintLog(".....EXISTE\n");
	} else {
	   # El directorio NO existe
	   PrintLog("CREANDO DIRECTORIO \[$gz_bck\]\n");
	   system("mkdir $gz_bck");
	   PrintLog(".....CREADO\n");
	}
}

sub moveGZ_BCK {
	my ($gz_bck) = @_[0];
	my ($novtt_log) = @_[1];

	PrintLog("MOVIENDO ARCHIVO (S) .GZ \[$gz_bck\] A [$FILE_DIR]\n");
	system("move $gz_bck\\*.gz $FILE_DIR >> $novtt_log 2>&1");
	PrintLog("....LISTO\n");
}

sub verifFileProc {
	my ($gz_bck) = @_[0];
	my ($novtt_log) = @_[1];
	
	$ENCONTRO = "N";
	$NOMBRE_ARCH = $FILE_DIR . $NIS . "p*.1";
	PrintLog("VERIFICANDO EXISTENCIA DE ARCHIVOS $NOMBRE_ARCH\n");
	$retval = system("dir $NOMBRE_ARCH /OD /B >$FILE_FIND_AUX 2>>$novtt_log");
	if ($retval == 0) {
		$ENCONTRO = "S";
	} else {
		PrintLog("NO SE ENCONTRARON ARCHIVOS CON EXTENSION p*.1\n");
	}
	$NOMBRE_ARCH = $FILE_DIR . $NIS . "p*.2";
	PrintLog("VERIFICANDO EXISTENCIA DE ARCHIVOS $NOMBRE_ARCH\n");
	$retval = system("dir $NOMBRE_ARCH /OD /B >>$FILE_FIND_AUX 2>>$novtt_log");
	if ($retval == 0) {
		$ENCONTRO = "S";
	} else {
		PrintLog("NO SE ENCONTRARON ARCHIVOS CON EXTENSION p*.2\n");
	}
	if ($ENCONTRO eq "S") {
		&createGZfiles($gz_bck,$novtt_log);
		&moveToBackupGZ($gz_bck,$novtt_log);
	}		
	PrintLog("FIN VERIFICACION EXISTENCIA DE ARCHIVOS ".$NIS."p*1 y ".$NIS.".p*.2\n");
}

sub createGZfiles {
	my ($gz_bck) = @_[0];
	my ($novtt_log) = @_[1];
	my ($row) = "";
	my ($FILE_COMP_AUX) = $FILE_DIR."gz_tar.bat";
  
  #Crea bat de compresion
	open (AUXFILE,">$FILE_COMP_AUX");
	print AUXFILE "\@ECHO OFF\n";
	print AUXFILE "cd $FILE_DIR \n";

	#Recorro la lista de p.*1 y p*.2 y genero los .gz
	open(FILE_GZ_FILE,"<$FILE_FIND_AUX");
			while ($row = <FILE_GZ_FILE>) {
				chomp($row);
				PrintLog("....GENERANDO \[$row.gz\] DE \[$row\]\n");
				print AUXFILE "c:\\tools\\gzip -f $row >>$novtt_log 2>&1 \n";
			}
	print AUXFILE "exit\n";
	close(FILE_GZ_FILE);
	#Elimino archivo auxiliar
	unlink ("$FILE_FIND_AUX");
	close (AUXFILE);

	$retval = system("$FILE_COMP_AUX");
	if ($retval != 0) {
		PrintLog("NO SE PUDO COMPRIMIR LA INFORMACION A ENVIAR ".$NIS."p*.1 y ".$NIS."p*.2\n");
	}
	#Elimino archivo auxiliar
	unlink ("$FILE_COMP_AUX");
}

#Realizo el backup de los archivos .gz
sub moveToBackupGZ {
	my ($gz_bck) = @_[0];
	my ($novtt_log) = @_[1];
	my $files_gz = $FILE_DIR.$NIS."p*.gz";
	
	PrintLog("MOVIENDO ARCHIVOS [$files_gz] A [$gz_bck]\n");
	system("move $files_gz $gz_bck >> $novtt_log 2>&1");
	PrintLog("...LISTO\n");
}

##########################################################################################
@rem = '-*- Perl -*-';
@rem = '
@echo off
perl c:\moa\bin\cdsnight.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
#
#  NOTE:  Don't change or move the above garbage (except the path to this file)!
#  This is the trick that lets this .CMD file be really a PERL script!
#
# require "registry.pl";
$USAGE="Usage: cdsnight [ -n PROJECT] [-t TimeServer ] [-viUV] [ServiceNameSuffix1[, ServiceNameSuffix2, ..., SuffixN]]
	-i = using inittab instead of NT Services Manager
	-n  project name, default is value of MOACURPROJ from registry
	-t  rdate time server name, default is none
	-v = verbose mode
	-U = print Usage and exit
	-V = print Version and exit\n";

$REV="Version 1.4    November 1, 1996\n"; 

$PROJNAME="";
$debugsw=0;
$TMPOUT="\\temp\\cdsnight.tmp";
do Getopts('n:ivUV') || die $USAGE;	
#printf ("ARGC =%d 0=%s,1=%s, 2=%s\n",$#ARGV,$ARGV[0],$ARGV[1], $ARGV[2]);

$PROJNAME  = $opt_n;
@SnSUFFIX  = @ARGV;
$debugsw   = ($opt_v ? 1 : 0);
$usingInit   = ($opt_i ? 1 : 0);
if ($usingInit == 0) {
	if (-x cdinstsv.exe) {
		$ok="good";
	}
	else {
		$usingInit = 1;
		if ($debugsw != 0) {
			print "\nusing Init\n";
		}
	}
}

if ($debugsw != 0) {
	printf("usingInit = %d\n", $usingInit);
}
if ( $opt_U ) { print "$USAGE"; exit 0; }        # show usage
if ( $opt_V ) { print "\n$0 $REV"; exit 0; } # show version number
$TimeServer = $opt_t;
if ( $TimeServer ne "") {
	system("rdate $TimeServer");
}

$SuffixCount = $#SnSUFFIX+1;
if ($debugsw) {
	printf ("SuffixCount =%d 0=%s,1=%s, 2=%s\n",$SuffixCount,$SnSUFFIX[0],$SnSUFFIX[1], $SnSUFFIX[2]);
	print $TMPOUT, "\n";
}
# Get MOAPROJ for this system
	# See if environment variable is set
	$moaproj = $ENV {'MOAPROJ'};
	if ($moaproj eq "") {
		# Get path to source from registry
		system("putreg >c:\\temp\\impwrk");
		open(CONFIG_RECORD,"c:\\temp\\impwrk") || die "Can't open config work file\n";
		read(CONFIG_RECORD,$config,1024);
		close(CONFIG_RECORD);
		system("del \\temp\\impwrk");
		$pos = 0;
		$pos = index($config,"MOAPROJ = \"",$pos);
		$pos = index($config,"\"",$pos);
		$pos = $pos + 3;
		$pos2 = index($config,"\"",$pos);
		$pos2 = $pos2 - $pos;
		$moaproj=substr($config,$pos,$pos2);
	}
if ($moaproj eq "") {
	die "MOAPROJ variable undefined\n"
	}

# If project name not specified, see if there's a MOACURPROJ.
if ($PROJNAME eq "") { 
	# See if environment variable is set
	$PROJNAME = $ENV {'MOACURPROJ'};
	if ($PROJNAME eq "") {
		# See if registry variable is set 
		$pos = 0;
		$pos = index($config,"MOACURPROJ",$pos);
		$pos = index($config,"\"",$pos);
		$pos = $pos + 1;
		$pos2 = index($config,"\"",$pos);
		$pos2 = $pos2 - $pos;
		$PROJNAME=substr($config,$pos,$pos2);
		}
	}
$PROJNAME eq "" && do {print "\nPlease specify project name.\n"; print "$USAGE"; exit 0};

$PROJOPT = "-n $PROJNAME";
if ($debugsw != 0) {print "setting up date for compressing  volatile CDS tables\n";}
$Date = eval(`date_add today -7 days`);
$ENV{'RetainDate'} = $Date;
if ($debugsw != 0) {
	printf ("RetainDate = %s\n", $Date);
	printf ("MOACURPROJ = %s\n", $PROJNAME);
}

$cds_dir = "$moaproj\\$PROJNAME\\cds";
if ($debugsw) {
	printf("cds_dir = '%s'\n", $cds_dir);
}
if ( -d $cds_dir) {
	if ($debugsw) {
		printf "$cds_dir is a directory\n";
	}
	unshift(@SnSUFFIX, "");
	$SuffixCount += 1;
}
else {
	if ($debugsw) {
		printf "$cds_dir doesn't exist\n";
	}
}
@services = (@SnSUFFIX);

while(@services) {
	if ($services[0] eq "") {
		$SOPT = "";
	} else {
		$SOPT = "-S $services[0]";
	}
	if ($debugsw != 0) { printf(" SOPT = '%s'\n", $SOPT);   }
	shift (@services);
	# kill cdsv to disconnect clients and clean up the cdsv heap
	# Assume MOACURPROJ is set;
	if ( $usingInit == 1 ) {
		system ("cdskill $PROJOPT $SOPT -v");
		# restart of cdsv handled via inittab ;
	}
	else {
		system("cdinstsv $PROJOPT $SOPT stop");
		if ($debugsw != 0) { system("/mksnt/date"); }
	}
}
if ( $usingInit == 1 ) {
	system("net start inittab >$TMPOUT 2>&1") ;
	sleep 30;
}
			   
@services = (@SnSUFFIX);

while(@services) {
	if ($services[0] eq "") {
		$SOPT = "";
		$CompressScriptsDir="";
	} else {
		$SOPT = "-S $services[0]";
		$CompressScriptsDir="\\$services[0]";
		if ($debugsw) {
			print "CompressScriptsDir = $CompressScriptsDir \n";
		}
	}
	if ($debugsw != 0) { printf(" SOPT = '%s'\n", $SOPT);   }
	shift (@services);
	# Assume MOACURPROJ is set;
	if ( $usingInit == 1 ) {
		# restart of cdsv handled via inittab ;
		system("net start inittab >$TMPOUT 2>&1") ;
		sleep 10;
	}
	else {
		sleep 5;
		system("cdinstsv $PROJOPT $SOPT start");
		if ($debugsw != 0) { system("/mksnt/date"); }
	}
	if ($debugsw != 0) { print " wait for cdsv to finish restarting\n  ";   }
	sleep 15 ;
	$nameresult = system ("cdsname $PROJOPT $SOPT >>$TMPOUT 2>&1");
	if ($nameresult != 0) {
		printf ("cdsname $SOPT result = %d\n", $nameresult);
		system("cdsstat $SOPT");
		exit 55; 
	}

##
## COMPRESS VOLATILE TABLES
##
	system("@echo compressing  volatile CDS tables\n");     
	$compressFile = sprintf("%s\\%s%s\\compress.ql", $moaproj, $PROJNAME, $CompressScriptsDir );
	if ($debugsw) {print $compressFile, "\n";  }
	if (! -f $compressFile) {
		printf("\nERROR: No such file as %s  !!\n", $compressFile);
		exit 55;
	} else {
			system("ql $SOPT < $compressFile");
			if ($debugsw) { print "ql $SOPT for compress ended\n";}
	}
	# kill cdsv to allow quiet during backup
	sleep 1;
	if ( $usingInit == 0 ) {
		if ($debugsw != 0) { printf("stoping with 'cdinstsv %s %s stop'\n", $PROJOPT, $SOPT);  }
		sleep 61;
		system("cdinstsv $PROJOPT $SOPT stop");
		if ($debugsw != 0) { system("/mksnt/date"); }
	} 
	else {
		system ("net stop inittab");
		system ("cdskill $PROJOPT $SOPT >> $TMPOUT 2>&1");
	}
}
$backlogfile="c:\\admin\\backupcp.log";
printf ("\n\nRemoving %s\n", $backlogfile);
unlink $backlogfile;
system("@echo Backing up Machine: $ENV{'COMPUTERNAME'} \n");
# system("ntbackup backup c:\ /a /v /d 'myMachine backup' /b /T Normal /L 'c:\admin\backupcp.log' /E");
sleep 1;
system("@echo \nBackup finished\n");
@services = (@SnSUFFIX);
$once = 1;
while(@services) {
	if ($services[0] eq "") {
		$SOPT = "";
	} else {
		$SOPT = "-S $services[0]";
	}
	if ($debugsw != 0) { printf("restart SOPT = '%s'\n", $SOPT);   }
	shift (@services);
	#
	# restart cdsv
	#
	if ( $usingInit == 0 ) {
		sleep 55;
		if ($debugsw != 0) { printf("restarting with 'cdinstsv %s %s start'\n", $PROJOPT, $SOPT); }
		system("cdinstsv $PROJOPT $SOPT start");
		if ($debugsw != 0) { system("/mksnt/date"); }
	}
	else {
		if ($once == 1) {
			system("net start inittab");
			$once = 0;
		}
	}
	sleep 15; 
	system ("cdsname $PROJOPT $SOPT >>$TMPOUT");

	#
	# CLEAR THE RECOVERY LOG
	system ("cdsclrlg $PROJOPT $SOPT");
}                           

#
# verify no errors
#
system("cdsevent -d1 $PROJOPT");

if ($debugsw == 0) {unlink $TMPOUT;}

system("@echo \nCDS nightly finished\n");
 
0;
;# getopts.pl - a better getopt.pl
;# Usage:
;#      do Getopts('a:bc');  # -a takes arg. -b & -c not. Sets opt_* as a
;#                           #  side effect.

sub Getopts {
    local($argumentative) = @_;
    local(@args,$_,$first,$rest);
    local($errs) = 0;
    local($[) = 0;

    @args = split( / */, $argumentative );
    while(@ARGV && ($_ = $ARGV[0]) =~ /^-(.)(.*)/) {
		($first,$rest) = ($1,$2);
		$pos = index($argumentative,$first);
		if($pos >= $[) {
		    if($args[$pos+1] eq ':') {
				shift(@ARGV);
				if($rest eq '') {
				    ++$errs unless @ARGV;
				    $rest = shift(@ARGV);
				}
				eval "\$opt_$first = \$rest;";
		    }
		    else {
				eval "\$opt_$first = 1";
				if($rest eq '') {
				    shift(@ARGV);
				}
				else {
				    $ARGV[0] = "-$rest";
				}
		    }
		}
		else {
		    print STDERR "Unknown option: $first\n";
		    ++$errs;
		    if($rest ne '') {
				$ARGV[0] = "-$rest";
		    }
		    else {
				shift(@ARGV);
		    }
		}
    }
    $errs == 0;
}

# ****************
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl



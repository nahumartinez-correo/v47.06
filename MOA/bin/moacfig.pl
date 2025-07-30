@rem = '-*- Perl -*-';
@rem = '
@echo off
callperl %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
#
#  NOTE:  Don't change or move the above garbage (except the path to this file)!
#  This is the trick that lets this .PL file be really a PERL script!
#
# $Name: $
# $Revision: 1.6 $
# $Date: 1998/10/01 21:47:22 $
#

# this subroutine will return you the value of the environment
# variable passed as argument 1.  The call to ENV is not trustworthy
# because it is case-sensitive
sub ExtGetEnv{
	local ($EnvVar) = @_;
	# get everything in lower case so we can compare apples
	# to apples
	$EnvVar =~ y/A-Z/a-z/;
	$isvar = 0;
	$found = 0;
	# the environment array contains first the variable name
	# then the variable value as the next element of the array
	for (%ENV) {
		# if a match was found, return the value
		$found && return $_;

		# if $isvar = 1 then this is a value not
		# a variable name.  Just continue to the
		# next element
		if ($isvar) {
			$isvar = 0;
			next;
		}
		y/A-Z/a-z/;	# convert it to lower case
		# we know the next element is a value so
		# set the flag
		$isvar = 1;

		# compare the two and set the found flag on a match
		if ( $_ eq $EnvVar ) { $found = 1 };
	}
	return "";
}

# The subroutine will set the global variables $CFG_moaproj and $CFG_moacurproj
# with the values for MOAPROJ and MOACURPROJ.  Environment variables are used
# if they are set, otherwise they are looked up in the Registry.

sub MOARegConfig {
local($tempdir) = "c:\\temp";
local($buf);
$CFG_moaproj = "";
$CFG_moacurproj = "";

$os=&ExtGetEnv("OS");
if ( $os eq "Windows_NT") {
	$redir_cmd="2>&1";
	}
else {
	$redir_cmd="";
	}
# Get registry MOA config
if (! -d $tempdir) {
	system"mkdir $tempdir" || die "Can't create $tempdir directory:  $!\n";
	}
system("putreg >$tempdir\\impwrk $redir_cmd");
open(CONFIG_RECORD,"$tempdir\\impwrk") || die "Can't open config work file $tempdir\\impwrk\n";
read(CONFIG_RECORD,$buf,1024);
close(CONFIG_RECORD);
system("del $tempdir\\impwrk");

# Get MOADIR
# See if environment variable is set
$CFG_moadir = &ExtGetEnv("MOADIR");
if ($CFG_moadir eq "") {
	$pos = 0;
	$pos = index($buf,"MOADIR",$pos);
	$pos = index($buf,"= \"",$pos);
	if ($pos < 0) {
		die "The MOADIR registry variable must be defined.\n";
		}
	else {
		$pos = index($buf,"\"",$pos);
		$pos = $pos + 1;
		$pos2 = index($buf,"\"",$pos);
		$pos2 = $pos2 - $pos;
		$CFG_moadir=substr($buf,$pos,$pos2);
		}
	}

# Get MOAPROJ
# See if environment variable is set
$CFG_moaproj = &ExtGetEnv("MOAPROJ");
if ($CFG_moaproj eq "") {
	$pos = 0;
	$pos = index($buf,"MOAPROJ = \"",$pos);
	if ($pos < 0) {
		die "The MOAPROJ registry variable must be defined.\n";
		}
	else {
		$pos = index($buf,"\"",$pos);
		$pos = $pos + 1;
		$pos2 = index($buf,"\"",$pos);
		$pos2 = $pos2 - $pos;
		$CFG_moaproj=substr($buf,$pos,$pos2);
		}
	}

# Get MOACURPROJ
# See if environment variable is set
$CFG_moacurproj = &ExtGetEnv ("MOACURPROJ");
if ($CFG_moacurproj eq "") {
	# See if registry variable is set 
 	$pos = 0;
	$pos = index($buf,"MOACURPROJ",$pos);
	if ($pos > 0) {
		$pos = index($buf,"\"",$pos);
		$pos = $pos + 1;
		$pos2 = index($buf,"\"",$pos);
		$pos2 = $pos2 - $pos;
		$CFG_moacurproj=substr($buf,$pos,$pos2);
		}
	}
}


# This subroutine will look up configuration in the project's ddproj.ini
# file.  It will look for all known variables in the [AIMS Config]
# section.  Defaults are used for all except Master Master.

sub MasterConfig {
	local($proj) = @_;
	local($inifile) = "$proj\\ddproj.ini";
	local($errmsg) = "Missing or incomplete configuration in\n  $inifile:\n";

#	&GetConfigVar($proj,"Master Master") | die "$errmsg Variable 'Master Master' not found. Can't continue\n";
#	$CFG_mastermast = $CFG_var;
	if (&GetConfigVar($proj,"Master Master") != 1) {
		die "$errmsg Variable 'Master Master' not found. Can't continue\n";
		}
	$CFG_mastermast = $CFG_var;

	$CFG_localmast = (&GetConfigVar($proj,"Local Master") ? $CFG_var : $CFG_mastermast);
	
	$CFG_lmsrc = (&GetConfigVar($proj,"LM Source") ? $CFG_var : "src");
	
	$CFG_mmsrc = (&GetConfigVar($proj,"MM Source") ? $CFG_var : "src");
	
	$CFG_worksrc = (&GetConfigVar($proj,"Working Source") ? $CFG_var : "src");

	$CFG_bitdir = (&GetConfigVar($proj,"Bitmap Dir") ? $CFG_var : "bitmaps");

	$CFG_icndir = (&GetConfigVar($proj,"Icon Dir") ? $CFG_var : "icn_dir");
}

# This subroutine will get a single requested variable from the [AIMS Config]
# section of the project's ddproj.ini file.  The value is placed in the
# general-use global variable, $CFG_var.  The routine returns 1 (True) if
# the variable is found, 0 (False) otherwise.

sub GetConfigVar {
	local($proj,$variable) = @_;
	local($inifile) = "$proj\\ddproj.ini";
	$CFG_var = "";
	$DEBUG && system("$echo_cmd GetConfigVar $variable");
	local($SectionFound) = 0;

	if (!open(FILE,$inifile)) {
		$DEBUG && system("$echo_cmd $errmsg  Can't open $inifile.");
		return 0;
		}
	while(<FILE>) {
		if ($SectionFound && /\[/) {  # End of section reached
			$DEBUG && system("$echo_cmd $errmsg  Variable \"$variable\" not found.");
			close(FILE);
			return 0;
			}
		if ($SectionFound && /^$variable\s*=\s*(.*)/) {
			$CFG_var=$1;
#			chop($CFG_var);
			$DEBUG && system("$echo_cmd Config value = $CFG_var");
			close(FILE);
			return 1;
			}
		if (/^\[AIMS Config\]/) {
			$SectionFound = 1;
			}
		}
	if (!$SectionFound) {
		$DEBUG && system("$echo_cmd $errmsg  Section \"\[AIMS Config\]\" not found in $inifile.");
		close(FILE);
		return 0;
		}
	close(FILE);
	return 0;
	}


1;
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .PL file be really a PERL script!
#
__END__
:endofperl


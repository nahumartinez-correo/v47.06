@rem = '-*- Perl -*-';
@rem = '
@echo off
callperl %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .BAT file be really a PERL script!
#
#
# $Name: $
# $Revision: 1.9 $
# $Date: 1998/10/01 21:47:43 $
#
#


# Purpose:
#   Imports source files from a project source directory.  Directories
#	are searched recursively.
#
# Notes: 
#   -A MOAPROJ and MOADIR variable must be defined either as an environment 
#    variable or in the registry (may be done through IBAE).  If both are set,
#    the environment variable values will be used.
#   -If project is not specified, MOACURPROJ (environment or registry
#    variable) will be used for project name.
#   -Directory names ending in "+" or "-" are skipped.	Also, "rcs",
#    bitmap, and icon directories are skipped.  File names are skipped
#	 according to the exceptions in the file "ofbxcpt.pl".
#   -This script requires the ofbxcpt.pl, getopts.pl, utils.pl and moacfig.pl libraries.
#    The libraries and this file must be in the directory configured as MOADIR.

# Add MOADIR directory to Perl's include search path
&GetMOADIR;
@INC[$#INC+1] = "$CFG_moadir\\bin"; # Add path to Include search path array

# NOTE:  utils.pl MUST be first in the list of libraries
require "utils.pl";
require "ofbxcpt.pl";
require "getopts.pl";
require "moacfig.pl";

$USAGE="USAGE: Imp [ -nPROJ -Iinclude -vlevel -smFUV ] directory|file [directory|file ...]
    -nPROJ     = project name
    -Iinclude  = path for include files
    -v[level]  = verbose mode (levels 1,2,3,4)
    -s         = suppress auto definition of forward-referenced objects
    -m         = write object with warning if already modified
    -F         = flags only
    -U         = print Usage and exit
    -V         = print Version and exit\n";

$REV="MOA AIMS Version 1.6
Part number: 85-35019-A06*
May 23, 1996\n";

$proj = "";
$include = "";
$LOOK="B";
$usage = 0;
$file_count=0;
$flags=0;
$no_auto_create=0;
$warn_modify=0;
$convert=0;
$r1=1;

&Getopts('n:I:v:usmFCSDBUV') || die $USAGE;
$proj           = $opt_n;
if ($opt_I) { 
	$include = "-I$opt_I";
	$include =~ s/\+/ -I/g;	   # Replace pluses to delimit multiple include paths
	}
$vnum           = $opt_v;
$unlock         = ($opt_u ? 1 : 0);
$no_auto_create = ($opt_s ? 1 : 0);
$warn_modify    = ($opt_m ? 1 : 0);
$flags          = ($opt_F ? 1 : 0);
$convert        = ($opt_C ? 1 : 0);
$unlock         = ($opt_u ? 1 : 0);
if ($opt_S) { $LOOK = "S" };
if ($opt_D) { $LOOK = "D" };
if ($opt_B) { $LOOK = "B" };

if ($opt_U) { print "$USAGE"; exit 0 };	 # show usage
if ($opt_V) { print "\n$0\n$REV"; exit 0 };  # show version number

if ($vnum) { $visual = 1; }
if ($vnum >= 2) { $visualx = 1; }
if ($vnum >= 3) { $visualxx = 1; }
if ($vnum >= 4) { $visualxxx = 1; }

&MOARegConfig;
$moaproj = $CFG_moaproj;

if ($proj eq "") { 
	$proj = $CFG_moacurproj;
	}

$proj eq "" && do {print "\nPlease specify working project name.\n"; print "$USAGE"; exit 1};

$projdir="$moaproj\\$proj";
if ( ! -d $projdir ) { die  "Project directory $projdir does not exist\n";}

# Get source directory name from ddproj.ini file
$SRC = (&GetConfigVar($projdir,"Working Source") ? $CFG_var : "src");

#Get icon directory name from ddproj.ini file
$ICNDIR = (&GetConfigVar($projdir,"Icon Dir") ? $CFG_var : "icn_dir");

$include = "$include -I$projdir\\$SRC\\include";

system("$echo_cmd IMP: Project:  $proj");
system("$echo_cmd IMP: Includes:  $include");
if ( $vnum eq 4) { system("$echo_cmd IMP: Compile Debug mode"); }
if ( $vnum eq 3) { system("$echo_cmd IMP: Extended verbose(3) mode"); }
if ( $vnum eq 2) { system("$echo_cmd IMP: Extended verbose mode"); }
if ( $vnum eq 1) { system("$echo_cmd IMP: Verbose mode"); }
if ( $LOOK eq "S") {system("$echo_cmd IMP: Standard Look");}
if ( $LOOK eq "D") {system("$echo_cmd IMP: Desktop Look");}
if ( $LOOK eq "B") {system("$echo_cmd IMP: Standard and Desktop Look");}
$no_auto_create && system("$echo_cmd IMP: No auto-creation of forward references");
$flags && system("$echo_cmd IMP: Import Flags Only"); 
$warn_modify && system("$echo_cmd IMP: Warning for Modification");
if ($convert) { $r1 = 1; system("$echo_cmd IMP: Converting files to moa"); }

&dodir(@ARGV);

if ($unlock ) {
	system("$echo_cmd IMP: Unlocking $proj");
	$LOCKFILE = "$projdir\\rcs\\lockdd";
	if (-f $LOCKFILE) {
		system("del /Q /F $LOCKFILE");
		}
	}

sub dodir {
foreach $fname (@_) 
{

if ( -d $fname ) {
	# If the file name passed in isn't a full path, prepend the current directory
	if ( (substr($fname,0,1) ne "\\") && (substr($fname,1,1) ne ":")) {
		$tmp = &doCommand ("cd");
		$visualxx && system("$echo_cmd Result of doCommand is $tmp");
		chop ($tmp);
		if ( substr($tmp,$length - 1,1) ne "\\" ) {
			$tmp = $tmp . "\\";
		}
		$fname = substr($tmp,2) . $fname;
		system("$echo_cmd Full pathname is $fname");
	}
	local ($dir) = $fname;
	system("$echo_cmd IMP: Importing $fname");
	opendir (DIR,$fname) || die "cant read dir $dir";
	local (@filenames) = readdir(DIR);
	closedir (DIR);

	chdir $dir || die "cant change dir $dir";
	local($cnt) = 0;
	local($list) = " ";
	for (@filenames) {
		if (-f $_) {
			$list[$cnt++] = $_;
		};
	}
	$list[$cnt] = "END_OF_LIST";
	if ($cnt) {
		&process(@list);}
	for (@filenames) {
		y/A-Z/a-z/;
		next if $_ eq '.';
		next if $_ eq '..';
		next unless -d $_;
		$name = "$dir\\$_";
		/^RCS/ && do { next; };
		/^RCS/ && do { next; };
		/^rcs/ && do { next; };
		/\+$/ && do { next; };
		/\-$/ && do { next; };
		/^bit_dir/ && do { next; };
		/^$ICNDIR/ && do { next; };
#		/^icons/ && do { next; };
#		/^icn_dir/ && do { next; };
		/^udo_dir/ && do { next; };
		/^noside/ && do { system("$echo_cmd IMP: SKIPPING NOSIDE DIRECTORY"); next; };
		&dodir($name);
	}
	chdir '..';
}
else {
	&process($fname);
}
}
}

sub process{
local ($comp_list) = "";
local (@note_list) = "";
local ($note_list_cnt) = 0;
for (@_){
	if ( $_ eq "END_OF_LIST" ) { last;};
	y/A-Z/a-z/;
	local ($filename) = $_;
	@ImportParms[1] = $filename;
	$ImportParms[2] = "";
	if ( -f $filename ){
	  if ($flags) {
		/^fld|^flag\.fld/ && do { 
		    $ImportParms[0] = "fld";
		    if ($visual) {system("$echo_cmd IMP: import fld -- $filename"); }
		    &do_import(@ImportParms);
			};
		/lst$|list$/ && do { 
			if ($visual) {system("$echo_cmd IMP: import $filename");}
			&do_compile(@ImportParms);
			};
	  }
	  else {
		$_ = $filename;
		y/A-Z/a-z/;
	  	{
            &ofbException($filename) && do { system("$echo_cmd IMP: skipping $filename"); last };
			/\.h$/ && do { 
				&do_conv(@ImportParms);
				last };
			/^appls/ && do { 
			    $ImportParms[0] = "apl";
			    if ($visual) {system("$echo_cmd IMP: import apl -- $filename"); }
			    &do_import(@ImportParms);
			    last};
        	/^bit/ && do { 
				if ($visual) {system("$echo_cmd IMP: import bit -- $filename");}
				$ImportParms[0] = "bit";
#we don' use bit_dir 		$ImportParms[2] = "-fbit_dir";
				&do_import(@ImportParms);
				last };
			/^icn/ && do { 
				$ImportParms[0] = "icn";
				$ImportParms[2] = "-f$ICNDIR";
				if ($visual) {system("$echo_cmd IMP: import icn -- $filename from directory $ICNDIR"); }
				&do_import(@ImportParms);
				last};
			/^udo/ && do { 
				if ($visual) {system("$echo_cmd IMP: import udo -- $filename");}
				$ImportParms[0] = "udo";
				$ImportParms[2] = "-fudo_dir";
				&do_import(@ImportParms);
				last };
			/^dsc/ && do { 
				if ($visual) {system("$echo_cmd IMP: import dsc -- $filename"); }
				$ImportParms[0] = "dsc";
				&do_import(@ImportParms);
				last };
			/^fld/ && do { 
				if ($visual) {system("$echo_cmd IMP: import fld -- $filename");}
				$ImportParms[0] = "fld";
				&do_import(@ImportParms);
				last };
			/^csr/ && do { 
				if ($visual) {system("$echo_cmd IMP: import csr -- $filename");}
				$ImportParms[0] = "csr";
				&do_import(@ImportParms);
				last };
			/^flag\.fld/ && do { 
				if ($visual) {system("$echo_cmd IMP: import fld (f) -- $filename"); }
				$ImportParms[0] = "fld";
				&do_import(@ImportParms);
				last };
			/^mnb/ && do {
				if ($visual) {system("$echo_cmd IMP: import mnb -- $filename"); }
				$ImportParms[0] = "mnb";
				&do_import(@ImportParms);
				last };
			/^mnu/ && do { 
				if ($visual) {system("$echo_cmd IMP: import mnu -- $filename"); }
				$ImportParms[0] = "mnu";
				&do_import(@ImportParms);
				last };
			/^pdm/ && do { 
				if ($visual) {system("$echo_cmd IMP: import pdm -- $filename"); }
				$ImportParms[0] = "pdm";
				&do_import(@ImportParms);
				last };
			/^nts/ && do { 
				# notes are special, they won't import
				# unless the object exists, so let's queue
				# them up and do them last
				$note_list[$note_list_cnt++] = "$filename";
				last };
			/^pat/ && do { 
				if ($visual) {system("$echo_cmd IMP: import pat -- $filename"); }
				$ImportParms[0] = "pat";
				&do_import(@ImportParms);
				last };
			/^pic/ && do { 
				if ($visual) {system("$echo_cmd IMP: import pic -- $filename"); }
				$ImportParms[0] = "pic";
				&do_import(@ImportParms);
				last };
			/^plb/ && do { 
				if ($visual) {system("$echo_cmd IMP: import plb -- $filename"); }
				$ImportParms[0] = "plb";
				&do_import(@ImportParms);
				last };
			/^pmb/ && do { 
				if ($visual) {system("$echo_cmd IMP: import pmb -- $filename"); }
				$ImportParms[0] = "pmb";
				&do_import(@ImportParms);
				last };
			/^pmu/ && do { 
				if ($visual) {system("$echo_cmd IMP: import pmu -- $filename"); }
				$ImportParms[0] = "pmu";
				&do_import(@ImportParms);
				last };
			/^tag/ && do { 
				if ($visual) {system("$echo_cmd IMP: import tag -- $filename"); }
				$ImportParms[0] = "tag";
				&do_import(@ImportParms);
				last };
			&do_conv(@ImportParms);
			# just add the file to a list to be compiled later
			$comp_list = "$comp_list $filename";
		}
	   }
	}
	else {
		system("$echo_cmd IMP: Skipping $filename");
	}
}
	# Now we'll compile all of the ofb files using
	# one command to save time
	if (length($comp_list) > 1 ){
		if ($visual) {system("$echo_cmd IMP: Compiling $comp_list");}
		$ImportParms[1] = $comp_list;
		&do_compile(@ImportParms);
	}
	# If we had any notes to import we'll take care of them now
	if ($note_list_cnt >= 1 ){
		for (@note_list){
			if ($visual) {system("$echo_cmd IMP: import nts -- $_"); }
			$ImportParms[1] = $_;
			$ImportParms[0] = "nts";
			&do_import(@ImportParms);
		}
		# There is currently a bug in import which causes
		# it to destroy the source path stored in the notes
		# for ofb procedure.  So, we'll just compile the darn
		# things again until we get this fixed
		if (length($comp_list) > 1 ){
			if ($visual) {system("$echo_cmd IMP: Recompiling $comp_list");}
			$ImportParms[1] = $comp_list;
			&do_compile(@ImportParms);
		}
	}
}

sub do_import{
	if ( $convert ) {
		system("$echo_cmd IMP: Converting @_[1]");
		$file_name = @_[1];
		system("pcl2ofb <$file_name >TEMP_CNV.OUT");
		system("attrib +r $file_name");
		system("co -l $file_name");
		system("copy TEMP_CNV.OUT $file_name");
		system("del TEMP_CNV.OUT");
		system("ci -f -u $file_name <c:\\temp\\ci.txt");
		return;
	}

	if ( $proj ) { $command = "import -n$proj $include @_[2]";}
	else {$command = "import $include @_[2]";}
	if ($warn_modify) { $command = "$command -m";}
	if ($visualxx) { $command = "$command -v";}
	if ($LOOK eq "S") {$command = "$command -Z\"LOOK=0\"";}
	if ($LOOK eq "D") {$command = "$command -Z\"LOOK=2\"";}
	if ($LOOK eq "B") {$command = "$command -Z\"LOOK=2\"";}
	$command = "$command @_[1]";
	if ($visualx) {system("$echo_cmd IMP: Running $command");}
	system($command);
}

sub do_compile{
	if ($convert) { return;}
	if ($proj) {$command = "bc -n$proj $include";}
	else {$command = "bc $include";}
	if ($no_auto_create) { $command = "$command -s";}
	if ($visualxx) { $command = "$command -v";}
	if ($visualxxx) { $command = "$command -d";}
	if ($warn_modify) { $command = "$command -m";}
	if ($LOOK eq "S") {$command = "$command -Z\"LOOK=0\"";}
	if ($LOOK eq "D") {$command = "$command -Z\"LOOK=2\"";}
	if ($LOOK eq "B") {$command = "$command -Z\"LOOK=2\"";}
	$command = "$command @_[1]";
	if ($visualx) {system("$echo_cmd IMP: Running $command");}
	system($command);
}

sub do_conv{
	if ( $convert ) {
			system("$echo_cmd IMP: Converting @_[1]");
			$file_name = @_[1];
		system("pcl2ofb <$file_name >TEMP_CNV.OUT");
		system("attrib +r $file_name");
		system("co -l $file_name");
		system("copy TEMP_CNV.OUT $file_name");
		system("del TEMP_CNV.OUT");
		system("ci -f -u $file_name <c:\\temp\\ci.txt");
	}
}

# This subroutine will set the global variable $CFG_moadir
# with the value for MOADIR.  Environment variable is used
# if set, otherwise it is looked up in the Registry.

sub GetMOADIR {
local($tempdir) = "c:\\temp";
local($buf);
$CFG_moadir = "";

$os=&ExtGetEnv("OS");
if ( $os eq "Windows_NT") {
	$redir_cmd="2>&1";
	}
else {
	$redir_cmd="";
	}
# See if environment variable is set
$CFG_moadir = &ExtGetEnv("MOADIR");
if ($CFG_moadir eq "") {
	# Get registry MOA config
	if (! -d $tempdir) {
		system"mkdir $tempdir" || die "Can't create $tempdir directory:  $!\n";
		}
	system("putreg >$tempdir\\impwrk $redir_cmd");
	open(CONFIG_RECORD,"$tempdir\\impwrk") || die "Can't open config work file $tempdir\\impwrk\n";
	read(CONFIG_RECORD,$buf,1024);
	close(CONFIG_RECORD);
	system("del $tempdir\\impwrk");

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
}

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

#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .BAT file be really a PERL script!
#
__END__
:endofperl

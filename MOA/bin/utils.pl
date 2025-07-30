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
# $Revision: 1.4 $
# $Date: 1998/10/01 21:47:28 $
#
#

#Set up a global variable for the echo command so it's generic
#between NT and Win95.  The mks toolkit echo gives us grief under
#NT so we use @echo... but that doesn't work under win95
#Figure out if it's nt or win95
$os=&ExtGetEnv("OS");
if ( $os eq "Windows_NT") {
	$echo_cmd="@echo";
	}
else {
	$echo_cmd = "echo";
	}

# Creates a directory including all necessary parent directories
# Works with UNC file names, forward or backward slashes
# Returns 1 if creation was usuccessful, 0 otherwise
sub createPath {
	local($success) = 0;
	local($path) = @_;
	local($parent);

	$DEBUG && print "createPath:  PATH = $path\n";
	if (-e "$path" | -e "$path\\") {
		$DEBUG && print "createPath:  $path exists\n";
		$success = 1;
		}
	else {
		@pieces = split(/[\\\/]/,$path);  #Splits path on back slash or fwd slash
		if ($#pieces > 0 ) {
			$newdir = pop(@pieces);
			$parent = join("\\",@pieces);
			$DEBUG && print "createPath:  Splitting:  Newdir = $newdir; Parent = $parent\n";
				$success = &createPath($parent);
				$DEBUG && print "createPath:  Creating $path\n";
				$success = mkdir ($path,0777);
			}	
		}
	return $success;
}

#This function splits a given include path using the standard ";" delimiter
#into separate -I<path> pieces to work with the MOA command-line utilities
sub splitIncludes {
	local($incl) = @_;
	local($buf);

	@pieces = split(/\;/,$incl);  #Splits include path on semi-colon
	foreach $path (@pieces) {
		$buf = "$buf -I$path";
		}
	return $buf;	
}

#This is a workaround for the fact that Windows 95 doesn't support the
#back quote system execute command (`).
#This function will execute the given command directing the output to
#a temporary file, then read the file into a global variable, $cmdout.
#The buffer size is 1024, so any command output beyond that will be
#truncated.

sub doCommand {
	local ($cmd) = @_;
	$cmdout = "";

	system("$cmd >c:\\temp\\plwrk");
	open(CONFIG_RECORD,"c:\\temp\\plwrk") || die "Can't open temporary work file, c:\\temp\\plwrk\n";
	read(CONFIG_RECORD,$cmdout,1024);
	close(CONFIG_RECORD);
	system("del c:\\temp\\plwrk");
	return $cmdout;
}

1;
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .PL file be really a PERL script!
#
__END__
:endofperl


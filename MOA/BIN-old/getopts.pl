@rem = '
@echo off
callperl %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .PL file be really a PERL script!
#
#
# $Name: $
# $Revision: 1.4 $
# $Date: 1998/10/01 21:47:13 $
#
#
# getopts.pl - a better getopt.pl
# Usage:
#      do Getopts('a:bc');  # -a takes arg. -b & -c not. Sets opt_* as a
#                           #  side effect.

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

1;
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .PL file be really a PERL script!
#
__END__
:endofperl


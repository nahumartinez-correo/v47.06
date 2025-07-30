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
# $Revision: 1.5 $
# $Date: 1998/10/01 21:47:26 $
#

#Notes:  Exerpt on pattern matching from "Programming Perl," by Larry Wall and Randal L. Schwartz:

#          .  Matches any character except newline
#   [a-z0-9]  Matches any single character of set
#  [^a-z0-9]  Matches any single character not in set
#         \d  Matches a digit, same as [0-9]
#         \D  Matches a non-digit, same as [^0-9]
#         \w  Matches an alphanumeric (word) character [a-zA-Z0-9_]
#         \W  Matches a non-word character[^a-zA-Z0-9_]
#         \s  Matches a whitespace char (space, tab, newline...)
#         \S  Matches a non-whitespace character

#         \n  Matches a newline
#         \r  Matches a return
#         \t  Matches a tab
#         \f  Matches a formfeed
#         \b  Matches a backspace (inside [] only)
#         \0  Matches a null character
#       \000  Also matches a null character because...
#       \nnn  Matches an ASCII character of that octal value
#  \metachar  Matches the character itself (\|, \., \*...)

#      (abc)  Remembers the match for later backreferences
#	      \1  Matches whatever first of parens matches
#         \2  Matches whatever second set of parens matches
#         \3  and so on...

#         x?  Matches 0 or 1 x's, where x is any of above
#         x*  Matches 0 or more x's
#         x+  Matches 1 or more x's
# 	  x{m,n}  Matches at least m x's but no more than n

#        abc  Matches all of a, b and c in order
#fee|fie|foe  Matches one of fee, fie or foe

#         \b  Matches a word boundary (outside [] only)
#         \B  Matches a non word boundary
#          ^  Anchors match to the beginning of a line or string
#          $  Anchors match to the end of a line or string

#Returns 1 if input string matches one of the strings in the filename exception list
#else returns 0.
sub ofbException {
	local($ret) = 0;
	y/A-Z/a-z/;
	local ($in) = @_;
	$in =~ y/A-Z/a-z/;
	$DEBUG && system("$echo_cmd Checking for exception:  $in");
	#start of exceptions  ( the names are all lowercased now!)
	if ($in =~ /^core/) { return 1; };
	if ($in =~ /^rcs_/) { return 1; };
	if ($in =~ /^rcs\.rc/) { return 1; };
	if ($in =~ /^readme/) { return 1; };
	if ($in =~ /\+$/) { return 1;};
	if ($in =~ /\-$/) { return 1;};
	if ($in =~ /\.err$/) { return 1; };
	if ($in =~ /^tmp/) { return 1; };
	if ($in =~ /^temp/) { return 1; };
#	if ($in =~ /^appls/) { return 1; };
	if ($in =~ /\.idx$/) { return 1; };
	if ($in =~ /\.dat$/) { return 1; };
	if ($in =~ /\.qck$/) { return 1; };
	if ($in =~ /\.out$/) { return 1; };
	if ($in =~ /\-v$/) { return 1; };
	if ($in =~ /^[0-9]/) { return 1; };
	if ($in =~ /^[a-z]{1,1}$/) { return 1; };
	if ($in =~ /^\-/) { return 1; };
	if ($in =~ /^\+/) { return 1; };
	if ($in =~ /^\./) { return 1; };
	if ($in =~ /[0-9][0-9][0-9]$/) { return 1; };

	return $ret;
}

1;
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .PL file be really a PERL script!
#
__END__
:endofperl


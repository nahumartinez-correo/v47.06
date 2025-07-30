@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\jnlpr.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#* History * * * * * * * * * * * * * * * * * * * * * * * * * 
#  V1  960430x <kls - new - Created
#  V2  960502x <kls - add - ql syntax error
#  V3  960503x <kls - chg - from -14 days to -1 month
#  V4  960520x <ac  - chg - from -1 month to -35 days
#  V5  960613x <kls - new - added exiting with any error status
#                   - new - added today's journal export to jnlexp.txt
#  V6  961104x <kls - chg - daily export criteria from > to ==
#                   - add - program and version output to log file
#  V7  970506x <kls - chg - adjust for sfb
#  V8  970624x <kls - chg - year, user, proj
#   8a 970708x <kls - chg - c:\apps\~
#   8b 970924x <kls - chg - c:\moa\bin
#   9  980421x <kls - chg - to fap3
#  10  980520x <kls - chg - new environment variables
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:40:01 $
#
#
#
$ver = "10";
$prg = "jnlpr(" . $ver . ")";
#
# Set the compression date
$tmpdate = eval(`date_add today -7 day`);
$yesterday =   eval(`date_add today -1  day`);

$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};
$temp = $ENV{MOACURPROJ_MAINT};
if (length($temp) != 0) {
   $proj = $temp;
} else {
   $proj = "fap3";
} 

&Stamp ();
print "${prg} Journal Extract and Compress \n" ;
print "${prg} ${Where} - ${Who} ${User} at ${Event}\n" ;
print "${prg} Extraction for ${yesterday} and Compression for ${tmpdate}\n" ;

$Jfile = "jnlexp.txt";                    #journal output file name
#
# Delete files
# 
if ( -f "/dlld/${Jfile}") { unlink "/dlld/${Jfile}"; }
if ( -f "/dlld/jnlpr.run") { unlink "/dlld/jnlpr.run"; }
if ( -f "/dlld/jnlpr.ql") { unlink "/dlld/jnlpr.ql"; }

#
# Create jnlpr.sig file  (the run from file)
#
open (TMPFILE, ">/dlld/jnlpr.run");
print TMPFILE "run from c:\\dlld\\jnlpr.ql;\n";
print TMPFILE "quit;\n";
close (TMPFILE);

#
# Create jnlpr.ql file (ql command file)
#

open (TMPFILE, ">/dlld/jnlpr.ql");

print TMPFILE "select from journal where tlr:jnl_date ==";
print TMPFILE " $yesterday";
print TMPFILE " dump into c:\\dlld\\";
print TMPFILE "${Jfile};\n";

print TMPFILE "compress journal retain where (tlr:jnl_bus_date >";
print TMPFILE " $tmpdate";
print TMPFILE " || hcomm:tran_status != 1);\n";

print TMPFILE "compress na_journal retain where (na:na_system_date >";
print TMPFILE " $tmpdate";
print TMPFILE " || hcomm:tran_status != 1);\n";

# 
# for compressing cif table records
#

print TMPFILE "compress cif retain where na:cif_num == 0;\n";

print TMPFILE "quit;\n";
close (TMPFILE);

#
# Compressing the records 
#

$out = "c:/dlld/jnlpr.out";
$cmd = "ql -n$proj < c:/dlld/jnlpr.run";
$RET = system("${cmd} > ${out} 2>&1");
open (OUT,"$out");
if ($RET > 0) {
   print "$prg ERROR: journal extract and compress =  ${RET}";
   while (<OUT>) { print "? $_"; }
} else {
   while (<OUT>) { print "- $_"; }
}
close(OUT);

if ( -f "/dlld/jnlpr.out") { unlink "/dlld/jnlpr.out"; }
if ( -f "/dlld/jnlpr.run") { unlink "/dlld/jnlpr.run"; }
if ( -f "/dlld/jnlpr.ql") { unlink "/dlld/jnlpr.ql"; }

exit($RET);


#
# Stamp sub-routine - sets date and time
#
sub Stamp {
    ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
    $year = 1900 + $year;
    $ampm = "a";
    $mon=$mon+1;    
    $mon="0" . $mon if $mon <= 9;
    $mday="0" . $mday if $mday <= 9;
    $ampm = "p" if $hour == 12;
    $hour = 12 if $hour < 1;
    if ($hour > 12) { 
        $hour = $hour - 12; 
        $ampm = "p";
    }
    $hour="0" . $hour if $hour <= 9;
    $min="0" . $min if $min <= 9;
    $Event = "${year}/${mon}/${mday} \@${hour}:${min}${ampm}";
}
# ****************
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl



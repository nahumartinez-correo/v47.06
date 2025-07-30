@rem = 'Perl Script for NT
@echo off
set PERLSCRIPT=%0
set PERLARGS=
:GETARG
set PERLARGS=%PERLARGS% %1
shift 
if not "%1"=="" goto GETARG
goto ENDOFPERL
';

#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 21:51:51 $
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

$| = 1;
#
# Misc variables
#
# V1 960614x <kls - new - created
# V2 970220x <kls - chg - adjusted for sfb
# V3 970624x <kls - chg - year, user, 
# V4 970708x <kls - chg - c:\apps\~   
# V5 970924x <kls - chg - c:\moa\bin  
# V6 980421x <kls - chg - to fap3  
# V7 980520x <kls - chg - new environment variables

$ver = 7;
$pgm = "EdtMoa(" . $ver . ")";
$Version="980520x <kls";
$VERBOSE=0;
$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};
$temp = $ENV{MOACURPROJ_MAINT};
if (length($temp) != 0) {
   $proj = $temp;
} else {
   $proj = "fap3";
}
$temp = $ENV{MOAPROJ_MAINT};
if (length($temp) != 0) {
   $moaproj = $temp;
} else {
   $moaproj = "c:\\moaproj";
}

$Infile = $moaproj . "\\" . $proj . "\\moa.edt";
$inifile = $moaproj . "\\" . $proj . "\\ddproj";
$cds_primary = "";
$copy_journal2 = "";
$copy_errors2 = "";
$newdd2 = "";
$SectionFound = 0;
$Found = 0;

#
#
#
&Stamp();
print "${pgm} Edit Mosaic ${Where} - ${Who} ${User} at $Event\n";

$errors = 0;
if ( -f "${Infile}") {
   print "\n${pgm} Reading configuration file ${Infile} found\n";
   open(AT,"${Infile}");
   while (<AT>) {            
      chop($_);
      $cnt = index($_,"CDS_PRIMARY_NP=");
      if ($cnt >= 0 ) { 
         $cds_primary = $_;
         print "Found ${cds_primary}\n";
      }
      $cnt = index($_,"copy_journal2=");
      if ($cnt >= 0 ) { 
         $copy_journal2 = $_;
         print "Found ${copy_journal2}\n";
      }
      $cnt = index($_,"copy_errors2=");
      if ($cnt >= 0 ) { 
         $copy_errors2 = $_;
         print "Found ${copy_errors2}\n";
      }
      $cnt = index($_,"newdd2=");
      if ($cnt >= 0 ) { 
         $newdd2 = $_;
         print "Found ${newdd2}\n";
      }
   }
   close(AT);
} else {
   print "\nWARNING: ${pgm} ddproj.ini configuration file ${Infile} NOT found\n\n";
}
if (length($cds_primary) != 0)  {
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "\nIs ${cds_primary} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $cds_primary = ""; }
}
while (length($cds_primary) == 0)  {
   print "${pgm} Enter CDS_PRIMARY_NP name: ";
   chop($cds_primary = <STDIN>);
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "Is CDS_PRIMARY_NP = ${cds_primary} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $cds_primary = ""; }
   if ($ans eq "y") { $cds_primary = "CDS_PRIMARY_NP=${cds_primary}"; }
}
if (length($copy_journal2) != 0)  {
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "\nIs ${copy_journal2} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $copy_journal2 = ""; }
}
while (length($copy_journal2) == 0)  {
   print "${pgm} Enter copy_journal2 name: ";
   chop($copy_journal2 = <STDIN>);
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "Is copy_journal2 = ${copy_journal2} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $copy_journal2 = ""; }
   if ($ans eq "y") { $copy_journal2 = "copy_journal2=${copy_journal2}"; }
}
if (length($copy_errors2) != 0)  {
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "\nIs ${copy_errors2} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $copy_errors2 = ""; }
}
while (length($copy_errors2) == 0)  {
   print "${pgm} Enter copy_errors2 name: ";
   chop($copy_errors2 = <STDIN>);
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "Is copy_errors2 = ${copy_errors2} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $copy_errors2 = ""; }
   if ($ans eq "y") { $copy_errors2 = "copy_errors2=${copy_errors2}"; }
}
if (length($newdd2) != 0)  {
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "\nIs ${newdd2} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $newdd2 = ""; }
}
while (length($newdd2) == 0)  {
   print "${pgm} Enter copy_newdd2 name: ";
   chop($newdd2 = <STDIN>);
   $ans = "a";
   while ($ans ne "y" && $ans ne "n") {
      print "Is newdd2 = ${newdd2} correct? ";
      chop($ans = <STDIN>);
      $ans =~ y/[A-Z]/[a-z]/;
   }
   if ($ans eq "n") { $newdd2 = ""; }
   if ($ans eq "y") { $newdd2 = "newdd2=${newdd2}"; }
}

if (! -f "${inifile}.ini") {
   print "Configuration file ${inifile}.ini NOT found";
   exit 2;
}
unlink("${inifile}.old");
rename("${inifile}.ini","${inifile}.old");

print "\n${pgm} Updating configuration file ${Infile}\n\n";
open(INF,">${Infile}");
print INF "$cds_primary\n";
print INF "$copy_journal2\n";
print INF "$copy_errors2\n";
print INF "$newdd2\n";
close(INF);


print "\n${pgm} Updating configuration file ${inifile}.ini\n";
open(NEW,">${inifile}.ini");
open(OLD,"${inifile}.old");
while(<OLD>) {
      $cnt = index($_,"CDS_PRIMARY_NP=");
      if ($cnt >= 0 ) { 
         print NEW "$cds_primary\n";
         print "Added $cds_primary\n";
         next;
      }
      $cnt = index($_,"copy_journal2=");
      if ($cnt >= 0 ) { 
         print NEW "$copy_journal2\n";
         print "Added $copy_journal2\n";
         next;
      }
      $cnt = index($_,"copy_errors2=");
      if ($cnt >= 0 ) { 
         print NEW "$copy_errors2\n";
         print "Added $copy_errors2\n";
         next;
      }
      $cnt = index($_,"newdd2=");
      if ($cnt >= 0 ) { 
         print NEW "$newdd2\n";
         print "Added $newdd2\n";
         next;
      }
      print NEW "$_";
}
close(OLD);
close(NEW);

#
#  Don't remove these last two lines - they're needed for NT!
#
__END__
:endofperl
perl %MOADIR_MAINT%\bin\edtmoa.cmd %PERLARGS%


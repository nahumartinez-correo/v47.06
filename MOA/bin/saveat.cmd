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
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:40:44 $
#
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
# V4  980520x <kls chg - new environment variables
# V3  980421x <kls chg - to fap3 
# V2c 970924x <kls chg - c:\moa\bin 
# V2b 970708x <kls chg - c:\apps\~  
# V2a 970624x <kls chg - log file names at_x.log where x = {1,2,3}
#                  new - add user to messages
#
$ver = "4";
$prg = "saveat(" . $ver . ")";
$Version="980520x <kls";
$VERBOSE=0;
$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};

if ( ! -d "/dlld") {
   print STDERR "$prg ERROR: * * * Directory \\dlld NOT FOUND * * *\n";
   exit;
}
#
# Test at_1.log 
#
$Size = -s "/dlld/at_1.log";
if ( $Size >= 20000 ) {                       #rotate logs around
   unlink("/dlld/at_3.log");
   rename("/dlld/at_2.log","/dlld/at_3.log"); 
   unlink("/dlld/at_2.log");
   rename("/dlld/at_1.log","/dlld/at_2.log"); 
   &Stamp ();
   open (NEW,"> /dlld/at_1.log");             # start a new log file
   print NEW "**********\n";
   print NEW "$prg Created on ${Where} - ${Who} ${User} at ${Event}\n";
   print NEW "**********\n\n";
   close(NEW);
}

$file = shift;
if (length($file) == 0) { $file = "/dlld/at1.out"; }

$aa = substr($file,0,1);
if ($aa eq "-") {
    $file = substr($file,1);
    if ( -f $file) {
       print STDOUT "- See output file $file -\n";
       open(FIL,$file);
       while (<FIL>) {                              # display file 
          print STDOUT "- $_";
       }
       close(FIL);
    } else {
       print STDOUT "$prg ERROR: ${file} NOT FOUND \n";
    }
    print STDOUT "\n";
    exit;
}

open (OUT,">> /dlld/at_1.log");
&Stamp ();
print OUT "**********\n";
print OUT "$prg Appended on ${Where} - ${Who} ${User} at ${Event}\n";
print OUT "$prg Output file $file \n\n";
if ( -f $file) {
   open(AT,$file);
   while (<AT>) {                              # append file to log
      print OUT "+ $_";
   }
   close(AT);
} else {
   print OUT "$prg ERROR: * * * ${file} NOT FOUND * * *\n";
   print OUT "**********\n\n";
}
close(OUT);

#
#  Don't remove these last two lines - they're needed for NT!
#
__END__
:endofperl
perl %MOADIR_MAINT%\bin\saveat.cmd %PERLARGS%


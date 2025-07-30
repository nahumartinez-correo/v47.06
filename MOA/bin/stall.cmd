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
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:41:06 $
#
#
#************************************************************************
#*                              stall                                   *
#************************************************************************
# 960621x Keith.s V2 - add if 1 sec then date/time stamp
#                    - chg date/time format
# 960624x Keith.s V3 - chg time/stamp literal Time:Stamp for ck4err search
# 970306x Keith.s V4 - chg allow * and / on command line
# 970509x Keith.s V5 - add sleep for performance improvment
# 970624x <kls   V5a - year, user, time improvements
# 970708x <kls   V5b - c:\apps\~   
# 970924x <kls   V5c - c:\moa\bin  
#                    - add - -x to time stamp
# 980421x <kls   V6  - to fap3  
# 980421x <kls   V7  - new environment variables
$|;

$ver = "7";

$x1 = time;
$seconds = shift;
$aa = substr($seconds,0,2);
if ($aa eq "-x") {
   $aa = "";
   for ($i = 0; $i <= 20; $i++) {
       $temp = shift;
       if (length($temp) == 0) { next; }
       $aa = $aa . " " . $temp; 
   }
   &Stamp();
   print STDERR "${Event} - ${aa}\n";
   exit;
}
if (length($seconds) > 0) { 
    $i = index($seconds,"*");
    if ($i > 0) {
       $save = $seconds . "=";
       $left = substr($seconds,0,$i);
       $left = $left + 0;
       $rest = substr($seconds,$i+1);
       $rest = $rest + 0;
       $seconds = $left * $rest;
    }
    $i = index($seconds,"/");
    if ($i > 0) {
       $save = $seconds . "=";
       $left = substr($seconds,0,$i);
       $left = $left + 0;
       $rest = substr($seconds,$i+1);
       $rest = $rest + 0;
       $seconds = $left / $rest;
    }
} else {
    print STDOUT "Stall(${ver}): Seconds NOT set - Reset to 60\n";
    $seconds = 60;
}

if ($seconds < 1) { 
      print STDOUT "Stall(${ver}): Seconds $save$seconds < 1 - Reset to 60\n";
      $seconds = 60; 
      }
if ($seconds > 600) { 
      print STDOUT "Stall(${ver}): Seconds $save$seconds > 600 - Reset to 600\n";
      $seconds = 600; 
      }

&Stamp();
if ($seconds == 1) {
   print STDOUT "Stall(${ver}): Date & Time:Stamp of ${Date} \@${Time}\n";
   exit;
}
print STDOUT "Stall(${ver}): ${Date} \@${Time} wait for ";
print STDOUT "${seconds} seconds";

if ($seconds > 10) { sleep($seconds - 5); }
while ( $seconds > 0 ) {
   $x2 = time;
   if ( $x2 - $x1 >= $seconds) { $seconds = 0; }
}
&Stamp();
$Rr = $x2 - $x1;
print STDOUT " (${Rr}) ${Time}\n\n";
#print STDOUT "${x1} and ${x2}\n";


#
# Stamp sub-routine - sets date and time
#
sub Stamp {
    ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
    #print ("$sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst\n");
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
    $hour = "0" . $hour if $hour <= 9;
    $min =  "0" . $min  if $min  <= 9;
    $sec =  "0" . $sec  if $sec  <= 9;
    $Event = "${year}/${mon}/${mday} \@${hour}:${min}:${sec} ${ampm}m";
    $Date = "${year}/${mon}/${mday}";
    $Time = "${hour}:${min}:${sec} ${ampm}";
}


#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:ENDOFPERL
perl %MOADIR_MAINT%\bin\stall.cmd %PERLARGS%


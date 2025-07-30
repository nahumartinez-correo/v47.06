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
#$Date: 1998/10/01 22:39:19 $
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

sub Logit {
   &Stamp ();
   open (LOG,">> /dlld/at_1.log");
   print LOG "**********\n";
   print LOG "$prg Checking for errors  \n";
   print LOG "$prg ${Where} - ${Who} ${User} at ${Event} \n";
   print LOG "$prg * * * @_\n";
   print LOG "**********\n\n";
   close(LOG);
}
$| = 1;
# V3 960625x <kls - add - backuplog to notification file and at1.log
#                 - add - delete old .ok and .err files
# V4 960626x <kls - add - backuplog size to notification file
#                 - chg - max 25 lines of backuplog to notification file
# V5 970220x <kls - chg - adjust for sigb
# V6 970506x <kls - chg - adjust for sfb
# V7 970520x <kls - chg - adjust for project
# V8 970521x <kls - chg - default log file is c:\\dlld\\bup.log
#                   add - user to message
#                   add - total number of lines of the out file
# V8a970624x <kls - chg - user to all messages
#                   chg - log files to at_x.log where x = {1,2,3}
#                   new - $proj for project at top of program
#                   new - add file to check on the cmd line
#  8b970708x <kls - chg - c:\apps\~
#  8c970924x <kls - chg - c:\moa\bin
#  9 980421x <kls - chg - to fap3 
# 10 980520x <kls - chg - new environment variables 
# 11 980605x <kls - add - use of the logevent.exe   
$ver = "11";
#
# Misc variables
#
$ini_txt = "Project Administration";
$prg = "ck4err(" . $ver . ")";
$VERBOSE = 0;
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

$dir = "\\dlld";

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
                                          #remove old notification files
unlink("\\dlld\\${Who}.err") if (-f "\\dlld\\${Who}.err");
unlink("\\dlld\\${Who}.ok") if (-f "\\dlld\\${Who}.ok");

$temp = shift;                         #get file to ck4err
if (length($temp) > 0) {
    $ck4 = $temp;
} else {
    $ck4 = "c:\\dlld\\at1.out";
}
if ($ck4 eq "-") { $ck4 = "c:\\dlld\\at1.out"; }

$temp = shift;                         #get file - backup file name 
if (length($temp) > 0) {
    $bkup = $temp;
} else {
    $bkup = "c:\\dlld\\bup.log";
}

$When = " ??????? ";
$What = " ??????? ";
$errors = 0;
if ( -f "$ck4") {
   open(AT,"$ck4");
   $t_lines = 0;
   while (<AT>) {            
      $t_lines++;
      $cnt = index($_,"Program is");
      if ($cnt >= 0 ) { 
         $What = substr($_,$cnt + 10);
         chop($What);
         $i = index($_,"Nightly");
         if ($i >= 0) {
            $XXX = "Nightly";
         } else {
            $XXX = "Weekly";
         }
         next;
      }
      $cnt = index($_,"Time:Stamp");
      if ($cnt >= 0 ) { 
         $When = substr($_,$cnt + 13);
         chop($When);
         next;
      }
      $cnt = index($_,"ERROR:");
      if ($cnt >= 0 ) { 
         $errors++;                         #count errors
         @List[$errors] = $_;               #keep errors
      }
   }
   close(AT);
} else {
   &Logit ("$ck4 NOT FOUND * * *");
   exit 2;
}

$Line = "$XXX at $When with command $What\n";

if ($errors > 0) {
   $file = "\\dlld\\${Who}.err";
} else {
   $file = "\\dlld\\${Who}.ok";
}

&Stamp();
open(NOT,">$file");
print NOT "********** Notification file $file\n";
print NOT " Checking for errors on ${Where} - ${Who} ${User} at ${Event}\n";
print NOT " $prg ${Line}";
print NOT " $prg ${errors} error(s) found out of ${t_lines} lines\n";
for ($i = 1; $i <= $errors; $i++) {
    print NOT "* * * ${i}. @List[$i]";
}
#&getbkup();
$size = -s "${bkup}";
if (${size} > 0) {
  open(XXX,"$bkup");
  print NOT "\n\n BACKUP file ${bkup} with ${size} characters\n\n";
  $j = 0;
  for ($i = 1; $i <= 25; $i++) {
      $Line = <XXX>; 
      if (length($Line) > 0) {
          print NOT "${i}. $Line"; 
          $j++;
      }
  }
  while (<XXX>) { $j++; }
  if ($j > 25) {
      print NOT "\n\n ONLY 25 lines of the BACKUPs $j lines are listed here!!!";
  }
  close(XXX);
  print NOT "\n\n";
} else {
  print NOT " BACKUP file <${bkup}> not found\n";
}
print NOT "**********\n\n";
close(NOT);
system ("type $file >> \\dlld\\at_1.log");

if ($errors > 0) {
    open(XX,">>$file");
    print XX "********** ERRORS in $ck4 file\n\n";
    open(AT,"$ck4");
    while (<AT>) {            
       print XX "? $_";
    }
    close(AT);
    close(XX);
}

$inifile = "$moaproj\\$proj\\ddproj.ini";
$variable = "copy_errors2";
$SectionFound = 0;
$Found = 0;

if (! -f $inifile) {
   &Logit ("ERROR: Config. file $inifile not found.");
   exit 2;
}
open(INI,$inifile);
while(<INI>) {
   if ($Found) { next; }
   if (index($_,"$ini_txt") >= 0) {
      $SectionFound = 1;
      next;
   }
   if ($SectionFound && /^$variable\s*=\s*(.*)/) {
      $Found=$1;
      last;
   }
}
close(INI);

if (!$SectionFound) {
   &LogEvent;
   exit;
}
if (!$Found) {
   &LogEvent;
   exit;
}
    
 $cmd = "copy " . $file . "  " . $Found;
 $cmd =~ s/A-Z/a-z/g;
#&Logit ("$cmd");

 $RET = system("${cmd} > \\dlld\\ck4err.out 2>&1");
 &Stamp ();
 open (OUT,">> /dlld/at_1.log");
 print OUT "********** Notification\n";
 print OUT " Checking for errors on ${Where} - ${Who} ${User} at ${Event}\n";
 print OUT " Copy Notification: ${cmd}\n";
 if ($RET > 0) {
    print OUT " $prg ERROR: Copy Notification return = ${RET}\n";
    open(EXT,"\\dlld\\ck4err.out");
    while(<EXT>) { print OUT " * * * $_"; }
    close(EXT);
 } else {
    print OUT " $prg Copy Notification Successful\n";
}
unlink("\\dlld\\ck4err.out");
print OUT "**********\n\n";
close(OUT);

exit($RET);

sub getbkup {
   $bkup = "";
   $dir = '/BKUPEXEC/NT/DATA';
   opendir(DIR,$dir);
   while (length($temp = readdir(DIR)) > 0) {
      if ($temp eq ".") { next; }
      if ($temp eq "..") { next; }
      $FILE = $temp;
      $FILE =~ s/a-z/A-Z/g;
      $cnt = index($FILE,".TXT");
      if ($cnt < 0) { next; }
      $age = -M "$dir\\$temp";
      $size = -s "$dir\\$temp";
      if ($age < 0.5) { 
         $bkup = $temp;
         closedir(DIR);
         return;
      }
   }
   closedir(DIR);
}
sub LogEvent {
   if ($errors > 0) {
      if ($errors > 1) { $temp = "were"; } else { $temp = "was"; }
      $msg = "CDS Nightly at $Who: There $temp $errors errors";
      $opt_e = "-ee";
      $opt_i = "-i9801";
   } else {
      $msg = "CDS Nightly at $Who: There were NO errors";
      $opt_e = "-ei";
      $opt_i = "-i9800";
   }
   $cmd = "logevent -m\"$msg\" $opt_e -cb $opt_i -sCk4Err.cmd -k$proj";
   $RET = system("${cmd} > \\dlld\\ck4err.out 2>&1");

   &Stamp ();
   open (OUT,">> /dlld/at_1.log");
   print OUT "********** Notification\n";
   print OUT " Checking for errors on ${Where} - ${Who} ${User} at ${Event}\n";
   print OUT " Event Log Notification: ${msg}\n";
   if ($RET > 0) {
      print OUT " $prg ERROR: Notification return = ${RET}\n";
      print OUT " Command: ${cmd}\n";
      open(EXT,"\\dlld\\ck4err.out");
      while(<EXT>) { print OUT " * * * $_"; }
      close(EXT);
   } else {
      print OUT " $prg Event Log Notification Successful\n";
  }
  unlink("\\dlld\\ck4err.out");
  print OUT "**********\n\n";
  close(OUT);
}
#
#  Don't remove these last two lines - they're needed for NT!
#
__END__
:endofperl
perl %MOADIR_MAINT%\bin\ck4err.cmd %PERLARGS%


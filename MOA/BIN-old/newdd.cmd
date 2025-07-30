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
#$Date: 1998/10/01 22:33:42 $
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
   local($log) = @_;
   $x = index($log,"XXX");
   if ($x >= 0) {
      &Stamp ();
      $log = substr($log,0,$x-1) . "                                     ";
      $log = substr($log,0,30) . " ${Where} - ${Who} ${User} at ${Event}";
      $log = $prg . " " . $log;
   }
   open (OUT,">> /dlld/at_1.log");
   print OUT "$log\n" if (length($log) > 0);
   close(OUT);
   
}

sub wt4 {
   local($seconds) = @_;
   if (length($seconds) <= 0) { $seconds = 10; }
   $x1 =time;
   $cnt = 0;
   while ( $seconds > 0 ) {
      $x2 = time;
      if ( $x2 - $x1 >= $seconds) { $seconds = 0; }
   }
   $Rr = $x2 - $x1;
   #print STDOUT " ${x2} - ${x1} = $Rr\n";

}
$| = 1;
#  "New DD on ${Where} - ${Who} ${User} at ${Event}\n";
# Change $proj = -ncfbd and CitNet 2000
#
# V1 960812x <kls - new - created from ck4err.cmd(4)
# V2 970507x <kls - chg - use cdsstat for the wait for loop
# 2a 970624x <kls - chg - 1900 + year
#                   new - add user to messages
# 2b 970708x <kls - chg - c:\apps\~  
# 2c 970924x <kls - chg - c:\moa\bin 
# V3 980421x <kls - chg - to fap3 
# V4 980520x <kls - chg - new environment variables
#                
$ver = "4";
#
# Misc variables
#
$prg = "newdd(" . $ver . ")";
$VERBOSE=0;
$proj = "fap3";
$ini_txt = "Project Administration";
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
$temp = $ENV{MOADIR_MAINT};
if (length($temp) != 0) {
   $moadir = $temp;
} else {
   $moadir = "c:\\moa\\bin";
}


$WorkIn = "\\dlld";
$WorkOut = "$moaproj\\$proj";
$WorkTemp = "\\dlld\\newdd.out";

if (! -d "${WorkIn}") { 
   system("mkdir ${WorkIn} > nul 2>&1");
   &Logit("Creating directory dlld XXX");
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
   open (NEW,"> /dlld/at_1.log");               # start a new log file
   print NEW "**********\n";
   print NEW "$prg Created on ${Where} - ${Who} ${User} at ${Event}\n";
   print NEW "**********\n\n";
   close(NEW);
}
&Logit("\n**********");
&Logit("Starting New DD XXX");
#
# Make system directories
#
if (! -d "${WorkOut}\\ddnew") { 
   &Logit("Creating directory ddnew");
   system("mkdir ${WorkOut}\\DDnew > ${WorkTemp} 2>&1");
   &LogTemp();
}
if (! -d "${WorkOut}\\ddold") { 
   &Logit("Creating directory ddold");
   system("mkdir ${WorkOut}\\DDold > ${WorkTemp} 2>&1");
   &LogTemp();
}
#
# Count number of DD parts
#
$found = 0;
$found++ if ( -f "${WorkIn}\\ddobj.idx");
$found++ if ( -f "${WorkIn}\\ddobj.dat");
$found++ if ( -f "${WorkIn}\\ddobj.qck");
#
# 
#
if ($found == 0) {                             # no data dictionary
   &Logit("NO New DD");
   &Notify("$prg NO New DD on ");
   &Logit("Ending   New DD XXX");
   &Logit("**********");
   unlink("${WorkTemp}");
   exit(0);
}
if ($found == 3) {                             # data dictionary
   &Logit("Copy New DD files to ddnew");
   &DoIt();
   if ($good == 0) {
      &Notify ("$prg New DD successful on ");
   } else {
      &Notify ("$prg ERROR: $temp failed on ");
   }
   &Logit("Ending   New DD XXX");
   &Logit("**********");
   unlink("${WorkTemp}");
   exit(0);
} else {                                       # partial DD
   &Logit("Partial New DD");
   &Logit("ERROR: Only $found of 3 parts found");
   &Notify("$prg ERROR: Partial New DD ");
   &Logit("Ending   New DD XXX");
   &Logit("**********");
   unlink("${WorkTemp}");
   exit(2);
}
exit(0);


#---------------------------------------------------------------------------
sub Notify {                            # copy notification to remote site

    local($note) = @_;
    if (length($note) <= 0) {
       &Logit("ERROR: Notification string missing.");
       &Logit("Notification ABORTED!!");
       return 2;
    }
    &Stamp ();
    $note = $note . "${Where} - ${Who} ${User} at ${Event}\n";

    $file = "\\dlld\\${Who}.ndd";
    $file =~ tr/A-Z/a-z/;

    &Logit("Notification file: $file");
    $tmp = $note;
    chop($tmp);
    &Logit("Note: $tmp");

    open(NFY,">$file");
    print NFY "$note\n";
    close(NFY);

    $inifile = "$moaproj\\$proj\\ddproj.ini";
    $variable = "new_dd2";
    $SectionFound = 0;
    $Found = 0;
    
    if (! -f $inifile) {
       &Logit("ERROR: Config. file $inifile not found.");
       return 2;
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
       &Logit("ERROR: Section \"$ini_txt\" not in $inifile.");
       &Logit("Notification ABORTED!!");
       return 2;
    }
    if (!$Found) {
       &Logit("ERROR: Variable $variable not found in $inifile.");
       &Logit("Notification ABORTED!!");
       return 2;
    }
     &Logit("Notify New DD status XXX");
     &Logit("Send $file to $Found");
    
     $cmd = "copy " . $file . "  " . $Found;
     $cmd =~ s/A-Z/a-z/g;
     $RET = system("${cmd} > $WorkTemp 2>&1");

     if ($RET > 0) {
        &Logit("ERROR: Copy Notification return = ${RET}");
        &LogTemp ();
     } else {
        &Logit("Copy Notification Successful");
    }
    unlink($file);
}    
#---------------------------------------------------------------------------
sub DoIt {                         # Do install
   $good = 0;
   system("copy ${WorkIn}\\ddobj.* ${WorkOut}\\ddnew > ${WorkTemp} 2>&1 ");

   if ( -f "${WorkOut}\\ddnew\\before.bat") { 
   system("del ${WorkOut}\\ddnew\\before.bat > nul 2>&1 ");
   }
   if ( -f "${WorkIn}\\ddnew\\before.bat") { 
   system("copy ${WorkIn}\\ddnew\\before.bat ${WorkOut}\\ddnew > ${WorkTemp} 2>&1 ");
   }
   if ( -f "${WorkOut}\\ddnew\\during.bat") { 
   system("del ${WorkOut}\\ddnew\\during.bat > nul 2>&1 ");
   }
   if ( -f "${WorkIn}\\during.bat") { 
   system("copy ${WorkIn}\\during.bat ${WorkOut}\\ddnew > ${WorkTemp} 2>&1 ");
   }
   if ( -f "${WorkOut}\\ddnew\\after.bat") { 
   system("del ${WorkOut}\\ddnew\\after.bat > nul 2>&1 ");
   }
   if ( -f "${WorkIn}\\after.bat") { 
   system("copy ${WorkIn}\\after.bat ${WorkOut}\\ddnew > ${WorkTemp} 2>&1 ");
   }
#
#
   $temp = "PreInstall script";

   if ( -f "${WorkIn}\\before.bat") {
      &Logit("$temp found");
      $RET = system("${WorkIn}\\before.bat > $WorkTemp 2>&1");
      if ($RET != 0) {
         &Logit("ERROR: $temp failed!!");
         $good = 1;
      }
      &LogTemp ();
      unlink("${WorkIn}\\before.bat"); 
   } else {
      &Logit("$temp NOT found");
   }
   if ($good > 0) { return; }
#
#
   $temp = "Shutdown CDS $proj";

   &Logit("$temp");
   $RET = system("cdinstsv $proj stop > $WorkTemp 2>&1");
   if ($RET != 0) {
      &Logit("ERROR: $temp failed!!");
      &LogTemp ();
      $good = 2;
      return;
   }
#
#
   $temp = "Copy current DD to ddold";

   &Logit("$temp");
   $RET = system("copy ${WorkOut}\\DDobj.* ${WorkOut}\\ddold > $WorkTemp 2>&1");
   if ($RET != 0) {
      &Logit("ERROR: $temp failed!!");
      &LogTemp ();
      $good = 3;
      return;
   }
#
#
   $temp = "Copy new DD";

   &Logit("$temp");
   $RET = system("copy ${WorkIn}\\DDobj.* ${WorkOut} > $WorkTemp 2>&1");
   if ($RET != 0) {
      &Logit("ERROR: $temp failed!!");
      &LogTemp ();
      $good = 4;
      return;
   }
   unlink("${WorkIn}\\DDobj.idx");
   unlink("${WorkIn}\\DDobj.dat");
   unlink("${WorkIn}\\DDobj.qck");
#
#
   $temp = "Install script";

   if ( -f "${WorkIn}\\during.bat") {
      &Logit("$temp found");
      $RET = system("${WorkIn}\\during.bat > $WorkTemp 2>&1");
      if ($RET != 0) {
         &Logit("ERROR: $temp failed!!");
         $good = 6;
      }
      &LogTemp ();
      unlink("${WorkIn}\\during.bat");
   } else {
      &Logit("$temp NOT found");
   }
   if ($good > 0) { return; }
#
#
   $temp = "Startup CDS $proj";

   &Logit("$temp");
   $RET = system("cdinstsv $proj start > $WorkTemp 2>&1");
   if ($RET != 0) {
      &Logit("ERROR: $temp failed!!");
      &LogTemp ();
      $good = 5;
      return;
   }
#
#
   &Logit("Wait for CDS to come up");
   for ($i = 1; $i < 20; $i++) {
       &wt4(120);
       &Logit("CDS wait loop $i");
       $RET = system("cdsstat $proj > $WorkTemp 2>&1");
       if ($RET == 0) { $i = 20; }
   }
#
#
   $temp = "PostInstall script";

   if ( -f "${WorkIn}\\after.bat") {
      &Logit("$temp found");
      $RET = system("${WorkIn}\\after.bat > $WorkTemp 2>&1");
      if ($RET != 0) {
         &Logit("ERROR: $temp failed!!");
         $good = 6;
      }
      &LogTemp ();
      unlink("${WorkIn}\\after.bat");
   } else {
      &Logit("$temp NOT found");
   }
}
#--------------------------------------------------------------------
sub LogTemp {
   open(OUT,">>\\dlld\\at_1.log");
   open(EXT,$WorkTemp);
   while(<EXT>) { print OUT " * * $_"; }
   close(EXT);
   close(OUT);
   unlink($WorkTemp);
}
#
#  Don't remove these last two lines - they're needed for NT!
#
__END__
:endofperl
perl %MOADIR_MAINT%\bin\newdd.cmd %PERLARGS%
rem if errorlevel 1 echo Perl ERROR

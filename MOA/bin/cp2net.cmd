@rem = 'Perl Script for NT - Copy file to a remote machine
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
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 21:50:53 $
#
#------------------------------------------------------------------------
#
# Logging sub-routine
#
sub Lprint {
    $LINE="@_";
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
    $sec="0" . $sec if $sec <= 9;
    $Event = "${year}/${mon}/${mday} \@${hour}:${min}:${sec}${ampm}";
    if (length($LINE) != 0) {
       $Output = "${Event} - ${LINE}\n";
    } else {
       $Output = "                 -\n";
    }
    $i = index($LINE,"==========");
    if ($i >= 0) { $Output = $LINE; }
    if (length($Output) > 77) { $Output = substr($Output,0,77) . "\n" }
    open(FILE,">>${Merge}\\cp2net_1.log");
    print FILE "$Output";
    close(FILE);
    print STDOUT "$Output" if $VERBOSE;
}
$|;
#------------------------------------------------------------------------
sub debug { print STDERR "$prg Debug: @_\n" if $VERBOSE; }

#------------------------------------------------------------------------
# V1  970613x <kls - new - created from cp2jet.cmd
#   a 970708x <kls - chg - c:\apps\~
#   b 970924x <kls - chg - c:\moa\bin
# V2  980421x <kls - chg - fap3
# V3  980520x <kls - chg - new environment variables
#
$ver = "3";
#
# Misc variables
#
$prg = "Cp2Net(" . $ver . ")";
$VERBOSE=0;
$LIST=0;
$Temp="\\temp";
$Merge="\\dlld";
$out = "\\temp\\cp2net.out";
$Sym='\\';

$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};

while ($_ = shift) {
    if (/^-v/) { $VERBOSE="1"; next; }
    if (/^-V/) { $VERBOSE="1"; next; }
    if (/^-l/) { $LIST="1"; next; }
    if (/^-L/) { $LIST="1"; next; }
    if (/^-.*/) { print "Unknown option $_\n";  print "$USAGE";  exit 1; }
    unshift(ARGV,$_);
    last;
}
srand($$);
$ext = sprintf("%03d",rand(1000));

if ( -f "/temp/verbose" ) {
    $VERBOSE = 1;
    &Lprint ("Verbose mode - file parameter found");
}

&Lprint ("                 - ====================================\n");
&Lprint ("$prg Domain: ${Where} Machine: ${Who} User: ${User}");

#
# Start log
#
$Size = -s "${Merge}\\cp2net_1.log";
&Lprint ("Log: File size = $Size");
if ( $Size >= 10000 ) {
   unlink("${Merge}/cp2net_3.log");
   rename("${Merge}/cp2net_2.log","${Merge}/cp2net_3.log"); 
   unlink("${Merge}/cp2net_2.log");
   rename("${Merge}/cp2net_1.log","${Merge}/cp2net_2.log"); 
   unlink("${Merge}/cp2net_1.log");
   &Lprint ("Log: Files rotated");
}

$branch = shift;         #get machine name
$branch =~ tr/a-z/A-Z/;  #upper case
$file = shift;           #get file name
$file =~ tr/a-z/A-Z/;    #upper case
$share = shift;          #get share name
$share =~ tr/a-z/A-Z/;   #upper case

if (length($branch) eq 0 && $LIST == 0) {
    &Lprint ("ERROR: Machine name not set ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
}
if (length($branch) eq 0 && $LIST == 1) {
    &Lprint ("ERROR: List name not set ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
}
if (length($file) eq 0) {
    &Lprint ("ERROR: File name not set ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
}
if (! -f "$file") {
    &Lprint ("ERROR: File $file not found ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
} else {
    &GetDir();
    &Lprint ("File: $file $path $size $date $time ");
}

#
# Get configuration 
#
if ( -f "${Merge}/cp2net.cfg" ) {
   open (CFG,"${Merge}/cp2net.cfg");
   while (<CFG>) {
       chop;
       $Index=index($_,"VERBOSE=");
       if ($Index >= 0) {
           $VERBOSE=substr($_,8);
           $VERBOSE=$VERBOSE+0;
           &Lprint ("Verbose mode - on");
       }
   }
   close(CFG);
}

if ($LIST == 0) { &Mach (); } else { &Lister (); }

#
# End Log
#
&Lprint ("$prg End ");
&Lprint ("");


#------------------------------------------------------------------------
sub Mach {
   #
   # Get machine name
   #
   $ret=system("ping -n 1 $branch >nul 2>&1"); 
   if ($ret != 0) {
      $SERVER = "";
      if ( -f "${Merge}/cp2net.dbm" ) {
         open (SER,"${Merge}/cp2net.dbm");
         while ($line = <SER>) {
             $line =~ tr/a-z/A-Z/;
             chop($line);
             if (length($line) < 10) { next; }
             $aa = substr($line,0,1);
             if ($aa eq "#") { next; }
             &Parse();
             if ($branch eq $word[0]) {
                $SERVER = $word[1];
                if (length($share) == 0) {
                   if (length($word[2]) != 0) {
                      $share = $word[2];
                   }
                }
                last;
             }
         } 
         close(SER);
     }
   } else {
      $SERVER = $branch;
   }
   if ($SERVER ne $branch) {
      &Lprint ("Machine: $branch - Changed to $SERVER");
   }
   if (length($SERVER) == 0) {
       &Lprint ("ERROR: $branch name not found in the cp2net.dbm file");
       &Lprint ("EXIT: ");
       &Lprint ("");
       exit "2";
   } 
   if (length($share) eq 0) {
       &Lprint ("ERROR: Share point not set ");
       &Lprint ("EXIT: ");
       &Lprint ("");
       exit "2";
   }

   &CopyIt ();
}
#------------------------------------------------------------------------
sub Lister  {
   if (! -f "$branch") {
       &Lprint ("ERROR: List file $branch not found ");
       &Lprint ("EXIT: ");
       &Lprint ("");
       exit "2";
   } 
   $save = $file;
   $file = $branch;
   &GetDir();
   &Lprint ("List: $file $size $date $time ");
   $file = $save;

   $cnt = 0;
   $cnt_err = 0;
   open(NET,"$branch");
   $tt1 = time;
   while ($line = <NET>) {
       $line =~ tr/a-z/A-Z/;
       chop($line);
       if (length($line) < 10) { next; }
       $aa = substr($line,0,1);
       if ($aa eq "#") { next; }
       &Parse();
       $branch = $word[0];
       $SERVER = $word[1];
       $share = $word[2];
       $cnt++;
       &CopyIt ();
       if ($ok == 1) { $cnt_err++; }
   } 
   $tt2 = time;
   close(NET);
   $temp = "List totals: Sites = $cnt";
   if ($cnt_err > 0) {
      $temp = $temp . " Errors = $cnt_err";
   }
   &Lprint ("$prg $temp");
   $xsec = $tt2 - $tt1;
   $xmin = 0;
   $xhr = 0;
   while ($xsec > 60) { 
      $xmin++;
      $xsec = $xsec - 60;
   }
   while ($xmin > 60) { 
      $xhr++;
      $xmin = $xmin - 60;
   }
   $temp = "List time: ";
   if ($xhr > 0) { $temp = sprintf("%s %d:",$temp,$xhr); }
   $temp = sprintf("%s%02d:%02d",$temp,$xmin,$xsec); 
   &Lprint ("$prg $temp");
}
#------------------------------------------------------------------------
sub CopyIt  {
   $ok = 0;
   &Lprint ("");
   &Lprint ("$prg file $file to $share at $SERVER");

   $ret=system("ping -n 1 $SERVER >nul 2>&1"); 
   if ($ret != 0) {
       &Lprint ("ERROR: Machine $SERVER not responding ");
       $ok = 1;
       return;
   }
   
   $Cmd = "COPY ${file} ${Sym}${Sym}${SERVER}${Sym}${share}";
   $Cmd = ${Cmd} . " > ${out} 2>&1";

   $x1 = time;                         #time command - start
   $ret = system("$Cmd");
   $x2 = time;                         #time command - end

   $xsec = $x2 - $x1;
   $xmin = 0;
   $xhr = 0;
   while ($xsec > 60) { 
      $xmin++;
      $xsec = $xsec - 60;
   }
   while ($xmin > 60) { 
      $xhr++;
      $xmin = $xmin - 60;
   }
   $temp = "Time: ";
   if ($xhr > 0) { $temp = sprintf("%s %d:",$temp,$xhr); }
   $temp = sprintf("%s%02d:%02d",$temp,$xmin,$xsec); 
   
   if ($ret != 0) {
       $ok = 1;
       &Lprint ("ERROR: $temp");
       &Lprint ("${Cmd}");
       open (ERR,"${out}");
       while (<ERR>) {
          chop;
          &Lprint ("? $_");
       }
       close(ERR);
   } else {
       open (TST,"${out}");
       &Lprint ("GOOD: $temp");
       while (<TST>) {
          chop;
          &Lprint ("+ $_");
       }
       close(TST);
   }
   unlink "${out}";
}
#------------------------------------------------------------------------
sub Parse {
   local($i);
   for ($i = 0; $i < 8; $i++) { $word[$i] = ""; }
   $count = 0;
   for ($j = 0; $j <= length($line); $j++) {
      $aa = substr($line,$j,1);
      if ($aa eq " " || $aa eq ";") {
         if ($word[$count] ne "") {
            $count++;
         }
      } else {
         $word[$count] = $word[$count] . $aa;
      }
   }
}
#------------------------------------------------------------------------
sub GetDir {
    $cmd = "dir $file > $out 2>&1";
    #print "$cmd\n";
    $ret = system("$cmd");
    if ($ret != 0) {
        $name = "xxxx";
        return;
    }
    open(TMP,$out);
    
    while (<TMP>) {
        $xx++;
        tr/a-z/A-Z/;
        chop;
        #print "$xx - $_\n";
        $line = $_;
        if (length($line) < 10) { next; }
        $aa = substr($line,0,1);
        if ($aa eq "#") { next; }
        $line =~ tr/A-Z/a-z/;
        $i = index($line,"volume ");
        if ($i >= 0) { next; }
        $i = index($line,"file(s) ");
        if ($i >= 0) { next; }
        $i = index($line,"bytes free");
        if ($i >= 0) { next; }
        $i = index($line,"directory of ");
        if ($i >= 0) { 
           $path = substr($line,$i+15);
           if ($path eq "\\") { $path = ""; }
           next; 
        }
        &Parse();
        $date = $word[0];
        $time = $word[1];
        $size = $word[2];
        $name = $word[3];
        if ($name eq ".") { next; }
        if ($name eq "..") { next; }
        if ($name eq $file) { return; }
    }
    close(TMP);
    #unlink($out);
}
#------------------------------------------------------------------------
#
#  Don't remove these last two lines - they're needed for NT!
#
__END__
:endofperl
perl %MOADIR_MAINT%\bin\cp2net.cmd %PERLARGS%


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
#$Date: 1998/10/01 22:34:31 $
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
    $esec = "\@${hour}:${min}:${sec} ${ampm}m";
    #default line: 19970621@061821a - xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    #if line is blank then blank line
    if (length($LINE) != 0) {
       $Output = "${Event} - ${LINE}\n";
    } else {
       $Output = "\n";
    }
    #if line has 10 ='s then ============= 19970621@061821p ============
    $i = index($LINE,"==========");
    if ($i >= 0) { $Output = "$LINE $Event $LINE\n"; }
    #if line starts with x then just the rest of the line
    if (substr($LINE,0,1) eq "x") { $Output = substr($LINE,1) . "\n"; }
    #if line starts with y then @0818:21a - xxxxxxxxxxxxxxxxxxxxxxxx
    if (substr($LINE,0,1) eq "y") { 
       $Output = $esec . " - " . substr($LINE,1) . "\n"; 
    }
    if (length($Output) > 77) { $Output = substr($Output,0,77) . "\n" }
    open(FILE,">>${work}\\sys_1.log");
    print FILE "$Output";
    close(FILE);
    print STDOUT "$Output" if $VERBOSE;
}

#------------------------------------------------------------------------
sub debug { print STDERR "$prg Debug: @_\n" if $VERBOSE; }

#------------------------------------------------------------------------
# V1  970616x <kls - new - created from cp2net.cmd
#  1a 970708x <kls - chg - c:\apps\~
#  1b 970925x <kls - chg - c:\moa\bin
# V2  980421x <kls - chg - to fap3
# V3  980520x <kls - chg - new environment variables
#
$ver = "3";
#
# Misc variables
#
$xxx1 = time;
$prg = "Sys(" . $ver . ")";
$VERBOSE=0;
$Temp="\\temp";
$work="\\temp";
$out = "${work}\\sys.out";
$cfg = "${work}\\sys.cfg";
$Sym='\\';

$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};

while ($_ = shift) {
    if (/^-v/) { $VERBOSE="1"; next; }
    if (/^-V/) { $VERBOSE="1"; next; }
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

#
# Start log
#
$Size = -s "${work}\\sys_1.log";
if ( $Size >= 10000 ) {
   unlink("${work}/sys_3.log");
   rename("${work}/sys_2.log","${work}/sys_3.log"); 
   unlink("${work}/sys_2.log");
   rename("${work}/sys_1.log","${work}/sys_2.log"); 
   unlink("${work}/sys_1.log");
   &Lprint ("===================");
   &Lprint ("x$prg Domain: ${Where} Machine: ${Who} User: ${User}");
   &Lprint ("xLog: Files rotated");
} else {
   &Lprint ("===================");
   &Lprint ("x$prg Domain: ${Where} Machine: ${Who} User: ${User}");
   &Lprint ("xLog: File size = $Size");
}

$list = shift;           #get list name
$list =~ tr/a-z/A-Z/;    #upper case
$file = shift;           #get file name
$file =~ tr/a-z/A-Z/;    #upper case

if (length($list) eq 0) {
    &Lprint ("yERROR: List name not set ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
}
if (! -f "$list") {
    &Lprint ("yERROR: List file $list not found ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
} else {
    $save = $file;
    $file = $list;
    &GetDir();
    &Lprint ("xList: $file $path $size $date $time ");
    $file = $save;
}
if (length($file) eq 0) {
    &Lprint ("yERROR: File name not set ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
}
if (! -f "$file") {
    &Lprint ("yERROR: File $file not found ");
    &Lprint ("EXIT: ");
    &Lprint ("");
    exit "2";
} else {
    &GetDir();
    &Lprint ("xFile: $file $path $size $date $time ");
    $file_cnt = 0;
    open (FIL,"${file}");
    while (<FIL>) {
       chop;
       if (length($_) <= 1) { next; }
       $file_cnt++;
       $file_a[$file_cnt] = $_;
    }
    close(FIL);
}
&Lprint ("xFile $file has $file_cnt lines ");

#
# Get configuration 
#
if ( -f "${cfg}" ) {
   open (CFG,"${cfg}");
   while (<CFG>) {
       chop;
       $Index=index($_,"VERBOSE=");
       if ($Index >= 0) {
           $VERBOSE=substr($_,8);
           $VERBOSE=$VERBOSE+0;
           &Lprint ("xVerbose mode - on");
       }
   }
   close(CFG);
}
for ($x = 1; $x <= $file_cnt; $x++) {
   &Lprint ("x$x <$file_a[$x]>");
}

$cnt_tot = 0;
$t_tot = 0;
$cnt_err = 0;
&SYS (); 

#
# End Log
#
$xxx2 = time;
$xsec = $xxx2 - $xxx1;
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
$temp =  "Time = ";
if ($xhr > 0) { $temp = sprintf("%s %d:",$temp,$xhr); }
$temp = sprintf("%s%02d:%02d",$temp,$xmin,$xsec); 

&Lprint ("$prg End $temp ");
&Lprint ("");


#------------------------------------------------------------------------
sub SYS  {
   $cnt = 0;
   open(LST,"$list");
   $tt1 = time;
   while ($line = <LST>) {
       $line =~ tr/a-z/A-Z/;
       chop($line);
       $cnt++;
       &Lprint ("");
       &Lprint ("===================");
       $aa = substr($line,0,2);
       if ($aa eq "#~") { 
          $aa = substr($line,2,1);
          $k = index("-ABCDEFGHIJKLMNOPQRSTWXYZ",$aa);
          if ($k > 0 && $k < 27) {
             $inline[$k] = substr($line,4);
             &Lprint ("xList $cnt $line");
             &Lprint ("xSet In list substitution: $aa = $inline[$k]");
          } else {
            &Lprint ("xList $cnt $line");
            &Lprint ("xERROR: In line substitution: $i $aa $inline[$k]");
          }
          next; 
       }
       $aa = substr($line,0,1);
       if ($aa eq "#") {  
           &Lprint ("xList $cnt $line");
           next;
       }
       $Lst = $line;
       &Parse();
       &DoIt ();
       if ($ok != 0) { $cnt_err++; }
   } 
   $tt2 = time;
   close(LST);
   &Lprint ("===================");
   $temp = "Totals: List $list lines = $cnt File $file lines = $file_cnt";
   &Lprint ("x$temp");
   $temp = "        Commands = $cnt_tot";
   $temp = $temp . " Errors = $cnt_err";
   #$xsec = $tt2 - $tt1;
   $xsec = $t_tot;
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
   $temp = $temp . " Time = ";
   if ($xhr > 0) { $temp = sprintf("%s %d:",$temp,$xhr); }
   $temp = sprintf("%s%02d:%02d",$temp,$xmin,$xsec); 
   &Lprint ("x$temp");
}
#------------------------------------------------------------------------
sub DoIt  {
   $ok = 0;
   for ($nex = 1; $nex <= $file_cnt; $nex++) {
      &Lprint ("xList $cnt $Lst");
      &Lprint ("xFCmd $nex $file_a[$nex]");
      $cnt_tot++;
      $line = $file_a[$nex];
      $aa = substr($line,0,2);
      if ($aa eq "#~") { 
         $aa = substr($line,2,1);
         $aa =~ tr/a-z/A-Z/;
         $k = index("-ABCDEFGHIJKLMNOPQRSTWXYZ",$aa);
         if ($k > 0 && $k < 27) {
            $inline[$k] = substr($line,4);
            &Lprint ("xSet In file substitution: $aa = $inline[$k]");
            &Lprint ("");
         } else {
            &Lprint ("xERROR: In line substitution: $k $aa $inline[$k]");
            &Lprint ("");
         }
         next; 
      }
      &SetLine();
      &SetInLine();
      &Lprint ("xCmd  $cnt_tot $line");
      $aa = substr($file_a[$nex],0,1);
      if ($aa eq "#") {
          &Lprint ("");
          next;
      }
      unlink "${out}";
      $Cmd = ${line} . " > ${out} 2>&1";

      $x1 = time;                         #time command - start
      $ret = system("$Cmd");
      $x2 = time;                         #time command - end

      $xsec = $x2 - $x1;
      $t_tot = $t_tot + $xsec;
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
      $temp = "Command # $cnt_tot Time: ";
      if ($xhr > 0) { $temp = sprintf("%s %d:",$temp,$xhr); }
      $temp = sprintf("%s%02d:%02d",$temp,$xmin,$xsec); 
   
      if ($ret != 0) {
          $ok = 1;
          &Lprint ("xERROR: $temp");
          open (ERR,"${out}");
          while (<ERR>) {
             chop;
             &Lprint ("x? $_");
          }
          close(ERR);
      } else {
          open (TST,"${out}");
          &Lprint ("xGOOD: $temp");
          while (<TST>) {
             chop;
             &Lprint ("x+ $_");
          }
          close(TST);
      }
      &Lprint ("");
      unlink "${out}";
   }
}
#------------------------------------------------------------------------
sub Parse {
   local($i);
   for ($i = 0; $i < 18; $i++) { $word[$i] = ""; }
   $w_cnt = 0;
   for ($j = 0; $j <= length($line); $j++) {
      $aa = substr($line,$j,1);
      if ($aa eq " " || $aa eq ";") {
         if ($word[$w_cnt] ne "") {
            $w_cnt++;
         }
      } else {
         $word[$w_cnt] = $word[$w_cnt] . $aa;
      }
   }
}
#------------------------------------------------------------------------
sub SetInLine {
   while (index($line,"~") > 0) {
      $j = index($line,"~");
      $aa = substr($line,$j+1,1);
      $aa =~ tr/a-z/A-Z/;
      $k = index("-ABCDEFGHIJKLMNOPQRSTWXYZ",$aa);
      $temp = "";
      $temp = $inline[$k];
      $left = substr($line,0,$j);
      $right = substr($line,$j+2);
      if (length($temp) > 0) {
         $line = $left . $temp . $right;
      } else {
         $line = $left . $right;
      }
   }
}
#------------------------------------------------------------------------
sub SetLine {
   for ($i = 0; $i <= 10; $i++) {
      $j = index($line,"~0");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[0] . $right;
      }
      $j = index($line,"~1");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[1] . $right;
      }
      $j = index($line,"~2");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[2] . $right;
      }
      $j = index($line,"~3");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[3] . $right;
      }
      $j = index($line,"~4");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[4] . $right;
      }
      $j = index($line,"~5");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[5] . $right;
      }
      $j = index($line,"~6");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[6] . $right;
      }
      $j = index($line,"~7");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[7] . $right;
      }
      $j = index($line,"~8");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[8] . $right;
      }
      $j = index($line,"~9");
      if ($j >= 0) {
         $left = substr($line,0,$j);
         $right = substr($line,$j+2);
         $line = $left . $word[9] . $right;
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
    unlink($out);
}
#------------------------------------------------------------------------
#
#  Don't remove these last two lines - they're needed for NT!
#
__END__
:endofperl
perl %MOADIR_MAINT%\bin\sys.cmd %PERLARGS%


@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\cklist.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 21:48:48 $
#
#************************************************************************
#*                                                                      *
#*                 cklist                                               *
#************************************************************************
# V5 980%20x <kls - chg - new environment variables
# V4 980421x <kls - chg - to fap3
# V3 970708x <kls - chg - c:\apps\~
# V2 970613x <kls - add - param checking
# V1 970610x <kls - new - created from mklist.cmd V1
#
$ver = 5;
$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};
$temp = $ENV{MOACURPROJ_MAINT};
if (length($temp) != 0) {
   $proj = $temp;
} else {
   $proj = "fap3";
}

$script = (@script = split(/\\/,$0), pop(@script));
$script =~ s/.cmd$//i;
$prg = $script . "(" . $ver . ")";
$USAGE = "\n$prg: Usage NOT set!\a\n\n";
$usage  = 0;
$wrkdir = "\\temp";
$rel = 0;
$spaces = "                                                    ";
$tab = substr($spaces,0,length($prg));

while ($_ = shift) {
    if (/^-v/) { $verbose="1"; next; }
    if (/^-V/) { $verbose="1"; next; }
    if (/^-r/) { $rel="1"; next; }
    if (/^-R/) { $rel="1"; next; }
    if (/^-:/) { print "$USAGE"; exit 0; }
    if (/^-.*/) { 
       print "$prg Unknown option $_\n";  
       print "$USAGE";  
       exit 1; 
    }
    unshift(ARGV,$_);
    last;
}

$driver = shift;                            #driver list file
$drive  = shift;                            #drive spec
$drive =~ tr/a-z/A-Z/;

if (length($driver) == 0) { $driver = "mklist.lst"; }
if (length($drive) == 0) { $drive = "C:"; }

$out = "${wrkdir}\\cklist.out";
$tmp = "${wrkdir}\\cklist.tmp";

&Stamp();
print STDOUT "$prg Started on ${Where} - ${Who} ${User} at $Event\n";

if ( ! -f $driver ) {
    print STDERR "$prg ERROR: Driver list file <${driver}> NOT found!\a\n";
    exit 2;
} else {
    $lk4 = $driver;
    &GetDir();
    print "#   List = $name - $size $date $time";
}

if ( ! -d "${drive}\\" ) {
    print STDERR "$prg ERROR: Drive <${drive}> NOT found!\a\n";
    exit 2;
}

if ($rel == 1) {  
   $cmd = "cd > $tmp 2>&1";
   $ret = system("$cmd");
   open(TMP,$tmp);
   $temp = <TMP>;
   close($tmp);
   print " in $temp \n";
} else {
   print "\n";
   if ($drive ne "C:") {  &NetUse();  }
   print "\n";
}

$total = 0;   # Total error count this run.
$cnt0 = 0;
$cnt1 = 0;
$cnt2 = 0;
$cnt3 = 0;

if ($rel == 1) { $drive = ""; }

open(DRV,"$driver");

while ($line = <DRV>) {
    chop($line);
    $aa = substr($line,0,1);
    if ($aa eq "#") { 
       printf "%s\n",$line;        #keep comment lines
       next; 
    }
    if (length($line) < 10) { next; }
    $line =~ tr/A-Z/a-z/;
    &Parse();
    $in_date = $word[2];
    $in_time = $word[3];
    $in_size = $word[1];
    $in_name = $word[0];
    if ($rel == 1) {
       $temp = $in_name;
       while (index($temp,"\\") >= 0) {
          $i = index($temp,"\\");
          $temp = substr($temp,$i+1);
       }
       $in_name = $temp;
    }
    $lk4 = ${drive} . ${in_name};
    #printf"%-40s %10s %8s %6s\n",$in_name,$in_size,$in_date,$in_time;
    $total++;
    &GetDir();
    if ($name ne $in_name) { 
       $cnt0++;
       printf"%s %s ",$in_name,$drive;
       printf " -- NOT Found! -- \n"; 
       next;
    }
    $temp = "";
    if ($size ne $in_size) { 
       $cnt1++;
       $temp =  " S(" . $in_size . " " . $size. ")"; 
    }
    if ($date ne $in_date) { 
       $cnt2++;
       $temp =  $temp . " D(" . $in_date . " " . $date . ")"; 
    }
    if ($time ne $in_time) { 
       $cnt3++;
       $temp =  $temp . " T(" . $in_time . " " . $time . ")"; 
    }
    if (length($temp) > 0) {
       printf"%s %s %s\n",$in_name,$drive,$temp;
    } else {
       if ($verbose == 1) { printf"%s %s \n",$in_name,$drive; }
    }
}

print "\n#   Totals: Files = $total ";
if ($cnt0 > 0) { print "  Not Found = $cnt0\n"; } else { print "\n"; }
if ($cnt1 + $cnt2 + $cnt3 > 0) { print "#   Differences: "; }
if ($cnt1 > 0) { print "  Size = $cnt1 "; }
if ($cnt2 > 0) { print "  Date = $cnt2 "; }
if ($cnt3 > 0) { print "  Time = $cnt3 "; }
print "\n";
&Stamp();
print STDOUT "$prg Ended   on ${Where} - ${Who} ${User} at $Event\n";

#-----------------------------------------------------------
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

#------------------------------------------------------------------------
sub Parse {
   local($i);
   for ($i = 0; $i < 8; $i++) { $word[$i] = ""; }
   $count = 0;
   for ($j = 0; $j <= length($line); $j++) {
      $aa = substr($line,$j,1);
      if ($aa eq " " || $aa eq ";") {
         if ( $word[$count] ne "") {
            $count++;
         }
      } else {
         $word[$count] = $word[$count] . $aa;
      }
   }
}
#------------------------------------------------------------------------
sub GetDir {
    $cmd = "dir $lk4 > $tmp 2>&1";
    #print "$cmd\n";
    $ret = system("$cmd");
    if ( $ret != 0) {
        $name = "xxxx";
        return;
    }
    
    open(TMP,$tmp);
    
    while ($line = <TMP>) {
        #print "$line";
        chop($line);
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
        if ($rel == 0) { $name = $path . "\\" . $name; }
        if ($name eq $in_name) { last; }
    }
    close(TMP);
    unlink($tmp);
}
#------------------------------------------------------------------------
sub NetUse {
    $cmd = "net use > $tmp 2>&1";
    $ret = system("$cmd");
    if ( $ret != 0) {
        $drive = "xxxx";
        return;
    }
    
    open(TMP,$tmp);

    while ($line = <TMP>) {
        chop($line);
        $line =~ tr/a-z/A-Z/;
        &Parse();
        $k = index($line,$drive);
        if ($k > 0) {
           print "#   Checking list with drive $drive - $word[2] \n";
        }
    }
    close(TMP);
    unlink($tmp);
}
#------------------------------------------------------------------------
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl

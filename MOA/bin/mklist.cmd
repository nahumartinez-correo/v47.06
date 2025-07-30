@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\mklist.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:33:19 $
#
#************************************************************************
#*                                                                      *
#*                 mklist                                               *
#************************************************************************
#  4 980520x      - chg new environment variables
#  3 980421x      - chg to fap3 
#   d  0708x      - chg c:\moa\bin 
#   c  0708x      - chg c:\apps\~  
#   b    24x      - add total bytes
#   a    23x      - adjust top comment lines (3 lines from 2)
# V2 970613x <kls - added param checking - not used 
#                 - add output file name 
# V1 970610x <kls - created from putadt.cmd V4
#
$ver = "4";
$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};
$script = (@script = split(/\\/,$0), pop(@script));
$script =~ s/.cmd$//i;
$prg = $script . "(" . $ver . ")";
$USAGE = "\n$prg: Usage not set!\a\n";
$usage  = 0;
$proj   = "fap3";
$wrkdir = "\\dlld";
$out = "${wrkdir}\\mklist.out";

while ($_ = shift) {
    if (/^-v/) { $verbose="1"; next; }
    if (/^-V/) { $verbose="1"; next; }
    if (/^-:/) { print "$USAGE"; exit 0; }
    if (/^-.*/) { 
       print "$prg Unknown option $_\n";  
       print "$USAGE";  
       exit 1; 
    }
    unshift(ARGV,$_);
    last;
}

$driver = shift;
$drive  = shift;

if (length($driver) == 0) { $driver = "mklist.lst"; }
if (length($drive) == 0) { $drive = "c:"; }

&Stamp();

print STDOUT "$prg Started on ${Where} - ${Who} ${User} at $Event\n";

if ( -f $driver ) {
    print STDERR "$prg ERROR: Output file <${driver}> already exists!\a\n";
    exit 2;
}

$total = 0;   # Total error count this run.
$tot_size = 0;
$cnt1 = 0;
$cnt2 = 0;
$cnt3 = 0;

$cmd = "dir . > $out 2>&1";
$ret = system("$cmd");
if ( $ret != 0) {
    print STDERR "$prg Directory listing failed!\a\n";
    exit 0;
}

open(OUT,$out);
open(DRV,">$driver");


while ($line = <OUT>) {
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
    $i = index($line,"<dir>");
    if ($i >= 0) { next; }
    $i = index($line,"directory of ");
    if ($i >= 0) { 
       $path = substr($line,$i+15);
       print DRV "#\n#$prg Path = $path \n";
       print DRV "#$prg on ${Where} - ${Who} ${User} at $Event\n#\n";
       print STDOUT "$prg Path = $path - Creating output file $driver\n\n";
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
    $name = $path . "\\" . $name;
    $total++;
    printf     "%-40s %10s  %8s  %-10s\n",$name,$size,$date,$time;
    printf DRV "%-40s %10s  %8s  %-10s\n",$name,$size,$date,$time;
    $i = index($size,",");
    if ($i >= 0) {
       $temp = substr($size,0,$i) . substr($size,$i+1);
    } else {
       $temp = $size;
    }
    $tot_size = $tot_size + $temp;
}
close(OUT);
close(DRV);

&Stamp();
print STDOUT "\n$prg There was a total of $total files = $tot_size\n";
print STDOUT "$prg Ended   on ${Where} - ${Who} ${User} at $Event\n";

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
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl

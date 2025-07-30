@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\ckver.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 21:50:02 $
#
#************************************************************************
#*                                                                      *
#*                                                                      *
#************************************************************************
# V1 960626x <kls - Created
# V2 960627x <kls - add - file size
# V3 970305x <kls - add - check for .htm files
#  3a   624x <kls - chg - general improvements
#  3b   708x <kls - chg - c:\apps\~
#  3c   924x <kls - chg - c:\moa\bin
# V4 980421x <kls - chg - to fap3
# V5 980520x <kls - chg - new environment variables
#
$ver = "5";
$temp = $ver;
$Who = $ENV{COMPUTERNAME};
$Where = $ENV{USERDOMAIN};
$User = $ENV{USERNAME};

$script = (@script = split(/\\/,$0), pop(@script));
$script =~ s/.cmd$//i;
$usage  = 0;
$Return = 0;
$prg = $script . "(" . $ver . ")";

&Stamp();

$dir = shift;
print "$prg Checking versions and sizes of batch files and scripts\n";
print "$prg On ${Where} - ${Who} ${User} at ${Event}\n";
if ($dir eq ".") {
   system("cd > \\temp\\ckver.out");
   open(OUT,"\\temp\\ckver.out");
   while (<OUT>) { $tmp = $_; last;}
   close(OUT);
   chop($tmp);
   unlink "\\temp\\ckver.out";
   $dir = $tmp;
   $dir =~ s#\\#/#g;
}
if ( ! -d $dir) {
   if (length($dir) == 0) { 
   print "\n$prg ERROR: A directory has not been specified !\n";
   } else {
   print "\n$prg ERROR: $dir is not a directory !\n";
   }
   exit;
}
print "$prg Path = $dir \n";
opendir(DIR,$dir);
while (length($file = readdir(DIR)) > 0) {
   if ($file eq ".") { next; }
   if ($file eq "..") { next; }
   $FILE = $file;
   $FILE =~ tr/a-z/A-Z/;
   $found = 0;
   $cnt = index($FILE,".CMD");
   if ($cnt >= 0) { $found = 1; }
   $cnt = index($FILE,".BAT");
   if ($cnt >= 0) { $found = 2; }
   $cnt = index($FILE,".HTM");
   if ($cnt >= 0) { $found = 3; }
   if ($found > 0) { 
      $xyz = "?";
      $size = -s "$dir\\$file";
      open(XXX,"$dir\\$file");
      while (<XXX>) {
         chop;
         tr/a-z/A-Z/;
         $i = index($_,"SET REV=");
         if ($i >= 0) { 
            $xyz = substr($_,$i + 8); 
            last;
         }
         $i = index($_,"VER = ");
         if ($i == 1) { 
            $xyz = substr($_,$i + 6); 
            last;
         }
         $i = index($_,"REV=");
         if ($i == 4) { 
            $i = index($_,"=");
            $xyz = substr($_,$i + 1); 
            last;
         }
      }
      close(XXX);
      $i = index($xyz,";");
      if ($i > 0) { $xyz = substr($xyz,0,$i); }
      $i = index($xyz,">");
      if ($i > 0) { $xyz = substr($xyz,0,$i); }
      $i = index($xyz,"\"");
      if ($i == 0) { $xyz = substr($xyz,1); }
      $i = index($xyz,"\"");
      if ($i  > 0) { $xyz = substr($xyz,0,$i); }
      $rrr = $xyz;
      $rrr =~ tr/A-Z/a-z/;
      $i = length($size);
      if ($i > 3) {
         $temp = substr($size,0,$i-3) . "," . substr($size,$i-3);
         $size = $temp;
      }
      $temp = sprintf("%30s %4s - %7s",$file,$rrr,$size);
      $list{$file} = $temp;
   }
}
closedir(DIR);

@slist = sort (keys(%list));
for ($i = 0; $i <= @slist; $i++) { printf("%s\n",$list{@slist[$i]});  } 

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
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl

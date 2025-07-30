@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\cpjnl.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:39:37 $
#
#
#************************************************************************
#*                             CpJnl                                    *
#*                Copy teller journal to a workstation                  *
#************************************************************************
# V1 960613x <kls - created
# V2 960621x <kls - chg - all print statements
# V3 960625x <kls - add - Xcopy cds and ql to a WS
# V4 970520x <kls - chg - fix for project 
# V5 970523x <kls - add - /E /I to the xcopy
# V6 970624x <kls - add - 1900 + year
# V7 970708x <kls - chg - c:\apps\~
# V8 970924x <kls - chg - c:\moa\bin
# V9 980421x <kls - chg - fap3
# 10 980520x <kls - chg - new environment variables
#
$ver = 10;

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

&Stamp();

$script = (@script = split(/\\/,$0), pop(@script));
$script =~ s/.cmd$//i;
$prg = $script . "(" . $ver . ")";
print STDOUT "$prg ${Where} - ${Who} ${User} at ${Event}\n";

$usage  = 0;
$Return = 0;
$wrkdir = "\\dlld";
$file   = 'jnlexp.txt';
$oldfile= 'jnlexp.old';
$cdslog = "$moaproj\\$proj\\cds\\cds_log";

&Stamp();

$temp = shift;
if ($temp eq "-x") {
    $arg = 1;
} else {
    $arg = 0;
}

if ( ! chdir(${wrkdir}) ) {
    print "$prg ERROR: Can't change to $wrkdir directory.\n";
    exit 2;
}

   $inifile = "$moaproj\\$proj\\ddproj.ini";
   $variable = "copy_journal2";
   $SectionFound = 0;
   $Found = 0;

   if (! -f $inifile) {
      printf("$prg ERROR: Config. file $inifile not found.\n");
      exit 2;
   }
   open(INI,$inifile);
   while(<INI>) {
      if ($Found) { next; }
      if (index($_,"Project Administration") >= 0) {
         $SectionFound = 1;
         next;
      }
      if ($SectionFound && /^$variable\s*=\s*(.*)/) {
         $Found=$1;
         next;
      }
   }
   close(INI);

   $Return = 0;
   if (!$SectionFound) {
      printf("$prg ERROR: \"Project Administration\" not found in $inifile.\n");
      $Return++;
   }
   if (!$Found) {
      printf("$prg ERROR: Variable $variable not found in $inifile.\n");
      $Return++;
   }
    
    $RET = 0;
    if ($Return == 0) {
        if ($wday == 0) { $temp = "SUN"; }
        if ($wday == 1) { $temp = "MON"; }
        if ($wday == 2) { $temp = "TUE"; }
        if ($wday == 3) { $temp = "WED"; }
        if ($wday == 4) { $temp = "THU"; }
        if ($wday == 5) { $temp = "FRI"; }
        if ($wday == 6) { $temp = "SAT"; }
    
        if ($arg == 0) {
           &DoJnl();
        } else {
           &DoTab();
        }
    }
$Return = $Return + $RET;

exit($Return);

#------------------------------------------------------------------
sub DoJnl {
        print ("$prg Copy journal file\n"); 
        if ( -f $file) {
            $cmd = "copy " . $file . "  " . $Found . "jnl" . $temp . ".sav";
            print "$prg Save journal -> $cmd\n";
            $RET = system("${cmd} > c:\\dlld\\cpjnl.out 2>&1");
            if ($RET > 0) {
               print "$prg ERROR: copy journal = ${RET} x";
               open(ERR,"c:/dlld/cpjnl.out");
               while (<ERR>) {
                  print "$_";
               }
               print "\n\n";
               close(ERR);
            }
            unlink("c:\\dlld\\cpjnl.out");
            $Return = $RET;
            unlink ("${oldfile}") if ( -f ${oldfile});
            rename ("${file}","${oldfile}");
            unlink ("${file}");
        } else {
          print ("$prg ERROR: Journal file $file not found.\n"); 
        }
}

#------------------------------------------------------------------
sub DoTab {
        $parm = " /E /I ";
        print ("$prg Copy CDS and QL files\n"); 
        $cds = "$moaproj\\$proj\\cds";
        if ( ! -d $cds ) {
           print "$prg ERROR: CDS Directory ${cds} not Found.\n";
           $Return++;
        } else { 
           $cmd = "Xcopy " . $cds . "  " . $Found . "cds" . $parm;
           print "$prg Save CDS -> $cmd\n";
           $RET = system("${cmd} > c:\\dlld\\xcpcds.out 2>&1 < nul");
           if ($RET > 0) {
              print "$prg ERROR: copy cds = ${RET}  ";
              #print "$prg Command is $cmd\n";
              open(ERR,"c:/dlld/xcpcds.out");
              while (<ERR>) {
                 print "$_";
              }
              print "\n\n";
              close(ERR);
           }
           unlink("c:\\dlld\\xcpcds.out");
        }
        $ql = "$moaproj\\$proj\\ql";
        if ( ! -d $ql ) {
           print "$prg ERROR: ql Directory ${ql} not Found.\n";
           $Return++;
        } else { 
           $cmd = "Xcopy " . $ql . "  " . $Found . "ql" . $parm;
           print "$prg Save QL -> $cmd\n";
           $RET = system("${cmd} > c:\\dlld\\xcpql.out 2>&1 < nul");
           if ($RET > 0) {
              print "$prg ERROR: copy ql = ${RET}  ";
              #print "$prg Command is $cmd\n";
              open(ERR,"c:/dlld/xcpql.out");
              while (<ERR>) {
                 print "$_";
              }
              print "\n\n";
              close(ERR);
           }
           unlink("c:\\dlld\\xcpql.out");
        }
}
#------------------------------------------------------------------
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

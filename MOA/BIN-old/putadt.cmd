@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\putadt.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:40:24 $
#
#
#************************************************************************
#*                                                                      *
#*                 Put Records using ql                                 *
#************************************************************************
# V1 960501x <kls - created
# V2 960621x <kls - general cleanup (for all other put scripts)
# V3 970220x <kls - made this script generic w\ file name
# V4 970221x <kls - chg - to use a driver file
# V5 970624x <kls - chg - year, user, proj 
# 5a 970708x <kls - chg - c:\apps\~   
# 5b 970924x <kls - chg - c:\moa\bin  
# V6 980421x <kls - chg - to fap3  
# V7 980520x <kls - chg - new environment variables
#
$ver = "7";
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


$script = (@script = split(/\\/,$0), pop(@script));
$script =~ s/.cmd$//i;
$prg = $script . "(" . $ver . ")";
$usage  = 0;
$wrkdir = '\dlld';
$driver = shift;

if (length($driver) == 0) { $driver = "putadt.drv"; }

$file   = $name . ".txt";
$oldfile = $name . ".old";

&Stamp();

print "$prg Put $proj records \n";
print "$prg STARTED on ${Where} - ${Who} ${User} at $Event\n";

if ( ! chdir(${wrkdir}) ) {
    print "$prg ERROR: Can't change to $wrkdir directory.\n";
    exit 2;
}
if ( ! -f $driver ) {
    print    "$prg Driver file <${driver}> not found!\n";
    exit 0;
}

$total = 0;   # Total error count this run.
$cnt1 = 0;
$cnt2 = 0;
$cnt3 = 0;

open(DRV,$driver);

while ($line = <DRV>) {
    chop($line);
    if (length($line) < 10) { next; }
    $aa = substr($line,0,1);
    if ($aa eq "#") { next; }
    &Parse();
    $table = $word[0];
    $name = $word[1];
    $file   = $name . ".txt";
    $oldfile = $name . ".old";
    $cnt1++;
    $t_errcnt=0;   
    if ( ! -f $file ) {
        print "$prg ${table} - File <${file}> NOT found!\n";
        $cnt3++;
    } else {
        $cnt2++;
        print "$prg $table - From <$file> Old <$oldfile> ";
        print " Save <${name}.new>\n";
        if ( -f $oldfile) { unlink ("${oldfile}"); }
        open (OUTFILE, ">${script}.ql");
        print OUTFILE "select all from ${table} dump into ${oldfile};\n";
        print OUTFILE "delete all from ${table};\n";
        print OUTFILE "quit;\n";
        close (OUTFILE);
        system ("ql -n${proj} < ${script}.ql > ${script}.tmp 2>&1");
        open (TEMP,"${script}.tmp");
        $rec_err = "ERROR";
        while ($record = <TEMP>) {
            chop ($record);
            $position = index($record," records deleted");
            if ($position > 0) {
                $start_pos = index($record,"> ");
                if ($start_pos >= 0 && $start_pos < $position) {
                    $start_pos+=2;
                    $end_pos = $position - $start_pos;
                    $rec_cnt = substr($record,$start_pos,$end_pos);
                } else {
                    $rec_cnt = substr($record,$0,$position);
                }
                $rec_err = "";
                last;
            }
        }
        close (TEMP);
        if ($rec_err eq "ERROR") {
           print "$prg ERROR: $table - Problem with deletion\n";
           open (ERR,"${script}.tmp");
           while (<ERR>) {
              print "* $_";
           }
           print "\n";
           close(ERR);
           $t_errcnt++;
        } else {
           print "$prg $table - $rec_cnt records deleted \n";
        }

        $command="insert into $table from $file;";
        system ("echo $command | ql -n${proj} > ${script}.tmp 2>&1");
        open (TEMP,"${script}.tmp");
        $rec_err = "ERROR";
        while ($record = <TEMP>) {
            chop ($record);
            $pos = index($record,"Inserted ");
            $start_pos = index($record,"Inserted ");
            if ($start_pos >= 0) {
                $end_pos = index($record," records.");
                if ($end_pos > $start_pos) {
                    $start_pos += 9;
                    $length = $end_pos - $start_pos;
                    $rec_cnt = substr($record,$start_pos,$length);
                    $rec_err = "";
                    last;
                }
            }
        }
        close (TEMP);
        if ($rec_err eq "ERROR") {
            print "$prg ERROR: $table - Problem with insertion.\n";
            open (ERR,"${script}.tmp");
            while (<ERR>) {
               print "* $_";
            }
            print "\n";
            close(ERR);
            $t_errcnt++;
        } else {
            print "$prg $table - $rec_cnt records inserted\n";
        }

#       print "$prg New records will be moved to ${name}.new\n";
        unlink ("${name}.new");
        rename ("${file}","${name}.new");
        unlink ("${file}");
        unlink ("${script}.tmp");
        unlink ("${script}.ql");
        if ($t_errcnt != 0) {
           print "${prg} $table - Finished with ${t_errcnt} errors.\n";
        }
    }
    $total = $total + $t_errcnt;
}

&Stamp();
print "${prg} TOTALS: $cnt1 tables - $cnt2 updated - ${cnt3} skipped";
print " - ${total} errors\n";
print "$prg ENDED  on ${Where} - ${Who} ${User} at $Event\n";

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

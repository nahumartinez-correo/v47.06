@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\lk4.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:32:55 $
#
# 1  950114@300p <kls - Fix for perl's problem with * command line expandion
#                  - Added 2 more levels of recursion
# 2  970416@300a <kls - chg - -i to upper/lower case search line
#                       add - -t directory, file, line, and found counts
#                       add - -b beep at end of search 
#                       add - -r skip rcs directories  
#                       add - -s short line  
#                       add - -u usage  
#                       add - -v verbose to set -t -a -s  
#                       add - if file or directory not supplied then use .  
# 3  980622@700a <kls - chg - new environment variables
#
$rev = "3";
$pgm = "Lk4(" . $rev . ")";
$|;

    $d_cnt = $f_cnt = $l_cnt = $x_cnt = 0;
    $ignore = $totals = $rcs = $short = $beep = $verbose = 0;
    while ($ARGV[0] =~ /^-/)  {
        $_ = shift;
        if (/^-i/) { $ignore = 1; next; }
        if (/^-I/) { $ignore = 1; next; }
        if (/^-t/) { $totals = 1; next; }
        if (/^-T/) { $totals = 1; next; }
        if (/^-r/) { $rcs = 1; next; }
        if (/^-R/) { $rcs = 1; next; }
        if (/^-s/) { $short = 1; next; }
        if (/^-S/) { $short = 1; next; }
        if (/^-b/) { $beep = 1; next; }
        if (/^-B/) { $beep = 1; next; }
        if (/^-v/) { $verbose = 1; next; }
        if (/^-V/) { $verbose = 1; next; }
        if (/^-u/) { &usage(); }
        if (/^-U/) { &usage(); }
        die "$pgm Unrecognized parameter: $_\n";  
    }
    if ($verbose == 1) { $totals = $short = $beep = $verbose; }

    $pat = shift;                           #search pattern
    $pat =~ s/!/\\!/g;
    $patu = $pat;
    $patu =~ tr/a-z/A-Z/;                   #upper case
    $patl = $patu;
    $patl =~ tr/A-Z/a-z/;                   #lower case

    if ($verbose == 1) {
    printf "%s - Looking for pattern <%s> in all files in the current directory\n\n",$pgm,$pat;
    }

    if ($ARGV[0] == "") { $ARGV[0] = "."; }

    foreach $f1 (@ARGV) {
       #next if $f1 eq '.';
       next if $f1 eq '..';
       next if (-B "$f1");
       if (-d "$f1") {
          $d_cnt++;
          opendir(DIR,$f1)  || die "Can not open dir: $f1";
          @file2 = readdir(DIR);
          closedir(DIR);
          chdir $f1 || die "Can not change dir: $f1";
#**********************************************************
          foreach $f2 (@file2) {
             next if $f2 eq '.';
             next if $f2 eq '..';
             next if (-B "$f2");
             if (-d "$f2") {
                $d_cnt++;
                opendir(DIR,$f2)  || die "Can not open dir: $f2";
                @file3 = readdir(DIR);
                closedir(DIR);
                chdir $f2 || die "Can not change dir: $f2";
#**********************************************************
                foreach $f3 (@file3) {
                   next if $f3 eq '.';
                   next if $f3 eq '..';
                   next if (-B "$f3");
                   if (-d "$f3") {
                      $d_cnt++;
                      opendir(DIR,$f3)  || die "Can not open dir: $f3";
                      @file4 = readdir(DIR);
                      closedir(DIR);
                      chdir $f3 || die "Can not change dir: $f3";
#**********************************************************
                      foreach $f4 (@file4) {
                         next if $f4 eq '.';
                         next if $f4 eq '..';
                         next if (-B "$f4");
                         if (-d "$f4") {
                            $d_cnt++;
                            opendir(DIR,$f4)  || die "Can not open dir: $f4";
                            @file5 = readdir(DIR);
                            closedir(DIR);
                            chdir $f4 || die "Can not change dir: $f4";
#**********************************************************
                            foreach $f5 (@file5) {
                               next if $f5 eq '.';
                               next if $f5 eq '..';
                               next if (-B "$f5");
                               if (-d "$f5") {
                                 $d_cnt++;
                                  opendir(DIR,$f5)||die "Can not open dir: $f5";
                                  @file6 = readdir(DIR);
                                  closedir(DIR);
                                  chdir $f5 || die "Can not change dir: $f5";
#**********************************************************
                                  foreach $f6 (@file6) {
                                     next if $f6 eq '.';
                                     next if $f6 eq '..';
                                     next if (-B "$f6");
                                     if (-d "$f6") {
                                        $d_cnt++;
                                        opendir(DIR,$f6)||die "Can not open dir: $f6";
                                        @file7 = readdir(DIR);
                                        closedir(DIR);
                                        chdir $f6 || die "Can not change dir: $f6";
#**********************************************************
                                        foreach $f7 (@file7) {
                                           next if $f7 eq '.';
                                           next if $f7 eq '..';
                                           next if (-B "$f7");
                                           if (-d "$f7") {
                                              $d_cnt++;
                                              print "TOO FAR DOWN: ";
                                              print "$f1\\$f2\\$f3\\$f4\\$f5";
                                              print "$f6\\$f7\n";
                                              next;
                                           }  #end if f7
                                           $name =  "$f1\\$f2\\$f3\\$f4\\$f5";
                                           $name =  "$name\\$f6\\$f7";
                                           $file = $f7;
                                           &lk4($file);
                                        }  #end foreach f7
#**********************************************************
                                  }        #end if f6
                                  $name =  "$f1\\$f2\\$f3\\$f4\\$f5\\$f6";
                                  $file = $f6;
                                  &lk4($file);
                                  }        #end foreach f6
#**********************************************************
                               chdir '..';
                               next;
                               }
                               $name =  "$f1\\$f2\\$f3\\$f4\\$f5";
                               $file = $f5;
                               &lk4($file);
                            }
#**********************************************************
                            chdir '..';
                            next;
                         }
                         $name =  "$f1\\$f2\\$f3\\$f4";
                         $file = $f4;
                         &lk4($file);
                   }
#**********************************************************
                       chdir '..';
                       next;
                   }
                   $name =  "$f1\\$f2\\$f3";
                   $file = $f3;
                   &lk4($file);
               }
#**********************************************************
                 chdir '..';
                 next;
             }
             $name =  "$f1\\$f2";
             $file = $f2;
             &lk4($file);
         }
#**********************************************************
          chdir '..';
          next;
       }
       $name =  "$f1";
       $file = $f1;
       &lk4($file);
   }   
if ($beep == 1) { print "\a"; }
if ($totals == 1) {
   printf "\n%s - Looking for pattern <%s> in a file or directory\n",$pgm,$pat;
   printf "%s - There were %s matches in %s directories ",$pgm,$x_cnt,$d_cnt;
   printf "%s files %s lines\n",$f_cnt,$l_cnt;
}
if ($beep == 1) { print "\a"; }

#-------------------------------------------------------------------------
sub usage {
   printf "\n%s - Looking for a pattern in all files in the current",$pgm;
   printf " directory \n%s - and following subdirectories \n",$pgm;
   printf "%s - Usage: lk4 [-b] [-i] [-t] [-u] [-v] pattern \n",$pgm;
   printf "%s -   -b = beep after search\n",$pgm;
   printf "%s -   -i = ignore case of pattern\n",$pgm;
   printf "%s -   -r = skip the rcs directories \n",$pgm;
   printf "%s -   -s = show only 79 characters of line with a +\n",$pgm;
   printf "%s -   -t = total directories, files, lines, and matches\n",$pgm;
   printf "%s -   -u = this usage display\n",$pgm;
   printf "%s -   -v = verbose (sets -b -s -t)\n",$pgm;
   exit;
}
#-------------------------------------------------------------------------
sub lk4 {
    $name =~ tr/A-Z/a-z/;
    if ($rcs == 1) {
        $i = index($name,"\\rcs\\");
        if ($i > 0) { return; }
    }
    open(FF,$file) || die "Can not OPEN $name: ";
    $f_cnt++;
    while (<FF>)  {
        $l_cnt++;
        $ok = 0;
        $i = index($_,$pat);                   #check regular pattern
        if ($i >= 0)  { $ok = 1; }
        if ($ignore == 1) {
            $temp = $_;
            $temp =~ tr/a-z/A-Z/;
            $i = index($temp,$patu);           #check upper case pattern
            if ($i >= 0)  { $ok = 1; }
            $temp =~ tr/A-Z/a-z/;
            $i = index($temp,$patl);           #check lower case pattern
            if ($i >= 0)  { $ok = 1; }
        }
        if ($ok == 1) {
           $x_cnt++;
           @words = split;
           $line = join(" ",@words);
           if ($short == 1) {
              $temp = $name . "~" . $. . "~" . $line;
              if (length($temp) > 78) { $temp = substr($temp,0,78) . "+"; }
              print $temp, "\n";
           } else { 
              print $name, "~", $., "~", $line,"\n";
           }
        }
    }
    close(FF);
}


#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl

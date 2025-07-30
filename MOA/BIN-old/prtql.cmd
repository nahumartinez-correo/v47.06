@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\prtql.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 22:34:08 $
#
# V3  970708x <kls - chg - c:\apps\~
# V4  970924x <kls - chg - c:\moa\bin
# V5  980421x <kls - chg - to fap3
# V6  980520x <kls - chg - new environment variables
#
$ver = 6;
$Prgm = "PrtQl(" . $ver . ")";

    $file = shift;                           #file name.

    if ( -f $file) {
        $Num = 0;
        open(FF,$file) || die "${Prgm}: Can not OPEN $file $!";
        while ($Line = <FF>)  {
           chop($Line);
           $Num++;
           $size = length($Line);
           printf(" * File = %s * record number = %s * size = %s\n",$file,$Num,$size);
           $temp = substr($Line,0,66);
           #printf("%s\n",$temp);
           $i = index($Line,"|");
           $count = 0;
           while ($i >= 0) {
              $i = index($Line,"|");
              $word = substr($Line,0,$i);
              $Line = substr($Line,$i +1);
              $i = index($Line,"|");
              if ($i < 0) {
                 $value = substr($Line,0);
              } else {
                 $value = substr($Line,0,$i);
              }
              $Line = substr($Line,$i +1);
              if (length($value) >= 1) {
                 $len = length($value);
                 printf("%28s %4d = %s\n",$word,$len,$value);
                 $count++;
              }
           }
           printf(" * File = %s * record number = %s ",$file,$Num);
           printf(" * size =  %s * fields = %s *\n",$size,$count);
           printf("\n");
        }
        close(FF);
    } else {
        print "${Prgm}: Can not find file: $file\n";
    }
    
#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl

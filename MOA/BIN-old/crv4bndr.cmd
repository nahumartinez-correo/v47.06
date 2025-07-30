@rem = '-*- Perl -*-';
@rem = '
@echo off
perl %MOADIR_MAINT%\bin\crv4bndr.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
';
$| = 1;
#
#  NOTE:  Don't change or move the above garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
#$Name: $
#$Revision: 1.1 $
#$Date: 1998/10/01 21:51:27 $
#
# 6   980520x < kls - new environment variables
# 5   980421x < kls - to fap3
# 4b  970924x < kls - c:\moa\bin
# 4a  970708x < kls - c:\apps\~ 
# 4   970530x < kls - 4 digit file format created from crv2bndr
#                   - adjusted for "NO PAY" in amount fields
#                   - adjusted for blank in amount fields
# 3   970507x < kls - added program and rev to print statemants
# 2.0 970123x < kls - changed to run from c:\moa\bin
# 1.0 950407x < kls - converted from unix to perl
#-------------------------------------------------------------------------
# crv2bndr  Converts the CRV Table from the Fed to a Bond Redemption
#           file in QL dump format suitable for loading the bndr ADT.
#
# The input file is the Fed CRVTABLE file received on an IBM format
# diskette.  The format of the data is:
#
#   Field           Length  Values/Description
#   --------------- ------  ------------------------------------------
#   Record type       1     E = E Bonds records.
#                           S = Savings certificates.  Not used here.
#                           N = EE Bonds records.
#   Redemption Year   4     Bond redemption year.
#   Redemption Month  2     Bond redemption month.
#   Issue Year        4     Bond issue year.
#   Redemption Values 6     This is a table of redemption values.
#                           There are 12 entries of 6 characters each.
#                           These entries are for the months of the 
#                           year beginning with January and ending
#                           with December.
#                           Note that these amounts are the $25.00 value
#                           and we convert them to the $100.00 value.
#
# The output file will be created in the same format as though the bndr
# ADT was extracted in dump format.
#-------------------------------------------------------------------------

$REV="Version 6 980520x <kls\n";
$ver = "6";
$pgm = "crv4bndr(" . $ver . ")";
print STDOUT "$pgm Converts the US Treasury Table to a Bond Redemption QL file\n\n";

while ($_ = shift) {
    /^-u/ && do { $usage = 1; last };
    /^-d/ && do { $debug = 1; next; };
    /^-/ && do { $usage = 1; last };
    unshift(ARGV,$_);
    last;
}
$USAGE="USAGE: $pgm  [-u] [-d] infile outfile \n \
	-u              Show usage \
	-d              Debug mode \n";

if ( $usage ){ 
	print "$USAGE";
	print "\n$pgm: $REV";
	exit 2;
}
#-------------------------------------------------------------------------

### ADT dump format field literal values.

$adt_app="tlr";                    # Application name.
$adt_series="adt_bndr_series";     # Bond series.
$adt_red_mon="adt_bndr_red_mon";   # Redemption month.
$adt_red_yr="adt_bndr_red_yr";     # Redemption year.
$adt_issue_yr="adt_bndr_issue_yr"; # Issue year.
$adt_amt="adt_bndr_amt";           # Redemption amounts table (12).

$InFile = shift;
$OutFile = shift;

if (! -f $InFile) {
    print "$pgm: Input file <$InFile> not found";
    exit 1;
}
if ( length($OutFile) == 0 ) {
    print "$pgm: Output file must be supplied";
    exit 1;
}
if ( -f $OutFile) {
    print "$pgm: Output file <$OutFile> already exists";
    exit 1;
}

open(FF,$InFile) || die "$pgm: Can not OPEN $InFile: $!";
open(FX,">$OutFile") || die "$pgm: Can not OPEN $OutFile: $!";

$InRec=0;

while (<FF>)  {

    $InRec++;
    if ($debug) { print STDERR "$pgm Processing record $InRec\n"; }

    $Series = substr($_,0,1);
    $Scnt++ if $Series eq 'S';      #count series S
    next if $Series eq 'S';         #skip series S

    $RedYr = substr($_,1,4);
    $RedMon = substr($_,5,2);
    $IssueYr = substr($_,7,4);
    $Amt1 = substr($_,11,6);
    $Amt2 = substr($_,17,6);
    $Amt3 = substr($_,23,6);
    $Amt4 = substr($_,29,6);
    $Amt5 = substr($_,35,6);
    $Amt6 = substr($_,41,6);
    $Amt7 = substr($_,47,6);
    $Amt8 = substr($_,53,6);
    $Amt9 = substr($_,59,6);
    $Amt10 = substr($_,65,6);
    $Amt11 = substr($_,71,6);
    $Amt12 = substr($_,77,6);

    if ($debug) {
      
        print STDERR "Record parse fields - - Raw.\n";
        print STDERR "Series       = $Series\n";
        print STDERR "Red Month    = $RedMon\n";
        print STDERR "Red Year     = $RedYr\n";
        print STDERR "Issue Year   = $IssueYr\n";
        print STDERR "Jan Amount   = $Amt1\n";
        print STDERR "Feb Amount   = $Amt2\n";
        print STDERR "Mar Amount   = $Amt3\n";
        print STDERR "Apr Amount   = $Amt4\n";
        print STDERR "May Amount   = $Amt5\n";
        print STDERR "Jun Amount   = $Amt6\n";
        print STDERR "Jul Amount   = $Amt7\n";
        print STDERR "Aug Amount   = $Amt8\n";
        print STDERR "Sep Amount   = $Amt9\n";
        print STDERR "Oct Amount   = $Amt10\n";
        print STDERR "Nov Amount   = $Amt11\n";
        print STDERR "Dec Amount   = $Amt12\n\n";
    }

    if ($Series eq 'E') {       #count series E
       $Ecnt++;
       if ($FirstE) {
           $eEndMon = $RedMon;
           $eEndYr = $RedYr;
       } else {
           $FirstE = 1;
           $eStartMon = $RedMon;
           $eStartYr = $RedYr;
           $eEndMon = $RedMon;
           $eEndYr = $RedYr;
          
       }
    }

    if ($Series eq 'N') {       #count series EE
       $Series = "EE";
       $Ncnt++;
       if ($FirstEE) {
           $eeEndMon = $RedMon;
           $eeEndYr = $RedYr;
       } else {
           $FirstEE = 1;
           $eeStartMon = $RedMon;
           $eeStartYr = $RedYr;
           $eeEndMon = $RedMon;
           $eeEndYr = $RedYr;
       }  

    }
    if ($Amt1 == "      " | $Amt1 == "NO PAY") { $Amt1 = 0; }
    if ($Amt2 == "      " | $Amt2 == "NO PAY") { $Amt2 = 0; }
    if ($Amt3 == "      " | $Amt3 == "NO PAY") { $Amt3 = 0; }
    if ($Amt4 == "      " | $Amt4 == "NO PAY") { $Amt4 = 0; }
    if ($Amt5 == "      " | $Amt5 == "NO PAY") { $Amt5 = 0; }
    if ($Amt6 == "      " | $Amt6 == "NO PAY") { $Amt6 = 0; }
    if ($Amt7 == "      " | $Amt7 == "NO PAY") { $Amt7 = 0; }
    if ($Amt8 == "      " | $Amt8 == "NO PAY") { $Amt8 = 0; }
    if ($Amt9 == "      " | $Amt9 == "NO PAY") { $Amt9 = 0; }
    if ($Amt10 == "      " | $Amt10 == "NO PAY") { $Amt10 = 0; }
    if ($Amt11 == "      " | $Amt11 == "NO PAY") { $Amt11 = 0; }
    if ($Amt12 == "      " | $Amt12 == "NO PAY") { $Amt12 = 0; }

    if (length($Amt1) == 0) { $Amt1 = 0; }
    if (length($Amt2) == 0) { $Amt2 = 0; }
    if (length($Amt3) == 0) { $Amt3 = 0; }
    if (length($Amt4) == 0) { $Amt4 = 0; }
    if (length($Amt5) == 0) { $Amt5 = 0; }
    if (length($Amt6) == 0) { $Amt6 = 0; }
    if (length($Amt7) == 0) { $Amt7 = 0; }
    if (length($Amt8) == 0) { $Amt8 = 0; }
    if (length($Amt9) == 0) { $Amt9 = 0; }
    if (length($Amt10) == 0) { $Amt10 = 0; }
    if (length($Amt11) == 0) { $Amt11 = 0; }
    if (length($Amt12) == 0) { $Amt12 = 0; }

    $Amt1 = $Amt1 * 4;
    $Amt2 = $Amt2 * 4;
    $Amt3 = $Amt3 * 4;
    $Amt4 = $Amt4 * 4;
    $Amt5 = $Amt5 * 4;
    $Amt6 = $Amt6 * 4;
    $Amt7 = $Amt7 * 4;
    $Amt8 = $Amt8 * 4;
    $Amt9 = $Amt9 * 4;
    $Amt10 = $Amt10 * 4;
    $Amt11 = $Amt11 * 4;
    $Amt12 = $Amt12 * 4;

    select(FX);
    printf FX ("%s:%s|%s|",$adt_app,$adt_series,$Series);
    printf FX ("%s:%s|%s|",$adt_app,$adt_red_mon,$RedMon);
    printf FX ("%s:%s|%s|",$adt_app,$adt_red_yr,$RedYr);
    printf FX ("%s:%s|%s|",$adt_app,$adt_issue_yr,$IssueYr);
#mwc    printf FX ("%s:%s|%s|1|0|11|",$adt_app,$adt_amt);
    printf FX ("%s:%s|1|0|11|",$adt_app,$adt_amt);


    $dollar = sprintf("%4.2f",$Amt1/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt2/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt3/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt4/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt5/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt6/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt7/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt8/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt9/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt10/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt11/100);
    printf FX ("%s|",$dollar);
    $dollar = sprintf("%4.2f",$Amt12/100);
#mwc    printf FX ("%s|\n",$dollar);
    printf FX ("%s\n",$dollar);
    $OutRec++;
    

}
close(FF);
close(FX);

printf STDOUT ("$pgm  Conversion completed successfully.\n\n");
printf STDOUT ("    Redemption: E bonds from %2s/%2s\n",$eStartMon,$eStartYr);
printf STDOUT ("                          to %2s/%2s\n",$eEndMon,$eEndYr);
printf STDOUT ("               EE bonds from %2s/%2s\n",$eeStartMon,$eeStartYr);
printf STDOUT ("                          to %2s/%2s\n",$eeEndMon,$eeEndYr);
printf STDOUT ("    Total E bonds records found:  %4d\n",$Ecnt);
printf STDOUT ("    Total EE bonds records found: %4d\n",$Ncnt);
printf STDOUT ("    Total S records skipped:      %4d\n",$Scnt);
printf STDOUT ("    Total input records:          %4d\n",$InRec);
printf STDOUT ("    Total output records:         %4d\n",$OutRec);

#
#  NOTE:  Don't change or move the following garbage!
#  This is the trick that lets this .CMD file be really a PERL script!
#
__END__
:endofperl

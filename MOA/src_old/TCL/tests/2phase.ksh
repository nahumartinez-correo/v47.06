:	'This script runs the CDS two phase commit regression test programs'
echo
echo "Testing two phase commit protocol in a multiple server environment"
echo

me=`whence $0`
orgdir=`pwd`
PRIMARYOPT=""
PROJ="_min"
pp=$MOADIR
this=$orgdir
this=${this%/*}
#goodbye tests
this=${this%/*}
#goodbye tcl
this=${this%/*}
#goodbye src
pp=${pp:-$this}
PATH=";.;$pp/bin;/mksnt;$PATH"

REV="V1.4.2 September 4, 1996"
rm -f /tmp/2phase.nrm /tmp/2phase.out /tmp/2phase.err
cp $orgdir/2phase.nrm /tmp

PRIMARYOSOPT=""

USAGE="*** USAGE *** $0  [-nPROJ]"
while getopts :n:VU option
do
	case $option in
		n) PROJ="$OPTARG" ;;
		V)	echo "\n$0 $REV"
			exit ;;
		U)	echo "$USAGE"
			exit 1 ;;
		 :) echo "$USAGE"
			exit 1 ;;
		\?) echo "Unknown option $OPTARG"
			echo "$USAGE"
			exit 1 ;;		
	esac
done
NPROJECT="-n$PROJ"

echo "killing the servers (if running)"
cdskill -v $NPROJECT -SA >/tmp/killout 2>&1
cdskill  $NPROJECT -SB	>/tmp/null 2>&1
cdskill  $NPROJECT -SC	>/tmp/null 2>&1

MOAPROJ=${MOAPROJ:-/tmp}
DDPATH=$MOAPROJ/$PROJ

TDIRA=$DDPATH/A.cds
TDIRB=$DDPATH/B.cds
TDIRC=$DDPATH/C.cds
rm -rf $TDIRA
rm -rf $TDIRB
rm -rf $TDIRC
mkdir $TDIRA
mkdir $TDIRB
mkdir $TDIRC

for SERVICENAME in A B C
do
	cdsv -s -c -S $SERVICENAME  $NPROJECT -v &
done

echo "cdservers started, waiting 10 seconds to start first test"
sleep 10

OK=""
FAILS=0;
ql -V
CLERRFILE=$TDIRA/ql.err; export CLERRFILE
ql -SB $NPROJECT >/tmp/2phase.out <<'@'
run from phase1.ql;
run from phase2.ql;
@

cat $CLERRFILE
echo "\n" >>/tmp/2phase.out

ql -SB $NPROJECT >> /tmp/2phase.out <<'@'
connect to A;
run from phase3.ql;
@
cat $CLERRFILE
echo "\n" >>/tmp/2phase.out
cdsstat $NPROJECT -SA
cdsstat $NPROJECT -SB
cdsstat $NPROJECT -SC

#gdiff +ignore-space-change +ignore-blank-lines /tmp/2phase.out 2phase.nrm > /tmp/2phase.err
diff -bw 2phase.nrm /tmp/2phase.out > /tmp/2phase.err
echo "LINES BELOW represent ERRORS ---------------"
cat /tmp/2phase.err
echo "LINES ABOVE represent ERRORS ^^^^^^^^^^^^^^^^"
echo " "
set `wc -l /tmp/2phase.err`
if [ $1 != 0 ]
then
	echo $0 script failed.  See /tmp/2phase.err for differences
	exit 1
fi
echo "killing the servers (if running)"
cdskill -v $NPROJECT -SA >>/tmp/killout 2>&1
cdskill  $NPROJECT -SB	>/dev/null 2>&1
cdskill  $NPROJECT -SC	>/dev/null 2>&1
rm -rf $TDIRA
rm -rf $TDIRB
rm -rf $TDIRC
exit 0

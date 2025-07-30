#!/bin/ksh	'This script runs one of the regression tests for CDS'
echo
echo "Testing repidx: change an index and then force a rollforward"
echo

me=`whence $0`
alias copy='cp -m'
orgdir=`pwd`
SERVICENAME=test
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
PATH=";;$pp/bin;/mksnt;$PATH"

REV="V1.4.2 September 4, 1996"
rm -f /tmp/repidx.nrm
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
MOAPROJ=${MOAPROJ:-/tmp}
DDPATH=$MOAPROJ/$PROJ
copy $orgdir/repidx.nrm /tmp

cdskill $NPROJECT -Stest >\tmp\devNull 2>&1
rm -f \tmp\devNull
rm -rf test
mkdir test
rm -f repidx.out repidx.err
cdsv -s $NPROJECT -Stest ./test &
echo "Waiting 10 seconds for server to start"
sleep 10
echo "test of replace_with_index_change rolling forward. (Approximately 30 seconds)"
ql -# $NPROJECT -Stest << '@' > /tmp/repidx.out 2>&1
run from repidx.ql;
@
cdskill $NPROJECT -Stest
#gdiff +ignore-space-change +ignore-blank-lines repidx.nrm /tmp/repidx.out > /tmp/repidx.err
diff -bw repidx.nrm /tmp/repidx.out > /tmp/repidx.err

set `wc -l /tmp/repidx.err`
if [ $1 -ne "0" ]
then
	echo "errors = $1"
	echo
	echo "Check the /tmp/repidx.err files for differences"
	echo
	exit 2
fi
cd test
#logdump cds_log
mv cds_log first_log
if [ -f /unix ]
then
	LOGLEN=962
else
	LOGLEN=992
fi
dd if=first_log of=cds_log bs=$LOGLEN count=1
cd ..
cdsv -s $NPROJECT -Stest ./test &
sleep 10
if tail -10 ./test/out |grep "Completed table opens"
then
	echo "repidx.ksh succeeded"
	cdskill $NPROJECT -Stest
else
	echo "repidx.ksh failed.  cdsv didn't respawn."
	exit 3
fi
exit 0

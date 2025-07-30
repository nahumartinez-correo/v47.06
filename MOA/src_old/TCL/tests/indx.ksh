:	'This script runs one of the holely keys regression tests for CDS'
echo
echo "Testing indexes support and find_n"
echo

alias copy=cp
me=`whence $0`
rm -f /tmp/indx.nrm
orgdir=`pwd`
copy $orgdir/indx.nrm /tmp
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

PATH=";.;$pp/bin;/mksnt;$PATH"

USAGE="*** USAGE *** $0  [-nPROJ]"
REV="V1.4.2 September 4, 1996"

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
PLIB=$MOADIR/lib

DDPATH=$MOAPROJ/$PROJ

cdskill $NPROJECT -Stest >>foo 2>&1
rm -rf test
mkdir test
rm -f indx.out indx.err

cdsv -s $NPROJECT -c -Stest ./test &
echo "Waiting 10 seconds for server to start"
sleep 10
echo "test of index support. (Approximately 30 seconds)"
ql $NPROJECT -Stest << '@' > /tmp/indx.out 2>&1
run from indx.ql;
@
diff -bw indx.nrm /tmp/indx.out > /tmp/indx.err
cdskill $NPROJECT -Stest
echo
echo "Check the /tmp/indx.err files for differences"
echo
set `wc -l /tmp/indx.err`
if [ $1 -ne "0" ]
then
	echo "errors = $1"
	exit 2
fi
exit 0





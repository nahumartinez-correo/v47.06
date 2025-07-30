:	'This script runs one of the holely keys regression tests for CDS'
echo
echo "Testing hole'ly keys one"
echo

me=`whence $0`
orgdir=`pwd`
alias copy='cp -m'
rm -f /tmp/holes1.nrm
copy $orgdir/holes1.nrm /tmp
SERVICENAME=test
PRIMARYOPT=""
PROJ="_min"
pp=$MOADIR
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
DDPATH=$MOAPROJ/$PROJ

cdskill $NPROJECT -Stest
rm -rf test
mkdir test
rm -f holes1.out holes1.err
cdsv -s $NPROJECT -c -E9 -Stest ./test &
echo "Waiting 10 seconds for server to start"
sleep 10
echo "test of hole'ly keys. (Approximately 60 seconds)"
ql -# $NPROJECT -Stest << '@' > /tmp/holes1.out 2>&1
run from holes1.ql;
@
#gdiff +ignore-space-change +ignore-blank-lines holes1.nrm /tmp/holes1.out > /tmp/holes1.err
diff -bw holes1.nrm /tmp/holes1.out > /tmp/holes1.err
cdskill $NPROJECT -Stest
echo
echo "Check the /tmp/holes1.err files for differences"
echo

set `wc -l /tmp/holes1.err`
if [ $1 -ne "0" ]
then
	echo "errors = $1"
	exit 2
fi
exit 0

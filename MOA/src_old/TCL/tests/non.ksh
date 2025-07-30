:
# test the non-unique indexes.  Specificall, test the fetch ops
# against indexes of type 0, 1, and cdskey
# assure that tests are conducted when the non-uniques have no neighbors
# and when they have neighbors (values on either side)

USAGE="*** USAGE *** $0  [-nPROJ]"

me=`whence $0`
MOAPROJ=${MOAPROJ:-/tmp}
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
export PATH

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

cdskill $NPROJECT -Stest >/tmp/non.err 2>&1
rm -rf ${CDS_DIRECTORY:-./test}
rm -f /tmp/non.out
mkdir  ${CDS_DIRECTORY:-./test}
cdsv -s $NPROJECT -Stest  ${CDS_DIRECTORY:-./test} &
echo "Waiting 10 seconds for server to start"
sleep 10
echo "Testing non-unique indexes (approx 10 seconds)"
ql $NPROJECT -Stest >/tmp/non.out 2>&1 <<'@'
run from non.ql;
@
#gdiff +ignore-blank-lines +ignore-space-change non.nrm /tmp/non.out > /tmp/non.err
diff -bw non.nrm /tmp/non.out > /tmp/non.err
cdskill $NPROJECT -Stest 
set `wc -l /tmp/non.err`
if [ $1 != 0 ]
then
	echo non.ql failed.  See /tmp/non.err for differences
	exit 1
fi
exit 0


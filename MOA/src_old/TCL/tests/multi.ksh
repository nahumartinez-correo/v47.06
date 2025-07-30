:
#  Test Multiple servers environment
#
echo
echo "Testing multiple server environment"
echo
USAGE="*** USAGE *** $0  [-nPROJ]"
SERVICENAME=tst1
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
THIS_DIR=`pwd`

rm -rf /tmp/tst1 /tmp/tst2 /tmp/tst3 > /tmp/null 2>&1
rm -f multi.out multi.err /tmp/multi.out /tmp/multi.err
mkdir /tmp/tst1
mkdir /tmp/tst2
mkdir /tmp/tst3

cdsv -s $NPROJECT -Stst1 /tmp/tst1 &
cdsv -s $NPROJECT -Stst2 /tmp/tst2 &
cdsv -s $NPROJECT -Stst3 /tmp/tst3 &
sleep 1
echo "Started 3 servers - waiting 20 seconds for servers to come up"
sleep 20
ql -# $NPROJECT -Stst1 < multi.ql > /tmp/multi.out 2>&1
#gdiff +ignore-space-change +ignore-blank-lines /tmp/multi.out multi.nrm > /tmp/multi.err
diff -bw multi.nrm /tmp/multi.out > /tmp/multi.err

cdskill $NPROJECT -Stst1
cdskill $NPROJECT -Stst2
cdskill $NPROJECT -Stst3

set `wc -l /tmp/multi.err`
if [ $1 -ne "0" ]
then
	echo multi test failed
	echo 
	echo "Check the /tmp/multi.err file for differences"
	echo
	exit 1
else
	echo multi test passed
	echo
	rm -r /tmp/tst1 /tmp/tst2 /tmp/tst3
fi
exit 0

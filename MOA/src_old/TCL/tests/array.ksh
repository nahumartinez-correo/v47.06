:
# Test arrays in cds

echo running $0

USAGE="*** USAGE *** $0  [-nPROJ]"
me=`whence $0`
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
PLIB=/usr/iscbr/lib/moa
export NETPATH=dnet

MOAPROJ=${MOAPROJ:-/tmp}
DDPATH=$MOAPROJ/$PROJ

CDS_DIRECTORY=./test
cdskill $NPROJECT -Stest >/tmp/null 2>&1
rm -rf $CDS_DIRECTORY
mkdir $CDS_DIRECTORY
rm -f /tmp/array.out /tmp/array.err
# add required fields that might be missing
impfld $NPROJECT > /tmp/null 2>&1 <<@
;field
[common]
field1[5] ASCII 20 [ ] sCDEAscUrDmpbdh 0 0 0 0 0 0 0
field2[5] ASCII 20 [ ] sCDEAscUrDmpbdh 0 0 0 0 0 0 0
@
sleep 1
cdsv -s $NPROJECT -Stest $CDS_DIRECTORY &
echo "Waiting 6 seconds for CDServer to start"
sleep 6
echo "testing array and outer array (approximately 15 seconds)"
ql $NPROJECT -Stest >/tmp/array.out 2>&1 <<@
run from array.ql;
@
#gdiff +ignore-space-change +ignore-blank-lines array.test array.out >/tmp/array.err
diff -bw array.nrm /tmp/array.out > /tmp/array.err
cdskill $NPROJECT -Stest
set `wc -l /tmp/array.err`
if [ $1 -ne "0" ]
then
	echo array test failed
	echo 
	echo "Check the /tmp/array.err file for differences"
	echo
	exit 1
else
	echo array test passed
	echo
	rm -rf $CDS_DIRECTORY/*
fi
exit 0




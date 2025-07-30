:
# Test inner natural join and outer join - 

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
rm -f join.out /tmp/join.err
cdsv -s $NPROJECT -Stest $CDS_DIRECTORY &
echo "Waiting 20 seconds for CDServer to start"
sleep 20
echo "testing join and outer join (approximately 2 minutes)"
ql $NPROJECT -Stest >join.out 2>&1 <<@
run from join.ql;
@
#gdiff +ignore-space-change +ignore-blank-lines join.test join.out >/tmp/join.err
diff -bw join.nrm join.out > /tmp/join.err
cdskill $NPROJECT -Stest
set `wc -l /tmp/join.err`
if [ $1 -ne "0" ]
then
	echo join test failed
	echo 
	echo "Check the /tmp/join.err file for differences"
	echo
	exit 1
else
	echo join test passed
	echo
	rm -rf $CDS_DIRECTORY/*
fi
exit 0




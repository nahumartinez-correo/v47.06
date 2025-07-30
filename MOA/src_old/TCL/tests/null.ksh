#!/usr/bin/ksh
# test using emptry string literals and is_missing agains various indexes,
# data types and comparators

USAGE="*** USAGE *** $0  [-nPROJ]"

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
export PATH
MOAPROJDIR=/moaproj

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
DDPATH=$MOAPROJDIR/$PROJ
if [ ! -d $DDPATH -o ! -r $DDPATH/DDobj.dat ]
then
	echo "Project $PROJ is not in $MOAPROJDIR"
	exit 2
fi

cdskill $NPROJECT -Stest >/dev/null 2>&1
rm -rf ${CDS_DIRECTORY:-./test}
rm -f /tmp/null.out /tmp/null.err
t=${CDS_DIRECTORY:-./test}
echo "t is '$t'"
mkdir  $t
export out=$t/out
cdsv -s $NPROJECT -Stest -E8 ${CDS_DIRECTORY:-./test} &
echo "Waiting 10 seconds for server to start"
sleep 10
echo "Testing selection criteria referencing NULL and empty string literals"
echo " (approx 30 seconds)"
ql $NPROJECT -Stest >/tmp/null.out 2>&1 <<'@'
run from null.ql;
@

#gdiff +ignore-blank-lines +ignore-space-change null.nrm /tmp/null.out > /tmp/null.err
diff -bw null.nrm /tmp/null.out > /tmp/null.err
set `wc -l /tmp/null.err`
if [ $1 != 0 ]
then
	echo "null.ksh failed.  See /tmp/null.err for differences"
	exit 1
fi
exit 0


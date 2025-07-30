
:
# Test virtual file descriptors - 
# Tests high range, selecting and deleting.

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
MOAPROJ=${MOAPROJ:-/tmp}
export PATH

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

DDPATH=$MOAPROJ/$PROH

#CDS_ENV=$PLIB/cds.env/$PROJ$SERVICENAME
#if [ -r $CDS_ENV ]
#then
#	. $CDS_ENV
#else
#	echo "$CDS_ENV file is missing.  So, will make new one"
#	cdsmkenv -n$PROJ -S$SERVICENAME -p$DDPATH -s none -T $orgdir/
#fi

cdskill $NPROJECT -Stest >/tmp/null 2>&1
rm -rf ${CDS_DIRECTORY:-./test}
mkdir  ${CDS_DIRECTORY:-./test}
rm -f ins.out select.out drop.out ins.err select.err drop.err
cdsv $NPROJECT -Stest -c -v ./test &
echo "Waiting 20 seconds for server to start"
sleep 20
echo "Testing create and insert (Approx 5 minutes)"
ql $NPROJECT -Stest >/tmp/ins.out 2>&1 <<'@'
run from ins.ql;
@
#gdiff +ignore-blank-lines +ignore-space-change ins.test /tmp/ins.out > /tmp/ins.err
diff -bw ins.nrm /tmp/ins.out > /tmp/ins.err
set `wc -l /tmp/ins.err`
if [ $1 != 0 ]
then
	echo ins.ql failed.  See /tmp/ins.err for differences
	exit 1
fi
echo "Testing selects and updates of tables"
ql $NPROJECT -Stest > /tmp/select.out 2>&1 <<'@'
run from sel.ql;
@
#gdiff +ignore-space-change +ignore-blank-lines select.test /tmp/select.out > /tmp/select.err
diff -bw select.nrm /tmp/select.out > /tmp/select.err
set `wc -l /tmp/select.err`
if [ $1 != 0 ]
then
	echo sel.ql failed.  See /tmp/select.err for differences
	exit 1
fi
cdsstat -T $NPROJECT -Stest
echo "Droping all of the tables (Approx. 4 minutes)"
ql $NPROJECT -Stest > /tmp/drop.out 2>&1 <<'@'
run from drop.ql;
@
#gdiff +ignore-space-change +ignore-blank-lines drop.test drop.out > /tmp/drop.err
diff -bw drop.nrm /tmp/drop.out > /tmp/drop.err
cdskill $NPROJECT -Stest
set `wc -l /tmp/drop.err`
if [ $1 != 0 ]
then
	echo drop.ql failed.  See /tmp/drop.err for differences
	exit 1
fi
rm -rf ./test/*
exit 0

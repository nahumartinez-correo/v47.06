
:
# Test projections and aggregates
# Tests projections, aggregates, formatting for ql...

USAGE="*** USAGE *** $0  [-nPROJ]"
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

case $# in
 0)
 	NPROJECT="-n_min"
 	opt=""
 	;;
 1)
	NPROJECT=$1
	;;
 *)
 	echo "$USAGE"
 	exit 1
 	;;
esac
cdskill $NPROJECT -Stest -v	 > /tmp/foo 2>&1
rm -rf test
mkdir test
rm -f proj.out proj.err /tmp/proj.out /tmp/proj.err
cdsv -s $NPROJECT -c -Stest ./test &
echo "Waiting 20 seconds for server to start"
sleep 20
echo "Simple create table, inserts, and selects (Approx. 30 seconds)"
ql -# $NPROJECT -Stest << '@' > /tmp/proj.out 2>&1
run from proj.ql;
@
#gdiff +ignore-space-change +ignore-blank-lines proj.test /tmp/proj.out > /tmp/proj.err
diff -bw proj.nrm /tmp/proj.out > /tmp/proj.err
cdskill $NPROJECT -Stest
set `wc -l /tmp/proj.err`
if [ $1 -ne "0" -a -r /tmp/proj.out -a -r /tmp/proj.err ]
then
	echo projection test failed
	echo 
	echo "Check the /tmp/proj.err file for differences"
	echo
	exit 1
else
	echo proj test passed
	echo
fi
exit 0

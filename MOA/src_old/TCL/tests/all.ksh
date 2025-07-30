:	"run all tests"
USAGE="*** USAGE *** $0  [-nPROJ]"
REV="V1.4.2 September 4, 1996"
this_dir=`pwd`
this=${this_dir##*/}
if [ $this != "tests" ]
then
	echo "must run while in directory $MOADIR/src/tcl/tests"
	exit
fi
pp=$MOADIR
this=`pwd`
this=${this%/*}
#goodbye tests
this=${this%/*}
#goodbye tcl
this=${this%/*}
#goodbye src
pp=${pp:-$this}

PATH=";.;$pp/bin;/mksnt;$PATH"
export CDS_NETPATH
echo "PATH is $PATH"
TEMPDIR=/tmp
mkdir $TEMPDIR > foo 2>&1
mkdir test >> foo 2>&1

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

integer good=0
integer bad=0
rm -f *.err

echo "starting test of index support. (Approximately 160 seconds)"
indx.ksh $NPROJECT > $TEMPDIR/indx.t 2>&1
if [ $? = 0 ]
then
	echo indx.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo indx.ksh failed
fi

echo "starting test of hole'ly keys. (Approximately 60 seconds)"
holes1.ksh $NPROJECT > $TEMPDIR/holes1.t 2>&1
if [ $? = 0 ]
then
	echo holes1.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo holes1.ksh failed
fi

echo starting cdsrtest
cdsrtest $NPROJECT >$TEMPDIR/cdsrtest.t 2>&1
ok=`tail -1 $TEMPDIR/cdsrtest.t | fgrep "Nice Job!" |wc -l`
if [ $ok -ne "1" ]
then
	echo cdsrtest failed look at $TEMPDIR/cdsrtest.t
	exit 1
else
	echo cdsrtest passed
	good=$good+1
fi

virtfd.ksh $NPROJECT
if [ $? = 0 ]
then
	echo virtfd.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo virtfd.ksh failed
fi
join.ksh $NPROJECT >$TEMPDIR/join.t 2>&1
if [ $? = 0 ]
then
	echo join.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo join.ksh failed
fi

multi.ksh $NPROJECT >$TEMPDIR/multi.t 2>&1
if [ $? = 0 ]
then
	echo multi.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo multi.ksh failed
fi
proj.ksh $NPROJECT >$TEMPDIR/proj.t 2>&1
if [ $? = 0 ]
then
	echo proj.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo proj.ksh failed
fi

non.ksh $NPROJECT >$TEMPDIR/non.t 2>&1
if [ $? = 0 ]
then
	echo non.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo non.ksh failed
fi

array.ksh $NPROJECT >$TEMPDIR/array.t 2>&1
if [ $? = 0 ]
then
	echo array.ksh succeeded
	good=$good+1
else
	bad=$bad+1
	echo array.ksh failed
fi

#2phase.ksh $NPROJECT >$TEMPDIR/2phase.t 2>&1
#if [ $? = 0 ]
#then
#	echo 2phase.ksh succeeded
#	good=$good+1
#else
#	bad=$bad+1
#	echo 2phase.ksh failed
#fi

echo $good successes
echo $bad failures
if [ $bad = 0 ]
then
	echo removing test directory
	rm -r test
fi
exit 0

#!/bin/ksh
#	'This script runs the regression test programs for CDS'
pp=$MOADIR
this=`pwd`
this=${this%/*}
#goodbye tests
this=${this%/*}
#goodbye tcl
this=${this%/*}
#goodbye src
pp=${pp:-$this}
MOADIR=$pp
PLIB=$MOADIR/lib
UNIX=0
alias copy="cp -m"

REV='Revision: V1.4.2 September 4, 1996'
USAGE="\n*** Usage *** $0 [-n project][-S servername] [-F] [-V] [-U] 
   -n (optional) project name,   default _xmp
   -S (Optional) server service name, default tst
   -F (Optional) start server with -F (no flush) option
   -V (Optional) returns Version of $0
   -U (Optional) display usage"

SERVICENAME=tst
FLUSHOPT=""
PRIMARYOPT=""
PROJ="_min"
#
# Evaluate command line
#
while getopts :n:S:FVU option
do
	case $option in
		n) PROJ="$OPTARG" ;;
		S) SERVICENAME="$OPTARG" ;;
		F) FLUSHOPT=" -F " ;;
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

LOGNAME=${LOGNAME:-moacds}
TDIR=$LOGNAME.cds
FIELD_A_ADDED=n
PATH=";;$pp/bin;$PATH"

findobj -n$PROJ common:field_a > /tmp/$$
FOUND=$?
if [ $FOUND != 0 ]
then
	if [ $FOUND = 100 -a $UNIX = 0 ]
	then
		echo "Adding field_a to your $PROJ dictionary"
		importfld -n$PROJ >> /tmp/$$ <<EOT
;fields
[common]

field_a[3] ASCII 20 [ ] SCDEAscurDmpbdh 0 0 0 0 0 0 0 
EOT
		if [ $? != 0 ]
		then
			echo "field_a missing and can't be added to dictionary"
			cat /tmp/$$
			rm -f /tmp/$$
			exit 20
		else
			FIELD_A_ADDED=y
		fi
	else
		echo "field_a missing and can't be added to dictionary"
		cat /tmp/$$
		rm -f /tmp/$$
		exit 21
	fi
fi

TST=tst
this_machine=`uname -n`
if [ -z "$DNP" ]
	then
		DNP=$this_machine
		SHADOWDNP=none
	fi
#	echo "export PROJECT=$PROJ
export CDS_SERVICE_NAME=tst
export CDS_PRIMARY_NP=$DNP
#export CDS_SHADOW_NP=$SHADOWDNP
export CDS_DIRECTORY=/tmp/$LOGNAME.cds

cdsstat -V
SERV="-S$SERVICENAME"
if [ $SERV = "-S" ]
then
	SERV=""
fi
cdsstat -n$PROJ $SERV >/tmp/$$
if [ $? = 0 ]
then
	cat /tmp/$$
	rm -f /tmp/$$
	echo "killing the server"
	if [ -x `which cdskill` ]
	then
		cdskill -n$PROJ $SERV
	else
		echo "can't run cdskill, goodbye!"
		exit 13
	fi
else
	echo "no server running yet"
	rm -f /tmp/$$
fi

cd /tmp
sleep 2
#	give logger task time to die
rm -rf $TDIR
mkdir $TDIR
cdsv -s -v $FLUSHOPT $SERV -n$PROJ &
echo "cdsv started, waiting 10 seconds to start first test"
sleep 10

OK=""
FAILS=0;
echo "testddl"
testddl -V
TDIR=/tmp/$TDIR
CLERRFILE=/tmp/ddl.err; export CLERRFILE
testddl -n$PROJ $SERV -M12000 2>/tmp/ddl.err
if [ $? = 0 ]
then
	OK=" testddl succeeded\n"
else
	BAD=" testddl failed\n"
	FAILS=y;
fi
#logdump $TDIR/cds_log

echo "testcl  test client dml"
testcl -V
CLERRFILE=/tmp/cl.err; export CLERRFILE
testcl  -n$PROJ $SERV -M12000 9 2>/tmp/cl.err
if [ $? = 0 ]
then
	OK="$OK testcl succeded\n";
else
	BAD="$BAD testcl failed\n"
	FAILS=y;
fi

echo "testluw  test logical units of work"
testluw -V
CLERRFILE=/tmp/luw.err; export CLERRFILE
testluw  -n$PROJ $SERV 2>/tmp/errs
if [ $? = 0 ]
then
	OK="$OK testluw succeeded\n"
else
	BAD="$BAD testluw failed\n";
	FAILS=y;
fi

echo "testnw  test no-wait I/O"
testnw -V
CLERRFILE=/tmp/nw.err; export CLERRFILE
testnw  -n$PROJ $SERV 2>/tmp/nw.err
if [ $? = 0 ]
then
	OK="$OK testnw succeeded\n"
else
	BAD="$BAD testnw failed\n"
	FAILS=y;
fi

echo "tcompres    test compress"
tcompres -V
CLERRFILE=/tmp/compress.err; export CLERRFILE
tcompres -n$PROJ $SERV 2>/tmp/compress.err
if [ $? = 0 ]
then
	OK="$OK tcompres succeeded\n"
else
	BAD="$BAD tcompres failed\n"
	FAILS=y;
fi

if [ $FAILS = 0 ]
then
	echo "$OK"
	echo "Nice Job!"
	cdskill -n$PROJ $SERV
	if [ $LOGNAME != "bodhi" ]
	then
		cd /tmp
		rm -rf $TDIR
	fi
	exit 0
else
	echo $OK
	echo $BAD
	echo "Sorry Charlie."
	cdskill -n$PROJ $SERV
	exit 1
fi

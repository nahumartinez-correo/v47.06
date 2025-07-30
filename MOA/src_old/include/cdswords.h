/* ----------------------------------------------------------- */
/*  1-byte tokens corresponding to PCL statements and their    */
/*  parameter keywords.  Routines which handle these tokens    */
/*  are cdslookup() and cdskeyword().                          */
/* ----------------------------------------------------------- */

#ifndef CDSWORDS_H
#define CDSWORDS_H

/* -------------------------------------------------------- */
/*  One-byte tokens which are not PCL statements, but are   */
/*  reserved words nonetheless.  The routines which deal    */
/*  with these symbols are token_lookup() and token_name()  */
/*  These defines DO NOT NEED TO BE IN ALPHABETICAL ORDER.  */
/* -------------------------------------------------------- */

#define CDS_ADDFIELDS       (byte) 0x01
#define CDS_AGGREGATE       (byte) 0x02
#define CDS_ALL             (byte) 0x03
#define CDS_APPEND          (byte) 0x04
#define CDS_AS              (byte) 0x05
#define CDS_ASCENDING       (byte) 0x06
#define CDS_AVG_RT          (byte) 0x07
#define	CDS_BECOMES         (byte) 0x08
#define CDS_BEGINWORK       (byte) 0x09
#define CDS_CANCELSCAN      (byte) 0x0A
#define CDS_CLEARBUFFER     (byte) 0x0B
#define CDS_COMMITWORK      (byte) 0x0C
#define	CDS_COMPRESS        (byte) 0x0D
#define	CDS_COUNT_RT        (byte) 0x0E
#define CDS_CREATETABLE     (byte) 0x0F
#define CDS_DELETE          (byte) 0x10
#define CDS_DESCENDING      (byte) 0x11
#define CDS_DROPTABLE       (byte) 0x12
#define CDS_ENDSELECT       (byte) 0x13
#define CDS_EXCLUSIVELOCK   (byte) 0x14
#define CDS_EXTRACT         (byte) 0x15
#define	CDS_FINDAGAIN       (byte) 0x16
#define CDS_FINDFIRST       (byte) 0x17
#define CDS_FINDLAST        (byte) 0x18
#define CDS_FINDN           (byte) 0x19
#define CDS_FINDNEXT        (byte) 0x1A
#define CDS_FINDPREV        (byte) 0x1B
#define CDS_FROM            (byte) 0x1C
#define	CDS_GETLOCKINFO     (byte) 0x1D
#define CDS_CONNECT         (byte) 0x1E
#define CDS_INTO            (byte) 0x1F
#define CDS_INSERT          (byte) 0x20
#define CDS_KEYCHANGE       (byte) 0x21
#define CDS_LOCK            (byte) 0x22
#define	CDS_LOCKTABLE       (byte) 0x23
#define CDS_MAX_RT          (byte) 0x24
#define CDS_MIN_RT          (byte) 0x25
#define CDS_MULTIDISPLAY    (byte) 0x26
#define CDS_NOADDFIELDS     (byte) 0x27
#define CDS_NODELETE        (byte) 0x28
#define CDS_NOKEYCHANGE     (byte) 0x29
#define CDS_NOLOCK          (byte) 0x2A
#define	MOA_CDS_NORESET     (byte) 0x2B
#define	CDS_NOWAIT          (byte) 0x2C
#define	CDS_READLOCK        (byte) 0x2D
#define CDS_RECORDCOUNT     (byte) 0x2E
#define CDS_RENAMETABLE     (byte) 0x2F
#define CDS_REPLACE         (byte) 0x30
#define MOA_CDS_RESET       (byte) 0x31
#define CDS_RESETLOG        (byte) 0x32
#define CDS_ROLLBACKWORK    (byte) 0x33
#define CDS_SELECT          (byte) 0x34
#define CDS_SETRECORD       (byte) 0x35
#define	CDS_SORT            (byte) 0x36
#define	CDS_SUM_RT          (byte) 0x37
#define CDS_UNLOCK          (byte) 0x38
#define	CDS_UNLOCKTABLE     (byte) 0x39
#define CDS_UPDATE          (byte) 0x3A
#define	CDS_WAIT            (byte) 0x3B
#define CDS_WHERE           (byte) 0x3C
#define CDS_WORK            (byte) 0x3D
#define CDS_DISCONNECT      (byte) 0x3E
#define CDS_STATUS          (byte) 0x3F
#define CDS_JOIN_RT         (byte) 0x40
#define CDS_PRESERVE        (byte) 0x41
#define CDS_ARRAY        	(byte) 0x42
#define CDS_SPARSE			(byte) 0x43
#define CDS_GENERATED		(byte) 0x44
#define CDS_UNIQUE			(byte) 0x45
#define CDS_SEMIDISTINCT	(byte) 0x46
#define CDS_CLEAR			(byte) 0x47
#define CDS_POP				(byte) 0x48
#define CDS_PUSH			(byte) 0x49
#define CDS_STARTCAPTURE	(byte) 0x4A
#define CDS_STOPCAPTURE		(byte) 0x4B
#endif

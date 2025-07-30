/* ----------------------------------------------------------- */
/*  1-byte tokens corresponding to each statement in the OFB   */
/*  language.  Routines which handle these tokens are          */
/*  pcllookup() and pclkeyword().                              */
/* ----------------------------------------------------------- */

#ifndef OFBWORDS_H
#define OFBWORDS_H

// -----------------------------------------------
// the following four verbs don't generate tokens,
// and therefore don't need to have the 0x80 bit
// set.  Therefore, they are defined to match
// the dictionaries to which correspond.
// -----------------------------------------------
#define	PROC			(byte) 10	/* these numbers should match	*/
#define	LISTLIST		(byte) 6	/* dataman's dictionary numbers	*/
#define	SCREEN			(byte) 9
#define DOCUMENT		(byte) 8
#define OFBASIC         (byte) 10
#define WINMENU         (byte) 19 

// ----------------------------------------------------------------
// Olivetti Financial BASIC (OFB) specific verbs are in the
// range 0x60 to 0xFB.  This is broken into to two sub-ranges
// so the verb switch case in pcl.c can be broken into two
// switch cases so the MSVC debugger can set breakpoints in
// the switch case.
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// The following macro coorelates to the BIG if - else statement
// in pcl.c that contains two large switch statements.
// Be very careful adding verbs as those verbs that are greater
// than 0x60 but less than 0xB0 should be in the first major switch
// case in pcl.c.  Those verbs that are greater than and equal to
// 0xB0 and less than 0xFC should be in the second major switch
// case in pcl.c
// ----------------------------------------------------------------
#define OFB_DEMARCATION_LINE  (byte) 0xB0

// ---------------------------------
// OFB verbs in range 0x60 - 0xAF
// ---------------------------------
#define ADDTOTAL        (byte) 0x60
#define ALLCONTROLS		(byte) 0x61		/* added in v8.13 */
#define ALTERBUTTON  	(byte) 0x62		/* added in v7.11 */
#define ALTERCONTROL  	(byte) 0x63		/* added in v8.13 */
#define ALTERCONTROLSET	(byte) 0x64		/* added in v8.13 */
#define BAILOUT    	    (byte) 0x65
#define BREAK           (byte) 0x66
#define CALCULATOR      (byte) 0x67
#define CALENDAR        (byte) 0x68
#define CALL            (byte) 0x69		// Was OFB_GOSUB
#define CASE            (byte) 0x6A
#define CDS             (byte) 0x6B
#define CLEANSTART      (byte) 0x6C
#define CLEARFIELD      (byte) 0x6D
#define CLEARWINDOW     (byte) 0x6E
#define COLLECT         (byte) 0x6F
#define COMBOPICK       (byte) 0x70
#define COMBOVERIFY     (byte) 0x71		// Was WNWRITE
#define COMM            (byte) 0x72
#define CONTINUE        (byte) 0x73
#define CRT             (byte) 0x74
#define CUTSCREEN    	(byte) 0x75		/* added in v7.11 */
#define DDE             (byte) 0x76
#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG      	    (byte) 0x77
#define DEFAULT         (byte) 0x78
#define DEMO            (byte) 0x79
#define DISABLECONTROL  (byte) 0x7A
#define DPRINT          (byte) 0x7B
#define DRAW            (byte) 0x7C
#define DUPFIELD        (byte) 0x7D
#define DUPTRAN         (byte) 0x7E
#define EDENPASTE       (byte) 0x7F
#define ELSE            (byte) 0x80
#define ENABLECONTROL   (byte) 0x81
#define ENDCOLLECTS     (byte) 0x82
#define ENDDOCUMENT	    (byte) 0x83
#define ENDFORM 	    (byte) 0x84
#define ENDIF           (byte) 0x85
#define ENDLIST         (byte) 0x86
#define ENDPROC         (byte) 0x87
#define ENDSCREEN	    (byte) 0x88
#define ENDSWITCH       (byte) 0x89
#define ENDTRAN         (byte) 0x8A
#define ENDUSE    	    (byte) 0x8B
#define ERRMSG          (byte) 0x8C
#define EXEC            (byte) 0x8D
#define EXIT            (byte) 0x8E		/* also used by PMENU code */
#define FPRINT          (byte) 0x8F
#define FYIMSG          (byte) 0x90
#define GETKEY          (byte) 0x91
#define GOTO            (byte) 0x92
#define HELPSTATEMENT   (byte) 0x93
#define HIDECONTROL     (byte) 0x94
#define HIDEFIELD       (byte) 0x95
#define INITSTATEMENT   (byte) 0x96
#define JOINSCREEN   	(byte) 0x97		/* added in v7.11 */
#define KEYDEF          (byte) 0x98
#define LET             (byte) 0x99
#define LINK            (byte) 0x9A
#define LOCALPRINT      (byte) 0x9B
#define LOOKUP          (byte) 0x9C
#define MEMBER          (byte) 0x9D
#define MOVE            (byte) 0x9E
#define NEXTFIELD       (byte) 0x9F
#define NOP             (byte) 0xA0
#define OFB_LOOP        (byte) 0xA1		// Was PCL ENDLOOP
#define PROTFIELD       (byte) 0xA2
#define RESETVALDONE    (byte) 0xA3
#define SETVALDONE      (byte) 0xA4
#define SHOWCONTROL     (byte) 0xA5
#define SHOWFIELD       (byte) 0xA6
#define SUBTOTAL        (byte) 0xA7
#define UNPROTFIELD     (byte) 0xA8

// --------------------------------------
// 0xA9 - 0xAF available for verb tokens
// --------------------------------------

// -------------------------------
// OFB verbs in range 0xB0 - 0xFB
// -------------------------------

#define OFB_BEEP        (byte) 0xB0		// Was PCL BELL
#define OFB_CONTROL	    (byte) 0xB1
#define OFB_DECLARE	    (byte) 0xB2
#define OFB_DLL		    (byte) 0xB3
#define OFB_DO          (byte) 0xB4		// Was PCL LOOP
#define OFB_ELSEIF      (byte) 0xB5		// MOA Release 2.0
#define OFB_ELSEIF2     (byte) 0xB6		// MOA Release 2.0
#define OFB_END		    (byte) 0xB7
#define OFB_ENDWHILE    (byte) 0xB8		// MOA Release 2.0
#define OFB_LOCAL       (byte) 0xB9		// Was WNREAD
#define OFB_MSGBOX      (byte) 0xBA
#define OFB_ODBC        (byte) 0xBB		// NOT in use yet...
#define OFB_SELECT      (byte) 0xBC		// Was PCL SWITCH
#define OFB_SUB         (byte) 0xBD
#define OFB_TOOLBAR     (byte) 0xBE		// MOA Release 2.0
#define OFB_TRACE  	    (byte) 0xBF
#define OFB_WHILE       (byte) 0xC0		// MOA Release 2.0
#define PDM_PDM         (byte) 0xC1
#define PICKLIST        (byte) 0xC2
#define PICKVERIFY      (byte) 0xC3
#define PLAY   	        (byte) 0xC4		/* added in v7.11 */
#define PMENU           (byte) 0xC5
#define POPUP           (byte) 0xC6
#define PRINTDOC        (byte) 0xC7
#define PRT             (byte) 0xC8
#define RECORD 	        (byte) 0xC9		/* added in v7.11 */
#define REDRAW 	        (byte) 0xCA
#define REFRESH         (byte) 0xCB
#define RESET           (byte) 0xCC
#define RESETFLAG       (byte) 0xCD
#define RETURN          (byte) 0xCE		/* also used by PMENU code */
#define SETAUXID        (byte) 0xCF
#define SETBCOLOR       (byte) 0xD0
#define SETFACE         (byte) 0xD1
#define SETFCOLOR       (byte) 0xD2
#define SETFLAG         (byte) 0xD3
#define SETFONT         (byte) 0xD4
#define SETPALETTE      (byte) 0xD5
#define SETSIZE         (byte) 0xD6
#define SETTIMER        (byte) 0xD7		/* added for Relese 4.0 */
#define SETTITLE        (byte) 0xD8
#define SINGLESTEP 	    (byte) 0xD9
#define SPY             (byte) 0xDA
#define SSBP1  	        (byte) 0xDB		/* BREAKPOINT opcodes */
#define SSBP2  	        (byte) 0xDC		/* can't be compiled */
#define SSBP3  	        (byte) 0xDD
#define SSBP4  	        (byte) 0xDE
#define SSBP5  	        (byte) 0xDF
#define STACK      	    (byte) 0xE0
#define STARTTRAN       (byte) 0xE1
#define SWAPDUP         (byte) 0xE2
#define TMENU           (byte) 0xE3
#define TRANDISPLAY	    (byte) 0xE4
#define UNDISPLAY       (byte) 0xE5
#define UNGETKEY        (byte) 0xE6
#define USE             (byte) 0xE7
#define VERIFYOFF  	    (byte) 0xE8
#define VERIFYON   	    (byte) 0xE9
#define DISPLAY         (byte) 0xEA
#define IF              (byte) 0xEB
#define NAP             (byte) 0xEC

// --------------------------------------
// 0xED - 0xFB available for verb tokens
// --------------------------------------
/* -------------------------------------------------------- */
/*  One-byte tokens which are not OFB statements, but are   */
/*  reserved words nonetheless.  The routines which deal    */
/*  with these symbols are token_lookup() and token_name()  */
/* -------------------------------------------------------- */


/* the following *must not* overlap the values of the TYPE_xxx */
/* defines in pcldefines.h, nor the PCLop values, that's why   */
/* they start with 0x01.  But, they're not verbs, so values    */
/* can overlap those of verbs...                               */

#define DDERETVAL       (byte) 0x10
#define CDSRETVAL       (byte) 0x11
#define COMMRETVAL      (byte) 0x12
#define PRTRETVAL       (byte) 0x13
#define CLEAR   		(byte) 0x14
#define POP     		(byte) 0x15
#define PUSH    		(byte) 0x16
#define CENTER          (byte) 0x17
#define CURRENTFIELD    (byte) 0x18
#define CURRENTTRAN     (byte) 0x19
#define FROM            (byte) 0x1A
#define ARRAY           (byte) 0x1B
#define LASTKEY         (byte) 0x1C
#define LEFT            (byte) 0x1D
#define NOCOLLECT       (byte) 0x1E
#define NOWAIT          (byte) 0x1F
#define OFF             (byte) 0x20
#define	FILL			(byte) 0x21
#define	THEN			(byte) 0x22
#define RETVAL          (byte) 0x23
#define RIGHT           (byte) 0x24
#define STATUSLINE      (byte) 0x25
#define TO              (byte) 0x26
#define TRAN    		(byte) 0x27
#define WIDTH           (byte) 0x28
#define SUBSTR          (byte) 0x29
#define MATCH           (byte) 0x2A
#define TESTFLAG        (byte) 0x2B
#define COMMHDR         (byte) 0x2C
#define ON              (byte) 0x2D
#define WINDOW          (byte) 0x2E

// ----------------
// 0x2F is unused 
// ----------------

#define FIRSTPTR		(byte) 0x30
#define	MSGPTR 			(byte) 0x30
#define	MATCHBEGIN      (byte) 0x31
#define	MATCHEND        (byte) 0x32
#define	TEMPPTRA        (byte) 0x33
#define	TEMPPTRB        (byte) 0x34
#define	TEMPPTRC        (byte) 0x35
#define	MSGBEGIN        (byte) 0x36
#define	MSGEND			(byte) 0x37
#define LASTPTR 		(byte) 0x37

#define NOVAL           (byte) 0x39

#define SMATCH          (byte) 0x3B
#define NEXTKEY         (byte) 0x3C
#define BITSOFF         (byte) 0x3D
#define NOTMATCH        (byte) 0x3E
#define NOTSMATCH       (byte) 0x3F

#define FIRSTAT 		(byte) 0x40
#define	ATMSG  			(byte) 0x40	/* these must match the above */
#define	ATMATCHBEGIN    (byte) 0x41 /* set of pointer names but   */
#define	ATMATCHEND      (byte) 0x42 /* with the 0x40 bit set...   */
#define	ATTEMPPTRA      (byte) 0x43
#define	ATTEMPPTRB      (byte) 0x44
#define	ATTEMPPTRC      (byte) 0x45
#define	ATMSGBEGIN      (byte) 0x46
#define	ATMSGEND		(byte) 0x47
#define LASTAT  		(byte) 0x47

#define IDLE  	    	(byte) 0x48		/* added for Relese 4.0 */
#define INTERVAL  		(byte) 0x49		/* added for Relese 4.0 */
#define TIMEOFDAY  		(byte) 0x4A		/* added for Relese 4.0 */
#define NOINPUT    		(byte) 0x4B		/* added for Relese 4.0 */

#ifdef SIXTEENBITCHARS
#define VERTICAL        (byte) 0x4F     /* chinese vertical print */
#endif /* SIXTEENBITCHARS */

#define OFB_LIKE   		(byte) 0x50

#define ALL             (byte) 0xB9
#define UPDATE          (byte) 0x7E

/* CDS tokens moved to separte token lookup table */
/* tokens in range 0x48-0x49 & 0xB0-0xCF  &       */
/* 0x70-0x80 are now available.                   */

/* ---------------------------------------------------------- */
/*  The following grouping is attribute/objects which can be  */
/*  attached to field names in OFB sources.  They must be in  */
/*  alpha order, with no 'holes', see make_suffix() in module */
/*  pcldecomp.c  (it must be modified if any change here)     */
/* ---------------------------------------------------------- */

#define FIRSTSUFFIX     (byte) 0x50
#define AUTOSKIP        (byte) 0x50
#define AUTOCLEAR       (byte) 0x51
#define CALCMODE        (byte) 0x52
#define DISPLAYLEN      (byte) 0x53
#define DURINGENTRY     (byte) 0x54
#define EDIT            (byte) 0x55
#define ERROROBJ        (byte) 0x56
#define FORCEUC         (byte) 0x57
#define ATENTRY         (byte) 0x58
#define HELP            (byte) 0x59
#define HIDDEN          (byte) 0x5A
#define HIGHLITE        (byte) 0x5B
#define LENGTH          (byte) 0x5C
#define ID              (byte) 0x5D
#define ALLOWMODIFY     (byte) 0x5E
#define NOPLDISPLAY     (byte) 0x5F
#define NUMCHARS        (byte) 0x60
#define AUTODUP         (byte) 0x61
#define DYNVAL          (byte) 0x62
#define AUTOEXTRACT     (byte) 0x63
#define POSTVAL         (byte) 0x64
#define PRE             (byte) 0x65
#define PROTECTED       (byte) 0x66
#define REQUIRED        (byte) 0x67
#define VALDONE         (byte) 0x68
#define AUTOAPPEND      (byte) 0x69
#define BLANKEDIT       (byte) 0x6A
#define SIGNALLOWED     (byte) 0x6B
#define PLACES          (byte) 0x6C
#define USERDATA        (byte) 0x6D
#define ALLOWDUP        (byte) 0x6E
#define AUTOIME         (byte) 0x6F
#define LASTSUFFIX      (byte) 0x6F


// -----------------------------------------------------
// The following macros ***MUST*** correspond the the
// macros defined in dataman.h with equivalent values.
// -----------------------------------------------------
#define FIRSTPREFIX     (byte) 1
#define	PreBIT			(byte) 1
#define	PreDOC			(byte) 2
#define	PreDSC			(byte) 3
#define	PreFLD			(byte) 4
#define	PreICN			(byte) 5
#define	PreLST			(byte) 6
#define	PreMNB			(byte) 7
#define	PreMNU			(byte) 8
#define	PrePAT			(byte) 9
#define	PrePCL			(byte) 10
#define	PreOFB			(byte) 10
#define	PrePIC			(byte) 11
#define	PrePLB			(byte) 12
#define	PrePMB			(byte) 13
#define	PrePMU			(byte) 14
#define	PreSCR			(byte) 15
#define	PreTAG			(byte) 16
#define	PreUDO			(byte) 17
#define	PreCSR			(byte) 18
#define	PrePDM			(byte) 19
#define	PreWMU			(byte) 20
#define	PreMLD			(byte) 21
#define LASTPREFIX      (byte) 21


#define REFERENCED      (byte) 0xFE		
#define FOREWARDREFERENCED		REFERENCED
#define BADCOMPILE		(byte) 0xFF		/* <=== this is not really a verb */

#endif

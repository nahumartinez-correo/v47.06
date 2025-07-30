/**********************************************************************/
/*                                                                    */
/*     Defines used by the PCL interpreter and compiler, and any      */
/*     other routines referencing PCL stuff...                        */
/*                                                                    */
/**********************************************************************/
#ifndef PCLDEFINES_H
#define PCLDEFINES_H

#define EXITNOW		(-99)		/* used by TMENU, menus.c */
#define SYSTEMDRIVER (-9275600)	/* an EXTREEEEEEEMLY unlikely keycode */

#define RTFLAG_intran	0x0001	/* transaction is active */
#define RTFLAG_batch_mode 0x0002/* TRUE if -x option was used */
#define RTFLAG_reentry	0x0004	/* TRUE if reentry in progress */
#define RTFLAG_prtreset	0x0008	/* TRUE if last print was RESET out of */
#define RTFLAG_inmenu 	0x0010	/* menu system is active */
#define RTFLAG_ATkbd  	0x0020	/* AT-Style keyboard */
#define RTFLAG_M4900kbd 0x0040	/* Model 4900 keyboard */
#define RTFLAG_colormon 0x0080	/* color monitor in use */
#define RTFLAG_9inmon   0x0100	/* 9" monitor in use */
#define RTFLAG_mono     0x0200	/* monochrome WP-V */
#define RTFLAG_color    0x0400	/* color WP-V */
#define RTFLAG_WEB      0x0800	/* WP-V Emulation Board */
#define RTFLAG_WP6      0x1000	/* WP-6 */
#define RTFLAG_nowindow 0x2000	/* no window currently allocated */
#define RTFLAG_listtran	0x4000	/* list transaction is active */
#define RTFLAG_Win32NT  0x8000	/* Windows 32 API on NT */
#define RTFLAG_Win32c   0x10000	/* Windows 32 API on Chicago */

#define PCLMAXWORDS		50		/* max PCL words per line (OBSOLETE!)    */
#define PCLLINESIZE		4096	/* max size of an input line             */
#define MAXLABELS		50		/* max number of labels allowed          */
#define	NUMFUNKEYS		33		/* number of function keys definable */

#define PCLSTACKDEPTH	25		/* # of levels deep we can stack calls   */

/* ======= Bit values for 'flags' argument to pcl() ==================== */
#define PCL_NOCOLLECT	0x01	/* display only, no data collect         */
#define PCL_DEBUGMODE	0x02	/* run pcl() in debugging mode           */
#define PCL_JSCANMODE	0x04	/* run pcl() in journal scan mode        */
#define PCL_CORRECTION	0x08	/* run pcl() in correction mode          */
#define PCL_FILL     	0x10	/* copy data from DUP area to STORAGE    */
#define PCL_NODISPLAY	0x20	/* don't display any of the screens      */
								/* used for reentry...					 */
#define PCL_LINK       	0x40	/* TRUE if LINK statement, for STARTTRAN */

/* ========== Bit values for WNREAD or WNWRITE statement =============== */
#define WN_KBPINP	0x10	/* read from Pin Pad                         */
#define WN_KBMAGC 	0x20	/* read from Keyboard's magstripe            */
#define WN_MAGCRD	0x40	/* read from separate magstripe device       */
#define WN_NOKEYS	0x08	/* don't allow keyboard input to abort       */
#define WN_TRACK1	0x01	/* track selection                           */
#define WN_TRACK2	0x02	/* track selection                           */
#define WN_TRACK3	0x03	/* track selection                           */

/* ======= Tokens used in compiled PCL procedures ====================== */
/* These *MUST NOT* conflict with the tokens listed in the second group  */
/* in pclwords.h                                                         */
/* --------------------------------------------------------------------- */
#define TYPE_ICON1		0x81	// Local constant, type INT 1-byte
#define TYPE_ICON2		0x82	// Local constant, type INT 2-bytes
#define TYPE_ICON3		0x83	// Local constant, type INT 3-bytes
#define TYPE_ICON4		0x84	// Local constant, type INT 4-bytes
#define TYPE_XCON1		0x85	// Prefix for 1-byte hex constant
#define TYPE_XCON2		0x86	// Prefix for 2-byte hex constant
#define TYPE_XCON3		0x87	// Prefix for 3-byte hex constant
#define TYPE_XCON4		0x88	// Prefix for 4-byte hex constant
#define TYPE_CCON 		0x89	// Prefix for 1-byte char constant
#define TYPE_SCON 		0x8A	// Prefix for string constant
#define TYPE_STRING		0x8B	// Local variable, type STRING
#define TYPE_DCON		0x8C	// Prefix for DECWORK constant
#define TYPE_INT		0x8D	// Local variable, type INT
#define TYPE_RETVAL		0x8E	// Token representing last return val
#define TYPE_CDSRETVAL	0x8F	// Token representing last return val
#define TYPE_COMMRETVAL	0x90	// Token representing last return val
#define TYPE_PRTRETVAL	0x91	// Token representing last return val
#define TYPE_NEXTKEY	0x92	// Token for next key in typeahead buff
#define TYPE_LASTKEY	0x93	// Token representing Lastkey from kbd
#define TYPE_ARRAY   	0x94	// Following items are index(s) & object
#define TYPE_DDERETVAL 	0x95	// Token representing last return val
#define TYPE_ARG     	0x96	// Following byte is no. of argument

#define PCL_op		0xE0		/* start of expression operators         */
#define	PCLplus		0xE0		/*  +   */
#define	PCLsub		0xE1		/*  -   */
#define	PCLmult		0xE2		/*  *   */
#define	PCLdiv		0xE3		/*  /   */
#define	PCLmod		0xE4		/*  %   */
#define	PCLneg		0xE5		/*  ~   */
#define PCLexp		0xE6		/* exponentiation	*/
                          		/*  ++ not allowed */
                       			/*  -- not allowed */
#define	PCLlt		0xE8		/*  <   */
#define	PCLgt		0xE9		/*  >   */
#define	PCLeq		0xEA		/*  ==  */
#define	PCLle		0xEB		/*  <=  */
#define	PCLge		0xEC		/*  >=  */
#define	PCLne		0xED		/*  !=  */
#define	PCLandb		0xEE		/*  &   */
#define	PCLorb		0xEF		/*  |   */
#define	PCLxorb		0xF0		/*  ^   */
#define	PCLand		0xF1		/*  &&  */
#define	PCLor		0xF2		/*  ||  */
#define	PCLshl		0xF3		/*  <<  */
#define	PCLshr		0xF4		/*  >>  */
#define	PCLlparen	0xF5		/*  (   */
#define	PCLrparen	0xF6		/*  )   */
#define	PCLassign	0xF7		/*  =   */
#define	PCLnot		0xF8		/*  !   */
#define	PCLminus	0xF9		/*  unary -   */
#define	PCLsmatch	0xFA		/*  MATCH operator for CDS  */
#define	PCLrmatch	0xFB		/*  RMATCH operator for CDS  */
#define	PCLnsmatch	0xFC		/*  NOTMATCH operator for CDS  */
#define	PCLnrmatch	0xFD		/*  NOTRMATCH operator for CDS  */
#define	PCLbitsoff	0xFE		/*  test bits off bit-wise operator */

#define OMITTEDPRTVAL	0x8000		/* omitted operand in PRT verb */
#define FLDARGPRTVAL	0x8001		/* field is arg of PRT verb */

/* ================ Opcodes for expression evaluation ============ */
#define OFB_ERR		(0)
#define BINOP		'B'
#define UNIOP		'U'
#define VAR			'V'
#define ALFA		'A'
#define DIGIT		'9'
#define INT			'I'

#define WP6			'6'			/* code for determining hardware type */

/* scroll bar ID's	*/
#define	PL_SCROLLBAR	77
#define RT_VSCROLLBAR	78
#define RT_HSCROLLBAR	79

/* ================ Internal codes for key characters ============ */
#include <ofbdefs.h>

#endif

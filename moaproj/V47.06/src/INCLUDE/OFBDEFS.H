/****x* Pendientes de agrupación/DEF:include:OFBDEFS
* NOMBRE
*   DEF:include:OFBDEFS
* DESCRIPCION
*   Comportamiento del objeto (obligatorio)
* ENTRADAS
*   Parámetros de entrada de la rutina, sean argumentos o campos que tiene en cuenta
* SALIDAS
*   Campos modificados como resultado o returns cuyos valores sean no triviales
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/
/* -------------------------------------------------------------- */
/*  Defines useful to the OFB programmer.  These symbols are the  */
/*  ones used by the C code in RT, PC, etc.  By using these       */
/*  symbols, rather than hard-coding the hex values, you protect  */
/*  yourself from future changes in keyboard and font handling.   */
/*     (This #include file is referenced by ppdefs.h)             */
/* -------------------------------------------------------------- */

#ifndef OFBDEFS_H
#define OFBDEFS_H

#define RTVERSION		20	/* Version 20, Release 2 series */
							/* make this 30 for Release 3   */

/* -------------------------------------------------------------- */
/*               Internal codes for key characters                */
/* -------------------------------------------------------------- */
/*  These are the codes that internal RT functions like the data  */
/*  collect driver, or the picklist driver, look for.  You MUST   */
/*  have set up some KEYDEFs to assign some of these during       */
/*  initialization, or RT won't be able to do much useful work    */
/*  with the keyboard.                                            */
/* -------------------------------------------------------------- */

#define RT_lowfence		0x180		/* RT_*** codes lie between */
#define RT_hifence		0x200		/* these two values */

#define RT_UP     		0x181
#define RT_DOWN     	0x182
#define RT_RIGHT     	0x183
#define RT_LEFT     	0x184
#define RT_PAGEUP		0x185
#define RT_PAGEDOWN		0x186
#define RT_SCROLLUP		0x187		/* shifted UPARROW   */
#define RT_SCROLLDOWN	0x188		/* shifted DOWNARROW */
#define RT_PGSCROLLUP	0x189		/* shifted PAGEUP    */
#define RT_PGSCROLLDOWN	0x18A		/* shifted PAGEDOWN  */
#define RT_ENTERFIELD	0x18B
#define RT_CLEARFIELD	0x18C
#define RT_TAB			0x18D
#define RT_BACKTAB		0x18E
#define RT_END			0x18F
#define RT_ESC			0x190
#define RT_SEND			0x191
#define RT_DUP			0x192
#define RT_BACKSPACE	0x193
#define RT_HOME     	0x194
#define RT_EXIT			0x195
#define RT_INSERT		0x196
#define RT_DELETE		0x197
#define RT_PRINTSCREEN	0x198	/* does LOCALPRINT, just like OFB verb */
#define RT_ALTPRINTSCREEN	0x199	/* does alternate local print (cal) */
#define RT_CLEARALL		0x19A	/* clear all fields on screen */
#define RT_PLUS     	0x19B	/* for use by the CALCULATOR function */
#define RT_MINUS    	0x19C	/* for use by the CALCULATOR function */
#define RT_MULTIPLY 	0x19D	/* for use by the CALCULATOR function */
#define RT_DIVIDE   	0x19E	/* for use by the CALCULATOR function */

#define RT_CUT		   	0x1A0	/* for use by DRAW MLFIELD statement */
#define RT_COPY		   	0x1A1	/* for use by DRAW MLFIELD statement */
#define RT_PASTE	   	0x1A2	/* for use by DRAW MLFIELD statement */

#define RT_FIELDLEFT  	0x1A3	/* cursor to start of field */
#define RT_FIELDRIGHT	0x1A4	/* cursor to end of field */

/* ------------------------------------------------------------------- */
/*  Equates useful to Data Collect, picklist key_filter routines, and  */
/*  possibly regular validation routines.  These aren't true keyboard  */
/*  characters -- instead they provide information to C or OFB code,   */
/*  or cause some special processing to take place.  They are          */
/*  generally ignored in places where a key value is being waited for, */
/*  like ERRMSG with a keyaction of 2 or 3.                            */
/*     ====== These 'key' codes are in the range 1F0-1FF hex ======    */
/* ------------------------------------------------------------------- */

#define RT_FIRST_NONKEY 0x1F0	/* fence for non-key keys */

#define RT_BAILOUT 		0x1F0	/* non-keyboard char, used in BAILOUT logic */
#define PL_CLICK		0x1FA	/* indicates a click that is NOT a selection */
#define PL_SELECT		0x1FB	/* indicates mouse SELECTION */
#define DC_MOUSE		0x1FC	/* used by internal mouse event logic */

/* ------------------------------------------------------------------- */
/*  These data collect specific values, when placed in the keyboard    */
/*  typeahead buffer by an UNGETKEY statement, cause data collect (the */
/*  COLLECT statement) to do some special processing.                  */
/*  See the section of the OFBDOC manual entitled "Data Collect".      */
/* ------------------------------------------------------------------- */

#define DC_FUNC1		0		/* do nothing (useful for during_entry) */
#define DC_FUNC2		0x1FD	/* do nothing but reestablish CURRENTFIELD */
#define DC_FUNC3		0x1FE	/* do an auto-ENTERFIELD */
#define DC_FUNC4		0x1FF	/* if field is full do auto-ENTERFIELD */
#define DC_FUNC5		0x1F9	/* reestablish datacollect via NEXTFIELD */
#define DC_FUNC_BAD		0x1F8	/* always generates badinput() */
#define DC_CURSOR_ON	0x1F7	/* Used in DISPLAY/POPUP only */
#define DC_CURSOR_OFF	0x1F6	/* Used in DISPLAY/POPUP only */

#define RT_LAST_NONKEY  0x1FF	/* fence for non-key keys */

#define PL_SCROLLTO		0x4000	/* starting offset for picklist scrollbar */
#define PL_SCROLLSELECT 0x2000	/* offset for picklist selection */

/*eject*/
/* ----------------------------------------------------------- */
/*  The following are function key values as returned by RT's  */
/*  low-level keyboard handler.                                */
/*  If some of these aren't converted (via KEYDEFs) to some of */
/*  the above values, then RT won't work very well.  e.g. on   */
/*  the AT-style keyboard, if RT_F28 isn't defined to be       */
/*  RT_ENTER, then there is no ENTER key in data collect.      */
/* ----------------------------------------------------------- */

#define RT_F1			0x1B1		/* These MUST be contiguous */
#define RT_F2			0x1B2
#define RT_F3			0x1B3
#define RT_F4			0x1B4
#define RT_F5			0x1B5
#define RT_F6			0x1B6
#define RT_F7			0x1B7
#define RT_F8			0x1B8
#define RT_F9			0x1B9
#define RT_F10			0x1BA
#define RT_F11			0x1BB
#define RT_F12			0x1BC
#define RT_F13			0x1BD
#define RT_F14			0x1BE
#define RT_F15			0x1BF
#define RT_F16			0x1C0
#define RT_F17			0x1C1
#define RT_F18			0x1C2
#define RT_F19			0x1C3
#define RT_F20			0x1C4
#define RT_F21			0x1C5
#define RT_F22			0x1C6
#define RT_F23			0x1C7
#define RT_F24			0x1C8
#define RT_F25			0x1C9
#define RT_F26			0x1CA
#define RT_F27			0x1CB
#define RT_F28			0x1CC
#define RT_F29			0x1CD
#define RT_F30			0x1CE
#define RT_F31			0x1CF
#define RT_F32			0x1D0
#define RT_F33			0x1D1		/* not available on Model 4900 kbd */

/* The following bit values may be OR'ed with the above RT function keys */
#define RT_SHIFT		0x0200		/* function key was Shifted */
#define RT_ALT			0x0400		/* function key was Alt-Shifted */
#define RT_CTRL			0x0800		/* function key was Control-Shifted */
#define RT_NPAD			0x1000		/* numeric key was from the Numeric Pad */



/*eject*/
/* ----------------------------------------------------------- */
/*              Box object type definitions                    */
/* ----------------------------------------------------------- */

#define RT_NOBORDER	   0	/* Borderless box */
#define RT_SINGLELINE  1	/* Single line border */
#define RT_DOUBLELINE  2	/* Double line border */
#define RT_TRIPLELINE  3	/* Triple line border */
#define RT_DROPSHADOW  4	/* Drop shadowed border */

/* ----------------------------------------------------------- */
/*             Text object attribute definitions               */
/*     (these are related to the DTxxx defines in tdspl.h)     */
/* ----------------------------------------------------------- */

#define RT_NORMAL 		0x00
#define RT_INVISIBLE	0x01	/* non-displayed */
#define RT_BOLD			0x02	/* Bold (char width unchanged) */
#define RT_UNDERLINE	0x04	/* Underline */
#define RT_ITALICS		0x08	/* Italics */
#define RT_GHOST		0x10	/* Ghosted */
#define RT_OUTLINE		0x20	/* Outlined */
#define RT_SHADOW		0x40	/* drop shadowed	*/
#define RT_REVERSED 	0x80	/* video-reversed (same as DTREVERSE) */

/* ----------------------------------------------------------- */
/*                   WPV Color Definitions                     */
/* ----------------------------------------------------------- */

#define RTC_DATACOLLECT 	0	/* Black on white */
#define RTC_POPUP			0	/* Black on white */
#define RTC_MENU			1	/* White on blue */
#define RTC_PICKLIST		1	/* White on blue */
#define RTC_CALENDAR		1	/* White on blue */
#define RTC_CALCULATOR		1	/* White on blue */
#define RTC_FYI				2	/* Black on yellow */
#define RTC_HELP			2	/* Black on yellow */
#define RTC_BOX				2	/* Black on yellow */
#define RTC_ERROR			3	/* White on red */

/* ------------------------------------------------------------- */
/*  symbols for flags in DRAW MLFIELD and DRAW MLTEXT statements */
/* ------------------------------------------------------------- */

#define ML_LEFT_AUTO_MULTILINE 		0  /* standard defaults */
#define ML_CENTER_AUTO_MULTILINE	1
#define ML_RIGHT_AUTO_MULTILINE		2

#define ML_LEFT_NOWRAP_MULTILINE	4
#define ML_CENTER_NOWRAP_MULTILINE	5
#define ML_RIGHT_NOWRAP_MULTILINE	6

#define ML_LEFT_AUTO_NOMULTI 		8
#define ML_CENTER_AUTO_NOMULTI		9
#define ML_RIGHT_AUTO_NOMULTI		10

#define ML_LEFT_NOWRAP_NOMULTI 		12
#define ML_CENTER_NOWRAP_NOMULTI	13
#define ML_RIGHT_NOWRAP_NOMULTI		14

/* ----------------------------------------------------------- */
/*             Manual required definitions                     */
/* ----------------------------------------------------------- */

/* These defines represent the bit settings used by the manual routine		*/
/* to determine the index type(s) to use when locating a 'smart help'		*/
/* index point																*/
#define		MANFLD			1
#define		MANSCR			2
#define		MANSCRFLD		4
#define		MANPCL			8		/* transactions are OFB routines		*/
#define		MANSTRING		16
#define		MANMNU			32
#define		MANALL			63		/* all bits set, do all index searches	*/
#define		MANNOINDEX		128

#define		HELP_NO_ACTION	0
#define		HELP_BEEP		1
#define		HELP_FYI		2
#define		HELP_MANUAL		3

#define		MAN_DEBUG_ON		1
#define		MAN_DEBUG_OFF		0

/* These defines are the error messages returned from manual */
#define	MAN_OPEN_ERROR			-100	/* Error opening manual file */
#define MAN_READ_ERROR			-101	/* Error reading manual file */
#define MAN_WRONG_VERSION		-102	/* Incompatible software versions */
#define MAN_INDEX_NOT_FOUND		-103	/* Index not found; don't go to pg 0 */
#define MAN_BAD_LIST			-104	/* Invalid options list */


/* ----------------------------------------------------------- */
/*             Calculator required definitions                 */
/* ----------------------------------------------------------- */

/* calculator mode	*/
#define	CALC_DOLLAR			0
#define	CALC_FLOAT			1

/* calculator tape load options	*/
#define	CALC_NO_LOAD		0
#define	CALC_LOAD_ON_ENTRY	1
#define	CALC_LOAD_ON_F2		2
#define	CALC_LOAD_ON_BOTH	3

/* calculator tape save options	*/
#define	CALC_NO_SAVE		0
#define	CALC_SAVE_ON_INSERT	1
#define	CALC_SAVE_ON_F1		2
#define	CALC_SAVE_ON_BOTH	3

/* calculator result codes	*/
#define	CALC_ALREADY_ACTIVE	(-100)	/* calculator already in use			*/
#define	CALC_WONT_FIT		(-101)	/* calculator won't fit on display		*/
#define	CALC_TAPE_NOT_SAVED	0		/* normal exit, tape not saved			*/
#define	CALC_TAPE_SAVED		1		/* normal exit, tape saved in tape field*/


	/* ---------------------------------------------------------- */
	/*  codes which should be returned by custom C routines when  */
	/*  bad arguments are encountered.							  */
	/* ---------------------------------------------------------- */

#define	CSR_FAIL_ARGCOUNT	-1001	/* wrong number of arguments passed */
#define	CSR_FAIL_ARGTYPE	-1002	/* wrong type of argument passed	*/
#define CSR_FAIL_FIELDTYPE	-1003	/* wrong field type(s) passed		*/


 	/*--------------------------------------------------------*/
 	/*                   csr:listcsr opcodes                  */
	/*--------------------------------------------------------*/

#define	GETLIST		0
#define	GETMEMBER	1
#define	GETELEMENT	2

#define	MAX_LIST_STRING	128

#define	LISTERR		(-1)


// ---------------------------------------------------------------------
//  MSGBOX Defines, these match up with the similarly named symbols in:
//		\MSVCNT\INCLUDE\WINUSER.H
// ---------------------------------------------------------------------
#define OFB_OK                       0x00000000
#define OFB_OKCANCEL                 0x00000001
#define OFB_ABORTRETRYIGNORE         0x00000002
#define OFB_YESNOCANCEL              0x00000003
// para que por defecto sea el primer boton o el segundo o el tercero
#define OFB_YESNOCANCEL1             0x00000003
#define OFB_YESNOCANCEL2             0x00000103
#define OFB_YESNOCANCEL3             0x00000203

#define OFB_YESNO                    0x00000004
// para que por defecto sea el primer boton o el segundo
#define OFB_YESNO1                   0x00000004
#define OFB_YESNO2                   0x00000104
#define OFB_RETRYCANCEL              0x00000005

#define OFB_ICONHAND                 0x00000010
#define OFB_ICONQUESTION             0x00000020
#define OFB_ICONEXCLAMATION          0x00000030
#define OFB_ICONASTERISK             0x00000040

#define OFB_ICONINFORMATION          OFB_ICONASTERISK
#define OFB_ICONSTOP                 OFB_ICONHAND

//MFM 50571
#define OFB_STOPYESNO1                   0x00000014

#define OFB_DEFBUTTON1               0x00000000
#define OFB_DEFBUTTON2               0x00000100
#define OFB_DEFBUTTON3               0x00000200

#define OFB_APPLMODAL                0x00000000
#define OFB_SYSTEMMODAL              0x00001000
#define OFB_TASKMODAL                0x00002000

#define OFB_NOFOCUS                  0x00008000
#define OFB_SETFOREGROUND            0x00010000
#define OFB_DEFAULT_DESKTOP_ONLY     0x00020000

#define OFB_TYPEMASK                 0x0000000F
#define OFB_ICONMASK                 0x000000F0
#define OFB_DEFMASK                  0x00000F00
#define OFB_MODEMASK                 0x00003000
#define OFB_MISCMASK                 0x0000C000

// ---------------------------------------------------------
//  Dialog Box Command IDs, these match up with symbols in:
//		\MSVCNT\INCLUDE\WINUSER.H
// ---------------------------------------------------------
#define OFB_IDOK                	1
#define OFB_IDCANCEL            	2
#define OFB_IDABORT             	3
#define OFB_IDRETRY             	4
#define OFB_IDIGNORE            	5
#define OFB_IDYES               	6
#define OFB_IDNO                	7

// ==========================================
// Menu flags for Add/Check/EnableMenuItem()
// ==========================================
#define MOA_INSERT          0x00000000
#define MOA_CHANGE          0x00000080
#define MOA_APPEND          0x00000100
#define MOA_DELETE          0x00000200
#define MOA_REMOVE          0x00001000

#define MOA_BYCOMMAND       0x00000000
#define MOA_BYPOSITION      0x00000400

#define MOA_SEPARATOR       0x00000800

#define MOA_ENABLED         0x00000000
#define MOA_GRAYED          0x00000001
#define MOA_DISABLED        0x00000002

#define MOA_UNCHECKED       0x00000000
#define MOA_CHECKED         0x00000008
#define MOA_USECHECKBITMAPS 0x00000200

#define MOA_STRING          0x00000000
#define MOA_BITMAP          0x00000004
#define MOA_OWNERDRAW       0x00000100

#define MOA_POPUP           0x00000010
#define MOA_MENUBARBREAK    0x00000020
#define MOA_MENUBREAK       0x00000040

#define MOA_UNHILITE        0x00000000
#define MOA_HILITE          0x00000080

#define MOA_SYSMENU         0x00002000
#define MOA_HELP            0x00004000
#define MOA_MOUSESELECT     0x00008000

#define RT_ALT_UP	  		1485
#define RT_ALT_DOWN     	1488
#define RT_ALT_RIGHT     	1489
#define RT_ALT_LEFT     	1487
#define RT_ALT_PAGEUP		1474
#define RT_ALT_PAGEDOWN		1482
#define RT_ALT_SCROLLUP		1997
#define RT_ALT_SCROLLDOWN	2000
#define RT_ALT_PGSCROLLUP	1986
#define RT_ALT_PGSCROLLDOWN	1994
#define RT_ALT_END			1481
#define RT_ALT_SEND			1468
#define RT_ALT_HOME     	1473
#define RT_ALT_INSERT		1472
#define RT_ALT_DELETE		1480
#define RT_ALT_PRINTSCREEN	1469
#define RT_ALT_PLUS     	1483
#define RT_ALT_MINUS    	1478
#define RT_ALT_NUMLOCK   	1475

#define RT_ALT_F1			0x5B1
#define RT_ALT_F2			0x5B2
#define RT_ALT_F3			0x5B3
#define RT_ALT_F4			0x5B4
#define RT_ALT_F5			0x5B5
#define RT_ALT_F6			0x5B6
#define RT_ALT_F7			0x5B7
#define RT_ALT_F8			0x5B8
#define RT_ALT_F9			0x5B9
#define RT_ALT_F10			0x5BA
#define RT_ALT_F11			0x5BB
#define RT_ALT_F12			0x5BC
#define RT_ALT_F13			0x5BD
#define RT_ALT_F14			0x5BE
#define RT_ALT_F15			0x5BF
#define RT_ALT_F16			0x5C0
#define RT_ALT_F17			0x5C1
#define RT_ALT_F18			0x5C2
#define RT_ALT_F19			0x5C3
#define RT_ALT_F20			0x5C4
#define RT_ALT_F21			0x5C5
#define RT_ALT_F22			0x5C6
#define RT_ALT_F23			0x5C7
#define RT_ALT_F24			0x5C8
#define RT_ALT_F25			0x5C9
#define RT_ALT_F26			0x5CA
#define RT_ALT_F27			0x5CB
#define RT_ALT_F28			0x5CC
#define RT_ALT_F29			0x5CD
#define RT_ALT_F30			0x5CE
#define RT_ALT_F31			0x5CF
#define RT_ALT_F32			0x5D0
#define RT_ALT_F33			0x5D1

#define RT_CTRL_A			1
#define RT_CTRL_B			2
#define RT_CTRL_C			3
#define RT_CTRL_D			4
#define RT_CTRL_E			5
#define RT_CTRL_F			6
#define RT_CTRL_G			7
#define RT_CTRL_H			8
#define RT_CTRL_I			9
#define RT_CTRL_J			10
#define RT_CTRL_K			11
#define RT_CTRL_L			12
#define RT_CTRL_M			13
#define RT_CTRL_N			14
#define RT_CTRL_O			15
#define RT_CTRL_P			16
#define RT_CTRL_Q			17
#define RT_CTRL_R			18
#define RT_CTRL_S			19
#define RT_CTRL_T			20
#define RT_CTRL_U			21
#define RT_CTRL_V			22
#define RT_CTRL_W			23
#define RT_CTRL_X			24
#define RT_CTRL_Y			25
#define RT_CTRL_Z			26

// ============================================================================
// the RT_ALT_* keys are not available when the Win32 menuing system is active
// ============================================================================

#define RT_SHIFT_A			65
#define RT_SHIFT_B			66
#define RT_SHIFT_C			67
#define RT_SHIFT_D			68
#define RT_SHIFT_E			69
#define RT_SHIFT_F			70
#define RT_SHIFT_G			71
#define RT_SHIFT_H			72
#define RT_SHIFT_I			73
#define RT_SHIFT_J			74
#define RT_SHIFT_K			75
#define RT_SHIFT_L			76
#define RT_SHIFT_M			77
#define RT_SHIFT_N			78
#define RT_SHIFT_O			79
#define RT_SHIFT_P			80
#define RT_SHIFT_Q			81
#define RT_SHIFT_R			82
#define RT_SHIFT_S			83
#define RT_SHIFT_T			84
#define RT_SHIFT_U			85
#define RT_SHIFT_V			86
#define RT_SHIFT_W			87
#define RT_SHIFT_X			88
#define RT_SHIFT_Y			89
#define RT_SHIFT_Z			90


// =======================================
// these defines must be aligned with the
// entries in dictobjs.h !!!!
// =======================================
#define	APL_OBJ		0		/* configuration record */
#define	BIT_OBJ		1		/* bit map */
#define	DOC_OBJ		2		/* document */
#define	DSC_OBJ		3		/* descriptor */
#define	FLD_OBJ		4		/* field record */
#define	ICN_OBJ		5		/* icon */
#define	LST_OBJ		6		/* compiled list */
#define	MNB_OBJ		7		/* Menu Body */
#define	MNU_OBJ		8		/* header */
#define	PAT_OBJ		9		/* edit pattern */
#define	OFB_OBJ		10		/* Olivetti Financial Basic subroutine */
#define	PIC_OBJ		11		/* pick list header */
#define	PLB_OBJ		12		/* Pick List Body */
#define	PMB_OBJ		13		/* Pick List Menu body */
#define	PMU_OBJ		14		/* Pick List Menu header */
#define	SCR_OBJ		15		/* screen */
#define	TAG_OBJ		16		/* tag object reference */
#define	UDO_OBJ		17		/* user defined object */
#define	CSR_OBJ		18		/* custom C src routine, its a real DD object */
#define	PDM_OBJ		19		/* Win/32 Pull Down Menu */
#define	WMU_OBJ		20		/* mosaic OA Win/32 Menu */
#define	MLD_OBJ		21		/* Multi Line Descriptor */


#endif
/* Linea Agregada por Martin para el reentry de 7880 */
#define CANCEL_REENVIE  99  /* Transaccion que no se termino de transmitir */
#define clave_sf1			 0xa5

/*******/

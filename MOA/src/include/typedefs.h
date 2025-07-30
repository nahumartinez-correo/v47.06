/***********************************************************/
/*                                                         */
/*   Typedefs used globally in both RunTime, EDEN, DAU,    */
/*   IMPORTxxx and EXPORTxxx utilities, plus CDS, COMM,    */
/*   etc.                                                  */
/*                                                         */
/* This is the Release 3.0 version, with "TYPED" FIELDS.   */
/***********************************************************/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#ifndef THIS_SYS_H
#	include <this_sys.h>
#endif

#ifndef _WINDOWS_
#include	"windows.h"
#endif

#define	XDLL	//_declspec (dllexport)	!!!!!!!omitted!!!!!! see RT.DEF

typedef unsigned char	byte;
typedef unsigned short  index;
#define bool BOOL
//typedef unsigned int    bool;
typedef unsigned int    uint;
typedef unsigned short	ushort;

typedef struct Icon {
    unsigned long data[32];	/* Bit array (32 * 32 bits) */
	} Icon;
typedef struct IconHeader {
	unsigned short	iconsize;
	unsigned short	icontype;	/* ISC, Windows	*/
	} IconHeader;
typedef struct rtIcon {
	IconHeader	iconhdr;
	Icon		icondat;
} rtIcon;

#pragma pack (push, 2)		// All structures use 2-byte alignment

typedef struct DDR {		// DD object Reference (formerly APPITM)
	byte	object;			// Type (e.g. PreFLD, PreDOC, etc.)
	byte	applno;			// Application number (1-n)
	index   itemno;			// Item number (1-n)
	} ddr;

typedef struct PCLDEF {
	ddr		ddref;			// R1: Keep DDR info easily accessible
	index	PPlength;		// length of PCL routine in bytes
	byte	*PPip;			// instruction pointer
	short	argcount;		// argument count for parameter passing
	} pcldef;
	/* compiled PCL procedure immediately follows the PCLDEF structure */

typedef struct SCRDEF {
	ddr		ddref;			// R1: Keep DDR info easily accessible
	index	PPlength;		/* length of SCR code in bytes */
	byte	*PPip;			/* instruction pointer */
	short	x;				/* upper left corner of menu box */
	short	y;				/* in pixels (column,row) */
	short	width;			/* width in pixels */
	short	length;			/* length in pixels */
	short	fldsort;		/* 0 = horizontal, 1 = vert, 2 = none */
	byte	forecolor;		/* foreground color */
	byte	backcolor;		/* background color */
	short	boxtype;		/* box type for this list */
	short	grid_horiz;		/* last grid used */
	short	grid_vert;
	} scrdef;
	/* compiled SCREEN immediately follows the structure itself */

typedef struct DOCDEF {
	ddr		ddref;			// R1: Keep DDR info easily accessible
	index	PPlength;		/* length of DOC code in bytes */
	byte	*PPip;			/* instruction pointer */
	short	scroll_horiz;	/* last scroll position */
	short	scroll_vert;
	short	grid_horiz;		/* last grid used */
	short	grid_vert;
	short	zoom;			/* last zoom level used */
	int		doc_flags;		/* misc. flags */
	} docdef;
	/* compiled DOCUMENT immediately follows the structure itself */

typedef struct NOTEDEF {
	short	note_len;		// Always at least 1 byte (NULL byte)
	short	path_len;
	short	note3_len;
	} notedef;
	// 3-part NOTEs text immediately follows the NOTEDEF structure

typedef struct RTOBJ {
	void	*ptr;
	ddr		obj;
	} rtobj;

	// ----------------------------------------------------------------
	//  # of args (-1 means variable), address of C routine within the
	//  DLL, ASCII name of entry point (which may be different than
	//  the name of the CSR object since DD names are more restricted
	// ----------------------------------------------------------------
typedef struct CSRDEF {		// Custom C source object definition
	short	flags;
	short	argc;
	int		(*procaddr)(int argc,rtobj *argv[]);
	char	procname[36];
	} csrdef;

#pragma pack (pop)
#include	"bitobj.h"
#pragma pack (push, 2)
	/* bitmap starts immediately after the bitobj structure	*/

typedef struct PICBODY {	/* PICKLIST body */
	index	numitems;		/* number of items in list */
	index	size;			/* size of list in bytes */
	} picbody;
	/* body (list) starts immediately after the header structure */

typedef struct MNUBODY {	/* (teller-style) MENU body */
	index	numitems;		/* number of items in list */
	index	size;			/* size of list in bytes */
	} mnubody;
	/* body (list) starts immediately after the header structure */

typedef struct PMNBODY {	/* PICKLIST-style MENU body */
	index	numitems;		/* number of items in list */
	index	size;			/* size of list in bytes */
	} pmnbody;
	/* body (list) starts immediately after the header structure */

typedef struct COMPLIST {	/* compiled list header */
	index	numitems;		/* number of items in list */
	index	size;			/* size of list in bytes */
	rtobj	editproc;		/* points to PCL edit procedure or NULL */
	} complist;
	/* body (list) starts immediately after the header structure */

typedef struct PICHEADER {
	index	x;				/* upper left corner of box */
	index	y;				/* in pixels (column,row) */
	index	width;			/* width in pixels */
	index	length;			/* length in pixels */
	byte	font;			/* font & color info */
	byte	face;
	byte	size;
	byte	forecolor;
	byte	backcolor;
	index	boxtype;		/* box type for this list */
	index	itemdepth;		/* number of rows to move per select */
	short	flags;			/* bit meanings are in picklist.h */
	rtobj	list;			/* ptr to list, after RAM loading */
	rtobj	title;			/* title of picklist selections */
	rtobj	key_filter;		/* proc to be given a shot at the keystroke */
	} picheader;
	/* list not assumed to start here... */

typedef struct MNUHEADER {
	index	x;				/* upper left corner of box */
	index	y;				/* in pixels (column,row) */
	index	width;			/* width in pixels */
	index	length;			/* length in pixels */
	byte	font;			/* font & color info */
	byte	face;
	byte	size;
	byte	forecolor;
	byte	backcolor;
	index	boxtype;		/* box type for this menu */
	rtobj	list;			/* ptr to list, after RAM loading */
	} mnuheader;
	/* list not assumed to start here... */

typedef struct PMNHEADER {
	index	x;				/* upper left corner of box */
	index	y;				/* in pixels (column,row) */
	index	width;			/* width in pixels */
	index	length;			/* length in pixels */
	byte	font;			/* font & color info */
	byte	face;
	byte	size;
	byte	forecolor;
	byte	backcolor;
	index	boxtype;		/* box type for this list */
	index	itemdepth;		/* number of rows to move per select */
	short	flags;			/* bit meanings are in picklist.h */
	rtobj	list;			/* ptr to list, after RAM loading */
	rtobj	title;			/* title of picklist selections */
	rtobj	key_filter;		/* proc to be given a shot at the keystroke */
	} pmnheader;
	/* list not assumed to start here... */

/*eject*/
typedef struct FLDFLAGS {

	/* these flags are assigned at field definition, and	*/
	/* should not be changed at run time					*/
    unsigned int type: 4;	/* Type - see #defines FT_*** below		*/
	unsigned autoskip     : 1;	/* auto ENTER_FIELD when full           */
	unsigned auto_clear   : 1;	/* clear field on ENDTRAN, RESET TRAN	*/
	unsigned auto_dup     : 1;	/* autodup on ENDTRAN, RESET TRAN       */
	unsigned auto_extract : 1;	/* EXTRACT ALL includes this field if 1 */
	unsigned auto_append  : 1;	/* APPEND ALL includes this field if 1  */
	unsigned sign_allowed : 1;	/* TRUE if field can be negative        */
	unsigned calc_mode    : 1;	/* right-to-left entry, no cursor movement */
	unsigned force_UC     : 1;	/* force alpha to Upper Case            */
	unsigned required     : 1;	/* data must be entered                 */
	unsigned allowdup     : 1;	/* Field May Be Dup'ed From Previous    */
	unsigned allowmodify  : 1;	/* no auto_clear on 1st char in datacoll*/
	unsigned protect      : 1;	/* no data entry allowed                */
	unsigned blankedit    : 1;	/* display blanks if field is empty     */
	unsigned nopldisplay  : 1;	/* don't display picklist if no verify  */

	/* this flag is set at import time, and updated at runtime */
	unsigned highlite     : 1;	/* video-reverse field when displayed   */

	/* these flags are not set at import, but may be updated at run-time */
	unsigned processed    : 1;	/* TRUE if already processed by PCL     */
	unsigned dataentered  : 1;	/* TRUE if data has been entered        */
	unsigned dup_dataentered : 1;/* copy of dup area's DATAENTERED flag  */
	unsigned dyn_prot     : 1;	/* dynamic: protect field flag          */
	unsigned hidden       : 1;   /* TRUE if field hidden by HIDEFIELD    */
	unsigned valdone      : 1;   /* TRUE if validation shouldn't be rerun*/
	unsigned fieldlisted  : 1;   /* TRUE if added to fields list         */
	unsigned array        : 1;   /* TRUE if this field is an array       */

	/* this flag is set at import time, and updated at runtime */
	unsigned autoIME      : 1;  /* envoke the IME if RT_keyboard_mode==IME */

	/* these flags are currently unused						*/
	/* use them as required... This is a 32-bit structure	*/
	unsigned filler01     : 1;
	unsigned filler02     : 1;	/* add more as needed... */
	} fldflags;

typedef struct  FIELD {
	/* these items are assigned at field definition, */
	/* and should not be changed at run time	     */
	ddr			ddref;			// R1: Keep DDR info easily accessible
	char		*name;			// R1: NULL or points to ASCII name
	char		ldelim; 		/* Left display bracket			   */
	char		rdelim; 		/*   ..same for right			   */
    short		length;			/* Unedited Length Of Data		   */
	byte		places;			/* digits to the right of the '.'  */
								/* ------------------------------- */
	byte		edit;			/* These byte fields will contain  */
	byte		atentry;		/* NOOBJ if there is no rtobj	   */
	byte		error;			/* attached to the field.  If they */
	byte		help;			/* contain any other value, it is  */
	byte		pre;			/* the index into the rtboj array  */
	byte		post_val;		/* that immediately follows the	   */
	byte		during_entry;	/* field definition in memory	   */
								/* dynamically attached val rtn	   */
								/* ------------------------------- */
	/* these items are updated at run-time	*/
	fldflags	flags;			/* Field Flags					   */
	short		displaylen;		/* field display length in bytes   */
    byte	  	*storage;		/* Pointer To Data Area			   */
    short		numchars;		/* Number of chars now in field    */
	short		dupnumchars;	/* # chars currently in dup area   */
	int			userdata;
/* the following zero length array saves space */
/*	rtobj		fldobj[0];					   */
	} field;
	/* An array of rtobj's immediately follows the structure,		*/
	/* there may be up to 7 fldobj's attached to the field).        */
	/* Field data storage immediately follows the array of rtobj's	*/
	/* Field dup storage immediately follows field data storage		*/

#define	NOOBJ			(byte)0xEE	/* unique code to make field structures	*/
									/* easier to locate in memory			*/
#define	MAXFLDOBJS		7			/* maximum # of rtobjs attached to field*/

/* --------------------------------- */
/*  Currently supported FIELD types  */
/* --------------------------------- */
#define	FT_ASCII		0	/* ASCII data, null terminated */
#define	FT_NUMERIC		1	/* ASCII digits only, null terminated */
#define	FT_DATE			2	/* ASCII digits only, null terminated */
#define	FT_TIME			3	/* ASCII digits only, null terminated */
#define	FT_INT			4	/* signed 32-bit integer */
#define	FT_FIXED		5	/* signed variable length fixed point number */
#define	FT_FLAGS		6	/* 32 1-bit flags */
#define	FT_IND			7	/* 4-byte indirect DD obj ref: OB AN HHLL */
#define	FT_BULK			8	/* unsigned 8-bit bytes (bulk, uninterpreted data */
#define	FT_GBCHINESE	9	/* 2-byte character data, 1-byte control codes.
				 * (20H,20H) - (20H,7FH) ASCI codes.
				 * (21H,21H) - (77H,7EH) GB Chinese codes. */

#define	FT_UNICODE		10	/* Unicode (16-bit) characters */
#define FT_TIMESTAMP 11 // ascii char yyyymmddhhmmssttttt
#define FT_TIMESTAMP_LENGTH 19		//yyyyMMddHHmmSSttttt
#define	FT_RINT			14	/* byte-reversed signed 32-bit integer */
#define FT_NULL			15	/* universal NULL VALUE */
/* used by CDServer only, not by RT, or other clients (unknown to PCL) */


/**********************************************************************/
/*																	  */
/* The following structure elements are used by the datacollect code, */
/* and are ignored by the data-dictionary.                            */
/*																	  */
/*       field.displaylen  field.numchars  field.dupnumchars          */
/*																	  */
/**********************************************************************/

/* The following typedef is used by the print facility provided by rt */
#pragma pack (pop)
#ifndef LIBPERF_H
#include "libperf.h"
#endif
#pragma pack (push, 2)

typedef struct PRTDEF {
		char winbytes[PERFLIB_EXTRA];
		int  bufflen;
		char buff[1];
		} prtdef;

#pragma pack (pop)
/**********************************************************************/
/*                                                                    */
/*     Following structure replaces "RTduts" and dutsname struct.     */
/*                                                                    */
/*                                                                    */
/**********************************************************************/
typedef struct {
	char			nodename[16];		/* System name */
	unsigned long	nodename_len;		/* length of node name string */
	ushort			clocktics;			/* Number of clock ticks/second */
} ppsysinfo;

// ======================
// Multi line descriptor
// ======================
typedef struct MLDESC {
	index	numitems;		// number of items in list
	index	size;			// size of list in bytes
	} mldesc;
	// list not assumed to start here...

typedef struct WMUDEF {
	ddr		ddref;			// R1: Keep DDR info easily accessible
	index	PPlength;		/* length of PCL routine in bytes */
	byte	*PPip;			/* instruction pointer */
	} wmudef;
	/* compiled WMU definition immediately follows the WMUDEF structure */

// ======================
// Win32 menu definition
// ======================
typedef struct PDMENU {		// Win32 PullDown Menu definition
	index	numitems;		// number of items in list
	index	size;			// size of list in bytes
	} pdmenu; 

//=========================================================================
// body (pdmenu) starts immediately after the header structure
//=========================================================================
// pull_down_menu_horiz_prompt, 1 per list
// flag_1  item_1_accelerator_key  item_1_action_object  item_1_prompt  
// flag_2  item_2_accelerator_key  item_2_action_object  item_2_prompt  
// flag_3  item_3_accelerator_key  item_3_action_object  item_3_prompt  
//
//   "                "                      "                "
//
// flag_n  item_n_accelerator_key  item_n_action_object  item_n_prompt  
// terminating_null_byte
//
// where:
//
// The flag_n parameter correspond to fuflag definition for the
// InsertMenu() Win32 API function parameters as documented in Win32 SDK.
//
// The item_n_action_objects may be one of: OFB, CSR, LST or PDM
//
// The item_n_prompt may be a string constant, descriptor, field
// or a BIT object.
//
//=========================================================================

// -------------------------------------------
// pull down menu linked list node definition
// -------------------------------------------
typedef struct PDMNODE {
	struct PDMNODE	*next;	
	struct PDMNODE	*prev;
	short		flags;			// INUSE, PROTECTED, etc.
	HMENU		hmenu;			// menubar handle
	HMENU		hpdmenu;		// menu handle
	byte		ano;			// menu appl number
	index		ino;			// menu item number
	UINT		position;		// zero based menu item
	pdmenu		*pdmdef;		// for later item modifications
	} pdmnode;


// ------------------------------------------------
// pull down menu item linked list node definition
// ------------------------------------------------
typedef struct PDMITEM {
	struct PDMITEM	*next;	
	struct PDMITEM	*prev;
	short		flags;		// INUSE, PROTECTED, etc.
	HMENU		hmenu;		// menubar handle
	HMENU		hpdmenu;	// pull down menu handle
	byte		ano;		// menu appl number
	index		ino;		// menu item number
	WORD		id;			// menu item identifier
	WORD		key;		// ONA's menu accelerator key 
	WORD		chr;		// underscore char for menu item
	byte		*action;	// the DDR for menu item
	} pdmitem;


#endif

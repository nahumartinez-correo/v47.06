// =====================================================================
//
//  This file is the analog of <rtglobal.h> which can't be used in
//  the CSR.DLL (or other external clients of RT.DLL) due to data 
//  definition differences when crossing DLL boundaries.
//
//  The file ..\RT\RT.DEF is also critical to the data access working
//  properly.  See the comments therein for further information.
//
//  This file <RTDEF.H> must be included in all custom C routines which
//  access any of this data!
//
// =====================================================================

#ifndef RTDEF_H
#define RTDEF_H

#ifdef DATAMAN_H
    #error please include rtdef.h before dataman.h
#endif

#ifdef CSRDEF_H
    #error please include rtdef.h before csrdef.h
#endif

#include <setjmp.h>

#ifndef PCLDEFINES_H
#	include <ppdefs.h>		// needed for NUMFUNKEYS
#endif
#ifndef DSPLIST_H
#	include <dsplist.h>
#endif
#ifndef RT_H
#	include <rt.h>
#endif
#ifndef _WINDOWS_
#	include <windows.h>
#endif
#ifndef RTFONTS_H
#	include <rtfonts.h>
#endif

#define XPDATA extern __declspec(dllimport)

XPDATA	field			*Currentfield;		// current field id
XPDATA	dlist			*Currentdlp;		// Display list ptr 4 current FLD
XPDATA	struct SLIST	*Current_screen;	// multi-screen tran header
XPDATA	short			cursor_on;			// remembers if cursor active
XPDATA	char			*date_std_edit;		// default date editpattern
XPDATA	int				RTerrno;
XPDATA	int				DDerrno;			// In DATAMAN.C
XPDATA	short			Debug;				// debugging output to console?
XPDATA	short			ESC_lock;			// TRUE if <ESC> gets passthru
XPDATA	HPALETTE		hDefaultPal;		// Default color palette
XPDATA	HWND			hWnd;
XPDATA	field			*List_tran_ano;		// list tran's appl no.
XPDATA	field			*List_tran_ino;		// list tran's item no.
XPDATA	int				Max_fields;			// max fields allowed per tran
XPDATA	short			Maxfontnum;	
XPDATA	RECT		 	PCLcursor;			// used for application cursor
XPDATA	struct DLIST	*PCLfields;
XPDATA	struct DLIST	*PCLscreen;
XPDATA	int	 			pickexitkeys[5];	// keys which'll exit picklists
XPDATA	int				Retval;				// return val from last PCL
XPDATA	int				RTddh;				// Data Dictionary Handle
XPDATA	int				*RTflags;			// flags used by runtime exec
XPDATA	struct COMPLIST	*RTprtmsg_nowait;	// ==> LST:common:prtmsg_nowait
XPDATA	short 			theBackColor;		// Last color set with BackColor
XPDATA	short 			theFace;			// Last style set with TextFace
XPDATA	short 			theFont;			// Last font set with TextFont
XPDATA	short 			theForeColor;		// Last color set with ForeColor
XPDATA	short 			theSize;			// Last font size set w/TextSize
XPDATA	short 			Cellwidth;			// Last average character width
XPDATA	field			*Tran_ano;			// current tran's appl no.
XPDATA	struct TRANREC	*Tran_current;		// Pointer to current Tran
XPDATA	field			*Tran_ino;			// current tran's item no.
XPDATA	char			Whichsystem[40];
XPDATA	char			Winmenu_flag;
XPDATA	char			*optarg;
XPDATA	int				optind;
XPDATA	int				opterr;
XPDATA	int				optopt;
XPDATA	byte			RT_boxfieldborder;
XPDATA	rtcontrol		*SLcontrolfree;
XPDATA	int				OFBflags;
XPDATA	short			RTstatus_y;
XPDATA	RECT			RTrect;
XPDATA	short			theTextExtra;
XPDATA	short			theSpaceExtra;
XPDATA	WMFontInfo		fonttbl[];
XPDATA	WMFonts			iscbr_fonts[];
XPDATA	byte			Box_color;
XPDATA	short			RTsuspend_reset;	// RESET key processing suspended
XPDATA	short			rtWidth;
XPDATA	short			rtHeight;
XPDATA	RECT			PCLrect;
XPDATA	struct DLIST	*PCLstatus;
XPDATA	byte			Common;
XPDATA	void 			*RTsysinfo;			// our own system info structure
XPDATA	struct DLIST	*PCLerrors;
XPDATA	DWORD 			TernaryXltTable[];
XPDATA	HINSTANCE		hprevinstance;
XPDATA	HINSTANCE		globhinstance;
XPDATA	int				globncmdshow;
XPDATA	LPSTR			globlpcmdline;
XPDATA	short			RTnowindow;
XPDATA	HPALETTE		hpalCurrentBMP;
XPDATA	WORD			nNumOfVisibleBMP;
XPDATA	WORD			nMaxColorsInBMP;
XPDATA	int				DefNumPal;			// in rtwindow.c
XPDATA	int				RT_BorderWidth;		// in rtwindow.c
XPDATA	int				RT_BorderHeight;	// in rtwindow.c
XPDATA	int				RT_TitleBarHeight;	// in rtwindow.c
XPDATA	short			RT_left;			// in rtwindow.c
XPDATA	short			RT_top;				// in rtwindow.c
XPDATA	short			RT_bottom;			// in rtwindow.c
XPDATA	short			RT_right;			// in rtwindow.c
XPDATA	int				PCLlineno;
XPDATA	short			Ghostselection;		// for ghosted menu selections
XPDATA	int				PCLlineno;
XPDATA	char			PCLname[21+21+1];
XPDATA	char			PCLfilename[80];
XPDATA	char			*PCLcurrentline;
XPDATA  short			compile_dict;
XPDATA  short			Autodef;    		// TRUE if auto def of OBJ's wanted
XPDATA	short			RTmenuoption;		// Init 51 supports 'or' ing of menu flags
XPDATA	struct SLIST	*Screen_base;		// rtglobal.h
XPDATA	field			*Nextfield;			// rtglobal.h
#ifndef DBMREC_C
XPDATA	int				Lastkey;			// rtglobal.h
#endif
XPDATA	unsigned long 	LastTag;			// rt/mouse.c
XPDATA	int				running_under_IBA;	// non-zero when running w/IBAE
XPDATA	short 			LanguageIndex;		// Multi-Language String support, in convers.c
XPDATA	char 			*LanguageSep;		// Multi-Language String support, in convers.c
XPDATA	unsigned short	RT_PPlusCompatible;
XPDATA	unsigned short	RT_StretchField;
#endif

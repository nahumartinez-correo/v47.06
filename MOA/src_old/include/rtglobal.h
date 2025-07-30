// =====================================================================
//  The following variables are used globally in RT.DLL.  The variables 
//  which require initialization are allocated in ..\MISC\RTGLOBALS.C 
//  Some of this data is exported to CSR.DLL via the file ..\RT\RT.DEF
// 
//  Note: this file is sorted alphabetically...
// =====================================================================

#ifndef RTGLOBALS_H
#define RTGLOBALS_H

#include <setjmp.h>

// ---------------------------------------------------------------
// Include various header files if they aren't already included...
// ---------------------------------------------------------------
#ifndef PCLDEFINES_H
#include <ppdefs.h>		// needed for NUMFUNKEYS
#endif

#ifndef DSPLIST_H
#include <dsplist.h>
#endif

#ifndef RT_H
#include <rt.h>
#endif

#ifndef NLS_H
#include <nls.h>
#endif

#ifndef _WINDOWS_
#	include <windows.h>
#endif

#ifdef DOGLOBALS
#  define GLOBAL
#else
#  define GLOBAL extern
#endif


GLOBAL	int				Argument_errors;

GLOBAL	short			Bell_unassigned_Fkey;
GLOBAL	short			Block_bound;// TRUE if bound events R ! 2 B dispatched
GLOBAL	byte			Box_color;
GLOBAL	struct {
			byte	opcode;					// original opcode byte
			byte	applno;
			index	itemno;
			byte	*location;				// address of opcode
			char	name[44];				// name of PCL proc
			}			Breakpoint[5];
GLOBAL	short			Breakpoint_happened;// used by singlestep.c

GLOBAL	byte			Calculator_color;
GLOBAL	byte			Calendar_color;
GLOBAL	field			*CDSerrmsg;			// ==> common:Cds_error_message
GLOBAL	field			*CDSlockretry;		// ==> FLD:common:Cds_lock_retry
GLOBAL	field			*CDSservername;		// => FLD:common:Cds_server_name
GLOBAL	int				CDSretval;			// ...from last CDS stmt
GLOBAL	int				RTerrno;
GLOBAL	index			Cleanstart_ino;		// Itemno of PCL proc to run
GLOBAL	byte			Cleanstart_ano;		// Applno of PCL proc to run
GLOBAL	field			*CSRstring;			// ==> FLD:common:csr_string
GLOBAL	short			Comm_nofilter;		// controls ASCII filtering
extern	byte			Common;				// 'common' application number
GLOBAL	int				COMMretval;			// ...from last COMM stmt
GLOBAL	byte			Current_color;		// currently selected WP-V color
GLOBAL	struct SLIST	*Current_screen;	// multi-screen tran header
GLOBAL	dlist			*Currentdlp;		// Display list ptr for current fld
GLOBAL	field			*Currentfield;		// current field id
GLOBAL	pcldef			*Currenttran;		// current transaction id
GLOBAL	short			cursor_on;			// remembers if cursor active

GLOBAL	short			Databell;			// TRUE if BEEP wanted in D.C.
GLOBAL	byte			Datacollect_color;	// color-pair values 1-4, or 0
GLOBAL	char			*DC_bad_date_error;	// message for bogus date err
GLOBAL	char			*DC_bad_time_error;	// message for bogus time err
GLOBAL	char			*DC_required_error;	// message for required data
GLOBAL	char			*DC_validation_error;// message for validation err
GLOBAL	int				DDEretval;			// ...from last DDE stmt
GLOBAL	short			Debug;				// debugging output to console?
GLOBAL	short			DebugKeys;			// special debug keys active?
GLOBAL	short	 		defInsertMode;		// default, set by INIT 9 stmt
GLOBAL	short			DIBWndActive;
GLOBAL	short			Discard_resetkey;	// INIT 32 (for SeaFirst)
GLOBAL	short			Dolongjump;			// TRUE if longjump wanted
GLOBAL	struct FIELD	**Dup_list;			// list of dup doc fields

GLOBAL	short			EDENflag;			// flag for DISPLAY stmt
GLOBAL	short			EndTranMsg;			// TRUE/FALSE via init 49
GLOBAL	byte			Error_color;
GLOBAL	short			Errorbell;			// TRUE if error BEEPs wanted
GLOBAL	short			ESC_lock;			// TRUE if <ESC> gets passthru

GLOBAL	short			Fieldval_with_mouse;// INIT 56 (for Primerica)
GLOBAL	int	 			fkeytable[4*NUMFUNKEYS]; // function key mappings
GLOBAL	short			Focus_rect_offset;
GLOBAL	byte			FYI_color;
GLOBAL	short			FWDREFDmsg;

GLOBAL	short			Ghostselection;		// for ghosted menu selections

GLOBAL	HANDLE			hAccelTable;
GLOBAL	HPALETTE		hDefaultPal;		// Default color palette
GLOBAL	short			rtHeight;			// window size (y) in pixels
GLOBAL	byte			Help_color;
GLOBAL	short			hscroll_val;		// horizontal scroll value
GLOBAL	HANDLE			hStdin;
GLOBAL	HWND			hWnd;

GLOBAL	struct INDREC	*INDbase;			// indirect object stack
GLOBAL	struct INDREC	*INDfreelist;		// indirect object stack

GLOBAL	struct KEYREC	*KEYbase;			// free list pointer 4 keydefs.
GLOBAL	struct KEYREC	*KEYrec;			// list pointer for keydefs.
GLOBAL	struct KEYREC	*KEYset;			// record pointer for keydefs.

GLOBAL	long 			Last_midnight;		// for io.c
GLOBAL	int				Lastkey;			// Last key read from keyboard
GLOBAL	mnuheader		*LastMenu;			// current (or last) menu id
GLOBAL	byte			Lasttran_appl_no;	// last tran's application no.
GLOBAL	index			Lasttran_item_no;	// last tran's index no.
GLOBAL	field			*List_tran_ano;		// list tran's appl no.
GLOBAL	field			*List_tran_ino;			// list tran's item no.
GLOBAL	index			LocalPrintIndex;	// index of common:local_print
GLOBAL	struct LTPPLIST	*LTPPfree;			// for exec_print.c and INIT 29
GLOBAL	struct LTPPLIST	*LTPPhead;			// for exec_print.c and INIT 29

GLOBAL	int				Max_fields;			// max fields allowed per tran
GLOBAL	short			Maxfontnum;			// highest logical font #
GLOBAL	byte			Menu_color;			// ...but 1-based here so we

GLOBAL	struct NAVREC	*Nav_base;			// list ptr for scrn nav.
GLOBAL	struct NAVREC	*Nav_current;		// list ptr for scrn nav.
GLOBAL	struct NAVREC	*Nav_freebase;		// freelist ptr for scrn nav.
GLOBAL	field			*Nextfield;			// if nonzero,field to goto next
GLOBAL	short			NoCDServer;			// flag for disabling CDS
GLOBAL	short			NoCommServer;		// flag for disabling HostComm


GLOBAL	POINT			PBpoint;			// for mouse.c
GLOBAL	int				PCL_entry_count;	// "id" of current pcl routine
GLOBAL	RECT		 	PCLcursor;			// used for application cursor
GLOBAL	RECT		 	PCLdirty;			// dirty rectangle during updates
GLOBAL	struct DLIST	*PCLerrmsg;			// for ERROR messages only
GLOBAL	struct DLIST	*PCLerrors;
GLOBAL	struct DLIST	*PCLfields;
GLOBAL	struct DLIST	*PCLfyimsg;			// for FYI messages only
GLOBAL	short			PCLkill_err;		// TRUE if need to remove ERRMSG
GLOBAL	short			PCLkill_fyi;		// TRUE if need to remove FYIMSG
GLOBAL	char			PCLname[21+21+1];
GLOBAL	RECT			PCLrect;
GLOBAL	struct DLIST	*PCLscreen;
GLOBAL	RECT			PCLstatrect;		// Rectangle defining status area
GLOBAL	struct DLIST	*PCLstatus;
GLOBAL	short	 		PCLtrace;			// from -T flag, PCL trace wanted
GLOBAL	struct DLIST	*PLmouseclick;		// flag for menus.c & mouse.c

GLOBAL	int	 			pickexitkeys[5];	// keys which'll exit picklists
GLOBAL	byte			Picklist_color;		// ...can tell if no color
GLOBAL	byte			Popup_color;		// ...actual values are 0-3,
GLOBAL	char			*Printdevice;		// current printer path
GLOBAL	prtdef			*Prt;				// rt print structure
GLOBAL	int				Prtbuflen;			// length of rt print buffer
GLOBAL	int				PRTretval;			// ...from last PRT stmt

GLOBAL	int				Retval;				// return val from last PCL
GLOBAL	short 			RT_active_idle_timer;
GLOBAL	unsigned short	RT_basescreencolor;	// base screen color for trans
GLOBAL	byte			RT_boxfieldborder;	// box type for flds with type 1
GLOBAL	char			*RT_cpu_node;		// CPU id
GLOBAL	pcldef			*RT_current_proc;	// pointer to procedure
GLOBAL	byte			RT_cursor_style;	// compressed or block cursor
GLOBAL	byte			RT_fieldrules;	 	// rev video & box delimiter rules
GLOBAL	byte			RT_iconpbborder;	// icon button border flag
GLOBAL	short			RT_ignoremouse;		// INIT 57 0 or 2
GLOBAL	int				RT_nopverbmsg;		// ctls display of msg 4 nop verbs
GLOBAL	byte			RT_pcltype;			// current object type
GLOBAL	short			RT_pickdc;			// picklist active in datacoll
GLOBAL	byte			RT_pickspacing; 	// picklist inter-line spacing
GLOBAL	int				RT_printdelayseconds; // sets delay for all print jobs
GLOBAL	byte			RT_reliefcheckbox;	// relief style check box	
GLOBAL	byte			RT_reliefradiobtn;	// relief style radio button	
GLOBAL	byte			RT_removing_popup;	// set when bringing down a popup
GLOBAL	byte			RT_sculpturedbox;	// 1 wide white box drawn sculptured
GLOBAL	unsigned short	RT_status_color;	// status line(s) color
GLOBAL	struct RT_TEFIELD *RT_TEfield_freelist;	// for TE controls
GLOBAL	struct RT_TEFIELD *RT_TEfield_list;	// for TE controls
GLOBAL	short 			RT_timer_event;
GLOBAL	struct STIMER_REC *RT_timer_freebase; // for do_settimer()
GLOBAL	struct STIMER_REC *RT_timer_head;	// for do_settimer()
GLOBAL	byte			RT_whiteonwhite;	// no setbcolor to white on white
GLOBAL	int				RT_wincomcompatible;// sets wincom compatiblity mode
GLOBAL	int				RT_EditFields;		// All fields are textedit fields
GLOBAL	int				RT_SpaceBold;		// pixel of space between bold chars
GLOBAL	int				RT_ControlDelay;	// Time before auto-popping control
GLOBAL	int				RT_AltKeyBehavior;	// Compatiblity prior to rt.dll fix 9/23/97. DRB Case #10882
GLOBAL	char			*RT_ControlSound1;	// Control down sound
GLOBAL	char			*RT_ControlSound2;	// Control up sound
GLOBAL	char			*RT_DateFold;   	// Determine default century for 6 digit dates
GLOBAL	char			*RT_window_node;	// Window CPU id
GLOBAL	byte			RT_winpushbuttons; 	// rev video & box delimiter rules
GLOBAL	byte			RTapplication;		// currently selected appl #
GLOBAL	struct PCLDEF	*RTbad_op_proc;		// ==> PCL:common:bad_op_proc
GLOBAL	int				RTddh;				// Data Dictionary Handle
GLOBAL	jmp_buf			RTenv;				// used by setjmp(), longjmp()
GLOBAL	byte 			RTerr_cleanup;		// avoid err recursion
GLOBAL	char			*RTerrfyi2;			// ERRMSG/FYIMSG pushbutton text
GLOBAL	char			*RTerrfyi3;			// ERRMSG/FYIMSG pushbutton text
GLOBAL	struct COMPLIST	*RTerrmsg_list;		// ==> LST:common:errmsg_list
GLOBAL	struct COMPLIST	*RTerrmsg_nowait;	// ==> LST:common:errmsg_nowait
GLOBAL	int				*RTflags;			// flags used by runtime exec
GLOBAL	byte 			RTfyi_cleanup;		// avoid fyi recursion
GLOBAL	field			*RTfyi_field;		// field to store atentry DSC in
GLOBAL	struct COMPLIST	*RTfyimsg_list;		// ==> LST:common:fyimsg_list
GLOBAL	short			RTinfront;			// TRUE if RT's window in front
GLOBAL	short	 		rtInsertMode;		// datacollect (insert/overtype)
GLOBAL	short			RTlogin;			// TRUE if rt to do user login
GLOBAL	short			RTmenuoption;		// INIT 51, for menu access
GLOBAL	struct PCLDEF	*RTmouse_proc;		// Setup by INIT 54
GLOBAL	unsigned short	RTmouse_proc_active;// mouse pcl proc call info
GLOBAL	void			(*RTmousehook)();	// for mouse.c
GLOBAL	struct COMPLIST	*RTprtmsg_list;		// ==> LST:common:prtmsg_list
GLOBAL	struct COMPLIST	*RTprtmsg_nowait;	// ==> LST:common:prtmsg_nowait
GLOBAL	RECT			RTrect;				// main window coordinates
GLOBAL	int				RTreset;			// TRUE if reset key pending
GLOBAL	short			RTstatus_y;			// row for top of status line
GLOBAL	short			RTsuspend_reset;	// RESET key processing suspended
GLOBAL	void 			*RTsysinfo;			// our own system info structure
GLOBAL	short			RTtitleopts;		// window bar title options
GLOBAL	short			RTvrespxls;			// INIT 50, for local print
GLOBAL	int				running_under_IBA;	// non-zero when running w/IBAE

GLOBAL	struct SLIST	*Screen_base;		// pointer for sl_xxx() stuff
GLOBAL	struct SLIST	*Screen_zero;		// pointer for initial screen
GLOBAL	short	 		SingleStep;			// from Ctrl-Z, source debugging
GLOBAL	short	 		SingleStep_around;	// skip calls from debugger
GLOBAL	short	 		SingleStep_outof;	// skip out of current SUB
GLOBAL	short			SRinterval;			// secs to auto update of status
GLOBAL	pcldef			*Status_Line_pcl;	// routine to put up status
GLOBAL	short			SystemDriver;		// transaction nesting level

GLOBAL	short			theBackColor;		// Last color set w/BackColor
GLOBAL	short			theFace;			// Last style set w/TextFace
GLOBAL	short			theFont;			// Last font set w/TextFont
GLOBAL	short			theForeColor;		// Last color set w/ForeColor
GLOBAL	long 			theMouseMode;		// Last mouse mode set w/MouseMode
GLOBAL	short			thePenHeight;		// Last pen height set w/PenSize
GLOBAL	short			thePenMode;			// Last pen mode set w/PenMode
GLOBAL	short			thePenPat;			// Last pen pattern set w/PenPat
GLOBAL	short			thePenWidth;		// Last pen width set w/PenSize
GLOBAL	short 			theSize;			// Last font size set w/TextSize
GLOBAL	short			theSpaceExtra;		// Last space extra set w/SpaceExtra
GLOBAL	short			theTextExtra;		// Last text extra set w/TextExtra
GLOBAL	short			theTextMode;		// Last text mode set w/TextMode
GLOBAL	short			theTextRotate;		// Last text rot angle w/TextRotate
GLOBAL	long 			Time_since_1970;	// for io.c, pclsubs.c
GLOBAL	int				Timeoflastkey;		// TOD that last key was hit
GLOBAL	short			Timing_wanted;		// TRUE if timing of I/O wanted
GLOBAL	short			Tmenukey;			// flag for menus.c & mouse.c
GLOBAL	short			TNLevel;			// transaction nesting level
GLOBAL	char			Tonode[20];
GLOBAL	field			*Tran_ano;			// current tran's appl no.
GLOBAL	struct TRANREC	*Tran_base;			// base pointer for Trans list
GLOBAL	struct TRANREC	*Tran_current;		// Pointer to current Tran
GLOBAL	struct TRANREC	*Tran_freebase;		// freelist pointer for Trans
GLOBAL	field			*Tran_ino;			// current tran's item no.
GLOBAL	struct TRANREC	*Tran_zero;			// list ptr for tran mgmt
GLOBAL	struct DDR  	*Trap_reset;		// routine to intercept RESET key
GLOBAL	field			*Tutorial_field1;	// holds tutorial line one
GLOBAL	field			*Tutorial_field2;	// holds tutorial line two

GLOBAL	byte			Unassigned_Fkey_appl;
GLOBAL	index			Unassigned_Fkey_item;
GLOBAL	struct PCLDEF  *User_error_proc;    // Setup by INIT 58
GLOBAL	struct PCLDEF  *User_fyi_proc;      // Setup by INIT 59
GLOBAL	struct PCLDEF  *User_nowait_gspread_proc;    // Setup by INIT 64
GLOBAL	short			UserAccess;			// menu authority flags

GLOBAL	short			vscroll_val;		// vertical scroll value

GLOBAL	short			Warningbell;		// TRUE if warning BEEPs wanted
GLOBAL	short			WC_ignoremouse;		// INIT 57 0 or 1
GLOBAL	short			WC_interval;		// watch cursor interval
GLOBAL	short			WC_purge;			// watch cursor purge key flag
GLOBAL	short			WC_watchon;			// watch cursor state
GLOBAL	char			Whichsystem[40];
GLOBAL	char			Whichuser[20];
GLOBAL	short			rtWidth;			// window size (x) in pixels
GLOBAL	char			Winmenu_flag;
GLOBAL	unsigned short	WP5_focus_color;	// bogus color means ignore
GLOBAL	unsigned short	WP6_focus_background;
GLOBAL	unsigned short	WP6_focus_foreground;

GLOBAL	byte			*PUM_ddr;			// Popup Menu definition
GLOBAL	byte			PUM_ano;			// Popup Menu appl number
GLOBAL	index			PUM_ino;			// Popup Menu appl number
GLOBAL	int				RTpic_mouse_goaway;	// del pic on exterior mouse click
GLOBAL	int				PL_itemdepth_data;	// get multiline picklist data

GLOBAL	char			STDProgname[];
GLOBAL	char			STDDescription[];
GLOBAL	char			STDAuthor[];
GLOBAL	char			STDCopyright[];
GLOBAL	char			STDPartno[];
GLOBAL	char			STDRelease[];
GLOBAL	char			STDCheckpt[];
GLOBAL	char			STDInterim[];
GLOBAL	char			STDLnkdate[];

GLOBAL	short			RT_check_timers;
GLOBAL	unsigned short	RT_PPlusCompatible;
GLOBAL	unsigned short	RT_StretchField;
GLOBAL	unsigned short	RT_keyboard_mode; // default to latin keyboard (no IME or Arabic)
GLOBAL	unsigned short	RT_screen_mode; // default to latin (left origin)
GLOBAL	short			Cellwidth;			// Last average character width

GLOBAL	int				RTfix_prt_edit_pat;	// init 79 fix for PRT PRINT EDIT field problem


int	BackEndOnly;		// 1 if RT is back end only for the appl.

#ifdef CONVERSIONDEFS	/* defined *ONLY* in conversion.c! */
	XDLL short 	TruncateFlag = 0;
	char		DecimalInsertion = '.';		/* for USA type systems */
	char		MoneySign = '$';
	char		MoneyString[4] = {"$"};
	char		CalcEditPattern[40] = {"-ZZZ,ZZZ,ZZZ,ZZZ,ZZ9.99"};
	short		Century;
	short		Year;
	short		XMoveCursor = 0;		/* move cursor in password	*/
	char 		*LanguageSep = "$$";	/* substring separator (INIT 67) */
	short		LanguageIndex = 1;		/* substring index (INIT 66) */
#else
	extern XDLL short	TruncateFlag;		/* how to truncate or round */
	extern char 	DecimalInsertion;	/* probably '.' or ',' */
	extern char 	MoneySign;			/* probably just '$' */
	extern char 	MoneyString[4];		/* displays as "$" */
	extern char 	CalcEditPattern[40];/* how calculator displays */
	extern short	Century;			/* used by edit patterns */
	extern short	Year;				/* used by edit patterns */
	extern short	XMoveCursor;		/* move cursor in password	*/
	extern char *LanguageSep;    /* substring separator (INIT 67) */
	extern short LanguageIndex;  /* substring index (INIT 66) */
#endif

#endif

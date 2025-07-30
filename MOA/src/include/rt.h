/* ---------------------------------------------------------- */
/*  Global #define's used in the Pinnacle Plus TELLER system  */
/* ---------------------------------------------------------- */
#ifndef RT_H
#define RT_H

#define ULOG(x) puts(x)

#ifndef NULL
#  define NULL	0
#endif

#ifndef _WINDOOWS_
#include <windows.h>
#endif

#include <dsplist.h>

#define VALIDATION	'V'

#define MENUTRAN			0x0100
#define RT_BASESCREENCOLOR	0x0100
#define	ERR_MSG				0
#define	FYI_MSG				1

#define	CSR_WINMENU		(0x01)
#define	VERB_WINMENU	(0x02)

/* ------------------------------------------------------------ */
/*  Structures used with the generalized list manager (gl_xxx)  */
/*  of which RT makes considerable use...                       */
/* ------------------------------------------------------------ */

	struct NAVREC {				/* navigation between screens */
		struct NAVREC	*next;
		struct NAVREC	*prev;
		short			flags;
		byte			*PPaddress;      /* address of 1st byte of current PROC */
		byte			*starting_point; /* address of DISPLAY statement */
		byte			*base_point;     /* base addr of USE'd proc */
		byte			*pseudo_return;  /* addr of instr following USE */
		struct SLIST	*screen;         /* link to corresponding SCREEN */
		};

	struct KEYREC {				/* holds function key definitions */
		struct KEYREC	*next;
		struct KEYREC	*prev;
		short			flags;
		int				keys[33*4];	/* this should be NUMFUNKEYS*4 */
		short			old_bell_no_fkey;	/* saves Bell_unassigned_Fkey */
		index			old_fkey_item;		/* saves Unassigned_fkay_appl */
		byte			old_fkey_appl;		/* saves Unassigned_fkay_item */
		};

	struct TRANREC {			/* defines a transaction in RT */
		struct TRANREC	*next;
		struct TRANREC	*prev;
		short			 flags;
		short			 x;
		short			 y;
		short			 width;
		short			 height;
		short			 boxtype;
		short			 boxattr;
		short			 appl_no;
		short			 item_no;
		struct SLIST	*base_tran_screen;
		struct SLIST	*save_Current_screen;
		struct NAVREC	*base_tran_navrec;
		struct FIELD	**fields_list;
		pcldef			*save_Currenttran;
		int				 save_RTflags;
		short			 save_cursor_on;
		RECT			 save_PCLcursor;
		field			*save_Currentfield;
		int				fields_list_index;
		};

	struct STACKEDFLD {			/* holds 'stacked' field */
		fldflags		flags;
		short			numchars;
		short			dupnumchars;
		int				userdata;
		byte			*data;
		};

	struct INDREC {				/* holds indirect object stack element */
		struct INDREC		*next;
		struct INDREC		*prev;
		short				flags;
		struct INDREC		*indlist;	/* ==> next INDREC in orthogonal list */
		struct STACKEDFLD	*copy;		/* ==> to stacked field data */
		field				*thefield;	/* addr of unique FIELD structure */
		};

	struct LTPPLIST {			/* defines a logical-to-physical printer */
		struct LTPPLIST	*next;	/* path mapping table */
		struct LTPPLIST	*prev;
		short			 flags;
		char			 logical_path[10];
		char			 physical_path[80];
		};

typedef	struct RTCONTROL {				/* holds function key definitions */
		struct RTCONTROL *next;
		struct RTCONTROL *prev;
		short			flags;
		short			ctl_flags;	/* disabled, pushed, etc.		*/
		short			type;		/* pushbutton, radiobutton, etc. */
		RECT			rect;
/* the following structure member is temporarily changed to int, CAH */
		int				cid;		/* control ID from WindowMan */
		HANDLE			hcontrol;	// Control handle for WindowsNT
		dlist			*dlp;		/* pointer to corresponding display list */
		byte			*title;		/* NULL, or points to title object */
		void			*object;	/* points to FIELD or object */
		void			*subject;	/* routine, value, or flagbit */
		unsigned long	tag;		/* tag for HIDECONTROL, etc. */
		short			fcolor;
		short			bcolor;		/* for lines in reliefbuttons */
		short			dfcolor;	/* foreground in down state	*/
		short			dbcolor;	/* background in down state	*/
		short			bordwidth;	/* width of relief button border	*/
		dlist			*dlp2;		// Points to related display list, if any
		byte			rtn_type;
		void			*rtn;
		} rtcontrol;

	struct RT_TEFIELD {				/* holds function key definitions */
		struct RT_TEFIELD	*next;
		struct RT_TEFIELD	*prev;
		short				flags;
		field				*fp;	/* point to FIELD structure */
/* the following structure member is temporarily changed to int, cah */
		int				*terec;	/* pointer to TE control thingie */
		short				boxtype;
		short				teflags;
		};

	struct PLFOCUS {			/* manages picklists for mouse controls */
		struct PLFOCUS	*next;
		struct PLFOCUS	*prev;
		short			 flags;
		struct plwork	*topmost;
		HWND			pickhwnd;
		};

	struct STIMER_REC {				/* defines a SETTIMER statement record */
		struct STIMER_REC	*next;
		struct STIMER_REC	*prev;
		short			 flags;
		byte			 timer_number;	/* ID in the range 0-255 */
		byte			 function;		/* IDLE/NOINPUT/INTERVAL/TIMEOFDAY */
		int				 seconds;
		long			 trigger_time;	/* in sec.s since 1970 */
		byte			 object;		/* PrePCL or PreCSR */
		byte			 ano;
		index			 ino;
		void			*funcptr;		/* Pointer to PCL/CSR struct */
		long			 own_id;		/* value from SETTIMER verb */
		};

#define MAX_RT_TIMERS 10
#define TIMER_SUM		0
#define TIMER_COUNT		1
#define TIMER_MIN		2
#define TIMER_MAX		3

/*
** External Data Definitions
*/

/*
** Allocated in ramalloc.c
*/
extern	struct DLIST	*FYImsg;		/* points to FYI msg block */
extern	short			Maxpicks;		/* Max no. of picklist elements */
extern	short			Maxboxes;		/* Max no. of dataentry boxes */
extern	dlist			*pickloc[];		/* holds picklist display list ptrs */
extern	char			blanks[];

/* ------------------- */
/*  Allocated in io.c  */
/* ------------------- */

extern	int				RTwid;
extern	dlist			*Ustatus;		/* status line for Appl use */
extern	dlist			*Estatus;		/* error msg line for Appl use */
extern	char			Sline2[];

/* ------------------------ */
/*  Allocated in supover.c  */
/* ------------------------ */
extern	int				SVOwid;			/* super'oride window ID */
extern	struct DLIST	*SVOscreen;
	
/* ------------------------ */
/*  Allocated in dates.c  */
/* ------------------------ */
extern	short			first_day_of_week;

#endif

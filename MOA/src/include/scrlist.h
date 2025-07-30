#ifndef SCRLIST_H
#define SCRLIST_H

#ifndef TYPEDEFS_H
#	include <typedefs.h>			/* needed for SLIST structure */
#endif
#ifndef RT_H
#	include <rt.h>			/* needed for SLIST structure */
#endif

/* window border widths	*/
#define	TITLEBARHEIGHT	19
#define	SCROLLBARWIDTH	16
#define	SCROLLBARHEIGHT	16
#define BORDERWIDTH		4
#define PDMENUHEIGHT	19

#define MAXSCRSTRING 128

typedef struct SCRSTRING {
	struct SCRSTRING	*next;
	struct SCRSTRING	*prev;
	short				flags;
	char				tstring[MAXSCRSTRING];
	} scrstring;

typedef struct SCRPARMS {
	short	x;
	short	y;
	short	width;
	short	height;
	short	boxtype;
	byte	forecolor;
	byte	backcolor;
	byte	wp5color;
	byte	nodisplay;
	} scrparms;

typedef struct ALTKEY {
	ddr			join_obj;
	rtcontrol	*crp;
	} altkey;

typedef struct SLIST slist;

struct SLIST {
	slist		*next;
	slist		*prev;
	short		flags;		/* INUSE, PROTECTED, etc. */
	short		x;			/* x co-ordinate of screen box */
	short		y;			/* y co-ordinate of screen box */
	short		width;		/* width of screen box */
	short		height;		/* height of screen box */
	short		type;		/* box type of screen box */
	byte		forecolor;
	byte		backcolor;
	byte		wp5color;
	dlist		*main;		/* Pointer to the dlist head for non-fields */
	dlist		*fields;	/* Pointer to the dlist head for fields */
	scrstring	*strings;	/* ptr to literal string list, Init by sl_insert */
	rtcontrol	*controls;	/* ptr to list of mouse controls */
	void		*nav;		/* Pointer to nav list, Init by caller */
	short		appl_no;	/* Application # of PCL routine */
	short		item_no;	/* index # of PCL routine */
	struct	PLFOCUS *plfocushead;	/* pick list focus head for screen */
	short		save_cursor_on;	/* cursor state */
	RECT		save_PCLcursor;	/* cursor definition */
	byte		ff_oldforecolor;
	byte		ff_oldbackcolor;
	int			ddh;		/* data dictionary handle for refresh */
	HANDLE		focused_control;	// Current NT control or NULL
	altkey		altkeys[36];	// 0-9 & A-Z controls
};

extern slist		*SLfree;
extern scrstring	*SLstringfree;

// --------------------------------------------------------------
//  Flags used in locally-defined CONTROLs like pushbutton, etc.
// --------------------------------------------------------------
#define	CTL_ALLOCCNT		20
#define	CTL_FLAG			0x8000
#define	CTL_MASK			0x7FFF
#define	CTL_CLEAR			0x0000
#define	CTL_INUSE			0x0001
#define	CTL_DOWN			0x0002
#define	CTL_PUSHED			0x0004
#define	CTL_DISABLED		0x0008
#define	CTL_SUBJECT_OFB		0x0010	// 'subject' is pointer to OFB object
#define	CTL_DOWN_MASK		0xFFFD
#define	CTL_PUSH_MASK		0xFFFB
#define	CTL_DISABLE_MASK	0xFFF7

	slist	*sl_create(void);
	slist	*sl_append(slist *slhead, scrparms *slp);
	void	sl_remove(slist *slhead, slist *slptr);
	void	sl_free(slist *slhead, int do_dl);
	void	sl_refresh(slist *slhead, RECT *dirty);
	void	sl_refreshfrom(slist *slhead, slist *slstart, RECT *dirty);
	void	sl_makecurrent(slist *slhd, slist *slptr);
	void free_controls(rtcontrol *crp,rtcontrol *freebase);
	void sl_calcbounds(slist *slptr,RECT *brect);
	
#define sl_getnext(slhd,slp) (nextq(slhd,slp))
#define sl_getprev(slhd,slp) (prevq(slhd,slp))
#define sl_getfirst(slhd) (gl_getfirst(slhd))
#define sl_getlast(slhd) (gl_getlast(slhd))
#endif

/* ---------------------------------------------------------- */
/*  This structure contains everything needed by the various  */
/*  PICKLIST routines.  By creating an auto storage version   */
/*  of this structure, and passing a pointer to it, all of    */
/*  picklist routines can be serially reusable and reentrant. */
/* ---------------------------------------------------------- */

#ifndef PICKLIST_H
#define PICKLIST_H

	/* -------------------------------------------------------------- */
	/* FLAG values for the 'flags' element of the picheader structure */
	/* -------------------------------------------------------------- */

#define PLF_NOSCROLLBAR		0x0001	/* no scroll bar wanted, ever */
#define PLF_CONDSCROLLBAR	0x0002	/* scrollbar wanted only if needed */
#define PLF_SCROLLBAR		0x0004	/* scrollbar always wanted */
#define PLF_ONECLICK 		0x0008	/* 1 click makes selection */
#define PLF_TWOCLICK 		0x0010	/* 2 clicks make selection */
#define PLF_SELECTCASE 		0x0020	/* case sensitive on selection */
#define PLF_MATCHCASE 		0x0040	/* case sensitive matching (PICKVERIFY) */
#define PLF_RELATIVEPOSN	0x0080	/* relative (vs: absolute) x,y pos'n */

struct plwork {

	/* --------------------------------------------------------- */
	/* The following three elements must be set up by the caller */
	/* BEFORE calling show_picklist or any of the other picklist */
	/* routines which require a pointer to a 'plwork' structure. */
	/* --------------------------------------------------------- */

	picheader		*picklist;		/* The picklist header to use */
	dlist			*plscreen;		/* Which display list to use */
	short			flags;
	short			foldflag;		/* TRUE if lc -> UC */

	/* -------------------------------------------------------------- */
	/* The following two elements MAY BE set up by the caller before  */
	/* calling show_picklist.  If the 2nd argument to show_picklist() */
	/* is FALSE, then it will set them up (which is the normal case)  */
	/* -------------------------------------------------------------- */

	dlist			*pickloc[50];
	char			**pickitem;
	char			*lastpick;	/* this points at the pickitem last chosen */

	/* ------------------------------------------------------------ */
	/* The following stuff is needed by the picklist routines and   */
	/* used to be static variables.  Now they are dynamic, which    */
	/* makes it possible for picklists to be reentered.  All this   */
	/* stuff is initialized by the picklist routines and SHOULD NOT */
	/* be set up by the caller...                                   */
	/* ------------------------------------------------------------ */

	dlist			*selbar;			/* for the REVERSED selection bar */
	dlist			*scrollbar_control;	/* for the pldrawcontrol() */
	dlist			*ourbox;
	dlist			*ourline;
	dlist			*ourtitle;
 	short			save_color;
	RECT			insidebox;
	int				max_items_displayed;	/* items shown in box */
	short			items_in_list;			/* total items in list */
	short			lines_in_box;			/* lines within box */
	short			items_in_box;			/* items in a box */
	int				lp1;
	int				lpn;
	int				lp;
	int				cp;
	int				canscroll;
	int				lastline;
	short			cellwidth;
	short			cellheight;
	struct DHRECT	selrect;
	HWND			plscroll;
	RECT			sbar;
	short			has_scrollbar;
	};

	/* ----------------------------------------------- */
	/* Function prototypes for modules using picklists */
	/* ----------------------------------------------- */

	XDLL int	show_picklist(struct plwork *pl, int pi_built);
	XDLL int	val_picklist(field *thisfield, struct plwork *pl, 
						short showflag);

	void	remove_picklist(struct plwork *pl);
	int		do_picklist(struct plwork *pl, short selbar_offset,
						short draw_selbar);
	void	display_picklist(struct plwork *pl, short pi_built, 
						short item_offset, short selbar_offset, 
						short selbar_list_offset);
	int		plproc(struct plwork *pl, int ch, short itemdepth);
	int 	callout_key_filter(struct plwork *pl, int ch);
	bool	ctl_under_pick(RECT *ctlrect);
	void	pl_drawcontrol(dlist *dlp);
	void 	pl_reverse(dlist *selbar);
	void	pl_drawstring(dlist *dlp);
#endif


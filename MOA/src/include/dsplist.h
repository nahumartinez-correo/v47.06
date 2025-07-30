/*&
**
**                 Display list structure
**
*/

#ifndef DSPLIST_H
#define DSPLIST_H

#ifndef LISTMGR_H
#include <listmgr.h>
#endif

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef TYPEDEFS_H
#include <typedefs.h>
#endif


typedef struct DLIST dlist;

struct DLIST {
	dlist	*next;
	dlist	*prev;
	short	flags;			/* INUSE, PROTECTED, etc. */
	short	uflags;			/* for FIELD programmer use only! */
	short	x;				/* x co-ordinate of object */
	short	y;				/* y co-ordinate of object */
	RECT	bounds;			/* Bounding rectangle */
	byte	font;			/* Font */
#ifdef SIXTEENBITCHARS
	byte	otherfont;		/* International font */
#endif /* SIXTEENBITCHARS */
	byte	size;			/* Font Size */
#ifdef SIXTEENBITCHARS
	byte	othersize;		/* International font size */
#endif /* SIXTEENBITCHARS */
	short	attr;			/* Display attributes */
	byte	forecolor;		/* Foreground color */
	byte	backcolor;		/* BackGround color */
	short	portid;			/* When used, contains the port (window) id used
							   to render the object */
	void	(*dspfunc)(dlist *);	/* Pointer to function to display obj */
	void	*obj;			/* Pointer to object to be displayed 
							   (or the object itself) */
	byte	join_ano;		/* for JOINSCREEN and CUTSCREEN */
	unsigned short	join_ino;	/* for JOINSCREEN and CUTSCREEN */
	rtobj	dyn_val;		/* field dynamic validation object instance */
};

extern dlist		*DLfree;
extern short		theWP5Color;

/*
** define for list flags
*/
#define DL_INUSE 		GL_INUSE
#define DL_ISVISIBLE 	0x0002
#define DL_PROTECTED 	0x0004
#define DL_NODISPLAY 	0x0008
#define DL_HILIT		0x0010
#define DL_BSPEC		0x0020		/* indicates bounds require special
									   processing at refresh time */
#define DL_INUPDATE		0x0040		/* Update Event in progress */
#define DL_RTDLPL		0x0100		/* used by RT for picklist lines */
#define DL_FIELDFOCUS	0x0200		/* used by RT's datacollect */
#define DL_CENTER		0x0400		/* used by RT's custom control text */
#define DL_LEFT			0x0800		/* used by RT's custom control text */
#define DL_RIGHT		0x1000		/* used by RT's custom control text */
#define DL_NOBOX		0x2000		/* used by RT's combo fields		*/

#define SL_INUSE 		GL_INUSE
#define SL_INUPDATE		0x0040		/* Update Event in progress */

extern dlist	*dl_create(void);
extern dlist	*dl_draw(dlist *dlhead,register short x,register short y,
					register RECT *bounds,register void * object,
					void (* func)(dlist *));
XDLL void		dl_refresh(dlist *dlptr, RECT *dirty);
XDLL void		dl_refresh_rect(RECT *);
extern void		dl_cleanup(dlist *);

#define dl_remove(dlhd,dlptr) gl_remove(dlhd,dlptr,DLfree)
#define dl_free(dlhd) gl_free(dlhd,DLfree)
#define dl_getnext(dlhd,dlp) (nextq(dlhd,dlp))
#define dl_getprev(dlhd,dlp) (prevq(dlhd,dlp))
#define dl_getfirst(dlhd) (gl_getfirst(dlhd))
#define dl_getlast(dlhd) (gl_getlast(dlhd))

extern void normalize_linerect(RECT *r, dlist *dlptr);

#endif

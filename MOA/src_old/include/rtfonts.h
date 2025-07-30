#ifndef RTFONTS_H
#define	RTFONTS_H
/* Text styles. */

#define normalText		0x00	/* Normal, is fastest by far. */
#define boldText		0x01	/* Emboldened, is wider than normal. */
#define fixedBoldText	0x02	/* Bold, but character width is same. */
#define underlineText	0x04	/* Underlined. */
#define italicText		0x08	/* Italicized. */
#define ghostText		0x10	/* Drawn in Gray (best w/dbl stroked font). */
#define outlineText		0x20	/* Outlined, is taller & wider than normal. */
#define shadowText		0x40	/* Drop shadow text	*/
#define	reverseText		0x80	/* Reverse video	*/

/* Text justification flags (for DrawJustText). */

#define leftJustify	0	/* Left justification.		*/
#define rightJustify	1	/* 	   Right justification. */
#define centerJustify	2	/*     Center Justification.	*/

/* For vertical justification, use leftJustify for top, rightJustify 
   for bottom, and centerJustify for (of course) center. */

/* Transfer modes, used by font and pattern operations. */
/* Boolean Source Transfer modes.  All systems, mono/color. */

#define srcCopy		0	/* Copy from source into window. */
#define srcOr		1	/* Or from source into window. */
#define srcXor		2	/* Xor from source into window. */
#define srcBic		3	/* Clear bits set in source in window. */
#define notSrcCopy	4	/* Copy inverse of source into window. */
#define notSrcOr	5	/* Or inverse of source into window. */
#define notSrcXor	6	/* Xor inverse of source into window. */
#define notSrcBic	7	/* Clear bits with zero in source in window. */



#define MAXMENUS	16		/* Max # of menus per connection */
#define	MAXEVTLEN	1024	/* Maximum # of Bytes/Event */
#define	NULLPTR	((void *)0)
#define	MAXWINDOWS	10		/* Maximum # of Windows/Connection */


/* hardcoded font table with point sizes to height/width */
typedef	struct
{
	int	font_number;
	int	point_size;
	int	height;
	int	width;
} WMFontInfo;

typedef struct
{
	HFONT	hfont;
	int		default_size;
} WMFonts;

struct accelkeytable
{
	DWORD	key;
	BYTE	enabled;
	WORD	data;
};
typedef struct accelkeytable AccelKeyTable;

struct winpat
{
	WORD	Index; 				/* system pattern index - 0xffff=special */
	HBITMAP	hBitmap;			/* bitmap handle for special pattern */
	HBRUSH	hBrush;				/* brush handle for special pattern */
};

typedef struct winpat WINPAT;

struct winfnt
{
	WORD	Index; 				/* Font Index */
	WORD	Size;				/* Font Size */
	WORD	Mode;				/* Font Mode */
	WORD	Face;				/* Font Face */
	DWORD	spExtra;			/* Font SpaceExtra */
	WORD	txExtra;			/* Font TextExtra */
	WORD	UsedFontSize;		/* the font size had to use */
};

typedef struct winfnt WINFNT;

struct winpen
{
	WORD	Index;				/* current pen pattern index */
	BYTE	Pattern[8];			/* current special pen bitmap */
	POINT	Size;				/* current pen size */
	WORD	Mode;				/* current pen mode */
	long	ForeColor;			/* current pen foreground color */
	long	BackColor;			/* current pen background color */
};

typedef struct winpen WINPEN;

struct Mevent
{
	long	message;
	RECT	where;
};

typedef struct Mevent Mevent;

struct MenuInfo
{
	int     MenuID;         /* menu ID from the application (on ACB) */
	BYTE    ItemCnt;        /* count the number of items in menu */
	BYTE	Disabled;		/* number of time menu was disabled */
};

typedef struct MenuInfo MenuInfo;

struct winwcb
{
	HWND	hWnd;				/* Window Handle */
	HDC		hDC;				/* Handle to Device Context */
	struct winccb *ccb_p;		/* pointer to ccb */
	WORD	WinID;				/* window identifier */
	RECT	Size;				/* window size and position */
	WORD	Style;				/* window style */
	WORD	goAway;				/* window options */
	long	mousemode;			/* mouse mode */
	HPALETTE	hPal;			/* handle to the logical palette for this win*/
	int		numPal;				/* number of colors in the palette */
	long	ForeColor;			/* current foreground color */
	long	BackColor;			/* current background color */
	POINT	Location;			/* current pen location */
	HFONT	hFont;				/* handle pointer to Font */
	WINFNT	CurFont; 			/* current Font Info */
	WINFNT	ReqFont; 			/* requested Font Info */
	WINPAT	Back;				/* current background pattern */
	WINPAT	Fill;				/* current fill pattern */
	WINPAT	Pen;				/* current pen pattern */
	HPEN	hPen;				/* current pen handle */
	WINPEN	CurPen;				/* current Pen Info */
	WINPEN	ReqPen;				/* requested Pen Info */
	WORD	flags;				/* window flags */
	HCURSOR	hCursor;			/* current cursor handle */
	HCURSOR	hAppCursor;			/* current application cursor handle */
	POINT	ApplHotSpot;		/* the hot spot offset for the application cursor */
	POINT	ApplCsrPos;
	short	MouseCsrVis;
	short	ApplCsrVis;
	HRGN	PendingRgn;			/* pending update region */
	HRGN	ClippingRgn;		/* clipping region - cliprect,cliprgn */
	HRGN	SaveClippingRgn;	/* clipping region - cliprect,cliprgn */
	HRGN	UpdateRgn;			/* update region - dirty region */
	long	UpdateTime;			/* time of last update */
	Mevent	LastMouse;			/* the last mouse event used */
	MenuInfo  menus[MAXMENUS];  /* Menu stuff */
	BYTE	BeginMenuFlag;		/* are we in BeginMenu-EndMenu sequence */
};

typedef struct winwcb WINWCB;

#endif

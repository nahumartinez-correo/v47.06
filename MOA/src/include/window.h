/* Window system definitions */

#ifndef NULL
#define NULL (0)
#endif
#ifndef EOF
#define EOF (-1)
#endif

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

/* Bitwise Source Transfer modes, TI34010 color only. */

#define srcBitCopy	8	/* Bitwise,        S => D */
#define srcBitAnd	9	/* Bitwise,    S & D => D */
#define srcBitAndNot	10	/* Bitwise,   S & ~D => D */
#define srcBitZero	11	/* Bitwise,        0 => D */
#define srcBitOrNot	12	/* Bitwise,   S | ~D => D */
#define srcBitXnor	13	/* Bitwise, ~(S ^ D) => D */
#define srcBitNot	14	/* Bitwise,       ~D => D */
#define srcBitNor	15	/* Bitwise, ~(S | D) => D */
#define srcBitOr	16	/* Bitwise,    S | D => D */
#define srcBitNop	17	/* Bitwise,        D => D */
#define srcBitXor	18	/* Bitwise,    S ^ D => D */
#define srcBitNotAnd	19	/* Bitwise,   ~S & D => D */
#define srcBitOne	20	/* Bitwise,       ~0 => D */
#define srcBitNotOr	21	/* Bitwise,   ~S | D => D */
#define srcBitNand	22	/* Bitwise, ~(S & D) => D */
#define srcBitNotCopy	23	/* Bitwise,       ~S => D */

/* Arithmetic Source Transfer modes, TI34010 color only. */

#define srcAddOver	24	/* Add numerically, with overflow. */
#define srcAddPin	25	/*  "       "     , pinned at maximum. */
#define srcSubOver	26	/* Subtract numerically, with overflow. */
#define srcSubPin	27	/*     "         "     , pinned at minimum. */
#define srcAdMax	28	/* dest = MAX(source, dest) */
#define srcAdMin	29	/* dest = MIN(source, dest) */

/* Force color number _result_ (after transfer mode operation's applied) */
/* of 0 to be transparent.  It's a flag, OR it with the desired transfer */
/* mode [i.e. PenMode(srdAddPin | transparent);].  TI34010 color only. */

#define transparent	32

/* Text styles. */

#define normalText	0	/* Normal, is fastest by far. */
#define boldText	1	/* Emboldened, is wider than normal. */
#define fixedBoldText	2	/* Bold, but character width is same. */
#define underlineText	4	/* Underlined. */
#define italicText	8	/* Italicized. */
#define ghostText	16	/* Drawn in Gray (use a dbl stroked font). */
#define outlineText	32	/* Outlined, is taller & wider than normal. */
#define strikeoutText	64	/* Strikeout. */

/* Text justification flags (for DrawJustText). */

#define leftJustify	0	/* Left justification.		*/
#define rightJustify	1	/* 	   Right justification. */
#define centerJustify	2	/*     Center Justification.	*/

/* For vertical justification, use leftJustify for top, rightJustify 
   for bottom, and centerJustify for (of course) center. */

/* Window types, parameter 'procID' in 'NewWindow' call. */

#define documentProc	0	/* Regular window with all the trimmings. */
#define dBoxProc	1	/* No title bar, inset border. */
#define plainDBox	2	/* No title, no dropshadow. */
#define altDBoxProc	3	/* No title, dropshadow. */
#define noGrowDocProc	4	/* Fixed size regular window. */
#define noBordDBox	5	/* No border/fill at all. */
#define iconWProc	6	/* Iconified window. */

/* Window visible, parameter 'visible' in 'NewWindow' call. */

#define hideWindow	0	/* Window invisible */
#define showWindow	1	/* Window visible */

/* Values for NewWindow's goAwayFlag field (window options). */

#define noOptions	0	/* No window options */
#define hasGoAway	1	/* Has go-away region (active window only). */
#define hasZoom		2	/* Has zoom region (active window only). */
#define hasSendToBack	4	/* Has send-to-back rgn (active wdo only). */
#define hasGrow		8	/* Has grow zone. */
#define hasDrag		16	/* Has drag area. */
#define isShy		32	/* Is shy, must hit drag region to select. */

/* Window order, parameter 'behindWindow' can be one of the following values
   or a windowid of another window to put this window behind. */

#define frontWindow	-1	/* Make this the frontmost window */
#define secondWindow	-2	/* Make this the second frontmost window */
#define backWindow	-100	/* Make this the backmost window */

/* Parameter 'requestUpdate' can have the following values: */

#define noupdateRequest	0	/* Dont generate an update event */
#define updateRequest	1	/* Generate an update event if this makes window dirty */

/* Standard patterns for numeric reference. */

#define blackPattern	((Pattern *)0)	/* Black pattern */
#define dkgreyPattern	((Pattern *)5)	/* Dark grey pattern */
#define greyPattern	((Pattern *)7)	/* Grey pattern */
#define ltgreyPattern	((Pattern *)10)	/* Light grey pattern */
#define whitePattern	((Pattern *)15)	/* White pattern */

/* Notification icons for Notify() function. */

#define noNotify	0	/* Remove notification icon. */
#define genericNotify	0x15	/* Generic notification icon. */
#define errorNotify	0x16	/* System Error notification icon. */
#define mailNotify	0x17	/* Mail notification icon. */
#define phoneNotify	0x18	/* Phone (talk) notification icon. */
#define clockNotify	0x19	/* Clock (alarm) notification icon. */

/* Event code definitions. */

#define MAXEVENT	9

#define mouseEvt	1
#define keyEvt		2
#define activateEvt	3
#define updateEvt	4
#define windowEvt	5
#define menuEvt		6
#define secondWindEvt	7	/* Second-window mouse event. */
#define scrapEvt	8	/* Clipboard updated event. */

/* Window event types, passed in the message field of a window event. */

#define windowGoaway	1
#define windowMove	2
#define windowResize	3
#define windowZoom 	4

/* Bitmask definitions to be used with 'GetEvent' and 'CheckEvent'. */

#define noWindow	0
#define anyWindow	(-1)
#define noEvent		0
#define anyEvent	(-1)

/* Parameter used in MouseMode command. The mousemode parameter is anded
   with the mouse keycode and flags and shipped to the application if
   the result is nonzero.  Use the key flags definition for specific events.
   Ex: MouseMode(F_KEYDOWN | F_KEYUP); gives all key down and up events.
   When using mouseAll all changes in mouse keys or position inside the
   window are reported.  Combined with HideCursor(); all changes anywhere
   are reported.
*/

#define mouseAll	0x10000000	/* All mouse events inside my window */
#define mouseStream	0x20000000	/* Make stream of mouse events */
#define mouseCrossWinds 0x40000000	/* Let mouse post to many windows. */

/* Verbs passed to the StdXXX routines by the ParsePicture command. */

#define frameVerb	0
#define paintVerb	1
#define eraseVerb	2
#define invertVerb	3
#define fillVerb	4

#define setVerb		5
#define sectVerb	6
#define unionVerb	7
#define diffVerb	8
#define xorVerb		9

/* Low-level structures. */

typedef struct Point {		/* Graphics point structure. */
	short v;		/* Vertical component. */
	short h;		/* Horizontal component. */
} Point;

#ifdef OLD_RECTS		/* So's to make folks happy we can support. */
				/* the old style of Rect structure. */
typedef struct Rect {
    short top;			/* 0: Topmost coordinate, etc. */
    short left;			/* 2: */
    short bottom;		/* 4: */
    short right;		/* 6: */
} Rect;

#else				/* Rect variants. */

typedef union Rect {		/* Graphics Rectangle structure. */
    struct {			/* It's either 4 edges or 2 points */
	int top;		/* depending upon need. */
	int left;
	int bottom;
	int right;
    } edge;
    struct {			/* This avoids error-prone casting. */
	Point topLeft;
	Point botRight;
    } corner;
} Rect;

#endif				/* Rect variants. */

typedef struct Region {
	unsigned short rgnSize;	/* Size of total region in bytes (min 10). */
	Rect rgnBBox;		/* Bounding box of region. */
	short rgnData;		/* Optional region data (maybe large). */
} Region;

typedef Region * RgnPtr;
typedef RgnPtr * RgnHandle;

typedef struct RegionHdr {
	unsigned short rgnSize;	/* Size of total region in bytes (min 10). */
	Rect rgnBBox;		/* Bounding box of region. */
} RegionHdr;

#ifndef NT
typedef struct Polygon {
	unsigned short polySize; /* Size of total polygon in bytes (min 10). */
	Rect polyBBox;		/* Bounding box of region. */
	Point polyPoints;	/* Polygon data points (maybe large). */
} Polygon, *PolyPtr, **PolyHandle;
#endif

typedef struct PolyHdr {
	unsigned short polySize; /* Size of total polygon in bytes (min 10). */
	Rect polyBBox;		/* Bounding box of region. */
} PolyHdr;

typedef struct Picture {
	unsigned long picSize;	/* Size of total picture in bytes. */
	Rect picFrame;		/* Picture frame. */
				/* Plus variable-sized picture data. */
} Picture, *PicPtr, **PicHandle;

#define picLParen 0
#define picRParen 1

typedef struct Pattern {
    unsigned char bytes[8];	/* Bit array (8 * 8 bits) */
} Pattern;

typedef struct Cursor {
    unsigned short data[16];	/* Cursor image. */
    unsigned short mask[16];	/* Cursor mask. */
    Point hotSpot;		/* Point aligned with mouse. */
} Cursor;

typedef struct Icon {
    unsigned long data[32];	/* Bit array (32 * 32 bits) */
} Icon;

typedef struct BitMap {		/* System Bitmap structure. */
	char *baseAddr;		/* Pointer to bit image. */
	short rowBytes;		/* Number of bytes per row of each plane. */
	Rect bounds;		/* Logical Bounding rectangle of planes. */
} BitMap;

typedef long Fixed;		/* Fixed-point data type. */

/*
** Actually a Fixed is the following structure.  We just call it a long so
** that it may be passed by _value_ rather than reference.
**
** typedef struct {
**	short Integer;		Integral part of fixed-point number.
**	short Fraction;		Fractional part of fixed-point number.
** } Fixed;
*/

typedef struct FontInfo {
    short ascent;		/* Height above baseline of font. */
    short descent;		/* Height below baseline of font. */
    short widMax;		/* Maximum character width. */
    short leading;		/* Row-to-row spacing of this font. */
} FontInfo;


typedef struct FontList {
    char fontnum;		/* Font number, 0..99 */
    char fontname[15];		/* Name, up to 15 chars, null-term if < 15. */
} FontList;

typedef struct PenState {	/* A pen state save record. */
    Point pnLoc;		/* Pen's location. */
    Point pnSize;		/* Pen's size. */
    short pnMode;		/* Pen's drawing mode. */
    Pattern pnPat;		/* Pen's pattern. */
} PenState;

/* Event record, used to pass events to the application */

typedef struct EventRecord {	/* An Event Record. */
	short who;		/* Window identifier */
	short what;		/* Event type. */
	long message;		/* Event message. */
	unsigned long when;	/* Time stamp. */
	Rect where;		/* Area or rectangle location. */
} EventRecord;

typedef struct HdwrType { /* A Hardware Type record. */
    short hdots;	/*  0: Horizontal dots total. */
    short vdots;	/*  2: Vertical dots total. */
    short hres;		/*  4: Horizontal dots-per-inch resolution. */
    short vres;		/*  6: Vertical dots-per-inch resolution. */
    short kbdtype;	/*  8: Kbd type, 0=none, 1=AT, 2=ISC, 3=Both. */
    short montype;	/* 10: Mon type, 0=9" BW, 1=14" BW, 2=14" Color. */
    short proctype;	/* 12: Proc type, 0=68000, 1=68010, 2=68020. */
    short nplanes;	/* 14: Number of video planes. */
    long memsize;	/* 16: Physical amount of memory. */
    short planesinuse;	/* 20: Number of planes available. */
    short colormodel;	/* 22: Color Model, 0=Mono, 1=WP5, 2=WEB, 3=WP6. */
} HdwrType;

typedef struct WmanVrsns { /* Window Manager Version flags -- 0x0100 == V1.00 */
    unsigned short wman; /* Packed BCD version of Window Manager. */
    unsigned short clunk; /* Packed BCD version, ClunkDraw ($1050 == V10.50 */
} WmanVrsns;

/* State structure passed to StdXXX routines by the ParsePicture command. */

typedef struct GrafState {
    Pattern *bkPat;		/* Background pattern. */
    Pattern *pnPat;		/* Pen pattern. */
    Pattern *fillPat;		/* Fill pattern. */
    Point pnLoc;		/* Pen location. */
    Point pnSize;		/* Pen size. */
    short pnMode;		/* Pen's transfer mode. */
    short txFont;		/* Font number for text. */
    short txFace;		/* Text's character style. */
    short txMode;		/* Text's transfer mode. */
    short txSize;		/* Font size for text. */
    Fixed spExtra;		/* Extra space for ASCII space chars. */
    Fixed txExtra;		/* Extra space for text chars. */
    short txRotate;		/* Text rotation value, degrees CCW. */
    short fgColor;		/* Foreground color. */
    short bkColor;		/* Background color. */
    short colrBit;		/* Color plane(s) in use. */
} GrafState;

/* Flag values for options field of the Standard File I/O structure. */
#define SF_GETFILE 0		/* Open a file. */
#define SF_PUTFILE 1		/* Save As a file. */
#define SF_ALLFILES 2		/* Show all files (.files, like ls -a). */
#define SF_HIDEBAD 4		/* Hide unselectables (instead of gray). */
#define SF_XPOS 8		/* Override X window position. */
#define SF_YPOS 0x10		/* Override Y window position. */
#define SF_UID 0x20		/* Override UID for permission check. */
#define SF_GID 0x40		/* Override GID for permission check. */

/* Standard File (File Picker) I/O structure. */
typedef struct SFDialog {
    short options;		/* Flags from above. */
    short xpos;			/* X window position override, if enabled. */
    short ypos;			/* Y window position override, if enabled. */
    short uid;			/* UID override, if enabled. */
    short gid;			/* GID override, if enabled. */
    char *initialname;		/* Opt. Initial (default) name to show. */
    char *shellmatch;		/* Opt. Shell-pattern match filter string. */
    char *regexpmatch;		/* Opt. Regular expr match filter string. */
    char *title;		/* Opt. Window Title. */
    char *returnedname;		/* Returned string, malloced for you. */
} SFDialog;

/*eject*/
#if !defined(lint) || defined(_lint)
/*** Following are the Window Manager interface routine declarations. ***/

int InitGraf(void);	/* Init graphics system, returns trap queue fd. */
void EndGraf(void);	/* Shuts down graphics system. */
int ConnectGraf(register char *system);	/* Connect to windowmanager on spec. system, NULL means own system, returns a connection fd. */
void DisconnectGraf(register int connectionfd);	/* Close a connection to a windowmanager. */
int WMIsLocal(register int connectionfd); /* TRUE if WM is local, FALSE if network. */
void (*Event(register int type, register void (*proc)()))(); /* Bind a procedure to an event. */
void (*EventWithArg(register int type, register void (*proc)(), register long extraarg))(); /* Bind a procedure with arg to an event. */
unsigned long EventError(void);
long GetKey(register int windowid); /* Get a key from spec. window */
int ReadKeys(int windowid, long *bp, int size);		/* Read buffered keys, from spec. window */
EventRecord *GetEvent(register int windowmask, register int eventmask);	/* Get next event of spec. types from spec. windows */
EventRecord *GetEventWithArg(register int windowmask, register int eventmask, long extraarg);	/* Get next event of spec. types from spec. windows */
int CheckKey(register int windowid); /* Check if key is available from spec. window */
int CheckEvent(register int windowmask, register int eventmask); /* Check if event of spec. types from spec. windows available */
int CheckEventWithArg(register int windowmask, register int eventmask, long extraarg);	/* Check if event of spec. types from spec. windows available */
int CheckNextEvent(register int windowmask, register int eventmask); /* Check if next event is of spec. types from spec. windows */
void ReleaseEvent(void); /* Makes current event wait return without being satisfied */
void FlushWindow(void);	/* Flush the window command buffer, done automatically when waiting for event */
void FlushTwice(void);	/* Flush the window command buffer, wait till done. */
void FlushDebug(int flag); /* Flush after every command?  For debugging. */

/* Create a new window on the system that connectionfd is connected to */
int NewWindow(Rect *boundsRect, char *title, int visible, int procID,
		    int behindWindow, int windowOptions, int connectionfd);
void DisposeWindow(register int windowid);	/* Dispose of a window. */
void SetWTitle(register int windowid, register char *title); /* Change window title */
void HideWindow(register int windowid);	/* Make window nonvisible */
#ifndef NT
void ShowWindow(register int windowid);	/* Make window visible */
#endif
void SelectWindow(register int windowid); /* Make window frontmost and input focus */
void SendBehind(register int windowid, register int behindWindow);/* Move window behind another window */
void DrawGrowIcon(register int windowid); /* Draw resizing Icon. */
void WindowStyle(register int styleflag);
#ifndef NT
void MoveWindow(int windowid, int hGlobal, int vGlobal, int makefront);	/* Move window to another location on screen */
#endif
void SizeWindow(int windowid, int newwidth, int newheight, int requestUpdate);	/* Change the size of a window */
void ZoomWindow(int windowid, Rect *newsize, int makefront); /* Zoom a window to new rect. */
void IconifyWindow(register int windowid); /* Iconify a window. */
void UnIconifyWindow(register int windowid); /* Restore (uniconify) a window. */
void Notify(register int windowid, register short notifyflag); /* Post a notification alert. */

void BeginUpdate(register int windowid); /* Tell windowmanager to only execute commands for dirty area. */
void BeginUpdateRect(register int windowid, register Rect *updateRect); /* Tell windowmanager to only execute commands for this dirty area. */
void EndUpdate(register int windowid); /* Tell windowmanager to execute commands on normal area */
void InvalRect(register Rect *badRect);	/* Make a rectangle in the window dirty */
void InvalRgn(register RgnHandle badRgn); /* Make a region in the window dirty */
void ValidRect(register Rect *goodRect); /* Make a rectangle in the window nondirty */
void ValidRgn(register RgnHandle goodRgn); /* Make a region in the window nondirty */

void SetPort(register int windowid); /* Select current window, for command output */
void SetPorts(register int windowmask);	/* Select current windows, for command output */
void GetPort(register int *windowid); /* Get current selected window */
void SetOrigin(register short horz, register short vert); /* Set window coordinate system origin */
void ClipRect(register Rect *rect);	/* Limit commands to a window to only be executed inside a rectangle */
void ClipRectSect(register Rect *rect); /* Further limit commands... */
void ClipRectUnion(register Rect *rect); /* Un-limit commands... */
void ClipRectDiff(register Rect *rect);
void ClipRectXor(register Rect *rect);
void SetClip(register RgnHandle rgn); /* Set clipping to a region. */
void SetClipSect(register RgnHandle rgn); /* Further limit commands... */
void SetClipUnion(register RgnHandle rgn); /* Un-limit commands... */
void SetClipDiff(register RgnHandle rgn);
void SetClipXor(register RgnHandle rgn);
RgnHandle LocalGetClip();	/* Get (local) clipping region. */
void BackPat(register Pattern *bgpattern);	/* Set background pattern */
void InitCursor(void);			/* Initialize mouse cursor */
#ifndef NT
void SetCursor(register Cursor *);	/* Define mouse cursor mask and data */
#endif
void HideCursor(void);			/* Make mouse cursor invisible */
#ifndef NT
void ShowCursor(void);			/* Make mouse cursor visible */
#endif
void ObscureCursor(void);
void ShieldCursor(register Rect *shieldRect, register Point *offsetPt);
void PlotIcon(register Rect *theRect, register Icon *theIcon);	/* Plot an iconwithin a rectangle. */
void ScreenRes(int *scrnHRes, int *srcnVRes);	/* Read screen resolution */
void GetPen(Point *pt);			/* Get drawing pen position */
void GetPenState(PenState *pnState);	/* Get drawing pen state variables */
void SetPenState(PenState *pnState);	/* Set drawing pen state variables */
void PenSize(register short width, register short height); /* Set drawing pen size */
void PenMode(register int mode);	/* Set drawing pen transfer mode */
void PenPat(register Pattern *penpattern);	/* Set drawing pen draw pattern */
void PenNormal(void);			/* Restore drawing pen to normal size, mode and pattern */
void MoveTo(register short horz, register short vert); /* Move drawing position absolute */
void Move(register short dh, register short dv); /* Move drawing poition relative */
#ifndef NT
void LineTo(register short horz, register short vert); /* Draw a line from previous drawing position to spec. position */
#endif
void Line(register short dh, register short dv); /* Same as above but relative */

#ifndef NT
void SetPixel(register short horz, register short vert); /* Draw one dot in spec. position */
#endif
void TextFace(register int styles);	/* Select styles for text drawing. */
void TextFont(register int font);	/* Select textfont to use in Char, Text and String commands */
void TextSize(register int size);	/* Select font size " */
void TextMode(register int mode);	/* Select text transfer mode. */
void TextRotate(register int degreesCCW); /* Select text rotation angle. */
void SpaceExtra(register Fixed extra);	/* Select extra spacing between ASCII space characters. */
void TextExtra(register Fixed extra);	/* Select extra spacing between characters. */
void DrawChar(char ch); /* Draw one character at current position using selected font */
void DrawString(register char *string);	/* Draw a null terminated string at current position using selected font */
#ifndef NT
void DrawText(register char *textBuf, register int firstByte, register int byteCount);	/* Draw a number of characters from an array of text */
#endif
void DrawText16(register unsigned short *textBuf, register int firstShort, register int shortCount); /* Draw characters using 16 bit character codes */
void DrawJustText(char *textBuf, short byteCount, Rect *justrect,
		  short hjustflag, short vjustflag); /* Justified text. */
int CharWidth(char ch);			/* Calculate width of a characters in pixels */
int StringWidth(register char *string);	/* Calculate width of a string in pixels */
int TextWidth(register char *textBuf, register int firstByte, register int byteCount);	/* Calculate width of a number of characters in an array */
int TextWidth16(register unsigned short *textBuf, register int firstShort, register int shortCount);	/* Calculate width of a number of 16 bit characters in an array */
long TextWidths(register char *textBuf, register short startOff, register short endOff);
int LocalCharWidth(int font, int face, int size, char chr);
int LocalStringWidth(int font, int face, int size, char *string);
int LocalTextWidth(int font, int face, int size, char *string, int len);
void GetFontInfo(FontInfo *info);	/* Get information about selected font */
void ForeColor(register short color);	/* Set Foreground color number. */
void BackColor(register short color);	/* Set Background color number. */
void ColorBit(register short colormask);	/* Set Color plane mask??? */
void SetRGBColor(unsigned short index, unsigned short red,
		 unsigned short green, unsigned short blue); /* 0-$FFFF */
void LocalGetFontInfo(int font, int face, int size, FontInfo *info);
int LocalFontIndex(int font, int face, int size);
void LocalTextBounds(int font, int face, int size, char *string, int len,
		     short horz, short vert, Rect *rectangle);
void LocalFlush(void);		/* Delete all locally-cached font tables. */
#ifndef NT
void SetRect(Rect *rect, short left, short top, short right, short bottom);	/* Load data into a rectangle structure */
#endif
#ifndef NT
void OffsetRect(Rect *rect, short dh, short dv);	/* Offset rectangle coordinates */
#endif
void InsetRect(Rect *rect, short dh, short dv);		/* Shrink the coordinates of a rectangle structure */
int SectRect(register Rect *src1, register Rect *src2, Rect *dstRect);	/* Calculate the section between to rectangles */
#ifndef NT
void UnionRect(register Rect *src1, register Rect *src2, Rect *dstRect); /* Calculate the union between two rectangles */
#endif
#ifndef NT
int PtInRect(register Point *pt, register Rect *rect); /* Check if a point is inside a rectangle */
#endif
void Pt2Rect(Point *, Point *, Rect *);		/* Load a rectangle structure from two point structures */
void PtToAngle(register Rect *rect, register Point *point, short *angle);
#ifndef NT
int EqualRect(register Rect *rect1, register Rect *rect2);	/* Check if two rectangles are equal */
#endif
int EmptyRect(register Rect *rect);	/* Check if a rectangle structure describes a rectangle with zero size */
long PinRect(register Rect *theRect, register Point *thePt); /* Clip a point within the rectangle. */
#ifndef NT
void FrameRect(register Rect *rect);	/* Put a frame around the inside of a rectangle */
#endif
void PaintRect(register Rect *rect);	/* Paint a rectangle with selected pen pattern */
void EraseRect(register Rect *rect);	/* Erase a rectangle to selected background pattern */
#ifndef NT
void InvertRect(register Rect *rect);	/* Invert all pixels inside a rectangle */
void FillRect(register Rect *rect, register Pattern *pat);	/* Paint a rectangle with spec. pattern */
#endif
void FrameOval(register Rect *rect);	/* Put a frame around the inside of an oval */
void PaintOval(register Rect *rect);	/* Paint an oval with selected pen pattern */
void EraseOval(register Rect *rect);	/* Erase an oval. */
void InvertOval(register Rect *rect);	/* Invert an oval. */
void FillOval(register Rect *rect, register Pattern *pat); /* Paint an oval with the spec. pattern. */
void FrameRoundRect(register Rect *rect, register short ovalWidth, register short ovalHeight);
void PaintRoundRect(register Rect *rect, register short ovalWidth, register short ovalHeight);
void EraseRoundRect(register Rect *rect, register short ovalWidth, register short ovalHeight);
void InvertRoundRect(register Rect *rect, register short ovalWidth, register short ovalHeight);
void FillRoundRect(register Rect *rect, register short ovalWidth, register short ovalHeight, register Pattern *pat);
void FrameArc(register Rect *rect, register short startAngle, register short arcAngle);
void FrameWedge(register Rect *rect, register short startAngle, register short arcAngle);
void PaintArc(register Rect *rect, register short startAngle, register short arcAngle);
void EraseArc(register Rect *rect, register short startAngle, register short arcAngle);
void InvertArc(register Rect *rect, register short startAngle, register short arcAngle);
void FillArc(register Rect *rect, register short startAngle, register short arcAngle, register Pattern *fillpat);
#define DisposeRgn(x) lwfree(x)	/* Temporary HACK! */
#ifndef NT
void OffsetRgn(RgnHandle region, short dh, short dv);
#endif
int PtInRgn(Point *point, RgnHandle region);
int RectInRgn(Rect *rect, RgnHandle region);
#ifndef NT
void FrameRgn(register RgnHandle rgn);
#endif
#ifndef NT
void PaintRgn(register RgnHandle rgn);
#endif
void EraseRgn(register RgnHandle rgn);
#ifndef NT
void InvertRgn(register RgnHandle rgn);
#endif
#ifndef NT
void FillRgn(register RgnHandle rgn, register Pattern *pat);
#endif
void ScrollRect(register Rect *rect, register short dh, register short dv, int requestUpdate);	/* Scroll the area inside a rectangle */
void CopyBits(register BitMap *srcBits, BitMap *dstBits, register Rect *srcRect, Rect *dstRect, int mode, int dummy);	/* Copy a binary image from memory to screen */
PicHandle OpenPicture(Rect *frame); /* Start collecting a picture. */
void ClosePicture(void);	/* Stop collecting a picture */
void PicComment(short kind, short dataSize, void *data);
void DrawPicture(PicHandle pict, Rect *dstRect); /* Draw a picture. */
void KillPicture(PicHandle pict); /* Dispose of a picture. */
void StdComment(short kind, short dataSize, void *data);
#ifndef NT
PolyHandle OpenPoly(void);	/* Start collecting a polygon. */
void ClosePoly(void);		/* Stop collecting a polygon. */
PolyHandle DupePoly(PolyHandle poly);	/* Duplicate a polygon. */
void KillPoly(PolyHandle poly);	/* Dispose of a polygon. */
void OffsetPoly(register PolyHandle poly, register short deltah, register short deltav);
void FramePoly(register PolyHandle poly); /* Put a frame around the inside of a polygon. */
void PaintPoly(register PolyHandle poly); /* Paint a polygon with selected pen pattern. */
void ErasePoly(register PolyHandle poly); /* Erase a polygon. */
void InvertPoly(register PolyHandle poly); /* Invert a polygon. */
void FillPoly(register PolyHandle poly, register Pattern *pat); /* Paint a polygon with the spec. pattern. */
#endif
void AddPt(Point *srcPt, Point *dstPt);	/* Add the coordinates of one point into another point */
void SubPt(Point *srcPt, Point *dstPt);	/* Subtract the coordinates of one point into another point */
void SetPt(Point *dstPt, int horz, int vert);	/* Load a point structure */
int EqualPt(Point *pt1, Point *pt2);	/* Check if two point structures describe the same point */

void Bell(void);		/* Beep the bell in the terminal */
void SetApplCursor(register short mode, register Cursor *cursor);	/* Define the application cursor */
void MoveApplCursor(register short x, register short y); /* Position the application cursor */
void HideApplCursor(void);	/* Make the application cursor invisible */
void ShowApplCursor(void);	/* Make the application cursor visible */
void MoveCursor(register short x, register short y);
void HiliteRect(register Rect *rect, register Pattern *pat, register int flag);	/* Hilite a rectangle area using color */
void HiliteClear(void);		/* Clear the hilite flags for a window */
void GetHdwrType(register HdwrType *hwtyperec);
void GetWManVrsns(register WmanVrsns *versions);
void SetKbdLEDs(register short LEDbitmask);	/* Bit 0 Scroll, Bit 1 NumLK */
void SetKbdXlate(register short translatemode);	/* 0 normal, 1 Int'l xlate. */
long TextWidHit(register char *string, register short byteCount, register short hoffset);
long LocalTextWidHit(int font, int face, int size, char *string, short byteCount, short hoffset);
Fixed SlopeFromAngle(register short angle);
short AngleFromSlope(register Fixed slope);
int LocalIsNew(int font, int face, int size);	/* TRUE if is a new combo. */

/* Menu functions */

void NewMenu(register short menuID, register char *menutitle); /* Create a new Menu */
void DisposeMenu(register int menuID);	/* Dispose of a Menu. */
void DisableMenu(register int menuID);	/* Disable a whole Menu. */
void EnableMenu(register int menuID);	/* Enable a whole Menu. */
#ifndef NT
void AppendMenu(register short menuID, register char *menuitems); /* Append items to a Menu. */
void DeleteMenu(register short menuID);	/* Delete all items in a Menu. */
#endif
void BeginMenu(register long menumessage); /* Beginning of Menu event processing. */
void EndMenu(register long menumessage); /* End of Menu event processing. */
void SetItem(register short menuID, register short item, register char *menuitem); /* Change an item in a Menu */
void SetItemMark(int menuID, int item, short markchar);	/* Change an item mark in a menu. */
void SetItemStyle(int menuID, int item, short style);	/* Change an item's style in a menu. */
void SetItemKey(int menuID, int item, long key);	/* Change an item's keyboard equivalent in a menu. */
void DisableItem(register short menuID, register short item); /* Disable a menu Item */
void EnableItem(register short menuID, register short item); /* Enable a menu Item */
void NoMenuKeyXlate(void);	/* Disable menu key equivalents temporarily. */
int GetMenuFlash(void);		/* Get number of menu flashes setting. */
int GetDblClickTime(void);	/* Get mouse double-click time setting. */
int LocalGetDblClickTime(void);	/* As above, but buffered. */

FontList *GetFontList(int *nfonts);
int GetFontSizes(int font, unsigned char *list, int listsize);
void MouseMode(long mode);

void ZeroScrap(void);		/* Erase the clipboard. */
void PutScrap(unsigned long length, unsigned long type, void *scrap); /* Put item in clip. */
void PublishScrap(register int flag); /* 0 == announce new clipboard, 1 == request clipboard. */
unsigned long GetScrap(void *scrap, unsigned long type, unsigned long maxsize); /* Get item from clip. */

#define CheckItem(menuID, item, checked) \
SetItemMark(menuID, item, (checked) ? 036 : ' ')

void SoundBite(char *name);

/* Miscellaneous routines. */

void PackBits(void **srcPtr, void **dstPtr, short srcBytes);
void PackBits2(void **srcPtr, void **dstPtr, long srcBytes);
void UnpackBits(void **srcPtr, void **dstPtr, short dstBytes);
void UnpackBits2(void **srcPtr, void **dstPtr, long srcBytes);
long StringLen(register char *string);
void BlockMove(void *source, void *dest, long nbytes);
long FixRound(register Fixed number);
Fixed Int2Fix(register int number);
short Fix2Int(register Fixed number);
unsigned short Fix2Frac(register Fixed number);
unsigned long HiWord(register long number);
unsigned long LoWord(register long number);
Fixed FixRatio(register short numerator, register short denominator);
Fixed FixMul(register Fixed a, register Fixed b);
long MulS(register short multiplier, register short multiplicand);
unsigned long MulU(register unsigned short multiplier, register unsigned short multiplicand);
short DivS(register long divisor, register short dividend);
unsigned short DivU(register unsigned long divisor, register unsigned short dividend);
long DivModS(register long divisor, register short dividend);
unsigned long DivModU(register unsigned long divisor, register unsigned short dividend);
short ModS(register long divisor, register short dividend);
unsigned short ModU(register unsigned long divisor, register unsigned short dividend);
short ScaleS(register short a, register short b, short c);	/* A * B / C */
unsigned short ScaleU(register unsigned short a, register unsigned short b, unsigned short c);
short ScaleRS(register short a, register short b, short c); /* Rounded A*B/C */
unsigned short ScaleRU(register unsigned short a, register unsigned short b, unsigned short c);
void ScalePt(register Point *p, register Rect *src, register Rect *dest);
void MapPt(register Point *p, register Rect *src, register Rect *dest);
void MapRect(register Rect *r, register Rect *src, register Rect *dest);
void MapRgn(register RgnHandle rh, register Rect *src, register Rect *dest);
#ifndef NT
void MapPoly(register PolyHandle ph, register Rect *src, register Rect *dest);
#endif
unsigned short Sqrt(register unsigned long value); /* Integer square root. */
unsigned short VectorLen(register unsigned short xlen, register unsigned short ylen); /* Vector Length. */
void *FastFind(register void *buffer, register void *target, register unsigned long bufsize, register unsigned long targetsize); /* Fast string (or whatever) search. */
void *FastCIFind(register void *buffer, register void *target, register unsigned long bufsize, register unsigned long targetsize); /* Case-insensitive version of above.  Slower. */
int RotateBMapCCW(BitMap *srcBMap, BitMap *dstBMap, void *(*allocfn)(unsigned long size));
int RotateBMapCW(BitMap *srcBMap, BitMap *dstBMap, void *(*allocfn)(unsigned long size));
RgnHandle BitMapToRgn(BitMap *srcBMap);
void SetMallocNFree(register void *(*newmalloc)(), register void (*newfree)()); /* Replace libwindow's normal malloc and free functions. */

/* For the following, the pointer should point to a structure of the form:
** struct {
**     long size;		/ * Size of this structure (in bytes). * /
**     short ncolors;		/ * Number of entries following. * /
**     struct {
**	   unsigned short index, red, green, blue;
**     } colors[NCOLORS];
** };
*/
int ColorPicker(void *p, int selected);	/* Returns when done. */
int NWColorPicker(void *p, int selected, int trapqfd); /* Returns PID. */

int FilePicker(SFDialog *p);	/* Returns when done. */
int NWFilePicker(SFDialog *p, int trapqfd); /* Returns PID. */

void ParsePicture(PicHandle ph);

/* Routines (of yours) called by ParsePicture: */
   void StdClipRect(int verb, Rect *rp);
   void StdBits(BitMap *sbs, int dum, Rect *sr, Rect *dr, int mode, int dum2);
   void StdLine(Point *from, Point *to, GrafState *gs);
   void StdText(char *text, int count, GrafState *gs);
   void StdRGBColor(int index, int red, int green, int blue);
   void StdRect(int verb, Rect *rect, GrafState *gs);
   void StdOval(int verb, Rect *rect, GrafState *gs);
   void StdRRect(int verb, Rect *rect, int hrad, int vrad, GrafState *gs);
   void StdArc(int verb, Rect *rect, int strtAng, int arcAng, GrafState *gs);
   void StdWedge(int verb, Rect *rect, int strtAng, int arcAng, GrafState *gs);
#ifndef NT
   void StdPoly(int verb, PolyHandle ph, GrafState *gs);
#endif
   void StdRgn(int verb, RgnHandle rh, GrafState *gs);
   void StdSetClip(int verb, RgnHandle rh);
   void StdText16(short *text, int count, GrafState *gs);
   void StdJustText(char *t, int cnt, Rect *, int hflg, int vflg, GrafState *);

#else
#include <isc/windowlint.h>
#endif	/* lint */

/*
** Variables for local reference.  These try to echo the state of
** the current drawing port.
*/

extern short theFont;		/* Last font set with TextFont. */
extern short theSize;		/* Last font size set with TextSize. */
extern short theFace;		/* Last style set with TextFace. */
extern short theTextMode;	/* Last text mode set with TextMode. */
extern short theTextRotate;	/* Last text rot. angle set with TextRotate. */
extern short thePenMode;	/* Last pen mode set with PenMode. */
extern long theMouseMode;	/* Last mouse mode set with MouseMode. */
extern short theForeColor;	/* Last color set with ForeColor. */
extern short theBackColor;	/* Last color set with BackColor. */
extern RgnHandle theClipper;	/* Last ClipRect/SetClip item. */

/*eject*/
/* Some routines implemented as macros */

#ifndef OLD_RECTS		/* Rectangle variants. */

#define SetRect(rect, l, t, r, b) \
{ \
	register Rect *_rp; \
	_rp = (rect); \
	_rp->edge.top = (t); \
	_rp->edge.left = (l); \
	_rp->edge.bottom = (b); \
	_rp->edge.right = (r); \
}

#ifndef NT
#define OffsetRect(rect, dh, dv) \
{ \
	register Rect *_rp; \
	register short _xoffs, _yoffs; \
	_rp = (rect); \
	_xoffs = (dh); \
	_yoffs = (dv); \
	_rp->edge.top += _yoffs; \
	_rp->edge.left += _xoffs; \
	_rp->edge.bottom += _yoffs; \
	_rp->edge.right += _xoffs; \
}
#endif

#define InsetRect(rect, dh, dv) \
{ \
	register Rect *_rp; \
	register short _xoffs, _yoffs; \
	_rp = (rect); \
	_xoffs = (dh); \
	_yoffs = (dv); \
	_rp->edge.top += _yoffs; \
	_rp->edge.left += _xoffs; \
	_rp->edge.bottom -= _yoffs; \
	_rp->edge.right -= _xoffs; \
}

#define Pt2Rect(pt1, pt2, rect) \
{ \
	register Rect *_rp; \
	register Point *_p1, *_p2; \
	_rp = (rect); \
	_p1 = (pt1); _p2 = (pt2); \
	_rp->edge.top = (_p1->v < _p2->v) ? _p1->v : _p2->v; \
	_rp->edge.left = (_p1->h < _p2->h) ? _p1->h : _p2->h; \
	_rp->edge.bottom = (_p1->v > _p2->v) ? _p1->v : _p2->v; \
	_rp->edge.right = (_p1->h > _p2->h) ? _p1->h : _p2->h; \
}

#else				/* Rectangle variants. */

#define SetRect(rect, l, t, r, b) \
{ \
	register Rect *_rp; \
	_rp = (rect); \
	_rp->top = (t); \
	_rp->left = (l); \
	_rp->bottom = (b); \
	_rp->right = (r); \
}

#define OffsetRect(rect, dh, dv) \
{ \
	register Rect *_rp; \
	register short _xoffs, _yoffs; \
	_rp = (rect); \
	_xoffs = (dh); \
	_yoffs = (dv); \
	_rp->top += _yoffs; \
	_rp->left += _xoffs; \
	_rp->bottom += _yoffs; \
	_rp->right += _xoffs; \
}

#define InsetRect(rect, dh, dv) \
{ \
	register Rect *_rp; \
	register short _xoffs, _yoffs; \
	_rp = (rect); \
	_xoffs = (dh); \
	_yoffs = (dv); \
	_rp->top += _yoffs; \
	_rp->left += _xoffs; \
	_rp->bottom -= _yoffs; \
	_rp->right -= _xoffs; \
}

#define Pt2Rect(pt1, pt2, rect) \
{ \
	register Rect *_rp; \
	register Point *_p1, *_p2; \
	_rp = (rect); \
	_p1 = (pt1); _p2 = (pt2); \
	_rp->top = (_p1->v < _p2->v) ? _p1->v : _p2->v; \
	_rp->left = (_p1->h < _p2->h) ? _p1->h : _p2->h; \
	_rp->bottom = (_p1->v > _p2->v) ? _p1->v : _p2->v; \
	_rp->right = (_p1->h > _p2->h) ? _p1->h : _p2->h; \
}

#endif				/* Rectangle variants. */

#define SetPt(dstPt, horz, vert) \
{ \
	register Point *_pt; \
	_pt = (dstPt); \
	_pt->v = (vert); \
	_pt->h = (horz); \
}

#define AddPt(srcPt, dstPt) \
{ \
	register Point *_spt, *_dpt; \
	_spt = (srcPt); \
	_dpt = (dstPt); \
	_dpt->v += _spt->v; \
	_dpt->h += _spt->h; \
}

#define SubPt(srcPt, dstPt) \
{ \
	register Point *_spt, *_dpt; \
	_spt = (srcPt); \
	_dpt = (dstPt); \
	_dpt->v -= _spt->v; \
	_dpt->h -= _spt->h; \
}

#define EqualPt(pt1, pt2) (*(long *)(pt1) == *(long *)(pt2))

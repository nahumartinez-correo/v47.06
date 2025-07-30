/**********************************************************************
*	MODULE:		crtkey.h
*
*	PURPOSE:
*		This is the include file that has all the int values that will
*		be returned as keycodes from the wp5 or vt200 window manager.
*		This was designed to give back ints in the range 0 - 0xFFFF
*		for regular printable keys. Function keys are indicated by
*		one of the flag bits in the high order word of the long word.
*		The values should not be changed in here but rather the
*		function they perform in the particular application could
*		change.
*
*	NOTES:
*
*
*   Copyright (c) 1988 ISC Systems Corporation
***********************************************************************/

/* Keycode flag definitions. */

#define F_KEYMASK	0x0000FFFF /* Mask to get keycode. */
#define F_FLAGMASK	0xFFFF0000 /* Mask to get key flags. */

#define F_FUNC		0x00010000 /* Key is a function key. */
#define F_SHIFT		0x00020000 /* Shift key is pressed with key. */
#define F_CTRL		0x00040000 /* Control key is pressed with key. */
#define F_ALT		0x00080000 /* Alt key is pressed with key. */

#define F_MOUSE1	0x00100000 /* Flags indicating the state of */
#define F_MOUSE2	0x00200000 /* the first 4 mouse buttons. */
#define F_MOUSE3	0x00400000
#define F_MOUSE4	0x00800000

#define F_KEYDOWN	0x01000000 /* Key down flag. */
#define F_KEYUP		0x02000000 /* Key up flag. */

#define	F_KBDIII	0x08000000 /* This keycode from AT / KB-III. */

/* Keycode definitions. */
/* NOTE:  All of the following codes will have F_FUNC set on receipt! */

#define	K_BCKSP2	0x08	/* KBD-II backspace key. */
#define K_TAB		0x09	/* Tab */
#define K_ENTER		0x0D	/* Enter */
#define K_ESC		0x1B	/* Esc */

#define K_HOME		0x40	/* Cursor home */

#define K_UP		0x41	/* Cursor control keys... */
#define K_DOWN		0x42
#define K_RIGHT		0x43
#define K_LEFT		0x44

#define K_PF1		0x45
#define K_PF2		0x46
#define K_PF3		0x47
#define K_PF4		0x48	/* Labeled "Local Print" on kbd-II. */

#define K_INSERT	0x51	/* Not avail. on kbd-II */ 
#define K_DELETE	0x52	/* Not avail. on kbd-II */
#define K_END		0x53	/* Not avail. on kbd-II */ 
#define K_PGUP		0x54	/* Not avail. on kbd-II */
#define K_PGDN		0x55	/* Not avail. on kbd-II */

#define K_PRNTSCRN	0x56	/* kbd-III only. */ 

#define K_RESET		0x57	/* kbd-II only */
#define K_SEND		0x58	/* kbd-II only */
#define K_CLRFLD	0x59	/* kbd-II only */

#define K_BREAK		0x5F	/* Break key, used by WindowMan. */

#define K_NPAD0		'0'	/* Numeric Pad Keys... */
#define K_NPAD1		'1'
#define K_NPAD2		'2'
#define K_NPAD3		'3'
#define K_NPAD4		'4'
#define K_NPAD5		'5'
#define K_NPAD6		'6'
#define K_NPAD7		'7'
#define K_NPAD8		'8'
#define K_NPAD9		'9'
#define K_NPLUS		'+'
#define K_NCOMMA	','
#define	K_NMINUS	'-'
#define K_NDOT		'.'
#define K_NPAD00	0x5E	/* Double zero (only on kbd-II). */


#define K_F1		0x61	/* Function Keys... */
#define K_F2		0x62
#define K_F3		0x63
#define K_F4		0x64
#define K_F5		0x65
#define K_F6		0x66
#define K_F7		0x67
#define K_F8		0x68
#define K_F9		0x69
#define K_F10		0x6A
#define K_F11		0x6B
#define K_F12		0x6C
#define K_F13           0x6D	/* kbd-II/Custom Cherry kbd-III only. */
#define K_F14           0x6E	/* kbd-II/Custom Cherry kbd-III only. */

#define K_F15           0x6F    /* Custom Cherry kbd-III only. */
#define K_F16           0x70    /* Custom Cherry kbd-III only. */
#define K_F17           0x71    /* Custom Cherry kbd-III only. */

#define K_F18           0x72	/* vt220/Custom Cherry kbd-III only. */
#define K_F19           0x73	/* vt220/Custom Cherry kbd-III only. */
#define K_F20           0x74	/* vt220/Custom Cherry kbd-III only. */

#define K_F21           0x75    /* Custom Cherry kbd-III only. */
#define K_F22           0x76    /* Custom Cherry kbd-III only. */
#define K_F23           0x77    /* Custom Cherry kbd-III only. */
#define K_F24           0x78    /* Custom Cherry kbd-III only. */

#define	K_SCROLL	0x7D
#define	K_NPENTER	0x7E
#define	K_BACKSP	0x7F

#define	SETKEYRAW	('k'<<8|0x01)
#define	KEYRAW		2	/* Flag for RAW device open in /dev/keyboard */

/* mouse / keyboard I/F stuff */

#define INITCURSOR	(('p'<<8)|200)
#define SETCURSOR	(('p'<<8)|201)
#define MOVECURSOR	(('p'<<8)|202)
#define COUPLECURSOR	(('p'<<8)|203)
#define UNCOUPLECURSOR	(('p'<<8)|204)
#define	RETURNTICKS	(('p'<<8)|205)
#define	GETKBDLEDS (('p'<<8)|206)
#define	SETKBDLEDS (('p'<<8)|207)
#define	GETVIDEOPARAMS (('p'<<8)|208)
#define	SETVIDEOPARAMS (('p'<<8)|209)
#define	SOUNDBELL (('p'<<8)|210)
#define	GETPALETTEDATA (('p'<<8)|212)
#define	SETPALETTEDATA (('p'<<8)|213)
#define	GETMOUSETABLE (('p'<<8)|214)
#define	SETMOUSETABLE (('p'<<8)|215)
#define	UNSETKBDMOUSEMODE (('p'<<8)|216)
#define	SETKBDMOUSEMODE (('p'<<8)|217)
#define	GETKBDMOUSEMODE (('p'<<8)|218)
#define GETTRANSFORM (('p'<<8)|219)
#define SETTRANSFORM (('p'<<8)|220)

struct ts_transform {		/* Transformation matrix for touch screen. */
        long tformA[2][2];	/* Actually of type 'Fixed'! */
        long tformB[2];
};

struct event {
	long	key;
	long	time;
	short	y;
	short	x;
	short	yabs;
	short	xabs;
};


struct	videoparams {
    short size;			/* size of structure */
    short revision;		/* version number of structure */
    unsigned char vidtype;	/* video type mono/colour/enhanced colour. */
    unsigned char nplanes;	/* number of active video planes for colour */
    unsigned char hilevel;	/* screen brightness when not dimmed */
    unsigned char lolevel;	/* screen brightness when dimmed */
    long dimtime;		/* number of seconds before dimming */
    unsigned char palettemode;	/* colour palette status 0=flash 1/2=fixed palette number */
    unsigned char volume;	/* volume control for bell */
    unsigned char bellstages;
    unsigned char bellrepeat;
    short bellfrequency;
    unsigned char belldata[32];	/* bell sound control table */
};


/* definitions for vidtype */

#define	MONOCHROME	0			/* monochrome board */
#define	COLOR		1			/* wp5 - colour board */

struct	palette_data {
    short palettenumber;
    unsigned char reds[16];
    unsigned char greens[16];
    unsigned char blues[16];
};

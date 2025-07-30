/* ----------------------------------------------------------- */
/*                  Box object definitions                     */
/* ----------------------------------------------------------- */
#ifndef TDSPL_H
#define TDSPL_H

#define DBOXNOBORD		0	/* Borderless box */
#define DBOXBORD1		1	/* Single line border */
#define DBOXBORD2		2	/* Double line border */
#define DBOXBORD3		3	/* Triple line border */
#define DBOXBORDDS		4	/* Dropshadow border */ 
#define DBOXUNDERLINE	5	/* draw underline only box */
#define DBOXRELIEF		6	/* relief box */
#define	DBOXCHISEL		7	/* sculptured box, outline	*/
#define	DBOX3DS			8	/* sculptured box, sunken	*/
#define	DBOX3DR			9	/* sculptured box, raised	*/
#define	DBOXRELIEFS1	10	/* 'a' single wide sunken relief box	*/
#define	DBOXRELIEFS2	11	/* 'b' double wide sunken relief box	*/
#define	DBOXRELIEFS3	12	/* 'c' triple wide sunken relief box	*/
#define	DBOXRELIEFR1	13	/* 'A' single wide raised relief box	*/
#define	DBOXRELIEFR2	14	/* 'B' double wide raised relief box	*/
#define	DBOXRELIEFR3	15	/* 'C' triple wide raised relief box	*/
#define	DBOXMAX			15	/* highest valid box type			*/



/* ----------------------------------------------------------- */
/*       Hilite Relative Rectangle object definitions          */
/* ----------------------------------------------------------- */

typedef struct DHRECT {
				long	left,top,bottom,right;	/* *RELATIVE* to x,y ! */
			} dhrect;

/* ----------------------------------------------------------- */
/*             Text object attribute definitions               */
/*    (same as RT_XXX defines in /usr/lib/pplus/pcldefs.h)     */
/* ----------------------------------------------------------- */

#define DTINVISIBLE		0x0001	/* non-displayed */
#define DTBOLD			0x0002	/* Bold */
#define DTUNDERLINE		0x0004	/* Underline */
#define DTITALICS		0x0008	/* Italics */
#define DTGHOST			0x0010	/* Ghosted */
#define DTOUTLINE		0x0020	/* Outlined */
#define DTERASE			0x0040	/* Erased */
#define DTREVERSE		0x0080	/* Reverse */
#define DTTRANSPARENT	0x0100	/* Transparent background mode */

/* ----------------------------------------------------------- */
/*              Box object attribute definitions               */
/* ----------------------------------------------------------- */

#define DTCOLOR_0		0x0001	/* color-pair zero */
#define DTCOLOR_1		0x0002	/* color-pair one */
#define DTCOLOR_2		0x0004	/* color-pair two */
#define DTCOLOR_3		0x0008	/* color-pair three */

/*
** Word relative Attribute Definitions
*/
#define DWBOLD			0x0002	/* Bold */
#define DWUNDERLINE		0x0004	/* Underline */
#define DWITALICS		0x0008	/* Italics */
#define DWGHOST			0x0010	/* Ghosted */
#define DWOUTLINE		0x0020	/* Outlined */
#define DWERASE			0x0040	/* Erased */
#define DWREVERSE		0x0080	/* Reverse */

/*
* icon types
*/
#define	WMANICON	0
#define	WINDOWSICON	1

/* ----------------------------------------------------------- */
/*              External function definitions                  */
/* ----------------------------------------------------------- */

	//XDLL void	drawstring(dlist *dlptr);
	//XDLL void	drawpicture(dlist *dlptr);
	//XDLL void	hilite(dlist *dlptr);
	//XDLL void InsetRect(RECT *rect, int h, int v);
	void	drawstring(dlist *dlptr);
	void	drawpicture(dlist *dlptr);
	void	hilite(dlist *dlptr);
	void	InsetRect(RECT *rect, int h, int v);

	void	TDsetcolor(int fore, int back);
	void	setboxrect(RECT *boxrect, int type, int x, int y,
						int width, int height);
	void	unsetboxrect(RECT *boxrect,int type,int x,int y,
						int width,int height);
	void	drawbox(dlist *dlptr);
	void	erasebox(dlist *dlptr);
	void	dl_get_hwinfo(void);
	void	drawline(dlist *dlptr);
	void	eraseline(dlist *dlptr);
	void	dodrawstr(dlist *dlptr, char *str, int strtype);
	void	drawtext(dlist *dlptr);
	void	erasestring(dlist *dlptr);
	void	erasetext(dlist *dlptr);
	int		clear_colorbox(RECT *box);
	void	set_colorbox(RECT *box, short a);
	void	check_color(RECT *boxrect, short a);
	void	drawicon(dlist *dlptr);
	void	eraseicon(dlist *dlptr);
	void	drawcolorpic(char *name, int width, int height, dlist *dlp,
						HANDLE);
	XDLL HANDLE	GetBMPSizeAndHandle (char* , int*,int*);
	BOOL GetBMPSize (char*, int*, int*);
	XDLL WORD PaletteSize(VOID FAR * pv);
	XDLL HANDLE DibFromBitmap(HBITMAP hbm, DWORD biStyle, WORD biBits,
			HPALETTE hpal);
	void	erasepicture(dlist *dlptr);
	void	set_look(register font, register face, register size,
					register forecolor, register backcolor);
#ifdef SIXTEENBITCHARS
	void	set_intllook(register otherfont, register othersize);
#endif /* SIXTEENBITCHARS */
	long	control_string(HDC hdc, short left, short top,
					char *caption, short len, short align, short draw);
	void	dl_drawreliefborder(register short x, register short y,
					register short boxwidth, register short boxheight, 
					register short topleft, register short botright,
					register short drawwidth);
	void dl_drawradiobutton(dlist *dlptr);
	void dl_drawcheckbox(dlist *dlptr);
	void calc_dlbounds(RECT	*r, dlist *dlptr);
	int getstretchmode(COLORREF color);
	void TDsetcolor(int fore,int back);

/* ----------------------------------------------------------- */
/*              External data definitions                      */
/* ----------------------------------------------------------- */
	extern TEXTMETRIC Tm;
	extern short DL_inupdate;	/* True when inside an update event */
	extern short Fullcolor;
#if 0
	extern HdwrType DL_hwinfo;	/* global hardware information from WM */
#endif

#endif

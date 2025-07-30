#ifndef TEXTEDIT_H
#define TEXTEDIT_H

/*
** Equates for the Text Edit package.
*/

/* Constants for use for TE alignment (carried over from windowman)	*/
#define teJustLeft 0		/* Left-justified (the default).   */
#define teJustCenter 1		/*        Center-justified         */
#define teJustRight (-1)	/*                 Right-justified */

/* flags used by TENew()	*/
#define	TEFLAG_JUSTMASK		0x03
#define	TEFLAG_LEFT			0x00
#define	TEFLAG_CENTER		0x01
#define	TEFLAG_RIGHT		0x02
#define	TEFLAG_WRAPHORIZ	0x04
#define	TEFLAG_IGNORERETURN	0x08
#define	TEFLAG_VSCROLLBAR	0x10
#define	TEFLAG_HSCROLLBAR	0x20

typedef struct TERec {
	HWND	hTEWnd;		/* window handle for edit control		*/
	RECT	destRect;	/* Rectangle text is processed in.		*/
	slist	*slp;		/* screen list that this TE belongs to	*/
	field	*fp;		/* the field attached to this TE		*/
	short	txFont;		/* Which font to use for text.			*/
	short	txFace;		/* Which font style to use for text.	*/
	short	txSize;		/* Which size font to use for text.		*/
	short	forecolor;	/* TE's forecolor						*/
	short	backcolor;	/* TE's backcolor						*/
	short	active;		/* TRUE if highlighted (active).		*/
} TERec;

/***********************************************/
/****** Text Edit's available functions. *******/
/***********************************************/

TERec	*TENew(RECT *destRect, byte teflags, field *fp);
void	TEDispose(TERec *pTE);
void	TESetText(char *text, short maxlength, TERec *pTE);
short	TEGetText(char *text, TERec *pTE);
void	TEClick(POINT *pt, short extend, TERec *pTE);
void	TESetSelect(short selStart, short selEnd, TERec *pTE);
long	TEGetSelect(TERec *pTE);
void	TEActivate(TERec *pTE);
void	TEDeactivate(TERec *pTE);
int		TEActivated(TERec *pTE);
void	TEKey(long key, TERec *pTE);
void	TECut(TERec *pTE);
void	TECopy(TERec *pTE);
void	TEPaste(TERec *pTE);
void	TEDelete(TERec *pTE);
void	TEUpdate(RECT *rUpdate, TERec *pTE);
void	TESelView(TERec *pTE);
void	TESpillGuts(TERec *pTE);
void	TextBox(char *str, short length, RECT *box, short just);
int 	GetStretchFieldInfo( TEXTMETRIC * finfo, short * ks, int * propadd, int len );
short	TEVisLine(char *buf, short maxlen, short lineno, TERec *pTE);
void	TEButtonUp(TERec *pTE, short x, short y);
HBRUSH	TESetColor(HDC hdc, HWND hTEWnd);
bool	TEPasteSetup(short teflags, field *fp);
char	*TEGetPasteString(void);
void	TEChange(dlist *dlp);
BOOL	TECopytoClip(TERec *pTE, char *str);

#endif

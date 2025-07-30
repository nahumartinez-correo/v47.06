/* Prototypes for the Font Functions */

#define	FIRSTWINFONT	100
#define MAXFONT			255
#define	WINFONTCOUNT	(MAXFONT - FIRSTWINFONT + 1)
#define	WINFONTNAMESIZE	64	/* LF_FULLFACESIZE	*/

typedef struct Pfontlist {
	char	fontnum;
	char	fontname[16];	/* Because Jim GOTTA (@#$!) have 15 char names */
	unsigned char	*fsizes;
	} Pfontlist;

typedef struct Wfontlist {
	char	fontnum;
	char	fontname[WINFONTNAMESIZE + 1];
	} Wfontlist;

extern Pfontlist *FontTable;
extern int		NumFonts;

unsigned char *GetFSizes( short fnum );
char *GetFontName( short fnum );
int  GetFontNumber( char *fname );
void LoadFontTable ( void );
void UnLoadFontTable(void);

/*
* help.h - defines & typedefs used by help and manual routines
*/

#ifndef HELP_H
#define HELP_H

#define		MAN_REVISION	2		/* bump when manual file format changes	*/

#define		MAN_EOL_MARKER		'\n'
#define		MAN_EOL_STRING		"\n"

#define		MAN_DEFAULT_COLS	78
#define		MAN_MIN_COLS		1
#define		MAN_MAX_COLS		106

#define		MAN_DEFAULT_LINES	16
#define		MAN_MIN_LINES		1
#define		MAN_MAX_LINES		32

#define		MAN_DEFAULT_FONT	"Woodland"
#define		MAN_DEFAULT_SIZE	12

#define		MAN_DEFAULT_TAB		4
#define		MAN_DEFAULT_MARGIN	1

#define		MAX_DNIX_NAME	14
#define		MAN_NAMESIZE	80

#define		MAN_NOMARKER	0
#define		MAN_MARKER		1
#define		MAN_PAGEBREAK	2

#define		MAN_BLOCKSIZE		2048	/* must be at least MANCOLSMANLINES	*/

#define		EXITHELP1		' '			/* key to exit with		*/
#define		EXITHELP2		RT_ESC		/* alternate exit key	*/
#define		HELPSEARCH1		's'			/* case insensitive search	*/
#define		HELPSEARCH2		'S'
#define		HELPCSEARCH1	'c'			/* case sensitive search	*/
#define		HELPCSEARCH2	'C'
#define		FWDSEARCH1		'n'			/* next occurrance			*/
#define		FWDSEARCH2		'N'
#define		BACKSEARCH1		'p'			/* prev occurrance			*/
#define		BACKSEARCH2		'P'
#define		HELPHELP1		'h'			/* Bring up helpfile		*/
#define		HELPHELP2		'H'
#define		HELPHELP3		'?'

#define		MAN_NEUTRAL		0
#define		MAN_FORWARD		1
#define		MAN_BACKWARD	-1

#define		MAN_FRAME		0
#define		MAN_REVERSE		1

#define	OK							0
#define	CANT_READ_MAN_FILE			-1

typedef struct HELPINDX
{
	short	hlp_appl;	/* application number				*/
	short	hlp_item;	/* field number						*/
	long	hlp_offset;	/* offset into text for this field	*/
} helpindx;

typedef struct SCRFLDINDX {
	short	scr_appl;
	short	scr_item;
	short	fld_appl;
	short	fld_item;
	long	hlp_offset;
	} scrfldindx;

typedef struct STRINGINDX {
	char	hlp_string[NAME_KEYSIZE];
	long	hlp_offset;
	} stringindx;

typedef struct HELPHDR {
	char	help_id[4];
	long	help_revision;
	short	help_rows;
	short	help_cols;
	short	help_font;
#ifdef SIXTEENBITCHARS
	short	help_otherfont;
#endif	/* SIXTEENBITCHARS */
	short	help_size;
#ifdef SIXTEENBITCHARS
	short	help_othersize;
#endif	/* SIXTEENBITCHARS */
	short	help_tabs;
	short	help_margin;
	long	text_start;
	long	field_count;
	long	screen_count;
	long	scrfld_count;
	long	tran_count;
	long	menu_count;
	long	string_count;
	long	page_count;
} helphdr;

typedef struct MANINFO {

	/* options from manual_options list	*/
	short	default_rows,
			default_cols,
			box_type,
			search_type,
			match_type,
			first_page,
			font,
#ifdef SIXTEENBITCHARS
			otherfont,
#endif	/* SIXTEENBITCHARS */
			size,
#ifdef SIXTEENBITCHARS
			othersize,
#endif	/* SIXTEENBITCHARS */
			tabs,
			margin;
	char	arrow_enable,
			*search_string,
			*filename,
			*title;

	/* run-time vars	*/
	int		man_fd;
	char	*textbufr;
	short	indexed;
	int		search_length;
	int		block_no;
	int		page_no;
	char	*nextpage;
	char	*eofptr;
	short	search_row;
	short	search_startcol;
	short	search_endcol;
	long	find_offset;
	char	case_sensitive;

	} maninfo;
#endif

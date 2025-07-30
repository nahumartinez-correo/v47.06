/* winhelp message codes - used only by C code */
#define	_WHSTART			'S'	/* start windows help system				*/
#define	_WHEXIT				'X'	/* close windows help						*/
#define	_WHGETENV			'E'	/* get current settings						*/
#define _WHINQKEY			'I'	/* inquire existence of an ASCII key		*/
#define	_WHSELKEY			'K'	/* select topic from an ASCII key			*/
#define	_WHNEXTPICKLINE		'P'	/* Get the next line in the topic name list	*/
#define	_WHLINKTOPIC		'L'	/* Create a link from a key to a topic		*/
#define	_WHGETTOPICTITLE	'T'	/* get the title of a topic from a key		*/
#define _WHOPTION		'O'

/* equivalent winhelp message codes for PCL use	*/
#define	WH_START			"S"
#define	WH_EXIT				"X"
#define WH_GETENV			"E"
#define WH_INQKEY			"I"
#define	WH_SELKEY			"K"
#define	WH_NEXTPICKLINE		"P"
#define	WH_LINKTOPIC		"L"
#define	WH_GETTOPICTITLE	"T"	/* get the title of a topic from a key		*/
#define WH_OPTION		"O"

/* winhelp error codes	*/
#define	WH_UNKNOWN		-201		/* unknown request code				*/
#define	WH_NOHELPFILE	-202		/* no windows help file				*/
#define	WH_NOKEYFILE	-203		/* no rt key-to-topic file			*/
#define	WH_NOPROJFILE	-204		/* no rt key-to-topic file			*/
#define	WH_NOMATCH		-205		/* no match for specified key		*/
#define	WH_NOTOPEN		-206		/* help system not initialized		*/
#define	WH_WINHELPERROR	-207		/* call to WinHelp failed			*/
#define WH_NOTEDITMODE	-208		/* not currently in edit mode		*/

/* These defines represent the bit settings used by the winhelp routine		*/
/* to determine the index type(s) to use when locating a 'smart help'		*/
/* index point																*/
#define		WH_FLD			1
#define		WH_SCR			2
#define		WH_SCRFLD		4
#define		WH_PCL			8		/* transactions are PCL routines		*/
#define		WH_STRING		16
#define		WH_MNU			32
#define		WH_ALL			63		/* all bits set, do all index searches	*/

/* used by whoption */
#define		WH_POPUP		0	/* 1=use HELP_CONTEXTPOPUP 0=use HELP_CONTEXT on field level help */
#define		WH_FINDER		1	/* 1=use HELP_FINDER 0=use HELP_CONTENTS */



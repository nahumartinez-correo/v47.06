/* ref message codes - used only by C code */
#define	_REFOPEN		'o'	/* open command (refcsr use only)		*/
#define	_REFEDITMODE	'e'	/* edit mode (refcsr use only)			*/
#define	_REFGETENV		'v'	/* get current settings (refcsr only)	*/
#define	_REFSELBOOK		'b'	/* select a book						*/
#define _REFINQASCII	'a'	/* inquire existence of an ASCII key	*/
#define	_REFSELASCII	'A'	/* select topic from an ASCII key		*/
#define	_REFSELTOPIC	't'	/* select a topic by number				*/
#define	_REFSTATUS		's'	/* get ref's current status				*/
#define	_REFWINDOW		'w'	/* display ref as the foreground window	*/
#define	_REFICONIFY		'i'	/* display ref as an icon				*/
#define	_REFEXIT		'q'	/* terminate ref						*/
#define	_REFEDITTOPIC	'e'	/* edit topic linked to ASCII key		*/
#define	_REFINQTCHANGED	'c'	/* inquire if current topic is changed	*/
#define	_REFSAVETOPIC	'S'	/* save current topic to file			*/
#define _REFGETTEXT		'G'	/* get text block from current topic	*/

/* equivalent ref message codes for PCL use	*/
#define REFOPEN			"o"
#define	REFSELBOOK		"b"
#define REFINQASCII		"a"
#define	REFSELASCII		"A"
#define	REFSELTOPIC		"t"
#define	REFSTATUS		"s"
#define	REFWINDOW		"w"
#define	REFICONIFY		"i"
#define	REFEXIT			"q"
#define	REFEDITTOPIC	"e"
#define	REFINQTCHANGED	"c"
#define	REFSAVETOPIC	"S"
#define	REFGETTEXT		"G"

/* refcsr defines */
#define	REFPICKLIST		"p"
#define REFGETENV		"v"

/* ref fatal exit codes	*/
#define	REFNOCONNECT		-200		/* can't connect to terminal		*/

/* ref message mode error codes */
#define	REFUNKNOWN			-201		/* unknown request code				*/
#define REFNOBOOK			-202		/* book locked or doesn't exist		*/
#define	REFNOTOPIC			-203		/* request topic is not valid		*/
#define	REFNOTINEDITMODE	-204		/* edit request in read-only mode	*/
#define	REFTOPICCHANGED		-205		/* current topic changed & not saved*/
#define	REFBUSY				-206		/* busy waiting for user input		*/
#define REFUNAUTHORIZED		-207		/* insufficient authority to access */
#define	REFNOTEXT			-208		/* can't get text when none in topic*/

/* refcsr error codes	*/
#define REFNOTOPEN			-209		/* ref process not active			*/
#define REFFORKFAILURE		-210		/* couldn't fork					*/
#define	REFEXECFAILURE		-211		/* couldn't exec ref				*/
#define REFALREADYOPEN		-212		/* already open, can't re-open		*/

/* These defines represent the bit settings used by the refcsr routine		*/
/* to determine the index type(s) to use when locating a 'smart help'		*/
/* index point																*/
#define		REFFLD			1
#define		REFSCR			2
#define		REFSCRFLD		4
#define		REFPCL			8		/* transactions are PCL routines		*/
#define		REFSTRING		16
#define		REFMNU			32
#define		REFALL			63		/* all bits set, do all index searches	*/

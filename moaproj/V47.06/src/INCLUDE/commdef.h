
/* The following defines are used to test the RETVAL after a COMM MATCH
   operation. */

#ifndef TRUE
#define TRUE             1
#endif

#ifndef FALSE
#define FALSE            0
#endif
/* The following are the token definitions for the comm data types: */

#define ASCII            0x30
#define EBCDIC           0x31
#define BCD              0x32
#define BINARY           0x33
#define ASCIIENCHEX      0x34
#define ZPD              0x35
#define ASCII_BULK       0x36
#define SBINARY          0x37
#define ALC              0x38
#define SEBC             0x39
#define FLAG             0x3a
#define USER             0x3b
#define BULK             0x3c

/* The following definitions are used by the COMM WAITFORMSG verb: */

#define RESET_OUT			3
#define TIMED_OUT			2

/* The following definitions are the header elements allowed to be tested
   by COMM_HDR verb: */

#define CSH_START				'\x21'		/* this is used to subtract for index */

#define CSH_CATEGORY            '\x21'
#define CSH_ERROR_CODE          '\x22'
#define CSH_FORMAT              '\x23'
#define CSH_BRACKET             '\x24'
#define CSH_REQUEST             '\x25'
#define CSH_RESOURCE_TYPE       '\x26'
#define CSH_REV_LEVEL           '\x27'
#define CSH_SEGMENT             '\x28'
#define CSH_SESSION_STATUS      '\x29'
#define CSH_SNA_BIND            '\x2a'
#define CSH_SNA_SENSE           '\x2b'
#define CSH_TERM_ADDRESS        '\x2c'
#define CSH_VERB                '\x2d'
#define CSH_SOURCE              '\x2e'
#define CSH_DEST                '\x2f'
#define CSH_CRT_ROW             '\x30'
#define CSH_CRT_COL             '\x31'
#define CSH_CRT_MATCH_LENGTH    '\x32'
#define CSH_CRT_WINDOW		    '\x33'
#define CSH_END_TO_END			'\x34'
#define CSH_HOST_TIME		    '\x35'
#define CSH_LINK_STAT		    '\x36'
#define CSH_COMM_STAT		    '\x37'
#define CSH_SRV_TYPE			'\x38'
#define CSH_LINK_CHANNEL		'\x39'
#define CSH_SESSION_NAME		'\x3a'
#define CSH_CHGDIR				'\x3b'
#define CSH_CONFIRM				'\x3c'
#define CSH_NEGRSP_ALLOWED		'\x3d'
#define CSH_RCV_MODE			'\x3e'
#define CSH_LINKTYPE			'\x3f'
#define CSH_AUXPORT				'\x40'

#define	 	L_VER			4		/* ASCII dec rev_level */

#define COMM_CRTEXIT          'a'
#define COMM_CRTDATA          'b'
#define COMM_CRTHELP          'c'

#define	API_KYBD_UNLOCKED			0
#define	API_OK						1
#define	API_MATCH					1
#define	API_NOMATCH					0

#define KEY_HIDE_WINDOW 900
#define KEY_SHOW_WINDOW 901
#define KEY_EXIT_WINDOW 902


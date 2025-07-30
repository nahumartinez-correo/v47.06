/* -------------------------------------------------------- */
/*  One-byte tokens which are not PCL statements, but are   */
/*  reserved words nonetheless.  The routines which deal    */
/*  with these symbols are token_lookup() and token_name()  */
/* -------------------------------------------------------- */


/* the following *must not* overlap the values of the TYPE_xxx */
/* defines in pcldefines.h, nor the PCLop values, that's why   */
/* they start with 0x01.  But, they're not verbs, so values    */
/* can overlap those of verbs...                               */

#if 0
/*
** Predefined Clipboard Formats, similar set in winuser.h
*/
#define DDE_TEXT             	(byte) 0x01
#define DDE_BITMAP           	(byte) 0x02
#define	DDE_METAFILEPICT     	(byte) 0x03
#define DDE_SYLK             	(byte) 0x04
#define DDE_DIF              	(byte) 0x05
#define DDE_TIFF             	(byte) 0x06
#define DDE_OEMTEXT          	(byte) 0x07
#define DDE_DIB              	(byte) 0x08
#define DDE_PALETTE          	(byte) 0x09
#define DDE_PENDATA          	(byte) 0x0A
#define DDE_RIFF             	(byte) 0x0B
#define DDE_WAVE             	(byte) 0x0C
#define DDE_UNICODETEXT      	(byte) 0x0D
#define	DDE_ENHMETAFILE      	(byte) 0x0E
#endif
	
/*
** P+ defines for DDE verb set
*/	
#define	DDE_INITIALIZE			(byte) 0x21
#define	DDE_CONNECT				(byte) 0x22
#define	DDE_ADVISE				(byte) 0x23
#define	DDE_REQUEST				(byte) 0x24
#define	DDE_POKE				(byte) 0x25
#define	DDE_EXECUTE				(byte) 0x26
#define	DDE_DISCONNECT			(byte) 0x27
#define	DDE_ABANDON_TRANS		(byte) 0x28
#define	DDE_ENABLE				(byte) 0x29
#define	DDE_UNINITIALIZE		(byte) 0x2A
#define	DDE_SERVER				(byte) 0x2B
#define	DDE_CONFIG				(byte) 0x2C
#define	DDE_SEND_DATA			(byte) 0x2D
#define	DDE_CONNECT_LIST		(byte) 0x2E
#define	DDE_QUERY_NEXT_SERVER	(byte) 0x2F
#define	DDE_SERVICE				(byte) 0x30
#define	DDE_TOPIC				(byte) 0x31
#define	DDE_DATA_CALLBACKPROC	(byte) 0x32
#define	DDE_MAX_CONVERSATIONS	(byte) 0x33
#define	DDE_HOT_LINK			(byte) 0x34
#define	DDE_WARM_LINK			(byte) 0x35
#define	DDE_COLD_LINK			(byte) 0x36
#define	DDE_DISABLE				(byte) 0x37
#define	DDE_CONVERSATION		(byte) 0x38
#define	DDE_DISCONNECT_LINK		(byte) 0x39

/*
** P+ defines for DLL/DECLARE verb set
*/

#define DLL_LIB					(byte) 0x40
#define DLL_ALIAS				(byte) 0x41
#define DLL_BYREF				(byte) 0x42
#define DLL_BYVAL				(byte) 0x43
#define DLL_DATAREF				(byte) 0x44
#define DLL_VARARGS				(byte) 0x45
#define DLL_BYOBJ				(byte) 0x46

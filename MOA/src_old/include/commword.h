/* -------------------------------------------------------- */
/*  One-byte tokens which are not PCL statements, but are   */
/*  reserved words nonetheless.  The routines which deal    */
/*  with these symbols are token_lookup() and token_name()  */
/* -------------------------------------------------------- */


/* the following *must not* overlap the values of the TYPE_xxx */
/* defines in pcldefines.h, nor the PCLop values, that's why   */
/* they start with 0x01.  But, they're not verbs, so values    */
/* can overlap those of verbs...                               */

#define	C_DELIMITER		(byte) 0x01
#define	C_FILL			(byte) 0x02
#define	C_GET			(byte) 0x03
#define	C_JUSTLEFT		(byte) 0x04
#define	C_JUSTRIGHT		(byte) 0x05
#define	C_PATTERN		(byte) 0x06
#define	C_PUT			(byte) 0x07
#define	C_RECEIVE		(byte) 0x08
#define	C_SEND			(byte) 0x09
#define	C_CATEGORY		(byte) 0x0A
#define	C_SENDCOMMREQ	(byte) 0x0B
#define	C_SENDCONFIRM	(byte) 0x0C
#define	C_SENDDATA		(byte) 0x0D
#define	C_FORMAT		(byte) 0x0E
#define	C_SETRESOURCE	(byte) 0x0F
#define	C_BRACKET		(byte) 0x10
#define	C_TRIMLEFT		(byte) 0x11
#define	C_TRIMRIGHT		(byte) 0x12
#define	C_TYPE			(byte) 0x13
#define	C_WAITFORMSG	(byte) 0x14
#define	C_WSLOG			(byte) 0x15
#define C_FROM          (byte) 0x16
#define C_TO            (byte) 0x17
#define C_MATCH         (byte) 0x18
#define C_BUFFER		(byte) 0x19
#define C_COL			(byte) 0x1A
#define C_CURSORABS		(byte) 0x1B
#define C_CURSORREL		(byte) 0x1C
#define C_FIELDABS		(byte) 0x1D
#define C_FIELDREL		(byte) 0x1E
#define C_FINDATTR		(byte) 0x1F
#define C_KYBDSTATUS	(byte) 0x20
#define C_ROW			(byte) 0x21
#define C_SETMODE		(byte) 0x22
#define C_SETWINDOW		(byte) 0x23
#define C_TESTATTR		(byte) 0x24
#define	C_PUTBUFFER		(byte) 0x25
#define C_PUT_DATA		(byte) 0x26
#define C_PUT_KEY		(byte) 0x27
#define C_ALLOCATE		(byte) 0x28
#define C_DEALLOCATE	(byte) 0x29
#define C_EDIT          (byte) 0x2A
#define C_LENGTH        (byte) 0x2B
#define C_FLUSH         (byte) 0x2C
#define C_RESET         (byte) 0x2D
#define C_SETEVENT      (byte) 0x2E
#define C_NO_RESET_ALLOWED (byte) 0x2F
#define C_RESET_ALLOWED (byte) 0x30

/* The following are the token definitions for the comm pointers: */

#define C_FIRSTPTR		(byte) 0x31
#define C_MSG			(byte) 0x31
#define C_MATCHBEGIN	(byte) 0x32
#define C_MATCHEND		(byte) 0x33
#define C_TEMPPTRA		(byte) 0x34
#define C_TEMPPTRB		(byte) 0x35
#define C_TEMPPTRC		(byte) 0x36
#define C_MSGBEGIN		(byte) 0x37
#define C_MSGEND		(byte) 0x38
#define C_LASTPTR 		(byte) 0x38

#define C_INIT_SEND		(byte) 0x40
#define C_INIT_RECEIVE	(byte) 0x41
#define C_START_LOG 	(byte) 0x42
#define C_STOP_LOG 		(byte) 0x43
#define C_RESET_LOG 	(byte) 0x44
#define C_PUT_COMM_DATA (byte) 0x45
#define C_USER_TYPE		(byte) 0x46
#define C_SEGMENT		(byte) 0x47
#define C_NOTIFY		(byte) 0x48
#define C_NONOTIFY		(byte) 0x49
#define C_PASSTHRU		(byte) 0x4a
#define C_STATUSONLY	(byte) 0x4b
#define C_FRONT			(byte) 0x4c
#define C_BACK			(byte) 0x4d
#define C_CURRENT		(byte) 0x4e
#define C_CONFIRM		(byte) 0x4f
#define C_GETCURSOR		(byte) 0x50
#define	C_TRIMOUT		(byte) 0x51

#define API_MAX_KEYS	20				/* <=== this is not really a verb */

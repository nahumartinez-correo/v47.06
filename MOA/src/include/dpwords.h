/* ----------------------------------------------------------- */
/*  1-byte tokens corresponding to PCL draw & print statements */
/*  Routines which handle these tokens are dp_token() and	   */
/*  dp_token_name().  These tokens can overlap tokens used for */
/*  CDS, COMM, and CRT statements.  The RESET token must be    */
/*  the same as the one in pclwords.h.						   */
/* ----------------------------------------------------------- */

#ifndef DPWORDS_H
#define DPWORDS_H

#define DP_BOX             (byte) 0x01
#define DP_CARRIAGERETURN  (byte) 0x02
#define DP_CENTERFOLD      (byte) 0x03
#define DP_CHECKBOX        (byte) 0x04
#define DP_FIELD           (byte) 0x05
#define DP_FLUSH           (byte) 0x06
#define DP_FORMFEED        (byte) 0x07
#define DP_FRAMERECT       (byte) 0x08
#define DP_HTAB            (byte) 0x09
#define DP_ICON            (byte) 0x0A

#define DP_MLFIELD         (byte) 0x0B	/* added for Release 4.0 */

#define DP_LANGUAGE        (byte) 0x0C
#define DP_LINE            (byte) 0x0D
#define DP_LINENUM         (byte) 0x0E
#define DP_LINETO          (byte) 0x0F
#define DP_LSPACING        (byte) 0x10
#define DP_MARGIN          (byte) 0x11
#define DP_NEWLINE         (byte) 0x12
#define DP_PAINTRECT       (byte) 0x13
#define DP_PENPAT          (byte) 0x14
#define DP_PENSIZE         (byte) 0x15
#define DP_PICTURE         (byte) 0x16

#define DP_MLTEXT          (byte) 0x17	/* added for Release 4.0 */

#define DP_POSITIONABS     (byte) 0x18
#define DP_POSITIONREL     (byte) 0x19
#define DP_PRINT           (byte) 0x1A
#define DP_PRTRESET        (byte) 0x1B
#define DP_PUSHBUTTON      (byte) 0x1C
#define DP_RADIOBUTTON     (byte) 0x1D
#define DP_SENDHDR         (byte) 0x1E
#define DP_SIGNATURE       (byte) 0x1F
#define DP_STYLE           (byte) 0x20
#define DP_TEXT            (byte) 0x21
#define DP_TEXTFONT        (byte) 0x22
#define DP_TEXTSIZE        (byte) 0x23
#define DP_TITLE           (byte) 0x24
#define DP_VTAB            (byte) 0x25
#define DP_GROUP           (byte) 0x26
#define DP_SIDE            (byte) 0x27		/* added in Release 4.0 */
#define DP_DOWNLOAD        (byte) 0x28		/* added in Release 4.0 */
#define DP_MACRO           (byte) 0x29		/* added in Release 4.0 */
#define DP_DELETEMACRO     (byte) 0x2A		/* added in Release 4.0 */
#define DP_DELETETEMP      (byte) 0x2B		/* added in Release 4.0 */
#define DP_DELETEALL       (byte) 0x2C		/* added in Release 4.0 */
#define DP_DUPLEX          (byte) 0x2D		/* added in Release 4.0 */
#define DP_COMBINATION     (byte) 0x2E		/* added in Release 4.0 */
#define DP_VOX             (byte) 0x2F		/* added in Release 4.0 */
#define DP_RELIEFBUTTON    (byte) 0x30		/* added in Release 4.0 */
#define DP_UP              (byte) 0x31		/* added in Release 4.0 */
#define DP_DOWN            (byte) 0x32		/* added in Release 4.0 */
#define DP_INVERT          (byte) 0x33		/* added in Release 4.0 */
#define DP_TEST            (byte) 0x34		/* added in Release 4.0 */
#define DP_NOCURSOR        (byte) 0x35		/* added in Release 4.0 */
#define DP_SCALE           (byte) 0x36		/* added in Release 5.0 */
#define DP_ORIENTATION     (byte) 0x37		/* added in Release 5.0 */
#define DP_PSOURCE         (byte) 0x38		/* added in Release 5.0 */
#define DP_TRANSPARENT     (byte) 0x39		/* added in Release 5.0 */

#define DP_LISTBOX         (byte) 0x3A		/* for AB2 Release 1.0 */
#define DP_COMBOBOX        (byte) 0x3B		/* for AB2 Release 1.0 */
#define DP_EDITBOX         (byte) 0x3C		/* for AB2 Release 1.0 */
#define DP_SPINNER         (byte) 0x3D		/* for AB2 Release 1.0 */
#define DP_MASKEDEDITBOX   (byte) 0x3E		/* for AB2 Release 1.0 */

#define DP_COMBOFIELD      (byte) 0x3F		/* for R1 */

#define DP_TEXTFONTNAME    (byte) 0x40		/* added in Release 5.0 */

#define DP_RESET           RESET	/* pclwords.h must be included */

#define PDM_CREATE			(byte) 0x50
#define PDM_APPEND			(byte) 0x51
#define PDM_POPUP			(byte) 0x52
#define PDM_INSERT			(byte) 0x53
#define PDM_DELETE			(byte) 0x54
#define PDM_ENABLEITEM		(byte) 0x55
#define PDM_CHECKITEM		(byte) 0x56
#define PDM_HILITEITEM		(byte) 0x57
#define PDM_GETSTATE		(byte) 0x58
#define PDM_SET				(byte) 0x59
#define PDM_HELP			(byte) 0x5A


#endif

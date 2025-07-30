//
// MODULENAME - wingsp.c
//
// $Name: R1V4C13 $
// $Revision: 1.29 $
// $Date: 1999/03/11 08:29:29 $
//
// Synopsis:
//  WINGSP  --  Networked Generalized Serial Port routines
//
//    This module contains code for the custom C routine 'gsp'
//    which takes a variable number of arguments.  The 1st arg
//    is one of 15 functions, as defined by the #define symbols
//    in the #include file "gsp.h".  The functions include:
//
//         Device open & close
//         Get & set TERMIO parameters
//         Get & put a character
//         Read & write (optional LRC on write)
//				NOTE!!! The read operation will allow both reset and key
//						input to get out of the operation.  DALE
//         Get last character received		(FRANK: Not done for 1st rel.)
//         Purge the input & output buffers
//         Set a seconds value for inter-character timeouts
//         Dump a buffer with ASCII control chars (FRANK: Not for 1st rel.)
//         Dump the current TERMIO structure (FRANK: Not for 1st rel.)
//         Turn ON/OFF the standard LRC routine
//         Compute an LRC using the standard routine
//
//
//    The remote comm port server (remsrvr) provides the network accessability
//	  to a serial device.  
//
//	AUTHOR:							H. Marc Lewis	Jan 1992
//
//	UNSCRUPULOUS TRANSMOGRIFIER:	Chuck Hartshorn Jan 1993
//	NT PORT							Frank Albrecht		1994
//	NT PORT TRANSMOGRIFIER:			Dale Bennett	Aug/Sept/Dec 1994
//	CLEANUP for REMSRVR				Dale Bennett	Mar 1995
//

#include "pch.h"

#include <gsp.h>
#include <defines.h>
#include <typedefs.h>
#include <convers.h>
#include <rtprotos.h>
#include <nowait.h>			// Must be *BEFORE* <rtdef.h> !!!!!!!!!
#include <rtdef.h>
#include <csrdef.h>			// Must be *AFTER* <rtdef.h>
#include <ntddser.h>
#include <mcnuser.h>

#define	NCC	9
//
// Ioctl control packet
//
struct termio {
	unsigned short	c_iflag;	/* input modes */
	unsigned short	c_oflag;	/* output modes */
	unsigned short	c_cflag;	/* control modes */
	unsigned short	c_lflag;	/* line discipline modes */
	char			c_line;		/* line discipline */
	unsigned char   c_ccs[NCC];	/* control chars */
	char			c_pad;		/* Padding..... */
};

//
//	Defines
//
#define CS_HIDE_DEF	1	// Hide Comm/Cs defs.
//#define DEBUG		1	// if defined turns debug on
//#define FRANK			// defined turns on functions that are currently not implemented

#ifdef DEBUG
#define GSP_LOG(L,X) if (L <= Gverbose) printf X; fflush(NULL);
#else
#define GSP_LOG(L,X)
#endif

#define DEFAULT					0L
#define WAIT_ERROR				0xFFFFFFFF
#define RETURN_IMEDIATELY		0L
#define NT_ERROR				-1
// The following values are listed here for reference
//		RT_ERROR				1				// defined in defines.h
//		FAILURE					1				// defined in defines.h
//		RT_SUCCESS				0				// defined in defines.h

#define KEY_INPUT_NOTALLOWED	0
#define KEY_INPUT_ALLOWED		-1

//
// Prototypes
//
	extern char				*getcfg();
	extern void				rt_nop_verb_msg(char *);

	static HANDLE			gsp_open(char *, int);
	static int 				gsp_gettermio();
	static int 				gsp_settermio();
	static int 				gsp_putc(int);
	static int		   		gsp_read(char *, int, char *);
	static int				gsp_write(char *, int);
	static int	 			gsp_purge();
	static unsigned char	gsp_lrc(unsigned char *);
	static int				gsp_modemstatus();
//	static int				gsp_getfcntl(int cmd, field *fp);
//	static int				gsp_setport(int cmd, int arg);

 	static int 				is_not_INT_field(struct RTOBJ *, field **);

	static void				remconnectdone();
	static void				remreaddone();
	static int				MySend(int, int);
	static int				gsp_wait(int);

	static void 			dump_buffer(char *,char *);
//	static void				dump_termio(char *, struct termio *);
//	static void				dump_history();
	static void				out_char(char);

	static long				gsprdinterchartime = 1;		// max seconds between chars
	static long				gsprdtotaltime = 1;			// max seconds to wait for any
	static long				gspwrttotaltime = 0;		// max seconds to write all
	static long				gspiopurge = 0;				// valid values are 0,1,2
	static char				gsp_lastchar = 0;
	static short			standard_lrc = 1;
	static struct termio	current;

	static char	*ctl_chars[32] = {
					"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
					"BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI",
					"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
					"CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US"
					};

#define MAX_MCN_DATA_LEN	255

	static struct  			//Message to remote server
	{
		int		cmd;		// command/status to/from server
		DWORD	err;		// error code from server
		int		status;		// completion status from server
		unsigned char msg[MAX_MCN_DATA_LEN + 1];	// message buffer
	} msg;

#ifdef HISTORY
	static unsigned char	history[MAX_MCN_DATA_LEN + 1];
	static short			history_size;
#endif //HISTORY

	static char				Gremhost[50] = {0};
	static int				Gverbose = GSPMAX;
	static int				Gmcnverbose = MCNMAX;
	static int				Gdebug2console = 1;
	static SOCKET			Gremsock;
	static SOCKET			Nowait;
	static int				nowait_id;
	static field			*fp_nowait;

	static int				rembytesread = 0;

	static char				buffer[MAX_MCN_DATA_LEN + 1];


//--------------------------------------------------------------------------------------
//	wingsp
//
//	Function:	The gsp functions entry point
//
//	Entry:		argc	-	number of arguments to process
//				argv	-	array of arguments to be processed
//
//	Return:		CSR_FAIL_ARGCOUNT	-	incorrect number of arguments
//				CSR_FAIL_ARGTYPE	-	incorrect argument type
//				GSPERR_BAD_ARGUMENT	-	socket or I/O values incorrect
//				GSPERR_KEY_OUT		-	a valid key was hit
//				GSPERR_TIMEOUT		-	a time out has occured
//				GSPERR_RESET		-	the reset key has been hit
//				NT_ERROR			-	any NT or MCN related error
//				RT_SUCCESS			-	everything OK
//
//--------------------------------------------------------------------------------------
XDLL int
wingsp(int argc, struct RTOBJ *argv[])
{
	static int		vf201fd = -1;
	int				flags, index, rval, retval, size;
	char			logicaldev[30];
	field			*fp, *fp1, *fp2, *fp6;
	char			bv;

	retval = RT_SUCCESS;
	GSP_LOG(GSPMIN, ("\n***********************************\n"));
	if (argc >= 1) {
		rtobj2int(argv[0], &rval);
		GSP_LOG(GSPMIN, ("wingsp(), cmd = %d\n", rval));

		switch (rval) {
			case GSP_OPEN:
				GSP_LOG(GSPCMD, ("GSP_OPEN:\n"));
				if (argc != 4) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (argv[3]->obj.object != CSRARG_FLD) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				fp = (field *)argv[3]->ptr;
				if (fp->flags.type != FT_INT) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				rtobj2str(argv[1], logicaldev, sizeof(logicaldev));	/* device */
				rtobj2int(argv[2], &flags);							/* open flags */

				rval = (int)gsp_open(logicaldev, flags);
				if (rval < 0) {
					/* error occurred, Retval contains error status */
					retval = rval;
					break;
				}
				GSP_LOG(GSPCMD, ("GSP_OPEN: fd = %d\n", rval));
				int2field(rval, fp);
				break;

			case GSP_CLOSE:
				GSP_LOG(GSPCMD, ("GSP_CLOSE:\n"));
				if (argc != 2) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[1], &(int)Gremsock);
				rval = MySend(GSP_CLOSE, 0);
				if (rval = RT_SUCCESS && msg.cmd != RT_SUCCESS) {
					retval = msg.err;
					break;
				}
				MCNClose(hWnd, Gremsock);
				Gremsock = 0;
				break;

			case GSP_GET_TERMIO:
			{
				int arg1, arg2, arg3, arg4, arg5;
				field   *result;

				GSP_LOG(GSPCMD, ("GSP_GET_TERMIO:\n"));
				if (argc != 8) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (rtobj2int(argv[1], &arg1) != RT_SUCCESS) {	/* c_iflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[2], &arg2) != RT_SUCCESS) {	/* c_oflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[3], &arg3) != RT_SUCCESS) {	/* c_cflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[4], &arg4) != RT_SUCCESS) {	/* c_lflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[5], &arg5) != RT_SUCCESS) {	/* c_line */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (argv[6]->obj.object != CSRARG_FLD) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				fp6 = (field *)argv[6]->ptr;
				if (fp6->flags.type != FT_ASCII) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (fp6->length < 8) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[7], &(int)Gremsock) != RT_SUCCESS) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}

				rval = gsp_gettermio();
				if (rval != RT_SUCCESS) {
					retval = rval;
					break;
				}
				result = (field *) argv[1]->ptr;
				int2field(current.c_iflag, result);
				result = (field *) argv[2]->ptr;
				int2field(current.c_oflag, result);
				result = (field *) argv[3]->ptr;
				int2field(current.c_cflag, result);
				result = (field *) argv[4]->ptr;
				int2field(current.c_lflag, result);
				result = (field *) argv[5]->ptr;
				int2field(current.c_line, result);
				memcpy(fp6->storage,current.c_ccs,8);
				fp6->storage[8] = 0;
				fp6->numchars = 8;
				break;
			}

			case GSP_SET_TERMIO:
			{
				int arg1, arg2, arg3, arg4, arg5;

				GSP_LOG(GSPCMD, ("GSP_SET_TERMIO:\n"));
				if (argc != 8) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (rtobj2int(argv[1], &arg1) != RT_SUCCESS) {	/* c_iflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[2], &arg2) != RT_SUCCESS) {	/* c_oflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[3], &arg3) != RT_SUCCESS) {	/* c_cflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[4], &arg4) != RT_SUCCESS) {	/* c_lflag */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[5], &arg5) != RT_SUCCESS) {	/* c_line */
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (argv[6]->obj.object != CSRARG_FLD) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				fp6 = (field *)argv[6]->ptr;
				if (fp6->flags.type != FT_ASCII || fp6->length < 8) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				if (rtobj2int(argv[7], &(int)Gremsock) != RT_SUCCESS) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				current.c_iflag = arg1;
				current.c_oflag = arg2;
				current.c_cflag = arg3;
				current.c_lflag = arg4;
				current.c_line = arg5;
				memcpy(current.c_ccs, fp6->storage, 8);
				current.c_ccs[8] = 0;
				rval = gsp_settermio();
				if (rval != RT_SUCCESS) {
					retval = rval ;
				}
				break;
			}

			case GSP_GETC:
				GSP_LOG(GSPCMD, ("GSP_GETC:\n"));
				if (argc != 3) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (is_not_INT_field(argv[1], &fp1)) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				rtobj2int(argv[2], &(int)Gremsock);

				rval = gsp_read(&bv, 1 ,"");
				if (rval != 1) {
					retval = rval ;
				}
				int2field((int)bv, fp1);
				break;

			case GSP_PUTC:
				GSP_LOG(GSPCMD, ("GSP_PUTC:\n"));
				if (argc != 3) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[1], &index);
				rtobj2int(argv[2], &(int)Gremsock);
				retval = gsp_putc(index);
				break;

			case GSP_READ:
				GSP_LOG(GSPCMD, ("GSP_READ:\n"));
				if (argc != 5) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[2], &size);			/* size */

				if (size > MAX_MCN_DATA_LEN) {
					retval = GSPERR_BAD_ARGUMENT;
					break;
				}

				fp2 = (field *)argv[3]->ptr;		/* end of data chars */
				if (fp2->flags.type != FT_ASCII) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}

				rtobj2int(argv[4], &(int)Gremsock);	/* file descriptor */
				fp = (field *)argv[1]->ptr;
				if (fp->flags.type != FT_ASCII) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}

				memset(fp->storage, 0, size);
				(short)fp->numchars = gsp_read((char *)fp->storage,
											size, (char *)fp2->storage);
				if (fp->numchars <= 0) {
					retval = fp->numchars;
				}
				break;

			case GSP_WRITE:
				GSP_LOG(GSPCMD, ("GSP_WRITE:\n"));
				if (argc != 4) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[2], &size);				 // length
				rtobj2int(argv[3], &(int)Gremsock);	  // file descriptor
				fp = (field *)argv[1]->ptr;			  // source of data
				if (fp->flags.type != FT_ASCII) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				retval = gsp_write((char *)fp->storage, size);
				break;

			case GSP_PURGE:
				GSP_LOG(GSPCMD, ("GSP_PURGE:\n"));
				if (argc < 2 || argc > 3) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (argc == 3) {
					if (rtobj2int(argv[1], &gspiopurge) != RT_SUCCESS) {
						retval = CSR_FAIL_ARGTYPE;
						break;
					}
					else if (gspiopurge < 0 || gspiopurge > 2) {
						retval = CSR_FAIL_ARGTYPE;
						break;
					}
					rtobj2int(argv[2], &(int)Gremsock);
				}
				else {
					rtobj2int(argv[1], &(int)Gremsock);
				}
				if (Nowait) {
					Nowait = 0;
					retval = MCNCancel(nowait_id);
					GSP_LOG(GSPMIN, ("GSP_PURGE: Cancel returned %d for %d\n", retval, nowait_id));
				}
				retval = gsp_purge();
				break;

			case GSP_SET_TIMEOUT:
				GSP_LOG(GSPCMD, ("GSP_SET_TIMEOUT:\n"));
				if (argc != 4) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[1], &gsprdinterchartime);
				rtobj2int(argv[2], &gsprdtotaltime);
				rtobj2int(argv[3], &gspwrttotaltime);
				GSP_LOG(GSPMIN,("GSP_SET_TIMEOUT: read interchartime = %d, read totaltime = %d, write totaltime = %d\n",
												gsprdinterchartime, gsprdtotaltime, gspwrttotaltime));
				break;

			case GSP_SET_DEBUG_LEVEL:
				GSP_LOG(GSPCMD,("GSP_SET_DEBUG_LEVEL:\n"));
				if (argc != 2) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[1], &size);
				Gverbose = size;
				GSP_LOG(GSPMIN,("GSP_SET_DEBUG_LEVEL: %d\n", Gverbose));
				break;

			case GSP_SET_LRC:
				GSP_LOG(GSPCMD, ("GSP_SET_LRC:\n"));
				if (argc != 2) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[1], &size);
				standard_lrc = (short)size;
				break;

			case GSP_COMPUTE_LRC:
				GSP_LOG(GSPCMD, ("GSP_COMPUTE_LRC:\n"));
				if (argc != 3) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (is_not_INT_field(argv[2], &fp1)) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				rtobj2str(argv[1], buffer, sizeof(buffer));
				rval = gsp_lrc((unsigned char *)buffer);
				int2field(rval, fp1);
				break;

			case GSP_MODEMSTATUS:
				GSP_LOG(GSPCMD, ("GSP_MODEMSTATUS:\n"));
				if (argc != 3) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				if (is_not_INT_field(argv[1], &fp1)) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}
				rtobj2int(argv[2], &(int)Gremsock);

				rval = gsp_modemstatus();
				if (rval != RT_SUCCESS) {
					retval = rval ;
					break;
				}
				int2field((int)msg.err, fp1);
				break;

			case GSP_DUMP_BUFFER:
//FRANK: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef FRANK
				GSP_LOG(GSPCMD, ("GSP_DUMP_BUFFER:\n"));
				if (argc != 3) {
					return(CSR_FAIL_ARGCOUNT);
				}
				rtobj2str(argv[1], buffer, sizeof(buffer));
				rtobj2str(argv[2], dumpbuff, sizeof(dumpbuff));
				dump_buffer(buffer, dumpbuff);
#else
				rt_nop_verb_msg("GSP_DUMP_BUFFER");
#endif //FRANK
				break;

			case GSP_GET_LASTC:
//FRANK: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef FRANK
				GSP_LOG(GSPCMD, ("GSP_GET_LASTC:\n"));
				if (argc != 2) {
					return(CSR_FAIL_ARGCOUNT);
				}
				if (vf201fd == -1) {
					return(GSPERR_NOTOPEN);		/* not OPENed */
				}
				if (argv[1]->obj.object != CSRARG_FLD) {
					return(CSR_FAIL_ARGTYPE);
				}
				fp = (field *) argv[1]->ptr;
				if (fp->flags.type != FT_INT) {
					return(CSR_FAIL_ARGTYPE);
				}
				*(int *)(fp->storage) = (int)gsp_lastchar;
#else
				rt_nop_verb_msg("GSP_GET_LASTC");
#endif //FRANK
				break;

			case GSP_DUMP_TERMIO:
// FRANK: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef FRANK
				GSP_LOG(GSPCMD, ("GSP_DUMP_TERMIO:\n"));
				if (argc != 2) {
					return(CSR_FAIL_ARGCOUNT);
				}
				rtobj2str(argv[1], buffer, sizeof(buffer));
				dump_termio(buffer, &current);
#else
				rt_nop_verb_msg("GSP_DUMP_TERMIO");
#endif //FRANK
				break;

			case GSP_HISTORY:
//HISTORY: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef HISTORY
				GSP_LOG(GSPCMD, ("GSP_HISTORY:\n"));
				if (argc != 1) {
					return(CSR_FAIL_ARGCOUNT);
				}
				dump_history();
#else
				rt_nop_verb_msg("GSP_HISTORY");
#endif //HISTORY
				break;

			case GSP_GETFCNTL:
//FRANK: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef FRANK
				GSP_LOG(GSPCMD, ("GSP_GETFCNTL:\n"));
				if (argc != 5) {
					return(CSR_FAIL_ARGCOUNT);
				}
				rtobj2int(argv[1], &(int)Gremsock);
				rtobj2int(argv[2], &i);			/* cmd */
				rtobj2int(argv[3], &ui);		/* arg */
				if (is_not_INT_field(argv[4], &fp1)) {
					return(CSR_FAIL_ARGTYPE);
				}
				return(gsp_getfcntl(i, ui, fp1));
#else
				rt_nop_verb_msg("GSP_GETFCNTL");
#endif //FRANK
				break;

			case GSP_SETFCNTL:
			case GSP_SETIOCTL:
//FRANK: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef FRANK
				GSP_LOG(GSPCMD, ("GSP_SETFCNTL/GSP_SETIOCTL:\n"));
				if (argc != 4) {
					return(CSR_FAIL_ARGCOUNT);
				}
				rtobj2int(argv[1], &(int)Gremsock);
				rtobj2int(argv[2], &i);		/* cmd || req */
				rtobj2int(argv[3], &ui);		/* arg */
				return(gsp_setport(rval, i, ui));
#else
				rt_nop_verb_msg("GSP_SETFCNTL/GSP_SETIOCTL");
#endif //FRANK
				break;

			case GSP_READ_NW:
				GSP_LOG(GSPCMD, ("GSP_READ_NW:\n"));
				if (argc != 5) {
					retval = CSR_FAIL_ARGCOUNT;
					break;
				}
				rtobj2int(argv[2], &size);	/* size */
				fp2 = (field *)argv[3]->ptr;		/* end of data chars */
				if (fp2->flags.type != FT_ASCII) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}

				rtobj2int(argv[4], &(int)Gremsock);	/* file descriptor */
				if (Nowait) {
					retval = MCNCancel(nowait_id);
					GSP_LOG(GSPMAX, ("GSP_READ_NW: Cancel returned %d for %d\n", retval, nowait_id));
				}
				fp_nowait = (field *) argv[1]->ptr;
				if (fp_nowait->flags.type != FT_ASCII) {
					retval = CSR_FAIL_ARGTYPE;
					break;
				}

				memset(fp_nowait->storage, 0, size);
				Nowait = Gremsock;
				retval = gsp_read((char *)fp_nowait->storage,
								  size,
								  (char *)fp2->storage);
				if (!retval) {
					GSP_LOG(GSPCMD, ("GSP_READ_NW: SUCCESS\n"));
					retval = RT_SUCCESS;
					break;
				}
				else {
					GSP_LOG(GSPERROR, ("GSP_READ_NW: FAILURE\n"));
					break;
				}
				break;

			case GSP_CANCEL:
//FRANK: Will not be implemented for first release of GSP Subroutines in PPNT.
#ifdef FRANK
				GSP_LOG(GSPCMD, ("GSP_CANCEL:\n"));
#else
				rt_nop_verb_msg("GSP_CANCEL");
#endif //FRANK
				break;

			default:
				GSP_LOG(GSPCMD, ("default:\n"));
				rterrorbox("ERROR: invalid command %d", rval);
				break;
		}
	}
	GSP_LOG(GSPMIN, ("wingsp() return %d\n", retval));
	return retval;
}

//--------------------------------------------------------------------------------------
//	wingsp call functions follow here
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//	gsp_open
//
//	Function:	open the network serial I/O port for the given logical device
//
//	Entry:		logicaldev	-	logical device name
//				flags		-	flags for open (remnant from P+ days)
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket or I/O values incorrect
//				RT_SUCCESS			-	everything OK
//				GSPERR_RESET		-	reset out of command
//
//--------------------------------------------------------------------------------------
static HANDLE
gsp_open(char *logicaldev, int flags)
{
	short	oops = FALSE;
	int		rval, result, sendlen, status, index;
	char	*portname;

	GSP_LOG(GSPFUNC, ("gsp_open() connect to Remote Server/Device Port = %s\n", logicaldev));
	//	Initialize the Event System
	//	Set MCN Logging
	//	#ifdef DEBUG
	//		MCNSetULog(Gmcnverbose, Gdebug2console, 0);
	//	#endif
	//
	//	Connect to Server
	//
	//	if (Gremsock) {
	//		MCNClose(hWnd, Gremsock);
	//	}
	Gremsock = 0;
	portname = logicaldev;
	if (*logicaldev != '/' || *(logicaldev + 1) != '/') {
		// FRANK: gethost name isn't available to me.
		//	if (!Gremhost[0]) {
		//		gethostname(Gremhost, sizeof(Gremhost));
		//	}
		//	if (!(portname = getcfg("-f5", "sysid", Gremhost, "path", logicaldev, NULL)) || *portname == 0) {
		//		GSP_LOG(GSPERROR, ("gsp_open() can't get portname for device (%s), host %s\n", logicaldev, Gremhost));
		//		return 0;
		//	}
		// FRANK: comenting out until netboss and getcfg issues get resolved.
	}
	GSP_LOG(GSPMAX, ("gsp_open() port name = %s\n", portname));

	sendlen = strlen(logicaldev);					// Cannot be greater than MAX
	for (index = 2; index < sendlen; index++) {
		if (*(logicaldev + index) == '/') {
			break;
		}
	}
	if (index == sendlen) {
		GSP_LOG(GSPERROR, ("gsp_open() port must include port name (//machine/REM_COMn)\n"));
		return (HANDLE)GSPERR_OPENFAILED;
	}
	*(logicaldev + index++) = 0;   					//	put string terminator

	//
	//	Make the network connection
	//
	GSP_LOG(GSPMIN,("gsp_open() connect to Remote Server %s device %s\n", logicaldev + 2, logicaldev + index));
	if ((status = MCNAsyncConnect(hWnd, MCN_TCP, remconnectdone, logicaldev + 2, logicaldev + index, MCN_TSDU, 0)) < 0) {
		GSP_LOG(GSPERROR, ("gsp_open() MCNAsyncConnect failed, returned %d\n", status));
		return (HANDLE)GSPERR_OPENFAILED;
	}
	GSP_LOG(GSPMIN,("gsp_open() MCNAsyncConnect returned %d\n", status));
	while (!Gremsock) {								// wait for connection to be established
		GSP_LOG(GSPMAX, ("gsp_open() call gsp_wait...waiting for connection...\n"));
		rval = gsp_wait(KEY_INPUT_NOTALLOWED);		// RT_SUCCESS or GSPERR_RESET
		if (rval != RT_SUCCESS) {
			if (!Gremsock) {						// OK to cancel request if no socket yet
				result = MCNCancel(status);			// cancel MCNAsyncConnect request
				if (result != status) {
					GSP_LOG(GSPERROR, ("gsp_open() MCNCancel of request %d failed\n", status));
				}
			}
			else if (Gremsock != INVALID_SOCKET){	// had valid socket so must close
				if ((result = MCNClose(hWnd, Gremsock)) < 0) {
					GSP_LOG(GSPERROR,("gsp_open() MCNClose failed, returned %d\n", result));
				}
				Gremsock = 0;
				break;
			}
		}
	}

	//
	//	Send open request to server
	//
	if (rval != RT_SUCCESS) {
		GSP_LOG(GSPERROR,("gsp_open() MCNAsyncConnect failed, user cancelled out\n"));
		Retval = rval;
		Gremsock = 0;
	}
	else if (Gremsock == INVALID_SOCKET) {
		GSP_LOG(GSPERROR,("gsp_open() MCNAsyncConnect failed, returned %d\n", Gremsock));
		Retval = GSPERR_OPENFAILED;						// MCNAsyncConnect failed
	}
	else {
		GSP_LOG(GSPMAX, ("gsp_open() connected, sock = %d\n", Gremsock));
		index += 4;									// Get past REM_
		sendlen -= index;
		memcpy (msg.msg, logicaldev + index, sendlen);
		msg.msg[sendlen++] = 0;
		msg.msg[sendlen++] = flags;					// the open flags
		msg.msg[sendlen] = 0;
		status = MySend(GSP_OPEN, sendlen);
		if (!status && !msg.status && msg.err == ERROR_ALREADY_ASSIGNED) {
			Retval = GSPERR_OPENFAILED;
			if ((status = MCNClose(hWnd, Gremsock)) < 0) {
				GSP_LOG(GSPERROR,("gsp_open() MCNClose failed, returned %d\n", status));
			}
			Gremsock = 0;
		}
		else { 
			Retval = Gremsock;
		}
	}
	GSP_LOG(GSPFUNC, ("gsp_open() returning %d\n", Retval)); 
	return (HANDLE)Retval;
}

//--------------------------------------------------------------------------------------
//	gsp_gettermio
//
//	Function:	get the TERMIO structure for network serial I/O port
//
//	Entry:		none	-	data is stored in msg structure in msg.msg
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket or I/O values incorrect
//				RT_SUCCESS			-	everything OK
//				GSPERR_RESET		-	reset out of command
//
//--------------------------------------------------------------------------------------
static int
gsp_gettermio()
{
	int status;

	Retval = RT_SUCCESS;
	GSP_LOG(GSPFUNC, ("gsp_gettermio() Gremsock = %d\n", Gremsock));
	if (Gremsock < 0) {
		GSP_LOG(GSPERROR, ("gsp_gettermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
		return(GSPERR_BAD_ARGUMENT);
	}
	msg.msg[0] = msg.msg[1] = 0;

	status = MySend(GSP_GET_TERMIO, 0); 
	if (status == RT_SUCCESS) {
		current.c_cflag &= ~CBAUD;
		GSP_LOG(GSPMAX, ("                Baud Rate      = %d\n", msg.msg[0] * 256 + msg.msg[1]));
		switch ((int)(msg.msg[0] * 256 + msg.msg[1])) {
			case 50:
				current.c_cflag |= B50;
				break;

			case 75:
				current.c_cflag |= B75;
				break;

			case 110:
				current.c_cflag |= B110;
				break;

			case 134:
				current.c_cflag |= B134;
				break;

			case 150:
				current.c_cflag |= B150;
				break;

			case 200:
				current.c_cflag |= B200;
				break;

			case 300:
				current.c_cflag |= B300;
				break;

			case 600:
				current.c_cflag |= B600;
				break;

			case 1200:
				current.c_cflag |= B1200;
				break;

			case 1800:
				current.c_cflag |= B1800;
				break;

			case 2400:
				current.c_cflag |= B2400;
				break;

			case 4800:
				current.c_cflag |= B4800;
				break;

			case 9600:
				current.c_cflag |= B9600;
				break;

			case 19200:
				current.c_cflag |= B19200;
				break;

			case 38400:
				current.c_cflag |= B38400;
				break;

			default:
//				rterrorbox("GSP_GET_TERMIO: INVALID BAUD RATE");
				// FRANK: may get bad value: set to 9600 default value.
				current.c_cflag |= B9600;
				break;
		}

		current.c_cflag &= ~CSIZE;
		GSP_LOG(GSPMAX, ("                Number of bits = %d\n", msg.msg[2]));
		switch (msg.msg[2]) {
			case 5:
				current.c_cflag |= CS5;
				break;

			case 6:
				current.c_cflag |= CS6;
				break;

			case 7:
				current.c_cflag |= CS7;
				break;

			case 8:
				current.c_cflag |= CS8;
				break;

			default:
				rterrorbox("GSP_GET_TERMIO: INVALID CHARACTER LENGTH");
				GSP_LOG(GSPERROR, ("gsp_gettermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
				return(GSPERR_BAD_ARGUMENT);
				break;
		}

		current.c_cflag |= PARENB;
		GSP_LOG(GSPMAX, ("                Parity         = %d\n", msg.msg[3]));
		switch (msg.msg[3]) {
			case NO_PARITY:
				current.c_cflag &= ~PARENB;
				break;

			case ODD_PARITY:
				current.c_cflag |= PARODD;
				break;

			case EVEN_PARITY:
				current.c_cflag &= ~PARODD;
				break;

			default:
				rterrorbox("GSP_GET_TERMIO: INVALID PARITY");
				GSP_LOG(GSPERROR, ("gsp_gettermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
				return(GSPERR_BAD_ARGUMENT);
				break;
		}

		GSP_LOG(GSPMAX, ("                Stop bits      = %d\n", msg.msg[4]));
		if (msg.msg[4] == STOP_BIT_1) {
			current.c_cflag &= ~CSTOPB;
		}
		else if (msg.msg[4] == STOP_BITS_1_5 ||
				 msg.msg[4] == STOP_BITS_2) {
			current.c_cflag |= CSTOPB;
		}
		else {
			rterrorbox("GSP_GET_TERMIO: INVALID STOP BITS");
			GSP_LOG(GSPERROR, ("gsp_gettermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
			return(GSPERR_BAD_ARGUMENT);
		}

		GSP_LOG(GSPMAX, ("                Eof            = %d\n", msg.msg[5]));
		current.c_ccs[4] = msg.msg[5];
    }
	else if (status == GSPERR_RESET) {
		GSP_LOG(GSPERROR, ("gsp_gettermio() returning = %d\n", GSPERR_RESET));
		return(status);
	}
	else {
		rterrorbox("GSP_GET_TERMIO: ERROR");
		GSP_LOG(GSPERROR, ("gsp_gettermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
		return(GSPERR_BAD_ARGUMENT);
	}
	GSP_LOG(GSPFUNC, ("gsp_gettermio() returning = %d\n", Retval));
	return(Retval);
}

//--------------------------------------------------------------------------------------
//	gsp_settermio
//
//	Function:	set the TERMIO structure for network serial I/O port
//
//	Entry:		none	-	data is stored in msg structure in msg.msg
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket or I/O values incorrect
//				FAILURE				-	errors from MySend
//				RT_SUCCESS			-	everything OK
//				GSPERR_RESET		-	reset out of command
//
//--------------------------------------------------------------------------------------
static int
gsp_settermio()
{
	int status;

	Retval = RT_SUCCESS;
	GSP_LOG(GSPFUNC, ("gsp_settermio() Gremsock = %d, current.c_cflag = %x\n",
								Gremsock, current.c_cflag));
	if (Gremsock < 0) {
		GSP_LOG(GSPERROR, ("gsp_settermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
		return(GSPERR_BAD_ARGUMENT);
	}
	msg.msg[0] = 0;
	switch (((int)current.c_cflag & CBAUD)) {
		case B50:
			msg.msg[1] = 50;
			break;

		case B75:
			msg.msg[1] = 75;
			break;

		case B110:
			msg.msg[1] = 110;
			break;

		case B134:
			msg.msg[1] = 134;
			break;

		case B150:
			msg.msg[1] = 150;
			break;

		case B200:
			msg.msg[1] = 200;
			break;

		case B300:
			msg.msg[0] = 300/256;
			msg.msg[1] = 300%256;
			break;

		case B600:
			msg.msg[0] = 600/256;
			msg.msg[1] = 600%256;
			break;

		case B1200:
			msg.msg[0] = 1200/256;
			msg.msg[1] = 1200%256;
			break;

		case B1800:
			msg.msg[0] = 1800/256;
			msg.msg[1] = 1800%256;
			break;

		case B2400:
			msg.msg[0] = 2400/256;
			msg.msg[1] = 2400%256;
			break;

		case B4800:
			msg.msg[0] = 4800/256;
			msg.msg[1] = 4800%256;
			break;

		case B9600:
			msg.msg[0] = 9600/256;
			msg.msg[1] = 9600%256;
			break;

		case B19200:
			msg.msg[0] = 19200/256;
			msg.msg[1] = 19200%256;
			break;

		case B38400:
			msg.msg[0] = 38400/256;
			msg.msg[1] = 38400%256;
			break;

		default:
			rterrorbox("GSP_SET_TERMIO: INVALID BAUD RATE %X", (int)current.c_cflag & CBAUD);
			GSP_LOG(GSPERROR, ("gsp_settermio() returning = %d\n", FAILURE));
			return GSPERR_BAD_ARGUMENT;
			break;
	}

	switch ((current.c_cflag & CSIZE)) {
		case CS5:
			msg.msg[2] = 5;
			break;

		case CS6:
			msg.msg[2] = 6;
			break;

		case CS7:
			msg.msg[2] = 7;
			break;

		case CS8:
			msg.msg[2] = 8;
			break;

		default:
			rterrorbox("GSP_SET_TERMIO: INVALID CHARACTER LENGTH %X", current.c_cflag & CSIZE);
			GSP_LOG(GSPERROR, ("gsp_settermio() returning = %d\n", GSPERR_BAD_ARGUMENT));
			return GSPERR_BAD_ARGUMENT;
			break;
	}

	if (!(current.c_cflag & PARENB)) {
		msg.msg[3] = NO_PARITY;
	}
	else if ((current.c_cflag & PARODD)) {
		msg.msg[3] = ODD_PARITY;
	}
	else {
		msg.msg[3] = EVEN_PARITY;
	}
		
	if ((current.c_cflag & CSTOPB)) {
		msg.msg[4] = STOP_BITS_2;
	}
	else {
		msg.msg[4] = STOP_BIT_1;
	}
	msg.msg[5] = current.c_ccs[4];

	GSP_LOG(GSPMAX, ("                Baud Rate      = %d\n", msg.msg[0] * 256 + msg.msg[1]));
	GSP_LOG(GSPMAX, ("                Number of bits = %d\n", msg.msg[2]));
	GSP_LOG(GSPMAX, ("                Parity         = %d\n", msg.msg[3]));
	GSP_LOG(GSPMAX, ("                Stop bits      = %d\n", msg.msg[4]));
	GSP_LOG(GSPMAX, ("                Eof            = %d\n", msg.msg[5]));

	status = MySend(GSP_SET_TERMIO, 6); 
	if (status == GSPERR_RESET) {
		Retval = status;
	}
	else if (status != RT_SUCCESS) {
		rterrorbox("GSP_SET_TERMIO: FAILED %d", status);
		Retval = FAILURE;
	}
	GSP_LOG(GSPFUNC, ("gsp_settermio() returning = %d\n", Retval));
	return(Retval);
}

//--------------------------------------------------------------------------------------
//	gsp_read
//
//	Function:	read data from network serial I/O port
//
//	Entry:		outbuff		-	pointer to buffer of data to write
//				numchars	-	number of bytes of data to write
//				eodchars	-	pointer to list of eod characters
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket is incorrect
//				GSPERR_NO_EOD		-	eod specified but did not get it
//				GSPERR_TIMEOUT		-	timed out before all characters received
//				return value from MySend
//
//--------------------------------------------------------------------------------------
static int
gsp_read(char *outbuff, int numchars, char *eodchars)
{
	int		ret;
	int		index = 0;
	char	message[7];

	Retval = RT_SUCCESS;
	GSP_LOG(GSPFUNC, ("gsp_read() Gremsock = %d\n", Gremsock));
	if (Gremsock < 0) {
		GSP_LOG(GSPERROR, ("gsp_read() failed, returning %d\n", GSPERR_BAD_ARGUMENT));
		return GSPERR_BAD_ARGUMENT;
	}

	msg.msg[index++] = (unsigned char)numchars;
	msg.msg[index++] = (char)gsprdinterchartime;
	msg.msg[index++] = (char)gsprdtotaltime;

	GSP_LOG(GSPMAX, ("gsp_read() numchars = %d, gsprdinterchartime = %d, gsprdtotaltime = %d\n",
						 numchars, gsprdinterchartime, gsprdtotaltime));
	GSP_LOG(GSPMAX, ("           "));
	while ((msg.msg[index] = *eodchars++)) {
		GSP_LOG(GSPMAX, ("eod = %x, ", msg.msg[index]));
		index++;
	}
	GSP_LOG(GSPMAX, ("\n"));

	strcpy(message, "failed");
 	ret = MySend(GSP_READ, index);
	if  (ret == RT_SUCCESS ||
		msg.err == GSPERR_NO_EOD ||		// could have numchars but no eod
		msg.err == GSPERR_TIMEOUT) {	// could have < numchars with timeout
		index = rembytesread - (sizeof(int) + sizeof(DWORD) + sizeof(int));
		if (index > 0) {
			memcpy(outbuff, msg.msg, index);	// move the data read
			outbuff[index] = 0;
#ifdef DEBUG
			dump_buffer("Data Read", outbuff);
#endif
#ifdef HISTORY
			memset(history, 0, sizeof(history);
			memcpy(history, msg.msg, index);
			gsp_lastchar = history[index];
			history_size = index;
#endif //HISTORY
			if (ret == RT_SUCCESS) {
				ret = index;
			}
			else {
				ret = msg.err;
			}
			strcpy(message, "passed");
		}
		else {
			ret = msg.err;
		}
	}
	GSP_LOG(GSPFUNC, ("gsp_read() %s, returning = %d\n", message, ret));
	return ret;
}

//--------------------------------------------------------------------------------------
//	gsp_putc
//
//	Function:	write one byte to network serial I/O port
//
//	Entry:		ch	-	character to write
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket is incorrect
//				return value from gsp_write
//
//--------------------------------------------------------------------------------------
static int
gsp_putc(int ch)
{
	char	out;

	GSP_LOG(GSPFUNC, ("gsp_putc() Gremsock = %d\n", Gremsock));
	out = (char) ch;
	return(gsp_write(&out, 1));
}

//--------------------------------------------------------------------------------------
//	gsp_write
//
//	Function:	write data to network serial I/O port
//
//	Entry:		buff	-	pointer to buffer of data to write
//				size	-	number of bytes of data to write
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket is incorrect
//				return value from MySend
//
//--------------------------------------------------------------------------------------
static int
gsp_write(char *buff, int size)
{
	int index = 0;
	Retval = RT_SUCCESS;

	GSP_LOG(GSPFUNC, ("gsp_write() Gremsock = %d\n", Gremsock));
	if (Gremsock < 0) {
		GSP_LOG(GSPERROR, ("gsp_write() returning %d\n", GSPERR_BAD_ARGUMENT));
		return(GSPERR_BAD_ARGUMENT);
	}

	msg.msg[index++] = (char)gspwrttotaltime; // NOTE!!! this value could be 0 !!!
	memcpy(&msg.msg[index], buff, size);
	index += size;
	if (standard_lrc) {
		msg.msg[index++] = gsp_lrc((unsigned char *)buff);
	}
	GSP_LOG(GSPMAX, ("gsp_write() numchars = %d, gspwrttotaltime = %d\n", size, gspwrttotaltime));
#ifdef DEBUG
	dump_buffer("Data to Write", (char *)&msg.msg[1]);    // incase [0] was 0
#endif
	Retval = MySend(GSP_WRITE, index);
	GSP_LOG(GSPFUNC, ("gsp_write() returning = %d\n", Retval));
	return Retval;
}

//--------------------------------------------------------------------------------------
//	gsp_purge
//
//	Function:	flush serial port and return last character if any
//
//	Entry:		none
//
//	Return:		GSPERR_BAD_ARGUMENT	-	socket is incorrect
//				return value from MySend
//
//--------------------------------------------------------------------------------------
static int
gsp_purge()
{
	int index = 0;
	Retval = RT_SUCCESS;

	GSP_LOG(GSPFUNC, ("gsp_purge() Gremsock = %d\n", Gremsock));
	if (Gremsock < 0) {
		GSP_LOG(GSPERROR, ("gsp_purge() returning %d\n", GSPERR_BAD_ARGUMENT));
		return(GSPERR_BAD_ARGUMENT);
	}
	msg.msg[index++] = (char)gspiopurge;
	Retval = MySend(GSP_PURGE, index);
#ifdef HISTORY
	gsp_lastchar = msg.msg[0];
	history[0] = gsp_lastchar;
	history_size = 1;
#endif //HISTORY
	GSP_LOG(GSPFUNC, ("gsp_purge() returning %d\n", Retval));
	return(Retval);
}

//--------------------------------------------------------------------------------------
//	gsp_lrc
//
//	Function:	compute the lrc for a buffer of data
//				This routine is a modified version of one written by
//				Luis F. Quinones of CitiBank Puerto Rico.
//
//	Entry:		ppbuf	-	pointer to the buffer of data
//
//	Return:		computed lrc value
//
//--------------------------------------------------------------------------------------
static unsigned char
gsp_lrc(unsigned char *ppbuf)
{
	unsigned int index = 0,  buflen = 0;
	unsigned char retlrc = 0;

	GSP_LOG(GSPFUNC, ("gsp_lrc() Gremsock = %d\n", Gremsock));
	buflen = strlen((char *)ppbuf);
	for (index = 1; index < buflen; index++) {
		retlrc ^= ppbuf[index];
	}
	GSP_LOG(GSPFUNC, ("gsp_lrc() returning = %x\n", retlrc));
	return(retlrc);
}

//--------------------------------------------------------------------------------------
//	gsp_modemstatus
//
//	Function:	return the modem status values.
//
//	Entry:		none
//
//	Return:		RT_SUCCESS		modem status returned in msg.err
//				see MySend for other errors returned
//
//--------------------------------------------------------------------------------------
static int
gsp_modemstatus()
{
	msg.msg[0] = msg.msg[1] = 0;
	Retval = RT_SUCCESS;
	GSP_LOG(GSPFUNC, ("gsp_modemstatus() Gremsock = %d\n", Gremsock));
	if (Gremsock < 0) {
		GSP_LOG(GSPERROR, ("gsp_modemstatus() failed, returning %d\n", GSPERR_BAD_ARGUMENT));
		return GSPERR_BAD_ARGUMENT;
	}
 	Retval = MySend(GSP_MODEMSTATUS, 0);
	//		The values or lines checked here are:
	//			CTS		MS_CTS_ON  = 0x0010		clear-to-send
	//			DSR		MS_DSR_ON  = 0x0020		data-set-ready
	//			RING	MS_RING_ON = 0x0040		ring indicator
	//			RLSD	MS_RLSD_ON = 0x0080		(receive-line-signal-detect)
	//
	GSP_LOG(GSPFUNC, ("gsp_modemstatus() modem status = 0x%4X, returning = %d\n", msg.err, Retval));
	return Retval;
}

static int
is_not_INT_field(struct RTOBJ *rp, field **fp)
{
	if (rp->obj.object != CSRARG_FLD) {
		return(CSR_FAIL_ARGTYPE);
	}
	*fp = (field *) rp->ptr;
	if ((*fp)->flags.type != FT_INT) {
		return(CSR_FAIL_ARGTYPE);
	}
	return(RT_SUCCESS);
}

//--------------------------------------------------------------------------------------
//	remreaddone
//
//	Function:	MCNAsyncRecv() completion function
//
//	Entry:		hwnd	-	window queue which receives network messages
//				sock	-	socket connection to specified server
//				len		-	size of receive data buffer
//				flags	-	unused
//				userp	-	user defined pointer
//
//	Return:		rembytesread is set according to value of len as follows
//				len < 0 &&
//				len != CANCEL	rembytesread = len	the error value
//
//				len < 0 &&
//				len == CANCEL	rembytesread = 0
//
//				otherwise		rembytesread = len	the number of bytes read
//
//--------------------------------------------------------------------------------------
void
remreaddone(HWND hwnd, SOCKET sock, int len, int flags, void *userp)
{
	int		length;
	char	message[10];

	if (len < 0) {
		if (len != -MCNECANCEL) {
			strcpy(message, "failed");
			msg.cmd = -1;		// to flag error condition
			rembytesread = len;
		}
		else {
			strcpy(message, "cancelled");
			// operation cancelled so no data read
		}
	}
	else {
		length = rembytesread - (sizeof(int) + sizeof(DWORD) + sizeof(int));
		if (length > 0 && userp) {
			memcpy(userp, msg.msg, length);
		}
		strcpy(message, "passed");
		if (Nowait != sock) {
			rembytesread = len;
		}
		else {
			if (msg.err != RT_SUCCESS) {
				fp_nowait->numchars = (short)msg.err;
			}
			else if ((length = len - (sizeof(int) + sizeof(DWORD) + sizeof(int))) > 0) {
				memcpy(fp_nowait->storage, msg.msg, length);
				fp_nowait->numchars = length;
				GSP_LOG(GSPFUNC, ("gsp_read_nw: OK, returning %d\n", length));
			}
			else {
				fp_nowait->numchars = RT_ERROR;
				GSP_LOG(GSPFUNC, ("gsp_read_nw: NOT OK, returning %d\n", length));
			}
			
			Nowait = 0;
			SendMessage(hWnd, WMU_GSPEVENT, GSP_READ_COMPLETE, 0L);
		}
	}
	GSP_LOG(GSPFUNC, ("remreaddone() sock = %d, MCNAsyncRecv %s, rembytesread = %d\n", Gremsock, message, rembytesread));
}

//--------------------------------------------------------------------------------------
//	remconnectdone
//
//	Function:	MCNAsyncConnect() completion function
//
//	Entry:		hwnd	-	window queue which receives network messages
//				sock	-	new socket connection to specified server
//				len		-	error indication
//				flags	-	unused
//				userp	-	user defined pointer
//
//	Return:		Gremsock is set according to value of len as follows
//				len < 0		Gremsock = len			the error value
//				otherwise	Gremsock = sock			the socket number
//
//--------------------------------------------------------------------------------------
void
remconnectdone(HWND hwnd, SOCKET sock, int len, int flags, void *userp)
{
	char	message[7];

	if (len < 0) {
		strcpy(message, "failed");
		Gremsock = INVALID_SOCKET;
	}
	else {
		// Connected to Remote Server
		strcpy(message, "passed");
		Gremsock = sock;
	}
	GSP_LOG(GSPFUNC, ("remconnectdone() sock = %d, MCNAsyncConnect %s connection to Remote Server, len = %d\n", sock, message, len));
}

//--------------------------------------------------------------------------------------
//	MySend
//
//	Function:	send a GSP command to remsrvr and receive the response from remsrvr
//				for that command
//
//	Entry:		cmd		-	is the GSP command
//				sendlen	-	length of any data that is to be sent
//
//	Return:		the following values are returned on exit
//				RT_SUCCESS		-	everything OK
//				NT_ERROR		-	any NT or MCN related error
//				GSPERR_KEY_OUT	-	a valid key was hit
//				GSPERR_TIMEOUT	-	a time out has occured
//				GSPERR_RESET	-	the reset key has been hit
//
//--------------------------------------------------------------------------------------
int MySend(int cmd, int sendlen)
{
	int accept_key, req_no, status;

	accept_key = KEY_INPUT_NOTALLOWED;
 	if (cmd == GSP_READ) {
		accept_key = KEY_INPUT_ALLOWED;
	}

	msg.cmd = cmd;
	msg.err = 0;
	msg.status = 0;
	GSP_LOG(GSPFUNC, ("MySend() cmd = %d, err = %d, status = %d, sendlen = %d\n", msg.cmd, msg.err, msg.status, sendlen));

	sendlen = sizeof(int) + sizeof(DWORD) + sizeof(int) + sendlen;	// Message length doesn't include command.
	if ((status = MCNSend(hWnd, Gremsock, (char *)&msg, sendlen, 0)) != sendlen) {
		GSP_LOG(GSPERROR, ("MySend() MCNSend failed, status %d\n", status));
		msg.err = (DWORD)status;
		GSP_LOG(GSPERROR, ("MySend() returning NT_ERROR\n"));
		return NT_ERROR;
	}
	GSP_LOG(GSPMAX, ("MySend() MCNSend returned %d\n", status));

	// Clear read flag.
	rembytesread = 0;
	if ((req_no = MCNAsyncRecv(hWnd, remreaddone, Gremsock, (char *)&msg, sizeof(msg), 0, 0)) < 0) {
		GSP_LOG(GSPERROR, ("MySend() MCNAsyncRecv failed, req_no = %d\n", req_no));
		msg.err = (DWORD)req_no;
		GSP_LOG(GSPERROR, ("MySend() returning NT_ERROR\n"));
		return NT_ERROR;
	}

	if (cmd == GSP_READ_NW) {
		GSP_LOG(GSPMAX, ("MySend() MCNAsyncRecv worked: req_no = %d\n", req_no));
		nowait_id = req_no;
	}
	GSP_LOG(GSPMAX, ("MySend() MCNAsyncRecv returned request number %d\n", req_no));
	if (Nowait != Gremsock) {
		GSP_LOG(0, ("MySend() while rembytesread is 0, call gsp_wait()\n"));
		while(!rembytesread) {
			status = gsp_wait(accept_key);
			if (status != RT_SUCCESS && !rembytesread) {
				GSP_LOG(GSPERROR, ("MySend() gsp_wait returned %d and rembytesread = 0\n", status));
				if (MCNCancel(nowait_id) == 0) {
					GSP_LOG(GSPERROR, ("MySend() MCNCancel(%d) failed\n", req_no));
				}
				msg.cmd = status;		// so the reset condition gets out
				break;
			}
			else if (status != RT_SUCCESS) {
				GSP_LOG(GSPERROR, ("MySend() gsp_wait returned %d and rembytesread = %d\n", status, rembytesread));
			}
		}
		if (rembytesread < 0) {
			status = rembytesread;
		}
	}
	else {
		status = RT_SUCCESS;
	}
	GSP_LOG(GSPFUNC, ("MySend() cmd = %d, err = %d, status = %d, rembytesread = %d, return %d\n",
						msg.cmd, msg.err, msg.status, rembytesread, status));
	return status;
}

//--------------------------------------------------------------------------------------
//	gsp_wait
//
//	Function:	call rt_wait and wait for either a reset key, any other key, a timeout
//				or an indication that everything is OK
//
//	Entry:		accept_key is a flag indicating whether to accept any key to reset out
//				of the routine
//
//	Return:		the following values are returned on exit
//				RT_SUCCESS		-	everything OK
//				GSPERR_KEY_OUT	-	a valid key was hit and the accept_key flag was set
//				GSPERR_TIMEOUT	-	a time out has occured
//				GSPERR_RESET	-	the reset key has been hit
//
//--------------------------------------------------------------------------------------
static int
gsp_wait(int accept_key)
{
	int	j, retval;
	short	save_suspend;
	HWND	trick;

	save_suspend = RTsuspend_reset;
	RTsuspend_reset = -1;		// set to -1 to handle ESC the way we want it
	trick = hWnd;
	j = rt_wait(trick, 0, 0, PM_NOREMOVE, 0, -1, accept_key);
	RTsuspend_reset = save_suspend;
	retval = RT_SUCCESS;
	if (j == KEY_OUT) {
		if (!(rt_get_nextkey() >= RT_FIRST_NONKEY && rt_get_nextkey() <= RT_LAST_NONKEY) &&
			  rt_get_nextkey() != RT_HOME &&
			  rt_get_nextkey() != RT_PRINTSCREEN &&
			  rt_get_nextkey() != RT_F13) {
			retval = GSPERR_KEY_OUT;
		}
	}
	else if (j == TIMED_OUT || j == RESET_TIMED_OUT || j == KEY_TIMED_OUT) {
		retval = GSPERR_TIMEOUT;
	}
	else if (j == RESET_OUT) {
		retval = GSPERR_RESET;
	}
	else {
		/* successful I/O completion */
		;
	}
#ifdef DEBUGCODEWANTED
	if (Debug) {
		if (retval != RT_SUCCESS) {		// only output info on errors
			GSP_LOG(GSPMIN, ("gsp_wait() returning %d\n", retval));
		}
	}
#endif
	return retval;
}

#ifdef FRANK // Not put in for first release of PPNT.
//--------------------------------------------------------------------------------------
//  Set the file control for this port
//--------------------------------------------------------------------------------------
static int
gsp_setport(int cmd, int arg)
{
	GSP_LOG(GSPFUNC, ("gsp_setport() Gremsock = %d\n", Gremsock));
	Retval = RT_SUCCESS;
	return(Retval);
}
#endif //FRANK


//--------------------------------------------------------------------------------------
//  Produce an ASCII dump, with control characters in English
//--------------------------------------------------------------------------------------

static void
out_char(char ch)
{
	if (ch < 0x20) {
		GSP_LOG(GSPMIN, ("<%s>", ctl_chars[ch & 0x1F]));
	}
	else if (isprint(ch)) {
		GSP_LOG(GSPMIN, ("%c", ch));
	}
	else {
		GSP_LOG(GSPMIN, ("%02X", ch));
	}
	GSP_LOG(GSPMIN, (" "));
}

static void
dump_buffer(char *title, char *t)
{
	int		js = 0;
	GSP_LOG(GSPMIN, ("%s: ", title));
	while (*t) {
		out_char(*t);
		if (++js == 32) {
			GSP_LOG(GSPMIN, ("\n"));
			js = 0;
		}
		++t;
	}
	GSP_LOG(GSPMIN, ("\n"));
}

#ifdef HISTORY
static void
dump_history()
{
	short	is, js = 0;
	char	*hx;

	hx = history;
	GSP_LOG(GSPMIN, ("Dump of last %d characters read\n", history_size));
	for (is = 0; is < history_size; ++is) {
		out_char(*hx++);
		if (hx > (history + sizeof(history))) {
			hx = history;
		}
		if (++js == 32) {
			GSP_LOG(GSPMIN, ("\n"));
			js = 0;
		}
	}
	GSP_LOG(GSPMIN, ("\n"));
}
#endif //HISTORY

#ifdef FRANK
static void
dump_termio(char *header, struct termio *tp)
{
	printf("\n%s\n", header);
	printf("c_iflag = %08X, c_oflag = %08X\n", tp->c_iflag, tp->c_oflag);
	printf("c_cflag = %08X, c_lflag = %08X\n", tp->c_cflag, tp->c_lflag);
	printf(" c_line = %02X,          c_cc = ", tp->c_line);
	hexdump(stdout, NULL, -2, tp->c_cc, 8, 1);
	putchar('\n');
}
#endif //FRANK

/************************************************************************
**
**  File:  hllapiWD  -  hllapi interface to WALL DATA 3270/5250
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.6 $
** $Date: 1996/03/08 19:37:50 $
**
*************************************************************************/

/************************************************************************
**   UPGRADE HISTORY
**
** V1.1   - Fixed a problem with the declaration of the hllapi call
**          variable CSRRTN that caused unpredictable Dr. Watson errors.
**
** V1.2   - Upgraded tracing routines to work faster
**
** V1.3   - Changed interface to WALL DATA HLLAPI to be 16 bit signed
**          shorts, since they have a 16 bit interface on both WINDOWS
**          and NT.	This was done by changing the HLL structure.
*************************************************************************/

#define	HLL_DEBUG

#include "pch.h"

#include <keycodes.h>
#include <typedefs.h>
#include <csrdef.h>
#include <convers.h>
#include <defines.h>
#include <rtprotos.h>
#if defined(R5)
#include <pcldefs.h>        /* PCL/C defines                        */
#else
#include <ofbdefs.h>
#endif
#include "eehllapi.h"

/************************************************************************
**   ERR Messages
************************************************************************/
static char	emsg_00a[] = "(%s): LoadLibrary of %s failed.  Error: %d";
static char	emsg_00b[] = "(%s): GetProcAddress of %s from %s failed.  Error: %d";
static char	emsg_01[] = "(%s): Called with %d arguments, requires 4 arguments";
static char	emsg_02[] = "(%s): Param 1 (%d) - Unsupported HLLAPI function number";
static char	emsg_03[] = "(%s): Param 1 must be CSRARG_FLD(FT_INT) or CSRARG_INT";
static char	emsg_04[] = "(%s): Param 1 - RT_ERROR returned from rtobj2int";
static char	emsg_05[] = "(%s): Param 1 (%d) - Invalid HLLAPI function number";
static char	emsg_06[] = "(%s): get_string_buffer error, fp_str->numchars = %d, len = %d";
static char	emsg_07[] = "(%s): Param %d - RT_ERROR returned from field2int";
static char	emsg_08[] = "(%s): hll_retval - RT_ERROR returned from int2field";
static char	emsg_09[] = "(%s): Param %d must be CSRARG_FLD of type %s";
static char	emsg_09i[] = "FT_INT";
static char	emsg_09s[] = "FT_BULK or FT_ASCII";
static char	emsg_10[] = "(%s): Param %d - RT_ERROR returned from field2int";
static char	emsg_11[] = "(%s): Param %d (%d) greater than internal buffer size (%d)";
static char	emsg_12[] = "(%s): Param %d (%d) greater than string field max length (%d)";
static char	emsg_13[] = "(%s): Param %d (%d) greater than current string field length (%d)";
static char	emsg_14[] = "(%s): hll_length - RT_ERROR returned from int2field";
static char	emsg_15[] = "(%s): Error (%d) returned from Query_Session_Status during Get_PS_Size";

/************************************************************************
**   Defines
************************************************************************/
#define     P_FUNC      0
#define     P_STR       1
#define     P_LEN       2
#define     P_POS       3
#define     HLLBUFLEN   8192

/************************************************************************
**   HLL Struct
************************************************************************/
typedef struct {
	short	func;				/* hllapi function code */
	char	string[HLLBUFLEN];	/* data buffer */
	short	length;				/* data buffer length */
	short	pos;				/* PS position - return code */
} HLL;

/************************************************************************
**   FCB Struct - For entries in "ftbl" (The function Table defined below)
************************************************************************/
typedef struct {
	int				func;				/* hllapi function code */
	unsigned short	flags;				/* function flags (see below definitions) */
	unsigned short	assumed_length;		/* assumed data length (if required) */
	int		(*rtn)(HLL *hll, rtobj *argv[]);  /* rtn to handle spcl cases */
} FCB;

/*--------------- flags definitions ---------------*/
#define		F_DISABLED		0x0000	/* function not available */
#define		F_ENABLED		0x8000	/* function is enabled */
#define		F_GETLEN		0x0001	/* Get Length value from RT field */
#define		F_PUTLEN		0x0002	/* Put Length value back into RT field */
#define		F_GETSTR		0x0004	/* Get String buffer from RT field */
#define		F_PUTSTR		0x0008	/* Put String buffer back into RT field */
#define		F_RONLY			0x0010	/* Assumed length used for str read only */
#define		F_BYPASS		0x0020	/* Bypass main code - execute handler rtn only */
#define		F_SVPSID		0x0040	/* Save first byte in string buf as PSID */
#define		F_CLRPSID		0x0080	/* Clear PSID storage */
#define		F_STRNULLTERM	0x0100	/* String is null terminated (for Get String length) */

#define		F_LENUSED		(F_GETLEN | F_PUTLEN)
#define		F_STRUSED		(F_GETSTR | F_PUTSTR)


/************************************************************************
**   Function Definitions
************************************************************************/
static FCB		*get_function_value( rtobj *optr );
static field	*get_param_field_ptr( rtobj *optr, int param );
static int		get_length_value( short *lenptr, field *fp_len );
static int		get_string_buffer( char *buf, int len, FCB *fcb, field *fp_str );
static int		put_string_buffer( char *buf, int len, field *fp_str );
static int		put_length_value( short len, field *fp_len );
static int		get_ps_size( HLL *hll, rtobj *argv[] );
#ifdef HLL_DEBUG
static int		hllapi_tracing( HLL *hll, rtobj *argv[] );
static void		trace_data( HLL *hll, int out );
static char		*find_func_name( int func );
static void		trace_write( char *fmt, ... );
#endif

/************************************************************************
**   Function Table
**
**   This table contains a list of all the HLLAPI function codes and the
**   routines that are called to handle each one of them.
************************************************************************/
static FCB		ftbl[] = {

#ifdef HLL_DEBUG
/*   -2 */  { -2,		 		// Disable Olivetti Hllapi Tracing
			(F_ENABLED | F_BYPASS),
				0,		hllapi_tracing },
/*   -1 */  { -1, 				// Enable Olivetti Hllapi Tracing
			(F_ENABLED | F_GETLEN | F_GETSTR | F_BYPASS),
				0,		hllapi_tracing },
#endif
/*    0 */  { OEMFUNCTION,
			(F_DISABLED),
				0,		0 },
/*    1 */  { CONNECTPS,
			(F_ENABLED | F_GETSTR | F_SVPSID),
				1,		0 },
/*    2 */  { DISCONNECTPS,
			(F_ENABLED | F_CLRPSID),
				0,		0 },
/*    3 */  { SENDKEY,
			(F_ENABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/*    4 */  { WAIT,
			(F_ENABLED),
				0,		0 },
/*    5 */  { COPYPS,
			(F_ENABLED | F_PUTSTR),
				0,		get_ps_size },
/*    6 */  { SEARCHPS,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_PUTLEN),
				0,		0 },
/*    7 */  { QUERYCURSORLOC,
			(F_ENABLED | F_PUTLEN),
				0,		0 },
/*    8 */  { COPYPSTOSTR,
			(F_ENABLED | F_GETLEN | F_PUTSTR),
				0,		0 },
/*    9 */  { SETSESSIONPARAMETERS,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_PUTLEN),
				0,		0 },
/*   10 */  { QUERYSESSIONS,
			(F_ENABLED | F_GETLEN | F_PUTSTR | F_PUTLEN),
				0,		0 },
/*   11 */  { RESERVE,
			(F_ENABLED),
				0,		0 },
/*   12 */  { RELEASE,
			(F_ENABLED),
				0,		0 },
/*   13 */  { COPYOIA,
			(F_ENABLED | F_GETLEN | F_PUTSTR),
				0,		0 },
/*   14 */  { QUERYFIELDATTRIBUTE,
			(F_ENABLED | F_PUTLEN),
				0,		0 },				
/*   15 */  { COPYSTRTOPS,
			(F_ENABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/*   18 */  { PAUSE,
			(F_ENABLED | F_GETLEN),
				0,		0 },
/*   20 */  { QUERYSYSTEM,
			(F_ENABLED | F_PUTSTR),
				35,		0 },
/*   21 */  { RESETSYSTEM,
			(F_ENABLED | F_CLRPSID),
				0,		0 },
/*   22 */  { QUERYSESSIONSTATUS,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_PUTSTR),
				0,		0 },
/*   23 */  { STARTHOSTNOTIFICATION,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_RONLY),
				6,		0 },
/*   24 */  { QUERYHOSTUPDATE,
			(F_ENABLED | F_GETSTR),
				1,		0 },
/*   25 */  { STOPHOSTNOTIFICATION,
			(F_ENABLED | F_GETSTR),
				1,		0 },
/*   30 */  { SEARCHFIELD,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_PUTLEN),
				0,		0 },
/*   31 */  { FINDFIELDPOSITION,
			(F_ENABLED | F_GETSTR | F_PUTLEN),
				2,		0 },
/*   32 */  { FINDFIELDLENGTH,
			(F_ENABLED | F_GETSTR | F_PUTLEN),
				2,		0 },
/*   33 */  { COPYSTRINGTOFIELD,
			(F_ENABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/*   34 */  { COPYFIELDTOSTRING,
			(F_ENABLED | F_GETLEN | F_PUTSTR),
				0,		0 },
/*   37 */  { DISPLAYCURSOR,
			(F_DISABLED | F_GETSTR),
				1,		0 },
/*   38 */  { DISPLAYPS,
			(F_DISABLED | F_GETSTR),
				1,		0 },
/*   39 */  { DELETEPS,
			(F_DISABLED | F_GETSTR),
				1,		0 },
/*   40 */  { WDSETCURSOR,
			(F_ENABLED),
				0,		0 },
/*   50 */  { STARTKSINTERCEPT,
			(F_DISABLED | F_GETLEN | F_GETSTR | F_RONLY),
				6,		0 },
/*   51 */  { GETKEY,
			(F_DISABLED | F_GETSTR | F_PUTSTR),
				8,		0 },
/*   52 */  { POSTINTERCEPTSTATUS,
			(F_DISABLED | F_GETSTR),
				2,		0 },
/*   53 */  { STOPKSINTERCEPT,
			(F_DISABLED | F_GETSTR),
				1,		0 },
/*   90 */  { SENDFILE,
			(F_DISABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/*   91 */  { RECEIVEFILE,
			(F_DISABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/*   99 */  { CONVERT,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_PUTLEN | F_RONLY),
				2,		0 },
/*  101 */  { CONNECTWINDOWSERVICES,
			(F_DISABLED | F_GETSTR),
				1,		0 },
/*  102 */  { DISCONNECTWINDOWSERVICES,
			(F_DISABLED | F_GETSTR),
				0,		0 },
/*  103 */  { QUERYWINDOWCOORDINATES,
			(F_DISABLED | F_GETLEN | F_GETSTR | F_PUTSTR),
				17,		0 },
/*  104 */  { WINDOWSTATUS,
			(F_DISABLED | F_GETSTR | F_PUTSTR),
				16,		0 },
/*  105 */  { CHANGELISTNAME,
			(F_DISABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/*  106 */  { CHANGEWINDOWNAME,
			(F_DISABLED | F_GETLEN | F_GETSTR),
				0,		0 },
/* 1000 */  { WINEXEC,
			(F_ENABLED | F_GETLEN | F_GETSTR | F_STRNULLTERM),
				0,		0 },
/* 1001 */  { SHOWSESSION,
			(F_ENABLED),
				0,		0 },
/* 1002 */  { GETSESSIONHWND,
			(F_DISABLED),
				0,		0 },
/* 1003 */  { ADVISEHOSTUPDATE,
			(F_DISABLED),
				0,		0 },
/* 1004 */  { RUNMACRO,
			(F_DISABLED),
				0,		0 },
/* 1005 */  { SETSESSIONPARAMEX,
			(F_DISABLED),
				0,		0 },
};
#define		FTBL_MAX	( sizeof(ftbl) / sizeof(FCB) )

/************************************************************************
**   HLLAPI Function Declaration and Pointer Storage
************************************************************************/
typedef	int (WINAPI *CSRRTN)(short *func, char *str, short *len, short *rc);

static CSRRTN	hllapi_rtn;

/************************************************************************
**   Global Storage
************************************************************************/
static char	hll_PS = ' ';
static char	dll_name[] = "EEHLLAPI.DLL";
static char	rtn_name[] = "hllapi";
static char	filenm[] = "hllapiWD";
static char	version[] = "Version 1.2";

/************************************************************************
**   DEBUG Global Storage
************************************************************************/
#ifdef HLL_DEBUG
static FILE		*trace_hdl = 0;
static int		trace_len = 16;
static char		dflt_trace[] = "c:\\temp\\hllapi.trc";
static char		hd1_in[] = "ENTER";
static char		hd1_out[] = " EXIT";
static char		hd2_1st[] = "  str: ";
static char		hd2_nxt[] = "       ";
static char		hd3_in[]  = "   Pos";
static char		hd3_out[] = "Retval";

typedef struct {
	int		fnum;		/* hllapi function code */
	char	*fname;		/* hllapi function name */
} DBGTBL;

DBGTBL		dbg_tbl[] = {
	{ -2,	"TRACING_OFF" },
	{ -1,	"TRACING_ON" },
	{ 0,	"OEMFUNCTION" },
	{ 1,	"CONNECTPS" },
	{ 2,	"DISCONNECTPS" },
	{ 3,	"SENDKEY" },
	{ 4,	"WAIT" },
	{ 5,	"COPYPS" },
	{ 6,	"SEARCHPS" },
	{ 7,	"QUERYCURSORLOC" },
	{ 8,	"COPYPSTOSTR" },
	{ 9,	"SETSESSIONPARAMETERS" },
	{ 10,	"QUERYSESSIONS" },
	{ 11,	"RESERVE" },
	{ 12,	"RELEASE" },
	{ 13,	"COPYOIA" },
	{ 14,	"QUERYFIELDATTRIBUTE" },
	{ 15,	"COPYSTRTOPS" },
	{ 18,	"PAUSE" },
	{ 20,	"QUERYSYSTEM" },
	{ 21,	"RESETSYSTEM" },
	{ 22,	"QUERYSESSIONSTATUS" },
	{ 23,	"STARTHOSTNOTIFICATION" },
	{ 24,	"QUERYHOSTUPDATE" },
	{ 25,	"STOPHOSTNOTIFICATION" },
	{ 30,	"SEARCHFIELD" },
	{ 31,	"FINDFIELDPOSITION" },
	{ 32,	"FINDFIELDLENGTH" },
	{ 33,	"COPYSTRINGTOFIELD" },
	{ 34,	"COPYFIELDTOSTRING" },
	{ 37,	"DISPLAYCURSOR" },
	{ 38,	"DISPLAYPS" },
	{ 39,	"DELETEPS" },
	{ 40,	"SETCURSOR" },
	{ 50,	"STARTKSINTERCEPT" },
	{ 51,	"GETKEY" },
	{ 52,	"POSTINTERCEPTSTATUS" },
	{ 53,	"STOPKSINTERCEPT" },
	{ 90,	"SENDFILE" },
	{ 91,	"RECEIVEFILE" },
	{ 99,	"CONVERT" },
	{ 100,	"ISINITIALIZED" },
	{ 101,	"CONNECTWINDOWSERVICES" },
	{ 102,	"DISCONNECTWINDOWSERVICES" },
	{ 103,	"QUERYWINDOWCOORDINATES" },
	{ 104,	"WINDOWSTATUS" },
	{ 105,	"CHANGELISTNAME" },
	{ 106,	"CHANGEWINDOWNAME" },
	{ 1000,	"WINEXEC" },
	{ 1001,	"SHOWSESSION" },
	{ 1002,	"GETSESSIONHWND" },
	{ 1003,	"ADVISEHOSTUPDATE" },
	{ 1004,	"RUNMACRO" },
	{ 1005,	"SETSESSIONPARAMEX" },
	{ 9999, "???????" },
};
#endif

/************************************************************************
**   CSR_HLLAPI
************************************************************************/
#if defined(R5)
int csr_hllapi(int argc, rtobj *argv[])
#else
XDLL int csr_hllapi(int argc, rtobj *argv[])
#endif
{
	HINSTANCE	llib;
	FCB		*fcb;
	field   *fp_pos;
	field   *fp_str;
	field   *fp_len;
	HLL		hll;
	int		readlen;
	int		savelen;
	int		rc;

#ifdef HLL_DEBUG
	trace_write( "Enter hllapiWD CSR, " );
#endif

// Load library if needed
	if( ! hllapi_rtn ) {
		if ( (llib = LoadLibrary(dll_name)) == NULL ) {
			rc = GetLastError();
			rterrorbox( emsg_00a, filenm, rtn_name, dll_name, rc );
			return(CSR_FAIL_ARGCOUNT);
		}
		if ( (hllapi_rtn = (CSRRTN)GetProcAddress(llib,rtn_name)) == NULL ) {
			rc = GetLastError();
			rterrorbox( emsg_00b, filenm, rtn_name, dll_name, rc );
			return(CSR_FAIL_ARGCOUNT);
		}
	}

// Check for proper number of parameters
	if (argc != 4) {
		rterrorbox( emsg_01, filenm, argc);
		return(CSR_FAIL_ARGCOUNT);
	}

// Go get the function code and verify it
	if ( (fcb = get_function_value( argv[P_FUNC] )) == 0 )
		return(CSR_FAIL_ARGTYPE);
	if ( ! (fcb->flags & F_ENABLED) ) {
		rterrorbox( emsg_02, filenm, fcb->func );
		return(CSR_FAIL_ARGTYPE);
	}       
	hll.func = (short) fcb->func;
#ifdef HLL_DEBUG
	trace_write("function = (%d) %s\n", hll.func, find_func_name(hll.func));
#endif

// Go get the third parameter (LENGTH) next.  We need it to tell how
// big the 2nd parameter (STRING) is.
	if ( fcb->flags & F_LENUSED ) {
		if ( (fp_len = get_param_field_ptr(argv[P_LEN], P_LEN)) == 0 )
			return(CSR_FAIL_FIELDTYPE);
		if ( fcb->flags & F_GETLEN ) {
			if ( (get_length_value( &hll.length, fp_len )) != 0 )
				return(CSR_FAIL_ARGTYPE);
		}
	}

// If there is an assumed buffer length (such as length=1 on a connect
// presentation space) then load it here.  This option usually is
// mutually exclusive from GET_LEN
	readlen = (int)hll.length;
	if ( fcb->assumed_length ) {
		readlen = fcb->assumed_length;
		if ( ! (fcb->flags & F_RONLY) )
			hll.length = fcb->assumed_length;
	}

// Go get the 2nd parameter (STRING) using either the supplied length
// or the assumed length.  Or, verify that the string is large enough
// to handle the amount of data to be written to it (assumed or otherwise)
	if ( fcb->flags & F_STRUSED ) {
		if ( (fp_str = get_param_field_ptr(argv[P_STR], P_STR)) == 0 )
			return(CSR_FAIL_FIELDTYPE);
		if ( fcb->flags & F_GETSTR ) {
			if ( (get_string_buffer(hll.string,readlen,fcb,fp_str)) != 0 )
				return(CSR_FAIL_ARGTYPE);
		}
	}

// The fourth param must always be supplied as an F_INT field since it
// returns the return code.  So, we verify it and load it every time.
	if ( (fp_pos = get_param_field_ptr(argv[P_POS], P_POS)) == 0 )
		return(CSR_FAIL_FIELDTYPE);
	if ( field2int(fp_pos, &rc) == RT_ERROR ) {
		rterrorbox( emsg_07, filenm, P_POS+1 );
		return(CSR_FAIL_ARGTYPE);
	}   
	hll.pos = (short) rc;
								 
// If this function needs some special attention, go call a handler
// routine at this point.  These routines may alter the hll variables.
	if ( fcb->rtn )	{
		rc = (*fcb->rtn)(&hll,argv);
		if ( (fcb->flags & F_BYPASS) || rc != 0 ) {
			rc = (int) hll.pos;
			if ( int2field(rc, fp_pos) == RT_ERROR ) {
				rterrorbox( emsg_08, filenm );
				return(CSR_FAIL_ARGTYPE);
			}   
			return( rc );
		}
	}

#ifdef HLL_DEBUG
// Dump input parameters to Trace File if enabled
	trace_data( &hll, 0 );
#endif

// Make the hllapi call. Save off the length byte in case we need to write
// data back out to the string buffer.

	savelen = (int) hll.length;
	(*hllapi_rtn)( &hll.func, hll.string, &hll.length, &hll.pos );

#ifdef HLL_DEBUG
// Dump output parameters to Trace File if enabled
	rc = 1;
	trace_data( &hll, rc );
#endif

// Save or clear the hll_PS byte if we connected or disconnected
	if ( fcb->flags & F_SVPSID )
		hll_PS = hll.string[0];
	if ( fcb->flags & F_CLRPSID )
		hll_PS = (char)' ';

// Put data back to the RT string buffer if needed
	if ( fcb->flags & F_PUTSTR ) {
		if ( put_string_buffer(hll.string,savelen,fp_str) != 0 )
			return(CSR_FAIL_ARGTYPE);
	}

// Put data back to the RT length field if needed
	if ( fcb->flags & F_PUTLEN ) {
		if ( put_length_value(hll.length,fp_len) != 0 )
			return(CSR_FAIL_ARGTYPE);
	}

// The return code is always passed back to the RT application
	rc = (int) hll.pos;
	if ( int2field(rc, fp_pos) == RT_ERROR ) {
		rterrorbox( emsg_08, filenm );
		return(CSR_FAIL_ARGTYPE);
	}   

#ifdef HLL_DEBUG
	trace_write( "-----Exit hllapiWD CSR----\n\n" );
#endif
	return( 0 );
}
/************************************************************************
**   Get_Function_Value
************************************************************************/
static FCB	*get_function_value( rtobj *optr )
{
	int		i, func;
	field	*fp = 0;

// Validate that parameter 0 (hllapi function number) is either a
// field of type INT or a constant INT
	if ( optr->obj.object == CSRARG_FLD )
		fp = (field *)optr->ptr;

	if ( optr->obj.object != CSRARG_INT && ( fp == 0 || fp->flags.type != FT_INT ) ) {
		rterrorbox(emsg_03, filenm);
		return( 0 );
	}       

// Convert the function number
	if ( rtobj2int(optr, &func) == RT_ERROR ) {
		rterrorbox(emsg_04, filenm);
		return( 0 );
	}       

// Validate the function number
	for ( i=0; i < FTBL_MAX; i++ )
		if ( func <= ftbl[i].func ) break;

	if ( i == FTBL_MAX || func != ftbl[i].func ) {
		rterrorbox( emsg_05, filenm, func );
		return( 0 );
	}       
	return( &ftbl[i] );
}

/************************************************************************
**   Get_Param_Field_Ptr
************************************************************************/
static field	*get_param_field_ptr( rtobj *optr, int param )
{
	field	*fp;

// Validate the parameter is of the correct field type
	if ( optr->obj.object == CSRARG_FLD ) {
		fp = (field *)optr->ptr;
		if ( param == P_STR ) {
			if ( fp->flags.type == FT_ASCII || fp->flags.type == FT_BULK )
				return( fp );
		} else {
			if ( fp->flags.type == FT_INT )
				return( fp );
		}
	}
	rterrorbox( emsg_09, filenm, param+1, (param == P_STR) ? emsg_09s : emsg_09i );
	return( 0 );
}

/************************************************************************
**   Get_Length_Value
************************************************************************/
static int	get_length_value( short *lenptr, field *fp_len )
{
	int		len;
	if ( field2int(fp_len, &len) == RT_ERROR ) {
		rterrorbox( emsg_10, filenm, P_LEN+1 );
		return(CSR_FAIL_ARGTYPE);
	}
	if ( HLLBUFLEN < len ) {
		rterrorbox( emsg_11, filenm, P_LEN+1, len, HLLBUFLEN );
		return(CSR_FAIL_ARGTYPE);
	}
	*lenptr = (short) len;
	return( 0 );
}
/************************************************************************
**   Get_String_Buffer
************************************************************************/
static int	get_string_buffer( char *buf, int len, FCB *fcb, field *fp_str )
{

// If string is defined as null terminated, ignore all length values
// and just read to null value.
	if ( fcb->flags & F_STRNULLTERM ) {
		lstrcpy( buf, fp_str->storage );
	} else {
		if ( fp_str->numchars < len ) {
			rterrorbox( emsg_06, filenm, fp_str->numchars, len );
			return(CSR_FAIL_ARGTYPE);
		}
		CopyMemory( buf, fp_str->storage, len );
	}
	return( 0 );
}

/************************************************************************
**   Put_String_Buffer
************************************************************************/
static int	put_string_buffer( char *buf, int len, field *fp_str )
{

// Make sure a ASCII field type ends with any null in the middle
// of the returned data
	if ( fp_str->flags.type == FT_ASCII ) {
		buf[len] = 0;			// Terminate string at max length
		len = lstrlen(buf);		// Get string length to first null
	}

// At this point, verify (if needed) that the RT string buffer will
// be large enough to handle data incoming from the hllapi call.
	if ( fp_str->length < len ) {
		rterrorbox( emsg_12, filenm, P_LEN+1, len, fp_str->length );
		return(CSR_FAIL_ARGTYPE);
	}

// Copy the data into field storage area
	if ( (fp_str->numchars = len) == 0 ) {
		ZeroMemory(fp_str->storage,fp_str->length);
		fp_str->flags.dataentered = 0;
	} else {
		MoveMemory((char *)fp_str->storage,buf,len);
		fp_str->flags.dataentered = 1;
		if ( fp_str->flags.type == FT_ASCII ) {
			fp_str->storage[len] = 0;	// Null terminate the ASCII field
		}
	}

	return( 0 );
}

/************************************************************************
**   Put_Length_Value
************************************************************************/
static int	put_length_value( short len, field *fp_len )
{
	if ( int2field((int)len, fp_len) == RT_ERROR ) {
		rterrorbox( emsg_14, filenm );
		return(CSR_FAIL_ARGTYPE);
	}
	return( 0 );
}
/************************************************************************
**   Get_PS_Size
************************************************************************/
static int	get_ps_size( HLL *hll, rtobj *argv[] )
{
	short	Hfunc;
	char	Hstring[20];
	short	Hlength;
	short	Hpos;

// Here is some special code.  When we copy the PS back into the
// RT field, we need to know how much data was gathered.  Although
// the PS is usually 1920 bytes, it may be larger depending on whether
// EAB is set or if the emulation is running at a different screen
// size.  So, we make a call to QUERY_SESSION_STATUS to find out
// how many bytes we will get when we call COPY_PS.

	Hfunc = 22;				/* QUERY SESSION STATUS function code */
	Hstring[0] = hll_PS;	/* PS short name */
	Hlength = 18;			/* length of data to receive */

	Hpos = 0;
	(*hllapi_rtn)( &Hfunc, Hstring, &Hlength, &Hpos );
	if ( Hpos ) {
		rterrorbox( emsg_15, filenm, Hpos );
		hll->pos = Hpos;
		return(CSR_FAIL_ARGTYPE);
	}   
	hll->length = *((WORD *)(&Hstring[11]));	/* No. of rows */
	hll->length *= *((WORD *)(&Hstring[13]));	/* No. of columns */
	if ( Hstring[10] )							/* if EAB on... */
		hll->length *= 2;			/* add attribute byte for each byte */

	return( 0 );
}

#ifdef HLL_DEBUG
/************************************************************************
**   Hllapi_Tracing
************************************************************************/
static int	hllapi_tracing( HLL *hll, rtobj *argv[] )
{
	char	*tracenm = dflt_trace;

	if ( hll->func = -1 ) {		// TRACING ON
		trace_len = (hll->pos > 16) ? hll->pos : 16;
		if ( trace_hdl == 0 ) {
			if ( hll->length > 0 ) {
				tracenm = hll->string;
				tracenm[hll->length] = 0;
			}
			if ( (trace_hdl = fopen(tracenm,"w")) == 0 ) {
				rterrorbox("(%s): Error opening Trace Filename (%s)", filenm, tracenm );
			}
		}
	} else {					// TRACING OFF
		fclose( trace_hdl ); 	
		trace_hdl = 0;
	}
	hll->pos = 0;
	return( 0 );
}
/************************************************************************
**   Trace_Data
************************************************************************/
static void	trace_data( HLL *hll, int out )
{
	char	outbuf[100];
	char	*bhead;
	char	*xptr, *cptr;
	char	*inbuf;
	int		inlen;
	unsigned char	uc;
	int		i;

	if ( ! trace_hdl ) return;

	trace_write("%s FUNCTION:%4d   Length=%d  %s=%d\n",
		((out)?hd1_out:hd1_in), hll->func, hll->length,
		((out)?hd3_out:hd3_in),hll->pos );

	bhead = hd2_1st;
	inlen = trace_len;
	inbuf = hll->string;
	while( inlen ) {
		FillMemory( outbuf, 100, (char)' ' );
		CopyMemory( outbuf, bhead, lstrlen(bhead) );
		xptr = &outbuf[7];
		cptr = &outbuf[58];
		outbuf[57] = outbuf[75] = '|';
		outbuf[76] = '\n';
		outbuf[77] = 0;
		for( i=0; i<16 && inlen != 0; i++, inbuf++, inlen-- ) {
			uc = (unsigned char)*inbuf >> 4;
			*xptr++ = (uc < 10) ? (uc + '0') : (uc + 'a' - 10);
			uc = (unsigned char)*inbuf & 0x0f;
			*xptr++ = (uc < 10) ? (uc + '0') : (uc + 'a' - 10);
			xptr++;

			*cptr++ = ( *inbuf < 0x20 || *inbuf > 0x7e ) ? '.' : *inbuf;
			if ( i == 7 ) {
				xptr++;
				cptr++;
			}
		}
		if ( i )
			trace_write("%s", outbuf );
		bhead = hd2_nxt;
	}
}
/************************************************************************
**   Find_Func_Name
************************************************************************/
static char		*find_func_name( int func )
{
	DBGTBL		*dp;

	for ( dp = dbg_tbl; dp->fnum != 9999; dp++ ) {
		if ( dp->fnum == func ) break;
	}
	return( dp->fname );
}

/************************************************************************
**   Trace_Write
************************************************************************/
static void		trace_write( char *fmt, ... )
{
	va_list		ap;

	if ( trace_hdl ) {
		va_start(ap, fmt);
		vfprintf( trace_hdl, fmt, ap );				/* put in msg in buffer */
		fflush( trace_hdl );
	}
}

#endif

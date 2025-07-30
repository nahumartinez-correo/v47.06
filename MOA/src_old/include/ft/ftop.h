
/*
 *	copyright (c) 1989 ISC Systems Corporation
 */

# ifndef	_Ffttypes
#	define	_Ffttypes
#	define	FTOP_H

#ifndef XPDATA
#define XPDATA
#endif

#ifndef _Fftdes
#include	<ft/ftdes.h>
#endif
#ifndef FTVAR_H
#	include	<ft/ftvar.h>
#endif

/*
 *	function codes
 *	these functions are the interface with the server
 */
enum {
	SVSETUP=1,
	SELECT,
	END_SELECT,
	SVINSERT,
	REPLACE,
	UPDATE_FLDS,
	FIND_FIRST,
	FIND_LAST,
	FIND_NEXT,
	FIND_PREV,
	FIND_N,
	CANCEL_SCAN,
	SVDELETE,
	SVWHO,
	UNLOCK,
	LOCK_TABLE,
	UNLOCK_TABLE,
	LIST_FILES,
	LIST_TABLES,
	LIST_SV_NAMES,
	RESET_LOG,
	COMPRESS,
	CREATE_TABLE,
	ALTER_TABLE,
	RENAME_TABLE,
	COPY_TABLE_DEF,
	DROP_TABLE,
	CREATE_INDEX,
	DROP_INDEX,
	CDS_COMMIT_WORK,
	CDS_2_PHASE_COMMIT,
	CDS_ROLLBACK,
	CDS_TABLE_SIZE,
	FETCH_AGAIN,
	CDSYNC,
	UPDATE_ALL,
	SVDELETE_ALL,
	CDS_PROJECT,
	SVSTOP,
	BECOME_PRIMARY,
	CDS_COMPLETE_PHASE2,
	CDS_JOIN,
	CDS_SELECT_FETCH,
	CDS_TEST_CONNECTION,
	CDS_BATCH_INSERT,
	CDS_TABLE_SIZES,
	CDS_OOB,
	CDS_UNION,
	OPEN_CDS_CAPTURE,
	CLOSE_CDS_CAPTURE,
	CDS_PREPARE,
	CDS_PREPARED,
	CDS_COMPLETED_PHASE2,
	CDS_PARTNER_SETUP,
	CDS_SetConnectOption,
	CDS_GetConnectOption,
	CDS_SetStmtOption,
	CDS_GetStmtOption,
	SHADOW_SETUP,
	SHADOW_VERIFY,
	SHADOW_WATCH,
	BE_SHADOW,
	INVALID_REQUEST		/* requests >= INVALID_REQUEST are errors */
};

# define	F_NOFUNC	0	/* for the 0'th and last entry */

# define	F_START		1

# define	F_END	    INVALID_REQUEST - 1

/*
 *	data type codes
 */

# define	F_DATATYPESTART	30	/* the first one */

#ifdef NO_ENUM
# define	F_CHAR		30	/* (char) */
# define	F_INT		31	/* (int)  */
# define	F_LONG		32	/* (long) */
# define	F_ADDR		33	/* (addr) */
# define	F_STRING	34	/* (string) */
# define	F_ARRAY		35	/* (array of addr) */
# define	F_NULL		36	/* (null entry)      */
#else
enum {F_CHAR=30, F_INT, F_LONG,F_ADDR,F_STRING,F_ARRAY,F_NULL};
#endif
# define	F_DATATYPEEND	36	/* the last one  */

/*
 * 	function arguments
 */

# define	ZMAXARGS	5		/* # of function arguments */
# define	ZMAXPARS	(ZMAXARGS + 1)	/* # arguments + result */

/*
 *	function attribute type codes
 */


# define	F_ARGTYPESTART	100	/* first available argytpe */

# define	INTVAL		100	/* the argument is an int */
# define	LONGVAL		101	/* the argument is an int */
# define	CHARVAL		102	/* it is a char */
# define	ADDR		103	/* it is a pointer */
# define	STRING		104	/* it is a string */
# define	CDS_ARRAY_PARM		105	/* it is an array */
# define	DTPAR		106	/* it is a dtpar */

# define	F_ARGTYPEEND	106	/* last available argtype */


# define	F_ARGWHERESTART	80

# define	LOCAL		80	/* argument in client address space */
# define	REMOTE		81	/* argument in server address space */
# define	NOWHERE		82	/* corresponds to the result entry
					   of a void or noret function */
# define	F_ARGWHEREEND	82


# define	F_ARGIOSTART	60

# define	INPARG		60	/* input argument */
# define	OUTARG		61	/* output argument */
# define	RETURN_VAL	62	/* return value */
# define	NORET		63	/* function doesn't return */
# define	CDS_VOID		64	/* function returns nothing */

# define	F_ARGIOEND	64


# define	FUNCNIL		(fgpar (*)())0	/* a nil function ptr as found
						   in the definition of fdes */

# define	FCAST		fgpar (*)()	/* use to cast ptr to function
						   returning any valid data type
						   to ptr to function returning
						   fgret */

#ifndef NDEBUG
# define	fcodeok(ftab, code)	( ( (code) >= F_START && \
				    (code) <= F_END)  && \
				    ftab[code].fid == (code) ? \
					true : false )

# define	ftypeok(type)	( ( (type) >= F_ARGTYPESTART && \
				    (type) <= F_ARGTYPEEND) ? \
					true : false )

# define	fwhereok(where)	( ( (where) >= F_ARGWHERESTART && \
				    (where) <= F_ARGWHEREEND) ? \
					true : false )


# define	fiook(io)	( ( (io) >= F_ARGIOSTART && \
				    (io) <= F_ARGIOEND) ? \
					true : false )
#else
# define	fcodeok(ftab, code)	true
# define	ftypeok(type)	true
# define	fwhereok(where)	true
# define	fiook(io)	true

#endif
# define	fgetname(ftab, code)	( ftab ? ftab[code].fname : (code == 1 ? "client setup" : "unknown!"))

# define	isvoid(ftab, code)	( ftab[code].fpar[0].io == CDS_VOID )

# define	isnoret(ftab, code)	( ftab[code].fpar[0].io == NORET )

# endif

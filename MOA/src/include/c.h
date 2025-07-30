/* c.h   AN AID to WRITING PORTABLE C CODE
 *       ALSO AN AID TO WRITING MAINTANABLE CODE
 *        In this regard, the LOCAL_ and GLOBAL_ macros are especially helpful.
 *
 *	copyright (c) 1989	ISC Systems Corporation
 */

#ifndef  _Hc
#	define  _Hc

#ifndef THIS_SYS_H
#   include  <this_sys.h>
#endif
#ifndef TYPEDEFS_H
#   include  <typedefs.h>
#endif

/*********** OPTIONS FOR CDS COMPILATION ******/
#ifdef DEBUGCODEWANTED
#  ifndef DEBUG
#	define DEBUG
#  endif
#endif
#if (defined(DEBUG) || defined(DEBUGCODEWANTED))
#define	CDSVDEBUG
#define CDSVSTATS
#else
#	ifndef NDEBUG
#		define NDEBUG
#	endif
#endif

/****  options for maximum performance are:
*#define NDEBUG
*
** and DEBUG, CDSVDEBUG, and CDSVSTATS should not be defined *
***********************************************************/

/*
 *	classes of procedures and variables
 */

# define  GLOBAL_PROC		/* global procedure declaration */
# define  GLOBAL_VAR		/* global variable declaration */
#ifdef ANSI_C_COMPILER
# define  GLOBAL_CONST	const	/* global constant declaration */
# define  LOCAL_PROC	static	/* local procedure declaration */
# define  LOCAL_VAR	static	/* local variable declaration */
# define  LOCAL_CONST	static const	/* local constant declaration */
#else
# define  GLOBAL_CONST	static	/* global constant declaration */
# define  LOCAL_PROC	static	/* local procedure declaration */
# define  LOCAL_VAR	static	/* local variable declaration */
# define  LOCAL_CONST	static 	/* local constant declaration */
#endif
# define  STATIC	static	/* static declaration within a procedure */

# define  global_proc	extern	/* global procedure reference */
#ifdef ANSI_C_COMPILER
# define  global_const	extern const	/* global constant reference */
#else
# define  global_const	extern 	/* global constant reference */
#endif
# define  global_var	extern	/* global variable reference */
# define  local_proc	static	/* local procedure reference */
				/* the reference is implied */
# define  local_var	extern	/* local variable reference */
				/* this tends to cause problems and */
				/* the whole reference may have */
				/* to be removed */
#ifdef ANSI_C_COMPILER
# define  local_const extern const	/* local constant reference */
#else
# define  local_const extern /* local constant reference */
#endif
/*
 *	register declarations
 */

# define  regpar	register	/* register parameter */
# define  regvar	register	/* register variable */

/*
 *	commands
 */

# define  cycle		for (;;)
# define  elif		else if

/*
 *	types
 */


#if (_MSC_VER >= 800)
#define SYSTEM_BOOLEAN
#endif
//#ifndef		SYSTEM_BOOLEAN
// ----------------------------------------------------
// The following is bogus, and needs to be corrected...
// ----------------------------------------------------
//typedef  int		boolean;	/* true or false */
//#define  true		((boolean) 1)
//#define  false		((boolean) 0)
//#endif
#ifndef true
# define  true		1
# define  false		0
#endif

typedef char	boolchar;		/*boolean in a byte */	

typedef  int		pos;	/* indicates non-negative usage */

/*
 *	constants
 */

# define  SYSERR	(-1)

# define  CHARNIL	((char *) 0)

# define  BASEOCTAL	8
# define  BASEDECIMAL	10
# define  BASEHEX	16

# define  BITSPERBYTE	8	/* if you change this, you're in trouble */
# define  BYTEMASK	0x0ff
# define  TWOBYTEMASK	0x0ffff
# define  BYTEMSB	0x080
# define  BYTENOMSB	0x07f

# define  ZPROGUNAME	12	/* max length of user name */

/*
 *	functions
 */

# define  EOS			'\0'
# define  EMPTYSTR		""
# define  strnull(x)		((x)[0] == EOS)
# define  setnull(x)		((x)[0] = EOS)


# define  ptrnil(x)		((x) == 0)
# define  setnil(x)		((x) = 0)

# define  tof(x)		((x) ? "true" : "false")

/*
 *	unix conventions
 */

# define  STDIN		0
# define  STDOUT	1
# define  STDERR	2

/*
 *	change this once every 100 years
 */

# define  CENTURY	1900

/*
 *	system parameters
 *
 *	also for redefining any of the above definitions
 *	also for declaring VOID_KEYWORD if appropriate
 */

/* Unix is the default OS */

#ifndef  OS_PCDOS
#ifndef  OS_VMS
# define  OS_UNIX
#endif
#endif

/*
 *	procedure types and declarations
 */

#ifdef  VOID_KEYWORD
# define  call		(void)	/* ignore any function return values */
#else
# define  void		int	/* procedure, no function value */
# define  call			/* ignore any function return values */
#endif

# define  noret		void	/* procedure does not return */

/*
 *	bytes, words and addresses
 */

typedef  int		word;

typedef  char		*addr;
typedef  void		(*taddr)();

#ifndef CDS_RECLEN
typedef	 unsigned short	reclen;	/* size of record length specifier */
#define CDS_RECLEN
#endif

# define  ADDRNIL	((addr) 0)

# define  addroff(p, n)		((addr)(p) + (n))
# define  addrinc(p, n)		(*(addr *)&(p) = (addr)(p) + (n))

#define	is_digit(c) ((c) >= '0' && (c) <= '9')
#ifndef NULL
#define NULL		0
#endif

#ifdef NT
typedef int	pid_t;
#endif

#endif

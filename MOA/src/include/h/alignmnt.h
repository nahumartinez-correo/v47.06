#ifndef  ALIGNMNT_H
#	define ALIGNMNT_H

# include  <h/system.h>

/*
 *	alignment on 1-byte boundary; i.e. no alignment requirements
 */

#ifdef  ALIGN_1

# define  ALIGNANY	0

# define  ALIGNBYTE	0
# define  ALIGNWORD	0

# define  ALIGNCHAR	0
# define  ALIGNSHORT	0
# define  ALIGNINT	0
# define  ALIGNLONG	0
# define  ALIGNFLOAT	0
# define  ALIGNDOUBLE	0

#endif

/*
 *	alignment on 2-byte boundary
 */

#ifdef  ALIGN_2

# define  ALIGNANY	1

# define  ALIGNBYTE	0
# define  ALIGNWORD	1

# define  ALIGNCHAR	0
# define  ALIGNSHORT	1
# define  ALIGNINT	1
# define  ALIGNLONG	1
# define  ALIGNFLOAT	1
# define  ALIGNDOUBLE	1

#endif

/*
 *	alignment on 4-byte boundary
 */

#ifdef  ALIGN_4

# define  ALIGNANY	3

# define  ALIGNBYTE	0
# define  ALIGNWORD	3

# define  ALIGNCHAR	0
# define  ALIGNSHORT	1
# define  ALIGNINT	3
# define  ALIGNLONG	3
# define  ALIGNFLOAT	3
# define  ALIGNDOUBLE	3

#endif

/*
 *	alignment on 8-byte boundary
 */

#ifdef  ALIGN_8

# define  ALIGNANY	7

# define  ALIGNBYTE	0
# define  ALIGNWORD	3

# define  ALIGNCHAR	0
# define  ALIGNSHORT	1
# define  ALIGNINT	3
# define  ALIGNLONG	3
# define  ALIGNFLOAT	3
# define  ALIGNDOUBLE	7

#endif


# define  RALIGNANY	7

# define  RALIGNBYTE	0
# define  RALIGNWORD	3

# define  RALIGNCHAR	0
# define  RALIGNSHORT	1
# define  RALIGNINT	3
# define  RALIGNLONG	3
# define  RALIGNFLOAT	3
# define  RALIGNDOUBLE	7


/*
 *	alignment function
 */

# define  align(x, type)	(((int)(x) + (type)) & ~(type))
# define  alignlong(x, type)	(((long)(x) + (type)) & ~(long)(type))

#ifdef  ADDRISINT
# define  alignaddr(x, type)	(addr)align (x, type)
#else
# define  alignaddr(x, type)	(addr)alignlong (x, type)
#endif


#endif

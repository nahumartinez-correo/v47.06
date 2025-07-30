/**********************************************************************/
/*                                                                    */
/*     Defines used globally everywhere in the package.  The more     */
/*     specific defines are found in files like 'pcldefines.h'.       */
/*                                                                    */
/**********************************************************************/

#ifndef DEFINES_H
#define DEFINES_H

#ifndef NULL
#  define NULL		(0)		/* null string pointer */
#endif

#ifndef TRUE
#  define TRUE		(1)
#endif

#ifndef FALSE
#  define FALSE		(0)
#endif

#define RT_ERROR		(1)
#define RT_SUCCESS		(0)

#define IDSIZE		14		/* max length of a name in chars */

#define MAXPCLOBJ	25000	/* max size of a pcl object record */
#define MAXPCLSRC	25000	/* max size of a pcl source module */
#define MAXEXPRBUF	250		/* max number of chars in expression buffer	*/

/* Transfer modes, used by font and pattern operations. */
/* Boolean Source Transfer modes.  All systems, mono/color. */

#define srcCopy		0	/* Copy from source into window. */
#define srcOr		1	/* Or from source into window. */
#define srcXor		2	/* Xor from source into window. */
#define srcBic		3	/* Clear bits set in source in window. */
#define notSrcCopy	4	/* Copy inverse of source into window. */
#define notSrcOr	5	/* Or inverse of source into window. */
#define notSrcXor	6	/* Xor inverse of source into window. */
#define notSrcBic	7	/* Clear bits with zero in source in window. */

#endif

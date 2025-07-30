
/* CLCONN.H   - CLient CONNECTion control block
 * formerly CLCONNECT.H   - CLient CONNECTion control block
 *
 *  See also ../cds/include/x/cmxbuf.h
 *	copyright (c) 1989	ISC Systems Corporation
 */

#ifndef	CLCONNECT_H
#	define	CLCONNECT_H
#  ifndef CMXBUF_H

#ifndef CDSLIMITS_H
# include	<cdslmts.h>
#endif
#ifndef DATAMAN_H
# include	<dataman.h>
#endif
#ifndef CLENV_H
#include "clenv.h"
#endif
#ifndef  _Fftdes
#include "ft\ftdes.h"
#endif


#pragma pack (push, 2)
typedef struct clconnect {
	struct	clconnect	*next;	/* list manager structure */
	struct	clconnect	*prev;
	short	flags;				/* list manager */
#include "clccb.h"
	void *fill2[5];
	short fill3;
} clconnect;
#pragma pack (pop)

# endif
#endif

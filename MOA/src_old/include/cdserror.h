/************************************************************************
*
*               CDSERROR.H
*
*   This include file contains the error codes associated with the file
*   access portion of the Central Data Server. The errors are ordered
*   in increasing severity. Some codes are in fact not errors but
*   identify failures which are tolerable.
*
*   These error codes are placed in the global variable cds_errno
*
**  (c) Copyrighted by ISC System Corp., 1989          All rights reserved.
*
************************************************************************/

#ifndef CDSERROR_H
#define CDSERROR_H

#ifndef XPDATA
#define XPDATA
#endif
#ifndef CLENV_H
#include "clenv.h"
#endif

extern    int    cds_errno;
XPDATA    int    Client_type;
XPDATA    int    debugsw;
XPDATA	const short Interface_Version;
extern    void    *cds_set_err(int, ... );    /* for CDS internal use only */
extern    void    cds_set_warn(int, ... );    /* for CDS internal use only */
extern	  void	  *clsetswerr(clenv *,int, ... );	/* for CDS internal use only */
extern    char    *cds_errmsg(void);    /*returns ptr to error description text*/
extern	  void	  cds_save_err( void );	/* save the error message */
extern	  void	  cds_restore_err(void); /* restore err msg from saved msg */
extern	void	cds_append_error (char *add);
extern	int	int_from_errmsg(void);
extern  void putout(const char *format, ...); /* put message to stderr, then add nl */
#include <cdserdef.h>

#endif	/*ifndef CDSERROR_H */

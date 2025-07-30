/*$
**              CDSFUNCS.H
** 
**  This include file contains the function prototypes for most of the
**  functions globally available from CDS,  the Central Data Server.
** 
**  If one of these functions does not succeed, an error code will
**  be placed in the global variable cds_errno.  Refer to cdserror.h
**
**  (C) Copyright by ISC System Corp., 1989          All rights reserved.
**
*/

#ifndef CDSFUNCS_H
#define CDSFUNCS_H
#ifndef _Hc
#	include "c.h"
#endif
#ifndef CDSLIMITS_H
#include "cdslmts.h"
#endif
#ifndef CLENV_H
#include "clenv.h"
#endif
#ifndef CLRB_H
#include "clrb.h"
#endif

#ifndef CDSDEFS_H
#include "cdsdefs.h"
#endif
#ifndef TYPEDEFS_H
# define bool BOOL
#endif

#ifndef CDSMACRO_H
#include "cdsmacro.h"
#endif

#if defined(SELECT_STRUCT_DEFINED) || defined(SVSVPROC_H)
#	ifndef CDS_H			// server side
#		include "cds.h"
#	endif
#	include "svcdsfn.h"		// server side
#else
#	include "clcdsenv.h"	// client side
#	include "clcdsfn.h"
#endif

#ifndef XPDATA
#define XPDATA extern
#endif
XPDATA const char Ascending[], Descending[];

int cds_udp_port(char *project_name, char *service_name_suffix, char *udp_service_name /*out*/);
int	Cds_Delete_All(cdsselect *context, luw_flag luw);
void *Cds_End_Select(cdsselect *context);
clrb *cdsnwEnd_Select(cdsselect *context);
void *Cds_Insert(cdsselect *context, void *buffer, lock_flag lock, luw_flag luw);
clrb *cdsnwInsert(cdsselect *context, void *buffer, lock_flag lock, luw_flag luw);
void *Cds_Replace(cdsselect *context, void *buffer, lock_flag lock,	luw_flag luw);
clrb *cdsnwReplace(cdsselect *context, void *buffer, lock_flag lock,
							luw_flag luw);
void *Cds_Update_Flds(cdsselect *context, void *buffer, lock_flag lock,
							luw_flag luw);
clrb *cdsnwUpdate_Flds(cdsselect *context, void *buffer, lock_flag lock,
							luw_flag luw);
int	Cds_Update_All(cdsselect *context, void *buffer, luw_flag luw);
clrb *cdsnwUpdate_All(cdsselect *context, void *buffer, luw_flag luw);
void *Cds_Find_First(cdsselect *context, lock_flag lock, word begin_luw);
void *Cds_Find_Last(cdsselect *context, lock_flag lock, word begin_luw);
void *Cds_Find_Next(cdsselect *context, lock_flag lock, word begin_luw);
void *Cds_Fetch_Again(cdsselect *context, lock_flag lock);
void *Cds_Find_Prev(cdsselect *context, lock_flag lock, word begin_luw);
foundset *CdsFindn(cdsselect *context, int n, int skip, byte reset_flag); 
foundset *Cds_Find_n(cdsselect *context, int n, int skip); 
			/* return next n or previous (-(-n)) qualifying records */
#define Cds_Find_First_n(context, n) Cds_Find_n(context, n, SHRT_MIN) 

clrb *cdsnwFind_First(cdsselect *context, lock_flag lock, word begin_luw);
clrb *cdsnwFind_Last(cdsselect *context, lock_flag lock, word begin_luw);
clrb *cdsnwFind_Next(cdsselect *context, lock_flag lock, word begin_luw);
clrb *cdsnwFind_Prev(cdsselect *context, lock_flag lock, word begin_luw);
clrb *cdsnwFindn(cdsselect *context, int n, int skip, byte reset_flag);
clrb *cdsnwFind_n(cdsselect *context, int n, int skip); 
			/* return next n or previous (-(-n)) qualifying records */
void *Cds_Unlock(cdsselect *context);
clrb *cdsnwUnlock(cdsselect *context);
void *Cds_Cancel_Scan(cdsselect *context);  /* Users don't call this */
void *Cds_Delete(cdsselect *context, luw_flag luw);
clrb *cdsnwDelete(cdsselect *context, luw_flag luw);
clrb *cdsnwDelete_All(cdsselect *context, luw_flag luw);


void *clwait(clrb *request_id);	/* CDS CLIENT WAIT */
#ifndef DUMPER_H
#	include "dumper.h"
#endif

#ifdef	CDSVSTATS
void cmstats_print(const void *ftab);
#endif
#ifndef _CDSTYPE_H
#include "cdstype.h"
#endif
#endif

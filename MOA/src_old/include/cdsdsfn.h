
/*$
**              CLCDSFUNCS.H
** 
**  This include file contains the function prototypes for CDS functions
**	which require different parameters at the client then at the server.
** 
**  If one of these functions does not succeed, an error code will
**  be placed in the global variable cds_errno.  Refer to cdserror.h
**
**  (C) Copyright by ISC System Corp., 1989          All rights reserved.
**
*/

#ifndef CLCDSFUNCS_H
#define CLCDSFUNCS_H

extern void *sv_who(char *server_id, int options);
extern void *Cds_Files(char *server_id);
extern void *Cds_Tables(char *server_id);
extern bool	Cds_Reset_Log( char *server_id );
extern void become_primary (char *server_id);
extern bool	cdsync(char *server_id, char *primary_machine,
					char *shadow_machine, bool quick);

extern bool	Cds_2_Phase_Commit(clconnect *coordinator, void *server_id_list, unsigned luw_id);
extern void *Cds_Project(cdsselect *cntxt, char *projection, bool allow_reset);
extern clrb *cdsnw_Project(cdsselect *cntxt, char *projection);
extern char *cds_fetch(register client_context *context, char op, register lock_flag lock, int begin_luw);
				/* op may be FIND_FIRST, FIND_LAST, FIND_NEXT, or FIND_PREV */
extern clrb *cdsnw_fetch(register client_context *context, char optype,
					register lock_flag lock, int begin_luw);

extern void *cds_update(register client_context *cntxt, char op, void *buff, lock_flag lock,
						register luw_flag luw);
						/* op may be SVINSERT, REPLACE, or UPDATE_FLDS */
extern clrb *cdsnwUpdate(register client_context *context, char op, void *buffer, lock_flag lock,
						register luw_flag luw);
extern client_context *get_join_context(join_list_param *table_list, const char *context_name);

extern clrb	*cdsnwselect(char *prjtn,client_context *cntxt,char *where,char *order);
extern client_context	*cds_select(char *projection, client_context *cntxt,
							char *criteria, const char *order);
extern client_context	*cds_join(char *projection, client_context *cntxt,
							char *criteria, char *order);
extern clrb *cdsnwjoin(char *projection, register client_context *cntxt, char *criteria);
#endif

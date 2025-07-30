
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
#ifndef CLENV_H
#include "clenv.h"
#endif
#ifndef CLCDSENV_H
#include "clcdsenv.h"
#endif
#ifndef CDSDEFS_H
#include "cdsdefs.h"
#endif
void catastrophe(void *ccb, int num, const char *function_name);
bool Cds_Initial (void *env, char *server_id);
extern void *sv_who(clenv *env, char *server_id, int options);
bool cdsStop(clenv *env, char *serverSuffix);	/* halts named server */	
extern void *Cds_Files(clenv *env, char *server_id);
extern void *Cds_Tables(clenv *env,  char *server_id);
extern bool	Cds_Reset_Log(clenv *env,  char *server_id );
extern void become_primary (clenv *env,  char *server_id);
extern bool	cdsync(clenv *env,  char *server_id, char *primary_machine,
					char *shadow_machine, bool quick);

extern bool	Cds_2_Phase_Commit(clconnect *coordinator, void *server_id_list, unsigned luw_id);
extern void *Cds_Project(cdsselect *cntxt, char *projection, bool allow_reset);
extern clrb *cdsnw_Project(cdsselect *cntxt, char *projection);
extern void *cds_fetch(register client_context *context, char op, register lock_flag lock, int begin_luw);
				/* op may be FIND_FIRST, FIND_LAST, FIND_NEXT, or FIND_PREV */
extern clrb *cdsnw_fetch(register client_context *context, char optype,
					register lock_flag lock, int begin_luw);

extern void *cds_update(register client_context *cntxt, char op, void *buff, lock_flag lock,
						register luw_flag luw);
						/* op may be SVINSERT, REPLACE, or UPDATE_FLDS */
extern clrb *cdsnwUpdate(register client_context *context, char op, void *buffer, lock_flag lock,
						register luw_flag luw);
int cdsBatchInsert(void *env, const char *table_name, foundset *record_set,luw_flag luw);
extern client_context *get_join_context(void *env, join_list_param *table_list, const char *context_name);

extern clrb	*cdsnwselect(char *prjtn,client_context *cntxt,char *where,char *order);
extern client_context	*cds_select(char *projection, client_context *cntxt,
							char *criteria, const char *order);
extern client_context	*cds_join(char *projection, client_context *cntxt,
							char *criteria, char *order);
extern clrb *cdsnwjoin(char *projection, register client_context *cntxt, char *criteria);

bool cdsInit(clenv *env, char *project_name, char *server_id,
		char *cds_host,long lastddfldupdate, long ddhash, short retry_count);
				/*initialize client / server connection */
int  cdsPing(clenv *env, char *service_name_suffix, int retries);
cdsselect *Cds_Select(void *env, char *projection, const char *table_name,
		 const char *context_name,  char *selection_criteria,
		 const void *order);
foundset *Cds_SelectnFetch(void *env, char *projection, const char *table_name,
		 char *selection_criteria, const void *order, unsigned int n);
cdsselect *Cds_Join(void *env, char *projection, join_list_param *table_list, const char *context_name,
			  char *selection_criteria, char *unused);
clrb *cdsnwJoin(void *env, char *projection, join_list_param *table_list,
			  const char *context_name, char *selection_criteria);
clrb *cdsnwSelect(void *env, char *projection, const char *table_name, 
              const char *context_name, char *selection_criteria, void *order);
void *Cds_Compress(void *env, const char *table_name, char *criteria);

bool Cds_Create_Table(void *env, const char *name, char *index_list, table_options *opts,
							char *path);
bool Cds_Alter_Table(void *env, char *name, char *index_list, char *updates,
							char *path);
bool Cds_Rename_Table(void *env, const char *name, const char *new_name);
bool Cds_Copy_Table_Def(void *env, const char *name, const char *new_name);
bool Cds_Drop_Table(void *env, const char *name);
bool Cds_Create_Index(void *env, const char *table_name, bool unique,
				bool allow_key_change, const char *index_name, char *index_list);
bool Cds_Drop_Index(void *env, const char *table_name, const char *index_name,
											char *index_list);

bool	Cds_Begin_Work(clenv *env);
bool	Cds_Commit( clenv *env );		/* simple or two phase commit */
bool	Cds_Begin_Batch(clenv *env );
bool	Cds_Commit_Batch( clenv *env );
bool	Cds_Rollback( clenv *env );
void *Cds_Server_Names(clenv *env, char *table_name);
long Cds_Table_Size(void *env, const char *table_name); /* reports bytes used in base */
void	sv_stop( clenv *env );	/* kills all servers for all users */
void	open_capture( clenv *env );	/* start capturing messages at servers*/
void	close_capture( clenv *env );	/* stop capturing messages at servers*/
void *Cds_Lock_Table(clenv *env, int lock_type, void *table_list);
bool Cds_Unlock_Table(clenv *env, int lock_type, void *table_list);
int SetConnectOption(void *ccb, int optionID, int value);
int SetStmtOption(void *ccb, int optionID, int value);
void *GetConnectOption(void *ccb, int optionID);
void *GetStmtOption(void *ccb, int optionID);
#endif

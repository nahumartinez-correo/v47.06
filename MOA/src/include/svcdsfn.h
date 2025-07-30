
/*$
**              SVCDSFUNCS.H
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

#ifndef SVCDSFUNCS_H
#define SVCDSFUNCS_H

extern char *sv_who(void *ccb, int options);
extern bool svIsConnected(void *ccb);
int svSetConnectOption(void *ccb, int option_number, int value);
void *svGetConnectOption(void *ccb, int option_number);
int svSetStmtOption(void *ccb, int option_number, int value);
void *svGetStmtOption(void *ccb, int option_number);
extern char *Cds_Files(void *ccb);
extern char *Cds_Tables(void *ccb);
extern bool	Cds_Reset_Log(void *ccb);
extern bool	cdsync(void *ccb, char *primary_machine,
					char *shadow_machine, bool quick);
extern bool	Cds_2_Phase_Commit(void *ccb, addr server_id_list, unsigned luw_id);
extern char *Cds_Project(cdsselect *cntxt, char *projection);
extern int  svBatchInsert(void *ccb, const char *table_name, foundset *record_set, luw_flag luw);
extern bool	Cds_Rollback(void *ccb);
extern bool Cds_Commit(void *ccb);
cdsselect *Cds_Select(void *ccb, char *projection, const char *table_name,
		 const char *context_name,  char *selection_criteria,
		 const void *order);
foundset *Cds_SelectnFetch(void *ccb, char *projection, const char *table_name,
		 char *selection_criteria, const void *order, unsigned int n);
cdsselect *Cds_Join(void *ccb, char *projection, join_list_param *table_list, const char *context_name,
			  char *selection_criteria, char *unused);
void *Cds_Compress(void *ccb, const char *table_name, char *criteria);
bool Cds_Create_Table(void *ccb, const char *name, char *index_list, table_options *opts,
							char *path);
bool Cds_Alter_Table(void *ccb, char *name, char *index_list, char *updates,
							char *path);
bool Cds_Rename_Table(void *ccb, const char *name, const char *new_name);
bool Cds_Copy_Table_Def(void *ccb, const char *name, const char *new_name);
bool Cds_Drop_Table(void *ccb, const char *name);
bool Cds_Create_Index(void *ccb, const char *table_name, bool unique,
				bool allow_key_change, const char *index_name, char *index_list);
bool Cds_Drop_Index(void *ccb, const char *table_name, const char *index_name,
											char *index_list);

bool	Cds_Begin_Work(void *ccb);
bool	Cds_Commit(void *ccb);		/* simple or two phase commit */
bool	Cds_Begin_Batch(void *ccb);
bool	Cds_Commit_Batch(void *ccb);
bool	Cds_Rollback(void *ccb);
long Cds_Table_Size(void *ccb, const char *table_name); /* reports bytes used in base */
void	sv_stop(void *ccb);	/* kills all servers for all users */
void	open_capture(void);	/* start capturing messages at servers*/
void	close_capture(void);	/* stop capturing messages at servers*/
void *Cds_Server_Names(void *ccb, char *table_name);

void *Cds_Lock_Table(void *ccb, int lock_type, void *table_list);
bool Cds_Unlock_Table(void *ccb, int lock_type, void *table_list);
#endif

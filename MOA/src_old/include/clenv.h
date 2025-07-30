#ifndef CLENV_H
#define CLENV_H

#ifndef DATAMAN_H
#include "dataman.h"
#endif

#ifndef CDSLIMITS_H
#include "cdslmts.h"
#endif
#ifndef LISTMSG_H
#include "listmgr.h"
#endif

typedef struct ccb_stack {
	struct ccb_stack *prev;
	struct clenv *env;
	struct LIST table_head_space;
	struct LIST context_head_space;
	struct LIST ccb_head_space;
	void *table_head;
	void *cntxt_head;
	void *ccb_head;
	long luw_id;
	short updates_pending;
	char distributed_LUW;
	char LUW_state;
} ccb_stack;

#define MAX_MESSAGE_LENGTH 200
#define CLENV_FINDER  0xCECECECE
typedef struct clenv {
	int finder; // =  CLENV_FINDER
	int RTddh;
	CRITICAL_SECTION dictionary;
	HWND GhWnd;
	HINSTANCE	globhinstance;
	int			globncmdshow;
	int	Client_type;	/* bit 0 on means use context buffers */
	short Updates_pending;
	short Cds_Stack_Depth;
	ccb_stack *Ccb_stack, *Ccb_stack_free;
	byte common_id, last_appl_id;
	byte LUW_State;
	byte Distributed_LUW;
	short phase2_retry_count;
	unsigned short buffer_ID;
	unsigned long Luw_id;
	unsigned long old_luw_id;
	char project[MAX_PROJ_NAME+1];
	char	DD_path[MAX_PATH_NAME + 1];
	char	Euname[USER_NAME_LEN+1];	/* effective user name */
	int cds_errno;
	struct LIST tab_head_space;
	struct cl_table	*tab_head, *tab_free;	/* for gl */
	struct LIST context_head_space;
	struct cl_context	*context_head, *context_free;	/* for gl */
	struct LIST ccb_head_space;
	struct clconnect	*ccb_head, *ccb_free;	/* for gl */
	struct LIST Rblhead_space;
	struct cds_request *Rblhead, *Rblfree;
	char last_appl_name[NAME_KEYSIZE+2];
	UCHAR Cds_error_msg[MAX_MESSAGE_LENGTH];
	field Cds_table_name, Cds_table_no, Cds_allow_new_field, Cds_buffer_size, Cds_minimum_record, 
	   Cds_allow_delete,  Cds_index_name, Cds_index_no, Cds_index_appl_name, Cds_index_field_name,
	   Cds_index_modifier,	Cds_unique_index, Cds_key_changable;
	field *flist[16];
	ATOM window_class;
	UCHAR *WindowClassName;   // = "CDS_Private"
	int pid;  // process id;
	DWORD threadID;
} clenv;
HANDLE clAllocEnv(char *project, int client_type);
bool Cds_Initial (void *env, char *serverNameSuffix);
HANDLE clAllocCDSEnv(char *project, int client_type, int ddh, HANDLE window_handle);
int clFreeEnv(HANDLE henv);	// deallocate CDS client environment
#endif

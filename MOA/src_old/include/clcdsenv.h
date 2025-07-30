
/*$
**              CLCDSENV.H
** 
**    This include file contains structures and function prototypes used
**    to support the multi-servr environment at the client.  client_table
**    structures are used to help the client keep track of the server
**    tables.  client_context structures are used so that the client can
**    keep track of all of its context on all of the servers that it is
**    connected to.
**
**  (C) Copyright by ISC System Corp., 1989          All rights reserved.
**
*/

/* may be automatically included in cdsfuncs.h */
#ifndef CLCDSENV_H
#define CLCDSENV_H

#ifndef _Hc
#include	<c.h>
#endif
#ifndef CDSLIMITS_H
#include	<cdslmts.h>
#endif
#ifndef CLRB_H
#include <clrb.h>
#endif
#ifndef CLCONNECT_H
#include <clconn.h>
#endif
#ifndef CDSDEFS_H
#	include "cdsdefs.h"
#endif

#define	GOOD_CONTEXT	0xB1FF

typedef struct column_information {
	char name[NAME_KEYSIZE+1];
	unsigned char  appl_id;
	unsigned char  type;
	unsigned short field_id;
	unsigned short length;
} CDS_column_info;

typedef struct cl_table
{
	struct	cl_table	*next;
	struct	cl_table	*prev;
	short	flags;				/* list manager required fields */
	char	name[TBLEN + 1];
	clconnect	*ccb;	/* connection associated with the table */
	reclen		buffsize;	/* for RT to use for non-dynamic buffer */
	CDS_column_info *column; /* often null */
}	client_table;

typedef	struct	cl_context
{
	struct	cl_context	*next;
	struct	cl_context	*prev;
	short	flags;				/* for list manager */
	unsigned short 	finder;		/* tag for valid context */
	client_table	*tbl;		/* table associated with context */
	char	join_tables_space[3 * sizeof(join_list_param)];
	void	*join_tables;
	char	context_name[CNTXTLEN + 1];
	char	state;		/* context state for rt */
	char	*putbase;	/* start of put buffer for rt */
	char	*put;		/* current put pointer for rt */
	char	*getbase;	/* buffer ptr returned from cds for rt */
	char	*get;		/* current record in get buffer for rt */
	void	*cursor;	/* remote context pointer */
	long	bufflen;	/* remaining length in put buffer for rt */
	clrb	*rqblk;		/* client request block for no-wait i/o */
	unsigned short	cmid;	/* client message identifier number */
} client_context;

/* select struct for client */
typedef	struct	cl_context	cdsselect;
#define SELECT_STRUCT_DEFINED 1

extern client_table	*find_cltab(clenv *env, const char *tab_name);
client_context *find_clcontext(clenv *env, const char *context_name);
client_context *find_clcontext_in_ccb (clconnect *ccb, const char *context_name);
extern bool	remove_clcontext(client_context *context);
extern	void	rm_tbl_clcontext (clenv *env, const char *tab_name, bool remote);
extern bool	rm_all_clcontexts (clenv *env, register char *server_id);
extern	client_context	*get_clcontext (void *env, const char *tab_name, const char *context_name);

extern	char	*isolate_tab_name(register const char *name);
extern bool close_cds_connect (clenv *env, register char *server_id);
extern int isConnected (clenv *env, register char *server_id);
#define CDS_SERVICE_NAME_UNKNOWN 0
#define CDS_SERVICE_CONNECTED 1
#define CDS_SERVICE_DISCONNECTED -1
extern void	close_all_clconnects (clenv *env);
extern bool clsvsetup(void *ccb, long dict_time, long ddhash);
extern void	clerrinit (clenv *env, char *strng);
extern bool cdsPush(clenv *);
extern bool cdsPop(clenv *);
extern void cdsClearStack(clenv *);
UCHAR *cdsErrMsg(clenv *env);
int	intFromErrMsg(clenv *env);

#endif

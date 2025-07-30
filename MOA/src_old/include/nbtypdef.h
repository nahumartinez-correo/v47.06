
/***********************************************************************
*
*             Data type defintions for NetBoss programs
*
*  ----------------------------------------------------------------
*
*                    ISC Systems Corporation
*                  Confidential / Proprietary
*   Copyright (C) 1989 by ISC-Bunker Ramo / Unpublished Work
*     For Use by ISC-Bunker Ramo and Authorized Users Only
*                     All rights reserved.
*
**********************************************************************/

#ifndef _NBTYPEDEFS_H
#define _NBTYPEDEFS_H

typedef	unsigned	short	WORD;
#if 0
typedef unsigned	short	BOOL;
#endif
typedef unsigned	short	INDEX;
typedef unsigned	short	USHORT;
typedef	unsigned	short	LEN_T;		/* holds data lengths */
typedef long splctxt;	/* Spooler context */
#ifndef DNIX
typedef struct iocb {
	void	(*func)();
	struct iocb *next;
	struct server *sv;
	int		fd;					/* fd on which we did this io (LISTEN only) */
	void *	data;
} iocb;

typedef struct partner {
	int		fd;					/* fd of channel to partner */
	int		closing;			/* TRUE if we're colsing partner */
	int		count;				/* how many requests outstanding to partner */
	int		openreq;			/* -1 or request number of pending NoWaitOpen */
} partner;
#endif /*ndef DNIX*/

struct NET_NODE
{
	char node_name[8 + 1];
	char node_status;
	long lupd;
};

typedef struct NET_NODE NET_NODE;

struct	server {
	struct	server	*srv_link;
	struct	buffer	*srv_rcvmsg;
	struct	buffer	*srv_datamsg; /* queued outgoing messages (l3) */
	struct	buffer	*srv_oobmsg;
#ifdef DNIX
	struct	level3	*srv_partner;
#else
	struct	buffer	*srv_quedmsg_head; /* first queued incoming message */
	struct	buffer	*srv_quedmsg_tail; /* last queued incoming message */
	partner	*srv_partner;
#endif
	struct	server	*dnpcreate_client;
	struct	NET_NODE *net_node;
	int		transf_fd;
	long lupd;
	void	*context;
	splctxt	ctxt;			/* spool context for next spool cmd */
	int	class;
	int		exec_tag;		 /* exec shell client expecting response */
	short	exec_cnt;
	short	exec_cntdb;
	char	node[24];		/* name of node entry */
	char	userid[9];		/* user identification */
	char	sysid[9];		/* system user signed on at */
	char	qued_msg;		/* true if message qued in serverprot */
	char	force_spool;	/* true if spooling records to update db */
	char	perm_cb;		/* 0=norm cl, 1=nb cl, 2=nb cl opened */
	char	lastseq[16];	/* sequence number of last transaction completed */
};

typedef	struct server server;

struct child
{
	void (*read_rtn)();
	void (*timer_rtn)();
	int pid;
	int tag;
	int read_rn;
	int timer_rn;
	int time_value;
	int out[2];
	int in[2];
	int bytes_read;
	int exit_status;
	server *sp;
	struct buffer *bp;
	struct child *next_child;
};

typedef struct child child;

#ifndef FALSE
#define	FALSE	0
#endif
#ifndef TRUE
#define	TRUE	1
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /* _NBTYPEDEFS_H */


/***********************************************************************
*
*   Message buffer, data length, and assorted  defintions for NetBoss
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

#ifndef _NBMSG_H
#define _NBMSG_H

#include <nbiflib.h>

#define BOSSERVICE ":NetBoss"	/* servicename used by the Boss */

/*
**  Lengths of various fields and data structures; system-wide size limits
*/
#define NBCB_KEYCOUNT 1			/* number of key fields */

#define MAXNODES	30			/* Max number of nodes in a logical network */
#define	NODENAME_L	8			/* max length of a node name (not incl. NUL) */
#define BUFF_SIZE 1380			/* Big enough to hold all netboss system msgs */

/*********************************************************************
*  NBS   COMM msg structure element defines
*********************************************************************/
/*  The following are for netboss netgotiation and NETMGR replacement */

#define	CS_NBS_STAT			0x5141	/* Netboss status req/rsp     */
#define	CS_NBS_EXEC_SHELL	0x5142	/* Netboss exec shell req/rsp */
#define	CS_NBS_DBREQ		0x5143	/* Netboss exec shell req/rsp */
#define	CS_NBS_NSPOOL		0x5144	/* Netboss get next spooled command */
#define	CS_NBS_REGISTER		0x5145	/* Netboss register user verb */
#define	CS_NBS_FSPOOL		0x5146	/* Netboss flush spool file */
#define	CS_NBS_SWROLES		0x5147	/* Netboss switch roles command */
#define	CS_NBS_JOINNET		0x5148	/* Netboss Join logical network request */
#define	CS_NBS_TRANSF_DB	0x5149	/* Netboss Transfer database to new dnp */
#define	CS_NBS_NDB_BLOCK	0x514a	/* Netboss get next database block */
#define	CS_NBS_DEL_DNP		0x514b	/* Netboss del dnp from logical network */
#define	CS_NBS_ADD_DNP		0x514c	/* Netboss add dnp to logical network */
#define	CS_NBS_KILL_CHILD	0x514d	/* Netboss kill children, netmgr client died */
#define	CS_NBS_NM_UNLOCK	0x514e	/* Netboss netmgr unlock */
#define	CS_NBS_CHG_DNPNAME	0x514f	/* Netboss change dnp name */
#define	CS_NBS_CHG_DNPCLASS	0x5150	/* Netboss change dnp class */
#define	CS_NBS_FJOINNET		0x5151	/* Netboss Join lnet forced request */
#define	CS_NBS_CPR			0x5152	/* Netboss Check point record */
#define	CS_NBS_RESOLVE		0x5153	/* Netboss resolve database req/rsp */
#define	CS_NBS_ESPOOL		0x5154	/* Netboss no more spooled commands */

/* length of spool file serial number */
#define SERIAL_L			sizeof(long) + 8 + 1

/*********************************************************************
*	NBS		COMM msg structures																				* 
*********************************************************************/

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char netboss_flag;		/* true if declared as netboss */
	char	serial[SERIAL_L];	/* start of rsp specific data */
	char	cl_sysid[25];
} CSS_NBS_STAT;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char comp_code;			/* success or failure code */
} CSS_NBS_NM_UNLOCK;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char	userid[10];		/* start of rsp specific data */
	char	sysid[9];
} CSS_NBS_REGISTER;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char flags;
	char	serial[SERIAL_L];	/* new lupd time from netboss if successful */
	char	class[sizeof(short)];
	char	tag[sizeof(int)];
	char	exitcode[sizeof(int)];
	char	cl_sysid[25];
	char	data[900];
} CSS_NBS_EXEC_SHELL;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char flags;
	char compl_code[sizeof(int)];	/* completion code */
	char lnetname[9];		/* name of network to join */
} CSS_NBS_JOINNET;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char	tag[sizeof(int)];
} CSS_NBS_KILL_CHILD;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */
	char req_rsp;			/* request/response */
	char flags;
	char sysid[9];
} CSS_NBS_TRANSF_DB;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */
	char req_rsp;			/* request/response */
	char flags;
	char sysid[9];
} CSS_NBS_ADD_DEL_DNP;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */
	char req_rsp;			/* request/response */
	char flags;
	char sysid[9];
	char newsysid[9];
} CSS_NBS_CHG_DNPNAME;

#define	CSL_NBS_CHG_DNPNAME ( (int)&((CSS_NBS_CHG_DNPNAME *)0)->newsysid + sizeof(((CSS_NBS_CHG_DNPNAME *)0)->newsysid) )

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */
	char req_rsp;			/* request/response */
	char flags;
	char sysid[9];
	char classlist[20];
} CSS_NBS_CHG_DNPCLASS;

#define	CSL_NBS_CHG_DNPCLASS ( (int)&((CSS_NBS_CHG_DNPCLASS *)0)->classlist + sizeof(((CSS_NBS_CHG_DNPCLASS *)0)->classlist) )

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */
	char req_rsp;			/* request/response */
	char flags;
	char cpr[sizeof(NET_NODE) * MAXNODES];
} CSS_NBS_CPR;

#define	CSL_NBS_CPR ( (int)&((CSS_NBS_CPR *)0)->cpr + sizeof(((CSS_NBS_CPR *)0)->cpr) )

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char flags;
	char sysid[9];
	char file;				/* 1=idx,2=dat,3=spool,4=nblupd,0=current file */
	char compl_code[sizeof(int)];
} CSS_NBS_NDB_BLOCK;

typedef struct
{
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */
	char sysid[9];
} CSS_NBS_RESOLVE;

#define CSS_NBS_DBHDR\
	unsigned char mq;		/* mq = Netman 'q' and 'm' flags */\
	char fc;				/* fc = function code */\
	char req[2];			/* request code */\
	char req_rsp;			/* request/response */\
	char flags;\
	char serial[SERIAL_L];\
	char type[sizeof(short)];\
	char connect_id[sizeof(short)];\
	char key_length[sizeof(short)];\
	char length[sizeof(short)];

typedef struct
{
	CSS_NBS_DBHDR			/* data base header */
} CSS_NBS_DBDUMMY;

#define CSL_PAD CSL_NBS_DBHDR - ( (int)&((CSS_NBS_DBDUMMY *)0)->key_length)

typedef struct
{
	CSS_NBS_DBHDR			/* netboss database header */
	char pad1[CSL_PAD];
	char key[KEYDATA_L + 1];
	char pad2;
	char data[DATAMAX];
} CSS_NBS_DBREQ;

#define	CSL_NBS_DBREQ	( (int)&((CSS_NBS_DBREQ *)0)->data + DATAMAX )

/* offset to CONMAN flag byte */
#define OFCHCFLG	( (int)&((CSS_NBS_DBREQ *)0)->flags)
/* offset to length fields */
#define OFCHLEN		( (int)&((CSS_NBS_DBREQ *)0)->key_length)
/* offset to data field */
#define	OFCHDATA	( (int)&((CSS_NBS_DBREQ *)0)->data)
/* offset to key field */
#define	OFCHKEY	( (int)&((CSS_NBS_DBREQ *)0)->key)



#endif /* _NBMSG_H */

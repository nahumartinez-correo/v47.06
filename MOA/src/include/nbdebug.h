
/***********************************************************************
*
*             Debugging defintions for NetBoss programs
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

#ifndef _NBDEBUG_H
#define _NBDEBUG_H

#include <stdio.h>
#include <nbtypdef.h>

void hex_dump(int , char *, int );

#ifdef	DEBUG
extern	BOOL	NBDebug;
#define	LOG(s,i)	if ( NBDebug ) { fprintf(stderr,"<DEBUG> "s,(i)),fflush(stderr);}
#else
#define	LOG(s,i)
#endif

#ifdef DEBUG
#define DUMP(lvl, b, l)	if ( NBDebugLevel >= lvl ) hex_dump(2, (b), (l))
#else
#define DUMP(lvl, b, l)	/* nothing */
#endif


#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

#ifdef DEBUG
extern	void	DebugLogTrace();
extern	void	LogMsg();
extern	char *	Tsprintf();

extern int NBDebugLevel;

#define DTRACE(args) DebugLogTrace ##args
#define DLOG(args) if (NBDebugLevel > 0) LogMsg ##args
#define DLOGINIT(Log_File_Name) \
	{\
	int fd;\
	int fd_zero=FALSE;\
	int fd_one=FALSE;\
	close(2);\
	while ((fd = open(Log_File_Name, 0411, 0666)) < 2) \
					/* CREAT | APPEND | WRONLY */\
	{\
		if (fd < 0)\
		{\
			char buf[160];\
			Tsprintf(buf, "echo 'cannot open log %s, errno %d' >>/tmp/panic", \
				Log_File_Name, errno);\
			system(buf);\
			exit(2);\
		}\
		if (fd == 0) fd_zero = TRUE;\
		if (fd == 1) fd_one = TRUE;\
	}\
	if (fd_zero) close(0);\
	if (fd_one) close(1);\
	}

#else	/* ndef DEBUG */
#define	DTRACE(x) /*nothing*/
#define DLOG(x) /*nothing*/
#define	DLOGINIT(x) /*nothing*/
#endif

#endif /* _NBDEBUG_H */

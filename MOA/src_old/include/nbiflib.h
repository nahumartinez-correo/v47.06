
/***********************************************************************
*
*            Defintions for NetBoss interface library programs
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

#ifndef _NBIFLIB_H
#define _NBIFLIB_H		/* multiple-include protection */

#include <winsock.h>

#define	CNTXLN	sizeof(long)
#define	MAXIMUM_CONTEXT	256			/* biggest context index we expect */

/******************************************************************************
*
*  Data structure used as arguments to NetBoss access functions.
*
******************************************************************************/
#define BOSS_OWNER "NB"	/* "owner" field for NetBoss records */
#define KEYDATA_L	40			/* Length of database "keydata" field */
#define	DATAMAX		255			/* Length of database "data" field */
#define REGMAX		KEYDATA_L + DATAMAX

#define DBPATH "lib/netboss"	/* where's our database directory? */
#define LNETID_FILE make_filename(DBPATH "/lnetid")

#define CSL_NBS_DBHDR 26

typedef struct
{
	unsigned short key_length;
	unsigned short data_length;
	char nbstuff[CSL_NBS_DBHDR]; /* netboss header info */
	char key[KEYDATA_L + 1];
	char pad;
	char data[DATAMAX];
} NBiorec;

typedef struct
{
	char nbstuff[CSL_NBS_DBHDR + sizeof(short) + sizeof(short)]; /* netboss header info */
	char reg_exprs[REGMAX];
} NBregiorec;

extern char NBlognetname[];
extern SOCKET netboss_fd;

/*
**  The permissible message types... (function codes passed by macros)
*/
#define CBFIRST	0			/* first command */
#define CS_NBS_CBI	0		/* Insert */
#define CS_NBS_CBD	1		/* Delete */
#define	CS_NBS_CBU	2		/* Update */
#define	CS_NBS_CBG	3		/* Get exact */
#define CS_NBS_CBN	4		/* (get) Next */	
#define CS_NBS_CBQ 	5		/* Quit (clean up database context ) */
#define	CS_NBS_CBZ	6		/* Zap (multiple delete) */
#define	CS_NBS_CBP	7		/* PUT (Update or Insert if not there) */
#define	CS_NBS_CBGR	8		/* Get (regular expression match) */
#define	CS_NBS_CBZR	9		/* Zap (regular expression match) */
#define	CS_NBS_CBGP	10		/* Get Partial */
#define	CS_NBS_CBM	11		/* Merge (Insert if not there, error if diff) */
#define CS_NBS_CBEG	12		/* End of gets, free context */
#define CBLAST	12			/* last command */


#define NBI(A) nball(CS_NBS_CBI, A, (NBiorec *)0)
#define NBD(A) nball(CS_NBS_CBD, A, (NBiorec *)0)
#define NBZ(A) nball(CS_NBS_CBZ, A, (NBiorec *)0)
#define NBU(A) nball(CS_NBS_CBU, A, (NBiorec *)0)
#define NBG(A,B) nball(CS_NBS_CBG, A, B)
#define NBN(A) nball(CS_NBS_CBN, (NBiorec *)0, A)
#define NBP(A) nball(CS_NBS_CBP, A, (NBiorec *)0)
#define NBGP(A,B) nball(CS_NBS_CBGP, A, B)
#define NBM(A) nball(CS_NBS_CBM, A, (NBiorec *)0)
#define NBEG(A) nball(CS_NBS_CBEG, (NBiorec *)0, (NBiorec *)0)

int NBZR(register NBregiorec *in);
int NBGR(register NBregiorec *in, register NBiorec *out);
int nball(register short msg_type, register NBiorec *in, register NBiorec *out);
int nb_rcv(register char *bp, register int len);
int NBpath(register char **path);
int NBopen();
int NBopen_exread(register char *path, register int fd);
int NBpath_p(register char **path, register char *lnetid);
int NBopen_p(register char *path, register char *lnetid);

#define NBclose() close(netboss_fd);netboss_fd = -1

#endif /* _NBIFLIB_H */

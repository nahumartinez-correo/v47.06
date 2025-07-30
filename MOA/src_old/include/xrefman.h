/*****************************************************************************
* Header File For Xref Manager
*****************************************************************************/
#ifndef XREFMAN_H
#define	XREFMAN_H

#define	REFEROR_KEYSIZE	18
#define	REFEREE_KEYSIZE	18
#define	UNREF_KEYSIZE	9
#define REFEROR	1
#define REFEREE	2
#define UNREF	3

#ifndef CBTREE_H
#	include <cbtree.h>
#endif

typedef struct TREEMAN {
	int				fdidx;		/* Index File Descriptor			*/
	int				fddat;		/* Data File Descriptor				*/
	struct btcommo	bt_refor;	/* btcommo Structure For referor Index */
	struct btcommo	bt_refee;	/* btcommo Structure For referee Index */
	struct btcommo	bt_unref;	/* btcommo Structure For unref   Index */
	} treeman;

extern treeman XRTm;

typedef struct XROBJ {
	byte	obj;		/* object type number */
	byte	applno;		/* application number */
	index	itemno; 	/* item number */
	}	xrobj;

typedef struct XRREF {
	xrobj	referor;	/* object that refers to something */
	xrobj	referee;	/* object that was refered to */
	}	xrref;
	/* Two null terminated strings immediately follows the */
	/* XRREF structure.  They will contain:                */
	/*                                                     */
	/*      the first string is the referor:               */
	/*          object name:application name:item name     */
	/*                                                     */
	/*      the second string is the referee:              */
	/*          object name:application name:item name     */


typedef struct XRUNREF {
	xrobj	unref;	/* unreferenced object */
	}	xrunref;
	/* One null terminated strings immediately follows the */
	/* XRUNREF structure.  It will contain:                */
	/*      object name:application name:item name         */

typedef struct CROBJ {
	byte	obj;		/* object type number */
	byte	applno;		/* application number */
	index	itemno;		/* item number */
	char	object[4];
	char	appl_name[21];
	char	item_name[21];
	}	crobj;

typedef struct CRREF {
	crobj	referor;	/* object that refers to something */
	crobj	referee;	/* object that was refered to */
	}	crref;

typedef struct CRUNREF {
	crobj	unref;	/* unreferenced object */
	}	crunref;

extern	bool	XRrunmode;	/* System Configuration Either RUN Or MAINTAIN */
extern	bool	XRopen;		/* Dictionary Open Flag */
extern	int		RTddh;		/* file handle for data dictionary */

extern	int		XRerrno;

/* Functions In xrefman.c */
bool	xrefinit(char *, bool);
bool	xrefclose(void);
void	xr_inc_update(void);
void	xr_dec_update(void);
void	xrsetsignal(void);

/* Functions In xrfminit.c */
bool	xrbtreeinit(void);
bool	xrbtreeopen(bool);
bool	xrbtreeclose(void);

/* Functions In xrfmmaint.c */
bool	xrbtreeinsert(char *, char *);
bool	xrbtreeinsertunref(char *);

/* Functions In xrgetlist.c */
int		xrgetlist(byte, byte, index, int, int, crunref **);
int		xrgetREFORlist(char *, int, crunref **);
int		xrgetREFEElist(char *, int, crunref **);
int		convert_key(char *, char *, crunref *);

/* Functions In xrmaint.c */
bool	xrefput(char *, char *);
bool	xrefunput(char *);

/* Functions In xrrun.c */
bool	xrunrefseq(int, crunref *);
bool	xrunrefrec(char *, crunref *);
void	convert_unref_key(char *, crunref *);
bool	xrrefeerec(char*, crref *);
bool	xrrefeeseq(int, crref *);
void	convert_refee_key(char *, crref *);
bool	xrreforseq(int, crref *);
bool	xrreforrec(char *, crref *);
void	convert_refor_key(char *, crref *);
bool	xrdate(char *);

#endif

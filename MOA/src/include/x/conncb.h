
#ifndef CONNECTCB_H
#define CONNECTCB_H

#ifndef CDSLIMITS_H
#	include <cdslmts.h>
#endif
typedef struct LOCK_PENDING
{
	struct LOCK_PENDING	*next;
	struct LOCK_PENDING	*prev;
	short  flags;		/* INUSE or not */
	void	*lock;		/* lock_struct	*lock; */
	void	*table;		/* Table	*table; */
} unlock_pending;

#pragma pack (push, 2)
typedef struct connectcb {	/** CONNECTION CONTROL BLOCK **/
/*	struct connectcb *next, *prev;*/
	char filler[8];
#define ONSCANQUE		2	/* bit 0 is reserved for list management */
/*#define ONPENDINGQUE	4	*/
	short flags;
	short fills;            /* filler */
	int (*iodone)();
	char fill[32];			/* filler to match struct in sv */
#include "x/ccbcomon.h"
	const void	*ftab;	   // the pointer isn't constant, the function table is.
} Connectcb;
#pragma pack (pop)
global_const char Shadow_Client_Name[];	/* ="_shadow_"  ALSO IN CDSV/connectcb.h */
#include <x/cbfuncs.h>
#endif

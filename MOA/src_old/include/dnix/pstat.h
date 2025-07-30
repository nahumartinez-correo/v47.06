/*
 *	(C) Copyright Diab Data AB, 1988
 */
#ifndef _DNIX_PSTAT_H
#define _DNIX_PSTAT_H

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

/*
 * Structure returned by pstat
 */
#define	PS_SPARSZ	80	/* Size of array holding start parameters */

#define PS_PINFO	0x8000	/* If bit set, dump all info for process */

struct	psinfo {
	short	ps_state;	/* Tcb state */
	short	ps_flags;	/* Tcb flags */
	short	ps_dpri;	/* Dynamic priority */
	short	ps_slct;	/* Timeslice downcount */
	dev_t	ps_ttyd;	/* Controlling tty device */

	ushort	ps_uid;		/* Effective user id */
	short	ps_pgrp;	/* Process group */
	short	ps_pid;		/* Process id */
	short	ps_ppid;	/* Parent process id */

	short	ps_acltm;	/* Alarm clock time */
	time_t	ps_utime;	/* u - time */
	time_t	ps_stime;	/* s - time */
	time_t	ps_swtim;	/* sw - time */

	long	ps_res1;	/* Reserved for future use */
	ulong	ps_textsz;	/* Text size */
	long	ps_res2;	/* Reserved for future use */
	short	ps_res3;	/* Reserved for future use */
	long	ps_res4;	/* Reserved for future use */
	ulong	ps_datasz;	/* Data size */
	long	ps_res5;	/* Reserved for future use */
	short	ps_res6;	/* Reserved for future use */
	long	ps_iopf;	/* I/O page fault */
	ulong	ps_stacksz;	/* Stack size */
	long	ps_res7;	/* Reserved for future use */
	short	ps_res8;	/* Reserved for future use */
	long	ps_mppf;	/* Memory page page fault */
	ulong	ps_upagesz;	/* Data size */
	laddr_t	ps_uaddr;	/* Upage address */
	short	ps_res9;	/* Reserved for future use */

	char	ps_spar[PS_SPARSZ];	/* Start parameters */

};
/*eject*/
/* Definition of ps_state */
#define PS_SSLEEP	1	/* Waiting */
#define PS_SFORK	2	/* Son of forking process */
#define PS_SRUN		3	/* Running or runnable */
#define PS_SSZOMB	5	/* Almost dead */
#define PS_SSTOP	6	/* Stopped */

/* Definition of ps_flags */
#define PS_FKERNEL	0x01	/* Kernel process */
#define PS_FLOCK	0x02	/* Locked in core */
#define	PS_FMIO		0x04	/* Map in VME memory or I/O page to process. */
#define PS_FSTRC	0x10	/* Process is being traced */
#define PS_FFSS		0x0100	/* File server process */

/*
 *	Fuser structure.
 */
struct fuser {
	ushort		fu_pid;		/* Process id */
	ushort		fu_uid;		/* User id */
	uint		fu_cnt;		/* # of open files for process */
	struct stat	fu_stat[2];	/* Stat stucture for open files */
};

#endif /* _DNIX_PSTAT_H */


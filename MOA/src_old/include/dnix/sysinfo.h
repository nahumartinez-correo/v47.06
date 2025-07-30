/*
 *	Structures and definitions for system activity moitoring
 *
 *	(C) Copyright Dataindustrier DIAB AB
 */

#ifndef _DNIX_SYSINFO_H
#define _DNIX_SYSINFO_H


#define CPU_IDLE	0
#define CPU_USER	1
#define CPU_KERNAL	2
#define CPU_WAIT	3

#define W_IO		0
#define W_SWAP		1
#define W_PIO		2

struct iostat
{
	dev_t	ios_dev;
	long	ios_actbit;
	long	ios_read;
	long	ios_write;
	long	ios_other;
	long	ios_errcnt;
	long	ios_blkerrcnt;
	long	ios_blkretrycnt;
	long	ios_blkcnt;
	time_t	ios_actst;
	time_t	ios_actt;
	time_t	ios_rspt;
};

struct Sysinfo
{
	time_t	cpu[4];
	time_t	wait[3];
	long	bread;
	long	bwrite;
	long	lread;
	long	lwrite;
	long	phread;
	long	phwrite;
	long	swapin;
	long	swapout;
	long	bswapin;
	long	bswapout;
	long	pswitch;
	long	syscall;
	long	sysread;
	long	syswrite;
	long	sysfork;
	long	sysexec;
	long	runque;
	long	runocc;
	long	swpque;
	long	swpocc;
	long	iget;
	long	namei;
	long	dirblk;
	long	readch;
	long	writech;
	long	rcvint;
	long	xmtint;
	long	mdmint;
	long	rawch;
	long	canch;
	long	outch;
	long	msg;
	long	sema;
/*
 *	Dnix special follows
 */
	long	core[3];
	long	proc[3];
	long	text[3];
	long	node[3];
	long	ofile[3];
	long	flock[3];
	long	swap[3];
	long	sem[3];
	long	semadj[3];
	long	shm[3];
	long	shmat[3];
	long	messq[3];
	long	pagesz;
	long	totmem;
	long	sysmem;
	long	runproc;
	struct iostat io_stat[33];
};

#endif /* _DNIX_SYSINFO_H */


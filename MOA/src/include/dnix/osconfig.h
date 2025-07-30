/*
 *	D-NIX configuration.
 *	--------------------
 *
 *	The configuration of D-NIX is done thrugh the following
 *	structure.
 *
 *	The 'oc_vers' field is a version number of the osconfig
 *	structure which ONLY!!! should be changed when entries
 *	in the structure change meaning.
 */

#ifndef _DNIX_OSCONFIG_H
#define _DNIX_OSCONFIG_H


#define	OC_VERS		2
/*
 *	Prevously there were some spare entries in the structure so
 *	programs who wrote some extra bytes wouldn't destroy the
 *	information following this structure. To avoid this problem
 *	the field 'oc_size' is implemented which should help programs
 *	writing to this strucure limit their writes.
 *
 *	Structure holding os configuration parameters.
 *	----------------------------------------------
 *
 *	"rfh" stands for "root file handler"
 */
struct	osconfig
{
	ushort	oc_vers;	/* Version number of this structure. */
	ushort	oc_size;	/* sizeof(struct osconfig). */
	ushort	oc_ntnod;	/* Number of tcb nodes */
	ushort	oc_ntad;	/* Number of tad nodes */
	ushort	oc_nnod;	/* Number of nodes */
	ushort	oc_nbuf;	/* Number of buffers for rfh */
	ushort	oc_nofid;	/* Number of open files in rfh */
	ushort	oc_nlock;	/* Number of file locks in rfh */
	ushort 	oc_nvol;	/* Number of mounted volumes on rfh */
	ushort	oc_nvsize;	/* Number of Mbyte virtual memory */
	ushort	oc_nmsg;	/* Number of message queues */
	ushort	oc_nsem;	/* Number of semaphore id's */
	ushort	oc_nsadj;	/* Number of sem adjust operations */
	ushort	oc_nshm;	/* Number of shared memory segs */
	ushort	oc_nashm;	/* Number of attached memory segs */
	ushort	oc_nsync;	/* Number of seconds between syncs */
	ushort	oc_nchtty;	/* Number of chars at tty input */
	ushort	oc_pftime;	/* Minutes before power failiure. */
	ushort	oc_ncterm;	/* Number of controlling terminal nodes. */
	ushort	oc_mdevlock;	/* Disable write on mounted device. */
	ushort	oc_nfsbuf;	/* Number of FSS cache pages */
	ushort	oc_nfsws;	/* Number of FSS workstations */
	ushort	oc_nfsprc;	/* Number of FSS processes */
	ushort	oc_sfsdat;	/* Size of FSS link data buffer */
	ushort	oc_nclists;	/* Number of clist structures */
	ushort	oc_ncallout;	/* Size of callout table */
	ushort	oc_vpsize;	/* Size of virtual memory page */
	ushort	oc_rmsize;	/* Size of real memory */
	ushort	oc_fbsize;	/* Size of file buffers for rfh */
	ushort	oc_nvplanes;	/* Number of video planes (wp5/wp6) */
	ushort	oc_oshalt;	/* O/S halt/panic processing */
	ushort	oc_fsecure;	/* File system security level */
	ushort	oc_mpoolsz;	/* Max size of memory pool in kbytes */
	ushort	oc_notu4;	/* Not used */
	ushort	oc_notu5;	/* Not used */
	ushort	oc_notu6;	/* Not used */
	ushort	oc_notu7;	/* Not used */
	ushort	oc_notu8;	/* Not used */
	ushort	oc_notu9;	/* Not used */
};

#endif /* _DNIX_OSCONFIG_H */


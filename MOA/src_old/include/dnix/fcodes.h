/*
 *	DNIX system function codes
 */
#define	F_OPEN		1	/* open */
#define	F_MVDIR		2	/* Move directory */
#define	F_CREATE	3	/* create */
#define	F_READ		4	/* read */
#define	F_WRITE		5	/* write */
#define	F_IOCR		6	/* i/o control read */
#define	F_IOCW		7	/* i/o control write */
#define	F_UNLINK	8	/* Unlink - delete */
#define	F_LINK		9	/* link */
#define	F_LOCATE	10	/* locate */
#define	F_CPROT		11	/* change protection */
#define	F_CUID		12	/* change user id */
#define	F_CGID		13	/* change group id */
#define	F_MOUNT		14	/* mount */
#define	F_MKNOD		15	/* Create special file */
#define	F_FPROT		16	/* fetch protection */
#define	F_EXCLUS	17	/* exclusive */
#define	F_PIPE		18	/* Allocate a pipe */
#define F_MKDIR		19	/* Create directory */
#define F_RMDIR		20	/* Remove directory */
#define F_ALLOC		21	/* Allocate contigous file */
#define F_LOCKF		22	/* File and record locking */
#define F_BIGHD		22

#define	F_UWRITE	23	/* Write to user buffer */
#define	F_UREAD		24	/* Read from user buffer */
#define	F_WAIT		25	/* Wait */
#define	F_PAUSE		26	/* Pause */
#define	F_TRACE		27	/* Trace */
#define F_DNIXSEM	28	/* Dnix semaphores */
#define F_NAP		29	/* Nap */
#define	F_T2REQ		30	/* Start of type 2 requests */
#define	F_TERMIN	30	/* terminate request */
#define	F_TRPLU		31	/* Terminate requests and pass lu */
#define	F_CANCEL	32	/* cancel */
#define	F_CLOSE		33	/* close */
#define	F_SEEK		34	/* seek */
#define	F_GTIME		35	/* get time */
#define	F_STIME		36	/* Set time */
#define	F_SPRI		37	/* set priority */
#define	F_SUID		38	/* set user id */
#define	F_SGID		39	/* set group id */
#define	F_OTQ		40	/* open trap queue */
#define	F_FORK		41	/* fork */
#define	F_EXEC		42	/* exec */
#define	F_SPAWN		43	/* spawn */
#define	F_AMOUNT	44	/* accept mounting */
#define	F_PASSRQ	45	/* pass on request */
#define	F_INPUT		46	/* input */
#define	F_OUTPUT	47	/* output */
#define	F_MINP		48	/* multiple input */
#define	F_MOUT		49	/* multiple output */
#define	F_AUTO		50	/* Start auto channel driver */
#define	F_GPID		51	/* get process id */
#define	F_GUID		52	/* get user id */
#define	F_GEUID		53	/* Get effectiv user id */
#define	F_GGID		54	/* get group id */
#define	F_GEGID		55	/* Get effectiv group id */
#define	F_SCMASK	56	/* set file creation mask */
#define	F_DLU		57	/* duplicate LU */
#define	F_SDLU		58	/* set duplicate LU */
#define	F_SLUF		59	/* Set lu flag */
#define	F_ORQ		60	/* Open request queue */
#define	F_EXIT		61	/* exit */
#define	F_SIGNAL	62	/* signal */
#define	F_ALARM		63	/* alarm */
#define	F_KILL		64	/* kill */
#define	F_SPLIM		65	/* Set priority limits */
#define	F_BREAK		66	/* Change core allocation */
#define	F_DSPR		67	/* Define speciall process */
#define	F_SPG		68	/* Set process group */
#define	F_LOGC		69	/* Log message on console (suser) */
#define	F_TMKIO		70	/* Terminate request and make io connection */
#define F_PSTAT		71	/* Get process status */
#define	F_OLURQ		72	/* Open lu on request queue */
#define	F_WHREQ		74	/* Write handler information to request */
#define	F_T2URD		75	/* Type 2 read from user buffer */
#define	F_T2UWR		76	/* Type 2 write to user buffer */
#define	F_SEGP		77	/* Segment processing */
#define	F_FCNTL		78	/* File control */
#define F_GPGRP		79	/* Get process group */
#define F_SPGRP		80	/* Set process group */
#define F_GPPID		81	/* Get parent process id */
#define F_UNAME		82	/* Get os info */
#define	F_RDREQ		83	/* Read request or handler info */
#define F_T1REQ		84	/* Do t1 request with uid,gid and pid */
#define	F_GROI		85	/* Get request owner info */
#define F_MEMRES	86	/* Make process memory resident */
#define F_GIVEUPCPU	87	/* Rescedule CPU */
#define F_XXX88		88	/* Free */
#define F_XXX89		89	/* Free */
#define F_XXX90		90	/* Free */
#define F_XXX91		91	/* Free */
#define F_XXX92		92	/* Free */
#define F_XXX93		93	/* Free */
#define F_XXX94		94	/* Free */
#define F_XXX95		95	/* Free */
#define F_XXX96		96	/* Free */
#define F_XXX97		97	/* Free */
#define F_XXX98		98	/* Free */
#define F_XXX99		99	/* Free */
#define F_XXX101	101	/* Free */
#define F_XXX102	102	/* Free */
#define F_XXX103	103	/* Free */
#define F_XXX104	104	/* Free */
#define F_XXX105	105	/* Free */
#define F_XXX106	106	/* Free */
#define F_XXX107	107	/* Free */
#define F_XXX108	108	/* Free */
#define F_XXX109	109	/* Free */
#define F_XXX110	110	/* Free */
#define F_XXX111	111	/* Free */
#define F_XXX112	112	/* Free */
#define F_XXX113	113	/* Free */
#define F_XXX114	114	/* Free */
#define F_XXX115	115	/* Free */
#define F_XXX116	116	/* Free */
#define F_XXX117	117	/* Free */
#define F_XXX118	118	/* Free */
#define F_BSDSTUFF	119	/* BSD Emulation Functions */
#define F_LOGSTRUCT	120	/* Log structure to errlog (suser) */
#define F_TESTTQ	121	/* Test trap queue requests */
#define F_LOCKMEM	122	/* Make memory area DMA'able */
#define F_WSET		123	/* Process working set stat's */
#define F_SYSINFO	124	/* Get/Set various system info fields */
#define F_FSSCTL	125	/* File System Server Control */
#define F_FREEMEM	126	/* Sbreak/Malloc Memory free-er 	*/
#define F_VDATA 	127	/* Virtual data/text file support	*/
#define	F_LAST		127	/* Last of functions codes 		*/

#define	F_MASK		127	/* Mask to extract function code. */

#define	F_NOWAIT	128	/* Nowait bit */

/*
 *	Get/Set Sysinfo (F_SYSINFO) subfunctions
 */
#define F_SYS_GETSAR	 1	/* Read Sysinfo structure */
#define F_SYS_GETOSCFIG	 2	/* Read osconfig structure */
#define F_SYS_SETNODNAME 3	/* Set node name */

/*
 *	Vdata (F_VDATA) options
 */
#define	V_OPEN		1	/* Open & ready vdata file	*/
#define	V_CLOSE		2	/* Close & release vdata file	*/

/*
 *	Freemem (F_FREEMEM) options
 */
#define	FR_RELEASE	0	/* release 'hole' in sbreak'ed memory 	*/


/*
 *	I/O control subfunction codes
 */
#define	SF_STAT		1	/* Stat subfunction */
#define SF_SYNC		2	/* Sync buffers */
#define SF_EXCL		3	/* Make exclusive */
#define SF_FEOF		4	/* Fetch eof pointer */
#define	SF_SFTM		5	/* Set file times */
#define	SF_FLUS		6	/* Flush buffers */
#define SF_DEVP		7	/* Read/Write device parameters */
#define SF_CHSIZ	8	/* Change file size */
#define SF_GETDENT	9	/* Get dir entries (file system indep.) */
#define SF_GETFH	10	/* Get file handle for file */
#define SF_STATFS	11	/* Stat on file system */

#define SF_FORMAT	(('F'<<8)|2)	/* Format. magic no taken from unix. */
#define SF_IDOSFMT	(('F'<<8)|3)	/* IDOS Format floppy 0x20 data pattern */
#define	SF_CTER		16	/* Define this as my controlling terminal */
#define SF_USTAT	17	/* File system statistics */
#define SF_RDCHK	18	/* Probe for data to read */

#ifdef	WP6
	/*
		NOTE:	The WP6 uses NWREADCHK (/dev/ecb, etc.). Also /dev/sound
				uses the upper 3 bytes of the 4 bytes ioctl r_par value
				to pass the khz (frequency) through to the driver.
	*/
#endif

/*				*/
/*	Access mode definitions	*/
/*				*/
/*	On open			*/
/*				*/
#define	AM_READ		0	/* Open for read */
#define	AM_WRITE	1	/* Open for write */
#define	AM_REWR		2	/* Open for read - write */
#define	AM_EXEC		3	/* Open for exec */
/*				*/
/*	On create		*/
/*				*/
#define	CM_OEX		000001	/* Others execute */
#define	CM_OWR		000002	/* Others write */
#define	CM_ORE		000004	/* Others read */
#define	CM_GEX		000010	/* Group execute */
#define	CM_GWR		000020	/* Group write */
#define	CM_GRE		000040	/* Group read */
#define	CM_WEX		000100	/* Owner execute */
#define	CM_WRW		000200	/* Owner write */
#define	CM_WRE		000400	/* Owner read */
#define	CM_STG		002000	/* Set temp group priv */
#define	CM_STU		004000	/* Set temp user priv */
#define	CM_DIR		040000	/* Directory */

/*					
 *	Open privilege definition	
 *	Given by handler on termination	
 *	of open,create and locate	
 */
#define	OP_EXEC		000001	/* Execute */
#define	OP_WRIT		000002	/* Write */
#define	OP_READ		000004	/* Read */
#define OP_APPD		000040	/* Returns random address at append */
#define	OP_NRAD		000100	/* No random address processing */
#define	OP_STRM		000200	/* Stream on data allowed */
#define	OP_CAND		000400	/* Cancel disable */
#define	OP_TGP		002000	/* Temporary group priv. */
#define	OP_TUP		004000	/* Temporary user priv. */
#define	OP_SDIR		010000	/* Search directory priv. */
#define	OP_OFCM		000770	/* Open functions mask */
/*
 *	Some dedicated lu numbers
 */
#define	LU_STDIN	0	/* Standard input */
#define LU_STDOUT	1	/* Standard output */
#define LU_STDERR	2	/* Standard error */
#define	LU_ROOT		21	/* Default root directory lu. */
#define	LU_CDIR		23	/* Default current directory lu. */
/*	Used by unix/xenix interface */
#define	RDIRLU		LU_ROOT	/* Root directory lu */
#define	CDIRLU		LU_CDIR	/* Current directory lu */
/*
 *	Set lu flag definitions
 */
#define	FIOCLEX		(('f'<<8)|1)	/* Close file on exec */
#define	FIONCLEX	(('f'<<8)|2)	/* Do not close file on exec */

/*
 *	Error codes
 *	===========
 *
 *	UNIX Compatible
 */

#define	EPERM	-1		/* Not owner */
#define	ENOENT	-2		/* No such file or directory */
#define	ESRCH	-3		/* No such process */
#define	EINTR	-4		/* Interrupted system call */
#define	EIO	-5		/* I/O error */
#define	ENXIO	-6		/* No such device or address */
#define	E2BIG	-7		/* Arg list too long */
#define	ENOEXEC	-8		/* Exec format error */
#define	EBADF	-9		/* Bad file number */
#define	ECHILD	-10		/* No children */
#define	EAGAIN	-11		/* No more processes */
#define	ENOMEM	-12		/* Not enough core */
#define	EACCES	-13		/* Permission denied */
#define	EFAULT	-14		/* Bad address */
#define	ENOTBLK	-15		/* Block device required */
#define	EBUSY	-16		/* Mount device busy */
#define	EEXIST	-17		/* File exists */
#define	EXDEV	-18		/* Cross-device link */
#define	ENODEV	-19		/* No such device */
#define	ENOTDIR	-20		/* Not a directory */
#define	EISDIR	-21		/* Is a directory */
#define	EINVAL	-22		/* Invalid argument */
#define	ENFILE	-23		/* File table overflow */
#define	EMFILE	-24		/* Too many open files */
#define	ENOTTY	-25		/* Not a typewriter */
#define	ETXTBSY	-26		/* Text file busy */
#define	EFBIG	-27		/* File too large */
#define	ENOSPC	-28		/* No space left on device */
#define	ESPIPE	-29		/* Illegal seek */
#define	EROFS	-30		/* Read-only file system */
#define	EMLINK	-31		/* Too many links */
#define	EPIPE	-32		/* Broken pipe */
#define	EDOM	-33		/* Argument too large */
#define	ERANGE	-34		/* Result too large */
#define ENOMSG	-35		/* No such message */
#define EIDRM	-36		/* Identifier removed */
#define EL2NSYNC -38
#define EL3HLT	-39
#define EL3RST	-40
#define ELNRNG	-41
#define EUNATCH	-42
#define ENOCSI	-43
#define EL2HLT	-44
#define EDEADLK	-45		/* Resource deadlock would occur */

/*
 *	DNIX Specials
 */
#define	EBADLU	EBADF		/* Bad lu */
#define	EUCLEAN	ENOSPC		/* Structure needs cleaning SYSV */
#define	ENONODES -119		/* No more nodes */
#define	EREQAB	-120		/* Request aborted */
#define	EBADFC	-121		/* Illegal function code */
#define	ESWAP	-122		/* Refered process is swapped */
#define	ECANCL	-123		/* Refferred request is canceled */
#define EDRVNF	-124		/* Device driver not found */
#define	ETIMOUT	-125		/* Timeout on driver */
#define ELOCKOK -126		/* Locking is ok - no open node needed */
#define ENOLOCK	-127		/* No locks for this process any more */

#define	EWAIT	-128		/* Request waiting for termination */


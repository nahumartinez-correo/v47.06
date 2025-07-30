/*
 * Structure returned by stat and fstat system calls
 */
#pragma pack (push, 1)
struct	dnixstat
{
	short	st_dev;		/* ID of a file handler managing this file */
	unsigned short st_ino;		/* File serial number */
	unsigned short st_mode;	/* File mode; see #defines below */
	short	st_nlink;	/* Number of links */
	ushort	st_uid;		/* User ID of the file's owner */
	ushort	st_gid;		/* User ID of the file's group */
	short	st_rdev;	/* ID of device */
				/* This entry is defined only for */
				/* special files */
	daddr_t	st_size;	/* File size in bytes */
#undef st_atime
#undef st_mtime
#undef st_ctime
	time_t	st_atime;	/* Time of last access */
	time_t	st_mtime;	/* Time of last data modification */
	time_t	st_ctime;	/* Time of last file status change */
				/* Times measured in seconds since */
				/* 1970-01-01 00:00:00 GMT */
	};
#pragma pack(pop)

#if 0
/*
 *	Symbolic constants for st_mode field
 */
#define	S_IFMT		0170000		/* type of file */
#define	S_IAMB		0000777		/* access mode bits */
#define S_IFIFO		0010000		/* FIFO special file */
#define	S_IFCHR		0020000		/* character special file */
#define S_IFMPC		0030000		/* reserved */
#define	S_IFDIR		0040000		/* directory */
#define S_IFNAM		0050000		/* reserved */
#define	S_IFBLK		0060000		/* block special file */
#define S_IFMPB		0070000		/* reserved */
#define	S_IFREG		0100000		/* regular file */
#define S_IF011		0110000		/* reserved */
/* #define S_IFLNK	0120000		* reserved for symbolic link */
#define S_IF013		0130000		/* reserved */
/* #define S_IFNON	0140000		* reserved for named socket */
#define S_IF015		0150000		/* reserved */
#define S_IF016		0160000		/* reserved */
#define S_IF017		0170000		/* reserved */
#define	S_ISUID		0004000		/* set user id on execution */
#define	S_ISGID		0002000		/* set group id on execution */
#define	S_ISVTX		0001000		/* save swapped text even after use */
#define	S_IREAD		0000400		/* read permission, owner */
#define	S_IWRITE	0000200		/* write permission, owner */
#define	S_IEXEC		0000100		/* execute/search permission, owner */
#define	S_ENFMT		S_ISGID		/* record locking enforcement flag */
#define	S_IRWXU		0000700		/* read, write, execute: owner */
#define	S_IRUSR		0000400		/* read permission: owner */
#define	S_IWUSR		0000200		/* write permission: owner */
#define	S_IXUSR		0000100		/* execute permission: owner */
#define	S_IRWXG		0000070		/* read, write, execute: group */
#define	S_IRGRP		0000040		/* read permission: group */
#define	S_IWGRP		0000020		/* write permission: group */
#define	S_IXGRP		0000010		/* execute permission: group */
#define	S_IRWXO		0000007		/* read, write, execute: other */
#define	S_IROTH		0000004		/* read permission: other */
#define	S_IWOTH		0000002		/* write permission: other */
#define	S_IXOTH		0000001		/* execute permission: other */	


/*
 *	File type macros
 */
#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)

/*
 *	Function prototypes
 */
extern	int	stat	 _PROTO((const char *path, struct stat *buf));
extern	int	fstat	 _PROTO((int fildes, struct stat *buf));
extern	int	chmod	 _PROTO((const char *path, mode_t mode));
extern	mode_t	umask	 _PROTO((mode_t cmask));
extern	int	mkdir	 _PROTO((const char *path, mode_t mode));
extern	int	mknod	 _PROTO((const char *path, mode_t mode, int dev));
extern	int	mkfifo	 _PROTO((const char *path, mode_t mode));
#endif

/* Needed to get good compiles under Unix */

#if 0
#define	S_IEXEC		0000100		/* execute/search permission, owner */
#define	S_ISUID		0004000		/* set user id on execution */
#define	S_ISGID		0002000		/* set group id on execution */
#define	S_IFMT		0170000		/* type of file */
#define	S_IFDIR		0040000		/* directory */
#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#endif

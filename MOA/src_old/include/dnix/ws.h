/*$
**
**	 Structures and #defines for the Unix SVR4 FSS implementation
**
*/

#define FH_ALLOC	100		/* 64 fc = file handler routine */
#define FH_DEALLOC	101		/* 65 */
#define FH_LOAD		102		/* 66 */
#define FH_STAT		103		/* 67 */
#define FH_CLOSE	104		/* 68 */
#define FH_CLOSERQ	105		/* 69 */
#define FH_CTTY		106		/* 6A */
#define FH_READ		107		/* 6B */
#define FH_WRITE	108		/* 6C */
#define FH_IPMNT	109		/* 6D */
#define FH_IPUMNT	110		/* 6E */
#define FH_SWAPIN	111		/* 6F */
#define FH_SWAPOUT	112		/* 70 */
#define FH_START	113		/* 71 */
#define FH_STOP		114		/* 72 */
#define FH_KILL		115		/* 73 */
#define FH_CLOCK	116		/* 74 */
#define FH_ACCESS	117		/* 75 */
#define FH_QSTAT	118		/* 76 */

#define WSBUFSZ	1380

#pragma pack (push, 1)
struct wsbuf
{
	int			w_slid;			/* SAFE LINK id */
	struct wsbuf *w_next;		/* Link to next buffer */
	int			w_id;			/* Machine id provided by FSS */
	int			w_bufsz;		/* Size of buffer to transfer */
	int			w_pri;			/* Priority of request */
	int			w_rpar;			/* Ruturn parameter */
	int			w_fc;			/* Function code */
	int			w_bcnt;			/* Buffer size */
	int			w_prad;			/* Random address */
	int			w_fid;			/* inode pointer */
	int			w_par;			/* Parameter */
	int			w_kpar;			/* Second parameter */
	int			w_flags;		/* Request flags */
	int			w_dev;			/* Device */
	int			w_uid;			/* User id */
	int			w_gid;			/* Group id */
	int			w_nph;			/* Node and process id */
	int			w_npl;			/* More node and process id */
	int			w_oflag;		/* Open node flags */
	int			w_offlag;		/* More open node flags */
	int			w_rdfid;		/* Root directory inode pointer */
	int			w_ext1;			/* For future extensions */
	int			w_ext2;			/* For future extensions */
	int			w_ext3;			/* For future extensions */
	int			w_ext4;			/* For future extensions */
	char		w_buf[WSBUFSZ];	/* Buffer for data */
};

#pragma pack(2)

struct touch {		/* Structure used to pass inode touch information */
	int	t_fid;
	short	t_flags;
};

#define WSHEADERSZ	((int)sizeof(struct wsbuf) - WSBUFSZ)	/* 100 */
#define WSDEV		makedev(17,0x40)
	
/*$
**
**	Following structure holds statistics for station
**
*/
struct wsstat {
	struct dutsname	st_sysinfo;	/* Workstations sysinfo structure */
	int		st_level;		/* File server level */
	int		st_online;		/* Number of connections */
	int		st_offline;		/* Number of broken connections */
	time_t	st_time;		/* Last connect/disconnect time */
#undef st_ctime
	time_t	st_ctime;		/* Acc total connect time */
	time_t	st_dtime;		/* Acc total disconnect time */
	int		st_req;			/* Number of requests */
	int		st_rxdata;		/* Amount of data received */
	int		st_txdata;		/* Amount of data transmitted */
	int		st_read;		/* Number of read requests */
	int		st_szread;		/* Number of bytes read */
	int		st_write;		/* Number of write requests */
	int		st_szwrite;		/* Number of bytes written */
	int		st_ofile;		/* Number of open files */
	int		st_maxofile;	/* Max number of files opened */
	int		st_swapblk;		/* Number of swap blocks allocated */
	int		st_maxblk;		/* Max number of swap blocks alloc'd */
	int		st_load;		/* Amount of data loaded (exec) */
	int		st_swapout;		/* Amount of data swapped out */
	int		st_swapin;		/* Amount of data swapped in */
	int		st_loadread;	/* Amt load data read from disk to page cache */
	int		st_swapread;	/* Amt swap data read from disk to page cache */
	short	st_flags;		/* Status flags copied from s_flags */
};

/*
 *	Structures local to file system server
 */


/*
 *	Structure defining an open file in FSS opened by a remote
 *	work station.
 */
struct fdesc {
	int		f_ip;		/* Unix file number */
	ushort	f_inode;	/* inode number */
	ushort	f_mode;		/* access bits */
	ushort	f_uid;		/* file's USERID */
	ushort	f_gid;		/* file's GROUPID */
	short	f_locked;	/* TRUE if file is locked */
};

/*
 *	Structure defining a mounted handler in FSS opened by a remote
 *	work station.
 */
struct hdesc {
	int		h_ip;			/* Unix file ID + 2048 */
	int		h_prad;			/* address of open node in WS */
	int		h_namelen;		/* length of filename in bytes */
	char	*h_filename;	/* Pointer to ASCII Unix path/filename */
};


/*
 *	Structure defining a connection of a remote work station
 */

#define WSNAMESZ		9		/* Length of workstation name */
#define FSS_MAXLINKS	64		/* Max # of softlinks per workstation */
#define	FSS_MAXFILES	144		/* Max # of open files per workstation */
#define	FSS_MAXGHOSTS	32		/* Max # of open ghost devices */
#define	FSS_MAXHANDLERS	20		/* Max # of open handlers per worstation */
#define	FSS_MAXSWAPTBLS	8		/* Max # of Swap Block Tables per workstation */
#define	FSS_MAXWSTN		64		/* Max # of workstations supported */
#define	SWAPBLOCK_COUNT	2048	/* Max # of swap blocks per swap table */

struct workstn {
	int				s_slid;				/* SAFE LINK id */
	int				s_pid;				/* Child process ID */
	short			s_datasize;			/* Max bytes in req/response */
	short			s_pagesize;			/* Workstation page size */
	short			s_flags;			/* Some flags */
	char			s_name[WSNAMESZ];	/* Name of workstation */
	struct wsstat	s_stat;				/* Statistics on station */
										/* Table of open fd's */
	struct fdesc	s_files[FSS_MAXFILES];
										/* Table of Mounted handlers */
	struct hdesc	s_hndl[FSS_MAXHANDLERS];
										/* Table of ghost device names */
	int				s_hndl_fence;		/* hi-water mark in s_hndl[] array */
	char		   *ghost_dev[FSS_MAXGHOSTS];
										/* table of swapblocks */
	unsigned char	s_swapblock[SWAPBLOCK_COUNT];
	char		   *s_swapfile;			/* pointer to RAM swapfile */
	short			s_swap_hiwater;		/* # of swapblocks currently alloc'd */
	short			s_nfiles;			/* # of open files */
	short			s_nhndl;			/* # of mounted handlers */
	int				s_msgs_in;			/* # of messages received from wkstn */
	int				s_msgs_out;			/* # of messages sent to workstn */
	short			s_count;			/* # of active requests  */
};

#pragma pack (pop)

/*
 *  Workstation status flags
 */
#define SF_ACTIVE	0x0001	/* Workstation is activated */
#define SF_CLOSING	0x0002	/* Workstation is closing down */
#define	SF_OPEN		0x0004	/* Workstation is open for normal traffic */
#define SF_CLOSPND	0x0008	/* Workstation has close pending */

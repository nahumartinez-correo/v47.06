/*$
**
**    Defines for the ISC-BR Workstation FileServer (Unix SVR4 version)
**
*/

#ifndef FALSE
	#define FALSE			(0)
	#define TRUE			!FALSE
#endif
#define WSBUF				(sizeof(struct wsbuf)-WSBUFSZ-4) 	/* 96 */
#define INFO				1
#define RR					2
#define REJ					3
#define RNR					4
#define SABM				5
#define UA					6
#define FRMR				7
#define DISC				8
#define DM					9
#define MUI					10

#define DNIXROOT			"/dnix/"		/* S5 file system on ONP */
#define GHOST_DEVICE		1024
#define WS_ACTIVE			0x0100
#define WS_ONLINE			0x0200

/*$
**	         DNIX system function codes
*/
#define	DNIX_OPEN		1	/* Open */
#define	DNIX_MVDIR		2	/* Move directory */
#define	DNIX_CREATE		3	/* Create */
#define	DNIX_READ		4	/* Read */
#define	DNIX_WRITE		5	/* Write */
#define	DNIX_IOCR		6	/* I/O control read */
#define	DNIX_IOCW		7	/* I/O control write */
#define	DNIX_UNLINK		8	/* Unlink - delete */
#define	DNIX_LINK		9	/* Link */
#define	DNIX_LOCATE		10	/* Locate */
#define	DNIX_CPROT		11	/* Change protection */
#define	DNIX_CUID		12	/* Change user id */
#define	DNIX_CGID		13	/* Change group id */
#define	DNIX_MOUNT		14	/* Mount */
#define	DNIX_MKNOD		15	/* Create special file */
#define	DNIX_FPROT		16	/* Fetch protection */
#define	DNIX_EXCLUS		17	/* Exclusive */
#define	DNIX_PIPE		18	/* Allocate a pipe */
#define DNIX_MKDIR		19	/* Create directory */
#define DNIX_RMDIR		20	/* Remove directory */
#define DNIX_ALLOC		21	/* Allocate contigous file */
#define DNIX_LOCKF		22	/* File and record locking */
#define DNIX_BIGHD		22

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

#if 0
#define SF_FORMAT	(('F'<<8)|2)	/* Format. magic no taken from unix. */
#define SF_IDOSFMT	(('F'<<8)|3)	/* IDOS Format floppy 0x20 data pattern */
#define	SF_CTER		16	/* Define this as my controlling terminal */
#define SF_USTAT	17	/* File system statistics */
#define SF_RDCHK	18	/* Probe for data to read */
#endif

#define DEBUG_D			0x01	/* Command line debugging */
#define DEBUG_F			0x02	/* Show details of file I/O */
#define DEBUG_N			0x04	/* Show network traffic info */
#define DEBUG_P			0x08	/* Show network packets */
#define DEBUG_T			0x10	/* Dump internal tables at exit */
#define DEBUG_W			0x20	/* Display WSBUF structure in packets */
#define DEBUG_C			0x40	/* OPEN/CLOSE file debugging */

/* ---------------------------------------------------------------- */
/*  Debug output is enabled by #define'ing the symbol DEBUG_OUTPUT  */
/*  and swapping the #defines below as desired.  PACKETS dumps all  */
/*  inbound/outbound control and data packets, TRACE shows 1 line   */
/*  per message in/out, and DETAILS is for other misc. info (like   */
/*  timers, etc.).													*/
/* ---------------------------------------------------------------- */
#ifndef DEBUG_OUTPUT
 #define DETAILS(a)
 #define FILEIO(a)
 #define NETWORK(a)
 #define PACKETS(a)
 #define TABLES(a)
 #define SHOWWSB(a)
#else
 #define DETAILS(a)	if (Debug & DEBUG_D) a	/* misc. internal info */
 #define FILEIO(a)	if (Debug & DEBUG_F) a	/* all file input/output */
 #define NETWORK(a)	if (Debug & DEBUG_N) a	/* non-packet network info */
 #define PACKETS(a)	if (Debug & DEBUG_P) a	/* all Ethernet packets in/out */
 #define TABLES(a)	if (Debug & DEBUG_T) a	/* internal tables are dumped */
 #define SHOWWSB(a)	if (Debug & DEBUG_W) a	/* display wsbuf structure */
#endif
#define PROFILE(a)	a	/* profiling of function call info */

	struct Etheraddr {
		unsigned char addr[6];
		};

	/* fs.c  */
	void		error(int die, char *string);
	void		my_exit(int rval);
	int			unix_file_id(int wsid, int w_fid);
	char		*unix_file_name(int wsid, int w_fid);

	/* fs_debug.c  */
	void		dump_elc_buffers(void);
	void 		dump_profile_info(int (*func)());
	char		*func_name(int func);
	int			lapb_type(char *buff);
	void		show_multicast(void);
	void		show_stat(struct dnixstat *db);
	void 		show_termio(struct termio *tp);
	void		show_wsb(struct wsbuf *tws, char *title, char *subtitle);

	/* fs_net.c  */
	int 		offer_service(void);
	int			nrns(int byte);
	int			get_a_packet(void);
	int			get_a_frame(void);
	void		put_a_packet(int length);
	void		setup_elc_buffers(void);
	void		must_get_RR(void);

	/* fs_tables.c  */
	void		init_wstable(char *onp_name);
	int			fss_emptyslot(void);
	char		*apply_softlink(char *filename, int wsid);
	void		dump_wstable(void);
	void		init_softlinks(void);
	void		dump_softlinks(void);
	int			fss_wstn(char *namep);
	void 		setup_wsnames(int flag);

	/* hexdump.c  */
	void		hexdump(FILE *fh, char *title, int oset, void *f,
						short len, short size);

	/* fs_misc.c  */
	int 		add_handler(int wsid,char *modfn,struct stat *sb,int prad);
	void 		become_other(int uid, int gid);
	void 		become_self(void);
	int 		chkownr(struct stat *sb, int fid, int wsid);
	void 		clear_stats(int wsid);
	int 		get_open_bits(struct stat *sb, int am, int oflag, int wsid);
	void		init_poll_stuff(void);
	int 		is_handler(int wsid,char *modfn, int *len);
	void		sac_command(void);		
	void 		sac_log(char *);
	void		sac_setup(void);
	void		sac_write(void);
	void 		show_wstable(int wsid, int (*func)());

	/* fs_ipc.c */
	int		 	check_ipc(void);
	void		ipc_init(void);

	/* fs_lock.c */
	int			local_fcntl(int wsid, int cmd, int dfd, flock_t *svr4);
	int			local_lockf(int wsid, int dfd, int cmd, int size);

	/* fs_x25.c */
	int			x25ioctl(int fd, int cmd, void *buf, int len, int timeout);

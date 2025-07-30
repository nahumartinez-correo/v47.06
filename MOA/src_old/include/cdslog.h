#ifndef CDSLOG_H
#define CDSLOG_H

#ifndef CDSLIMITS_H
#	include <cdslmts.h>
#endif

#define	SYNC_PERIOD	60	/*60 seconds between O/S generated sync's*/
#define MIN_LOG_LENGTH sizeof(log_rec) + sizeof(log_reclen)
#define MAX_LOG_LENGTH 0x00020000 /* this big for pre-images are included*/
#define KLB 1	/* bytes containing key length */

#define APPLY_LOG_FORWARD "APPLY_LOG"
#include <cdslgdf.h>

typedef	long	log_reclen;

#pragma pack (push, 2)
typedef	struct	log_rec	/* recovery log record header */
{
	log_reclen	len;
	time_t	time_stamp;		/* seconds since epoc */
	byte	action;			/* must be one of log actions*/
	byte	luw;			/* begin, commit, continue or null */
	byte	table_number;	/* from tables table */
	byte	other_luws;		/* used only on commit records */
	void	*user;			/* derived number for client task */
	char    table_ID[8];	/* internal table name */
} log_rec;

typedef struct gtran_id
{
	char	user_name[USER_NAME_LEN + 1];
	char	machine[MACHINE_NAME_LEN];
	int		process_id;
	unsigned long luw_id;	/* based on time stamp at client */
} gtran_id;

typedef struct	shared_quiet
{
	volatile long	last_echoed_quiet;	/* seek address in echo log */
	volatile long	last_read_quiet;
	volatile long	check_sum;		/* used only at reset time */
	volatile char	reset_notice[4];	/* == REST when need to reset log */
	pid_t   el_pid;			/* process_id of el process */
} shared_quiet_mem;

typedef struct	commit_table {
	short	status;
	short	commit_found;
	cdsselect	*context;
} commit_table;
enum	table_index_state	{ // used in commit_table status
	UNKNOWN=0,
	MUST_REBUILD,
	SUSPENSE
};

#pragma pack (pop)
extern OVERLAPPED LogReadOverlap;
global_var short  Max_table_number;
global_var	bool	Dont_Log,Log_disabled, Rolling_Back;
#ifdef NT
# include <logio.h>
global_var LOGFILE Log_fd, Echo_fd;
#else
global_var	int		Log_fd, Echo_fd;
global_var LogPipe0[2];
global_var LogPipe1[2];
#endif
global_var bool NoFlush;
global_var	void	*Initial_Table_Sizes;
global_var	long	Initial_Log_Size, Log_eof;
global_var	char	*Log_path;
global_var	time_t	Last_Log_Time;


global_proc	bool do_flush( void );
global_proc	long	cdslog_open( void );
global_proc	bool cdslog(void *ccb, byte action, cdsselect *context, const char *buf, luw_flag luw);
global_proc	long	find_log_eof(LOGFILE fd);
#ifdef NT
#define FindLogEof(fd) find_log_eof(fd)
#else
global_proc	long	FindLogEof(LOGFILE fd);
#endif
global_proc	long	read_til_end(LOGFILE fd);
global_proc	bool	verify_log_record(regpar log_rec *log);
global_proc	char	*cdsReadBackward(LOGFILE fd, long *position);
global_proc	char	*cdsReadForward(regpar LOGFILE fd);	/* fd is file_descriptor */
#ifdef NT
#define log_seek(fd, position, seektype) LogSeek(fd, position,seektype)
#else
global_proc	long	log_seek(LOGFILE fd, long position, int seektype); /* returns on error */
#endif
global_proc	long	logSeek(regpar LOGFILE fd, regpar long position, int seektype); /* no return on error */
global_proc	long	logSeek_and_set_eof(LOGFILE fd, long position, int seektype);
global_proc	log_rec *last_action(LOGFILE fd, long *eof_position);	/* fd is file_descriptor */
global_proc	log_rec *last_clean_point(LOGFILE fd, long *eof_position);	/* fd is file_descriptor */
#ifdef NT
#define ReadForward(fd) cdsReadForward(fd)
#define ReadBackward(fd, position) cdsReadBackward(fd, position)
#define Last_Action(fd, eof_position) last_action(fd, eof_position)
#define LastCleanPoint(fd, eof_position) last_clean_point(fd, eof_position)
#else
global_proc	char	*ReadForward(LOGFILE fd);	/* fd is file_descriptor */
global_proc	char	*ReadBackward(LOGFILE fd, long *position);
global_proc	log_rec *Last_Action(LOGFILE fd, long *eof_position);
global_proc	log_rec *LastCleanPoint(LOGFILE fd, long *eof_position);
#endif
global_proc int		rollforward(LOGFILE fd, long log_eof, log_rec *log_record, commit_table *ctable);
/*lint -e565 */
#ifdef CONNECTCB_H
global_proc void rollback(struct connectcb *user);
global_proc void clear_pending_unlocks(struct connectcb *user);
global_proc void restart_2_phase_commit(struct connectcb *user, log_rec *lrec);
global_proc void write_pipe(const char *buf, size_t len);
global_proc void read_pipe(char *buf);
#endif
/*lint +e565 */
global_proc	void do_rollbacks(long eof_position);
global_proc long	roll_forward_from_echo(commit_table table_array[],
					       long local_log_length,
					       long max_length);
global_proc	int quiet_point( void );
global_proc void cdslog_quiese( void );
global_proc void mark_death( void );
global_proc	void	reset_out_file(void);

#endif


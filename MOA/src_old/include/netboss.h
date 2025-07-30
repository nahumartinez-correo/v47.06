/* conman state flag values */

#define	INITIAL_CONNECT	0
#define	ACTIVE_NEGOTIATING	1
#define	PASSIVE_NEGOTIATING 2
#define	ACTIVE_BOSS	3
#define	ACTIVE_SECONDARY 4

#define NETBOSS			1
#define NETBOSS_OPENED	2
#define SECONDARY		3
#define RETRY			4
#define RESOLVE			5

#define	RD  0
#define	WR  1

/* netmgr replacement command timeout values */
#define SPL_CMD_TIMEOUT   300
#define USER_CMD_TIMEOUT  600

/* NetBoss internal structures / flags */

/* flags mask definitions */
#define NB_SPOOLED	0x01	/* command should be spooled if not executed */

#define put_long put_int
#define get_long get_int

extern NET_NODE *theNetwork;
extern server *theNetcbs[];
extern server	boss;
extern server		*self;		/* control block pointer for self */
extern server		*processing_join;
extern char *mysystem;
extern short num_clients;
extern int	state;						/* global state of conman */

extern char	lupd_time[];

extern CSS_NBS_STAT rsp_stat;
extern CSS_NBS_STAT req_stat;

extern int		lupd_fd;
extern short	exec_cnt;
extern short	num_nodes;
extern short	num_connected;
extern int 		spool_timeout;
extern int 		user_timeout;
#ifndef DNIX
extern iocb *Iocb_freelist;
extern partner *Partner_freelist;
extern int SACyes;
#endif

#define CS_NBSCMD		'P'        /* NETBOSS COMMAND				*/

void nb_allocate(register CSS_ALLOCATE *, register server *, struct buffer *, register int);
void nb_comreq(register CSS_COMM_REQ *, register server *, struct buffer *, register int);
void nb_data(register CSS_DATA *data, register server *sp, struct buffer *bp);
void nb_identify(register CSS_IDENTIFY *identify, server *sp, struct buffer *bp);
void nb_estatus(register server *sp);

int getlnetid(register char *buf);
void error(register int level, register int number);
void dispose_context(register server *sp);
void exec_shell_rsp(register CSS_NBS_EXEC_SHELL *exrsp,
	register struct buffer *bp, server *nb);
#ifdef DNIX
int memcmp(register char *s1, register char *s2, register int n);
#else
void wrcomp(iocb * ip, int rpar, int rn);
struct buffer * deque_incoming(struct server *sp);
#endif
void * get_struct(void *free_list, int size);
void free_struct(void *free_list, void *element);



/* ### move these to buffer.h later */
#define FLAG_F		0x04	/* first element */
#define FLAG_L		0x08	/* last element */
#define FLAG_SC		0x10	/* spool the command if required */
#define FLAG_RDB	0x20	/* Resolve Database check for conflicts */
#define FLAG_FS		0x40	/* command from spool file */
#define FLAG_D		0x80	/* last of all the elements (done) */

#define ACI_TYPE	1
#define NB_TYPE		2
#define CL_TYPE		3
#define NBS_TYPE	4

/* Network i/f definitions */

#define	WINDOWSIZE	2
#define	PACKETSIZE	1024

void *process_open(register void *id);
void process_opened(register server *sp);
void process_close(register server *sp);
void process_msg(register server *sp, register struct buffer *bp);
void process_oobmsg(register server *sp, register struct buffer *bp);
void que_brdcst(register struct server *, register struct buffer *, char *);
void send_data_msg(register void *, register server *, struct buffer *);

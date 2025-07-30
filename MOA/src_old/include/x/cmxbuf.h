
/*
 *	copyright (c) 1989	ISC Systems Corporation
 */

#ifndef	CMXBUF_H
#define	CMXBUF_H

# ifndef	CMBUF_H
#	include	<cm/cmbuf.h>
# endif
# ifndef CDSLIMITS_H
#	include	<cdslmts.h>
# endif
# ifndef DATAMAN_H
#	include <dataman.h>
# endif
#ifndef CLENV_H
#include "clenv.h"
#endif

#define MAXTSDU_OVHD 4
#define BP_FINDER 0xBEBEBEBE
/* values for b_flags */
#define CM_READ_RESULT_OBTAINED 2
#define CM_READ_CANCELED 4
#define CM_BUFFER_ORPHANED 64
typedef struct cmbuffer {
	struct cmbuffer *b_next;
	struct clconnect *ccb;		/* owner */
	int finder;
	unsigned short b_ID;
	unsigned short b_flags;
	unsigned short b_start;
	unsigned b_size_used;
	unsigned b_size;
	unsigned char *cmbuf;
	unsigned char *b_data;
} cmbuffer;
#pragma pack (push, 2)
typedef struct clconnect {
	struct	clconnect	*next;	/* list manager structure */
	struct	clconnect	*prev;
	short	flags;				/* list manager */
#include "clccb.h"
	cmbuffer *freebuf; /* list of free buffers */
	cmbuffer *read_already;
	cmbuffer *nbp;
	cmbuffer *pending;  /* not yet read */
	HWND	cWnd;
	short closing;	// 1 if closing
} clconnect;
#pragma pack (pop)
#define fromSender_fd sock
#define toReceiver_fd fromSender_fd

extern	addr Sv_result;

void	cmbufenough(cmbuffer *bp, const char *caller,
				addr **offptr, addr *dataptr, pos need);
void	cmbreget(cmbuffer *, pos);
void	cmbreplace(clconnect *, void *, pos);
void	cmbufinit(clconnect *);
addr	cmbstdset(clconnect *);
void	cmbufdump(cmbuffer *, char *);
void	clrbufhptr(clconnect *);			   
void	clrbuf(clconnect *);
cmbuffer *cmgetebuf(clconnect *ccb);
void cmrelbuf(cmbuffer *bp);
int	cmrecv(int	code, clconnect *ccb, void *clientrb, bool commdbg);
int	cmsend(void *request_block_address, pos function_code, cmbuffer *bp, bool commdbg);
addr cmbufp(cmbuffer *bp);
void cmfreebuf(cmbuffer *bp);
# endif


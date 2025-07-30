
/*
 *	copyright (c) 1989   ISC Systems Corporation
 */

	/* flags to keep various statistics */

# define	S_SEND		1
# define	S_CTLSEND	2
# define	S_RECEIVE	3
# define	S_CTLRECEIVE	4
global_proc void s_stat(cmbuffer *bp, pos fcode, pos sflag);

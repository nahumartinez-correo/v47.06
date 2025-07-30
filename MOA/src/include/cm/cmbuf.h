
/*
 *	copyright (c) 1989	ISC Systems Corporation
 */

# ifndef	CMBUF_H
#	define	CMBUF_H

#ifndef _Hc
# include	<c.h>
#endif
#ifndef CDSMACRO_H
#   include <cdsmacro.h>
#endif

#define bufoffset(buf, vl)	(addr)((addr)(buf) + GET_INT(vl))
#define	OFFSET(a,b)	( (addr)a - (addr)b )
#define PORTINC 1

/*
 * 	common LAN message buffer header
 */
typedef	struct
{
	char	flag;		/* more flag */
	char	filler;
	char    fill;
	char    code;		/* function code */
	void	*request_block; /* request_ID */
	size_t  size;		/* the size of the message (includes header) */

}	cmbufhdr;
# define	FLAG_NORMAL	'\0'
# define	FLAG_MORE	'\002'

# define	CMZSTDBUF	1024	/* std buffer size */
# define	CMZCHUNK	4096	/* buffer expansion chunk size */

# endif

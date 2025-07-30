	/*  CDS CLIENT REQUEST CONTROL BLOCK */
/* to use clrb.h, you should include either clconnect.h or x/cmxbuf.h **/

#ifndef  CLRB_H
#	define CLRB_H
#	ifndef CMXBUF_H
#	define NOCMXBUF_H 1
#	else
#	define NOCMXBUF_H 0
#	endif
typedef struct cds_request
{
		struct cds_request *next;
		struct cds_request *prev;
		short	flags;
		byte	finder;	/* value VALID_CLRB in valid request blocks */
#define VALID_CLRB 0xCB
		byte	function_code;   /* from ft/ftop.h */
		void	*fn_dependent;	/* for no_wait, holds fn dependent information */
		void	*remote_ptr;	/*pointer to object in server memory */
		void	*connect;		/* client to server connection control block*/
		void	*msg;	/* for caching responses received out of sequence */
		int		msg_space_size; /* size of cache buffer */
} clrb;

#ifdef CLCONNECT_H
#	define NOCLCONNECT_H 0
#else
#	define NOCLCONNECT_H 1
#endif
/*lint -e553 it's oK if these aren't defined */
#if NOCLONNECT_H + NOCMXBUF_H
/* don't define the following if clconnect is not typedef'ed */
#else
global_var clrb *Rblhead, *Rblfree;

global_proc clrb *create_clrb(regpar clconnect *connect,
							regpar void *remote_ptr,	/* usually context pointer */
							int function_code);
global_proc void destroy_clrb(regpar clrb *rb);
global_proc void release_clrb(regpar clrb *rb);
global_proc clrb *find_clrb(regpar clconnect *connect, regpar void *remote_ptr);
global_proc void close_connect_clrbs(regpar clconnect *connect);
#endif
#undef NOCMXBUF_H
#undef NOCLCONNECT_H
/*lint +e553 */
#endif


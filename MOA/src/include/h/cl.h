
/*
 *	copyright (c) 1989	ISC Systems Corporation
 */

#ifndef	_Fclx
#	define	_Fclx

#ifndef  _Hc
#	include	<c.h>
#endif
#ifndef	_Ffttypes
#	include	<ft/ftop.h>
#endif
#ifndef CMXBUF_H
#include	<x/cmxbuf.h>
#endif
#ifndef CLRB_H
#include	<clrb.h>
#endif

global_proc	void	*clrun (regpar clconnect *connect, regpar pos code);
global_proc	clrb	*clnwrun (regpar clconnect *connect, regpar pos code);
#ifndef XPDATA
#define XPDATA extern
#endif
XPDATA char Euname[];
XPDATA clconnect *ccb_head;	
#endif


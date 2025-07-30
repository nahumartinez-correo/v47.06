/*$
** MODULENAME - listappnd.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.9 $
** $Date: 1996/04/04 17:20:58 $
**
** Synopsis:
**   
** External Function List:
**   listappend()
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include "pch.h"

#include <typedefs.h>
#include <defines.h>
#include <rtprotos.h>
#include <ppdefs.h>
#include <convers.h>
#include <rtdef.h>
#include <dataman.h>
#include <pclwords.h>
#include <errdefs.h>
#include <clconn.h>
#include <cdsfuncs.h>
#include <cmfield.h>
#include <clcdsfn.h>
#ifndef CLCDSENV_H
#include <clcdsenv.h>
#endif
#include <rtable.h>
#include "cdscsr.h"
/* ---------------------------------------------------------------- */
/* 	listappend()		           										*/
/*	Append a set of fields referenced in a compiled list.           */
/*  Usage: call CSR:listarray context_name compiled_list            */
/* ---------------------------------------------------------------- */
XDLL int 
listappend(int argc, rtobj *argv[], int type)
{
	complist		*lp;
	int				mcount, ecount, membersize;
	byte			*mp, *ep;
	byte			ano;
	index			ino;
	field			*fp;
	short			ms;
	char			buf[256];
	char			fn_listappend[]="listappend";
	cdsselect		*tptr;

	/* obtain select context */
	if (rtobj2str(argv[0],buf,sizeof(buf)) == RT_ERROR) {
		swerrorvmsg(buf,CSR_BAD_CONTEXT,fn_listappend,1);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	if ((tptr = find_clcontext(CdsEnv, buf)) == NULL) {
		swerrorvmsg(buf,CSR_BAD_CONTEXT,fn_listappend,1);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	/* the parameter has to be a list or indirect field	*/
	if (argv[1]->obj.object != LST && argv[1]->obj.object != FLD &&
		argv[1]->obj.object != TYPE_ARRAY) {
		rterrorbox(swerrormsg(LS_FAIL_ARGTYPE));
		return(RT_ERROR);
		}
	/* if parameter is a field make sure it is an indirect field
	   that points to a list */
	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND) {
			if (*(fp->storage) == LST) {
				ino = (fp->storage[2] << 8) + fp->storage[3];
				if ((lp = (complist *)ddget(RTddh, *(fp->storage),
					fp->storage[1], ino)) == 0) {
					rterrorbox(swerrormsg(LS_NOLIST));
					return(RT_ERROR);
					}
				}
			else {
				rterrorbox(swerrormsg(LS_NOLIST));
				return(RT_ERROR);
				}
			}
		else {
			rterrorbox(swerrormsg(LS_NOIND));
			return(RT_ERROR);
			}
		}
	else { /* get a pointer to the list	*/
		if ((lp = (complist *)ddget(RTddh, argv[1]->obj.object,
			argv[1]->obj.applno, argv[1]->obj.itemno)) == 0) {
			rterrorbox(swerrormsg(LS_NOLIST));
			return(RT_ERROR);
			}
		}
	mp = (byte *)(lp + 1);
	mcount = lp->numitems;
	while (mcount--) {
		/* skip over MEMBER token, element count and member size */
		ep = mp + 4;
		ecount = (int)(*(mp + 1));
		while (ecount--) {
			if (*ep == FLD || *ep == TYPE_ARRAY) { 
				fp = rt_get_fp(ep,&ms,NULL);
				if (fp) {
					ep += ms - 3;
					ano = *ep++;
					ino = *ep++ << 8;
					ino |= *ep++;
					if (rtaddfield(tptr,fp,ano,ino,fp) == RT_ERROR) {
						swerrorvmsg(buf,CSR_BAD_TL_APPEND,ano,ino);
						rterrorbox(buf);
						return(RT_ERROR);
						}
					}
				else
					ep += ms;
				}
			else
				ep += tsize(ep);
			}
		/* increment mp to the next member	*/
		membersize = (mp[2] << 8) + mp[3];
		mp += (membersize + 4);
		}
	return(RT_SUCCESS);
}


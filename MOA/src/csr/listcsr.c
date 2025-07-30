/*$
** MODULENAME - listcsr.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.12 $
** $Date: 1996/03/08 19:37:54 $
**
** Synopsis:
**
** External Function List:
**   listaddfields
**   listdupfields
**   listfields
**   listexec
**   listcsr
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/

#include "pch.h"

#include <ppdefs.h>
#include <typedefs.h>
#include <defines.h>
#include <rtprotos.h>
#include <convers.h>
#include <rtdef.h>
#include <dataman.h>
#include <ofbwords.h>
#include <errdefs.h>
#include <ofbdefs.h>
#include <clconn.h>
#include <cdsfuncs.h>
#include <cmfield.h>
#include <clcdsfn.h>
#ifndef CLCDSENV_H
#include <clcdsenv.h>
#endif
#include <rtable.h>

	XDLL int listaddfields(int argc, rtobj *argv[]);
	XDLL int listdupfields(int argc, rtobj *argv[]);
	XDLL int listfields(int argc, rtobj *argv[], int type);
	XDLL int listexec(int argc, rtobj *argv[]);
	XDLL int listcsr(int argc, rtobj *argv[]);

/* ---------------------------------------------------------------- */
/* 					listaddfields 									*/
/*  'Touch' all fields in a compiled list by calling add_field_rec  */
/*  for each field referenced in the list							*/
/* ---------------------------------------------------------------- */

XDLL int 
listaddfields(int argc, rtobj *argv[])
{
	return(listfields(argc, argv, 0));
}

/* ---------------------------------------------------------------- */
/* 					listdupfields 									*/
/*  Put what's in the dup area into field storage from a list 		*/
/*  using dup_fld_explicit routine.									*/
/* ---------------------------------------------------------------- */

XDLL int 
listdupfields(int argc, rtobj *argv[])
{
	return(listfields(argc, argv, 1));
}



/* ---------------------------------------------------------------- */
/* 					listfields 										*/
/*  If type == 0 'touch' all fields in a compiled list by calling   */
/*  add_field_rec for each field referenced in the list.			*/
/*  if type == 1 put what's in the dup area into field storage		*/
/*  from a compiled list by calling dup_fld_explicit.				*/
/* ---------------------------------------------------------------- */

XDLL int 
listfields(int argc, rtobj *argv[], int type)
{
	complist		*listptr;
	int				membercount,
					elcount,
					membersize;
	byte			*memberptr,
					*elptr;
	index			itemno;
	field			*fp;
	short			ms;

	/* the parameter has to be a list or indirect field	*/
	if (argv[0]->obj.object != LST && argv[0]->obj.object != FLD &&
		argv[0]->obj.object != TYPE_ARRAY) {
		rterrorbox(swerrormsg(LS_FAIL_ARGTYPE));
		return(RT_ERROR);
		}

	/* if parameter is a field make sure it is an indirect field
	   that points to a list */

	if (argv[0]->obj.object == FLD) {
		fp = argv[0]->ptr;
		if (fp->flags.type == FT_IND) {
			if (*(fp->storage) == LST) {
				itemno = (fp->storage[2] << 8) + fp->storage[3];
				if ((listptr = (complist *)ddget(RTddh, *(fp->storage),
					fp->storage[1], itemno)) == 0) {
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
	else {
				/* get a pointer to the list	*/
		if ((listptr = (complist *)ddget(RTddh, argv[0]->obj.object,
			argv[0]->obj.applno, argv[0]->obj.itemno)) == 0) {
			rterrorbox(swerrormsg(LS_NOLIST));
			return(RT_ERROR);
			}
		}

	memberptr = (byte *)(listptr + 1);
	membercount = listptr->numitems;
	while (membercount--) {
		/* skip over MEMBER token, element count and member size	*/
		elptr = memberptr + 4;
		elcount = (int)(*(memberptr + 1));
		while (elcount--) {
			if (*elptr == FLD || *elptr == TYPE_ARRAY) { 
				fp = rt_get_fp(elptr,&ms,NULL);
				if (fp) {
					if (type == 0)
						add_field_rec(fp);
					else
						dup_fld_explicit(fp);
					}
				}
			elptr += tsize(elptr);
			}
		/* increment memberptr to the next member	*/
		membersize = (memberptr[2] << 8) + memberptr[3];
		memberptr += (membersize + 4);
		}
	return(RT_SUCCESS);
}



// =========================================================
//  This function moved into MENUS.C in the ..\RT directory
// =========================================================

XDLL int 
listexec(int argc, rtobj *argv[])
{
	return(rt_listexec(argc,argv));
}



XDLL int 
listcsr(int argc,rtobj *argv[])
{
	static complist	*listptr = 0;
	static byte		*memberptr,
					*elptr;
	static int		memberno,
					elcount;
	int				opcode,
					objtype,
					itemno;
	field			*objname,
					*applname,
					*itemname,
					*string,
					*fp;
	int				length;

	/* 2 & 6 are the only valid arg counts	*/
	if (argc != 2 && argc != 6)
		return(CSR_FAIL_ARGCOUNT);

	/* get the opcode	*/
	rtobj2int(argv[0], &opcode);

	switch(opcode) {
		case GETLIST:
			listptr = 0;
			memberptr = 0;
			/* check argument count */
			if (argc != 2)
				return(CSR_FAIL_ARGCOUNT);

			/* insure that it's really a list or a field	*/
			if (argv[1]->obj.object != LST && argv[1]->obj.object != FLD)
				return(CSR_FAIL_ARGTYPE);

			if (argv[1]->obj.object == FLD) {
				fp = argv[1]->ptr;
				if (fp->flags.type == FT_IND) {
					if (*(fp->storage) == LST) {
						itemno = (fp->storage[2] << 8) + fp->storage[3];
						if ((listptr = (complist *)ddget(RTddh, *(fp->storage),
							fp->storage[1], (index)itemno)) == 0) {
							return(LISTERR);
							}
						}
					}
				}
			else {
					/* get the pointer to the new list	*/
			if ((listptr = (complist *)ddget(RTddh, argv[1]->obj.object,
				argv[1]->obj.applno, argv[1]->obj.itemno)) == 0) {
				return(LISTERR);
				}
			}


			memberptr = (byte *)(listptr + 1);
			return(listptr->numitems);
			break;
		case GETMEMBER:
			memberptr = 0;
			/* check argument count	*/
			if (argc != 2)
				return(CSR_FAIL_ARGCOUNT);

			/* insure that a list is selected	*/
			if (listptr == 0)
				return(LISTERR);

			/* get the member number	*/
			rtobj2int(argv[1], &memberno);
 			if (memberno < 0 || memberno >= listptr->numitems)
				return(LISTERR);

			/* offset to the nth member	*/
			memberptr = (byte *)(listptr + 1);
			while (memberno > 0) {
				memberptr += 2; /* skip over MEMBER tok,element count */
				length = ((int)(*(memberptr + 1))); /* get member length */
				memberptr += 2; /* over member length */
				memberptr += length; /* bump to next member */
				memberno--;
				}
			return((int)(*(memberptr + 1)));
			break;
		case GETELEMENT:
			/* check argument count	*/
			if (argc != 6)
				return(CSR_FAIL_ARGCOUNT);

			/* insure that a list and member are selected	*/
			if (listptr == 0 || memberptr == 0)
				return(LISTERR);

			/* get the element number	*/
			rtobj2int(argv[1], &elcount);
 			if (elcount < 0 || elcount >= (int)(*(memberptr + 1)))
				return(LISTERR);

			/* check the target fields	*/
			if (argv[2]->obj.object != FLD || argv[3]->obj.object != FLD ||
				argv[4]->obj.object != FLD || argv[5]->obj.object != FLD) {
				return(LISTERR);
				}

			/* set up pointers to fields & clear the fields	*/
			objname = (field *)argv[2]->ptr;
			clr_fld_storage(objname);
			applname = (field *)argv[3]->ptr;
			clr_fld_storage(applname);
			itemname = (field *)argv[4]->ptr;
			clr_fld_storage(itemname);
			string = (field *)argv[5]->ptr;
			clr_fld_storage(string);

			
			/* insure that fields will hold the right stuff	*/
			if (objname->flags.type != FT_ASCII ||
				objname->length < 3 ||
				applname->flags.type != FT_ASCII ||
				applname->length < NAME_KEYSIZE ||
				itemname->flags.type != FT_ASCII ||
				itemname->length < NAME_KEYSIZE ||
				string->flags.type != FT_ASCII ||
				string->length < MAX_LIST_STRING) {
				return(LISTERR);
				}

			/* skip over MEMBER token, element count and member size	*/
			elptr = memberptr + 4;

			/* advance pointer to the correct element	*/
			while (elcount) {
				elptr += tsize(elptr);
				elcount--;
				}

			/* turn it all into the appropriate results	*/
			objtype = *elptr;
			if (objtype <= DICT_COUNT){
				/* dictionary object, save applname, itemname, and	*/
				/* dictionary name in the fields provided			*/
				str2field(dddictname((byte)objtype), objname);
				itemno = (int)(*(elptr + 2) << 8) + *(elptr + 3);
				str2field(ddname(RTddh, (byte)objtype, *(elptr + 1),
					(index)itemno), itemname);
				if (objtype == CSR) {
					/* no real application for CSRs	*/
					str2field("CSR", applname);
					}
				else
					str2field(ddapplname(RTddh, *(elptr + 1)), applname);
				}
			else {
				/* it's a constant, make it into a string	*/
				pcl2field(elptr, string,NULL);
				}

			return(objtype);
			break;
		default:
			return(CSR_FAIL_ARGTYPE);
			break;
		}
}


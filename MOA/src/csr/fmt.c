/*$
** MODULENAME - fmt.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.10 $
** $Date: 1997/01/09 22:01:50 $
**
** Synopsis:
**   
**
** External Function List:
**   fmt
**   fmt_fill
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/

#include "pch.h"

#include <typedefs.h>
#include <ofbdefs.h>
#include <pclwords.h>
#include <convers.h>
#include <defines.h>
#include <dsplist.h>
#include <tdspl.h>
#include <scrlist.h>
#include <rt.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <errdefs.h>
#include <decmath.h>
#include <rtprotos.h>
#include <picklist.h>
#include <memalloc.h>
#include <msgrtn.h>

#define WKBUFLEN	256			/* length of various working buffers */

	int		c_saaddpl(char *name, char *val);
	byte	*_get_mem(register short i, register byte *bp);
	extern int	_file_putline(int file,char *line);
	//extern int	file_putline(int argc, rtobj *argv[]);

/*%
**
**  FMT       - format a line outputing to a picklist or a file
**
**	Usage:
**		CALLC fmt list,type,name or file handle
**
**	Parameters:
**		parm1 - 	List
**		parm2 - 	type 1 = picklist 2 = file
**		parm3 - 	name of picklist or handle of file or field
**
**	Returns:
**		none
**
*/

XDLL int 
fmt(int argc, rtobj *argv[])
{
	complist	*cl;
	byte		*mem_ptr,
				mem_cnt;
	char		*ptr;
	index		itemno;
	field		*fp;
	int			i,
	   			line,
			    oldline,
	    		col,
			    type,
			    fhdl,
	    		rc;
	short 		objlen;
	char 		name[WKBUFLEN],
				val[WKBUFLEN],
				text[WKBUFLEN];
	
	if (argc < 3) {
		mre(-1,"fmt requires at least 3 arguments");
		return(CSR_FAIL_ARGCOUNT);
		}
	if (argv[0]->obj.object == CSRARG_LST) {
		cl = (complist *)ddget(RTddh, argv[0]->obj.object,
					argv[0]->obj.applno, argv[0]->obj.itemno);
		}
	else {
		if (argv[0]->obj.object == CSRARG_FLD) {
			fp = argv[0]->ptr;
			itemno = (fp->storage[2] << 8) + fp->storage[3];
			cl = (complist *)ddget(RTddh, fp->storage[0],fp->storage[1], itemno);
			}
		else {
			mre(-1,"fmt requires first argument to be a list");
			return(-2);
			}
		}
	if (cl == NULL) {
		mre(-1,"fmt Invalid list");
		return(-2);
		}

	rtobj2int(argv[1], &type);
	switch (type) {
		case 1:
				rtobj2str(argv[2],name,sizeof(name));
				if ('\0' == name[2]) {
					mre(-1,"fmt type 1 requires symbolic name as last arg");
					return(CSR_FAIL_ARGTYPE);
					}
				break;
		case 2:
				rtobj2int(argv[2], &fhdl);
				break;
		}

	line = oldline = 1;
	memset(text,0,WKBUFLEN-1);
	memset(text,' ',WKBUFLEN-2);
	for (i = 1; i < cl->numitems; i++) { 
		mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;

		if (*mem_ptr == FLD) {
			fp = rt_get_fp(mem_ptr,&objlen,NULL);
			if (fp == NULL) {
				mre(-1,"fmt Invalid line");
				return(CSR_FAIL_ARGTYPE);
				}
			field2int(fp, &line);
			}
		else {
			pcl2int(mem_ptr,&line,NULL);
			}
		mem_ptr += tsize(mem_ptr);
		if (*mem_ptr == FLD) {
			fp = rt_get_fp(mem_ptr,&objlen,NULL);
			if (fp == NULL) {
				mre(-1,"fmt Invalid column");
				return(CSR_FAIL_ARGTYPE);
				}
			field2int(fp, &col);
			}
		else {
			pcl2int(mem_ptr,&col,NULL);
			}
		mem_ptr += tsize(mem_ptr);

		fp = rt_get_fp(mem_ptr,&objlen,NULL);
		if (fp != NULL) {
			if ((fp->displaylen+1) > (WKBUFLEN-2)) {
				mre(-1,"fmt field to big");
				return(CSR_FAIL_ARGTYPE);
				}
			rc = editfill(&val[0],fp->storage,fp,' ');
			}
		else {
			pcl2str(mem_ptr,&val[0],WKBUFLEN-1,NULL);
			}
		while (oldline < line) {
			ptr = text;
			ptr = ptr + StringLen(text) - 1; /* remove trailing spaces */
			while (*ptr == ' ') {
				ptr--;
				}
			ptr++;
			*ptr = '\0';
			switch (type) {
				case 1:
						if (text[0] == '\0') {
							text[0] = ' ';
							text[1] = '\0';
							}
						c_saaddpl(name,text);
						break;
				case 2:
						_file_putline(fhdl,text);
						break;
				case 3:
						fp = (field *) argv[2]->ptr;
						str2field(text,fp);
						return(RT_SUCCESS);
						break;
				}
			memset(text,0,WKBUFLEN-1);
			memset(text,' ',WKBUFLEN-2);
			oldline++;
			}
		if (line) memcpy(&text[col],&val[0],StringLen(&val[0]));
		}
	ptr = text;
	ptr = ptr + StringLen(text) - 1; /* remove trailing spaces */
	while (*ptr == ' ') {
		ptr--;
		}
	ptr++;
	*ptr = '\0';
	if (text[0] == '\0') {
		return(RT_SUCCESS);
		}
	switch (type) {
		case 1:
			c_saaddpl(name,text);
			return(RT_SUCCESS);
		case 2:
			if (_file_putline(fhdl,text) != 0) {
				mre(-1,"fmt Putline failed");
				return(CSR_FAIL_ARGTYPE);
				}
			return(RT_SUCCESS);
		case 3:
			fp = (field *) argv[2]->ptr;
		    str2field(text,fp);
			return(RT_SUCCESS);
		default:
			return(RT_ERROR);
		}
}
/*%
**
**  FMT_fill     - develepment routine to fill a field
**
**	Usage:
**		CALLC fmt_fill, IND
**
**	Parameters:
**		parm1 - 	inderect field
**		parm2 - 	result         
**
**	Returns:
**		none
**
*/

XDLL int 
fmt_fill(int argc, rtobj *argv[])
{
	field		    *fp,*fp1;
	byte appno;
	index itemno;
	int 	len;

	fp = argv[0]->ptr;
	fp1 = argv[1]->ptr;
	if (fp->flags.type == FT_IND) {
		appno = fp->storage[1];
		itemno = (fp->storage[2] << 8) | fp->storage[3];
		fp = (field *)ddget(RTddh, fp->storage[0],fp->storage[1], itemno);
		len = editlength(fp);
		switch (fp->flags.type) {
			case FT_ASCII:
						memset(fp1->storage,'X',len);
						break;
			case FT_NUMERIC:
			case FT_DATE:
			case FT_TIME:
			case FT_INT:
			case FT_FIXED:
						memset(fp1->storage,'9',len);
						break;
			}
		fp1->numchars = len;
		return(RT_SUCCESS);
		}
	return(RT_ERROR);
}

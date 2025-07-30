/*$
** MODULENAME - totalist.c
** $Name: R1V4C13 $
** $Revision: 1.11 $
** $Date: 1999/03/11 08:28:51 $
**
** Synopsis:
**   perform totalling operations based on compiled
**   directives and then append the bucket field(s)
**   identified in the list members into the cdserver
**   record context buffer.
**
** External Function List:
**
** Notes:
**
**  (c) Copyright by ISC-Bunker Ramo Corp., 1991
**        All rights reserved.
*/
#include "pch.h"

#include <typedefs.h>
#include <defines.h>
#include <rtprotos.h>
#include <convers.h>
#include <rtdef.h>
#include <csrdef.h>
#include <errdefs.h>
#include <cdsfuncs.h>
#include <cmfield.h>
#include <rtable.h>
#include "cdscsr.h"

#define XPLUS	'+'
#define	XMINUS	'-'

typedef struct tlfieldx {
	field	*fp;
	byte	ano;
	index	ino;
	} tlfield;

int fldorpcl(register byte *bp);
tlfield *find_fp(register tlfield *flds, register field *fp);

/*$
**	Function:	total_list
**
**	Author:		Chuck Hartshorn				Fall 1991
**
**	Purpose:	Perform totalling operations based on compiled
**              list directives and then append the bucket field(s)
**              identified in the list members into the cdserver
**              record context buffer.
**
**	Usage:		CALL CSR:TOTAL_LIST compiled_list context_name \
**	      		    flag_field correction_bit
**
**	Returns 	RT_SUCCESS	totalling & appending completed without error
**	        	RT_ERROR   totalling or appending field failed
**				
**	NOTES:		The totals list structure expected is:
** 
** member 0  Total List ID, List Modifier
** member 1  Source Field, +/- Flag, Bucket Field, 
**   :       Member Modifier, Correction Status
**   :
** member n  Source Field, +/- Flag, Bucket Field, 
**   :       Member Modifier, Correction Status
**
** Using the contents of this list, the following will take place:
**
** Load/Validate totals list from indirect object - argv[0]
** Execute global list routine object 
** Loop while member is available
**		Examine source field 
**		Examine Add/Subtract Flag, ("+" || "-")
**		Bucket field
**		Execute modifier routine || test field for data || nothing
**		Examine correction flag switch
** End total loop 
** Append bucket fields to CDS context in argv[1]
**
**/
XDLL int
total_list(int argc,rtobj *argv[])
{
	complist	*lp;
	field		*fp, *fp_flag;
	tlfield		*flds, *flds_base;
	decwork		*source, *bucket, *result;
	short		memctr, ms;
	byte		*bp;
	char		buf[256];
	char		addsub;
	int			i, correction_bit;
	index		ino;
	cdsselect	*tptr;
	char		fn_total_list[]="total_list";

#if 0
	if (argc != 4) {
		swerrorvmsg(buf,CSR_ARG_COUNT,fn_total_list,4,argc);
		rterrorbox(buf);
		return(RT_ERROR);
		}
#endif
	/* Load/Validate list */
	if (argv[0]->obj.object != CSRARG_FLD) {
		swerrorvmsg(buf,CSR_EXPECT_FLD,fn_total_list,0);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	fp = argv[0]->ptr;
	if (fp->flags.type != FT_IND) {			/* argv[0] is not an indirect */
		swerrorvmsg(buf,CSR_EXPECT_IND,fn_total_list,0);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	if (*(fp->storage) != LST) {			/* no list in indirect */
		swerrorvmsg(buf,CSR_EXPECT_LSTIND,fn_total_list,0);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	ino = (fp->storage[2] << 8) + fp->storage[3];
	if ((lp = 
		(complist *)ddget(RTddh,*(fp->storage),fp->storage[1],ino)) == 0) {
		swerrorvmsg(buf,CSR_NO_LST,fn_total_list,0);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	/* obtain select context */
	if (rtobj2str(argv[1],buf,sizeof(buf)) == RT_ERROR) {
		swerrorvmsg(buf,CSR_BAD_CONTEXT,fn_total_list,1);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	if ((tptr = find_clcontext(CdsEnv, buf)) == NULL) {
		swerrorvmsg(buf,CSR_BAD_CONTEXT,fn_total_list,1);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	if (argv[2]->obj.object != CSRARG_FLD) {
		swerrorvmsg(buf,CSR_EXPECT_FLD,fn_total_list,2);
		rterrorbox(buf);
		}
	/* obtain flag field */
	fp_flag = argv[2]->ptr;
	if (!fp_flag || fp_flag->flags.type != FT_FLAGS) {
		swerrorvmsg(buf,CSR_BAD_FLAGFLD,fn_total_list,2);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	/* obtain correction bit */
	if (rtobj2int(argv[3],&correction_bit) == RT_ERROR) {
		swerrorvmsg(buf,CSR_BAD_INT_OBJ,fn_total_list,3);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	bp = (byte *)lp;
	bp += sizeof(complist);
	/* 2 elements required in header member */
	if (bp[1] != 2) {
		swerrorvmsg(buf,CSR_BAD_TOTAL_LIST_HDR,fn_total_list);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	bp += 4;				/* Jump past opcode, itemcnt, and length */
	if (*bp != TYPE_SCON) {	/* 0,0 Must be constant == "totals" */
		swerrorvmsg(buf,CSR_BAD_TOTAL_LIST_HDR,fn_total_list);
		rterrorbox(buf);
		return(RT_ERROR);	
		}
	pcl2str(bp,buf,sizeof(buf),NULL);
	if (strcmp(buf,"totals") != 0) {
		swerrorvmsg(buf,CSR_BAD_TOTAL_LIST_HDR,fn_total_list);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	bp += tsize(bp);
	if ((i = fldorpcl(bp)) == RT_ERROR) {	/* List modifier */
		swerrorvmsg(buf,CSR_BAD_TOTAL_LIST_HDR,fn_total_list);
		rterrorbox(buf);
		return(RT_ERROR);
		}
	if (i != 0)						/* Check fld/pcl for return value */
		return(-1);
	
	source = (decwork *)alloca(MAXDECWORK);
	bucket = (decwork *)alloca(MAXDECWORK);
	result = (decwork *)alloca(MAXDECWORK);
	flds_base = (tlfield *)alloca(lp->numitems * sizeof(tlfield));
	flds = flds_base;
	for (i = 0; i < lp->numitems; i++) {
		flds->fp = (field *) 0;
		flds++;
		}
	
	flds = flds_base;
	bp += tsize(bp);
	for (memctr = 1; memctr < lp->numitems; memctr++) {	/* Main Totals Loop */
		/* Must have five elements */
		if (bp[1] != 5)	{
			rterrorbox(swerrormsg(CSR_BAD_TL_MEMBER));
			return(RT_ERROR);
			}
		bp += 4;			/* Jump past opcode, itemcnt, and length */
		switch (*bp) {	/* Examine Source Field */
			case TYPE_SCON:
			case FLD:
			case TYPE_ARRAY:
				pcl2decwork(bp,source,NULL);
				break;
			default:
				return(RT_ERROR);
		}
		
		bp += tsize(bp);
		/* Examine Add/Subtract Flag */
		if (*bp != TYPE_SCON) {
			rterrorbox(swerrormsg(CSR_BAD_TL_MEMBER));
			return(RT_ERROR);
			}
		pcl2str(bp,buf,sizeof(buf),NULL);
		addsub = buf[0];	/* get operator */
		if (addsub != XMINUS && addsub != XPLUS) {
			rterrorbox(swerrormsg(CSR_BAD_TL_MEMBER));
			return(RT_ERROR);
			}
		bp += tsize(bp);
		fp = rt_get_fp(bp,&ms,NULL);
		if (fp == NULL) {
			rterrorbox(swerrormsg(CSR_BAD_TL_MEMBER));
			return(RT_ERROR);
			}
		
		field2decwork(fp,bucket);
		bp += ms;
		if ((flds = find_fp(flds_base,fp)) != (tlfield *) 0) {
			flds->ano = *(bp - 3);
			flds->ino = *(bp - 2) << 8;
			flds->ino |= *(bp - 1);
			flds->fp = fp;
			}
		
		/* Evaluate member modifier */
		if ((i = fldorpcl(bp)) == RT_ERROR) {
			rterrorbox(swerrormsg(CSR_BAD_TL_MEMBER));
			return(RT_ERROR);
			}
		bp += tsize(bp);
		if (i != 0) {		/* Check fld/pcl for return value */
			bp += tsize(bp);
			continue;
			}
		if (*bp != TYPE_SCON) {
			rterrorbox(swerrormsg(CSR_BAD_TL_MEMBER));
			return(RT_ERROR);
			}
		pcl2str(bp,buf,sizeof(buf),NULL);		/* observe correction status */
		bp += tsize(bp);
		if (buf[0] == 'y' || buf[0] == 'Y') {
			/* perform addition or subtraction to bucket */
			if (*(int *)(fp_flag->storage) & (1 << correction_bit)) {
				if (addsub == XPLUS)
					addsub = XMINUS;
				else
					addsub = XPLUS;
				}
			}
		if (addsub == XPLUS)
			decadd(result,bucket,source);
		else
			decsub(result,bucket,source);
		decwork2field(result,fp);
		flds++;
	}	/* End of Main Totals Loop */
	
	/* add bucket fields to record */
	flds = flds_base;
	while (flds->fp) {
		if (rtaddfield(tptr,flds->fp,flds->ano,flds->ino,flds->fp) == RT_ERROR) {
			swerrorvmsg(buf,CSR_BAD_TL_APPEND,flds->ano,flds->ino);
			rterrorbox(buf);
			return(RT_ERROR);
			}
		flds++;
		}
	return (RT_SUCCESS);
}

/*
** fldorpcl
** 
** Check contents of object.  If a field, see if a value has been entered.
** If a pcl routine, execute and check RETVAL to return direction.
**
** Returns:  RT_SUCCESS  All is well
**           -2       OFB Routine advises a return
**           -2       FLD is empty - return
**           RT_ERROR    All is not well
*/
int
fldorpcl(byte *bp)
{
	field	*fp;
	char	buf[256];
	int		i;
	short	ms;

	if (*bp == TYPE_SCON) {
		pcl2str(bp,buf,sizeof(buf),NULL);
		if (buf[0]== '0') {
			return(RT_SUCCESS);
			}
		return(RT_ERROR);
		}
	if (*bp == FLD || *bp == TYPE_ARRAY) { /* Check for field */
		fp = rt_get_fp(bp,&ms,NULL);
		if (fp != NULL) {
			switch (fp->flags.type) {
				case FT_INT:
				case FT_FIXED:
					field2int(fp,&i);
					if (i == 0) {
						return(-2);
						}
					break;
				case FT_NUMERIC:
				case FT_DATE:
				case FT_TIME:
				case FT_ASCII:
					if (fp->numchars == 0) {
						return(-2);
						}
					break;
				default:
					return(RT_ERROR); /* can't convert other fld types */
				}
				return(RT_SUCCESS);
			}
		else
			return(RT_ERROR);
	}				
	if (*bp == OFB) { /* OFB Routine - execute! */
		pcl(0,OFB,*(bp + 1),(index)((*(bp + 2) << 8) + *(bp + 3)),NULL,NULL);
		if (Retval != 0) {
			return(-2); /* Exit per global list routine */
			}
		return(0);
	}
	return(RT_ERROR);  /* Couln't find a field or OFB routine */
}

/*
** find_fp
** 
** Search for match for (field *) in null terminated list of 
** tlfield structures.
**
** Returns:  null if field is in list
**           Next available tlfield ptr if field is not in list
*/
tlfield *
find_fp(tlfield *flds, field *fp)
{
	while (flds->fp) {
		if (flds->fp == fp)
			return((tlfield *)0);	/* field is in list */
		flds++;
		}
	return(flds);	/* field not in list, so return next available tlfield */
}

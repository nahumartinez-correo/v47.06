/*$
** MODULENAME - ind_asgn.c
** Author: Bob Spratt    Oct 1996
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.1 $
** $Date: 1996/10/10 00:08:36 $
**
**	Synopsis:
**		This function assign the value of parameter 2 to the
**		field pointed to by an indirect field
**	args
**		arg0 : source field (stored in an indirect)
**		arg1 : dest field or list
**
**	RETVALS:
**		0 		: success
*/

#include <pch.h>

#include <typedefs.h>		/* contains DD object structure defs */
#include <rtdef.h>		/* Contains the Retval global declaration */
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <convers.h>
#include <rt.h>
#include <defines.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <msgrtn.h>	  
#include <c.h>
#include <clconn.h>
#include <cdsfuncs.h>
#include <cmfield.h>
#include <clcdsfn.h>
#ifndef CLCDSENV_H
#include <clcdsenv.h>
#endif
#include <rtable.h>
#include <wtypes.h>
#include <winspool.h>

XDLL int ind_assign(int argc, rtobj *argv[]);

int ind_assign(int argc, rtobj *argv[])
{
	field *src;
	field *dest;
	index index_no;
	byte  index[4];
	byte  *work;
	complist *lp;
	short	ms;
	int 	member_cnt,element_cnt,member_size,first_member=0,member_nbr=0;
	byte *member_ptr,*element_ptr;
	int		obj_type;


	// argument must be of type fld
	if ( argv[0]->obj.object != FLD && argv[0]->obj.object != LST ) {
		mre(-1,"Argument 0 must be a field or compiled list");
		return(CSR_FAIL_ARGTYPE);
		}

	// argument must be of type fld
	if ( argv[1]->obj.object != FLD && argv[1]->obj.object != LST ) {
		mre(-1,"Argument 1 must be a field or compiled list");
		return(CSR_FAIL_ARGTYPE);
		}

	// if both args are fields, just do the conversion and end
	if ( argv[0]->obj.object == FLD && argv[1]->obj.object == FLD ) {
		dest = (field *)argv[0]->ptr;
		src = (field *)argv[1]->ptr;

//		if ( src->flags.type == FT_IND && dest->flags.type == FT_IND){
//			mre(-1,"Both arguments cannot be an indirect field");
//			return(CSR_FAIL_ARGTYPE);
//		}
		if ( src->flags.type == FT_IND ){	// get the pointer to the real field
			index_no = (src->storage[2] << 8) + src->storage[3];
			if ((src = (field *)ddget(RTddh,*(src->storage),src->storage[1],index_no)) == 0){
				mre(-1,"ind_assign: INDIRECT FIELD SHOULD POINT TO A FIELD");
				return(CSR_FAIL_ARGTYPE);
			}
		}

		if ( dest->flags.type == FT_IND ){	// get the pointer to the real field
			index_no = (dest->storage[2] << 8) + dest->storage[3];
			if ((dest = (field *)ddget(RTddh,*(dest->storage),dest->storage[1],index_no)) == 0){
				mre(-1,"ind_assign: INDIRECT FIELD SHOULD POINT TO A FIELD");
				return(CSR_FAIL_ARGTYPE);
			}
		}
		// do the conversion (assignment)
		work=(byte *)&src->ddref;
		index[0] = work[0];
		index[1] = work[1];
		index[2] = work[3];
		index[3] = work[2];
		pcl2field((byte *)index,dest,NULL);
		add_field_rec(dest);
	
		return(RT_SUCCESS);
	}
	if (argv[1]->obj.object == LST) {
		mre(-1,"ind_assign: PARAMETER 1 CANNOT BE A LIST");
		return(CSR_FAIL_ARGTYPE);
	}
	src = (field *)argv[1]->ptr;
	if (src->flags.type != FT_IND){
		mre(-1,"ind_assign: PARAMETER 1 MUST BE AN INDIRECT FIELD");
		return(CSR_FAIL_ARGTYPE);
	}
	index_no = (src->storage[2] << 8) + src->storage[3];
	if ((src = (field *)ddget(RTddh,*(src->storage),src->storage[1],index_no)) == 0){
		mre(-1,"ind_assign: INDIRECT FIELD SHOULD POINT TO A FIELD");
			return(CSR_FAIL_ARGTYPE);
	}
	if ((lp = (complist *)ddget(RTddh,argv[0]->obj.object,
						argv[0]->obj.applno,argv[0]->obj.itemno)) == 0)	{
		mre(-1,"ind_assign: CANNOT OPEN LIST");
		return(CSR_FAIL_ARGTYPE);
	}

	member_ptr = (byte *) (lp + 1);
	member_cnt = lp->numitems;
	first_member = 1;
	while (member_cnt--)	{
		// Skip over the MEMBER token, element Count and Member size
		element_ptr = member_ptr + 4;
		element_cnt = (int)(*(member_ptr+1));
		obj_type = *element_ptr;
		switch (obj_type)	{
			case FLD:
				dest = rt_get_fp(element_ptr,&ms,NULL);
				if (dest->flags.type == src->flags.type){
					work=(byte *)&src->ddref;
					index[0] = work[0];
					index[1] = work[1];
					index[2] = work[3];
					index[3] = work[2];
					pcl2field((byte *)index,dest,NULL);
					add_field_rec(dest);
					return(member_nbr);
				}
				break;
		}				// End if switch
		member_nbr++;
		member_size = (member_ptr[2] << 8) + member_ptr[3];
		member_ptr += member_size + 4;
		first_member = 0;
	}
	return -1;
}

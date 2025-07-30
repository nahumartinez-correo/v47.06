/*$
** MODULENAME - getname.c
**
** $Name: R1V4C13 $
** $Revision: 1.4 $
** $Date: 1999/03/11 08:28:48 $
**
** Synopsis:
**   Get field object application name and item name.
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/

/*
	RT custom CSR to get object name
	CALL CSR:getname INDfield, appl_destination, item_destination

	where
		indfld  =       IND object containing pointer to field.
		appl_destination =   ascii type field to recieve application name
		item_destination =   ascii type field to recieve item name
*/


#include <pch.h>

#include <rtdef.h>		/* Contains the Retval global declaration */
#include <csrdef.h>
#include <convers.h>
#include <defines.h>
#include <msgrtn.h>	  

/* error codes */
XDLL
int getname(argc,argv)
int		argc;
rtobj	*argv[];
	{
	field	*ind_fld,*appl_dest, *item_dest;                                      
	index	item;
	char	*name;

	/* check for correct argument count     */
	if (argc != 3)	{
		mre(-1,"Invalid Number of arguments passed");
		return(CSR_FAIL_ARGCOUNT);
		}

	/* check for correct argument type      */
	if(argv[0]->obj.object != FLD){
		mre(-1,"Invalid argument type (0)");
		return(CSR_FAIL_ARGTYPE);
		}
	if(argv[1]->obj.object != FLD){
		mre(-1,"Invalid argument type (1)");
		return(CSR_FAIL_ARGTYPE);
		}
	if(argv[2]->obj.object != FLD){
		mre(-1,"Invalid argument type (2)");
		return(CSR_FAIL_ARGTYPE);
		}
	ind_fld = (field *)(argv[0]->ptr);
	if(ind_fld->flags.type != FT_IND){
		mre(-1,"First argument must be type indirect");
		return(CSR_FAIL_ARGTYPE);
		}
	appl_dest = (field *)(argv[1]->ptr);
	if(appl_dest->flags.type != FT_ASCII){
		mre(-1,"Second argument must be type ascii");
		return(CSR_FAIL_ARGTYPE);
		}
	item_dest = (field *)(argv[2]->ptr);
	if(item_dest->flags.type != FT_ASCII){
		mre(-1,"Third argument must be type ascii");
		return(CSR_FAIL_ARGTYPE);
		}

	name = ddapplname(RTddh,ind_fld->storage[1]);
	if (!name){
		mre(-1,"Invalid application number in call to ddapplname");
		return(-1);
		}

	if ( strlen(name) <= (uint)appl_dest->length){
		str2field(name,appl_dest);
	}
	item = (ind_fld->storage[2] << 8) + ind_fld->storage[3];
	name = ddname(RTddh,ind_fld->storage[0],ind_fld->storage[1],item);
	if (!name){
		mre(-1,"Invalid item number name in call to ddname");
		return(-1);
		}
	if ( strlen(name) <= (uint)item_dest->length){
		str2field(name,item_dest);
		}
	return(RT_SUCCESS);
	}

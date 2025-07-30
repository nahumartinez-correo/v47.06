/*$
** MODULENAME - runofb.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.6 $
** $Date: 1996/03/08 19:38:40 $
**
** Synopsis:
**   This function will execute the PCL routine (applno:indexno) that
**   was passed as a parameter.
**
** External Function List:
**   pcl
**
** Notes:
**		
** (c) Copyright by ISC-Bunker Ramo Corp., 1989
**          All rights reserved.
**
*/

#include "pch.h"

#include <typedefs.h>		/* contains DD object structure defs */
#include <rtdef.h>		/* Contains the Retval global declaration */
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <convers.h>
#include <rt.h>
#include <rtprotos.h>

XDLL int 
runofb(int argc, rtobj *argv[])
{
	field	*fp;
	index	itemno;
	int 	applno,i;
	int		ap_it_val;  /* The application and item index */

	/* don't have to check for argument count, the compiler will	*/
	/* have done that already										*/

	/* check for correct argument types	*/
	if (argv[0]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);

	/* get pointer to field structure */
	fp = (field *)argv[0]->ptr;
	
	/* Convert field to int for processing */
	field2int(fp, &ap_it_val);
	
	/* Get the index number */
	itemno = ap_it_val % 100000; 
	
	/* Get the Application number */
	applno = ap_it_val / 100000; 
	
	if (applno && itemno) {
		/* CALL IT */
		/* Params are:
		   1: Flags for this transaction
		   2: applno
		   3: itemno
		   4: Pointer if applno and itemno are zero
		*/
		i = pcl(0,PCL,(byte)applno,itemno,(pcldef *)0,NULL);
		if (!i)
			return(-1);
		}
	else
		return(-1);
	return(0);
}

/*$
** MODULENAME - getofbin.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.8 $
** $Date: 1996/03/08 19:37:44 $
**
**	Synopsis:
**      This function will stuff an indirect-type field with a pointer to
**      a dictionary object specified by name and type.  The difference
**      between this CSR and the LET verb is the ability to assign a
**      pointer to an object named in the CONTENTS of an ASCII field.
**      Given a field as the object to assign, the LET verb will assign
**      a pointer to the field itself.  This CSR will assign a pointer
**      to the object name in the field's storage.  A literal string or 
**      descriptor is acceptable for specifying the object name to assign, 
**      though the LET verb is the preferred method in that case.  The 
**      application name, object name, and type are expected as separate 
**      parameters.  If the object type is not specified, the default type 
**      is "PCL."
**
**	External Function List:
**		none.
**
**	Notes:
**		
**  (c) Copyright by ISC-Bunker Ramo Corp., 1989
**        All rights reserved.
**
** Roger Bailey, Winter 1989-90
** Modified to add optional object type argument--Jeanette Sazama, April 1991
** Modified to require an IND-type target field and to add the typeno to
** the result for valid IND field contents--Jeanette Sazama, May 1991
*/

#include "pch.h"

#include <typedefs.h>
#include <ofbdefs.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <convers.h>
#include <defines.h>
#include <rt.h>
#include <rtprotos.h>


XDLL int
getofbindx(int argc,rtobj *argv[])
{
	index		itemno;
	byte 		applno;
	char        typeno;
	int			appl,item;
	byte        *bp;
	byte		rc;
	char		itemname[NAME_KEYSIZE + 1],
				applname[NAME_KEYSIZE + 1];
	char        objtype[4];
	field		*result;

	/* ---------------------------------------- */
	/*	Arguments are:                          */
	/*		1: Applicaton name or dd obj        */
	/*		2: Object name or dd obj            */
	/*		3: Result field	 (must be IND type)	*/
	/*      4: Object type (optional)           */
	/*            default is "PCL"              */
	/*         "DD" = dd obj                    */
	/* ---------------------------------------- */

	typeno = PCL;
	objtype[0] = '\0';
	if (argc == 4)
	{
		if (rtobj2str(argv[3], objtype, sizeof(objtype)) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
		if (strcmp(objtype,"DD") != 0)
		{
			if ((typeno = dddictindex(objtype)) < 0)
				return(CSR_FAIL_ARGTYPE);
		}
	}

	if (strcmp(objtype,"DD") != 0)
	{
		/* get the application name and item name	*/
		if (rtobj2str(argv[0], applname, sizeof(applname)) != RT_SUCCESS ||
			rtobj2str(argv[1], itemname, sizeof(itemname)) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
		/* Get the application number	*/
		applno = ddapplindex(RTddh,applname);
		/* Get the index number */
		itemno = ddindex(RTddh,typeno,applno,itemname);
	}	
	else
	{
		if (rtobj2int(argv[0], &appl) != RT_SUCCESS ||
			rtobj2int(argv[1], &item) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
		itemno = (index) item;
		applno = (byte) appl;
		typeno = LST;
		rc = (byte) ddgetstat(RTddh,typeno,applno,itemno);
		if (rc == DDSTATFAIL)
		{
			typeno = PCL;
		}
	}

	/* make sure the target field is usable	*/
	if (argv[2]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);

	/* make sure it's a valid target field */
	result = argv[2]->ptr;
	if (result->flags.type != FT_IND)
		return(CSR_FAIL_ARGTYPE);

	if (applno == 0 || itemno == 0)
		clr_fld_explicit(result);
	else {
		bp = result->storage;
		*bp++ = typeno;
		*bp++ = applno;
		*bp++ = itemno >> 8;
		*bp = itemno & 0xff;
		result->numchars = 4;
	}
	return(0);
}


/*$
** MODULENAME - apl_index.c
**
**	$Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
**	$Revision: 1.6 $
**	$Date: 1996/03/08 19:28:26 $
**
**	Synopsis:
**		This function will return the application number
**
**	External Function List:
**		none.
**
**	Notes:
**		
**  (c) Copyright by ISC-Bunker Ramo Corp., 1989
**        All rights reserved.
**
*/

#include "pch.h"

#include <typedefs.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <rtprotos.h>
#include <convers.h>
#include <defines.h>


XDLL int
apl_index(argc,argv)
int argc;
rtobj *argv[];
{
	byte 		applno;
	char		applname[NAME_KEYSIZE + 1];
	field		*result;

/*Arguements are:
  1: Applicaton name
*/

	if (argc != 2)
		return (CSR_FAIL_ARGCOUNT);

	if (rtobj2str(argv[0], applname, sizeof(applname)) != RT_SUCCESS)
	{
		return (CSR_FAIL_ARGTYPE);
	}

	/* make sure the target field is usable	*/
	if (argv[1]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);

	/* make sure it's a valid target field */
	result = argv[1]->ptr;
	if (!isASCIIfield(result) && !isNUMERICfield(result))
		return(CSR_FAIL_ARGTYPE);

	applno = ddapplindex(RTddh,applname);

	int2field(applno,result);

	return(0);
}

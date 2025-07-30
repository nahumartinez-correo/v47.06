/*$
** SCRAP  --
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:38:52 $
**
**	Synopsis:
**		CSR's that support moving data into and outof scrap.
**		(a.k.a. Clipboard) via the Scrap Manager utilities
**		provided by Windowman.  The following functions are
**		supported:
**			copyfromscrap(field)
**			copytoscrap(field)
**
**	EXAMPLES:
**	call csr:copyfromscrap FLD:scrapfield
**	call csr:copytoscrap CURRENTFIELD
**
**  (C) Copyright 1991 by ISC-Bunker Ramo.  All rights reserved.
**
**  Cathy Higel, Spring-1991
*/

#include "pch.h"

#include <defines.h>
#include <typedefs.h>
#include <rtprotos.h>
#include <dataman.h>
#include <ofbdefs.h>

XDLL int
copyfromscrap(int argc, rtobj * argv[])
{
	field	*fp;

	if (argv[0]->obj.object != FLD)
		return CSR_FAIL_ARGTYPE;

	fp = argv[0]->ptr;
	return(fieldfromscrap(fp));
}



XDLL int
copytoscrap(int argc, rtobj * argv[])
{
	field	*fp;

	if (argv[0]->obj.object != FLD)
		return CSR_FAIL_ARGTYPE;

	fp = argv[0]->ptr;
	return(fieldtoscrap(fp));
}


/*$
** STDSUBS  --  RunTime collection of C-Language subroutines
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:39:16 $
**
** Synopsis:
**   This module contains all the "Standard C Subroutines" used
**   by the Pinnacle Plus runtime system.
**
** External Function List:
**   varies.
**
** Notes:
**   None
**
**  (C) Copyright 1989 by ISC Systems Corp.  All rights reserved.
**
*/
#include "pch.h"

#include <keycodes.h>
#include <typedefs.h>
#include <dsplist.h>
#include <csrdef.h>
#include <dataman.h>
#include <ppdefs.h>

XDLL int 
yesorno(int argc, rtobj *argv[])
{
	field	*fp;

	/* insure that it's a field	*/
	if (argv[0]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);

	/* arguments ok, set pointer to the field	*/
	fp = argv[0]->ptr;

	/* empty field passes validation	*/
	if (!fp->numchars)
		return(VAL_PASS);

	if (fp->storage[0] == 'Y' || fp->storage[0] == 'y' ||
		fp->storage[0] == 'N' || fp->storage[0] == 'n' )
			return(VAL_PASS);

	return(VAL_FAIL_W_MSG);
}


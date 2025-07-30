/*$
** TRIMFIELD  --  Remove trailing (or leading) spaces (or zeroes) from
**                a field and optionally move the field to another field
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:39:02 $
**
**	External Function List:
**		none
**
**	Notes:
**		This function may become a PCL verb someday...
**
**
**  (c) Copyright by ISC-Bunker Ramo Corp., 1990
**        All rights reserved.
**
**  H. Marc Lewis, Spring 1990
*/

#include "pch.h"

#include <typedefs.h>		/* contains DD object structure defs 	*/
#include <csrdef.h>			/* contains C subroutine structure defs */
#include <ofbdefs.h>		/* PCL/C defines						*/
#include <convers.h>
#include <rt.h>
#include <rtprotos.h>

XDLL int 
trimfield(int argc,rtobj *argv[])
{
	field	*fp;
	field	*fp2;
	short	i;
	byte	*p;

	/* check for correct argument count	*/
	if (argc < 1 || argc > 2)
		return(CSR_FAIL_ARGCOUNT);

	/* check for correct argument types	*/
	if (argv[0]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);
	if (argc == 2 && argv[1]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);

	fp = (field *) argv[0]->ptr;
	if (fp->flags.type != FT_ASCII && fp->flags.type != FT_NUMERIC)
		return(CSR_FAIL_FIELDTYPE);

	if (argc == 2) {
		fp2 = (field *) argv[1]->ptr;
		if (fp2->flags.type != FT_ASCII && fp2->flags.type != FT_NUMERIC)
			return(CSR_FAIL_FIELDTYPE);
		}

	/* trim from the right */
	i = (short) StringLen((char *)fp->storage) - 1;
	p = fp->storage + i;
	while (i >= 0) {
		if (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n')
			break;
		*p-- = '\0';
		--i;
		}
	fp->numchars = i + 1;

	if (argc == 2) {
		/* then copy field to target field */
		str2field((char *)fp->storage, fp2);
		}

	/* return all OK code */
	return(VAL_PASS);
}

/*$
** MODULENAME - what it is
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.6 $
** $Date: 1996/03/08 19:38:16 $
**
**	Synopsis:
**		Description of the functions of this module
**
**	External Function List:
**		List of external functions contained in this module
**
**	Notes:
**		Any special linkage requirements, other notes about this
**		module.
**
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

#define EOS '\0'

XDLL int 
pat_it(int argc, rtobj *argv[])
{
	byte	*in;
	byte	*out;
	field	*fpin,*fpout;
	char	*first_nonblk;

	/* check for correct argument count	*/
	if (argc != 2)
		return(CSR_FAIL_ARGCOUNT);

	fpin = (field *)argv[0]->ptr;
	in = fpin->storage;
	fpout = (field *)argv[1]->ptr;
	out = fpout->storage;
	if (fpin->displaylen > fpout->length)
	{
		return (-1);
	}
	if (editfill ((char *)out,(void *)in,fpin,' ') == RT_SUCCESS)
	{
		if (*out != ' ')
		{
			return (0);
		}
		first_nonblk = (char *) out;
		while (*first_nonblk == ' ')
		{
			first_nonblk++;
		}
		strcpy (out,first_nonblk);
		fpout->numchars = strlen(out);
		return (0);
	}
	return (-1);
}

/*$
** MODULENAME - fld_to.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:37:40 $
**
** Synopsis:
**   
**
** External Function List:
**   fld_to_upper
**   fld_to_lower
**   
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/

#include "pch.h"

#include <typedefs.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <rtprotos.h>
#include <convers.h>
#include <rt.h>
#include <rtprotos.h>

#define EOS '\0'

/***********************************************************************/
/*  fld_to_upper                                                       */
/*        Field to UPPER CASE routine                                  */
/*        Parms. int argc - number of entries in argv                  */
/*               char *argv[] - pointer to parameters                  */
/*                    *argv[0] - field                                 */
/*        returns - return UPPER CASE string in *argv[0]               */
/*                - return code (0 = RT_SUCCESS)                       */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
XDLL int
fld_to_upper(int argc, rtobj *argv[])
{
	byte	*ptr;
	field	*fp;

	/* argument has to be a field	*/
	if (argv[0]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);

	/* make sure it's ASCII	*/
	fp = (field *)argv[0]->ptr;
	if (!isASCIIfield(fp))
		return(CSR_FAIL_ARGTYPE);

	/* switch bytes to upper case	*/
	ptr = fp->storage;
	while (*ptr != EOS)
	{
		*ptr = toupper (*ptr);
		ptr++;
	}
	return (VAL_PASS);
}
/***********************************************************************/
/*  fld_to_lower                                                       */
/*        Field to lower CASE routine                                  */
/*        Parms. int argc - number of entries in argv                  */
/*               char *argv[] - pointer to parameters                  */
/*                    *argv[0] - field                                 */
/*        returns - return lower CASE string in *argv[0]               */
/*                - return code (0 = RT_SUCCESS)                       */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
XDLL int
fld_to_lower(int argc, rtobj *argv[])
{
	byte	*ptr;
	field	*fp;

	/* argument has to be a field	*/
	if (argv[0]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);

	/* make sure it's ASCII	*/
	fp = (field *) argv[0]->ptr;
	if (!isASCIIfield(fp))
		return(CSR_FAIL_ARGTYPE);

	/* switch bytes to lower case	*/
	ptr = fp->storage;
	while (*ptr != EOS)
	{
		*ptr = tolower (*ptr);
		ptr++;
	}
	return (VAL_PASS);
}

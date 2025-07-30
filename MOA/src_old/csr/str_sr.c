/*$
** MODULENAME - str_search
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.7 $
** $Date: 1996/03/08 19:38:54 $
**
**	Synopsis:
**		This function will find a string within another string
**		
**	args
**		arg0 : field to be searched (must be a field) (INPUT)
**		arg1 : string to search for (FLD,DSC,"Text"). (INPUT)
**		arg2 : field to get the result	(OUTPUT)
**		arg3 : (optional) start searching offset 
**
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
#include <memalloc.h>
#include <msgrtn.h>	  

#define EOS '\0'

XDLL int 
str_search(int argc, rtobj *argv[])
{
	byte	*str;
	int	offset = 0;
	field	*fp;
	field	*search_fp;
	char	SearchString[255];

	/* check for correct argument count	*/
	if (argc < 3)
		return(CSR_FAIL_ARGCOUNT);

	// argument 1 MUST be a field
	if (argv[0]->obj.object != FLD)	{
		mre(-1,"str_search: Invalid Argument[0] type passed");
		return(CSR_FAIL_ARGTYPE);
		}
	search_fp = (field *)argv[0]->ptr;
	str = search_fp->storage;

	// argument 2 can be a fld or a dsc or a text string
	if (rtobj2str(argv[1],SearchString,sizeof(SearchString)) == RT_ERROR)	{
		mre(-1,"str_search: Bad Search String passed");
		return(RT_ERROR);
		}
	
	// argument 3 must be a field
	if (argv[2]->obj.object != FLD)	{
		mre(-1,"str_search: Invalid Argument[2] type passed");
		return(CSR_FAIL_ARGTYPE);
		}
	fp = (field *)argv[2]->ptr;

	// argument 4 specifies the starting offset
	if (argc == 4)
		if (rtobj2int(argv[3],&offset) == RT_ERROR)	{
			mre(-1,"str_search: Bad Search Offset passed");
			return(RT_ERROR);
		}
	// check for a possible error on starting offset being
	// greater that the number of characters
	if ( offset > search_fp->numchars ) {
		offset = -1;
		int2field (offset,fp);
		return -1;
	}
	// add the starting offset (0 if not passed)
	str += offset;
	while (*str != EOS)
	{
		if (strncmp(str,SearchString,strlen(SearchString)) == 0)
		{
			// found it let's set the location
			int2field (offset,fp);
			return (0);
		}
		str++;
		offset++;
	}
	// not found!
	offset = -1;
	int2field (offset,fp);
	return (-1);
}

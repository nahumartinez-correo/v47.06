/*$
**
** SAMISC -- Miscellaneous small routines for System Admin
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.10 $
** $Date: 1996/03/08 19:38:44 $
**
**	Synopsis:
**		Assorted.
**
**	External Function List:
**		orsklock - set ESC_lock TRUE (lock RESET key)
**		orskunlk - set ESC_lock FALSE (unlock RESET key)
**		sacrypt - encrypt a string
**		sagetnode - get node address
**		sacharcheck - checks for special characters
**
**	Notes:
**      None.
**
**  (C) Copyright 1989 by ISC Systems Corp.  All rights reserved.
**
*/
#include "pch.h"

#include <defines.h>
#include <typedefs.h>
#include <dsplist.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include "pclwords.h"
#include <ofbdefs.h>
#include <convers.h>
#include <rt.h>
#include <rtprotos.h>
#include <misc.h>

/*%
**
**  SAORSKLOCK - lock the RESET key out
**
**	Usage:
**		CALLC saorsklock
**
**	Parameters:
**		none
**
**	Returns:
**		ret		-	always zero
**
**	Synopsis:
**      Sets ESC_lock (see rtglobals.c) to TRUE.  This causes the ESC key
**		to be passed thru, rather than act as <RESET>.
**
**	Errors:
**		none
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**		none
*/
XDLL int 
saorsklock(int argc, rtobj *argv[])
{
	++ESC_lock;
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("leaving saorsklock: ESC_lock = %d\n", ESC_lock);
#endif
	return 0;
}
/*%
**
**  SAORSKUNLK - unlock (enable) the RESET key
**
**	Usage:
**		CALLC saorskunlk
**
**	Parameters:
**		none
**
**	Returns:
**		ret		-	always zero
**
**	Synopsis:
**      Sets ESC_lock (see rtglobals.c) to FALSE.  This causes the ESC key
**		to act as <RESET>.
**
**	Errors:
**		none
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**		none
*/
XDLL int 
saorskunlk(int argc, rtobj *argv[])
{
	if (ESC_lock > 0)
		--ESC_lock;
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("leaving saorskunlk: ESC_lock = %d\n", ESC_lock);
#endif
	return 0;
}

/*%
**
**  SAGETNODE - get the node name
**
**	Usage:
**		CALLC sagetnode field
**
**	Parameters:
**		parm1 - field to receive node name value
**
**	Returns:
**		ret		-	VAL_FAIL_NO_MSG if uname() call pukes
**		            VAL_PASS if everything is OK
**					CSR_FAIL_ARGCOUNT (-3) - wrong number of args
**					CSR_FAIL_ARGTYPE (-4) - wrong type of args
**
**	Synopsis:
**      Gets the node name of the machine our windows are on.
**
**	Errors:
**		none
**
**  Author:
**		Kim Juarez - May, 1989
**
**	Notes:
**		revised to use Whichsystem instead of doing a uname() - cap 890620
*/
XDLL int 
sagetnode(int argc, rtobj *argv[])
{
	if (argv[0]->obj.object != CSRARG_FLD ||
		!isASCIIfield((field *)argv[0]->ptr))
	{
		rterrorbox("sagetnode requires an ASCII field as its argument");
		return CSR_FAIL_ARGTYPE;
	}
	str2field(Whichsystem, (field *)argv[0]->ptr);
	return (VAL_PASS);
}

/*%
**
**  SACRYPT - encrypt a string (for password)
**
**	Usage:
**		CALLC sacrypt field
**
**	Parameters:
**		parm1 - field containing string tobe encrypted
**
**	Returns:
**		ret		-	0 if everything was OK
**                  field passed as second parm contains the string
**
**	Synopsis:
**      Encrypts the string, using crypt() system call.
**		Puts the encrypted result in the requested field, truncating the
**		result if it's too long for that field.
**
**	Errors:
**		none
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**		International version is the same - crypt() system call is changed.
*/
XDLL int 
sacrypt(int argc, rtobj *argv[])
{
	char temp[128];

	if (rtobj2str(argv[0], temp, sizeof(temp)) == RT_ERROR)
	{
		rterrorbox("First sacrypt argument must be evaluable as a string");
		return CSR_FAIL_ARGTYPE;
	}
	if (argv[1]->obj.object != FLD)
	{
		rterrorbox("Target for sacrypt must be a field");
		return(CSR_FAIL_ARGTYPE);
	}
	str2field(crypt(temp, "SA"), (field *)argv[1]->ptr);
	return 0;
}

/*%
**
**  SACHARCHECK - checks for illegal special characters
**
**	Usage:
**		CALLC sacharcheck field_ptr
**
**	Parameters:
**		field_ptr:	field *			- pointer to field
**
**	Returns:
**		Zip
**
**	Synopsis:
**      Puts up an error message if illegal special characters are found
**
**	Errors:
**		none
**
**  Author:
**		 Mary Rupert - July, 1989
**
**	Notes:
**		none
*/
XDLL int 
sacharcheck(int argc, rtobj *argv[])
{
	char	*char_ptr;
	int		i;
	int		len;
	char	fldbuf[256];

	if (argv[0]->obj.object != CSRARG_FLD ||
		!isASCIIfield((field *)argv[0]->ptr))
	{
		rterrorbox("sacharcheck requires a field as its argument");
		return CSR_FAIL_ARGTYPE;
	}
	field2str((field *)argv[0]->ptr, fldbuf, sizeof(fldbuf));
	char_ptr = fldbuf;
	len = StringLen(char_ptr);

	for (i = 0; i < len; i++)
	{
		/* valid character: A-Z a-z 0-9 . - _ */
		if ((char_ptr[i] >= 'A'  && char_ptr[i] <= 'Z') ||
			 (char_ptr[i] >= 'a'  && char_ptr[i] <= 'z') ||
			 (char_ptr[i] >= '0' && char_ptr[i] <= '9') ||
			 char_ptr[i] == ' ' || char_ptr[i] == '_' || 
			 char_ptr[i] == '.' || char_ptr[i] == '-')
		{
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("valid character <%c> at index %d\n", char_ptr[i], i);
#endif
			continue;
		}

		/* invalid character */
		else
		{
#			ifdef DEBUGCODEWANTED
				if (Debug)
					printf("INVALID character <%c> at index %d\n", char_ptr[i], i);
#			endif
			rterrorbox(
				"Valid characters are alphabetics, digits, period, hyphen and underscore.");
			return -2;
		}
	}
	return 0;
}
/*%
**
**  SA_GET_FPTR - gets pair of fields in next list member
**
**	Usage:
**		i = sa_get_fptr(fieldlist, num_items, num_fields, field_ptr);
**
**	Parameters:
**		fieldlist: 	 byte **       pointer to pointer to field list
**		num_items:	 int *		   pointer to number of items in list
**		num_fields:	 int 		   number of fields in list
**		field_ptr:	 field **      array of pointers to fields returned
**
**	Returns:
**		-1:			error in LIST
**
**  Author:
**		Mary Rupert - FEB 1990
**
*/
sa_get_fptr(byte **fieldlist, int *num_items, int num_fields, field **field_ptr)
{
	int		i;
	short	objsize;

#	ifdef DEBUGCODEWANTED
		field	**db_field_ptr = field_ptr;
		if (Debug)
			printf("in sa_get_fptr\n");
#	endif

	if (*num_items <= 0)
	{	
		rterrorbox("Field List does not contain enough MEMBERS.");
		return -1;
	}
	
#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("This should be MEMBER byte %x: %x\n",
				MEMBER, **fieldlist);
#	endif

	(*fieldlist)++; 				/* move past MEMBER byte */
#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("This should be # of elements byte: %d\n", **fieldlist);
#	endif

	/* if not correct number of elements in member, error */
	if (**fieldlist != num_fields)
	{
		rterrorbox("Incorrect number of fields in Field List member.");
		return -1;
	}
	(*fieldlist)++;					/* move past # of elements */
#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("Length of member: %d\n",
				(**fieldlist << 8)| *(*fieldlist + 1));
#	endif
	*fieldlist += 2;				/* move past length */

	for (i = 1; i <=num_fields; i++)
	{
#		ifdef DEBUGCODEWANTED
			if (Debug)
				printf("This should be FLD type %x or TYPE_ARRAY %x: %x\n",
					FLD, TYPE_ARRAY, **fieldlist);
#		endif
		if (**fieldlist != FLD &&
			**fieldlist != TYPE_ARRAY) 		/* member must be a field */
		{
			rterrorbox("Field List contains non-field member.");
			return -1;
		}

		if (!(*field_ptr = (field *)rt_get_fp(*fieldlist, &objsize,NULL)))
		{
			rterrorbox("Field in List does not exist.");
			return -1;
		}
		*fieldlist += objsize;
		field_ptr++;
	}
	(*num_items)--;

#	ifdef DEBUGCODEWANTED
		if (Debug)
			for (i=1; i<=num_fields; i++)
				printf("returning field%d = %d\n", i, *db_field_ptr++);
#	endif
	return 0;
}

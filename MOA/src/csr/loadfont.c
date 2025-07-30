/*$
** MODULENAME - loadfont.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:37:58 $
**
**	Synopsis:
**     This routine loads the windowman measurement table for a given font.
**     The intended purpose is to enable preloading of information for 
**     specified fonts during initialization time of an application so 
**     there is no delay for font retrieval the first time it is drawn.
**     (There will still be a first time performance payment for retrieval 
**     of data dictionary objects.)
**
**	External Function List:
**		none.
**
**	Notes:
**		
**  (c) Copyright by ISC-Bunker Ramo Corp., 1989
**        All rights reserved.
**
** Jeanette Sazama, August 1991
*/

#include "pch.h"

#include <csrdef.h>
#include <typedefs.h>
#include <convers.h>
#include <defines.h>
#include <ppdefs.h>
#include <rtprotos.h>


XDLL int
loadfont(int argc,rtobj *argv[])
{
	int			fontno, faceno, sizeno;
#if 0
	TEXTMETRIC	info;
#endif

	/* ---------------------------------------- */
	/*	Arguments are:                          */
	/*		1: Font number                      */
	/*		2: Face number                      */
	/*		3: Size number                      */
	/* ---------------------------------------- */

	/* get the font information */
	if (rtobj2int(argv[0], &fontno) != RT_SUCCESS ||
		rtobj2int(argv[1], &faceno) != RT_SUCCESS ||
		rtobj2int(argv[2], &sizeno) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	
#if 0
	LocalGetFontInfo(fontno,faceno,sizeno,&info);
#endif
	return (0);
}

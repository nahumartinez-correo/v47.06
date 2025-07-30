/*$
** CLEAR_ARRAY --  Clear an array of FIELDs 
**
**	$Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
**	$Revision: 1.6 $
**	$Date: 1996/03/08 19:32:54 $
**
**	External Function List:
**		none
**
**	Notes:
**		This function may become a PCL verb someday...
**
**
**  (c) Copyright by ISC-Bunker Ramo Corp., 1991
**        All rights reserved.
**
**  H. Marc Lewis, Summer 1991
*/

#include "pch.h"

#include <typedefs.h>		/* contains DD object structure defs 	*/
#include <ofbdefs.h>		/* PCL/C defines						*/
#include <convers.h>
#include <rt.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <csrdef.h>

XDLL int
clear_array(int argc, rtobj *argv[])
{
	field	*fp;
	long	rowcol;
	short	rows, cols;
	short	i, j;
	index	ino;
	byte	ano;

	/* check for correct argument count	*/
	if (argc != 1)
		return(CSR_FAIL_ARGCOUNT);

	/* check for correct argument type	*/
	if (argv[0]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);

	fp = (field *) argv[0]->ptr;

	if (!fp->flags.array)
		return(CSR_FAIL_FIELDTYPE);		/* must be an array field */
	ano = argv[0]->obj.applno;
	ino = argv[0]->obj.itemno;
	rowcol = ddarraysize(RTddh,ano,ino);
	rows = (short)(rowcol >> 16);
	cols = rowcol & 0xFFFF;
	if (rows > 0) {
		/* Two-dimensions */
		for (i=0; i < rows; ++i) {
			for (j=0; j < cols; ++j) {
				fp = ddaget(RTddh,ano,ino,i,j);
				if (fp)
					clr_fld_explicit(fp);
				}
			}
		}
	else {
		/* One-dimensional */
		for (j=0; j < cols; ++j) {
			fp = ddaget(RTddh,ano,ino,-1,j);
			if (fp)
				clr_fld_explicit(fp);
			}
		}

	/* return all OK code */
	return(VAL_PASS);
}

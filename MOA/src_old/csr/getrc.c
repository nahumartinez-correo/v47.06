/*$
** MODULENAME - 
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.6 $
** $Date: 1996/03/08 19:37:46 $
**
**	FUNCTION:	Figure out the current row/column of an array field.
**				The field must be the "current field", otherwise the
**				notion of "current row/column" is meaningless and this
**				routine will return -1.
**
**	USAGE:		CALL CSR:getrowcol field,row,col
**
**	PARAMETERS:	
**		field:			a reference to the field on which to operate.
**						Note: this can't be CURRENTFIELD!  The purpose
**						of this argument is to avoid having this routine
**						look up the appl/item numbers.
**		row:			A place for this routine to put the current row.
**		col:			A place for this routine to put the current column.
**
**	NOTE:  Row and column may be omitted.  If one is passed, both must be.
**					
**	RETURNS: 	The same thing placed in col (if specified).
**				Negative numbers indicate failure.
**				Note that "row" will be invalid if the field is 1-dimensional.
**
**	AUTHOR:		Carl Paukstis		August, 1991
*/
#include "pch.h"

#include <typedefs.h>
#include <rtprotos.h>
#include <convers.h>
#include <rtdef.h>
#include <dataman.h>
#include <ofbdefs.h>

XDLL int 
getrowcol(int argc, rtobj *argv[])
{
	field			*fp;
	field			*row, *col;
	unsigned long	rowcol = 0xFFFFFFFF;
	long			asize;
	short			maxrow, maxcol, r, c, found;

	if (argc != 1 && argc != 3) {
		return CSR_FAIL_ARGCOUNT;
		}

	/* the first parameter has to be a reference to the current field */
	if (argv[0]->obj.object != FLD) {
		return(CSR_FAIL_ARGTYPE);
		}
	asize = ddarraysize(RTddh, argv[0]->obj.applno, argv[0]->obj.itemno);
	if (asize <= 0)
		/* Field was not found or is not an array field */
		return -1;
	maxrow = (asize >> 16) - 1;
	maxcol = asize - 1;
	found = 0;
	if (maxrow == -1) /* singly-dimensioned array */
		for (c = 0; c <= maxcol; c++) {
			fp = ddaget(RTddh, argv[0]->obj.applno, argv[0]->obj.itemno, -1, c);
			if (fp == Currentfield) {
				/* Currentfield is an occurrence of the field passed */
				found = 1;
				break;
				}
			}
	else { /* doubly-dimensioned array */
		for (r = 0; r <= maxrow; r++) {
			for (c = 0; c <= maxcol; c++) {
				fp = ddaget(RTddh, argv[0]->obj.applno, argv[0]->obj.itemno, r, c);
				if (fp == Currentfield) {
					/* Currentfield is an occurrence of the field passed */
					found = 1;
					break;
					}
				}
			if (found)
				break;
			}
		}
	if (!found)
		/* Currentfield is NOT an occurrence of the field passed */
		return -1;

	/* do the actual work */
	rowcol = ddgetrowcol(RTddh, argv[0]->obj.applno, argv[0]->obj.itemno, Currentfield);
	if (rowcol == -1)
		return -1;

	/* 2nd and 3rd parameters are optional. If present, must be INT fields */
	if (argc > 1) {
		if (argv[1]->obj.object != FLD) 
			return CSR_FAIL_ARGTYPE;
		row = (field *)argv[1]->ptr;
		if (row->flags.type != FT_INT)
			return CSR_FAIL_ARGTYPE;
		if (argv[2]->obj.object != FLD)
			return CSR_FAIL_ARGTYPE;
		col = (field *)argv[2]->ptr;
		if (col->flags.type != FT_INT)
			return CSR_FAIL_ARGTYPE;
		int2field(rowcol >>16 & 0xFFFF,row);
		int2field(rowcol & 0xFFFF,col);
		}

	return ((int)(rowcol & 0xFFFF));
}

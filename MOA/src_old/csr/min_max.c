/*$
** MODULENAME - min_max.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:38:14 $
**
** Synopsis:
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include "pch.h"

#include <typedefs.h>
#include <csrdef.h>
#include <decmath.h>
#include <convers.h>
#include <ofbdefs.h>
#include <rt.h>
#include <rtprotos.h>

/***********************************************************************/
/*  min_max                                                            */
/*        General 'C' return min or max value                          */
/*        Parms. int argc - number of entries in argv                  */
/*                    *argv[0] - 0 = return min, 1 = return max        */
/*                    *argv[1] - RETURN VALUE (field)                  */
/*                    *argv[n] - field1                                */
/*                    *argv[n+1] - field n                             */
/*                                                                     */
/*   ******** UP TO 8 FIELDS CAN BE CHECKED   **********               */
/*                                                                     */
/*        returns - return string in argv[1]                           */
/*                - return code (0 = RT_SUCCESS)                       */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#define TRUE 1
#define FALSE 0
#define RET_MIN 0
#define RET_MAX 1

XDLL int
min_max(int argc, rtobj *argv[])
{
	int		rc, ret_type, cnt;
	decwork	*val, *pd;
	char	nines[21] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
	
	val = (decwork *) alloca(24);
	pd  = (decwork *) alloca(24);
	memcpy(val->digits,nines,21);
	val->sign = 1;
	val->length = 21;
	val->places = 0;
	rc = VAL_PASS;
	if (argc < 2) {
		rterrorbox("CSR:min_max -- Not enough arguments");
		return(CSR_FAIL_ARGCOUNT);
		}
	rtobj2int(argv[0],&ret_type);
	if (ret_type != 0 && ret_type != 1) {
		rterrorbox("CSR:min_max -- Check type not = 0 (min) or 1(max)");
		return(CSR_FAIL_ARGCOUNT);
		}

	if (ret_type != RET_MIN)
		val->sign = -1;
	cnt = 2;
	while (cnt < argc) {
		rtobj2decwork(argv[cnt],pd);
		if (ret_type == RET_MIN && deccmp(pd,val) < 0)
			memcpy(val,pd,24);
		else {
			if (ret_type == RET_MAX && deccmp(pd,val) > 0)
				memcpy(val,pd,24);
			}
		cnt++;
		}
	decwork2field(val,(field *) argv[1]->ptr);
	return(VAL_PASS);
}

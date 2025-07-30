/*$
** MODULENAME - timecsr.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.5 $
** $Date: 1996/03/08 19:38:58 $
**
** Synopsis:
**   CSRs to do time add/subtract
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include "pch.h"

#include	<defines.h>
#include	<typedefs.h>
#include	<rtdef.h>
#include	<csrdef.h>
#include	<rtprotos.h>
#include	<convers.h>

#define DATESIZE	8
#define	TIMESIZE	6
extern	field		*CsrString;

#define SEC_PER_DAY (24 * 60 * 60) /* number of seconds in a day */

static void sec2time(int,int*,int*,int*);	/* seconds to time int(HHMMSS) */
static int time2sec(int);		/* time from int(HHMMSS) to seconds */

/*eject*/
/* HEADER: int f_time_subtract(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Return the time difference between times in 2 fields
*                   The result will be negative if date/time 2 > date/time 1.
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
* 						[0] = first date
*						[1] = first time
*						[2] = second date
*						[3] = second time
*						[4] = destination field for result
*							If the result field is a "time" type, the 
*							result is expressed in hours,minutes,seconds.
*							Else the result is in seconds.
*
*   RETURN -		0 on success
*					Various negative values on error
*
*	REV. HISTORY -	Date		Name	Description
*					03/21/91	CAP		Created
*****************************************************************************/
XDLL int 
f_time_subtract(int argc, rtobj *argv[])
{
	char	date1[DATESIZE + 1], 
			date2[DATESIZE + 1];
	int time1, time2;
	int wktime2, wktime1;
	field * fp;
	int diff = 0;
	int hh, mm, ss;

	if (argc != 5)
		return CSR_FAIL_ARGCOUNT;

	/* obtain date1 value */
	if (rtobj2str(argv[0], date1, sizeof(date1)) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;
	/* obtain time1 value */
	if (rtobj2int(argv[1], &time1) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;

	/* obtain date2 value */
	if (rtobj2str(argv[2], date2, sizeof(date2)) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;
	/* obtain time2 value */
	if (rtobj2int(argv[3], &time2) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;

	/* get result field pointer */
	if (argv[4]->obj.object != FLD 
		|| ! isNUMERICfield(fp=(field *)argv[4]->ptr))
		return CSR_FAIL_FIELDTYPE;

	/* get date difference in seconds */
	diff = SEC_PER_DAY * date_subtract(date1, date2);

	/* convert times to seconds */
	wktime1 = time2sec(time1);
	wktime2 = time2sec(time2);

	/* add time difference to date difference */
	diff += wktime1 - wktime2;

	/* format and return total difference */
	if (fp->flags.type == FT_TIME) /* result is a time field */
	{
		sec2time(diff, &hh, &mm, &ss);
		int2field(hh*10000 + mm*100 + ss, fp);
	}
	else
		int2field(diff, fp);
		
	return 0;
}
/*eject*/
/* HEADER: int f_seconds2hhmmss(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Convert a "number of seconds" quantity to three fields 
*                   representing hours, minutes, and seconds.
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
* 						[0] = seconds
*						[1] = target field for hours
*						[2] = target field for minutes
*						[3] = target field for seconds
*
*   RETURN -		0 on success
*					Various negative values on error
*
*	REV. HISTORY -	Date		Name	Description
*					03/21/91	CAP		Created
*****************************************************************************/
XDLL int 
f_seconds2hhmmss(int argc, rtobj *argv[])
{
	field * fphh, * fpmm, * fpss;
	int seconds, hh, mm, ss;

	if (argc != 4)
		return CSR_FAIL_ARGCOUNT;

	/* obtain seconds value */
	if (rtobj2int(argv[0], &seconds) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* validate result fields */
	if (argv[1]->obj.object != FLD 
		|| ! isNUMERICfield(fphh=(field *)argv[1]->ptr))
		return CSR_FAIL_ARGTYPE;
	if (argv[2]->obj.object != FLD 
		|| ! isNUMERICfield(fpmm=(field *)argv[2]->ptr))
		return CSR_FAIL_ARGTYPE;
	if (argv[3]->obj.object != FLD 
		|| ! isNUMERICfield(fpss=(field *)argv[3]->ptr))
		return CSR_FAIL_ARGTYPE;

	sec2time(seconds, &hh, &mm, &ss);
	
	/* format and return results */
	int2field(hh, fphh);
	int2field(mm, fpmm);
	int2field(ss, fpss);
		
	return 0;
}
/*eject*/
/* HEADER: int f_hhmmss2seconds(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Convert three fields representing hours, minutes, and 
*                   seconds to a single quantity of seconds
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*						[0] = hours
*						[1] = minutes
*						[2] = seconds
* 						[3] = target field for seconds
*
*   RETURN -		0 on success
*					Various negative values on error
*
*	REV. HISTORY -	Date		Name	Description
*					03/21/91	CAP		Created
*****************************************************************************/
XDLL int 
f_hhmmss2seconds(int argc, rtobj *argv[])
{
	field *fp;
	int time, hh, mm, ss;

	if (argc != 4)
		return CSR_FAIL_ARGCOUNT;

	/* obtain hh, mm, ss values */
	if (rtobj2int(argv[0], &hh) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[1], &mm) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[2], &ss) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* validate result field */
	if (argv[3]->obj.object != FLD 
		|| ! isNUMERICfield(fp=(field *)argv[3]->ptr))
		return CSR_FAIL_ARGTYPE;

	time = hh * 10000 + mm * 100 + ss;

	int2field(time2sec(time), fp);
	
	return 0;
}
/*eject*/
/*
**  Convert a quantity of seconds into three integers representing
**  the number of hours, minutes, and seconds.
*/
static void
sec2time (int seconds, int *hh, int * mm, int * ss)
{
	*hh = seconds / 3600;
	seconds %= 3600;
	*mm = seconds / 60;
	*ss = seconds % 60;
}

/*
**  Convert a single "time" value expressed as an integer in the format
**  hhmmss, into the number of seconds equal to that many hours, minutes,
**  and seconds.
*/
static int
time2sec (int time)
{
	int hh, mm, ss;

	hh = time / 10000;
	time %= 10000;
	mm = time / 100;
	ss = time % 100;
	ss += mm * 60;
	ss += hh * 3600;

	return ss;
}

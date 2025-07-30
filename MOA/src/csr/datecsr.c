/*$
** MODULENAME - datecsr.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.9 $
** $Date: 1996/03/08 19:37:30 $
**
**	Synopsis:
**		csr interface to routines in dates.c
**
**	External Function List:
**
**	Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/


#include	"pch.h"

#include	<ppdefs.h>
#include	<defines.h>
#include	<typedefs.h>
#include	<rtdef.h>
#include	<csrdef.h>
#include	<rtprotos.h>
#include	<convers.h>
#include	<misc.h>
#include	<rtdef.h>

/* #define		DEBUGWANTED */
#define	DATESIZE		(8 + 1)		// internal date plus null byte
#define	MAXPAT			(256 + 1)	// max pattern plus null byte

int daycount(int,int);

/* HEADER: int f_valid_date(int, rtobj**)
******************************************************************************
*	FUNCTION -		Call valid_date to check a date for validity.
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURNS -		VAL_PASS       -    When date is valid
*                   VAL_FAIL_W_MSG -    When date is not valid
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*					06/19/89	CAH		Add argc, argv function call format
*****************************************************************************/
XDLL int 
f_valid_date(int argc, rtobj *argv[])
{
	char	date[DATESIZE];		/* date string			*/
	char	pattern[MAXPAT];	/* edit pattern to apply to date string */

	/* confirm correct argument count */
	if (argc != 1 && argc != 2)
		return(CSR_FAIL_ARGCOUNT);

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* empty fields pass the validation	*/
	if (strcmp(date, "        ") == 0 || strlen(date) == 0)
		return(VAL_PASS);

	/* obtain edit pattern */	
	if (argc == 1)
		strcpy(pattern, date_std_edit);	/* no edit pattern, use default */
	else
	{	/* edit pattern provided passed by invoking function */
		if (rtobj2str(argv[1], pattern, sizeof(pattern)) == RT_ERROR)
			return(CSR_FAIL_ARGTYPE);
	}
	/*	invoke validation function */
	if (valid_date(date, pattern) != VAL_PASS)
		return(VAL_FAIL_W_MSG);
	else
		return(VAL_PASS);
}
/*eject*/
/* HEADER: int f_day_of_week(int, rtobj**)
******************************************************************************
*	FUNCTION -		Return the day of week for the date in a field.
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURNS -		day    -    1 = Sunday, 7 = Saturday
*                               0 = Invalid date
*
*	REV. HISTORY -	Date		Name	Description
*
*					03/27/89	GRN		Created
*					06/19/89	CAH		Add argc, argv function call format
*****************************************************************************/
XDLL int 
f_day_of_week(int argc, rtobj *argv[])
{
	char	date[DATESIZE];

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return(day_of_week(date));
}
/*eject*/
/* HEADER: int day_name(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Move the day of the week in text form to csr_string field
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          VAL_PASS        -  Indicates day name moved to
*                                      csr_string field
*                   VAL_FAIL_NO_MSG -  Couldn't get day name from day
*
*	REV. HISTORY -	Date		Name	Description
*					02/21/89	GRN		Created
*					06/19/89	CAH		Add argc, argv function call format
*****************************************************************************/
XDLL int 
day_name(int argc, rtobj *argv[])
{
	int		day;
	field	*fp;

	rtobj2int(argv[0], &day);	/* obtain day of week number */
	/* has to be an ascii type field 	*/
	if (argv[1]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	fp = argv[1]->ptr;
	if (isASCIIfield(fp) == 0)
		return(CSR_FAIL_ARGTYPE);

	/* check for bad entry */
	if (day < 1 || day > 7)
		day = 0;

	/* return the result */
	str2field(dayname(day),fp);
	if (day)
		return(VAL_PASS);
	else
		return(VAL_FAIL_NO_MSG);
}
/*eject*/
/* HEADER: int month_name(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Move the name of a month in text form to csr_string field
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          VAL_PASS        -  Indicates month name moved to
*                                      csr_string field
*                   VAL_FAIL_NO_MSG -  Couldn't get month name from month
*
*	REV. HISTORY -	Date		Name	Description
*					02/21/89	GRN		Created
*					06/19/89	CAH		add argc, argv call format
*****************************************************************************/
XDLL int 
month_name(int argc, rtobj *argv[])
{
	int		month;
	field	*fp;

	rtobj2int(argv[0], &month);
	/* has to be an ascii type field 	*/
	if (argv[1]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	fp = argv[1]->ptr;
	if (isASCIIfield(fp) == 0)
		return(CSR_FAIL_ARGTYPE);

	/* check for bad entry */
	if (month < 1 || month > 12)
		month = 0;

	/* return the result */
	str2field(monthname(month),fp);
	if (month)
		return(VAL_PASS);
	else
		return(VAL_FAIL_NO_MSG);
}
/*eject*/
/*
******************************************************************************
* HEADER: int f_day_of(int argc, rtobj *argv[])
* HEADER: int f_month_of(int argc, rtobj *argv[])
* HEADER: int f_year_of(int argc, rtobj *argv[])
******************************************************************************
*	FUNCTION -		day_of    - Return numeric day number from a date
*                   month_of  - Return numeric month number from a date
*                   year_of   - Return numeric year number from a date
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          day_of    - Returns numeric day number from date
*                               1 = Sunday, 7 = Saturday
*                   month_of  - Returns numeric month number from date
*                               1 = January, 12 = December
*                   year_of   - Returns numeric year number from date
*
*                   0         - Returned if date not valid
*
*	REV. HISTORY -	Date		Name	Description
*					02/21/89	GRN		Created
*					06/19/89	CAH		add argc, argv format
*****************************************************************************/
XDLL int 
f_day_of(int argc, rtobj *argv[])
{
	char	date[DATESIZE];

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return(day_of(date));
}



XDLL int 
f_month_of(int argc, rtobj *argv[])
{
	char	date[DATESIZE];

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return(month_of(date));
}


XDLL int 
f_year_of(int argc, rtobj *argv[])
{
	char	date[DATESIZE];

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return(year_of(date));
}
/*eject*/
/* HEADER: int f_date_subtract(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Return the number of days between dates in 2 fields
*                   The result will be negative if date2 > date1
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          days      - Number of days between dates
*                               DATEERRCOUNT if either date isn't valid
*
*	REV. HISTORY -	Date		Name	Description
*					02/21/89	GRN		Created
*					06/19/89	CAH		implement argc, argv format
*****************************************************************************/
XDLL int 
f_date_subtract(int argc, rtobj *argv[])
{
	char	date1[DATESIZE], 
			date2[DATESIZE];

	/* obtain date1 value */
	if (rtobj2str(argv[0], date1, sizeof(date1)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* obtain date2 value */
	if (rtobj2str(argv[1], date2, sizeof(date2)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	return(date_subtract(date1, date2));
}
/*eject*/
/* HEADER: int f_date_add(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Given the starting date, this function creates a
*                   date which is the given number of months, days or
*                   years from the starting date.  Negative numbers
*                   may be given to move backward from the starting date.
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          VAL_PASS        -  Indicates new date copied to
*                                      csr_string field
*                   VAL_FAIL_NO_MSG -  Couldn't create new date
*
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*					06/19/89	CAH		use argc, argv format
*****************************************************************************/
XDLL int 
f_date_add(int argc, rtobj *argv[])
{
	char	date[DATESIZE];
	char	fldbuf[DATESIZE];
	int		qty;
	char	type;
	field	*fp;
	int		result;

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* obtain quantity to add	*/
	rtobj2int(argv[1], &qty);
	
	/* obtain type -  'm'onths, 'd'ays, 'y'ears */
	rtobj2char(argv[2], &type);

	/* has to be an ascii type field big enough to hold a date	*/
	if (argv[3]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	fp = argv[3]->ptr;
	if (isASCIIfield(fp) == 0 || fldstosize(fp) < (DATESIZE))
		return(CSR_FAIL_ARGTYPE);
	result = date_add(date, qty, type, fldbuf);
	str2field(fldbuf,fp);
	return(result);
}
/*eject*/
/* HEADER: int f_date_compare(int, rtobj **)
******************************************************************************
*	FUNCTION -		Compare two dates and return which is the larger (the
*                   most future of the two dates)
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          days      - _2GREATER if date2 > date1
*                               _1GREATER if date1 > date2
*                               EQUAL if date1 = date2
*                               VAL_FAIL_NO_MSG if date1 or date2 not valid
*
*	REV. HISTORY -	Date		Name	Description
*					02/21/89	GRN		Created
*					06/19/89	CAH		implement argc, argv format
*****************************************************************************/
XDLL int 
f_date_compare(int argc, rtobj *argv[])
{
	char	date1[DATESIZE], 
			date2[DATESIZE];

	/* obtain date1 value */
	if (rtobj2str(argv[0], date1, sizeof(date1)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* obtain date2 value */
	if (rtobj2str(argv[1], date2, sizeof(date2)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	return(date_compare(date1, date2));
}
/*eject*/
/* HEADER: int f_date_convert(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Convert a date from one format to another
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          VAL_PASS        -  Indicates new date copied to
*                                      csr_string field
*                   VAL_FAIL_NO_MSG -  Couldn't create new date
*
*	REV. HISTORY -	Date		Name	Description
*					02/21/89	GRN		Created
*					06/19/89	CAH		implement argc, argv format
*****************************************************************************/
XDLL int 
f_date_convert(int argc, rtobj *argv[])
{
	char	date[DATESIZE],
			src_pattern[MAXPAT],
			dest_pattern[MAXPAT];
	char	*fldbuf;
	int		argno, len;
	field	*fp;

	/* confirm correct argument count */
	if (argc < 3 || argc > 4)
		return(CSR_FAIL_ARGCOUNT);

	argno = 0;
	/* obtain date value */
	if (rtobj2str(argv[argno++], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	/* obtain edit patterns */	
	if (argc == 3) {
		strcpy(src_pattern, date_std_edit);	/* no edit pattern, use default */
		if (rtobj2str(argv[argno++], dest_pattern, sizeof(dest_pattern)) == RT_ERROR)
			return(CSR_FAIL_ARGTYPE);
		}
	else {	/* argc == 4 */
		/* obtain src_pattern */
		if (rtobj2str(argv[argno++], src_pattern, sizeof(src_pattern)) == RT_ERROR)
			return(CSR_FAIL_ARGTYPE);
		/* obtain destination pattern */
		if (rtobj2str(argv[argno++], dest_pattern, sizeof(dest_pattern)) == RT_ERROR)
			return(CSR_FAIL_ARGTYPE);
		}

	/* has to be an ascii type field big enough to hold a date	*/
	if (argv[argno]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	fp = argv[argno]->ptr;
	if (isASCIIfield(fp) == 0 || fldstosize(fp) < DATESIZE)
		return(CSR_FAIL_ARGTYPE);
	len = fldstosize(fp);
	fldbuf = (char *)alloca(len);
	if (date_convert(fldbuf, len, date,
		src_pattern, dest_pattern) == VAL_PASS) {
	  	str2field(fldbuf,fp);
		return(VAL_PASS);
		}
	else {
		date_set_error(fldbuf);
	  	str2field(fldbuf,fp);
		return(VAL_FAIL_NO_MSG);
		}
}
/*eject*/
/* HEADER: int f_day_of_year(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Convert a date to Gregorian format.
*                            eg. Jan 1 returns 1
*                                Dec 31 returns 365 or 366 (or 355 in 1752)
*
*	PARAMETERS -	int	argc		parameter count
*					rtobj	*argv	parameter values
*
*   RETURN          days     - Gregorian day number, or
*                              VAL_FAIL_NO_MSG if date is not valid
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*					06/19/89	CAH		use argc, argv format
*****************************************************************************/
XDLL int	
f_day_of_year(int argc, rtobj *argv[])
{
	char	date[DATESIZE];

	/* obtain date value */
	if (rtobj2str(argv[0], date, sizeof(date)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	return(day_of_year(date));
}
/*eject*/
/* HEADER: int last_day(int, *rtobj[])
******************************************************************************
*	FUNCTION -		Return the last day of the given month
*
*	PARAMETERS -	argc	parameter count	
*                  argv		parameter values	
*
*   RETURN          VAL_PASS        -  Indicates new date copied to
*                                      csr_string field
*                   VAL_FAIL_NO_MSG -  Couldn't create new date
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*					06/19/89	CAH		Add argc, argv function call format
*****************************************************************************/
XDLL int 
last_day(int argc, rtobj *argv[])
{
	char	fldbuf[DATESIZE];
	int		month, 
			year;
	int		day;
	field	*fp;

	/* get parameter values */
	rtobj2int(argv[0], &month);
	rtobj2int(argv[1], &year);

	/* has to be an ascii type field big enough to hold a date	*/
	if (argv[2]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	fp = argv[2]->ptr;
	if (isASCIIfield(fp) == 0 || fldstosize(fp) < DATESIZE)
		return(CSR_FAIL_ARGTYPE);

#	ifdef DEBUGWANTED
		printf("last_day: month is '%d', year is '%d'\n", month, year);
#	endif

	if (month < 1 || month > 12 || year < 1 || year > 9999) {
		date_set_error((char *)fp->storage);
		month = 0;
		}
	else {
		if (year == 1752 && month == 9)
			day = 30;
		else
			day = daycount(yeartype(year),month);
		make_date_string(fldbuf, year, month, day);
		str2field(fldbuf,fp);
		}

	if (!month)
		return(VAL_FAIL_NO_MSG);
	else
		return(VAL_PASS);
}

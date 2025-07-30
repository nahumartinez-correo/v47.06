/*$
** MODULENAME - rangechk.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.6 $
** $Date: 1996/03/08 19:38:34 $
**
** Synopsis:
**
** External Function List:
**   f_rangechk()
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include	"pch.h"
#include	<typedefs.h>


#include	<ppdefs.h>
#include	<rtprotos.h>
#include	<rtdef.h>
#include	<csrdef.h>
#include	<defines.h>
#include	<convers.h>

XDLL int f_rangechk(int,rtobj **);
int rangechk(char *, char *, char);
static int range_compare(char *, char *, char *, int, int);

#define	MAXSTRING	32
#define	MAXVAL		512		/* maximum length of field to test	*/
#define	MAXRANGE	2048	/* maximum length of range string	*/

/* HEADER: int f_rangechk(int argc, *rtobj[])
******************************************************************************
*	FUNCTION -		Perform a range check on a field, using the given
*                   range and numeric flag
*
*	PARAMETERS -	fld    -	Pointer to a field structure
*                   range  -    Pointer to string containing range
*                   numeric-    Pointer to numeric flag string
*                               ("y" = numeric compare,
*                               otherwise string compare)
*
*   RETURNS -		VAL_PASS  -	If field value is within the range string
*                               contained in field.parms.ptr
*                   VAL_FAIL  - If field value is empty,
*                               field value is not in range, or
*                               the range string is not intelligible.
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*					06/19/89	CAH		Implement argc, argv[] format.
*****************************************************************************/
XDLL int 
f_rangechk(int argc,rtobj *argv[])
{
	char	numeric;
	char	test_value[MAXVAL + 1];
	char	test_range[MAXRANGE + 1];
	field	*fp;

	/* obtain value for test */
	if (rtobj2str(argv[0], test_value, sizeof(test_value)) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	
	/* obtain range value for test */
	if (rtobj2str(argv[1], test_range, sizeof(test_range)) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	
	/* numeric or character compare ? */
	if (rtobj2char(argv[2], &numeric) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	/* make sure it's Y or N	*/
	numeric |= 0x20;	/* make lower case	*/
	if (numeric != 'y' && numeric != 'n')
		return(CSR_FAIL_ARGTYPE);

	/* empty field passes the test	*/
	if (argv[0]->obj.object == FLD) {
		fp = argv[0]->ptr;
		if (fp->numchars == 0)
			return(VAL_PASS);
		}

	return(rangechk(test_value, test_range, numeric));
}

/* HEADER: int rangechk(char *fldval, char *range, char numeric_only)
******************************************************************************
*	FUNCTION -		Perform a range check on a field.
*
*	PARAMETERS -	fldval -	Pointer to value target.
*                   range  -    Pointer to string containing range value
*					numeric_only - 'y' for numeric test, 'n' for string test
*
*   RETURNS -		VAL_PASS  -	If field value is within the range string
*                               contained in field.parms.ptr
*                   VAL_FAIL_NO_MSG  -  If field value is empty,
*                               field value is not in range, or
*                               the range string is not intelligible
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*****************************************************************************/
int 
rangechk(char *fldval, char *range, char numeric_only)
{
	char	lo_alpha[MAXSTRING + 1],
			hi_alpha[MAXSTRING + 1],
			*dest;
	int		hi_lo,				/* set this flag when a range is used	*/
			length,				/* length of target string(s)			*/
			done = 0,
			result = VAL_FAIL_NO_MSG,
			numeric;

#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("rangechk: parms are '%s'  '%s'  '%c'\n",
			 fldval ? fldval : "\"\"", range ? range : "\"\"",
			  numeric_only ? numeric_only : '"');
#	endif

	/* The range string is assumed to be a null terminated	*/
	/* character string.  The ','s & ' 's in the string		*/
	/* separate unique values or ranges of values.  The '-'s*/
	/* separate the low and high values within a range		*/
	/* For example, the range string "1,3,5,7-10,15"		*/
	/* indicates that the unique values of 1,3,5 and 15 are	*/
	/* valid, as are the numbers 7-10.  (If the '-', ',' or	*/
	/* ' ' are required characters in a range string, they	*/
	/* may be preceeded by a '\'.  The '\' itself must occur*/
	/* twice if it is to be used in a range string.)		*/

	if (!strlen(fldval))
		/* empty input string passes validation	*/
		return(VAL_PASS);

	dest = lo_alpha;
	length = 0;
	hi_lo = 0;
	numeric = numeric_only == 'y' ? 1 : 0;

	while (!done) {
		switch (*range) {

			case '\\':
				/* escape character, take next char literally	*/
				range++;

				if (!*range)
					/* no next character, can't escape EOL	*/
					return(VAL_FAIL_NO_MSG);

				/* move char to destination string	*/
				*dest++ = *range++;


				/* make sure it's not too long	*/
				if (++length > MAXSTRING)
					return(VAL_FAIL_NO_MSG);

				/* make sure it's terminated for string comparisons	*/
				*dest = '\0';
				break;

			case '\0':
				/* end of range string, time to wrap it up	*/
				result = range_compare((char *)fldval, lo_alpha, hi_alpha,
					hi_lo, numeric);
				done = 1;
				break;

			case ' ':
			case ',':
				/* separator for values, do range check on current string(s)*/
				result = range_compare(fldval,
					lo_alpha, hi_alpha, hi_lo, numeric);

				if (result == VAL_PASS)
					done = 1;
				else {
					/* reset 'em and try again	*/
					range++;
					dest = lo_alpha;
					length = 0;
					hi_lo = 0;
					}
				break;

			case '-':
				/* separator for hi-lo range entries	*/
				if (hi_lo)
					/* can't have a third entry in range	*/
					return(VAL_FAIL_NO_MSG);

				/* move on to upper limit string	*/
				range++;
				hi_lo = 1;
				dest = hi_alpha;
				length = 0;
				break;

			default:
				/* test for non-numeric character in numeric string	*/
				if (!isdigit(*range) && numeric) {
					return(VAL_FAIL_NO_MSG);
					}

				/* move current char to destination string	*/
				*dest++ = *range++;

				if (++length > MAXSTRING)
					return(VAL_FAIL_NO_MSG);

				/* terminate string for string comparisons	*/
				*dest = '\0';
				break;
			}
		}
	return(result);
}
/*eject*/
/* HEADER: int range_compare(char *value, char *lo_alpha, char *hi_alpha,
							int hi_lo, int numeric)
******************************************************************************
*	FUNCTION -		This function performs a range check.  The check will
*                   be on numeric values if numeric is set, or string values
*                   if numeric is not set.  The check will require an exact
*                   match on lo_alpha if hi_lo is not set, but will perform
*                   a test between lo_alpha and hi_alpha if hi_lo is set.
*
*	PARAMETERS -	value    -  The field's value to be tested
*                   lo_alpha -  The low end of the test range.  This field is
*                               also used for an exact match when a range
*                               test is not being performed.
*                   hi_alpha -  The high end of the test range
*                   hi_lo    -  Set if test should be on a range of values
*                               from lo_alpha to hi_alpha, reset if test
*                               should be a match on lo_alpha
*                   numeric  -  Set if test should be on numeric values,
*                               reset if test should be on strings
*
*   RETURNS -		VAL_PASS   -	If field value is within the range string or
*                               matches the lo_alpha string
*                   VAL_FAIL_NO_MSG  -  If field value is not in range
*
*	REV. HISTORY -	Date		Name	Description
*
*					02/21/89	GRN		Created
*****************************************************************************/
static 
range_compare(value, lo_alpha, hi_alpha, hi_lo, numeric)
	char	*value,
			*lo_alpha,
			*hi_alpha;
	int		hi_lo,
			numeric;
{
	long	hi,
			lo,
			val,
			temp;
	int		result = VAL_FAIL_NO_MSG;
	char	*hi_ptr,
			*lo_ptr;

	if (numeric) {
		str2int(value, (int *)&val);
		str2int(lo_alpha, (int *) &lo);
		if (hi_lo) {
			/* perform hi/lo range check	*/
			str2int(hi_alpha, (int *)&hi);
			if (lo > hi) {
				temp = hi;
				hi = lo;
				lo = temp;
				}
			/* see if the value falls within the hi/lo range	*/
			if (val >= lo && val <= hi)
				result = VAL_PASS;
			}
		else {
			/* single value check looking for exact match 	*/
			if (val == lo)
				result = VAL_PASS;
			}
		}
	else {
		if (hi_lo) {
			/* perform hi/lo range check	*/
			if (strcmp(lo_alpha, hi_alpha) < 0) {
				/* set pointers so that lo_ptr points to smaller string	*/
				lo_ptr = lo_alpha;
				hi_ptr = hi_alpha;
				}
			else {
				lo_ptr = hi_alpha;
				hi_ptr = lo_alpha;
				}

			/* see if the value falls within the hi/lo range	*/
			lo = strcmp(value, lo_ptr);
			hi = strcmp(value, hi_ptr);
			if (lo == 0 || hi == 0 || (lo > 0 && hi < 0)) {
				/* entered string's length must be between lo and hi values	*/
				if (strlen(value) >= strlen(lo_ptr) &&
					strlen(value) <= strlen(hi_ptr)) {
					result = VAL_PASS;
					}
				}
			}
		else {
			/* single value check looking for exact match 	*/
			if (!strcmp(value, lo_alpha))
				result = VAL_PASS;
			}
		}
	return(result);
}
#ifdef STANDALONE
main(argc, argv)
	int		argc;
	char	*argv[];
{
	field	fld;

	/* to test this function, the command line should be of the format:	*/
	/* range fld_value test_range numeric_y_n							*/

	if (argc != 4) {
		printf("usage: range fld_value test_range numeric_y_n\n");
		exit(0);
		}

	fld.storage = (byte *)argv[1];
	fld.flags.numeric_only = (*argv[3] == 'y' ? 1 : 0);
	fld.parms.ptr = argv[2];
	printf("f_rangechk returned %d\n",
		f_rangechk(&fld, (char *)fld.parms.ptr, argv[3]));
	printf("rangechk returned %d\n", rangechk(argv[1], argv[2], argv[3]));

}
#endif

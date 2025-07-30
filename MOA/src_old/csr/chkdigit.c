/*$
** MODULENAME - chkdigit.c
**
**	$Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
**	$Revision: 1.7 $
**	$Date: 1996/03/08 19:32:52 $
**
**	Synopsis:
**		Modulo Check Digit Validation, parameter checking.
** 
**	External Function List:
**		f_check_digit()
**
**	Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/

#include "pch.h"

#include <defines.h>
#include <typedefs.h>
#include <defines.h>
#include <ofbdefs.h>
#include <rt.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <csrdef.h>
#include <convers.h>

#define	IT_FAILED			-1
#define	BAD_MODULUS_VALUE	-2
#define	NON_NUMERIC_FIELD	-3
#define	BAD_WEIGHT_STRING	-4
#define	BAD_CODE			-5

#define	MAXVAL				256

	XDLL int 	f_check_digit(int, rtobj**);
	int 		check_digit(char *, int, char *, char, int, int);

/* Modulo Check Digit Validation */
/* parameter checking */

XDLL int 
f_check_digit(int argc,rtobj *argv[])
{
	char	value[MAXVAL + 1],
			weight[MAXVAL + 1],
			crossfoot;
	int		modval,
			code1,
			code2;

	if (rtobj2str(argv[0], value, sizeof(value)) != RT_SUCCESS ||
		rtobj2int(argv[1], &modval) != RT_SUCCESS ||
		rtobj2str(argv[2], weight, sizeof(weight)) != RT_SUCCESS ||
		rtobj2char(argv[3], &crossfoot) != RT_SUCCESS ||
		rtobj2int(argv[4], &code1) != RT_SUCCESS ||
		rtobj2int(argv[5], &code2) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

		return(check_digit(value, modval, weight, crossfoot, code1, code2));
}

/* Modulo Check Digit Validation */
/*int check_digit(value, modulus_value, weight_string, crossfoot, code1, code2)*/
int check_digit(
	char	*value,				/* value to be checked						*/
	int		modulus_value,		/* Modulus to be used in calculation		*/
	char	*weight_string,		/* null string ("") if no weighting,		*/
								/*		weights separated by commas			*/
	char	crossfoot,			/* 'y' for crossfoot, 'n' for no crossfoot	*/
	int		code1,				/* if check sum is two digit & == mod:		*/
								/*		1 - error							*/
								/*		2 - crossfoot						*/
								/*		3 - check sum = 0					*/
	int		code2)				/* if check sum is two digit & != mod:		*/
								/*		1 - error							*/
								/*		2 - crossfoot						*/
								/*		3 - check sum = 0					*/
{
	int		status;			/* VAL_PASS or VAL_FAIL_W_MSG is returned	*/
	int		check_sum,
			digit,
			digit_count,
			weight_digit,
			action,
			vallength,
			weight_len,
			chk_digit;
	char	weight_flag,
			*digitptr,
			*comma_ptr;
	char	val_buf[MAXVAL];

#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("checkdigit: %c%s%c, %d, %c%s%c, '%c', %d, %d\n",
				'"', value, '"', modulus_value,
					'"', weight_string, '"', crossfoot, code1, code2);
#	endif

	/* check modulus value parameter	*/
	if (modulus_value < 1) {
		return(BAD_MODULUS_VALUE);
		}

	vallength = StringLen(value);
	if (vallength == 0) {
		/* it passes if it's empty (catch the empty fields		*/
		/* by setting the required bit in the field definition	*/
		return(RT_SUCCESS);
		}

	status = RT_SUCCESS;
	check_sum = 0;

	/* set weight flag if a weight string supplied	*/
	weight_len = StringLen(weight_string);
	weight_flag = weight_len ? 'y' : 'n';

	/* make sure value string is equal in length to the weight string */
	/* to do so, prepend with ascii zeroes as needed                  */
	weight_len = weight_len / 2 + 1;
	if (vallength < weight_len) {
		weight_len++;
		memset(val_buf,'0',weight_len);
		memcpy(&val_buf[weight_len - vallength],value,vallength);
		val_buf[weight_len] = '\0';
		value = val_buf;
		vallength = weight_len;
		}
	/* sum each digit up to check digit	*/
	digitptr = value;
	digit_count = 1;
	while (digit_count < vallength && status == RT_SUCCESS) {
		/* if conversion failed, field value is not all numeric */
		if (!isdigit(*digitptr))
			status = NON_NUMERIC_FIELD;
		else {
			digit = (int)*digitptr - (int)'0';
#		ifdef DEBUGCODEWANTED
			if (Debug)
				printf("checkdigit: digit = %d\n", digit);
#		endif

			/* if weight string was specified do weighting for digit */
			if (weight_flag == 'y') {
				/* if no weight string left, error */
				if (! StringLen(weight_string))
					return(BAD_WEIGHT_STRING);

				/* find next comma */
				comma_ptr = weight_string;
				while (*comma_ptr && *comma_ptr != ',')
					comma_ptr++;

				/* terminate string here, get it's numeric equivalent	*/
				if (! StringLen(weight_string))
					return(BAD_WEIGHT_STRING);
				weight_digit = atoi(weight_string);
				if (*comma_ptr) {
					/* there's more weight string	*/
					*comma_ptr = '\0';
					weight_string = comma_ptr + 1;
					}
				else{
					/* no more weight string, point to end of string	*/
					weight_string = comma_ptr;
					}

				/* multiply current digit by weight value	*/
				digit *= weight_digit;
	
				/* do crossfooting if specified and more than one digit */
				if ((crossfoot == 'y' || crossfoot == 'Y') && digit > 9) {
					/* add digits in 2 place number	*/
					digit = (digit / 10) + (digit % 10);
					}
				}
	
			check_sum += digit;
			digit_count++;
			digitptr++;
			} /* digit converts to integer */
		}	/* for each digit in field value, except check digit */

#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("checkdigit: string checksum = %d\n", check_sum);
#	endif

	/* if any weight string left over, error */
	if (status == RT_SUCCESS && StringLen(weight_string) > 1) {
		return(BAD_WEIGHT_STRING);
		}

	/* do final calculations and check */
	if (status == RT_SUCCESS) {
		/* get check digit from value being checked */
		chk_digit = (int)*(value + vallength - 1) - (int)'0';
#		ifdef DEBUGCODEWANTED
			if (Debug)
				printf("checkdigit: chk_digit = %d\n", chk_digit);
#		endif

		/* check digit = modulus value - mod (sum) */
		digit = check_sum % modulus_value;
		check_sum = modulus_value - digit;

		/* if check_sum is two digits */
		if (check_sum > 9) {
			if (check_sum == modulus_value) {
				/* if check sum equal to the modulus value */
				action = code1;
				}
			else {
				/* check_sum not equal to the modulus_value */
				action = code2;
				}
#			ifdef DEBUGCODEWANTED
			if (Debug)
				printf("checkdigit: check_sum = %d, final action = %d\n",
					check_sum, action);
#			endif

			/* take requested action */
			switch (action) {
				/* error */
				case (1):
					status = IT_FAILED;
					break;
				/* cross-foot */
				case (2):
					check_sum = (check_sum / 10) + (check_sum % 10);
					break;		
				/* set to zero */
				case (3):
					check_sum = 0;
					break;
				default:
					status = BAD_CODE;
					break;
				}
			}
	
		if (status == RT_SUCCESS) {
#ifdef DEBUGCODEWANTED
				if (Debug)
					printf(
						"checkdigit: final check, check_sum = %d, chk_digit = %d\n", check_sum, chk_digit);
#endif
			/* finally do the check! */
			if (check_sum != chk_digit) {
				status = IT_FAILED;
				}
			}
		}	/* status == RT_SUCCESS */

	return(status);
}

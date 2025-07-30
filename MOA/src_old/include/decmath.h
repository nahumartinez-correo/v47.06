#include <typedefs.h>

#ifndef DECMATH_H
#define DECMATH_H

#define DECMAX			21			/* max number of work digits */
#define DIVEXTRA		4			/* extra digits for division */
#define MIN_OF(x, y)		((x) > (y) ? (y) : (x))
#define MAX_OF(x, y)		((x) > (y) ? (x) : (y))
#define DMSUCCESS		0
#define DMOVERFLOW		1
#define DMUNDERFLOW		2
#define DIV0			3
#define	RT_TRUNCATE		0	/* truncate unwanted digits (Normal case) */
#define	RT_ROUNDED		1	/* round down if <5, else round up */
#define	RT_ROUNDEDUP	2	/* round up if any digits are >0 */

typedef struct DECWORK {
	char	sign;			/* -1 for negative, +1 for positive				*/
	char	length;			/* total number of digits in number 		 	*/
	char	places;			/* implied decimal is 'places' from the right	*/
	char	digits[1];		/* up to DECMAX digits							*/
	} decwork;				/* bcd digits *ARE NOT* packed (one per byte)	*/

#define	MAXDECWORK	(sizeof(decwork) - 1 + DECMAX)

extern XDLL int deccmp(decwork *num1, decwork *num2);
extern XDLL int decadd(decwork *num1, decwork *num2, decwork *num3);
extern XDLL int decsub(decwork *num1, decwork *num2, decwork *num3);
extern XDLL int decmul(decwork *num1, decwork *num2, decwork *num3);
extern XDLL int decdiv(decwork *num1, decwork *num2, decwork *num3);
extern XDLL int decmod(decwork *num1, decwork *num2, decwork *num3);

#endif

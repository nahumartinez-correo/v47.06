#ifndef _CDSTYPE_H
#define _CDSTYPE_H
/*lint -e46 */

typedef struct cdstype {
	unsigned char dimension :2; /* 0=not array, 1=one dimension, 2= dimensions*/
								/* 3 is invalid */
	unsigned char member   : 1;	/* 1 = single member, 0 = whole array */
								/* must be zero if dimension is 0 */
	unsigned char dtype    : 5;	/* in range FT_ASCII, FT_NUMERIC, FT_FIXED, etc */
} cdstype;

/*lint +e46 */
#define IS_MEMBER (byte) 0x20
#endif


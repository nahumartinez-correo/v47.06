/*
** definition of an ISC Bitmap
*/

#ifndef BITOBJ_H
#define BITOBJ_H
#ifndef _WINDEF_
#include "windef.h"
#endif

#pragma pack (push, 2)
/*
** file header
**
** rowBytes is decoded as follows:
**
** mono:  XXXX XXXX XXXX XXXX
**
**        Where X is the number of bytes needed per row (always even).
**
** color: XXXX XXXX XXXX YYYZ
**
**        Where X/8 is the number of bytes needed per row.
**        Where Y is the number of bits per pixel:   0x000 = 1 bit
**                                                   0x001 = 2 bits
**                                                   0x010 = 4 bits
**                                                   0x011 = 8 bits
**        Where Z is set to indicate a color bitmap.
**
** On color bitmaps, the BO_FL_PALETTE bit is set in the flags field.
** This signifies that following the header is a 'short' containing the
** number of PALETTE structures that follow.  While a bitmap may be
** capable of having 16 colors, the actual image may use fewer colors
** and only have PALETTE entries for the colors used.
*/
struct BITOBJ {
	short	version;		/* struct format */
	short	rowBytes;		/* bytes per row */
	short	width;			/* width in bits */
	short	height;			/* height in bits */
	short	horz_res;		/* horizontal resolution */
	short	vert_res;		/* vertical resolution */
	short	flags;			/* TBD bit flags */
	short	compress;		/* compression type */
};
typedef struct BITOBJ bitobj;

typedef struct BitMap {		/* System Bitmap structure. */
	char *baseAddr;		/* Pointer to bit image. */
	short rowBytes;		/* Number of bytes per row of each plane. */
	RECT bounds;		/* Logical Bounding rectangle of planes. */
} BitMap;

/*
** palette entry
*/
struct PALETTE {
	unsigned short ind;		/* color index */
	unsigned short red;		/* red gun value */
	unsigned short green;	/* green gun value */
	unsigned short blue;	/* blue gun value */
};
typedef struct PALETTE palette;

#pragma pack (pop, 2)

/*
** flags
*/
#define BO_FL_PALETTE (0x0001)	/* Palette entries follow */

/*
** compress types
*/
#define BO_COMP_NONE (0)		/* No Compression */
#define BO_COMP_PACKBITS (1)	/* PackBits() */

#endif

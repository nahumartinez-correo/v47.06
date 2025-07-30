/* ==================================================== */
/*                                                      */
/* dictobjs.h header file that contains macros          */
/* that define the data dictionary objects.             */
/*                                                      */
/* DO NOT CHANGE THESE DEFINES FOR A CUSTOMER PROJECT!  */
/*                                                      */
/* ==================================================== */

#ifndef DICTOBJS_H
#define DICTOBJS_H

/* --------------------------------------- */
/* these defines must be aligned with the  */
/* table entries in fmtbl.c!!!!            */
/* --------------------------------------- */
#define	APL		0		/* configuration record	*/
#define	BIT		1		/* bit map				*/
#define	DOC		2		/* document				*/
#define	DSC		3		/* descriptor			*/
#define	FLD		4		/* field record			*/
#define	ICN		5		/* icon					*/
#define	LST		6		/* compiled list		*/
#define	MNB		7		/* Menu Body			*/
#define	MNU		8		/* header				*/
#define	PAT		9		/* edit pattern			*/
#define	PCL		10		/* PCL routine			*/
#define	OFB		10		/* Olivetti Financial Basic subroutine */
#define	PIC		11		/* pick list header		*/
#define	PLB		12		/* Pick List Body		*/
#define	PMB		13		/* Pick List Menu body	*/
#define	PMU		14		/* Pick List Menu header*/
#define	SCR		15		/* screen				*/
#define	TAG		16		/* tag object reference	*/
#define	UDO		17		/* user defined object - the actual data MUST have	*/
						/* a long at the front of it which is the length of */
						/* the data (including the long itself)             */
#define	CSR		18		/* custom C source routine, now its a real DD object */
#define	PDM		19		/* Win/32 Pull Down Menu */
#define	WMU		20		/* mosaic OA Win/32 Menu */
#define	MLD		21		/* Multi Line Descriptor */

#define DICT_COUNT	22	/* number of dictionary types	*/
						/* DICT_COUNT must be 1 greater than the last */
						/* dictionary object number */

#endif

	/* --------------------------------------------------------------- */
	/*  Information for developers wishing to use the C subroutine     */
	/*  library facility of Pinnacle Plus.  This file contains most    */
	/*  of the information your C subroutine should need to #include.  */
	/*  You, however, should also read the Pinnacle Plus Runtime and   */
	/*  PCL notes document (there's a section on C subs)...            */
	/* --------------------------------------------------------------- */

#ifndef CSRDEF_H
#define CSRDEF_H
#ifndef DATAMAN_H
#	include	<dataman.h>
#endif
#include <time.h>

#define MAX_CSR_NAME		20	/* size of name, same as DD names	   */
#define	MAX_STD_CSR		  1000	/* CSRs 1-999 are "Standard" routines, */
								/* 1000 & up are "Custom" (i.e. yours) */
#define	VAL_PASS		  0		/* return value for validation success */
#define	VAL_FAIL_W_MSG	  -1	/* return value for validation failure */
								/* -1 tells runtime to put up an error */
								/* and ring the keyboard's bell.	   */
#define	VAL_FAIL_NO_MSG	  -2	/* return value for validation failure */
								/* -2 tells runtime not to put up an   */
								/* error or ring the keyboard's bell.  */

#define CSRARG_FLD		FLD		/* arg is FIELD structure pointer		*/
#define CSRARG_DSC		DSC		/* arg is pointer to DESCRIPTOR			*/
#define CSRARG_PCL		PCL		/* arg is PCLDEF structure pointer		*/
#define CSRARG_PAT		PAT		/* arg is pointer to EDIT PATTERN		*/
#define CSRARG_MNU		MNU		/* arg is MENU header structure ptr		*/
#define CSRARG_MNB		MNB		/* arg is Menu Body						*/
#define CSRARG_PIC		PIC		/* arg is a PICk list header			*/
#define CSRARG_PLB		PLB		/* arg is a Pick List Body				*/
#define CSRARG_PMB		PMB		/* arg is a Picklist style Menu Body	*/
#define CSRARG_PMU		PMU		/* arg is a Picklist style MenU header	*/
#define CSRARG_LST		LST		/* arg is PMLIST structure pointer		*/
#define CSRARG_ICN		ICN		/* arg is ICON structure pointer		*/
#define CSRARG_DOC		DOC		/* arg is DOCUMENT structure pointer	*/
#define CSRARG_SCR		SCR		/* arg is SCREEN structure pointer		*/
#define CSRARG_BIT		BIT		/* arg is BIT object structure pointer	*/
#define CSRARG_UDO		UDO		/* arg is UserDefined object pointer	*/
#define CSRARG_CSR		CSR		/* arg is CSR structure pointer			*/
#define CSRARG_WMU		WMU		/* arg is WMU structure pointer			*/
#define CSRARG_PDM		PDM		/* arg is PDM structure pointer			*/
#define CSRARG_MLD		MLD		/* arg is MLD structure pointer			*/

#define CSRARG_STRING	DICT_COUNT + 1	/* arg is ptr to null-term'd string */
#define CSRARG_FIXED	DICT_COUNT + 2	/* arg is ptr to a double float		*/
#define CSRARG_INT   	DICT_COUNT + 3	/* arg is an int,short,char,or long	*/
										/* CSRARG_INT  is the only one of	*/
										/* these that *ISN'T* a pointer to	*/
										/* something!						*/
#define CSRARG_VOID		DICT_COUNT + 4	/* arg is not one of the above...	*/


/* An Event Record as set up in the user_wants_mouseclick logic		*/
/* Note: This replaces the windowman EventRecord structure, and		*/
/* references to the old structure will have to be changed to		*/
/* use this new structure!											*/
/* A commented-out copy of the EventRecord structure is included	*/
/* for reference.													*/

typedef struct CSREvent {
	HWND who;			/* Window identifier */
	int message;		/* Windows event type (WM_xxx)*/
	clock_t when;		/* Time stamp. */
	RECT where;			/* Area or rectangle location. */
} CSREvent;
#if 0
typedef struct EventRecord {	/* An Event Record. */
	short who;		/* Window identifier */
	short what;		/* Event type. */
	long message;		/* Event message. */
	unsigned long when;	/* Time stamp. */
	Rect where;		/* Area or rectangle location. */
} EventRecord;
#endif

typedef int (*int_func)();  /* function that returns an int */

	/* ----------------------------------------------------- */
	/*  Definition of C subroutine argument vector elements  */
	/*  is via struct RTOBJ as defined in typedefs.h         */
	/* ----------------------------------------------------- */


/* =================== Example C routine =================================

	C Subroutines are passed two arguments, a count of the number of
	"true" arguments, and a pointer to an array of pointers to structures
	of type RTOBJ (as defined in typedefs.h).  The RTOBJ struct gives
	a pointer to the data (or the data in the case of CSRARG_INT), its
	type (as obj.object), and the application number and item number as
	obj.applno and obj.itemno (in the case of Data Dictionary objects).
	The argc'th member of the argv array will contain a NULL pointer.

	#include <csrdef.h>
	#include <ofbdefs.h>

	subroutine(argc,argv)
	  int argc;
	  struct RTOBJ *argv[];
	{
		if (argc != whatever)
			return(CSR_FAIL_ARGCOUNT);
		if (argv[0]->obj.object != CSRARG_FLD) {
			errorbox("1st arg to subroutine must be FIELD");
			return(CSR_FAIL_ARGTYPE);
			}
		else
			modify_field(arg[0]->ptr);
		etc...
		return(VAL_PASS);
	}
*/

/* ------------------------------------------------------------------ */
/*  A full set of conversion routines, including conversion from      */
/*  rtobj's to int, string, etc. is available in module conversion.o  */
/*  in the misc.a library.  See the "C Programmer's Guide".           */
/* ------------------------------------------------------------------ */

#endif

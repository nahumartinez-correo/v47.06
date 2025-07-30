/*$
** MODULENAME - prt_pic.c
** Author: Bob Spratt    Aug 1995
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.3 $
** $Date: 1996/03/08 21:54:52 $
**
**	Synopsis:
**		This function will create a picklist of all of the available
**		printers through print manager
**	args
**		arg0 : a picklist header to use
**
**	RETVALS:
**		0 		: success
*/

#include <pch.h>

#include <typedefs.h>		/* contains DD object structure defs */
#include <rtdef.h>		/* Contains the Retval global declaration */
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <convers.h>
#include <rt.h>
#include <defines.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <msgrtn.h>	  
#include <c.h>
#include <clconn.h>
#include <cdsfuncs.h>
#include <cmfield.h>
#include <clcdsfn.h>
#ifndef CLCDSENV_H
#include <clcdsenv.h>
#endif
#include <rtable.h>
#include <wtypes.h>
#include <winspool.h>

static	picbody		*picbod=NULL;
static	picheader	*ph=NULL;

XDLL int print_picklist(int argc, rtobj *argv[]);
XDLL int print_picklist_free();

int print_picklist(int argc, rtobj *argv[])
{
	char		*pbptr;
	int		totbytes=0;
   	PRINTER_INFO_1	Printer_List[50];
	DWORD		Printer_dwType;
	DWORD		Printer_lpdwNeeded;
	DWORD		Printer_lpdwReturned;
	int 		i=0;
	int			z=0;
	
	// get all of the local and network connected printers
	Printer_dwType = PRINTER_ENUM_CONNECTIONS + PRINTER_ENUM_LOCAL;

	// retrieve all of the printer names into the array
	// of printer info structures
	if ( !EnumPrinters(Printer_dwType,NULL,1,(char *)Printer_List,sizeof(Printer_List),&Printer_lpdwNeeded,&Printer_lpdwReturned))
		return RT_ERROR;

	// none were found, lets get out of here
	if ( Printer_lpdwReturned < 1 )
		return RT_ERROR;

	// argument must be of type PIC
	if ( argv[0]->obj.object != PIC ) {
		mre(-1,"Argument must be a picklist header");
		return(CSR_FAIL_ARGTYPE);
		}

	// get the picklist from the arguments passed
	ph = (picheader *)argv[0]->ptr;

	// calculate the total memory needed for the piclist body
	while ( Printer_List[i].pName != NULL && i < 50)
		totbytes += strlen(Printer_List[i++].pName) + 1;

	// malloc the storage for the body
	picbod = (picbody *)malloc(sizeof(picbody) + totbytes);
	pbptr = (char *) (picbod + 1);
	picbod->numitems = 0;
	i = 0;
	// add the items to the piclist
	while ( Printer_List[i].pName != NULL && i < 50) {
		strcpy(pbptr,Printer_List[i].pName);
		pbptr += (strlen(Printer_List[i++].pName) + 1);
		picbod->numitems++;
	}
	picbod->size = totbytes;
	ph->list.ptr = picbod;
	return(RT_SUCCESS);
}

// free the memory allocated by the print_picklist
// subroutine
int print_picklist_free()
{
	free(picbod);
	return(RT_SUCCESS);
}	

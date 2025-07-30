/*$
** MODULENAME - prtscrn.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 $
** $Revision: 1.10 $
** $Date: 1997/11/18 21:33:12 $
**
** Synopsis:
**   csr to do some printut type functions
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include        "pch.h"

#include        <defines.h>
#include        <typedefs.h>
#include        <ofbdefs.h>
#include        <ofbwords.h>
#include        <rtdef.h>
#include        <csrdef.h>
#include        <rt.h>
#include        <rtprotos.h>
#include        <convers.h>

// 	extern int DDerrno;		// Trick: in DATAMAN.H

/*eject*/
// HEADER: int print_utility(int, *rtobj[])
//****************************************************************************
//	FUNCTION -		Capture the desktop or active window and print the image.
//
//	PARAMETERS -	int	argc		parameter count
//					rtobj	*argv	parameter values
// 						[0] = image options
//						[1] = print options
//
//  RETURN -		0 on success
//					Various negative values on error
//
//	REV. HISTORY -	Date		Name	Description
//					04/13/94	DRB		Created
//***************************************************************************
int print_utility(int argc, rtobj *argv[])
{
	int     capture_option, print_option, xscale, yscale;
	static  HANDLE hCSRdll;
	FARPROC procaddr;
	char filename[128];

	if (argc != 4)
		return CSR_FAIL_ARGCOUNT;

	/* obtain image value */
	if (rtobj2int(argv[0], &capture_option) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;

	/* obtain print value */
	if (rtobj2int(argv[1], &print_option) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;

	// verify that CSR proc exists in the DIBAPI.DLL
	// get proc address from DIBAPI.DLL
	if (hCSRdll == NULL) {
		hCSRdll = LoadLibrary("DIBAPI.DLL");
		if (hCSRdll == NULL) {
			printf("print_utility: unable to locate \"DIBAPI.DLL\"\n");
			DDerrno = GetLastError();
			return 0;
		}
	}

	/* check for file save option */
	if ( print_option == 9 ) {
		if ( rtobj2str(argv[2], filename, sizeof(filename) ) == RT_ERROR)
			return CSR_FAIL_ARGTYPE;
		// Obtain required proc address
		procaddr = GetProcAddress(hCSRdll,"DoCaptureFile");
		if (procaddr == NULL) {
			DDerrno = GetLastError();
			return 0;
		}
		// call the entry dll
	  	return ((procaddr) (capture_option, (LPSTR)filename));
	}
	else {
	  /* obtain print xscale */
	  if (rtobj2int(argv[2], &xscale) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;

	  /* obtain print yscale */
	  if (rtobj2int(argv[3], &yscale) == RT_ERROR)
		return CSR_FAIL_ARGTYPE;

	  // Obtain required proc address
	  procaddr = GetProcAddress(hCSRdll,"DoCapture");
	  if (procaddr == NULL) {
		DDerrno = GetLastError();
		return (DDerrno);
	  }

	  // call the entry dll
	  return ((procaddr) (capture_option, print_option, xscale, yscale));
	}
}

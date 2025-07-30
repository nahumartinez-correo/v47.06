/*$
** MODULENAME - copyrec.c
** Author: Bob Spratt    May 1995
**
**	$Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
**	$Revision: 1.4 $
**	$Date: 1996/04/04 17:20:48 $
**
**	Synopsis:
**		This function will take the current selected record from
**		the source select and move the buffer to another select context.
**		this is used in the emergency journaling to move records
**		from one table to the another.
**	args
**		arg0 : Name of the context copied. (INPUT)
**		arg1 : Name of the destination context. (OUTPUT)
**
**	RETVALS:
**		0 		: success
**		-1		: source context invalid
**		-2		: destination context invalid
**		-3		: destination buffer size < source buffer size
**		-4		: source buffer pointer is null
**		-5		: buffer pointer too large to fit
**		RT_ERROR: invalid parameters.
*/

#include "pch.h"	
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
#include "cdscsr.h"

XDLL int copy_record(int argc, rtobj *argv[]);

copy_record(int argc, rtobj *argv[])
{
	char		Buf[256];
	reclen		BufLength;
	byte		*rp;
	cdsselect	*SourcePtr;
	cdsselect	*DestPtr;
	
	if (argc < 2)	{
		mre(-1,"Invalid Number of arguments passed");
		return(CSR_FAIL_ARGCOUNT);
		}
	/* obtain source select context */
	if (rtobj2str(argv[0],Buf,sizeof(Buf)) == RT_ERROR) {
		mre(-1,"Invalid Source Context");
		return(-1);
		}
	if ((SourcePtr = find_clcontext(CdsEnv, Buf)) == NULL) {
		mre(-1,"Invalid Source Context");
		return(-2);
		}
	/* obtain destination select context */
	if (rtobj2str(argv[1],Buf,sizeof(Buf)) == RT_ERROR) {
		mre(-1,"Invalid Destination Context");
		return(-1);
		}
	if ((DestPtr = find_clcontext(CdsEnv, Buf)) == NULL) {
		mre(-1,"Invalid Destination Context");
		return(-2);
		}
	// if the destination buffer length is less than the source we will
	// stop it here.
	if ( DestPtr->bufflen < SourcePtr->bufflen ) {
		mre(-1,"Destination Buffer Size Smaller Than Source Buffer Size");
		return(-3);
		}
	  
	// get the buffer pointer..
	if (SourcePtr->state & MULTIREC)		/* multirecord buffer 		   */
		rp = (byte *) SourcePtr->get;		/* record pointer 			   */
	else						/* single record buffer		   */
		rp = (byte *) SourcePtr->getbase;	/* record pointer 			   */

	// is the record there? This could happen if they do a select
	// with and no cds find* or if no records are found but they call
	// this thing anyway
	if ( rp == NULL ) {
		mre(-1,"Source Context Invalid (NULL Record Pointer)");
		return(-4);
		}
	// grab the length out of the buffer	
	BufLength = RECLEN(rp);
	// this should probably never happen, but just in case..
	if ( BufLength > DestPtr->bufflen ) {
		mre(-1,"Buffer too large");
		return(-5);
		}
	memcpy(DestPtr->putbase,rp,BufLength);
	// update the put pointer to the end of the buffer
	DestPtr->put = (byte *)DestPtr->putbase + BufLength;
	// update varable  that tells the space left in the buffer
	DestPtr->bufflen = DestPtr->tbl->buffsize - BufLength - 2;
	return(0);
 }


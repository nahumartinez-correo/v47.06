/*$
** MODULENAME - new_pic.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.10 $
** $Date: 1996/03/08 19:38:16 $
**
** Synopsis:
**   Redraws a new picture over the current one
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/


#include "pch.h"

#include <typedefs.h>
#include <rtprotos.h>
#include <tdspl.h>
#include <dsplist.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>			/* contains C subroutine structure defs */
#include <dsplist.h>
#include <scrlist.h>
#include <defines.h>
#include <rtprotos.h>
#include <convers.h>
#include <memory.h>
#include <memalloc.h>

	extern			HANDLE hRTdll;

	void			csr_drawbmp(dlist *);

	static	void	*rtdrawbmp;
	static	char	filename[128];
	static  int		x_loc, y_loc, width, height, fore, back;


XDLL int
new_pic(int argc, rtobj *argv[])
{
	char	buf[128];
	dlist	*dlp;
	int		type, sv_nNumOfVisibleBMP;
	struct _stat	filestat;

	if (rtobj2int(argv[0],&type) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (type <= 0 || type > 2) {
		rterrorbox("newbmp: Invalid option");
		return(CSR_FAIL_ARGTYPE);
		}
	if (type == 1) {
		if (rtobj2str(argv[1],buf,sizeof(buf)) == RT_ERROR)
			return(CSR_FAIL_ARGTYPE);
		if (argc != 2)
			return(CSR_FAIL_ARGTYPE);
		x_loc = y_loc = width = height = fore = back = 0;
		}
	if (type == 2) {
		if (argc != 8)
			return(CSR_FAIL_ARGTYPE);
		if (rtobj2int(argv[1],&x_loc) != RT_SUCCESS ||
			rtobj2int(argv[2],&y_loc) != RT_SUCCESS ||
			rtobj2int(argv[3],&width) != RT_SUCCESS ||
			rtobj2int(argv[4],&height) != RT_SUCCESS ||
			rtobj2int(argv[5],&fore) != RT_SUCCESS ||
			rtobj2int(argv[6],&back) != RT_SUCCESS ||
			rtobj2str(argv[7],buf,sizeof(buf)) == RT_ERROR)
			return(CSR_FAIL_ARGTYPE);
		}
	if (hRTdll == NULL) {
		hRTdll = LoadLibrary("rt.dll");
		if (!hRTdll)
			return(RT_ERROR);
		}
	if (rtdrawbmp == NULL) {
		rtdrawbmp = GetProcAddress(hRTdll,"drawpicture");
		if (!rtdrawbmp)
			return(RT_ERROR);
		}
	strcpy(filename,xlate_filename(buf));
	dlp = dl_getfirst(PCLscreen);
	while (dlp) {
		if (dlp->dspfunc == rtdrawbmp || dlp->dspfunc == csr_drawbmp) {
			if  (_stat(filename,&filestat)) {
				sprintf(buf,"newbmp: Error opening picture file%s",filename);
				rterrorbox("newbmp: Error opening picture file");
				return(RT_ERROR);
				}
			// --------------------------------------
			// clear previous bitmap rectangle
			// with current screen background color
			// --------------------------------------
			InvalRect(&dlp->bounds);
#if DEBUGCODEWANTED
if (Debug) {
printf("invalrect on: t,l,r,b = %d,%d,%d,%d\n",dlp->bounds.top,
dlp->bounds.left,
dlp->bounds.right,
dlp->bounds.bottom);
}
#endif
			// set up new bitmap location
			dlp->dspfunc = csr_drawbmp;
			if (dlp->bounds.left != x_loc)
				dlp->bounds.left = x_loc;
			if (dlp->bounds.top != y_loc)
				dlp->bounds.top = y_loc;
			SetRect(&dlp->bounds, dlp->x, dlp->y,
        		dlp->x + width, dlp->y + height);
			// ------------------------------------------------------------
			// Width and Height are ignored, the bitmap is not re-sized...
			// Forecolor and Backcolor are not supported by .BMP formatted
			// bitmaps.
			// ------------------------------------------------------------
			sv_nNumOfVisibleBMP = nNumOfVisibleBMP;
			csr_drawbmp(dlp);
			nNumOfVisibleBMP = sv_nNumOfVisibleBMP;
			UpdateWindow(hWnd);
			return(0);
			}
	 	dlp = dl_getnext(PCLscreen,dlp);
		}
	rterrorbox("newbmp: No picture currently displayed");
	return (-1);
}


void
csr_drawbmp(dlist *dlp)
{
	struct _stat	filestat;

	if (_stat(filename,&filestat))
		return;

	drawcolorpic(filename,0,0,dlp,NULL);
}


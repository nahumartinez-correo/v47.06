/*$
** MODULENAME - stamptxt.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.3 $
** $Date: 1996/03/08 19:38:54 $
**
** Synopsis:
**   ??
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include	"pch.h"

#include	<typedefs.h>
#include	<ofbdefs.h>
#include	<convers.h>
#include	<errno.h>
#include	<defines.h>
#include	<rtdef.h>
#include	<dataman.h>
#include	<csrdef.h>
#include	<pclwords.h>
#include	<errdefs.h>
#include	<rtprotos.h>
#include	<tdspl.h>
#include	<dsplist.h>
#include	<rtfonts.h>

void
drawcleartext(dlist *dlp)
{
	HDC			hdc;
	char		out[256];
	int			oldmode,
				width;
	TEXTMETRIC	finfo;
	HFONT		hfont;

	rtobj2str(&dlp->dyn_val, out, sizeof(out));
	hdc = getdc();
	oldmode = GetBkMode(hdc);
	SetBkMode(hdc, TRANSPARENT);
	set_look(dlp->font,dlp->attr,dlp->size, dlp->forecolor,dlp->backcolor);
	hfont = selectfont();
	SelectObject(hdc, hfont);
	if (dlp->bounds.left == 0 && dlp->bounds.right == 0) {
		/* rectangle not set yet	*/
		getfont(&finfo);
		width = LocalStringWidth(dlp->font, dlp->attr, dlp->size, out);

		switch (dlp->uflags) {
			case 0:
				/* left justified, don't need to change x coordinate	*/
				break;
			case 1:
				/* center justified	*/
				dlp->x -= (short)(width / 2);
				break;
			case 2:
				/* right justified	*/
				dlp->x -= (short)width;
				break;
			}
		dlp->bounds.left = dlp->x;
		dlp->bounds.top = dlp->y - finfo.tmAscent;
		dlp->bounds.bottom = dlp->bounds.top + finfo.tmHeight;
		dlp->bounds.right = dlp->bounds.left + (short)width;
		}
	ExtTextOut(hdc, dlp->x, dlp->y, 0, (RECT FAR *)NULL,
		out, strlen(out), (LPINT)NULL);
	SetBkMode(hdc, oldmode);
	releasedc();
}


int 
stamp_text(int argc, rtobj *argv[])
{
	int		x,
			y,
			just;
	char	thestring[256];
	dlist	*dlp;

	/* check for correct argument count	*/
	if (argc != 4)
		return(CSR_FAIL_ARGCOUNT);

	if (rtobj2int(argv[0], &x) != RT_SUCCESS ||
		rtobj2int(argv[1], &y) != RT_SUCCESS ||
		rtobj2int(argv[2], &just) != RT_SUCCESS ||
		rtobj2str(argv[3], thestring, sizeof(thestring)) != RT_SUCCESS) {
		return(CSR_FAIL_ARGTYPE);
		}
	if (argv[3]->obj.object != CSRARG_FLD &&
		argv[3]->obj.object != FLD && argv[3]->obj.object != DSC) {
		/* has to be a field or descriptor, inline text won't		*/
		/* work because it won't be there when it's time to refresh	*/
		return(CSR_FAIL_ARGTYPE);
		}
	dlp = dl_draw(PCLscreen, (short)x, (short)y, NULL, NULL, NULL);
	dlp->dspfunc = drawcleartext;
	dlp->uflags = (short)just;
	dlp->dyn_val = *argv[3];	/* save rtobj in dyn_val slot	*/
	drawcleartext(dlp);
	return(RT_SUCCESS);
}


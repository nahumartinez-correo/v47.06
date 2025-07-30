// =========================================================================
//
// MODULENAME - wclist.c
//
// $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
// $Revision: 1.10 $
// $Date: 1996/03/08 19:39:06 $
//
// Synopsis:
//  This routine (CSR:WCLIST) provides an interface to some code inside
//  the IBA's Compiled List Author for manipulating COMPLIST objects in
//  memory.  It has the same calling conventions as the CSR of the same
//  name in Pinnacle Plus (thus making conversion of PCL to OFB code
//  easier).  The work, however, is now done in a C++ Class object within
//  the IBA, so this routine must call there to have the work done.
//  
//  This trick is accomplished by having the IBA call the second routine
//  below (WCLIST_SETUP) whose sole purpose is to stuff the address of
//  the routine inside the COMPLSIT Author into 'ibafunc' just below.
//  This allows WCLIST() to call from here (C code) into the IBA (C++ code)
//
//  H. Marc Lewis, June-1994, Spokane, WA
// =========================================================================

#include "pch.h"

#include <typedefs.h>
#include <ppdefs.h>
#include <pclwords.h>
#include <dpwords.h>
#include <dsplist.h>
#include <tdspl.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <textedit.h>

extern void drawfield();
extern void redrawcontrol();


	// -----------------------------------------------------------------
	// This variable will hold the address of the IBA's internal routine
	// -----------------------------------------------------------------
	static int	(*ibafunc)(int funcno, int argc, rtobj *argv[]);


XDLL int 
wclist(int argc,rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(1,argc,argv));
	else
		return(-99);
}

XDLL int
parsename(int argc,rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(2,argc,argv));
	else
		return(-99);
}

XDLL int
checkobj(int argc,rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(3,argc,argv));
	else
		return(-99);
}

static HANDLE hRTdll;
static void	*drawtefldfunc,*drawfieldfunc,*redrawcontrolfunc,*drawtextfunc,*drawstringfunc;

static int loadrtfuncs()
{
	if (hRTdll == NULL) {
		// Get real address we need
		hRTdll = LoadLibrary("rt.dll");
		if (!hRTdll)
			return(-1);
		drawtefldfunc = GetProcAddress(hRTdll,"drawTEfield");
		if (!drawtefldfunc)
			return(-1);
		drawfieldfunc = GetProcAddress(hRTdll,"drawfield");
		if (!drawfieldfunc)
			return(-1);
		redrawcontrolfunc = GetProcAddress(hRTdll,"redrawcontrol");
		if (!redrawcontrolfunc)
			return(-1);
		drawtextfunc = GetProcAddress(hRTdll,"drawtext");
		if (!drawtextfunc)
			return(-1);
		drawstringfunc = GetProcAddress(hRTdll,"drawstring");
		if (!drawstringfunc)
			return(-1);
		}
	return 0;
}

static enable_by_fld(fp)
field	*fp;
{
	dlist	*dl, *lsthead;

	loadrtfuncs();
	lsthead = NULL;
	for (;;) {
		if (lsthead == NULL) 
			lsthead = PCLscreen;
		else if (lsthead == PCLscreen)
			lsthead = PCLfields;
		else
			break;


		dl = dl_getfirst(lsthead);
		while (dl) {
			if (dl->dspfunc == drawfieldfunc) {
				field *nfp;
				nfp = dl->obj;
				if (nfp == fp) {
					dl->attr &= ~DTGHOST;
					drawfield(dl);
						}
				dl = dl_getnext(lsthead,dl);
				continue;
				}
			if (dl->dspfunc == drawtefldfunc) {
				field		*nfp;
				struct RT_TEFIELD *tr = dl->obj;
				nfp = tr->fp;
				if (nfp == fp) {
					dl->attr &= ~DTGHOST;
					fp->flags.dyn_prot = 0;
					TESetText((char *)fp->storage,fp->length,(struct TERec *)tr->terec);
					drawTEfield(dl);
					}
				dl = dl_getnext(lsthead,dl);
				continue;
				}
			if (dl->dspfunc == redrawcontrolfunc) {
				rtcontrol	*crp;
				field		*crpfp;
				int			*ip,bitval;
				char		fvalu[40];

				crp = dl->obj;
				switch (crp->type) {
					case DP_CHECKBOX:
						crpfp = crp->object;
						if (crpfp != fp)
							break;
						ip = (int *) crpfp->storage;
						bitval = (int) crp->subject;
						if (*ip & (1 << bitval))
							SetCtlValue((HWND)crp->cid,1);
						else
							SetCtlValue((HWND)crp->cid,0);
						crp->flags &= ~DL_PROTECTED;
						HiliteControl((HWND)crp->cid,0);
						break;
					case DP_RADIOBUTTON:
						crpfp = crp->object;
						if (crpfp != fp)
							break;
						pcl2str((byte *)crp->subject,fvalu,sizeof(fvalu)-1,NULL);
						if (strcmp(fp->storage,fvalu) == 0)
							SetCtlValue((HWND)crp->cid,1);
						else
							SetCtlValue((HWND)crp->cid,0);
						crp->flags &= ~DL_PROTECTED;
						HiliteControl((HWND)crp->cid,0);
						break;
					default:
						break;
					}
				dl = dl_getnext(lsthead,dl);
				continue;
				}
			dl = dl_getnext(lsthead,dl);
			}
	}
	return 0;
}

static disable_by_fld(fp)
field	*fp;
{
	dlist	*dl, *lsthead;

	loadrtfuncs();
	lsthead = NULL;
	for (;;) {
		if (lsthead == NULL) 
			lsthead = PCLscreen;
		else if (lsthead == PCLscreen)
			lsthead = PCLfields;
		else
			break;


		dl = dl_getfirst(lsthead);
		while (dl) {
			if (dl->dspfunc == drawfieldfunc) {
				field	*nfp;
				nfp = dl->obj;
				if (nfp == fp) {
					dl->attr |= DTGHOST;
					fp->flags.dyn_prot = 1;
					drawfield(dl);
					}
				dl = dl_getnext(lsthead,dl);
				continue;
				}
			if (dl->dspfunc == drawtefldfunc) {
				field		*nfp;
				struct RT_TEFIELD *tr = dl->obj;
				nfp = tr->fp;
				if (nfp == fp) {
					dl->attr |= DTGHOST;
					fp->flags.dyn_prot = 1;
					TEDeactivate((struct TERec *)tr->terec);
					drawTEfield(dl);
					}
				dl = dl_getnext(lsthead,dl);
				continue;
				}
			if (dl->dspfunc == redrawcontrolfunc) {
				rtcontrol	*crp;
				field		*crpfp;

				crp = dl->obj;
				switch (crp->type) {
					case DP_CHECKBOX:
					case DP_RADIOBUTTON:
						crpfp = crp->object;
						if (crpfp != fp)
							break;
						crp->flags |= DL_PROTECTED;
						HiliteControl((HWND)crp->cid,255);
						break;
					default:
						break;
					}
				dl = dl_getnext(lsthead,dl);
				continue;
				}
			dl = dl_getnext(lsthead,dl);
			}
	}
	return 0;
}

static enable_by_str(sp)
char	*sp;
{
	dlist	*dl;
	char	*dlstr;

	loadrtfuncs();
	dl = dl_getfirst(PCLscreen);
	while (dl) {
		if (dl->dspfunc == drawstringfunc) {
			dlstr = dl->obj;
			if (strcmp(dlstr,sp) == 0) {
				dl->attr &= ~DTGHOST;
				drawstring(dl);
				}
			}
		else if (dl->dspfunc == drawtextfunc) {
			dlstr = dl->obj;
			if (strncmp(&dlstr[1],sp,strlen(sp)) == 0) {
				dl->attr &= ~DTGHOST;
				drawtext(dl);
				}
			}
		dl = dl_getnext(PCLscreen,dl);
		}
	return 0;
}

static disable_by_str(sp)
char	*sp;
{
	dlist	*dl;
	char	*dlstr;

	loadrtfuncs();
	dl = dl_getfirst(PCLscreen);
	while (dl) {
		if (dl->dspfunc == drawstringfunc) {
			dlstr = dl->obj;
			if (strcmp(dlstr,sp) == 0) {
				dl->attr |= DTGHOST;
				drawstring(dl);
				}
			}
		else if (dl->dspfunc == drawtextfunc) {
			dlstr = dl->obj;
			if (strncmp(&dlstr[1],sp,strlen(sp)) == 0) {
				dl->attr |= DTGHOST;
				drawtext(dl);
				}
			}
		dl = dl_getnext(PCLscreen,dl);
		}
	return 0;
}

XDLL int
dev_enable(argc,argv)
int		argc;
rtobj	**argv;
{
	short	i;

	for (i=0; i<argc; i++) {
		switch(argv[i]->obj.object) {
			case CSRARG_FLD:
				enable_by_fld(argv[i]->ptr);
				break;
			case CSRARG_STRING:
				enable_by_str(argv[i]->ptr);
				break;
			default:
				break;
			}
		}
	return 0;
}

XDLL int
dev_disable(argc,argv)
int		argc;
rtobj	**argv;
{
	short	i;

	for (i=0; i<argc; i++) {
		switch(argv[i]->obj.object) {
			case CSRARG_FLD:
				disable_by_fld(argv[i]->ptr);
				break;
			case CSRARG_STRING:
				disable_by_str(argv[i]->ptr);
				break;
			default:
				break;
			}
		}
	return 0;
}

XDLL int
devcsr_info(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(6,argc,argv));
	else
		return(-99);
}

XDLL int
devcsr_qbox(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(7,argc,argv));
	else
		return(-99);
}

XDLL int
devcsr_newobj(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(8,argc,argv));
	else
		return(-99);
}

XDLL int
Pickcontrol(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(9,argc,argv));
	else
		return(-99);
}


XDLL int
Pickremove(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(10,argc,argv));
	else
		return(-99);
}


XDLL int
Pickdisplay(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(11,argc,argv));
	else
		return(-99);
}


XDLL int
PickLoad(int argc, rtobj *argv[])
{
	if (ibafunc)
		return((*ibafunc)(12,argc,argv));
	else
		return(-99);
}

// ====================================================================
//  This routine is called *ONLY* by the IBA, and *SHOULD NOT* be made
//  accessable to OFB code.  i.e. don't put an entry in the CSR import
//  file for this routine!
// ====================================================================
XDLL void
wclist_setup(void *arg)
{
	ibafunc = arg;
}

/*$
** MODULENAME - matchobj.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.9 $
** $Date: 1996/03/08 19:38:10 $
**
**	Synopsis:
**		This function will generate a picklist of objects that match the
**		application and key that were passed as a parameter.
**		The picklist will not be displayed if any of the picklist items 
**		matches the contents of the given field.
**
**	Returns:	0  if picklist returned normally
**				1  if given field passed validation--no picklist displayed
**				2  if no matches found
**			  < 0  if picklist exited by RESET
**				CSR_FAIL_ARGTYPE  if an invalid argument type is discovered
**
**	External Function List:
**		none.
**
**	Parameters:
**		Application
**		Key to search for
**		Field name for validation and stuffing the selection
**		Picklist header
**
**  (c) Copyright by ISC-Bunker Ramo Corp., 1989
**        All rights reserved.
**
*/

#include "pch.h"

#include <typedefs.h>
#include <ofbdefs.h>
#include <defines.h>
#include <dsplist.h>
#include <tdspl.h>
#include <picklist.h>
#include <convers.h>
#include <rt.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <memalloc.h>
#include <dataman.h>


#define	MAXSTRING	20
#define MAX_RECS	1000

XDLL int 
matchobj(int argc, rtobj *argv[])
{
	char			**matches;
	int			retval,numrecs,totrecs,i;
	byte 			otype = PCL;
	byte 			applno;
	picheader		*scrplist;
	picbody			pm;
	picbody			*saveptr;
	char			*names, *np;
	struct plwork pl;
	char			applname[MAXSTRING + 1],
					key[MAXSTRING + 1],
					valobject[MAXSTRING + 1],
					object_type[MAXSTRING + 1];
	field			*fp;
	
	if (argv[2]->obj.object != FLD || argv[2]->ptr == NULL)
		return(CSR_FAIL_ARGTYPE);

	if (argv[3]->obj.object != PIC || argv[3]->ptr == NULL)
		return(CSR_FAIL_ARGTYPE);

	if (rtobj2str(argv[0], applname, sizeof(applname)) != RT_SUCCESS ||
		rtobj2str(argv[1], key, sizeof(key)) != RT_SUCCESS ||
		rtobj2str(argv[2], valobject, sizeof(valobject)) != RT_SUCCESS)
		return (CSR_FAIL_ARGTYPE);

	// allow an optional object type to be passed as the
	// last parameter
	if ( argc == 5 ){
		// argument 5 can be a fld or a dsc or a text string
		// but it must be an integer value
		if (rtobj2str(argv[4],object_type,sizeof(object_type)) == RT_ERROR)	{
			return(CSR_FAIL_ARGTYPE);
		}
		if (strcmp("APL",object_type) == 0)	otype = APL;
		if (strcmp("BIT",object_type) == 0)	otype = BIT;
		if (strcmp("DOC",object_type) == 0)	otype = DOC;
		if (strcmp("DSC",object_type) == 0)	otype = DSC;
		if (strcmp("FLD",object_type) == 0)	otype = FLD;
		if (strcmp("ICN",object_type) == 0)	otype = ICN;
		if (strcmp("LST",object_type) == 0)	otype = LST;
		if (strcmp("MNB",object_type) == 0)	otype = MNB;
		if (strcmp("MNU",object_type) == 0)	otype = MNU;
		if (strcmp("PAT",object_type) == 0)	otype = PAT;
		if (strcmp("PCL",object_type) == 0)	otype = PCL;
		if (strcmp("OFB",object_type) == 0)	otype = OFB;
		if (strcmp("PIC",object_type) == 0)	otype = PIC;
		if (strcmp("PLB",object_type) == 0)	otype = PLB;
		if (strcmp("PMB",object_type) == 0)	otype = PMB;
		if (strcmp("PMU",object_type) == 0)	otype = PMU;
		if (strcmp("SCR",object_type) == 0)	otype = SCR;
		if (strcmp("TAG",object_type) == 0)	otype = TAG;
		if (strcmp("UDO",object_type) == 0)	otype = UDO;
		if (strcmp("CSR",object_type) == 0)	otype = CSR;
		if (strcmp("WMU",object_type) == 0)	otype = WMU;
		if (strcmp("PDM",object_type) == 0)	otype = PDM;
		if (strcmp("MLD",object_type) == 0)	otype = MLD;
	}	

	fp = (field *)argv[2]->ptr;	/* get field to stuff selection later*/
	scrplist = (picheader *)argv[3]->ptr;
	saveptr=scrplist->list.ptr;
	scrplist->list.ptr = &pm;

	applno = ddapplindex(RTddh,applname);
	numrecs = totrecs = 0;
	names = mem_alloc(MAX_RECS * 22);
	np = names;
	numrecs = ddgetlist(RTddh,otype, applno, LISTINIT, key, &matches);
	while (numrecs > 0) {
		if ((totrecs + numrecs) > MAX_RECS)
			numrecs = ((totrecs + numrecs) - MAX_RECS);
		totrecs += numrecs;
		for (i=0; i<numrecs;np+=22,i++) {
			if (strcmp(matches[i],valobject) == 0) {
				ddgetlist(RTddh,otype, applno, LISTSTOP, (char *)0, (char ***)0);
				mem_free(names);
				scrplist->list.ptr=saveptr;
				return(1);  /* Don't display picklist if match found with
						       valobject */
				}
			memcpy(np,matches[i],21);
			}
		if (totrecs >= MAX_RECS) {
			break;
			}
		numrecs = ddgetlist(RTddh,otype, applno, LISTCONT, key, &matches);
		}
	if ((numrecs < 0) || (totrecs == 0)) {
		ddgetlist(RTddh,otype, applno, LISTSTOP, (char *)0, (char ***)0);
		mem_free(names);
		scrplist->list.ptr=saveptr;
		return(2);
		}
	numrecs = totrecs;
	np = names;
	pl.pickitem = (char **)alloca(sizeof(char *) * (numrecs + 1));
	for (i=0; i<numrecs; np+=22,i++)
		pl.pickitem[i] = np;
	pm.numitems = numrecs;
	ddgetlist(RTddh,otype, applno, LISTSTOP, (char *)0, (char ***)0);

	pickexitkeys[0] = 0;
	unshow_fyi();
	pl.picklist = scrplist;
	pl.plscreen = PCLfields;
	pl.foldflag = FALSE;
	retval = show_picklist(&pl,TRUE);
	scrplist->list.ptr=saveptr;

	if (retval >= 0) {
		if (str2field(pl.pickitem[retval],fp) == RT_ERROR) {
			mem_free(names);
			return (CSR_FAIL_ARGTYPE);
			}
		refreshfield(0,0,fp);
		mem_free(names);
		return 0;
		}
	else {
		mem_free(names);
		return retval;
		}
}

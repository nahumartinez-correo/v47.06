// ===============================================================
//     XXX      X                 X
//      XX     XX                XX
//      XX    XXXXX             XXXXX   XX XXX    XXXXX    XX XXX
//   XXXXX     XX                XX      XXX XX       X    XXX XX
//  XX  XX     XX                XX      XX      XXXXXX    XX  XX
//  XX  XX     XX XX             XX XX   XX      X   XX    XX  XX
//   XXXX X     XXX    XXXXX      XXX   XXXX     XXXXX X   XX  XX
// ===============================================================
//
// DT_TRAN  --  Desktop 'transaction stack' manager
//
// $Name: R1V4C13 $
// $Revision: 1.13 $
// $Date: 1999/03/11 08:29:09 $
//
// Synopsis:
//      This module contains routines which manage the DeskTop's
//      transaction stack.  The selected transaction is actually
//      invoked by some OFB code in a module called:
//
//               OFB:drv:driver_main
//
// External Function List:
//
// Notes:
//
//  (C) Copyright 1989, ISC Systems Corp.  All rights reserved.
//  (C) Copyright 1994, Olivetti N.A.  All rights reserved.
//
//  Author: R.F. GEER
//  Modified: H.M.Lewis, Dec-94 -- added comments & error checks
//
// ===============================================================

#include "pch.h"

#include <typedefs.h>
#include <convers.h>
#include <defines.h>
#include <dsplist.h>
#include <rtdef.h>
#include <csrdef.h>
#include <rtprotos.h>
#include <memalloc.h>
#include <misc.h>
#include <msgrtn.h>
#include <csr.h>

#define SSIZE	30

	static byte		appno[SSIZE];			// Application number
	static index	itemno[SSIZE];			// Item number
	static field	**fld_stack[SSIZE];		// ==> current TRAN's fields_list
	static int		fld_index[SSIZE];		// == cur TRAN's fields_list_index
	static RECT		pclcursor[SSIZE];
	static field	*currentfield[SSIZE];
	static int		cur_stack = -1;			// Index of last thing 'stacked'

	static int _dt_tran_init(byte ano, index ino);
	static int _dt_tran_start(byte ano, index ino);
	static int _dt_tran_restart(byte ano, index ino);
	static int _dt_tran_end(byte ano, index ino);
	static int _dt_tran_reset(void);
	static int _dt_tran_save_curfld(int argc, rtobj *argv[]);
	static int _dt_tran_debug(int argc, rtobj *argv[]);
	static int	dt_debug_fld(int argc, rtobj *argv[]);
	static int	dt_debug_stack(int argc, rtobj *argv[], int type);
	extern int	sadebug (int argc, rtobj *argv[]);



// ===========================================
//  First arg must be an integer (the 'type')
// ===========================================
XDLL int 
dt_tran(int argc, rtobj *argv[])
{
	int		type;
	int		rval;
	field	*fp;
	byte	ano;
	index	ino;

	if (argc < 1) {
		mre(-1,"CSR:dt_tran: Argument Count Error");
		return(-1);
		}

	if (rtobj2int(argv[0],&type) != RT_SUCCESS) {
		mre(-1,"CSR:dt_tran: 1st arg must be an integer");
		return(CSR_FAIL_ARGTYPE);
		}

	if (type < DT_TRAN_SAVE_CURFLD && argc < 2) {
		mre(-1,"CSR:dt_tran: Missing 2nd argument");
		return(-1);
		}

	// ------------------------------------------------------
	// Now verify that the 2nd arg is legitimate for those
	// functions which require it to be a non-empty IND field
	// ------------------------------------------------------
	switch (type) {
		case DT_TRAN_INIT:
		case DT_TRAN_START:
		case DT_TRAN_RESTART:
		case DT_TRAN_END:
				fp = argv[1]->ptr;
				if (fp == NULL) {
					mre(-1,"CSR:dt_tran: Arg #2 must be an IND field");
					return(RT_ERROR);
					}
				if (fp->flags.type != FT_IND) {
					mre(-1,"CSR:dt_tran: Arg #2 must be an IND field");
					return(RT_ERROR);
					}
				ano = fp->storage[1];
				ino = (fp->storage[2] << 8) | fp->storage[3];
						if (ano == 0 || ino == 0) {
							mre(-1,"CSR:dt_tran: Empty IND field (arg #2)");
							return(RT_ERROR);
							}
				break;
		}

	// ---------------------------------
	// Invoke the appropriate subroutine
	// ---------------------------------
	switch (type) {
		case DT_TRAN_INIT:
							rval = _dt_tran_init(ano,ino);
							break;
		case DT_TRAN_START:
							rval = _dt_tran_start(ano,ino);
							break;
		case DT_TRAN_RESTART:
							rval = _dt_tran_restart(ano,ino);
							break;
		case DT_TRAN_END:
							rval = _dt_tran_end(ano,ino);
							break;
		case DT_TRAN_RESET:
							rval = _dt_tran_reset();
							break;
		case DT_TRAN_SAVE_CURFLD:
							rval = _dt_tran_save_curfld(argc,argv);
							break;
		case 99:
							rval = _dt_tran_debug(argc,argv);
							break;
		default:
							rval = RT_ERROR;
							break;
		}
	return(rval);
}




// ==========================================
//  Clear the stack fields and sets the BASE
// ==========================================
static int 
_dt_tran_init(byte ano, index ino)
{
	int		i;

   	for (i = 0; i < SSIZE; i++) {
		// ------------------------------------------------
		// Clear most everything associated with this entry
		// ------------------------------------------------
		appno[i]  = 0;
		itemno[i] = 0;
		if (cur_stack >= 0 && fld_stack[i] != 0 && i > 0)
			mem_free(fld_stack[i]);		// only if not first entry
		fld_stack[i]    = 0;
		fld_index[i]    = 0;
		currentfield[i] = 0;
		}
	// --------------------------------------------------------
	// Now set the first entry to point to the LIST pointed to
	// by the INDIRECT field which was our 2nd argument
	// --------------------------------------------------------
	appno[0]     = ano;
	itemno[0]    = ino;
	fld_stack[0] = Tran_current->fields_list;
	fld_index[0] = Tran_current->fields_list_index;
	cur_stack    = 0;
	return(RT_SUCCESS);
}



// =====================================
//  Set the current DeskTop transaction
// =====================================
static int 
_dt_tran_start(byte app, index item)
{
	int		i;

	// -----------------------------------------------------------
	// Arg #1 is an INDIRECT field pointing to a LST or OFB object
	// -----------------------------------------------------------
	if (app == appno[0] && item == itemno[0]) {
		// -----------------
		// Stack is empty...
		// -----------------
		cur_stack = 0;
		for (i = 0; Tran_current->fields_list[i] != (field *) 0; i++)
			Tran_current->fields_list[i]->flags.fieldlisted = 0;
		Tran_current->fields_list[0]    = NULL;
		Tran_current->fields_list_index = 0;
		return(RT_SUCCESS);
		}
	// ---------------------------------------
	// Stack not empty, so look for empty slot
	// ---------------------------------------
	for (i = 1; i < SSIZE; i++) {
		if (fld_stack[i] == 0) {
			// -----------------------------------
			// Found empty slot, so allocate it...
			// -----------------------------------
			fld_index[cur_stack] = Tran_current->fields_list_index;
			cur_stack            = i;
			appno[i]             = app;
			itemno[i]            = item;
#ifdef DEBUGCODEWANTED
if (Debug) {
  printf("dt_tran_start():cur_stack = %d, appno[i] = %d, itemno[i]= %d\n",
	   cur_stack, appno[i], itemno[i]);
}
#endif
			Tran_current->fields_list=mem_alloc((Max_fields+1)*sizeof(field*));
			Tran_current->fields_list[0]    = NULL;
			Tran_current->fields_list_index = 0;
			fld_stack[i]                    = Tran_current->fields_list;
			fld_index[i]                    = Tran_current->fields_list_index;
			return(RT_SUCCESS);
			}
		}
	mre(-1,"CSR:dt_tran: Stack Overflow");
	return(RT_ERROR);
}



// =======================================
//  Reset the current DeskTop transaction
// =======================================
static int 
_dt_tran_restart(byte app, index item)
{
	int		i;

	for (i = 0; i < SSIZE; i++) {
		if (app == appno[i] && item == itemno[i]) {
			cur_stack = i;
			Tran_current->fields_list = fld_stack[i];
			Tran_current->fields_list_index = fld_index[i];
			for (i = 0; Tran_current->fields_list[i] != (field *) 0; i++)
				Tran_current->fields_list[i]->flags.fieldlisted = 1;
			Tran_current->fields_list_index = i;
			Currentfield = currentfield[cur_stack];
			memcpy(&PCLcursor,&pclcursor[cur_stack],sizeof(RECT));
			return(RT_SUCCESS);
			}
		}
	mre(-1,"CSR:dt_tran: Restart Tran Failed");
	return(RT_ERROR);
}



// ================================================================
//  End (and erase from the stack) the current DeskTop transaction
// ================================================================
static int 
_dt_tran_end(byte app, index item)
{
	int		i;

	for (i = 0; i < SSIZE; i++) {
		if (app == appno[i] && item == itemno[i]) {
			// ------------------------------
			// Found the one we want to 'end'
			// ------------------------------
#ifdef DEBUGCODEWANTED
if (Debug) {
  printf("dt_tran_end():cur_stack = %d, appno[i] = %d, itemno[i] = %d\n",
		 cur_stack, appno[i], itemno[i]);
}
#endif
			Tran_current->fields_list = fld_stack[i];
			if (i != 0) {
				// ----------------------------------
				// Don't do this for the 'first' tran
				// ----------------------------------
				sav_fld_list(Tran_current->fields_list);
				clr_fld_list(Tran_current->fields_list);
				appno[i] = 0;
				itemno[i] = 0;
				mem_free(fld_stack[i]);
				fld_stack[i] = 0;
				fld_index[i] = 0;
				currentfield[i] = 0;
				}
			// ----------------
			// Set back to BASE
			// ----------------
			cur_stack = 0;
			Tran_current->fields_list = fld_stack[0];
			for (i = 0; Tran_current->fields_list[i] != (field *) 0; i++)
				Tran_current->fields_list[i]->flags.fieldlisted = 0;
			Tran_current->fields_list[0] = NULL;
			Tran_current->fields_list_index = 0;
			return(RT_SUCCESS);
			}
		}
	mre(-1,"CSR:dt_tran: TRAN_END on non-existant Tran");
	return(RT_ERROR);
}



// ====================================
//  Clear the entire transaction stack
// ====================================
static int 
_dt_tran_reset()
{
	int i,j;

	for (j = 1; j < SSIZE; j++) {
		if (fld_stack[j] != 0) {
			// ---------------------------------
			// Non-empty element, so clear it...
			// ---------------------------------
			Tran_current->fields_list = fld_stack[j];
			clr_fld_list(Tran_current->fields_list);
			for (i = 0; Tran_current->fields_list[i] != (field *) 0; i++)
				Tran_current->fields_list[i]->flags.fieldlisted = 0;
			Tran_current->fields_list[0] = NULL;
			Tran_current->fields_list_index = 0;
			mem_free(fld_stack[j]);
			appno[j] = 0;
			itemno[j] = 0;
			fld_stack[j] = 0;
			fld_index[j] = 0;
			currentfield[j] = 0;
			}
		}
	cur_stack = 0;
	Tran_current->fields_list = fld_stack[0];
	for (i = 0; Tran_current->fields_list[i] != (field *) 0; i++)
		Tran_current->fields_list[i]->flags.fieldlisted = 0;
	Tran_current->fields_list[0] = NULL;
	Tran_current->fields_list_index = 0;
	return(RT_SUCCESS);
}



// ===============================================================
//  Save info about the CURRENTFIELD in the current stack element
// ===============================================================
static int 
_dt_tran_save_curfld(int argc, rtobj *argv[])
{
	currentfield[cur_stack] = Currentfield;
	memcpy(&pclcursor[cur_stack],&PCLcursor,sizeof(RECT));
	return(RT_SUCCESS);
}


// =========================================
//  Debugging routine, various functions...
// =========================================
static int 
_dt_tran_debug(int argc, rtobj *argv[])
{
	int type;

	if (rtobj2int(argv[1],&type) != RT_SUCCESS)
		return(RT_ERROR);
	
	if (type == DT_FLD) {
		dt_debug_fld(argc, argv);
		return(RT_SUCCESS);
		}
	
	if (type == DT_STACK || type == 103) {
		dt_debug_stack(argc, argv, type);
		return(RT_SUCCESS);
		}
	
	if (type == DT_PIC) {
		sadebug(argc, argv);
		return(RT_SUCCESS);
		}
	
	return(RT_SUCCESS);
}



// =================================================================
//  Put DD name of object pointed to by 3rd argument (an IND field)
//  into the ASCII field identified by the 4th argument.
// =================================================================
static int
dt_debug_fld(int argc, rtobj *argv[])
{
	field	*fp;
	char	*ptr;
	char	val[28];
	byte	app;
	index	itm;

	if (argc < 4) {
		mre(-1,"CSR:dt_tran Debug field: requires at least 4 arguments");
		return(CSR_FAIL_ARGCOUNT);
		}
	fp = argv[2]->ptr;
	if (fp->flags.type != FT_IND) {
		mre(-1,"CSR:dt_tran Debug field: requires 2nd arg to be IND");
		return(CSR_FAIL_ARGCOUNT);
		}
	app = fp->storage[1];
	if (app == 0) {
		val[0] = 0;
		}
	else {
		itm = (fp->storage[2] << 8) | fp->storage[3];
		ptr = (char *)ddname(RTddh,fp->storage[0],app,itm);
		strcpy(&val[0],ptr);
		}

	fp = argv[3]->ptr;
	str2field(val,fp);
	return(RT_SUCCESS);
}



// ====================================================
//  Dump the entire transaction stack for debugging...
// ====================================================
static int
dt_debug_stack(int argc, rtobj *argv[], int type)
{
 	FILE	*fc_hdl = ((FILE *)0);
	field	*fp, **flp;
	int		i,k;
	char	*ptr;
	char	val[128];
	char	line[128];
	byte	app;
	index	item;
	char	filename[256];

	sprintf(filename, "%sdebug_int", tempdir());
	fc_hdl = fopen(filename,"w");
	if (fc_hdl == (FILE *) 0 )
		return(-103);

	sprintf(&line[0],"Current Tran = %d\n",cur_stack);
	if (fputs(line,fc_hdl) == EOF)
		return(-100);
	
	sprintf(&line[0],"     BASE APP = %d\n",appno[0]);
	if (fputs(line,fc_hdl) == EOF)
		return(-100);
	
	for (i = 0; i < SSIZE; i++) {
		if (appno[i] > 0) {
			flp = fld_stack[i];
			k = 0;
			while ((fp = *flp++) != NULL)
				k++;
			ptr = (char *)ddname(RTddh,LST,appno[i],itemno[i]);
			strcpy (&val[0],ptr);
			sprintf(&line[0],"   %02d %s  %d\n",i,&val[0],k);
			if (fputs(line,fc_hdl) == EOF)
				return(-100);
			if (currentfield[i] != 0) {
					fp   = currentfield[i];
					app  = fp->ddref.applno;
					item = fp->ddref.itemno;
					ptr  = (char *)ddname(RTddh,FLD,app,item);
					strcpy (&val[0],ptr);
					sprintf(&line[0],"        Current Field = %s\n",&val[0]);
				}
			else
				sprintf(&line[0],"        Current Field = NULL\n");
			if (fputs(line,fc_hdl) == EOF)
				return(-100);
			if (type == 103) {
				flp = fld_stack[i];
				while ((fp = *flp++) != NULL) {
					app  = fp->ddref.applno;
					item = fp->ddref.itemno;
					ptr  = (char *)ddname(RTddh,FLD,app,item);
					strcpy(&val[0],ptr);
					sprintf(&line[0],"        %s\n",&val[0]);
					if (fputs(line,fc_hdl) == EOF)
						return(-100);
					}
				}
			}
		else {
			sprintf(&line[0],"   %02d\n",i);
			if (fputs(line,fc_hdl) == EOF)
				return(-100);
			}
		}
	fclose(fc_hdl);
	return(RT_SUCCESS);
}

/*$
** MODULENAME - proclist.c
**
** $Name: R1V4C13 $
** $Revision: 1.41 $
** $Date: 1999/03/11 08:28:49 $
**
** Synopsis:
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
#include <defines.h>
#include <rtdef.h>
#include <rtprotos.h>
#include <convers.h>
#include <dataman.h>
#include <ofbwords.h>
#include <errdefs.h>
#include <ofbdefs.h>		/* PCL/C defines						*/
#include <clconn.h>
#include <cdsfuncs.h>
#include <cmfield.h>

#include <tdspl.h>
#include <scrlist.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <pclwords.h>
#include <dpwords.h>
#include <rtable.h>
#include <textedit.h>
#include <msgrtn.h>
#include <csr.h>

#define	COPY_TO	1
#define	COPY_FROM 2
#define COMPARE_DUP	3

extern		HANDLE hRTdll;
#include "cdscsr.h"
static bool mangle_field(register dlist *whichlist, 
									register field *fp, byte token);
static int  myrtobj2str(rtobj *p, char *buffer, int size);
static void	swapdup(field *fp);
static short init_arg(rtobj *arg, void **ptr1, void **ptr2);
static int  get_field_arg(byte type, rtobj *rp, void **ptr1, void **ptr2,
					 int *memnum, int *elemnum);
byte *_get_mem(register short i, register byte *bp);
static		complist *rtobj2lst(rtobj *p);
static int	compare_field_dup(field *fp);
static int	process_unary(int function, int argc, rtobj *argv[]);
static int	process_binary(int function, int argc, rtobj *argv[]);
static		do_unary_field(int function, int array_only, rtobj *ap,
			int row, int count);
static 		append_fields(int argc,rtobj *argv[]);
static		do_unary(int function, field *fp);
static		do_binary_field(int function, rtobj *sap, rtobj *dap, int srow,
			int drow, int count);
static int	process_append(int argc,rtobj *argv[]);
static int	process_scr_name(int argc,rtobj *argv[]);
byte  * 	pclkeyword(register  int i);
int 		GetFieldObj(byte *Pos,byte *FiledAppl,index *FieldItem,int *row,int *col);
int			GetArraySubscripts(byte *Array,int *row,int *col);
int			AppendScrFields(cdsselect *CDSselectPtr,byte *CodePointer,int ExceptCnt,byte *ExceptPtr);
int 		ScrDecompileAppend(cdsselect *Tptr,byte *buffer,int count,byte *Ptr);
int 		ScrDecompile(byte *buffer,rtobj *fp,int Size,int NumCol);
int			DupCurrentScrFields(int operation);
int			DupScrFields(int operation, rtobj *ScrObj);
int 		list_append(cdsselect *tptr,complist *lp,int FldIndex);
/*$
**  Function:  proc_list
**
**  Author:    Brian Gering            Summer 1992
**			   Gary Nance			   Winter 1994		
**  		   Nagesh Nookala		   Winter 1994 	
**  Purpose:  Perform functions against a list such as:
**
**              Binary functions (process_binary)
**                F_COPY      - Copy a set of fields to a second set of
**                              fields
**              Unary functions (process_unary)
**                F_CPTODUP   - Save the field to it's dup area, return 0
**                F_COMPARE   - Compare the field's dup and storage area,
**                              return 0 if equal, 1 if changed
**                F_SWAP      - Swap the dup and storage areas of a field
**                F_CPFROMDUP - Copy the field dup area to the storage
**                              area, return 0
**                F_CLEAR     - Clear a set of fields
**                F_CLEARARRAY - Clear a set of fields (array fields only)
**                F_REFRESH   - Refresh a set of fields
**                F_PROTECT   - Protect a set of fields
**                F_UNPROTECT - Unprotect a set of fields
**                F_EMPTY     - Check if all fields empty (TRUE or FALSE)
**                F_DATAPRESENT     - Check if all fields have data (TRUE or FALSE)
**		  F_APPEND    - Append the fields to the CDS buffer (get the fields list
**				if the element is a screen.)
**		  F_GETFIELDS_LIST - Takes the name of a screen and return all the field
**				names into an array passed.
**
**
**  Usage:    CALL CSR:PROCESS_LIST function (function specific parameters)
**
**  Returns:    function specific
**
*/
XDLL int
proc_list(int argc, rtobj *argv[])
{
	int type;

	if (argc < 1)
		return(CSR_FAIL_ARGCOUNT);
	if (rtobj2int(argv[0],&type) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	switch (type) {
		case F_CPTODUP:
		case F_COMPARE:
		case F_SWAP:
		case F_CPFROMDUP:
		case F_CLEAR:
		case F_CLEARARRAY:
		case F_REFRESH:
		case F_PROTECT:
		case F_UNPROTECT:
		case F_HIDEFIELD:
		case F_SHOWFIELD:
				return(process_unary(type, argc, argv));
		case F_EMPTY:
		case F_DATAPRESENT:
				if (process_unary(type, argc, argv)) return 0;
				else return 1;
		case F_COPY:
				return(process_binary(type, argc, argv));
		case F_APPEND:
				return(process_append(argc,argv));
		case F_GETFIELDS_LIST:
				return(process_scr_name(argc,argv));

		}
	return(CSR_FAIL_ARGTYPE);
}
/*eject*/
/*
**  process_unary - process unary operation against a list
**
**
**  Usage:    CALL CSR:PROCESS_LIST function list [row] [count]
**         where:
**            function  - function defined in proclist.h
**            list      - FLD or LST If a single field, process only
**                        that field. If a list, process all fields
**                        in the list.
**            row       - staring row of array to perform function
**                        (0 relative). default=0
**            count     - number of array elements to perform function
**                        on, default=all
**
**  RETURNS 0                - Success (all functions except F_COMPARE)
**                             Field not changed (function F_COMPARE)
**          1                - Field has changed (function F_COMPARE)
**         CSR_FAIL_ARGCOUNT - incorrect number of arguments
**         CSR_FAIL_ARGTYPE  - type of argument is incorrect
**         - (NOFIELD)       - can not find a field specified in
**                             list in data dictionary
**         -2                - not enough elements in array field
*/
static int
process_unary(int function, int argc, rtobj *argv[])
{
    short	type;
    int		memnum = 0, elemnum = 0;
    int		array_only = 0;
    int		row = -1, count = -1, retval, i;
    rtobj	ap;
    void	*vp1, *vp2;
    short	remember_cursor;
	byte 	ApplNo=0;
	index 	ItemNo=0;
			// push all the fields on CURRENTSCREEN to DUP area
	if ((function == F_CPTODUP)	&& (argc == 1))
		return (DupCurrentScrFields(COPY_TO));
			// DUP all the fields on CURRENTSCREEN 
	if ((function == F_CPFROMDUP)	&& (argc == 1))
		return (DupCurrentScrFields(COPY_FROM));
			// compare all the fields on CURRENTSCREEN with their DUP area
	if ((function == F_COMPARE)	&& (argc == 1))
		return (DupCurrentScrFields(COMPARE_DUP));

    if (argc < 2 || argc > 4)
		return(CSR_FAIL_ARGCOUNT);

    if (argc >= 3 && rtobj2int(argv[2],&row) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
    
    if (argc >= 4 && rtobj2int(argv[3],&count) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

    if (row < 0)
		row = 0;
    
    if (function == F_CLEARARRAY) {
		function = F_CLEAR;
		array_only = 1;
    	}
    
    /* If the first argument is a screen then copy all the fields to the dup area */
	
	if (argv[1]->obj.object == CSRARG_SCR && (function == F_CPTODUP 
				|| function == F_CPFROMDUP ||	function == F_COMPARE))	{
		if (function == F_CPTODUP)
			return (DupScrFields(COPY_TO,argv[1]));
		if (function == F_CPFROMDUP)
			return (DupScrFields(COPY_FROM,argv[1]));
		if (function == F_COMPARE)
			return (DupScrFields(COMPARE_DUP,argv[1]));
		}
    if ((type = init_arg(argv[1], &vp1, &vp2)) < 0)
		return(CSR_FAIL_ARGTYPE);
    if (type == CSRARG_SCR && function == F_REFRESH) {
		remember_cursor = cursor_on;
		hide_cursor();
		dl_refresh(PCLfields,NULL);
		if (remember_cursor)
			show_cursor(0,0); /* redraw cursor if necessary */
		return(1);
    	}
	retval = 0;
    while(! retval)
		if ((i = get_field_arg((byte)type, &ap, &vp1, &vp2, &memnum, &elemnum)) < 0)
	    	retval = i;
		else if (i > 0)
	    	break;
		else if (ap.obj.object == PCLsub || ap.obj.object == CSRARG_INT)
	    	continue;
		else if (ap.obj.object == CSRARG_FLD || ap.obj.object == TYPE_ARRAY)	{
		    retval = do_unary_field(function, array_only, &ap, row, count);		 	
			}
		else
	    	retval = -2;
    	return(retval);
}


/*
**  process_append - Appends a lits of fields to the CDS buffer passed.
**	The list should be in the following format.
**			MEMBER cds_field,["ODBC field name",RT_FIELD1,RT_FIELD2,..]
**	The function takes a parameter which will tell make it append RT_FIELD1
**	or RT_FIELD2 etc.. to be appended as CDS_FIELD based on whether the 
**	index is 2,3,..
**	If the the number of elements is less than the index passed, then the 
**	CDS_FIELD will be appended as itself.
**	The LIST can have members which are also LISTs following the same format.
**	In this case, this function will process that list also.
**	The first member of the list can be an OFB procedure. If it is then the 
**	OFB procedure will get executed and it the return value is negative, the 
**	LIST will not be processed further.
**
**  Usage:    CALL CSR:PROCESS_LIST function list [row] [count]
**         where:
**            function  - function defined in proclist.h
**            context   - The CDS buffer context name
**            list      - FLD or LST If a single field, process only
**                        that field. If a list, process all fields
**                        in the list and if the element is a screen 
**						  process all the fields on the screen.
**						
**  RETURNS 0                - Success
**         CSR_FAIL_ARGCOUNT - incorrect number of arguments
**         CSR_FAIL_ARGTYPE  - type of argument is incorrect
**         - (NOFIELD)       - can not find a field specified in
**                             list in data dictionary
**         -2                - not enough elements in array field
*/

static int
process_append(int argc, rtobj *argv[])
{
	complist *lp;
	field	*fp;
	char	buf[256];
	cdsselect *tptr;
	int		FldIndex;
	index	IndexNo;
	if (argc != 4)
		return(CSR_FAIL_ARGCOUNT);
	if (rtobj2str(argv[1],buf,sizeof(buf)) == RT_ERROR)	{
		mre(-1,"proc_list: BAD CONTEXT NAME PASSED");
		return(RT_ERROR);
		}
	if ((tptr = find_clcontext (CdsEnv, buf)) == NULL)	{
		mre(-1,"proc_list: BAD CONTEXT NAME PASSED");					 
		return (RT_ERROR);
		}
	if (argv[2]->obj.object != LST && argv[2]->obj.object != FLD &&
		argv[2]->obj.object !=  TYPE_ARRAY)	{
		mre(-1,"proc_list: ARGUMENT 2 should be a LIST or FLD");
		return (CSR_FAIL_ARGTYPE);
		}
	/* If the parameter is a FLD make sure that it is pointing to a LIST */
	if (argv[2]->obj.object == FLD)	{
		fp = argv[2]->ptr;
		if (fp->flags.type == FT_IND)	{
			IndexNo = (fp->storage[2] << 8) + fp->storage[3];
			if ((lp = (complist *)ddget(RTddh,*(fp->storage),fp->storage[1]
							,IndexNo)) == 0)	{
				mre(-1,"proc_list: INDIRECT FIELD SHOULD POINT TO A LIST");
				return(CSR_FAIL_ARGTYPE);
				}
			}
		else	{
			mre(-1,"proc_list: ARG 2 SHOULD BE A LIST OR IND FIELD");
			return (CSR_FAIL_ARGTYPE);
			}
		}
	else { // Get a pointer to the list
		if ((lp = (complist *)ddget(RTddh,argv[2]->obj.object,
						argv[2]->obj.applno,argv[2]->obj.itemno)) == 0)	{
			mre(-1,"proc_list: ERROR IN GETTING THE LIST");
			return (CSR_FAIL_ARGTYPE);
			}
		}
	if (rtobj2int(argv[3],&FldIndex) == RT_ERROR)	{
		mre(-1,"proc_list: The Index should be specifeld");
		return(RT_ERROR);
		}
	else if (FldIndex == 1)	{
			mre(-1,"The index can not be 1");
			return (-2);
			}
	return (list_append(tptr,lp,FldIndex));
}


static int
list_append(cdsselect *tptr,complist *lp, int FldIndex)
{
	byte *CodePointer;
	byte *GenericCode;
	byte *MemberPtr,*ElementPtr;
	complist	*list;
	field	*fp;
	field	*fp1;
	int 	MemberCnt,ElementCnt,MemberSize,FirstMember=0;
	int		CurrentElement=0;
	int		SaveNo;
	int		SaveIndex;
	int		ObjType;
	short	CurRow,CurCol;
	long 	ArraySize;
	int		Rows;
	int 	Cols;
	int 	Total;
	byte	ApplNo;
	index	ItemNo;
	short	ms;

	MemberPtr = (byte *) (lp + 1);
	MemberCnt = lp->numitems;
	FirstMember = 1;
	while (MemberCnt--)	{
		// Skip over the MEMBER token, element Count and Member size
		ElementPtr = MemberPtr + 4;
		ElementCnt = (int)(*(MemberPtr+1));
		ObjType = *ElementPtr;
		//----------------------------------------------------------------//
		//-----Check to see if the first member is an OFB proc or LIST---//
		//---------------------------------------------------------------//
		switch (ObjType)	{
			case OFB:
				if (FirstMember != 1)	{
					mre(-1,"proc_list:OFB SUB should be the first element");
					return (-2);
					}
				else	{
					ElementPtr++;
					ApplNo = *ElementPtr++;
					ItemNo = *ElementPtr++ << 8;
					ItemNo |= *ElementPtr++;
					if (pcl(0,PCL,ApplNo,ItemNo,0,NULL) != RT_SUCCESS)
						return 0;
					if (Retval < 0)
						return 0;
					}
				break;
			case LST:
				ElementPtr++;
				ApplNo = *ElementPtr++;
				ItemNo = *ElementPtr++ << 8;
				ItemNo |= *ElementPtr++;
				if ((list = (complist *)ddget(RTddh,(byte)LST,ApplNo,ItemNo)) 
							== 0)	{
					mre(-1,"proc_list: ERROR IN GETTING THE SUB-LIST");
					}
				else
					list_append(tptr,list,FldIndex);
				break;
			case SCR:
   				ElementPtr++;
			   	ApplNo = *ElementPtr++;
				ItemNo = *ElementPtr++ << 8;
				ItemNo |= *ElementPtr++;
				//	ElementPtr += tsize (ElementPtr);
				GenericCode = ddget(RTddh,(byte) SCR,ApplNo,ItemNo);	 
				CodePointer = GenericCode + sizeof(struct SCRDEF);
			    Total=0;
				Total = ScrDecompileAppend(tptr,CodePointer,ElementCnt,ElementPtr);
				if (Total < 0)
					return -2;
				break;
			case FLD:
			case TYPE_ARRAY:
				fp = rt_get_fp(ElementPtr,&ms,NULL);
				if (fp)	{
					if ((fp->flags.array) && (ObjType != TYPE_ARRAY))	{	
			//--------------------------------------------------------------//
			// Append all if an array field name without subscript is passed
			//--------------------------------------------------------------//
						ElementPtr += ms - 3;
						ApplNo = *ElementPtr++;
						ItemNo = *ElementPtr++ << 8;
						ItemNo |= *ElementPtr++;
						ArraySize = ddarraysize(RTddh,ApplNo,ItemNo);
						if (ArraySize != -1)	{
							Cols = ArraySize & 0x00ff;
							Rows = (ArraySize >> 16) & 0x00ff;
							if (Rows == 0)
								CurRow = -1;
							CurCol = 0;
							while (CurCol < Cols && CurRow < Rows)	{
								fp = (field *) ddaget(RTddh,ApplNo,ItemNo,CurRow,CurCol);
								if (fp)	{
									if (rtaddfield(tptr,fp,ApplNo,
											ItemNo,fp) == RT_ERROR)	{
										mre(-1,"proc_list:Error in appending the field");
										return (-2);
										}
									}
								if (CurCol >= (Cols - 1)) {
									CurRow++;
									CurCol = 0;
									}
								else
									CurCol++;
								}
							}
						}
					else	{
						ElementPtr += ms - 3;
						ApplNo = *ElementPtr++;
						ItemNo = *ElementPtr++ << 8;
						ItemNo |= *ElementPtr++;
						fp1 = fp;
						SaveNo = 1;
						SaveIndex = FldIndex;
						if (ElementCnt > 1)	{
							fp1 = rt_get_fp(ElementPtr,&ms,NULL);
							if (!fp1)	{
								fp1 = fp;		// incase we don't find element
								ElementPtr += tsize(ElementPtr); // skip odbc name
								SaveNo = 2;
								}
							else
								SaveIndex = FldIndex - 1;
							}
						for (CurrentElement=SaveNo;CurrentElement<ElementCnt;
							 	CurrentElement++)	{
						    if (CurrentElement == SaveIndex)	{
								fp1 = rt_get_fp(ElementPtr,&ms,NULL);
								if (!fp1)	{
									fp1 = fp;
									break;
									}
								}
							else	{
								ElementPtr += tsize(ElementPtr);
								}
							}
						if (rtaddfield(tptr,fp1,ApplNo,
							ItemNo,fp) == RT_ERROR)	{
							mre(-1,"proc_list:Error in appending the field");
							return (-2);
							}
						}
				 	}		// end of IF
			  	break;
			}				// End if switch
		MemberSize = (MemberPtr[2] << 8) + MemberPtr[3];
		MemberPtr += MemberSize + 4;
		FirstMember = 0;
		}
		return (RT_SUCCESS);
	}

/*
**  process_scr_name - Returns the list of fields into an array passed.
**	Purpose: This is used for Journaling
**
**  Usage:    CALL CSR:PROCESS_LIST function list [row] [count]
**         where:
**            function  - function defined in proclist.h
**            NAME      - SCREEN name to be processed
**			  FIELD     - array field that will get the fields names into.
**							(Should be ASCII)
**			 RETURNS 0       - Success (all functions except F_COMPARE)
**         CSR_FAIL_ARGCOUNT - incorrect number of arguments
**         CSR_FAIL_ARGTYPE  - type of argument is incorrect
**         - (NOFIELD)       - can not find a field specified in
**                             list in data dictionary
**         -2                - not enough elements in array field
*/
static int
process_scr_name(int argc, rtobj *argv[])
{
	arrayhdr	*fhdr;
	byte		*CodePointer;
	byte		*GenericCode;
	field		*fp;
	field 		*fp1;
	index		IndexNo;
	byte		ApplNo;
	int			dcols;
	int			drows;
	int			FieldSize;
	long		ArraySize;

    if (argc != 3)
		return(CSR_FAIL_ARGCOUNT);
	if ((argv[1]->obj.object != SCR && argv[1]->obj.object != FLD)) {
	  	mre(-1,"proc_list: second arg should be a SCR or IND FLD");
		return (CSR_FAIL_ARGTYPE);
		}
	fp1 = argv[2]->ptr;
	if (!fp1->flags.array) {										 
		mre(-1,"proc_list: third arg should be an ARRAY");
		return (CSR_FAIL_ARGTYPE);
		}
	
	/* If the parameter is a FLD make sure that it is pointing to a LIST */					  
	if (argv[1]->obj.object == FLD)	{
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND)	{
			IndexNo = (fp->storage[2] << 8) + fp->storage[3];
			if ((GenericCode = ddget(RTddh,(byte) SCR,fp->storage[1],
						IndexNo)) == 0)	{
				mre(-1,"proc_list: INDIRECT FIELD SHOULD POINT TO A SCR");
				return(CSR_FAIL_ARGTYPE);
				}		;	 
			CodePointer = GenericCode + sizeof(struct SCRDEF);
			}
		else	{
			mre(-1,"proc_list: ARG 1 SHOULD BE A SCR OR IND FIELD");
			return (CSR_FAIL_ARGTYPE);
			}
		}
	else { // Get a pointer to the list
	    fp = argv[1]->ptr;
		ApplNo = argv[1]->obj.applno;
		IndexNo = argv[1]->obj.itemno;
		if ((GenericCode = ddget(RTddh,(byte) SCR,ApplNo,
					IndexNo)) == 0)	{
			mre(-1,"proc_list: Error in Getting the Given SCREEN");
			return(CSR_FAIL_ARGTYPE);
			}		;	 
			CodePointer = GenericCode + sizeof(struct SCRDEF);
		}
	ArraySize = ddarraysize(RTddh, argv[2]->obj.applno, argv[2]->obj.itemno);
	dcols = (short)(ArraySize & 0x00ffff);
	drows = (short)((ArraySize>> 16) & 0x00ffff);
	if (drows != 0)	{
	     mre(-1,"proc_list is expecting only a single dimentional array fld");
		 return (-2);
		 }
	fhdr = (arrayhdr *)fp1 - 1;
	FieldSize = fhdr->sizeperfield;
	return (ScrDecompile(CodePointer,argv[2],FieldSize,dcols));
	}





static 
do_unary_field(int function, int array_only, rtobj *ap, int row, int count)
{
	int			k, retval = 0, arraysize;
	short		rows, cols;
	arrayhdr	*fhdr;
	field		*fp;

	if (ap->obj.object == TYPE_ARRAY) {
		arraysize = ddarraysize(RTddh, ap->obj.applno, ap->obj.itemno);
		cols = (short)(arraysize & 0x00ffff);
		rows = (short)((arraysize >> 16) & 0x00ffff);
		if (rows != 0)
		     arraysize = cols * rows;
		if (count == -1) {
			/* from starting row through end of array	*/
			count = arraysize - (row * cols);
			}
		if( rows != 0 ) {
			if( row + count > ((rows-row)*cols) )
				return( -2 );
			}
		else {
			if( row + count > cols )
				return( -2 );
			}
 		fp = (field *)(ap->ptr);
		fhdr = (arrayhdr *)fp - 1;
		if (rows == 0)
			fp = (field *)((char *)fp + (row * fhdr->sizeperfield));
		else
			fp = (field *)((char *)fp + ((row * cols) * fhdr->sizeperfield));
		for (k=0; k<count; k++) {
       		if ((retval = do_unary(function, fp)))
				break;
			fp = (field *)((char *)fp + fhdr->sizeperfield);
			}
		}
	else if (! array_only)
		retval = do_unary(function, ap->ptr);

	return(retval);
}


static 
do_unary(int function, field *fp)
{
	int retval = 0;

	switch(function) {
		case F_CLEAR:
			clr_fld_storage(fp);
			fp->flags.valdone     = 0;
			fp->flags.dataentered = 0;
			break;
		case F_REFRESH:
			refreshfield(0, 0, fp);
			break;
		case F_PROTECT:
			fp->flags.dyn_prot = 1;
			add_field_rec(fp);
			break;
		case F_UNPROTECT:
			fp->flags.dyn_prot = 0;
			break;
		case F_SWAP:
			swapdup(fp);
			add_field_rec(fp);
			break;
		case F_CPTODUP:
			sav_fld_storage(fp);
			add_field_rec(fp);
			break;
		case F_COMPARE:
			retval = compare_field_dup(fp);
			break;
		case F_CPFROMDUP:
			dup_fld_storage(fp);
			add_field_rec(fp);
			break;
		case F_DATAPRESENT:
			// Exit the loop one of the fields is blank
			if (fp->numchars == 0)
				retval = TRUE;
			break;
		case F_EMPTY:
			// exit the loop, one of the fields has data
			if (fp->numchars != 0)
				retval = TRUE;
			break;
		case F_HIDEFIELD:
			fp->flags.hidden = 1;
			mangle_field(PCLfields,fp,HIDEFIELD);
		break;
		case F_SHOWFIELD:
			fp->flags.hidden = 0;
			mangle_field(PCLfields,fp,SHOWFIELD);
		break;
	}
	return(retval);
}
/*eject*/
/*
** process_binary - process binary function against a list
**
**  Parms: function ==  F_COPY    Copy one set of fields to another
**
**  Usage:    CALL CSR:PROCESS_LIST F_COPY src_list dest_list [srcrow]
**                   [dstrow] [count]
**         where:
**            src_list  - compiled pcl list contianing source fields
**            dest_list - compiled pcl list contianing destination fields
**            srcrow    - staring row of array in source list
**                        (0 relative). default=0, -1 for optional
**            dstrow    - staring row of array in destination list
**                        (0 relative). default=0, -1 for optional
**            count     - number of array elements to perform function
**                        on, default=all
**
**  RETURNS 0                 - Success
**          - BADELEMENTCOUNT - number of fields in src_list not equal
**                                  to number in dest_list
**          CSR_FAIL_ARGCOUNT - incorrect number of arguments
**          CSR_FAIL_ARGTYPE  - type of argument is incorrect
**          - (NOFIELD)       - can not find a field specified in
**                              list in data dictionary
**          -2                - not enough elements in array field
*/
static int
process_binary(int function, int argc, rtobj *argv[])
{
	int			srow = -1, drow = -1, count = -1, retval, i;
	void	*svp1, *svp2, *dvp1, *dvp2;
	int		smemnum = 0, selemnum = 0, dmemnum = 0, delemnum = 0;
	short	stype, dtype;
	rtobj	sap, dap;

	if (argc < 3 || argc > 6)
		return(CSR_FAIL_ARGCOUNT);
	
	/* second argument has to be a list */
	if (argv[1]->obj.object != CSRARG_LST
			 || argv[2]->obj.object != CSRARG_LST
			 || function != F_COPY)
		return(CSR_FAIL_ARGTYPE);

	if (argc >= 4 && rtobj2int(argv[3],&srow) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	if (argc >= 5 && rtobj2int(argv[4],&drow) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	if (argc >= 6 && rtobj2int(argv[5],&count) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);

	if (drow == -1)
		drow = 0;
	if (srow == -1)
		srow = 0;
	
	if ((stype = init_arg(argv[1], &svp1, &svp2)) < 0)
		return(CSR_FAIL_ARGTYPE);

	if ((dtype = init_arg(argv[2], &dvp1, &dvp2)) < 0)
		return(CSR_FAIL_ARGTYPE);

	retval = 0;
	while(! retval) {
		if ((retval = get_field_arg((byte)stype, &sap, &svp1, &svp2, 
				&smemnum, &selemnum)) < 0)
			break;
		i = retval;
		if ((retval = get_field_arg((byte)dtype, &dap, &dvp1, &dvp2,
				&dmemnum, &delemnum)))
			break;
		if ((i && ! retval) || (! i && retval))
			retval = -BADELEMENTCOUNT;
		else if (i || retval)
			break;
		else if (sap.obj.object == PCLsub || dap.obj.object == PCLsub
				 || dap.obj.object == CSRARG_INT)
			continue;
		else if ((sap.obj.object == CSRARG_INT
				 || sap.obj.object == CSRARG_FLD
				 || sap.obj.object == TYPE_ARRAY) &&
				(dap.obj.object == CSRARG_FLD
				 || dap.obj.object == TYPE_ARRAY))
			retval = do_binary_field(function, &sap, &dap, srow, drow, count);
		else
			retval = -2;
		}
	return(retval);
}

static do_binary_field(int function, rtobj *sap, rtobj *dap, int srow,
	int drow, int count)
{
	byte	*pcl_ptr;
	field	*fp;
	int		k, numcol1,numcol2, numcol,as1, as2;
	int		scols=0,srows=0,drows=0, dcols=0,col=0,scol=0;
	int 	ApplSet=0;
	arrayhdr	*fhdr;
	long	l;
	byte pa[] = {	TYPE_ARRAY, 0xff, TYPE_ICON1, 0x00,
						PreFLD, 0x00, 0x00, 0x00};
	byte pa2[] = {	TYPE_ARRAY, 0xff, TYPE_ICON1, 0x00,
					TYPE_ARRAY, 0xff, TYPE_ICON1, 0x00,
						PreFLD, 0x00, 0x00, 0x00};

	as1 = as2 = -1;
	if (sap->obj.object == TYPE_ARRAY) {
	 	as1 = ddarraysize(RTddh, sap->obj.applno, sap->obj.itemno);
		scols = (short)(as1 & 0x00ffff);
		srows = (short)((as1 >> 16) & 0x00ffff);
		if (srows != 0)					// If 2-d array
		     as1 = scols * srows;
		if (count == -1)				// If no. of rows to copy is not given
			numcol1 = as1 - (srow * scols);
		else
	        numcol1 = count;
		if (srows > 0)	{
			if ((scols * srow + numcol1) > as1)	{  // If the elements to copy > size
				mre(-1,"Elements to copy overruns Source Size\n");
				return -2;
				}
			}
		else	{
			if ((srow + numcol1) > as1)	{  // If the elements to copy > size
				mre(-1,"Elements to copy overruns Source Size\n");
				return -2;
				}
			}

		if (srows > 0)	{				// If 2-D array
			pa2[3] = (byte)(srow);
			pa2[7] = (byte)(0);
			pcl_ptr = (byte *)pa2;
			}
		else	{ 						// If 1-D array
		    pa[3] = (byte)(srow);
			pcl_ptr = (byte *)pa;
			}
		}
	else if (sap->obj.object == CSRARG_FLD) { 
		// rjn, Integers used to fall thru here and
		// core dump, so only do this is its a field						
		numcol = 1;
		pcl_ptr = (byte *)(&pa[4]);
		fp = (field *)(sap->ptr);
		if (fp->flags.array) {			// If the field is array type
			ApplSet = 1;
			l = ddgetrowcol(RTddh, fp->ddref.applno, fp->ddref.itemno, fp);
		  	scols = (short)(l & 0x00ffff);
			srows = (short)((l >> 16) & 0x00ffff);
			if (srows >= 0)	{ 				// If 2-D array
				pa2[9] = fp->ddref.applno;  // Use this instead of sap->applno
				pa2[10] = ((fp->ddref.itemno) >> 8) & 0xff;
				pa2[11] = (fp->ddref.itemno) & 0xff;
				pa2[3] = (byte)(srows);
				pa2[7] = (byte)(scols);
				pcl_ptr = (byte *)pa2;
				}
			else	{ 
				pa[5] = fp->ddref.applno;
				pa[6] = ((fp->ddref.itemno) >> 8) & 0xff;
				pa[7] = (fp->ddref.itemno) & 0xff;
				if (scols)	{
					pcl_ptr =  (byte *)pa;
					pa[3] = (byte)scols;
					}
				}
			}
		}
	if (ApplSet == 0)	{
		if (srows > 0)	{
			pa2[9] = sap->obj.applno;
			pa2[10] = (sap->obj.itemno >> 8) & 0xff;
			pa2[11] = (sap->obj.itemno) & 0xff;
			}	
		else	{
			pa[5] = sap->obj.applno;
			pa[6] = ((sap->obj.itemno) >> 8) & 0xff;
			pa[7] = (sap->obj.itemno) & 0xff;
			}
		}
	if (dap->obj.object == TYPE_ARRAY) {
		as2 = ddarraysize(RTddh, dap->obj.applno, dap->obj.itemno);
		dcols = (short)(as2 & 0x00ffff);
		drows = (short)((as2>> 16) & 0x00ffff);
		if (drows != 0)
		     as2 = dcols * drows;
		if (count == -1) {
			/* from starting row through end of array	*/
			numcol2 = as2 - (drow * dcols);
			}
		else {
			 numcol2 = count;
			}
		if (drows > 0)	{
			if ((dcols * drow + numcol2) > as2)	{  // If the elements > size
				mre(-1,"Elements to copy overruns Destination Size\n");
				return -2;
				}
			}
		else	{
			if ((drow + numcol2) > as2)	{ // If the elements to copy > size
				mre(-1,"Elements to copy overruns Destination Size\n");
				return -2;
				}
			}
		if ((sap->obj.object == TYPE_ARRAY) && (numcol1 != numcol2))	{
			mre(-1,"The Array Sizes of Source and Destination do not match\n");
			return -2;
			}
		else
			numcol = numcol1;
		fhdr = (arrayhdr *)(dap->ptr) - 1;
		if (drows > 0)
			fp = (field *)((char *)dap->ptr + ((drow * dcols )* fhdr->sizeperfield));
		else
			fp = (field *)((char *)dap->ptr + (drow * fhdr->sizeperfield));
		}
	else	{
		fp = (field *)dap->ptr;
		}
   	col = 0;
	if (srows > 0 && count != -1)
		numcol = scols * numcol;
   	for (k= 0;k < numcol;k++) {
#ifdef DEBUGCODEWANTED
	if (Debug) {
		if (srows > 0) 
			printf("F_COPY:copying element row %d and col %d\n",srow,scol);
		else if (numcol > 1)
				printf("F_COPY:copying element %d of %d\n",k,numcol);
			else
				printf("F_COPY:copying one element\n");
		}
#endif
	   	if (sap->obj.object == CSRARG_INT)
			clr_fld_storage(fp);
		else if (pcl2field(pcl_ptr, fp,NULL) == RT_ERROR)
			return(- NOFIELD);

		add_field_rec(fp);
		if (dap->obj.object == TYPE_ARRAY)
			fp = (field *)((char *)fp + fhdr->sizeperfield);
		if (sap->obj.object == TYPE_ARRAY)  {
		    if (srows > 0) {
			    if ((k+1)/scols >col) {
					col++;
					srow++;
					}
				scol = k + 1 - scols * col;
				pa2[3] = srow;
				pa2[7] = scol;
				}
			else
			     pa[3] = k+1+srow;
		    }
		}
	return(0);
}

  
static int
myrtobj2str(rtobj *p, char *buffer, int size)
{
	field *fp;

	if (p->obj.object == FLD) {
		fp = (field *)(p->ptr);
		if (fp->flags.type == FT_IND)
			return(pcl2str(fp->storage, buffer, size, NULL));
		}
	return(rtobj2str(p, buffer, size));
}
static short init_arg(rtobj *arg, void **ptr1, void **ptr2)
{
	short	type;

	if (arg->obj.object == CSRARG_FLD) {
		/* single field	or array field */
		if (! arg->ptr)
			return(CSR_FAIL_ARGTYPE);
		*ptr1 = (void *)arg;
		type = CSRARG_FLD;
		}
	else if (arg->obj.object == CSRARG_LST) {
		/* list of fields */
		*ptr1 = (void *)(arg->ptr);
		type = CSRARG_LST;
		}
	else 
		type = -1;
	return(type);
}

static int 
get_field_arg(byte type, rtobj *rp, void **ptr1, void **ptr2,
			int *memnum, int *elemnum)
{
	int	i, j, retval;
	static void	*rtdrawTE;

	if (hRTdll == NULL) {
		hRTdll = LoadLibrary("rt.dll");
		if (!hRTdll)
			return(RT_ERROR);
	}
	
	if (rtdrawTE == NULL) {
		rtdrawTE = GetProcAddress(hRTdll,"drawTEfield");
		if (!rtdrawTE)
			return(RT_ERROR);
	}
	
	retval = 0;
	switch(type) {
		case CSRARG_FLD:
			if (*ptr1) {
				memcpy(rp, *((char **)(ptr1)), sizeof(rtobj));
				*ptr1 = 0;
			}
			else
				retval = 1;
			break;
		
		case CSRARG_LST:
			{
			byte		mem_cnt;
			short		ms;
			complist	*cl;
			byte		*mem_ptr;
			int k;
			field		*fp;

			i = *memnum;
			j = *elemnum;
			cl = *(complist **)ptr1;
			rp->obj.object = 0;
			
			for (; i < cl->numitems; i++) {
				mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
				mem_cnt = mem_ptr[1];
				mem_ptr += 4;
				for (k=0; k<j; k++)
					mem_ptr += tsize(mem_ptr);
				
				/* process each field in member p */
				if (j < mem_cnt) {
					if (*mem_ptr == PCLsub) {
						mem_ptr++;
						rp->obj.object = PCLsub;
						break;
						}
					else if (*mem_ptr == 0x81 && mem_ptr[1] == 0) {
						mem_ptr += 2;
						rp->obj.object = CSRARG_INT;
						break;
						}
					
					if (! (fp = rt_get_fp(mem_ptr,&ms,NULL)))
						return(-NOFIELD);
					
					rp->ptr = fp;
					if (*mem_ptr == TYPE_ARRAY) {
						rp->obj.applno = mem_ptr[5];
						rp->obj.itemno = (mem_ptr[6] << 8) | mem_ptr[7];
						rp->obj.object = CSRARG_FLD; 
						}
					else {
						rp->obj.applno = mem_ptr[1];
						rp->obj.itemno = (mem_ptr[2] << 8) | mem_ptr[3];
						if (*mem_ptr == PreFLD && fp->flags.array)
							rp->obj.object = TYPE_ARRAY;
						else
							rp->obj.object = CSRARG_FLD; 
						}
					break;
					
				} else
					j = 0;
				
			}
			
			if (i < cl->numitems) {
				*memnum = i;
				*elemnum = j + 1;
				}
			else
				retval = 1;
			}
			break;
		
		case CSRARG_SCR:
			{
			dlist	*flist, *baselist;
			itemidx	*idx;

			flist = *((dlist **)ptr1);
			baselist = *((dlist **)ptr2);
			
			if (flist) {
				if (flist->dspfunc == rtdrawTE) {
					struct RT_TEFIELD	*tr = flist->obj;
					rp->ptr = tr->fp;
				} 
				else
					rp->ptr = flist->obj;
				
				rp->obj.object = CSRARG_FLD;
				if (! (idx = ddgetitemidx(RTddh, FLD, rp->ptr)))
					retval = -NOFIELD;	/* failed to find item in index */
				else {
					rp->obj.applno = (idx->flapit & APPLNO) >> APPLSHIFT;
					rp->obj.itemno = idx->flapit & ITEMNO;
					flist = dl_getnext(baselist,flist);
					*ptr1 = (void *)flist;
				}
			}
			else
				retval = 1;
			}
			break;
		
		default:
			retval = 1;
		}
	return(retval);
}

void
swapdup(field *fp)
{
	byte	*dp, *ep, token;
	int		is;

	/* swap the storage & dup areas */
	dp = fp->storage;
	ep = flddupptr(fp);
	is = fldstosize(fp);    /* get actual byte size */
	while (is--) {
		token = *dp;
		*dp++ = *ep;
		*ep++ = token;
		}
	is = fp->numchars;
	fp->numchars = fp->dupnumchars;
	fp->dupnumchars = is;
	is = fp->flags.dataentered;
	fp->flags.dataentered = fp->flags.dup_dataentered;
	fp->flags.dup_dataentered = is;
}
static int
compare_field_dup(field *fp)
{
	int		size;

	/* always different if numchars has changed	*/
	if (fp->numchars != fp->dupnumchars) {
		return(1);
		}


	switch (fp->flags.type) {
		case FT_ASCII:
		case FT_NUMERIC:
		case FT_DATE:
		case FT_TIME:
			/* these types are null terminated	*/
			if (strcmp((char *)(fp->storage), (char *)flddupptr(fp))) {
				return(1);
				}
			break;
		case FT_INT:
		case FT_FLAGS:
		case FT_IND:
			/* these types are all 4-byte (32 bit) values	*/
			if (memcmp(fp->storage, flddupptr(fp), 4)) {
				return(1);
				}
			break;
		case FT_FIXED:
			/* use numchars for the amount of data entered, then add	*/
			/* 3 bytes of overhead for the fixed field header			*/
			size = fp->numchars + 3;
			if (memcmp(fp->storage, flddupptr(fp), size)) {
				return(1);
				}
			break;
		case FT_BULK:
			if (memcmp(flddupptr(fp), fp->storage, fldstosize(fp))) {
				return(1);
				}
			break;
		}
	/* no mismatch found	*/
	return(0);
}
static complist *rtobj2lst(rtobj *p)
{
	field *fp;
	byte appno;
	index itemno;

	if (p->obj.object == FLD) {
		fp = (field *)(p->ptr);
		if (fp->flags.type != FT_IND || fp->storage[0] != PreLST)
			return(0);
		appno = fp->storage[1];
		itemno = (fp->storage[2] << 8) | fp->storage[3];
		return((complist *)ddget(RTddh, LST, appno, itemno));
		}
	else if (p->obj.object == LST)
		return((complist *)p->ptr);
	return((complist *)0);
}
static bool
mangle_field(register dlist *whichlist, register field *fp, byte token)
{
	dlist			*dlp;
	static void		*rtdrawTE;
	static void		*rtdrawfield;

	if (hRTdll == NULL) {
		hRTdll = LoadLibrary("rt.dll");
		if (!hRTdll)
			return(RT_ERROR);
		}
	if (rtdrawTE == NULL) {
		rtdrawTE = GetProcAddress(hRTdll,"drawTEfield");
		if (!rtdrawTE)
			return(RT_ERROR);
		}
	rtdrawfield = GetProcAddress(hRTdll,"drawfield");
	if (!rtdrawTE)
		return(RT_ERROR);
	if (whichlist) {
		/* then if this field is in the current display */
		/* list, either show it, or erase it... */
		dlp = dl_getfirst(whichlist);
		while (dlp) {
			if (dlp != Currentdlp) {
				/* don't allow hiding of current field */
				if (dlp->dspfunc == rtdrawfield) {
					if (fp == (field *) dlp->obj) {
						if (token == HIDEFIELD)
							dl_refresh_rect(&dlp->bounds);
						else
							drawfield(dlp);
						return(TRUE);
						}
					}
				else if (dlp->dspfunc == rtdrawTE) {
					struct RT_TEFIELD *rtfl;
					rtfl = dlp->obj;
					if (fp == rtfl->fp) {
						if (token == HIDEFIELD) {
							RECT temp;
							byte boxtype;
							short offset;
							offset = -2;	/* clear additional region */
											/* even if no box          */
							temp = dlp->bounds;
							boxtype = (byte)rtfl->boxtype; 
							if (boxtype == DBOXBORDDS ||
							    boxtype == DBOXBORD2)
								offset = -4;
							else if (boxtype == DBOXBORD3)
								offset = -6;
							/* clear box delimiter also */
							InsetRect(&temp,offset,offset);
							dl_refresh_rect(&temp);
							}
						else
							drawTEfield(dlp);
						return(TRUE);
						}
					}
				}
			dlp = dl_getnext(whichlist,dlp);
			}
		}
	return(FALSE);
}
/* This function will do the decompile of the screen and append the fields to the
	CDS buffer 
*/

int ScrDecompileAppend(cdsselect *CdsSelectPtr,byte *buffer,int ExceptCnt,byte *ExceptPtr)

{
	int			total = 0;			/* counts lines decompiled */
	int 		SaveExCnt=0;
	int 		*Row,row;
	int			*Col,col;
	byte 		*ip;
	byte		*Address[2400];
	int 		 counter =0;
	byte 		*ApplNo,appl;
	index 		*ItemNo,item;
	byte 		ExceptAppl;
	byte		*SaveExceptPtr;
	index		ExceptItem;
	int 		TotalStatements;
	int			CurrentStatement=0;
	int 		AppendYN=TRUE;
	short		ms=0;
	field		*fp=0;
	field 		*FieldAppend;
	int			save_in_IBA;

	// Must set this so pcldecompile() will not deallocate local variables.
	save_in_IBA = running_under_IBA;
	running_under_IBA = 1; 
	TotalStatements = pcldecompile(0,buffer,Address);
	running_under_IBA = save_in_IBA; 

	Col = &col;
	Row = &row;
	ApplNo = &appl;
	ItemNo = &item;
	ExceptCnt = ExceptCnt-1;
	ip = buffer;
	SaveExceptPtr = ExceptPtr;
	while (TotalStatements--) {
		SaveExCnt = ExceptCnt;
		ExceptPtr = SaveExceptPtr;
		ip = Address[CurrentStatement++];
	   	if ((*ip) == DRAW) {
		   		/* update count for return value */
			if (GetFieldObj(ip,ApplNo,ItemNo,Row,Col) != -2)
				total++;
			else
				continue;
			while (SaveExCnt--)	{
			   	fp = rt_get_fp(ExceptPtr,&ms,NULL);
				if (fp)	{
				    ExceptPtr += ms - 3;
					ExceptAppl = *ExceptPtr++;
					ExceptItem = *ExceptPtr++ << 8;
					ExceptItem |= *ExceptPtr++;
					if (!(ExceptAppl == *ApplNo && ExceptItem == *ItemNo))
				    	AppendYN = TRUE;
					else	{
						AppendYN = FALSE;
						break;
						}
						ms = 0;
					}      // END IF
				else	{
					mre(-1,"ERROR IN GETTING THE EXCEPTION FIELD");
					return (RT_ERROR);
					}	
				}		// END WHILE
			if (AppendYN == TRUE)	{
			 	if (*Row == -1 && *Col == -1)
			 	  	FieldAppend = ddget(RTddh,FLD,*ApplNo,*ItemNo);	
				else
					FieldAppend = ddaget(RTddh,*ApplNo,*ItemNo,(short)*Row,(short)*Col);
				if (FieldAppend != 0)	{
				   	if (rtaddfield(CdsSelectPtr,FieldAppend,*ApplNo,
						*ItemNo,FieldAppend) == RT_ERROR)	{
						mre(-1,"proc_list:Error in appending the field");
						return (-2);
						}
					}
				}
				
			}
 		}   // End of while
	return(total);
}

/* This function decompiles and returns the field names into an array of
	ASCII fields
*/
 int ScrDecompile(byte *buffer,rtobj *dap,int FieldSize,int NumCols)

{
	int			TotalFields=0;			/* counts lines decompiled */
	int 		*Row,row;
	int			*Col,col;
	byte 		*ip;
	byte		*Address[2400];
	int 		 counter =0;
	byte 		*ApplNo,appl;
	char 		Name[256];
	char 		RowStr[10];
	char 		ColStr[10];
	index 		*ItemNo,item;
	int 		TotalStatements;
	field		*FieldPtr;
	int			CurrentStatement=0;
	int			save_in_IBA;

	// Must set this so pcldecompile() will not deallocate local variables.
	save_in_IBA = running_under_IBA;
	running_under_IBA = 1; 
	TotalStatements = pcldecompile(0,buffer,Address);
	running_under_IBA = save_in_IBA; 

	FieldPtr = (field *) dap->ptr;
	Col = &col;
	Row = &row;
	ApplNo = &appl;
	ItemNo = &item;
	ip = buffer;
	while (TotalStatements--) {
		ip = Address[CurrentStatement++];
	   	if ((*ip) == DRAW) {
			if (GetFieldObj(ip,ApplNo,ItemNo,Row,Col) != -2)
				++TotalFields;
			else
			    continue;
		    if (TotalFields > NumCols)	{
		       	mre(-1,"The Number of fields on the screen > array size of the field");
				return -2;
				}
			strcpy(Name,ddapplname(RTddh,*ApplNo));
			strcat(Name,":");
			strcat(Name,ddname(RTddh,FLD,*ApplNo,*ItemNo));
			if (*Row != -1)	{
				strcat(Name,"[");
				strcat(Name,itoa(*Row,RowStr,10));
				strcat(Name,"]");
				}
			if (*Col != -1)	{
				strcat(Name,"[");
				strcat(Name,itoa(*Col,ColStr,10));
				strcat(Name,"]");
				}
			str2field(Name,FieldPtr);
			FieldPtr = (field *)((char *)dap->ptr + TotalFields * FieldSize);
			}
	  	}
	return(TotalFields);
}

//-----------------------------------------------------------------------------
// This function eliminates all the other DRAW VERBS which do not 
// involve fields and returns the application and item numbers of the
// fields.
//-----------------------------------------------------------------------------

int GetFieldObj (byte *Pos,byte *applNo,index *itemNo,int *row,int *col)

{
byte Token;
byte *array;
byte ApplNo;
index ItemNo;
short i;
++ Pos;  // Skip over DRAW VERB
Token = *Pos++;
*row = -1;
*col = -1;
switch (Token)	{
	case DP_FIELD:
	case DP_COMBOFIELD:
		Pos = Pos + 2;	// Skip Row 
		Pos = Pos + 2;  // Skip Column
		i = *Pos++;
		if (i == TYPE_ARRAY)	{
			array = Pos - 1;
			Pos += tsize (array) - 5;
			i = *Pos++;
			array++;
			GetArraySubscripts(array,row,col);
			}
		ApplNo = *Pos++;
		ItemNo = *Pos++ << 8;
		ItemNo |= *Pos++;
		break;
	case DP_MLFIELD:
		Pos += 2;   	// Skip Row
		Pos += 2;   	// Skip Col
		Pos += 2;	  	// Skip Height
		Pos += 2;		// Skip Width
		Pos += 1;		// Skip Box Type
		Pos += 1;		// Skip Flags
		i = *Pos++;
		if (i == TYPE_ARRAY)	{
			array = Pos - 1;
			Pos += tsize (array) - 5;
			i = *Pos++;
			array++;
			GetArraySubscripts(array,row,col);
			}
		ApplNo = *Pos++;
		ItemNo = *Pos++ << 8;
		ItemNo |= *Pos++;
		break;
	default:
	    return -2;
	}
	*applNo = ApplNo;
	*itemNo = ItemNo;
	return 0;
}

//-------------------------------------------------------------------
// This function returns the array subscripts taking the byte pointer
// which points to the begining of the storage of array info.
//-------------------------------------------------------------------
int GetArraySubscripts(byte *Array,int *Row,int *Col)

{
byte *s1;
byte *s2;
byte *WorkPtr;
/* First SubScript */
s1 = Array;
if (*Array == 0xFF)	{
   	Array++;
	Array += tsize(Array);
	}
else
	Array += Array[0]* 256 + Array[1] + 2;
/* Second Subscript */
s2 = NULL;
if (*Array == TYPE_ARRAY)	{
	s2 = ++Array;
	if (*Array == 0xFF)	{
		Array++;
		Array += tsize(Array);
		}
	else
		Array += Array[0] * 256 + Array[1] + 2;
	}
WorkPtr = s1;
if (*s1 == 0xFF)	{
	*Col = *(WorkPtr+2);
	++WorkPtr;
	}
else
	WorkPtr += 2;
if (s2)	{
	WorkPtr = s2;
	if (*s2 == 0xFF)	{
		*Row = *Col;
		*Col = *(WorkPtr+2);
		++WorkPtr;
		}
	}
	return 0;
}


/*  function ==  CurrentScrFieldsToDup
**  Author: Nagesh Nookala 1/15/195
**  Description:
**		Pushes all the fields on the CurrentScreen into the dup area 
**		if the argument is  COPY_TO and pops from dup area otherwise
**  RETURNS 0                 - Success
**			1				  - only for COMPARE_DUP (some field has changed)
**          -2                - Error in pushing the elements
**/

int
DupCurrentScrFields(int Operation)

{
	static void	*rtdrawTE1, *rtdrawTE2, *rtdrawfield;
	dlist	*CurScrFlds,*ListPtr;
	int retval;

	field	*FieldPtr;
	if (hRTdll == NULL) {
		hRTdll = LoadLibrary("rt.dll");
		if (!hRTdll)
			return(RT_ERROR);
	}
	if (rtdrawTE1 == NULL) {
		rtdrawTE1 = GetProcAddress(hRTdll,"drawTEfield");
		if (!rtdrawTE1)
			return(RT_ERROR);
	}
	if (rtdrawTE2 == NULL)	{
		rtdrawTE2 = GetProcAddress(hRTdll,"drawTEtext");
		if (!rtdrawTE2)
			return(RT_ERROR);
	}
	if (rtdrawfield == NULL)	{
		rtdrawfield = GetProcAddress(hRTdll,"drawfield");
		if (!rtdrawfield)
			return(RT_ERROR);
	}

	CurScrFlds = Current_screen->fields; 	// Get the list of fields on scr
	ListPtr	= dl_getfirst(CurScrFlds);		// Get the first element
	while (ListPtr)	{						// Do until all the fields are done
		if ((ListPtr->dspfunc == rtdrawTE1) || (ListPtr->dspfunc == rtdrawTE2)) {
			FieldPtr = ((struct RT_TEFIELD *)(ListPtr->obj))->fp;
			if (FieldPtr == NULL)
				return -2;	
		} else if (ListPtr->dspfunc == rtdrawfield) {
			FieldPtr = (field *)ListPtr->obj;
			if (FieldPtr == NULL)
				return -2;	
		} else {
			// Ignore: draw box, draw line, draw pl string, pl hilite and pl scrollbar
			FieldPtr = NULL;
		}

		if (FieldPtr) {
			switch (Operation)	{
			  case COPY_TO: 		// Copy to Dup Area
				sav_fld_storage(FieldPtr);
				add_field_rec(FieldPtr);
				break;
			  case COPY_FROM:
				dup_fld_storage(FieldPtr);
				add_field_rec(FieldPtr);
				break;
			  case COMPARE_DUP:
				retval = compare_field_dup(FieldPtr);
				if (retval == 1)
					return 1;
				break;		
			}
		}
		ListPtr = dl_getnext(CurScrFlds,ListPtr);
	}
	return 0;
}


/* ********* Function : DupScrFields **************************************/
/* Author 	  : Nagesh Nookala	1/30/1995							   ****/
/* Description: Copies all the fields on the passed screen to their dup****/
/*              area/restore them/compare them                         ****/
/* retval	  : -2 something went wrong. (MesageBox will get displayed)****/
/* 		      : 0 for success/no-change in fields (for f_compare)     *****/
/*   		  : 1 for there is a change in the field (for f_compare)  *****/
/*																	  *****/
/**************************************************************************/

int DupScrFields(int Operation, rtobj *ScrObj)

{
	byte 		*buffer;
	byte 		*GenericCode;
	int			TotalFields=0;			/* counts lines decompiled */
	int 		*Row,row;
	int			*Col,col;
	int 		retval;
	byte 		*ip;
	byte		*Address[2400];
	int 		 counter =0;
	byte 		*ApplNo,appl;
	index 		*ItemNo,item;
	int 		TotalStatements;
	field		*FieldPtr;
	int			CurrentStatement=0;
	int			save_in_IBA;

	appl = ScrObj->obj.applno;
	item = ScrObj->obj.itemno;
	if ((GenericCode = ddget(RTddh,(byte) SCR,appl,
					item)) == 0)	{
		mre(-1,"proc_list: Error in Getting the Given SCREEN");
		return(CSR_FAIL_ARGTYPE);
		}		;	 
	buffer = GenericCode + sizeof(struct SCRDEF);

	// Must set this so pcldecompile() will not deallocate local variables.
	save_in_IBA = running_under_IBA;
	running_under_IBA = 1; 
	TotalStatements = pcldecompile(0,buffer,Address);
	running_under_IBA = save_in_IBA; 

	Col = &col;
	Row = &row;
	appl = 0;
	item = 0;
	ApplNo = &appl;
	ItemNo = &item;
	ip = buffer;
	while (TotalStatements--) {
		ip = Address[CurrentStatement++];
	   	if ((*ip) == DRAW) {
			if (GetFieldObj(ip,ApplNo,ItemNo,Row,Col) != -2)
				++TotalFields;
			else
			    continue;
			if (*Row == -1 && *Col == -1)
			  	FieldPtr = ddget(RTddh,FLD,*ApplNo,*ItemNo);	
			else
				FieldPtr = ddaget(RTddh,*ApplNo,*ItemNo,(short)*Row,(short)*Col);
			if (FieldPtr != NULL)	{
				switch (Operation)	{
					case COPY_TO: 		// Copy to Dup Area
						sav_fld_storage(FieldPtr);
						add_field_rec(FieldPtr);
						break;
					case COPY_FROM:
						dup_fld_storage(FieldPtr);
						add_field_rec(FieldPtr);
						break;
					case COMPARE_DUP:
						retval = compare_field_dup(FieldPtr);
						if (retval == 1)
							return 1;
						break;		
					}									// End switch
				}
			if (TotalFields > 2400)	{
		       	mre(-1,"The Number of fields on the screen > 2400");
				return -2;
				}
			}
	  	}
	return 0;
}


/*$
**
** MODULENAME - 

**
** $Name: R1V4C13 $
** $Revision: 1.18 $
** $Date: 1999/03/11 08:28:46 $
**
** Synopsis:
**   CSR for parsing the desktop menu list
**
**  Copyright (c) 1994 Olivetti North America.
**  All rights reserved.
**
**	Written by: Nagesh Nookala Winter 1994
**
**	Internal Functions called from OFB
**
**		dt_meu_parse()		 front-end func for parsing the compiled list
**
*/

#include "pch.h"

#include <typedefs.h>
#include <defines.h>
#include <rtprotos.h>
#include <convers.h>
#include <rtdef.h>
#include <dataman.h>
#include <errdefs.h>
#include <msgrtn.h>

/*$
**
**		Parse the list, compare the access bits with the common:menu_flags 
**	and initialize all the fields that are used in constructing the menu.
**  The fields that are used in constructing the menu are 
**				Name of the menu item
**				Action: This will point to another menu or transaction
**						or OFB procedure
**				Indicator: If the Action points to another menu, this will 
**						be "M"
**				Access: This will indicate the permissions of for current
**						user on the current menu item.
**				Menu Title: This is the title of the menu.
**	
**
**	Args
**		argc	- number of args
**		argv	- array of pointers to structures
**
**   ARGUMENTS 1 through 5 SHOULD be Array fields
**
**   Argument 0 is the DESKTOP MENU LIST (INPUT)
**   Argument 1 is the ON FLAGS			 (OUTPUT)
**   Argument 2 is the OFF FLAGS		 (OUTPUT)
**   Argument 3 is the NAME OF MENU ITEM (OUTPUT)
**   Argument 4 is the ACTION			 (OUTPUT)
**   Argument 5 is the INDICATOR IF THE ITEM CALLS  ANOTHER MENU (OUTPUT)
**   Argument 6 is the field that will get 0,-1 or -2 based on common:menu_flags
**   					and the ON and OFF flags	(OUTPUT)
**   Argument 7 is the field that gets the Menu Title which will be the first
**   					element in the desktop menu list	(OUTPUT)
**
*/

XDLL int 
dt_menu_parse(int argc,rtobj *argv[])
{
	arrayhdr		*ArrayHdr1=0,
					*ArrayHdr2=0,
					*ArrayHdr3=0,
					*ArrayHdr4=0,
					*ArrayHdr5=0,
					*ArrayHdr6=0;
	byte            ApplNo,
	 				*bp,
			        typeno;
	complist		*ListPtr = 0;
	byte			*MemberPtr=0,
					*PtrTemp=0,
	                rc,
					*ElementPtr=0;			 	
	int		 		MemberCnt,
					MemLength,
					i,
					ElementCnt,
					FlagBits,
					ComFlagBits;
	int				ObjType,
					Counter;
	unsigned short	*CommonMenuFlags=0;
 	index 			ItemNo;
	field			*FieldPtr[6]={0,0,0,0,0,0},
					*FldPtr=0,
					*TitleFldPtr;
	static short    firstime = TRUE;
	static	field	*MenuField=0;
	short			sval;

	//------------------------------------------------------------
	//-- Get the filed common:menu_flags for checking with the 
	//-- Access bits
	//------------------------------------------------------------
	if (firstime) {
		MenuField = ddgetbyname(RTddh,FLD,Common,"menu_flags");
		if (MenuField == NULL || MenuField->flags.type != FT_FLAGS) {
			mre(-1,swerrormsg(NOMENUFLAGS));
			RTerrno = BADMENUNAME;
			return(RT_ERROR);
			}
		firstime = FALSE;
		}
	CommonMenuFlags = &sval;
	sval = * (int *)(MenuField->storage) & 0xFFFF;
	if (argc != 8) 
    		return(CSR_FAIL_ARGCOUNT);    // Must have 8 arguments
    			/* get the opcode	*/
	ListPtr = 0;
	MemberPtr = 0;
			/* insure that it's really a list or a field	*/
	if (argv[0]->obj.object != LST && argv[0]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	FieldPtr[0] = argv[1]->ptr;
	FieldPtr[1] = argv[2]->ptr;
	FieldPtr[2] = argv[3]->ptr;
	FieldPtr[3] = argv[4]->ptr;
	FieldPtr[4] = argv[5]->ptr;
	FieldPtr[5] = argv[6]->ptr;
	TitleFldPtr = argv[7]->ptr;
	if (!FieldPtr[0]->flags.array || 
		!FieldPtr[1]->flags.array || 
		!FieldPtr[2]->flags.array ||
		!FieldPtr[3]->flags.array || 
		!FieldPtr[4]->flags.array ||
		!FieldPtr[5]->flags.array)	{
			mre (-1,"dt_parse_menu: Arguments 1 \
				through 6 should be Array fields");
		    return(CSR_FAIL_ARGTYPE);
		}
	ArrayHdr1 = (arrayhdr *)(FieldPtr[0]) - 1;
	ArrayHdr2 = (arrayhdr *)(FieldPtr[1]) - 1;
	ArrayHdr3 = (arrayhdr *)(FieldPtr[2]) - 1;
	ArrayHdr4 = (arrayhdr *)(FieldPtr[3]) - 1;
	ArrayHdr5 = (arrayhdr *)(FieldPtr[4]) - 1;
	ArrayHdr6 = (arrayhdr *)(FieldPtr[5]) - 1;
	if (argv[0]->obj.object == FLD) {
		FldPtr = argv[0]->ptr;
		if (FldPtr->flags.type == FT_IND) {
			if (*(FldPtr->storage) == LST) {
				ItemNo = (FldPtr->storage[2] << 8) + 
							FldPtr->storage[3];
				if ((ListPtr = (complist *)ddget(RTddh, *(FldPtr->storage),
					FldPtr->storage[1], (index)ItemNo)) == 0) {
					return(LISTERR);
					}
				}
  		 	}
		}
	else {
			/* get the pointer to the new list	*/
		if ((ListPtr = (complist *)ddget(RTddh, argv[0]->obj.object,
			argv[0]->obj.applno, argv[0]->obj.itemno)) == 0) {
			return(LISTERR);
			}
		}
	MemberPtr = (byte *)(ListPtr + 1);
	MemberCnt = ListPtr->numitems;
	MemberPtr += 2;  // Skip over the MEMBER token 
	MemLength = (int)(*(MemberPtr + 1)); 
	MemberPtr += 2;		   // Skip over the member length
	MemberPtr += MemLength;	 // Bump to Next Memeber
	ElementCnt = (int )(*(MemberPtr+1));
	if (ElementCnt == 1)	{	  // Do this only if there is title
	                              // Make the R1 menus work too
		PtrTemp = MemberPtr + 4;  // Skip until the data
		pcl2field(PtrTemp,TitleFldPtr,NULL);  // Get the title
		MemberPtr += 2;  // Skip over the MEMBER token 
		MemLength = (int)(*(MemberPtr + 1)); 
		MemberPtr += 2;		   // Skip over the member length
		MemberPtr += MemLength;	 // Bump to Next Memeber
		MemberCnt = MemberCnt - 1;
		}
	for (Counter=0;Counter < (MemberCnt-1);Counter++)	{
		if (Counter > 9) 
			break;
	    ElementCnt = (int )(*(MemberPtr+1));
	    ElementPtr = MemberPtr + 4;  
	    	// skip over member token element count and member length 
		i = 0;
		if (ElementCnt != 5)	{
			clr_fld_explicit(FieldPtr[4]);
			}
		while (ElementCnt)	{
		    ObjType = *ElementPtr;
			if (ObjType <= DICT_COUNT && i == 3)	{
			    ItemNo = (int)(*(ElementPtr + 2) << 8) + *(ElementPtr + 3);
			    ApplNo = *(ElementPtr + 1);
				if (FieldPtr[i]->flags.type != FT_IND)
				    return(CSR_FAIL_ARGTYPE);
				if (ApplNo == 0 || ItemNo == 0)
					clr_fld_explicit(FieldPtr[i]);
				else {
			        typeno = ObjType;
					rc = (byte) ddgetstat(RTddh,typeno,ApplNo,ItemNo);
					if (rc == DDSTATFAIL)	{
						typeno = PCL;
						}
					bp = FieldPtr[i]->storage;
					*bp++ = typeno;
					*bp++ = ApplNo;
					*bp++ = ItemNo >> 8;
					*bp = ItemNo & 0xff;
					}
				}
			else	{
			   		/* it's a constant, make it into a string	*/
				pcl2field(ElementPtr, FieldPtr[i],NULL);
			        /* If we are at flags and com flags */
				if (i == 0) {
					pcl2int(ElementPtr,&FlagBits,NULL);
					}
				else	{
					if (i == 1)	{
						pcl2int(ElementPtr,&ComFlagBits,NULL);
						}
					}
				}
				i++;
				ElementCnt--;
				ElementPtr += tsize(ElementPtr);   // Got next element
			}
			// check the RTmenuoption (set by init 51) to see
			// if we "or" or "and" the menu flags 
			if (RTmenuoption){
				int2field(0,FieldPtr[5]);
				if (FlagBits) {
					if ((*CommonMenuFlags & FlagBits) != 0){
						int2field(0,FieldPtr[5]);
						}
					else {
						if (Ghostselection != 0)
							int2field(-1,FieldPtr[5]);
						else
							int2field(-2,FieldPtr[5]);
				    	}
				}
				if (ComFlagBits) {
					if ((*CommonMenuFlags & ComFlagBits) == 0){
						int2field(0,FieldPtr[5]);
						}
					else {
						if (Ghostselection != 0)
							int2field(-1,FieldPtr[5]);
						else
							int2field(-2,FieldPtr[5]);
				    	}
				}
					

			}
			else{
				if (((*CommonMenuFlags & FlagBits) == FlagBits) && 
					((~*CommonMenuFlags & ComFlagBits) == ComFlagBits))	{
					int2field(0,FieldPtr[5]);
					}
				else	{
					if (Ghostselection != 0)
						int2field(-1,FieldPtr[5]);
					else
						int2field(-2,FieldPtr[5]);
			    	}
			}
			FieldPtr[0] = (field *)((char *)FieldPtr[0] + 
							(ArrayHdr1->sizeperfield));
			FieldPtr[1] = (field *)((char *)FieldPtr[1] + 
							(ArrayHdr2->sizeperfield));
			FieldPtr[2] = (field *)((char *)FieldPtr[2] + 
							(ArrayHdr3->sizeperfield));
			FieldPtr[3] = (field *)((char *)FieldPtr[3] + 
							(ArrayHdr4->sizeperfield));
			FieldPtr[4] = (field *)((char *)FieldPtr[4] + 
							(ArrayHdr5->sizeperfield));	   
			FieldPtr[5] = (field *)((char *)FieldPtr[5] + 
							(ArrayHdr6->sizeperfield));
			MemberPtr += 2;  // Skip over the MEMBER token 
			MemLength = (int)(*(MemberPtr + 1)); 
			MemberPtr += 2;		   // Skip over the member length
			MemberPtr += MemLength;	 // Bump to Next Memeber

		}
		return(RT_SUCCESS);
	}


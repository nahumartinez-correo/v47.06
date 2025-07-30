//
// MODULENAME - removr.c
//
// $Name: R1V4C13 R1V4C12 R1V4C11 $
// $Revision: 1.7 $
// $Date: 1997/09/26 23:21:24 $
//
#include "pch.h"

#include <defines.h>
#include <dsplist.h>
#include <rtdef.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <convers.h>
#include <rtprotos.h>

/* NOTE: use tab settings of 2 */

/* Miscellaneous defines used in this module */
/* Remember to allow 1 byte for null */
#define NUM_OVR_REQ			10  	/* initial number of override requests */
#define REQ_OPER_LEN		14+1	/* length of operator requesting override */
#define REQ_WS_LEN			24+1	/* length of workstation name of operator requesting override */
#define REQ_DATE_LEN		20+1	/* length of override request date */
#define REQ_TIME_LEN		 8+1	/* length of override request time */
#define REQ_MSG1_LEN		70+1	/* length of override request message part 1 */
#define REQ_MSG2_LEN		70+1	/* length of override request message part 2 */
#define RCV_OPER_LEN		 8+1	/* length of operator id receiving override */
#define RCV_WS_LEN			24+1	/* length of workstation name receiving override */
#define PICK_LINE_LEN		64+1	/* max length of line for picklist */
#define MAX_OVR_DISP		50  	/* max overrides in picklist */
#define REQ_PRODUCT_LEN		 3+1	/* length of field describing product, PP, P8, PSP */

/* Layout of information for a remote override request */
typedef struct ovr_req {
	struct ovr_req *next;			/* required by list manager */
	struct ovr_req *prev;			/* required by list manager */ 
	short flags;							/* required by list manager */
	char req_oper[REQ_OPER_LEN];	/* operator requesting override  */
	char req_ws[REQ_WS_LEN];			/* workstation name of operator requesting override  */ 
	char req_date[REQ_DATE_LEN];	/* date of override request, stored as yyyymmdd */
	char req_time[REQ_TIME_LEN];	/* time of override request, stored as hh:mm:ss */
	char req_msg1[REQ_MSG1_LEN];	/* part 1 of message sent with override request */
	char req_msg2[REQ_MSG2_LEN];	/* part 2 of message sent with override request */
	char rcv_oper[RCV_OPER_LEN];	/* operator's id of operator receiving override request */
	char rcv_ws[RCV_WS_LEN];			/* workstation name of operator receiving override request */
	char req_product[REQ_PRODUCT_LEN];	/* short description to determine product override came from */
																			/* PP - Pinnacle Plus, P8 - Pinnacle 8. */
	unsigned short seq_num;				/* unique number given each override request  */
} OVR_REQ;

/* Function prototypes */

static void check_ovr_init(void);

static short item_count = 1;	/* Gets incremented each time insert an override into list */
int c_sanewpl(char *name, int x, int y, int w_width, int w_height, int font, int face, int size, 
  int forecolor, int backcolor, int boxwidth, int boxheight, int boxtype, 
  rtobj *title, index itemdepth, short flags, rtobj *key_filter);
int c_saaddpl(char *name, char *val);
int c_sadopl(char *name, char *target, int targlen);
int c_sadroppl(char *name);

OVR_REQ *ovr_free_ptr;		/* Pointer to memory block for override structures */
OVR_REQ *ovr_list_ptr;		/* Pointer to list of override structures */

/*eject*/

/***********************************************************************

	FUNCTION:	Insert an override request into the override request list.

	USAGE: call saovreq_insert

	PARAMETERS:
		argv[0] rtobj *	- pointer to "ro_req_oper" field or equivalent
		argv[1] rtobj *	- pointer to "ro_req_ws" field or equivalent
		argv[2] rtobj *	- pointer to "ro_req_date" field or equivalent
		argv[3] rtobj *	- pointer to "ro_req_time" field or equivalent
		argv[4] rtobj *	- pointer to "ro_req_msg1" field or equivalent
		argv[5] rtobj *	- pointer to "ro_req_msg2" field or equivalent
		argv[6] rtobj *	- pointer to "ro_rcv_oper" field or equivalent
		argv[7] rtobj *	- pointer to "ro_rcv_ws" field or equivalent
		argv[8] rtobj * - pointer to "ro_req_product" field or equivalent 
	
	RETURNS:
		status:			int			0	:all is well
									CSR_FAIL_ARGTYPE :bad field argument type

	NOTE:
		ro       - stands for remote override
		req      - stands for request
		op, oper - stands for operator
		rcv      - stands for receiving
		ws       - stands for workstation
		msg      - stands for message

***********************************************************************/
XDLL int 
saovreq_insert(int argc, rtobj *argv[])
{

	OVR_REQ *ovr_item_ptr;	/* Pointer to an override req in override list */
	field	*req_date_ptr;
	char	eddatebuf[REQ_DATE_LEN];
	int 	retval = 0;

	/* check for correct argument count	*/
	if (argc != 9)
		return(CSR_FAIL_ARGCOUNT);

	if (argv[0]->obj.object != CSRARG_FLD ||
	    argv[1]->obj.object != CSRARG_FLD ||
	    argv[2]->obj.object != CSRARG_FLD ||
	    argv[3]->obj.object != CSRARG_FLD ||
	    argv[4]->obj.object != CSRARG_FLD ||
	    argv[5]->obj.object != CSRARG_FLD ||
	    argv[6]->obj.object != CSRARG_FLD ||
	    argv[7]->obj.object != CSRARG_FLD ||
	    argv[8]->obj.object != CSRARG_FLD )
	{
		rterrorbox("saovrreq_insert requires field arguments");
		return(CSR_FAIL_ARGTYPE);
	}

	/* create override list structure if necessary */
	check_ovr_init();
	
	/* get a pointer to override list, so we can add a new item */
	ovr_item_ptr = gl_append(ovr_list_ptr, ovr_free_ptr);

	/* Now move data from fields into override list */
	if (rtobj2str(argv[0], ovr_item_ptr->req_oper, sizeof(ovr_item_ptr->req_oper)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;
	if (rtobj2str(argv[1], ovr_item_ptr->req_ws, sizeof(ovr_item_ptr->req_ws)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;

	/* edit the date before moving it to output	*/
	req_date_ptr = (field *)argv[2]->ptr;
	editfill(eddatebuf, req_date_ptr->storage, req_date_ptr, ' ');
	memset(eddatebuf + strlen(eddatebuf), ' ', REQ_DATE_LEN - strlen(eddatebuf));
	memcpy(ovr_item_ptr->req_date, eddatebuf, sizeof(eddatebuf));
	ovr_item_ptr->req_date[REQ_DATE_LEN - 1] = '\0';

	if (rtobj2str(argv[3], ovr_item_ptr->req_time, sizeof(ovr_item_ptr->req_time)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;
	if (rtobj2str(argv[4], ovr_item_ptr->req_msg1, sizeof(ovr_item_ptr->req_msg1)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;
	if (rtobj2str(argv[5], ovr_item_ptr->req_msg2, sizeof(ovr_item_ptr->req_msg2)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;
	if (rtobj2str(argv[6], ovr_item_ptr->rcv_oper, sizeof(ovr_item_ptr->rcv_oper)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;
  	if (rtobj2str(argv[7], ovr_item_ptr->rcv_ws, sizeof(ovr_item_ptr->rcv_ws)) == RT_ERROR)
		retval = CSR_FAIL_ARGTYPE;

	rtobj2str(argv[8], ovr_item_ptr->req_product, sizeof(ovr_item_ptr->req_product));
	if (!strlen(ovr_item_ptr->req_product))	{
		ovr_item_ptr->req_product[0] = 'P';		/* Default to Pinnacle Plus "PP" */
		ovr_item_ptr->req_product[1] = 'P';
		ovr_item_ptr->req_product[2] = '\0';
	}

	/* Now assign unique sequence number for override request */ 
	if (!item_count)
	  item_count++;	/* Bump item_count to 1 if it wrapped back to 0 */
    ovr_item_ptr->seq_num = item_count++;
	
	return(retval);
}
/*eject*/

/***********************************************************************

	FUNCTION:	Delete an override request into the override request list.

	USAGE: call saovreq_delete

	PARAMETERS:
		seq_num_ptr		rtobj *		- pointer to "ro_seq_num" field
	  OR
		req_op_ptr		rtobj *		- pointer to "ro_req_oper" field
		req_ws_ptr		rtobj *		- pointer to "ro_req_ws" field
	
	  You can either delete an override by calling routine with sequence 
	  number or the requesting operator and requesting workstation but 
	  do not use all three parameters.

	RETURNS:
		status:			int			0	:all is well
									-1	:invalid number of arguments
									-2	:no override found to delete
									CSR_FAIL_ARGTYPE :bad field argument type
	NOTE:
		ro       - stands for remote override
	    seq_num  - stands for sequence number
		req      - stands for request
		op,oper  - stands for operator
		ws       - stands for workstation

***********************************************************************/
XDLL int 
saovreq_delete(int argc, rtobj *argv[])
{
	OVR_REQ *ovr_item_ptr;	/* Pointer to an override req in override list */
	field	*req_op_ptr, *req_ws_ptr, *seq_num_ptr;
	int found;
	int seq_num_to_find;
	int status;
	
	if (argc == 1)
		if (argv[0]->obj.object != CSRARG_FLD)
		{
			rterrorbox("saovrreq_delete requires field arguments");
			return(CSR_FAIL_ARGTYPE);
		}
		else
		{
			seq_num_ptr = (field *)argv[0]->ptr;
			if (field2int(seq_num_ptr,&seq_num_to_find) == RT_ERROR)
				return(CSR_FAIL_FIELDTYPE);
		}
	else if (argc == 2)
		if (argv[0]->obj.object != CSRARG_FLD ||
			argv[1]->obj.object != CSRARG_FLD)
		{
			rterrorbox("saovrreq_delete requires field arguments");
			return(CSR_FAIL_ARGTYPE);
		}
		else
		{
			/* Assign pointers to point to respective field info */
			req_op_ptr = (field *)argv[0]->ptr;
			req_ws_ptr = (field *)argv[1]->ptr;
		}
	else
	{
		rterrorbox("saovrreq_delete has invalid number of field arguments");
		return(-1);
	}

	check_ovr_init();	/* create override list structure if necessary */

	/* Now loop through override list looking for specific override to delete */
	found = FALSE;	/* assume no items found */
	status = -2;	/* assume return status is no items found to delete */

	ovr_item_ptr = gl_getfirst(ovr_list_ptr);
	while (ovr_item_ptr && !found)
	{
		if (argc == 1)	/* delete using sequence number */
		{
			if (ovr_item_ptr->seq_num == seq_num_to_find)
				found = TRUE;
		}
		else				/* delete using requesting operator and workstation */
		{
			if (strncmp((char *)ovr_item_ptr->req_oper, (char *)req_op_ptr->storage, req_op_ptr->numchars) == 0 &&
					strncmp((char *)ovr_item_ptr->req_ws, (char *)req_ws_ptr->storage, req_ws_ptr->numchars) == 0)
				found = TRUE;
		}

		if (found)
		{
			gl_remove(ovr_list_ptr,ovr_item_ptr,ovr_free_ptr);
			status = 0;
		}
		else
			ovr_item_ptr = gl_getnext(ovr_list_ptr, ovr_item_ptr);
	}

	return(status);
}
/*eject*/

/***********************************************************************

	FUNCTION:	Determine the number of outstanding override requests.

	USAGE: call  saovreq_count

	PARAMETERS: none

	RETURNS:
		status:			int			number of overrides pending

***********************************************************************/
XDLL int 
saovreq_count(int argc, rtobj *argv[])
{
	OVR_REQ *ovr_item_ptr;	/* Pointer to an override req in override list */
	int ovr_count;

	check_ovr_init();	/* create override list structure if necessary */

	ovr_count = 0;
	ovr_item_ptr = gl_getfirst(ovr_list_ptr);
	while (ovr_item_ptr)
	{
		ovr_count++;
		ovr_item_ptr = gl_getnext(ovr_list_ptr, ovr_item_ptr);
	}	
	
	return(ovr_count);
}

/*eject*/

/***********************************************************************

	FUNCTION:	Display picklist of pending override requests.

	USAGE: call  saovreq_pldisp

	PARAMETERS: none

	RETURNS:
		status:			int			0 : no selection made
									-1: no overrides for display
								sequence number of override chosen

	NOTE:
		ro       - stands for remote override
		req      - stands for request
		op, oper - stands for operator
		rcv      - stands for receiving
		ws       - stands for workstation
		msg      - stands for message

***********************************************************************/
XDLL int 
saovreq_pldisp(int argc, rtobj *argv[])
{
	OVR_REQ *ovr_item_ptr;	/* Pointer to an override req in override list */
	char pick_line[PICK_LINE_LEN];
	unsigned short ovr_index[MAX_OVR_DISP];
	int index_count = 0;
	int item_pick;
	int status;

	check_ovr_init();	/* create override list structure if necessary */

	if (!(ovr_item_ptr = gl_getfirst(ovr_list_ptr)))
	{
		rterrorbox("Sorry, No Override Requests Are Pending.");
		return(-1);	
	}

	memset(ovr_index, 0, sizeof(ovr_index)); 
	c_sanewpl("ovr_disp", 48, 126, 0,0,theFont, theFace, theSize, theForeColor, theBackColor, 520, 0, 1, NULL, 1, 0, NULL);	/* setup delimeters to picklist */
	while (ovr_item_ptr && index_count < MAX_OVR_DISP)
	{
		/* create line for picklist display */
		memset(pick_line, ' ', sizeof(pick_line)); 
		pick_line[sizeof(pick_line) -1] = '\0';
		memcpy(pick_line, ovr_item_ptr->req_oper, StringLen(ovr_item_ptr->req_oper));
		memcpy(pick_line + 18, ovr_item_ptr->req_ws, StringLen(ovr_item_ptr->req_ws));
		memcpy(pick_line + 35, ovr_item_ptr->req_date, StringLen(ovr_item_ptr->req_date));
		memcpy(pick_line + 56, ovr_item_ptr->req_time, StringLen(ovr_item_ptr->req_time));
	
	/* Save off sequence number of override into array position which */
	/* matches item number of picklist. This is done inorder to determine */
	/* which sequence number of override to return base on picklist choice */  

		ovr_index[index_count] = ovr_item_ptr->seq_num;
		index_count++;	  

		/* adds line into the picklist */
	  	c_saaddpl("ovr_disp",pick_line);

		/* get next override for picklist display */
		ovr_item_ptr = gl_getnext(ovr_list_ptr, ovr_item_ptr);
	}	

	if (index_count == MAX_OVR_DISP)
	{
		rterrorbox("Warning, reached max number of overrides that can be displayed");
	}
	rterrmsg_via_list(1,NULL,"Select an Override Request and press ENTER",-1);
	item_pick = c_sadopl("ovr_disp",(char *)0, 0); /* display the picklist */

	/* Reset or something pressed, exit */
	if (item_pick < 0)
		status = 0;
	else
		status = ovr_index[item_pick];

	c_sadroppl("ovr_disp");

	return(status);
}
/*eject*/

/***********************************************************************

	FUNCTION:	Get an override request from the override list.

	USAGE: call saovreq_get

	PARAMETERS:
	
		seq_num_ptr		field *		- pointer to "ro_seq_num" field
		req_op_ptr		field *		- pointer to "ro_req_oper" field
		req_ws_ptr		field *		- pointer to "ro_req_ws" field
		req_date_ptr	field *		- pointer to "ro_req_date" field
		req_time_ptr	field *		- pointer to "ro_req_time" field
		req_msg1_ptr	field *		- pointer to "ro_req_msg1" field
		req_msg2_ptr	field *		- pointer to "ro_req_msg2" field
		rcv_op_ptr		field *		- pointer to "ro_rcv_oper" field
		rcv_ws_ptr		field *		- pointer to "ro_rcv_ws" field	
		req_product_ptr		field *		- pointer to "ro_req_product" field	

	RETURNS:
		status:			int		0	:all is well
											-1	:no matching override found
									CSR_FAIL_ARGTYPE :bad field argument type

	NOTE:
		ro       - stands for remote override
		req      - stands for request
		op, oper - stands for operator
		rcv      - stands for receiving
		ws       - stands for workstation
		msg      - stands for message

		If enter with a sequence number of 0, the first override in 
		structure list is got and it's sequence number is returned.
***********************************************************************/
XDLL int 
saovreq_get(int argc, rtobj *argv[])
{
	OVR_REQ *ovr_item_ptr;	/* Pointer to an override req in override list */
	field	*seq_num_ptr, *req_op_ptr, *req_ws_ptr;
	field	*req_date_ptr, *req_time_ptr, *req_msg1_ptr;
	field	*req_msg2_ptr, *rcv_op_ptr, *rcv_ws_ptr, *req_product_ptr; 
	int found;
	int seq_num_to_find;
	int status;

	if (argv[0]->obj.object != CSRARG_FLD ||
			argv[1]->obj.object != CSRARG_FLD ||
			argv[2]->obj.object != CSRARG_FLD ||
			argv[3]->obj.object != CSRARG_FLD ||
			argv[4]->obj.object != CSRARG_FLD ||
			argv[5]->obj.object != CSRARG_FLD ||
			argv[6]->obj.object != CSRARG_FLD ||
			argv[7]->obj.object != CSRARG_FLD ||
			argv[8]->obj.object != CSRARG_FLD ||
			argv[9]->obj.object != CSRARG_FLD )
	{
		rterrorbox("saovrreq_insert requires field arguments");
		return(CSR_FAIL_ARGTYPE);
	}

	/* Assign pointers to point to respective field info */
	seq_num_ptr = (field *)argv[0]->ptr;
	req_op_ptr = (field *)argv[1]->ptr;
	req_ws_ptr = (field *)argv[2]->ptr;
	req_date_ptr = (field *)argv[3]->ptr;
	req_time_ptr = (field *)argv[4]->ptr;
	req_msg1_ptr = (field *)argv[5]->ptr;
	req_msg2_ptr = (field *)argv[6]->ptr;
	rcv_op_ptr = (field *)argv[7]->ptr;
	rcv_ws_ptr = (field *)argv[8]->ptr;
	req_product_ptr = (field *)argv[9]->ptr;

	check_ovr_init();	/* create override list structure if necessary */

	/* Now loop through override list looking for specific override to get */
	found = FALSE;	/* assume no matching override found */
	status = -1;		/* assume return status is no matching override found */

	if (field2int(seq_num_ptr,&seq_num_to_find) == RT_ERROR)
		return(CSR_FAIL_FIELDTYPE);
	ovr_item_ptr = gl_getfirst(ovr_list_ptr);
	while (ovr_item_ptr && !found)
	{
		if (ovr_item_ptr->seq_num == seq_num_to_find || seq_num_to_find == 0)
		{
			/* Override found, move data from override list to fields */
			str2field(ovr_item_ptr->req_oper,req_op_ptr);
			str2field(ovr_item_ptr->req_ws,req_ws_ptr);
			str2field(ovr_item_ptr->req_date,req_date_ptr);
			str2field(ovr_item_ptr->req_time,req_time_ptr);
			str2field(ovr_item_ptr->req_msg1,req_msg1_ptr);
			str2field(ovr_item_ptr->req_msg2,req_msg2_ptr);
			str2field(ovr_item_ptr->rcv_oper,rcv_op_ptr);
			str2field(ovr_item_ptr->rcv_ws,rcv_ws_ptr);
			str2field(ovr_item_ptr->req_product,req_product_ptr);
			if (seq_num_to_find == 0)
				int2field(ovr_item_ptr->seq_num, seq_num_ptr);
			found = TRUE;
			status = 0;
		}
		else
			ovr_item_ptr = gl_getnext(ovr_list_ptr, ovr_item_ptr);
	}	

	return(status);
}
/*eject*/

static void check_ovr_init(void)
/***********************************************************************

	FUNCTION:	Determine if override list structure has been created
		        and if not then create override list structure.

	USAGE: internal to this routine

	PARAMETERS: none

	RETURNS: nothing
***********************************************************************/
{
	if (!ovr_free_ptr)
	{
		/* free area not yet initialized for override list, so create */
		ovr_free_ptr = gl_createfreev(sizeof(OVR_REQ), NUM_OVR_REQ);

		/* set up list header to overrides */
		ovr_list_ptr = gl_create();
	}
}

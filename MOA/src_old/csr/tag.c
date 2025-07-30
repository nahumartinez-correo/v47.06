// ==================================================================
// MODULENAME - tag.c
//
// $Name: R1V4C13 $
// $Revision: 1.17 $
// $Date: 1999/03/11 08:28:50 $
//
//  A subroutine to alter the appearance of controls/buttons on the
//  current screen -- used by the 'Application Desktop' in the BASE.
//
//  Probably written by Randy Geer
//  Altered slightly for mosaic OA by H. Marc Lewis
//  Added new functions for MOSAIC desktop driver by Nagesh Nookala
// ==================================================================

#include "pch.h"

#include <typedefs.h>
#include <convers.h>
#include <defines.h>
#include <dsplist.h>
#include <rtdef.h>
#include <csrdef.h>
#include <dpwords.h>
#include <errdefs.h>
#include <rtprotos.h>
#include <msgrtn.h>
#include <csr.h>

	byte	*_get_mem(register short i, register byte *bp);
	int		_redraw_tag(int argc, rtobj *argv[]);
	int		_redraw_tag_all(int argc, rtobj *argv[]);
	int		_redraw_tag_list(int argc, rtobj *argv[]);
	int		_status_of_tag(int argc, rtobj *argv[]);
	void	_do_redraw_crp(rtcontrol *crp, int attr, int sfc, int sbc,
							int fc, int bc, int dfc, int dbc, int down, 
							int hidden, int disabled);
	void	_do_redraw(unsigned long iu, int attr, int sfc, int sbc,
							int fc, int bc, int dfc, int dbc, int down, 
							int hidden, int disabled);
	void	_do_redraw_all(int cmd, int attr, int sfc, int sbc,
							int fc, int bc, int dfc, int dbc, int down, 
							int hidden, int disabled);
	int		_get_max_buts (int argc, rtobj *argv[]);
	int		_redraw_but_list (int argc, rtobj *argv[]);

    void       status_save(unsigned long iu,field *fp1);
    void       status_rest(unsigned long iu,field *fp1);
      
    int      _save_tag_list(int argc, rtobj *argv[]);
	void	DrawButtons(unsigned long ,char *);

//#define TAG_REDRAW		1
//#define TAG_REDRAW_LIST	2
//#define TAG_STATUS		3
//#define TAG_REDRAW_ALL	4
	int		save_attr[100];
	int		save_sfc[100];
	int		save_sbc[100];
	int		save_fc[100];
	int		save_bc[100];
	int		save_dbc[100];
	int		save_dfc[100];
	short	save_flags[100];
	int		save_cnt;


XDLL int 
tag(int argc, rtobj *argv[])
{
	int type = -1;

	rtobj2int(argv[0], &type);
    if ((type != 5) && (type != 6) && (type != 7)) {
        if (argc != 10) 
		      mre(-1,"Custom C subroutine: tag\n...Requires 10 arguments");
	}
    else   {
           if ((type == 5) && (argc < 3))
               mre (-1,"Custom C subroutine: tag\n...Requires atleast 4 arguments");
		   if ((type == 6) && (argc != 2))
     		   mre (-1,"Custom C subroutine: tag\n...Requires 2 arguments");
		   if ((type == 7) && (argc != 3))
     		   mre (-1,"Custom C subroutine: tag\n...Requires 2 arguments");
	}
    switch (type) {
		case TAG_REDRAW:
			return(_redraw_tag(argc, argv));
			break;
		case TAG_REDRAW_LIST:
			return(_redraw_tag_list(argc, argv));
			break;
		case TAG_STATUS:
			return(_status_of_tag(argc, argv));
			break;
		case TAG_REDRAW_ALL:
			return(_redraw_tag_all(argc, argv));
			break;
		case TAG_SAVE_LIST:
			return(_save_tag_list(argc, argv));
			break;
		case TAG_GET_MAX_BUTS:
		    return(_get_max_buts(argc, argv));
	    case REDRAW_TAG_LIST:
			return(_redraw_but_list(argc, argv));
			break;
		default:
			return(CSR_FAIL_ARGTYPE);
			break;
		}

}


int
_redraw_tag_list(int argc, rtobj *argv[])
{
	int				sfc = -1,  sbc = -1, attr = -1,fc = -1, 
					bc = -1, dfc = -1, dbc = -1,
					down = -1, hidden = -1, disabled = -1;
	unsigned long	iu;
	byte			mem_cnt;
	complist		*cl;
	byte			*mem_ptr;
	field			*fp = 0;
	index 			itemno;
	int 			i, j;
	char			pos[4];

	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND) {
			if (*(fp->storage) == LST) {
				itemno = (fp->storage[2] << 8) + fp->storage[3];
				cl = (complist *)ddget(RTddh, *(fp->storage),fp->storage[1], itemno);
				}
			else {
				mre(-1,"tag: REDRAW LIST first arg not a list");
				return(CSR_FAIL_ARGTYPE);
				}
			}
		else {
			mre(-1,"tag: REDRAW LIST first arg not a list");
			return(CSR_FAIL_ARGTYPE);
			}
		}
	else {
		cl = (complist *) argv[1]->ptr;
		}
	if (rtobj2int(argv[2], &sfc) == RT_ERROR
		|| rtobj2int(argv[3], &sbc) == RT_ERROR
		|| rtobj2int(argv[4], &fc) == RT_ERROR
		|| rtobj2int(argv[5], &bc) == RT_ERROR
		|| rtobj2int(argv[6], &dfc) == RT_ERROR
		|| rtobj2int(argv[7], &dbc) == RT_ERROR
		|| rtobj2int(argv[8], &attr) == RT_ERROR
		|| rtobj2str(argv[9], &pos[0],sizeof(pos)) == RT_ERROR) {
		mre(-1,"tag: REDRAW LIST invalid field type");
		return(CSR_FAIL_ARGTYPE);
		}

	switch (pos[0]) {
		case ' ':
			down = -1;
			break;
		case '0':
			down = 0;
			break;
		case '1':
			down = 1;
			break;
		}
	switch (pos[1]) {
		case ' ':
			hidden = -1;
			break;
		case '0':
			hidden = 0;
			break;
		case '1':
			hidden = 1;
			break;
		}
	switch (pos[2]) {
		case ' ':
			disabled = -1;
			break;
		case '0':
			disabled = 0;
			break;
		case '1':
			disabled = 1;
			break;
		}
	for (i = 0; i < cl->numitems; i++) { 
		mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;
		/* skip each tag in list */
		for (j = 0; j < mem_cnt; j++) {
			if (*mem_ptr != TAG)
				return(-2);
			iu  = ((unsigned long)(*mem_ptr)) << 24;
			iu |= ((unsigned long)(mem_ptr[1])) << 16;
			iu |= ((unsigned long)(mem_ptr[2])) << 8;
			iu |= (unsigned long)(mem_ptr[3]);
			_do_redraw(iu, attr, sfc, sbc, fc, bc, dfc, dbc, down, 
						hidden, disabled);
			mem_ptr += tsize(mem_ptr);
			}
		}
	return(0);
}
//-----------------------------------------------------------------------
// This Routine takes a list of tags and a list of fields ans copies the 
// Positions of the tags (hidden,down,enable) into the fields and vice-versa
// If the first member of the list of fields points to an array field, then
// the rest of the fields in the list will be ignored and the status of each
// tag in the tag list will be placed in the array field starting at row and 
// column specified and 
// Args:   LISToftags, LISTof_fields, TAG_SAVE(or)RESTORE, row_to_start,
//			,col_to_start, NO_OF_TAGS_TO_SAVE/RESTORE
//-------------------------------------------------------------------------------
int
_save_tag_list(int argc, rtobj *argv[])
{
	int			down = -1, hidden = -1, disabled = -1;
	unsigned long	iu;
	byte			mem_cnt,mem_cnt1,appl_no;
	index           item_no;
	complist		*cl,*cl1;
	byte			*mem_ptr,*mem_ptr1;
	field			*fp = 0,*fp1 = 0;
	long            l;
	index 			itemno;
	int 			i,num_col,num_row,drow,dcol,save_rest,count;
	char			array;
    rtobj                   *rp,rp_mem;
	short		        ms;
    arrayhdr                *fhdr;
    rp = &rp_mem;
    array = 'N';   
	// The first list is a list of tags. Get the list even if it is
	// pointed to by an IND field
	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND) {
			if (*(fp->storage) == LST) {
				itemno = (fp->storage[2] << 8) + fp->storage[3];
				cl = (complist *)ddget(RTddh, *(fp->storage),fp->storage[1], itemno);
				}
			else {
				mre(-1,"tag: REDRAW LIST first arg not a list");
				return(CSR_FAIL_ARGTYPE);
				}
			}
		else {
			mre(-1,"tag: REDRAW LIST first arg not a list");
			return(CSR_FAIL_ARGTYPE);
			}
		}
	else {
		cl = (complist *) argv[1]->ptr;
		}
	// The second list is a list of fields which will be used for saving or
	// restoring the data. Get the list.
	if (argv[2]->obj.object == FLD) {
		fp1 = argv[2]->ptr;
		if (fp1->flags.type == FT_IND) {
			if (*(fp1->storage) == LST) {
				itemno = (fp1->storage[2] << 8) + fp1->storage[3];
				cl1 = (complist *)ddget(RTddh, *(fp1->storage),fp1->storage[1], itemno);
				}
			else {
				mre(-1,"tag: REDRAW LIST first arg not a list");
				return(CSR_FAIL_ARGTYPE);
				}
			}
		else {
			mre(-1,"tag: REDRAW LIST first arg not a list");
			return(CSR_FAIL_ARGTYPE);
			}
		}
	else {
		cl1 = (complist *) argv[2]->ptr;
		}
	// This will decide whether to save or restore the tag statuses
    if (argc > 3) 
		    rtobj2int(argv[3], &save_rest);
	// The next three fields are used only if the first field of 
	// the fields list is an array field
    if (argc > 4) 
		rtobj2int(argv[4], &drow);
    if (argc > 5) 
		rtobj2int(argv[5], &dcol);
	if (argc > 6) 
		rtobj2int(argv[6], &count); 
	i = 0;
	// Get the first element in the fields list which will have or receive
	// the status of each tag in the other list.
	mem_ptr1 = _get_mem((short )i,(byte *)(cl1 + 1));
	mem_cnt1 = mem_ptr1[1];
	mem_ptr1 += 4;
	if (! (fp1 = rt_get_fp(mem_ptr1,&ms,NULL)))	{
	   	mre (-1,"tag: SAVE LIST no such field");
		return(-NOFIELD);
		}
	// If the first element points to an array, then get the size of the 
	// array and check whether the array size will be big enough to hold 
	// statuses of all the elements in the tag list.
	if (fp1->flags.array)    {
		array = 'Y';
		appl_no = mem_ptr1[1];
		item_no= (mem_ptr1[2] << 8) | mem_ptr1[3];
		l = ddarraysize(RTddh,appl_no,item_no);
		num_row = (l >> 16) & 0x00ffff;
	    num_col = l & 0x00ffff;
		if ((drow > (num_row -1)) && (drow) && (num_row))	{
            mre (-1,"tag: SAVE LIST row number is greater than no. of rows");
			return(-2);
           	}
        if (num_col < (count + dcol))  { 
			mre (-1,"tag: SAVE LIST col number is greater than no. of cols");
			return(-2);
       		}
	    fhdr = (arrayhdr *)(fp1) - 1;
		fp1 = (field *)((char *)fp1 + ((drow * num_col + dcol) * fhdr->sizeperfield));
       	}
	else
		array = 'N';
	// For each element in the list, get the status or set the status of the control
	// identified by the tag.
	for (i = 0; i < count; i++) {
	 				// skip each tag in list 
	    mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;
		if (*mem_ptr != TAG) {
        	mre (-1,"tag: SAVE LIST first element should be a list of tags");
  			return(-2);
        	}
		iu  = ((unsigned long)(*mem_ptr)) << 24;
		iu |= ((unsigned long)(mem_ptr[1])) << 16;
		iu |= ((unsigned long)(mem_ptr[2])) << 8;
		iu |= (unsigned long)(mem_ptr[3]);
	
		if (save_rest == TAG_SAVE)
	    	status_save(iu, fp1);
		else
		    status_rest(iu, fp1);
 		mem_ptr += tsize(mem_ptr);
		// Get the next field pointer
		if (array == 'N')	{
			mem_ptr1 += tsize(mem_ptr1); 
			if (! (fp1 = rt_get_fp(mem_ptr1,&ms,NULL)))
		    	return(-NOFIELD);
			}
        else
	        fp1 = (field *)((char *)fp1 + fhdr->sizeperfield);

		}		// End of for loop
	return(0);
}


int
_redraw_tag(int argc, rtobj *argv[])
{
	int				sfc = -1,  sbc = -1, attr = -1,fc = -1, 
					bc = -1, dfc = -1, dbc = -1,
					down = -1, hidden = -1, disabled = -1;
	unsigned long	iu;
	field			*fp = 0;
	byte 			appno;
	index			 itemno;
	char			pos[4];

	if (rtobj2int(argv[2], &sfc) == RT_ERROR
		|| rtobj2int(argv[3], &sbc) == RT_ERROR
		|| rtobj2int(argv[4], &fc) == RT_ERROR
		|| rtobj2int(argv[5], &bc) == RT_ERROR
		|| rtobj2int(argv[6], &dfc) == RT_ERROR
		|| rtobj2int(argv[7], &dbc) == RT_ERROR
		|| rtobj2int(argv[8], &attr) == RT_ERROR
		|| rtobj2str(argv[9], &pos[0],sizeof(pos)) == RT_ERROR) {
		mre(-1,"tag: REDRAW invalid field type");
		return(CSR_FAIL_ARGTYPE);
		}

	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND) {
			appno = fp->storage[1];
			itemno = (fp->storage[2] << 8) | fp->storage[3];
			iu  = ((unsigned long)TAG) << 24;
			iu |= ((unsigned long)(appno)) << 16;
			iu |= (unsigned long)(itemno);
			}
		else {
			return(CSR_FAIL_ARGTYPE);
			}
		}
	else {
		if (argv[1]->obj.object != TAG)
			return(CSR_FAIL_ARGTYPE);
		iu  = ((unsigned long)TAG) << 24;
		iu |= ((unsigned long)(argv[1]->obj.applno)) << 16;
		iu |= (unsigned long)(argv[1]->obj.itemno);
		}
	switch (pos[0]) {
		case ' ':
			down = -1;
			break;
		case '0':
			down = 0;
			break;
		case '1':
			down = 1;
			break;
		}
	switch (pos[1]) {
		case ' ':
			hidden = -1;
			break;
		case '0':
			hidden = 0;
			break;
		case '1':
			hidden = 1;
			break;
		}
	switch (pos[2]) {
		case ' ':
			disabled = -1;
			break;
		case '0':
			disabled = 0;
			break;
		case '1':
			disabled = 1;
			break;
		}
	_do_redraw(iu, attr, sfc, sbc, fc, bc, dfc, dbc, down, 
				hidden, disabled);
	return(0);
}


void
_do_redraw(unsigned long iu, int attr, int sfc, int sbc, int fc, 
			int bc, int dfc, int dbc, int down, int hidden, int disabled)
{
	rtcontrol		*crp;

	crp = gl_getfirst(Current_screen->controls);
	while (crp && (crp->flags & GL_INUSE)) {
		/* only if list is non-empty */
		while (crp) {
			if (iu == crp->tag)
				break;
			crp = gl_getnext(Current_screen->controls,crp);
			}
		if (crp) {
			_do_redraw_crp(crp, attr, sfc, sbc, fc, bc, dfc, dbc,
			down, hidden, disabled);
			/* done with this one, get the next list element */
			crp = gl_getnext(Current_screen->controls,crp);
			}
		}
}


int 
_redraw_tag_all(int argc, rtobj *argv[])
{
	int				sfc = -1,  sbc = -1, attr = -1,fc = -1, 
					bc = -1, dfc = -1, dbc = -1,
					down = -1, hidden = -1, disabled = -1;
	int				cmd;
	char			pos[4];

	if (rtobj2int(argv[1], &cmd) == RT_ERROR
		|| rtobj2int(argv[2], &sfc) == RT_ERROR
		|| rtobj2int(argv[3], &sbc) == RT_ERROR
		|| rtobj2int(argv[4], &fc) == RT_ERROR
		|| rtobj2int(argv[5], &bc) == RT_ERROR
		|| rtobj2int(argv[6], &dfc) == RT_ERROR
		|| rtobj2int(argv[7], &dbc) == RT_ERROR
		|| rtobj2int(argv[8], &attr) == RT_ERROR
		|| rtobj2str(argv[9], &pos[0],sizeof(pos)) == RT_ERROR) {
		mre(-1,"tag: REDRAW invalid field type");
		return(CSR_FAIL_ARGTYPE);
		}

	switch (pos[0]) {
		case ' ':
			down = -1;
			break;
		case '0':
			down = 0;
			break;
		case '1':
			down = 1;
			break;
		}
	switch (pos[1]) {
		case ' ':
			hidden = -1;
			break;
		case '0':
			hidden = 0;
			break;
		case '1':
			hidden = 1;
			break;
		}
	switch (pos[2]) {
		case ' ':
			disabled = -1;
			break;
		case '0':
			disabled = 0;
			break;
		case '1':
			disabled = 1;
			break;
		}
	_do_redraw_all(cmd, attr, sfc, sbc, fc, bc, dfc, dbc, down, 
					hidden, disabled);
	return(0);
}


void 
_do_redraw_all(int cmd, int attr, int sfc, int sbc, int fc, int bc, 
				int dfc, int dbc, int down, int hidden, int disabled)
{
	rtcontrol		*crp;
	short			is;
	//	int s_attr, s_sfc, s_sbc, s_fc, s_bc, s_dfc, s_dbc;
	//	short			s_flags;
	save_cnt = 0;
	crp = gl_getfirst(Current_screen->controls);
	while (crp && (crp->flags & GL_INUSE)) {
		if (crp) {
			if (crp->type == DP_RELIEFBUTTON) {
				if (save_cnt >= 100) {							
					mre(-1,"tag: Number of Buttons is more than 100");
					return;
					}
				if (cmd == 1) {
					save_attr[save_cnt] = crp->dlp->attr;  // Save the attribs
						save_sfc[save_cnt] = crp->dlp->forecolor;
					save_sbc[save_cnt] = crp->dlp->backcolor;
					save_fc[save_cnt] = crp->fcolor;
					save_bc[save_cnt] = crp->bcolor;
					save_dbc[save_cnt] = crp->dbcolor;
					save_dfc[save_cnt] = crp->dfcolor;
					save_flags[save_cnt] = crp->flags;

					_do_redraw_crp(crp, attr, sfc, sbc, fc, bc, dfc, dbc,
					down, hidden, disabled);
					}
				else {
					crp->dlp->attr = save_attr[save_cnt];  // Restore attribs
						crp->dlp->forecolor = save_sfc[save_cnt];
					crp->dlp->backcolor = save_sbc[save_cnt];
					crp->fcolor = save_fc[save_cnt];
					crp->bcolor = save_bc[save_cnt];
					crp->dbcolor = save_dbc[save_cnt];
					crp->dfcolor = save_dfc[save_cnt];
					crp->flags = save_flags[save_cnt];
					is = GetCtlValue((HWND)crp->cid) ? DP_UP : DP_DOWN;
					if (crp->flags & ~DL_PROTECTED) {
						HiliteControl((HWND)crp->cid,0);
						}
					if (crp->flags & DL_PROTECTED) {
						HiliteControl((HWND)crp->cid,255);
						}
					alter_button(crp, is, (field *)0);
					}
				save_cnt = save_cnt + 1;
				}
			/* done with this one, get the next list element */
			crp = gl_getnext(Current_screen->controls,crp);
			}
		}
}


void 
_do_redraw_crp(rtcontrol *crp, int attr, int sfc, int sbc, int fc, 
				int bc, int dfc, int dbc, int down, int hidden, int disabled)
{
	short			is;

	if (attr >= 0)
		crp->dlp->attr = (short) attr;
	if (sfc >= 0)
		crp->dlp->forecolor = sfc;
	if (sbc >= 0)
		crp->dlp->backcolor = sbc;
	if (fc >= 0)
		crp->fcolor = fc;
	if (bc >= 0)
		crp->bcolor = bc;
	if (dbc >= 0)
		crp->dbcolor = dbc;
	if (dfc >= 0)
		crp->dfcolor = dfc;
	if (down >= 0) {
		if (down == 1) {
			is = DP_DOWN;
			}
		else {
			is = DP_UP;
			}
		}
	else {
		is = GetCtlValue((HWND)crp->cid) ? DP_UP : DP_DOWN;
		}
	if (hidden >= 0) {
		if (hidden == 1 && crp->flags & DL_ISVISIBLE) {
			crp->flags &= ~DL_ISVISIBLE;
			HideControl((HWND)crp->cid);
			}
		if (hidden == 0 && crp->flags & ~DL_ISVISIBLE) {
			crp->flags |= DL_ISVISIBLE;
			ShowControl((HWND)crp->cid);
			}
		}
	if (disabled >= 0) {
		if (disabled == 1 && crp->flags & ~DL_PROTECTED) {
			crp->flags |= DL_PROTECTED;
			HiliteControl((HWND)crp->cid,255);
			}
		if (disabled == 0 && crp->flags & DL_PROTECTED) {
			crp->flags &= ~DL_PROTECTED;
			HiliteControl((HWND)crp->cid,0);
			}
		}
	/* Added conditions to check for hidden and disabled also -Nagesh */
	if (attr >= 0 ||
			fc >= 0 ||
			bc >= 0 ||
			dfc >= 0 ||
			dfc >= 0 ||
			down >= 0 ||
			hidden >= 0 ||
			disabled >= 0) {
		alter_button(crp, is, (field *)0);
		}
}


int 
_status_of_tag(int argc, rtobj *argv[])
{
	unsigned long	iu;
	int				fore, back, dfore, dback;
	rtcontrol		*crp;
	short			attr;
	field		    *result,*fp;
	byte appno;
	index itemno;
	char pos[4];

	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND) {
			appno = fp->storage[1];
			itemno = (fp->storage[2] << 8) | fp->storage[3];
			iu  = ((unsigned long)TAG) << 24;
			iu |= ((unsigned long)(appno)) << 16;
			iu |= (unsigned long)(itemno);
		}
		else	{
			return(CSR_FAIL_ARGTYPE);
		}
	}
	else {
		if (argv[1]->obj.object != TAG)
			return(CSR_FAIL_ARGTYPE);
		iu  = ((unsigned long)TAG) << 24;
		iu |= ((unsigned long)(argv[1]->obj.applno)) << 16;
		iu |= (unsigned long)(argv[1]->obj.itemno);
	}
	crp = gl_getfirst(Current_screen->controls);
	if (crp && (crp->flags & GL_INUSE)) {
		while (crp) {
			if (iu == crp->tag) {
				break;
			}
			crp = gl_getnext(Current_screen->controls,crp);
		}
		if (crp) {
			fore = crp->dlp->forecolor;
			result = (field *) argv[2]->ptr;
			int2field(fore,result);

			back = crp->dlp->backcolor;
			result = (field *) argv[3]->ptr;
			int2field(back,result);

			dfore = crp->fcolor;
			result = (field *) argv[4]->ptr;
			int2field(dfore,result);

			dback = crp->bcolor;
			result = (field *) argv[5]->ptr;
			int2field(dback,result);

			dfore = crp->dfcolor;
			result = (field *) argv[6]->ptr;
			int2field(dfore,result);

			dback = crp->dbcolor;
			result = (field *) argv[7]->ptr;
			int2field(dback,result);

			attr = crp->dlp->attr;
			result = (field *) argv[8]->ptr;
			int2field(attr,result);

			if (GetCtlValue((HWND)crp->cid) & DP_UP) {
				pos[0] = '0';
			}
			else {
				pos[0] = '1';
			}

			if (crp->flags & DL_ISVISIBLE) {
				pos[1] = '0';
			}
			else {
				pos[1] = '1';
			}
			if (crp->flags & DL_PROTECTED) {
				pos[2] = '1';
			}
			else {
				pos[2] = '0';
			}
			pos[3] = 0;
			result = (field *) argv[9]->ptr;
			str2field(&pos[0],result);
			return(0);
		}
	}
	return(-1);
}


void
status_save(unsigned long iu,field *fp1)

{
	rtcontrol		*crp;
	char pos[4];
	crp = gl_getfirst(Current_screen->controls);
	
	if (crp && (crp->flags & GL_INUSE)) {
		    while (crp) {
			    if (iu == crp->tag) {
			   		break;
				}
			    crp = gl_getnext(Current_screen->controls,crp);
		    }
		    
			if (crp) {
			    if (GetCtlValue((HWND)crp->cid) & DP_UP)
				    pos[0] = '0';
				else 
					pos[0] = '1';
				
				if (crp->flags & DL_ISVISIBLE)
				    pos[1] = '0';
				else
				    pos[1] = '1';
				
				if (crp->flags & DL_PROTECTED)
				    pos[2] = '1';
				else
					pos[2] = '0';
				
				pos[3] = '\0';
			    str2field(&pos[0],fp1);
				}
		}
}


void
status_rest(unsigned long iu,field *fp1)

{
	rtcontrol		*crp;
	short           is;
	char pos[4];
	
	field2str(fp1,pos,4);
	
	if (strlen(pos) == 0)	
		return;
	
	crp = gl_getfirst(Current_screen->controls);
	if (crp && (crp->flags & GL_INUSE)) {
		while (crp) {
			if (iu == crp->tag) {
				break;
			}
			crp = gl_getnext(Current_screen->controls,crp);
		}
		    
		if (crp) {
			if (pos[0] == '1') 
				is = DP_DOWN;	    
			else 
				is = DP_UP;
				
			is = GetCtlValue((HWND)crp->cid) ? DP_UP : DP_DOWN;
				
			if (pos[1] == '1')	{
				crp->flags &= ~DL_ISVISIBLE;
				HideControl((HWND)crp->cid);
			}
			else {
				crp->flags |= DL_ISVISIBLE;
				ShowControl((HWND)crp->cid);
			}
				
			if (pos[2] == '1')	{
				crp->flags |= DL_PROTECTED;
				HiliteControl((HWND)crp->cid,255);
			}
			else { 
				crp->flags &= ~DL_PROTECTED;
				HiliteControl((HWND)crp->cid,0);
			}
			    
			alter_button (crp, is, (field *)0);
		}
	}
}

/* This function is a Crude way of finding the number of buttons on the
   given screen. It assumes that the last digit of the screen name tells
   you the number of buttons and if it is 0 the number of buttons is 10
   Usage: tag INDfld (or SCR name)
   RETVAL = No. of buttons on the screen
*/

int
_get_max_buts(int argc, rtobj *argv[])
{   
    char            name[25],*temp;                            		
 	field			*fp = 0;
	byte            appl_no;
	index 			item_no;
	int 			name_len,max_buts;

	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		
		if (fp->flags.type == FT_IND) {
			if (*(fp->storage) == SCR) {
				item_no = (fp->storage[2] << 8) + fp->storage[3];
				appl_no = fp->storage[1];
			}
			else {
				mre(-1,"tag: GET MAX BUTS first arg not a scr");
				return(CSR_FAIL_ARGTYPE);
			}
		}
		else {
			mre(-1,"tag: GET MAX BUTS first arg not a scr");
			return(CSR_FAIL_ARGTYPE);
		}
	}
	else {
		if (argv[1]->obj.object == SCR)   {
			item_no = argv[1]->obj.itemno;
			appl_no = argv[1]->obj.applno;
		}
		else {
		    mre(-1,"tag: GET MAX BUTS first arg not a scr");
			return(CSR_FAIL_ARGTYPE);
		}
	}
	
	temp = ddname (RTddh,SCR,appl_no,item_no);
	
	if (temp != NULL) {
		 strcpy (name,temp);
		 name_len = strlen (name);
		 max_buts = atoi (&name[name_len - 1]);
		 
		 if (max_buts == 0)
		      max_buts = 10;
			  
		 return (max_buts);
	}
	
	return -1;
}

/* This Routine takes a list of tags and a list of fields and redraws the 
   buttons associated with those tags depending upon the fields (normal if
   the field is 0 and ghosted if the field is -1. This is used in desktop
   menu draw code in the Foundation Application.
   Args:   LISToftags,LISTof_fields
*/
int
_redraw_but_list (int argc, rtobj *argv[])
{
	int			down = -1, hidden = -1, disabled = -1;
	unsigned long	iu;
	byte			mem_cnt;
	complist		*cl;
	byte			*mem_ptr;
	field			*fp = 0,*fp1 = 0;
	long            l;
	index 			itemno;
	int 			i,num_col,num_row,count,auth;
	char			array;
    rtobj                   *rp,rp_mem;
	arrayhdr                *fhdr;
    
	rp = &rp_mem;
    array = 'N';
	
	if (argv[1]->obj.object == FLD) {
		fp = argv[1]->ptr;
		if (fp->flags.type == FT_IND) {
			if (*(fp->storage) == LST) {
				itemno = (fp->storage[2] << 8) + fp->storage[3];
				cl = (complist *)ddget(RTddh, *(fp->storage),fp->storage[1], itemno);
			}
			else {
				mre(-1,"tag: REDRAW LIST first arg not a list");
				return(CSR_FAIL_ARGTYPE);
			}
		}
		else {
			mre(-1,"tag: REDRAW LIST first arg not a list");
			return(CSR_FAIL_ARGTYPE);
		}
	}
	else {
		cl = (complist *) argv[1]->ptr;
	}
   
	if (argv[2]->obj.object == FLD) {
		fp1 = argv[2]->ptr;
	}
	else {
   		mre(-1,"tag: REDRAW LIST second arg not a field");
		return(CSR_FAIL_ARGTYPE);	
   }
   
	count = cl->numitems;
	if (fp1->flags.array) {
		l = ddarraysize(RTddh, argv[2]->obj.applno, argv[2]->obj.itemno);
		num_row = (l >> 16) & 0x00ffff;
	    num_col = l & 0x00ffff;
		
		if (num_row > 0) {
           	mre (-1,"tag: REDRAW BUTTONS second argument should be one dimentional array");
			return(-2);
        }
        
		if (num_col < count)  {
           	mre (-1,"tag: REDRAW BUTTONS number of tags is not equal to size of the array");
           	return(-2);
        }
		fhdr = (arrayhdr *)(fp1) - 1;
   }
   else	{
		mre (-1,"tag: REDRAW BUTTONS number of tags is not equal to size of the array");
        return(-2);	
	}
	
	for (i = 0; i < count; i++) { 
	    mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;
		
		if (*mem_ptr != TAG) {
        	mre (-1,"tag: REDRAW BUTTONS first element should be a list of tags");
  			return(-2);
        }
		
		iu  = ((unsigned long)(*mem_ptr)) << 24;
		iu |= ((unsigned long)(mem_ptr[1])) << 16;
		iu |= ((unsigned long)(mem_ptr[2])) << 8;
		iu |= (unsigned long)(mem_ptr[3]);
		
		auth = 0;
		field2int (fp1,&auth);
		if (auth == 0)
		    DrawButtons(iu,"000");
		else
			DrawButtons(iu,"001");
	   	
		mem_ptr += tsize(mem_ptr);
		fp1 = (field *)((char *)fp1 + fhdr->sizeperfield);
	}
	return(0);
}


void
DrawButtons(unsigned long iu,char *state)

{
	rtcontrol		*crp;
	short           is;
	char pos[4];
	
	strcpy(pos,state);
	crp = gl_getfirst(Current_screen->controls);
	
	if (crp && (crp->flags & GL_INUSE)) {
		while (crp) {
			if (iu == crp->tag) {
				break;
			}
			crp = gl_getnext(Current_screen->controls,crp);
		}
		    
		if (crp) {
			if (pos[0] == '1') 
				is = DP_DOWN;	    
			else 
				is = DP_UP;	  
				
			is = GetCtlValue((HWND)crp->cid) ? DP_UP : DP_DOWN;
				
			if (pos[1] == '1')	{
				crp->flags &= ~DL_ISVISIBLE;
				HideControl((HWND)crp->cid);
			}
			else {
				crp->flags |= DL_ISVISIBLE;
				ShowControl((HWND)crp->cid);
			}
				
			if (pos[2] == '1')	{
				crp->flags |= DL_PROTECTED;
				HiliteControl((HWND)crp->cid,255);
			}
			else	{ 
				crp->flags &= ~DL_PROTECTED;
				HiliteControl((HWND)crp->cid,0);
			}
			    
			alter_button (crp, is, (field *)0);
		}
	}
}


		 




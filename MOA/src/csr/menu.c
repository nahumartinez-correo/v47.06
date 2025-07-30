/*$
** MODULENAME - menu.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.12 $
** $Date: 1996/03/08 19:38:12 $
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
#include <ofbdefs.h>
#include <convers.h>
#include <defines.h>
#include <dsplist.h>
#include <tdspl.h>
#include <scrlist.h>
#include <rt.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <pclwords.h>
#include <rtprotos.h>
#include <msgrtn.h>

void c_menuevt();
void refresh_statusarea();
static int make_win_menu (int menuid);

#define NULLCL ((complist *) 0)
#define	MAXPDMENU	10

typedef struct pdm {
	HMENU		hpopmenu;
	complist	*cl;
	char		flag;
	} pulldownmenu;

static	pulldownmenu	pdxmenu[MAXPDMENU];
static	HMENU			hMenu;

byte	*_get_mem(register short i, register byte *bp);

#define WIN_MENU_INIT 		20
#define WIN_MENU_ON 		21
#define WIN_MENU_OFF 		22
#define WIN_MENU_ITEM_ON 	23
#define WIN_MENU_ITEM_OFF 	24
#define WIN_MENU_DROP 		25
#define WIN_MENU_DELETE		26

#define WIN_MENU_ITEM_SET 	30
#define WIN_MENU_ITEM_CHECK 31
#define WIN_MENU_ITEM_MARK 	32
#define WIN_MENU_ITEM_STYLE 33
#define WIN_MENU_ITEM_KEY   34


#define WIN_MENU_ALL_ON		10
#define WIN_MENU_ALL_OFF	11
#define WIN_MENU_EXECUTE 	12

#define WIN_MENU_HELP		99


XDLL int
win_menu(int argc, rtobj *argv[])
{
	long	check;
	int		type, menuid, item_num, key;
	char	val[128];
	RECT	wrect;
	byte	*pclptr;

	if (Winmenu_flag & VERB_WINMENU) {
		rterrorbox("CSR:win_menu, ERROR: Winmenu verbs active");
		return(-1);
		}
		
	if (argc < 1) {
		rterrorbox("CSR:win_menu: Parm count error");
		return CSR_FAIL_ARGCOUNT;
		}
	if (rtobj2int(argv[0],&type) != RT_SUCCESS) {
		rterrorbox("CSR:win_menu: Type Error");
		return CSR_FAIL_ARGTYPE;
		}
	if (type > 19 && type != 99) {
		if (argc < 2) {
			rterrorbox("CSR:win_menu: Parm count error");
			return CSR_FAIL_ARGCOUNT;
			}
		if (rtobj2int(argv[1],&menuid) != RT_SUCCESS || 
			menuid < 1 || menuid > MAXPDMENU) {
			rterrorbox("CSR:win_menu: Menu number Error");
			return CSR_FAIL_ARGTYPE;
			}
		menuid--;
		}
	if (type > 29 && type != 99) {
		if (argc < 3) {
			rterrorbox("CSR:win_menu: Parm count error");
			return CSR_FAIL_ARGCOUNT;
			}
		}
	switch(type) {
		case WIN_MENU_INIT:
			if (pdxmenu[menuid].flag == 1) {
				DeleteMenu(hMenu,menuid,MF_BYPOSITION);
				DrawMenuBar(hWnd);
				pdxmenu[menuid].cl       = NULLCL;
				pdxmenu[menuid].flag     = 0;
				pdxmenu[menuid].hpopmenu = 0;
				}
			pdxmenu[menuid].cl = (complist *)ddget(RTddh, argv[2]->obj.object,
					argv[2]->obj.applno, argv[2]->obj.itemno);
			if (pdxmenu[menuid].cl == NULLCL) {
				rterrorbox("CSR:win_menu: Invalid list");
				return(-2);
				}
			if (!(Winmenu_flag & CSR_WINMENU)) {
				if (!GetWindowRect(hWnd,&wrect))
					return(-2);
				rtwdw_resize(0,PDMENUHEIGHT);
				if (!MoveWindow(hWnd,wrect.left,wrect.top,
				                wrect.right - wrect.left,
								wrect.bottom - wrect.top + PDMENUHEIGHT,TRUE))
					return(-2);
				do_PCLredraw();	/* refresh status line wdw changed size */
				Winmenu_flag |= CSR_WINMENU;
				}
			key = make_win_menu(menuid);
			if (key == 0)
				SetMenu(hWnd,hMenu);
			return(key);
			break;
		case WIN_MENU_ON:
			if (rtobj2int(argv[1],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: menu number Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num -= 1;
			if (pdxmenu[menuid].flag == 0) {
				check = EnableMenuItem(hMenu,item_num,MF_ENABLED|MF_BYPOSITION);
				pdxmenu[menuid].flag = 1;
				if (! DrawMenuBar(hWnd))
					check = GetLastError();
				}
			break;
		case WIN_MENU_ALL_ON:
			for (menuid = 0; menuid < MAXPDMENU; menuid++) {
				if (pdxmenu[menuid].flag == 0) { 
					EnableMenuItem(hMenu,menuid,MF_ENABLED|MF_BYPOSITION);
					pdxmenu[menuid].flag = 1;
					}
				}
			if (! DrawMenuBar(hWnd))
				check = GetLastError();
			break;
		case WIN_MENU_OFF:
			if (rtobj2int(argv[1],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: menu number Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num -= 1;
			if (pdxmenu[menuid].flag == 1) {
				check = EnableMenuItem(hMenu,item_num,MF_GRAYED|MF_BYPOSITION);
				pdxmenu[menuid].flag = 0;
				if (! DrawMenuBar(hWnd))
					check = GetLastError();
				}
			break;
		case WIN_MENU_ALL_OFF:
			for (menuid = 0; menuid < MAXPDMENU; menuid++) {
				if (pdxmenu[menuid].flag == 1) {
					EnableMenuItem(hMenu,menuid,MF_GRAYED|MF_BYPOSITION);
					pdxmenu[menuid].flag = 0;
					}
				}
			if (! DrawMenuBar(hWnd))
				check = GetLastError();
			break;
		case WIN_MENU_ITEM_ON:
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num--;
			EnableMenuItem(pdxmenu[menuid].hpopmenu,item_num,
			               MF_ENABLED|MF_BYPOSITION);
			break;
		case WIN_MENU_ITEM_OFF:
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num--;
			EnableMenuItem(pdxmenu[menuid].hpopmenu,item_num,
			               MF_GRAYED|MF_BYPOSITION);
			break;
		case WIN_MENU_ITEM_SET:
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			if (rtobj2str(argv[3],val,127) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Set Item Error");
				return CSR_FAIL_ARGTYPE;
				}
			if (argc == 5) {
				if (rtobj2int(argv[4],&key) != RT_SUCCESS) {
					rterrorbox("CSR:win_menu: Item Enable Switch Error");
					return CSR_FAIL_ARGTYPE;
					}
				if (key == 1)
					check = MF_CHECKED;
				else
					check = MF_UNCHECKED;
				}
			else	/* default state is enabled */
				check = MF_ENABLED;
			/* now pick up menu item action */
			pclptr = _get_mem((short)item_num,(byte *)(pdxmenu[menuid].cl+1));
			pclptr += 4;				/* member, cnt, length */
			pclptr += tsize(pclptr);	/* on/off switch */
			pclptr += tsize(pclptr);	/* item prompt, now point to */
			                        	/* pcl procedure to call */
			item_num--;		/* do this after _get_mem() call */
			if (! ModifyMenu(pdxmenu[menuid].hpopmenu,item_num,
			           MF_BYPOSITION|MF_STRING,(UINT)pclptr,val))
			   return(6);
			EnableMenuItem(pdxmenu[menuid].hpopmenu,item_num,
			               MF_BYPOSITION | check);
			break;
		case WIN_MENU_ITEM_CHECK:
			/* this is a simple toggle from the current state */
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num--;
			if (rtobj2int(argv[3],&key) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Set Check Error");
				return CSR_FAIL_ARGTYPE;
				}
			if (key == 1)
				check = MF_CHECKED;
			else
				check = MF_UNCHECKED;
			if (! CheckMenuItem(pdxmenu[menuid].hpopmenu,item_num,
			                    MF_BYPOSITION | check))
				return(7);
			break;
		case WIN_MENU_DROP:
		case WIN_MENU_DELETE:
			if (! DeleteMenu(hMenu,menuid,MF_BYPOSITION))
				return(8);
			DrawMenuBar(hWnd);
			pdxmenu[menuid].hpopmenu = 0;
			pdxmenu[menuid].cl       = NULLCL;
			pdxmenu[menuid].flag     = 0;
			refresh_statusarea();
			break;
		case WIN_MENU_ITEM_MARK:
		case WIN_MENU_ITEM_STYLE:
		case WIN_MENU_ITEM_KEY:
		case WIN_MENU_EXECUTE:
			return(10);		/* not supported */
			break;
#if 0
		case WIN_MENU_ITEM_MARK:
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num--;
			if (rtobj2int(argv[3],&key) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Mark Item Error");
				return CSR_FAIL_ARGTYPE;
				}
			SetItemMark(menuid,item_num,key);
	    	break;
		case WIN_MENU_ITEM_STYLE:
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			if (rtobj2int(argv[3],&key) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Style Item Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num--;
			SetItemStyle (menuid,item_num,key);
			break;
		case WIN_MENU_ITEM_KEY:
			if (rtobj2int(argv[2],&item_num) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Item number Error");
				return CSR_FAIL_ARGTYPE;
				}
			if (rtobj2int(argv[3],&key) != RT_SUCCESS) {
				rterrorbox("CSR:win_menu: Key Item Error");
				return CSR_FAIL_ARGTYPE;
				}
			item_num--;
			SetItemKey(menuid,item_num,key);
			break;
		case WIN_MENU_EXECUTE:
			ReleaseEvent();
			break;
#endif
		case WIN_MENU_HELP:
			if (TRUE == WinHelp(hWnd,"\\pplus\\lib\\rthelp.hlp",HELP_CONTENTS,0L))
				return(RT_SUCCESS);
			else
				return(GetLastError());
			break;
		
		default:
			rterrorbox("CSR:win_menu: Invalid option");
			return(-2);
			break;
		}
	return(0);
}

/**************************/
/* build popup menu items */
/**************************/
static int
make_win_menu (int menuid)
{
	char	prompt[128];
	byte	*mem_ptr, *saveptr;
	byte	mem_cnt;
	field	*fp;
	long	state;
	int		i, onoff;
	short	objlen;
	char	ok;

	for (i = 0; i < pdxmenu[menuid].cl->numitems; i++) {
		mem_ptr = _get_mem((short)i,(byte *)(pdxmenu[menuid].cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;	// type, number of items and length
		if (i == 0) {
			saveptr = mem_ptr;
			if (hMenu == NULL) {
				hMenu = CreateMenu();
				if (hMenu == NULL)
					return(1);
				}
			pdxmenu[menuid].hpopmenu = CreateMenu();
			if (pdxmenu[menuid].hpopmenu == NULL)
				return(1);
			}
		else {
			// -------------------------------------------------
			// Expecting: int_object, string_object, OFB_object
			// -------------------------------------------------
			if (*mem_ptr == FLD) {
				fp = rt_get_fp(mem_ptr,&objlen,NULL);
				if (fp == NULL) {
					rterrorbox("CSR:win_menu: Invalid On/Off");
					return CSR_FAIL_ARGTYPE;
					}
				field2int(fp, &onoff);
				}
			else
				pcl2int(mem_ptr,&onoff,NULL);
			switch (onoff) {
				case 0:
					state = MF_GRAYED;
					break;
				case 1:
					state = MF_ENABLED;
					break;
				case 2:
					state = MF_SEPARATOR;
					break;
				case 3:
					state = MF_MENUBREAK;
					break;
				case 4:
					state = MF_MENUBARBREAK;
					break;
				}
			mem_ptr += tsize(mem_ptr);
			if (state == MF_GRAYED || state == MF_ENABLED) {
				if (*mem_ptr == FLD) {
					fp = rt_get_fp(mem_ptr,&objlen,NULL);
					if (fp == NULL) {
						rterrorbox("CSR:win_menu: Invalid String");
						return CSR_FAIL_ARGTYPE;
						}
					strcpy (prompt,(char *)fp->storage);
					}
				else
					pcl2str(mem_ptr,prompt,120,NULL);
				mem_ptr += tsize(mem_ptr);
				}
			ok = InsertMenu(pdxmenu[menuid].hpopmenu,0xFFFFFFFF,
			                MF_BYPOSITION | state,(UINT)mem_ptr,prompt);
			if (ok == FALSE)
				return(3);
			}
		}
	pcl2str(saveptr,prompt,120,NULL);	/* menu title */
	ok = AppendMenu(hMenu, MF_POPUP,(UINT)(pdxmenu[menuid].hpopmenu),prompt);
	if (ok == FALSE)
		return(3);
	pdxmenu[menuid].flag = 1;
	return(0);		/* all's well */
}
			
void
refresh_statusarea()
{
	int key;
	RECT	wrect;

	/* assume that no menus exist before loop */
	Winmenu_flag &= ~CSR_WINMENU;
	for (key = 0; key < MAXPDMENU; key++) {
		if (pdxmenu[key].hpopmenu != 0)
			Winmenu_flag |= CSR_WINMENU;
			break;
		}
	if (! (Winmenu_flag & CSR_WINMENU)) {
		GetWindowRect(hWnd,&wrect);
		rtwdw_resize(0,- PDMENUHEIGHT);
		if (!MoveWindow(hWnd,wrect.left,wrect.top,
						wrect.right - wrect.left,
						wrect.bottom - wrect.top - PDMENUHEIGHT,TRUE))
		do_PCLredraw();	/* refresh status line as wdw changed size */
		}
}

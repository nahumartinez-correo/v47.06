// ==================================================================
//
// Resource - Header File
//
// Resources in each subdirectory of mosaic OA start
// on a fixed boundary so additions can be made without
// affecting other directories.
//
// Resources are in src/nls/english.rc
// ==================================================================

#ifndef RES_H
#define RES_H

// ==================================================================
//      DIRECTORY STARTING POINTS
// ==================================================================
#define RT_DIR             0
#define MISC_DIR         300

//#define AUXCLI_DIR     500
//#define CBTREE_DIR     650
//#define CDCL_DIR       800
//#define CDS_DIR       1000
//#define CDSV_DIR      1200
//#define CM_DIR        1500
//#define COMM_DIR      1800
//#define CSR_DIR       2100
//#define C_UTILS_DIR   2400
//#define DATAMAN_DIR   2700
//#define DB_DIR        3000
//#define DDSPEC_DIR    3300
//#define DECMATH_DIR   3600
//#define DRIVER_DIR    3900
//#define DSPLIST_DIR   7200
//#define EXPORT_DIR    7500
//#define IMPORT_DIR    7800
//#define MCN_DIR       8100
//#define MSGRTN_DIR    8700
//#define NETBOSS_DIR   9000
//#define OFB_DIR       9300
//#define OLIPR46S_DIR  9600
//#define OLIPR50_DIR   9900
//#define ORA_DIR      10200
//#define QL_DIR       10500

// ==================================================================
//      SYMBOLIC CONSTANTS
// ==================================================================
//rt.c
#define RT_USAGE_TITLE			RT_DIR+1
#define RT_USAGE    			RT_DIR+2
#define RT_USAGE_CDS			RT_DIR+3
#define RT_USAGE_TUV			RT_DIR+4
#define RT_USAGE_W  			RT_DIR+5
#define RT_USAGE_M  			RT_DIR+6
#define RT_USAGE_R  			RT_DIR+7
#define RT_USAGE_cdfhi			RT_DIR+8
#define RT_USAGE_mntwx			RT_DIR+9
#define RT_USAGE_MORE			RT_DIR+10
#define RT_DDOPEN_FAILED		RT_DIR+11
#define RT_MATH_ERR 			RT_DIR+12
#define RT_CRIGHT1  			RT_DIR+13
#define RT_CRIGHT2  			RT_DIR+14
#define RT_INITMSG  			RT_DIR+15
#define RT_IBAE_END_WARN		RT_DIR+16
#define RT_CTL_C_TO_EXIT		RT_DIR+17
#define RT_REQD_FIELD_NOT_FOUND	RT_DIR+18
#define RT_ANY_KEY_TO_CONT		RT_DIR+19
#define RT_REQD_DIR_NOT_FOUND	RT_DIR+20
#define RT_REQD_FILE_NOT_FOUND	RT_DIR+21
#define RT_USAGE_REGSRVR		RT_DIR+22
#define RT_USAGE_UNREGSRVR		RT_DIR+23
#define RT_USAGE_AUTOMATION		RT_DIR+24
//cal.c
#define CAL_NO_FIT			RT_DIR+30
#define CAL_CURRENT_FIELD_VALUE		RT_DIR+31
#define CAL_PRINT_PROMPT		RT_DIR+32
#define CAL_DAY          		RT_DIR+33
#define CAL_WEEK         		RT_DIR+34
#define CAL_MONTH        		RT_DIR+35
#define CAL_YEAR         		RT_DIR+36
#define CAL_OUT_OF_RANGE  		RT_DIR+37
#define CAL_CURRENT_DATE  		RT_DIR+38
#define CAL_INVALID_DATE  		RT_DIR+39
//calc.c
#define CALC_NO_FIT        		RT_DIR+45
#define CALC_TITLE         		RT_DIR+46
#define CALC_PRINT_PROMPT  		RT_DIR+47
#define CALC_TAPE_STORED   		RT_DIR+48
#define CALC_NO_TAPE       		RT_DIR+49
#define CALC_TAPE_RETRIEVED		RT_DIR+50
#define CALC_DIVIDE_BY_ZERO		RT_DIR+51
#define CALC_TAPE_FULL     		RT_DIR+52
#define CALC_NAME          		RT_DIR+53
#define CALC_NAME_C        		RT_DIR+54
#define CALC_OUT_OF_RANGE  		RT_DIR+55
#define CALC_TOTAL_KEY        		RT_DIR+56
#define CALC_MODE_KEY        		RT_DIR+57
#define CALC_GET_KEY        		RT_DIR+58

//datacoll.c
#define DATCOL_NULL_DLIST_PTR  		RT_DIR+60
#define DATCOL_POSTVAL_FAILURE 		RT_DIR+61
#define DATCOL_FIELD_TO_BIG    		RT_DIR+62
#define DATCOL_UNDEF_FIELD_CSR 		RT_DIR+63
//demo.c
#define DEMO_ECHO_WS       		RT_DIR+69
//execprt.c
#define EXPRT_NO_SIGNATURE 		RT_DIR+70
#define EXPRT_ALREADY_OPEN 		RT_DIR+71
#define EXPRT_OPEN_FAILED  		RT_DIR+72
#define EXPRT_AUX_MSG      		RT_DIR+73
#define EXPRT_NO_IBAE_AUX  		RT_DIR+74
#define EXPRT_ABORT        		RT_DIR+75
#define EXPRT_CREATE_FAILED		RT_DIR+76
#define EXPRT_SETABORT_FAILED		RT_DIR+77
#define EXPRT_STARTDOC_FAILED		RT_DIR+78
#define EXPRT_TERMINATED		RT_DIR+79
#define EXPRT_NOP_VERB  		RT_DIR+80
//expr.c
#define EXPR_OVERFLOW			RT_DIR+85
//fldsubs.c
#define FLDSUB_BOGUS_DLIST		RT_DIR+86
#define FLDSUB_ERROR      		RT_DIR+87
//help.c
#define HELP_NOT_ASSIGNED 		RT_DIR+90
#define HELP_NO_LIST      		RT_DIR+91
//io.c
#define IO_NOT_COMPILED   		RT_DIR+100
#define IO_BOGUS          		RT_DIR+101
#define IO_KBD_OVERFLOW   		RT_DIR+102
#define IO_INTERNAL_ERROR 		RT_DIR+103
//manual.c
#define MAN_HELPFILE      		RT_DIR+110
#define MAN_NO_MANUAL    		RT_DIR+111
#define MAN_NOT_COMPATIBLE		RT_DIR+112
#define MAN_INVALID_OP_LIST		RT_DIR+113
#define MAN_LIST_NOT_FOUND 		RT_DIR+114
#define MAN_DEFAULTS_USED  		RT_DIR+115
//menus.c
#define MENUS_INVALID_TYPE	RT_DIR+120
#define MENUS_NO_CSUB     	RT_DIR+121
#define MENUS_NO_ITEM     	RT_DIR+122
#define MENUS_NO_CLIST    	RT_DIR+123
#define MENUS_NO_PMUOBJ   	RT_DIR+124
//pcl.c
#define PCL_COMPILATION_ERROR	RT_DIR+130
#define PCL_UNKNOWN_TOKEN    	RT_DIR+131
#define PCL_FWDREF_ERROR     	RT_DIR+132
#define PCL_BOGUS_TRAN       	RT_DIR+133
#define PCL_BOGUS_ROUTINE    	RT_DIR+134
//pclsubs.c
#define PCL_NO_RESETCOMM	RT_DIR+140
#define PCL_COMBOVER_NO_TAG	RT_DIR+141
#define PCL_COMBOVER_NO_PLIST	RT_DIR+142
#define PCL_NO_COMBOICON     	RT_DIR+143
#define PCL_CANNOT_CONTINUE  	RT_DIR+144
#define PCL_COMBOPICK_NO_TAG 	RT_DIR+145
#define PCL_COMBOPICK_NO_PLIST	RT_DIR+146
#define PCL_RECURSION_WARN    	RT_DIR+147
#define PCL_CALLSTACK_ERROR   	RT_DIR+148
//picklist.c
#define PICK_NULL_LIST_PTR    	RT_DIR+155
#define PICK_BOGUS_CSR        	RT_DIR+156
//ramalloc.c
#define RAM_NO_REQUIRED_DESC  	RT_DIR+160
#define RAM_NO_MALLOC_SPACE   	RT_DIR+161
#define RAM_DEFAULT_BUFSIZE   	RT_DIR+162
#define RAM_NO_BUFSPACE       	RT_DIR+163
//rtwindow.c
#define RTWIN_TITLE                  RT_DIR+170
#define RTWIN_ALREADY_RUNNING        RT_DIR+171
#define RTWIN_CREATE_POPMENU_ERROR   RT_DIR+172
#define RTWIN_PDM_CREATE_ERROR       RT_DIR+173
#define RTWIN_CHANGE_MAXCOLOR        RT_DIR+174
//winmenu.c
#define WINMENU_EXPECTING_PDM        RT_DIR+180
#define WINMENU_NO_HANDLE_INTFLD     RT_DIR+181
#define WINMENU_NO_STATE_INTFLD      RT_DIR+182
#define WINMENU_PDM_LOCATE_ERROR     RT_DIR+183
#define WINMENU_PDM_CREATE_ERROR     RT_DIR+184
#define WINMENU_NO_RT_WINDOWRECT     RT_DIR+185
#define WINMENU_RESIZE_RT_RECT_ERROR RT_DIR+186
#define WINMENU_APPENDMENU_ERROR     RT_DIR+187
#define WINMENU_MENU_NOT_IN_RUNTIME  RT_DIR+188
#define WINMENU_MENUBAR_ERROR        RT_DIR+189
#define WINMENU_INVALID_PDM_ITEM     RT_DIR+190
#define WINMENU_SUBMENU_DELETE_ERROR RT_DIR+191
#define WINMENU_INSERTMENU_ERROR     RT_DIR+192
#define WINMENU_APPEND_PDM_ERROR     RT_DIR+193
#define WINMENU_INVALID_MENU_HANDLE  RT_DIR+194
#define WINMENU_CHANGE_MENUBAR_ERROR RT_DIR+195
#define WINMENU_ITEM_COUNT_NOT_AVAIL RT_DIR+196
#define WINMENU_PDM_NOT_ON_MENUBAR   RT_DIR+197
#define WINMENU_ITEM_NUM_ERROR       RT_DIR+198
#define WINMENU_SET_ITEM_ERROR       RT_DIR+199
#define WINMENU_ITEM_ENA_SW_ERROR    RT_DIR+200
#define WINMENU_INVALID_OPERATION    RT_DIR+201
#define WINMENU_CREATE_REDUNDANT_ERR RT_DIR+202
#define WINMENU_CREATE_MAIN_ERROR    RT_DIR+203
#define WINMENU_APPEND_POPUP_ERROR   RT_DIR+204
#define RTFONTS_NOT_FOUND            RT_DIR+205

//dates.c
#define MISC_DAYNAME_ERROR           MISC_DIR+1
#define MISC_DAYNAME_SUNDAY          MISC_DIR+2
#define MISC_DAYNAME_MONDAY          MISC_DIR+3
#define MISC_DAYNAME_TUESDAY         MISC_DIR+4
#define MISC_DAYNAME_WEDNESDAY       MISC_DIR+5
#define MISC_DAYNAME_THURSDAY        MISC_DIR+6
#define MISC_DAYNAME_FRIDAY          MISC_DIR+7
#define MISC_DAYNAME_SATURDAY        MISC_DIR+8

#define MISC_SHORTDAYNAME_ERROR      MISC_DIR+9
#define MISC_SHORTDAYNAME_SUNDAY     MISC_DIR+10
#define MISC_SHORTDAYNAME_MONDAY     MISC_DIR+11
#define MISC_SHORTDAYNAME_TUESDAY    MISC_DIR+12
#define MISC_SHORTDAYNAME_WEDNESDAY  MISC_DIR+13
#define MISC_SHORTDAYNAME_THURSDAY   MISC_DIR+14
#define MISC_SHORTDAYNAME_FRIDAY     MISC_DIR+15
#define MISC_SHORTDAYNAME_SATURDAY   MISC_DIR+16

#define MISC_MONTHNAME_ERROR         MISC_DIR+17
#define MISC_MONTHNAME_JANUARY       MISC_DIR+18
#define MISC_MONTHNAME_FEBRUARY      MISC_DIR+19
#define MISC_MONTHNAME_MARCH         MISC_DIR+20
#define MISC_MONTHNAME_APRIL         MISC_DIR+21
#define MISC_MONTHNAME_MAY           MISC_DIR+22
#define MISC_MONTHNAME_JUNE          MISC_DIR+23
#define MISC_MONTHNAME_JULY          MISC_DIR+24
#define MISC_MONTHNAME_AUGUST        MISC_DIR+25
#define MISC_MONTHNAME_SEPTEMBER     MISC_DIR+26
#define MISC_MONTHNAME_OCTOBER       MISC_DIR+27
#define MISC_MONTHNAME_NOVEMBER      MISC_DIR+28
#define MISC_MONTHNAME_DECEMBER      MISC_DIR+29

#endif

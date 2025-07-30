/****x* Pendientes de agrupación/DEF:include:drv
* NOMBRE
*   DEF:include:drv
* DESCRIPCION
*   Comportamiento del objeto (obligatorio)
* ENTRADAS
*   Parámetros de entrada de la rutina, sean argumentos o campos que tiene en cuenta
* SALIDAS
*   Campos modificados como resultado o returns cuyos valores sean no triviales
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

/*****************************************************************/
/*                                                               */
/*  Defines for driver and desktop                               */
/*                                                               */
/*****************************************************************/
#define DT_DEBUG 0

#define CORREO_ON          1
#define CORREO_OFF         0
#define CORREO_DEBUG  CORREO_ON

/* include the CSR defintions for PROC_LIST DT_TRAN AND TAG */
#include "csr.h"
/*                                                               */
/* Workarea button numbers                                       */
/*                                                               */
#define CB0		0
#define CB1		1
#define CB2		2
#define CB3		3
#define CB4		4
#define CB5		5
#define CB6		6
#define CB7		7
#define CB8		8
#define CB9		9
#define CB10	10
#define CB11	11
#define CB12	12
#define CB13	13
#define CB14	14
#define CB15	15
#define CB16	16
#define CB17	17
#define CB18	18
#define CB19	19

#define SEND	20
#define ESC		21

/*                                                               */
/* Used by desktop for navigation buttons                        */
/*   NOT FOR USE BY APPLLCATIONS                                 */
/*                                                               */
#define N_UP	22
#define N_TOP	23
#define N_BACK	24

/*                                                               */
/* Side bar button numbers                                       */
/*                                                               */
#define SB0		25
#define SB1		26
#define SB2		27
#define SB3		28
#define SB4		29
#define SB5		30
#define SB6		31
#define SB7		32
#define SB8		33
#define SB9		34

/*                                                               */
/* Used by desktop for utility/function button numbers           */
/*   NOT FOR USE BY APPLLCATIONS                                 */
/*                                                               */
#define UB0		35
#define UB1		36
#define UB2		37
#define UB3		38
#define UB4		39
#define FB0		40
#define FB1		41
#define FB2		42
#define FB3		43
#define FB4		44
#define FB5		45
#define FB6		46
#define FB7		47
#define FB8		48
#define SPB0	49
#define SPB1	50
#define SPB2	51
#define SPB3	52
#define SPB4	53
#define SPB5	54
#define SPB6	55
#define SPB7	56

/*  teller buttons */
#define TFB0	57
#define TFB1	58
#define TFB2	59
#define TFB3	60
#define TFB4	61
#define TFB5	62
#define TFB6	63
#define TFB7	64
#define TFB8	65
#define TFB9	66
#define TFB10	67
#define TFB11	68

/*  loans buttons */
#define LFB0	69
#define LFB1	70
#define LFB2	71
#define LFB3	72
#define LFB4	73
#define LFB5	74
#define LFB6	75
#define LFB7	76
#define LFB8	77
#define LFB9	78
#define LFB10	79
#define LFB11	80

/*  new accounts buttons if we are not using sidebar. You can delete
	this if you are using the sidebar */
#define NFB0	81
#define NFB1	82
#define NFB2	83
#define NFB3	84
#define NFB4	85
#define NFB5	86
#define NFB6	87
#define NFB7	88
#define NFB8	89
#define NFB9	90
#define NFB10	91
#define NFB11	92

/* Sucursal buttons */
#define SFB3	96

/*                                                               */
/* Maximum number of nested lists per application                */
/*                                                               */
/* NOTE: This is also the size of history (back key)             */
/*                                                               */
#define STACK_SIZE	10
#define PREV_APP_STACK_SIZE	100

/*                                                               */
/* Turns watch on or off and does someother keyboard cleanup     */
/*                                                               */
#define WATCH_ON   CALL ofb:drv:watch_on
#define WATCH_OFF  CALL ofb:drv:watch_off

/*                                                               */
/* Used to specify function for CSR:tag                          */
/*                                                               */
/*                                                               */
/* Used to specify function for CSR:fmt                          */
/*                                                               */
#define FMT_TO_PIC 		1
#define FMT_TO_FILE	 	2
#define FMT_TO_FIELD	3

/*                                                               */
/* Used to specify function for CSR:proc_list                    */
/*                                                               */
/*                                                               */
/* Used to specify function for CSR:win_menu                     */
/*                                                               */
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
#define WIN_MENU_ITEM_KEY	34

#define WIN_MENU_ALL_ON 	10
#define WIN_MENU_ALL_OFF	11
#define WIN_MENU_EXECUTE 	12


/*                                                               */
/* Used by driver to parse header                                */
/*                                                               */
#define END_TRAN   SUBSTR(header,4,1)
/* gpb:Reservo la ultima posicion para quitar/agregar   */
/* la pantalla drv:desktop_toolbar si se define         */
/* el caracter "R" en el segundo MEMBER de una LISTTRAN */
/*#define NAME       SUBSTR(header,9,20)*/
#define NAME       SUBSTR(header,9,19)
#define START_TRAN SUBSTR(header,3,1)
#define CUT_SCREEN SUBSTR(header,0,1)
#define CUT_SIDE   SUBSTR(header,1,1)
#define DIS_SIDE   SUBSTR(header,2,1)
#define DRV_PAGE_UP F18


#include "desktop.h"
#include "field.h"
#define CB20	20
#define CB21	21

/*******/

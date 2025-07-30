/****x* Pendientes de agrupación/DEF:includes:cl
* NOMBRE
*   DEF:includes:cl
* DESCRIPCION
*   Comportamiento del objeto (obligatorio)
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

#include "base.h"
#include "color.h"

/**********************/
/* Screen positioning */
/**********************/
#define CL_X 0
#define CL_Y 45
#define CL_W 640
#define CL_H 360

/*********************************************************/
/* POPUP border type,color,width.                        */
/*********************************************************/
#define CL_POP_BORD_COLOR 30
#define CL_POP_WIDTH 4
#define CL_POP_BORD 6,CL_POP_BORD_COLOR,CL_POP_WIDTH

/*********************************************************/
/* OK and CANCEL button x,y /width height and desc       */
/*********************************************************/
#define CL_OK_PB 72,18,0,1,0,14,1,DSC:common:ok_dsc
#define CL_CANCEL_PB 72,18,0,1,0,14,1,DSC:common:cancel_dsc
#define CL_DUP_PB 72,18,0,1,0,14,1,DSC:common:dup_dsc
#define CL_SELECT_PB 72,18,0,1,0,14,1,DSC:common:select_dsc
#define CL_GRAPH_PB 72,18,0,1,0,14,1,DSC:common:graph_dsc
#define CL_PRINT_PB 72,18,0,1,0,14,1,DSC:common:print_dsc
#define CL_DELETE_PB 72,18,0,1,0,14,1,DSC:common:delete_dsc
#define CL_CALCULATE_PB 72,18,0,1,0,14,1,DSC:common:calculate_dsc
#define CL_EXIT_PB 72,18,0,1,0,14,1,DSC:common:exit_dsc
#define CL_POP_BORD 6,CL_POP_BORD_COLOR,CL_POP_WIDTH

/*****************************************************/
/* These defines are used to tailor parts of the     */
/* Consumer loans system. There are also .h files    */
/* in the different sub directories which can tailor */
/* different function which are less global than     */
/* these.                                            */
/*      .                                            */
/* 0 = DO NOT INSTALL THIS FUNCTION                  */
/* 1 = INSTALL THIS FUNCTION                         */
/*****************************************************/
#define SPEED_KEY 1
#define QUICK_KEY 1
#define CROSS_SELL 1

/********************************************/
/* SALES SCREENS - If SELECTION = 1 then    */
/* after last sales screen. A multi display */
/* of the loan types is displayed. If a loan*/
/* type is entered flow goes to the CREATE  */
/* Loan Tran. with loan type prefilled.     */
/* Otherwise it returns from were it came.  */
/* If SELECTION = 0 then after last sales   */
/* screen flow return from were it came.    */
/********************************************/
#define SELECTION 1

/********************************************/
/* WARNING INCREASING FIELD SIZES may cause */
/* Screen to display funny. ie. Field larger*/
/* than the screen or overlapping fields    */
/********************************************/

/********************************************/
/* APPLICATION NUMBER - Sets the size and   */
/* type of the application number used in   */
/* The Consumer Loans system.               */
/********************************************/
#define APPL_NBR_SIZE  8
#define APPL_NBR_TYPE_NUMERIC 1

/********************************************/
/* LOGIN ID'S         - Sets the size       */
/*      of the login_id operator number     */
/* branch number                            */
/********************************************/
#define LOGIN_ID_SIZE 8

/********************************************/
/* LOAN TYPE          - Sets the size       */
/* of the Loan class, sub class ,type used  */
/* in The Consumer Loans system.            */
/***** LTYPE_SIZE = LC_SIZE + LC_SUB_SIZE   */
/********************************************/
#define LC_SIZE  2
#define LC_SUB_SIZE 2
#define LTYPE_SIZE  4
#define LC_POSITION 100
/* IF LC_SUB_SIZE = 1 LC_POSITION = 10 */
/* IF LC_SUB_SIZE = 2 LC_POSITION = 100 */
/* IF LC_SUB_SIZE = 3 LC_POSITION = 1000 */
/* ect                                    */

/* These defines are used in tracking and ques */
/* They relate directly to the order of the PICKLIST */
/* #PICKcl_trk_que                             */
#define TRACKING                0
#define DEC                             1
#define DIS                             2
#define DL                              3
#define PC                              4
#define NOTE_PAD                5
#define APPL                    6
#define CREDIT_REPORT   7
#define DEBT_RATIO              8
#define CREDIT_SCORE    9
#define EMPLOYMENT              10
#define DEPOSIT                 11
#define INSURANCE               12
#define APPRAISAL               13
#define TITLE_REPORT    14


/* These defines are used with the custom "C" routine */
/* CALL CSR:min_max                                   */
#define RET_MIN 0
#define RET_MAX 1



/*****************************************************/
/* SPECIAL FOR ARKANSAS - Payment calculator         */
/*      .                                            */
/* 0 = NOT ARKANSAS                                  */
/* 1 = ARKANSAS                                      */
/*****************************************************/
#define ARKANSAS 0
#include "field.h"

/*******/

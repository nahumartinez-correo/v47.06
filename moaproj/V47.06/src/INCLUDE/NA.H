/****x* Pendientes de agrupación/DEF:include:NA
* NOMBRE
*   DEF:include:NA
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

; The following defines are for New Accounts custom transaction names
#define CIF_HOST_NAME         ofb:na:TRANcif_host
#define CIF_CREATE_NAME       ofb:na:TRANcif_create
#define CIF_LOCAL_NAME        ofb:na:TRANcif_local


/*
* Values used for the CIFsession_list multidisplay on the Title
* Setup Screen.
*/
#define CIF_MULTI_TOP         97
#define CIF_MULTI_BOT         257
#define CIF_MULTI_SHIFT1      63
#define CIF_MULTI_SHIFT2      36

/* The following define is for transaction status.  Hostcomm tran */
/* status defines are maintained in hcomm_std.h.  The "pending"   */
/* status, however, is used only for New Accounts and is not      */
/* related to a host response, so does not belong in the          */
/* hcomm_std.h file.  The value for PENDING should not match any  */
/* of the status defines in the hcomm_std.h file.                 */

#define PENDING       200



;The following define is for the field name which is to hold the
;customer name in the CIF table.

#define CIF_NAME_FIELD			pers_info_name

/*******/

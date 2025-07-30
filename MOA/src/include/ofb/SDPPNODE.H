/****x* Pendientes de agrupación/DEF:include:sdppnode
* NOMBRE
*   DEF:include:sdppnode
* DESCRIPCION
*	sdpnode.h - defines to be used with pnode submit concept
*	(c) by ISC-BUNKER RAMO Systems Corp., 1991      All rights reserved.
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

/* revision system control information */
/*  $Header: X:/moaproj/demo/src/include/RCS/sdppnode.h 1.1 1994/10/25 16:49:23 nagesh Exp $ */

/* define the codes for the different type of data being passed from */
/* the application.                                                  */

#define		SDP_SUB_REQ		   "$SDP$"
#define		SRC_PATH_NAME		"A"		/* 255 */
#define		DST_PATH_NAME		"B"		/* 255 */
#define		TMP_PATH_NAME		"C"     /* 255 */
#define		FILE_TYPE			"D"		/* 8   */
#define		MAIL_TYPE			"E"		/* 8   */
#define		QUE_PRIORITY		"F"		/* 8   */
#define		DST_NODE			"G"		/* 16  */
#define		SRC_USER_ID			"H"		/* 16  */
#define		SRC_SYSTEM_ID		"I"		/* 16  */
#define		SRC_RESRC_ID		"J"		/* 24  */
#define		PROCESS_PDS			"K"		/* 255 */
#define		NEWNAME				"L"		/* 8   */
#define		DST_USER_ID			"M"		/* 16  */
#define		DST_SYSTEM_ID		"N"		/* 16  */
#define		DST_RESRC_ID		"O"		/* 24  */
#define		PROCESS_NAME		"P"		/* 8   */

/*******/

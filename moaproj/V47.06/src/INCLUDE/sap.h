/****x* Pendientes de agrupaci�n/DEF:include:sap
* NOMBRE
*   DEF:include:sap
* DESCRIPCION
*   Comportamiento del objeto (obligatorio)
* ENTRADAS
*   Par�metros de entrada de la rutina, sean argumentos o campos que tiene en cuenta
* SALIDAS
*   Campos modificados como resultado o returns cuyos valores sean no triviales
* IMPACTOS
*   Otros objetos que deber�n ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentaci�n relevante que deber� ser actualizada por la modificaci�n de la rutina
* SOURCE
*/

#define	COMPILA_SAP

#define SAP_ON		"C"
#define SAP_OFF		"D"

#define ERRSAP_TIMEOUT	-100

#define SAP_CANT_MAX_COMP	20
#define SAP_CANT_MAX_MPAG	30

/***************************************************************************/
/* El siguiente define indica la cantidad maxima de elemenentos que pueden */
/* ser recibidos mediante la funcion de validacion de Solicitante y la de  */
/* Destinatario.                                                           */
/* CUIDADO!!: De cambiar el valor de este define deberan ser recompiladas  */
/*            la  rutina  FVAL de la aplicacion "post" (ver otras)         */
/***************************************************************** FGB *****/
#define SAP_MAX_ELEM_RCV	999


/*******/

/****x* Pendientes de agrupación/DEF:include:LIB
* NOMBRE
*   DEF:include:LIB
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

/*--------------------------------------------------------------*/
/* Main Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name for copyright bit map */
#define LIB_SPEC_MENU		spec_tran_menu
/* specify title for the special transactions menu */
#define LIB_SPEC_TTL_MENU	"SPECIAL TRANSACTIONS MENU"
/*--------------------------------------------------------------*/
/* Utility Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name for the custom global keys routine */
#define LIB_UTIL_KEYS		KEYScustom_global
/*--------------------------------------------------------------*/
/* Help Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name for options help pick list */
#define LIB_HELP_PICK		help_options
/*--------------------------------------------------------------*/
/* Mag Stripe Module Tailoring									*/
/*--------------------------------------------------------------*/
/* specify mag stripe read commands for tracks 1, 2, and 3 */
#define LIB_MAGS_MRDTR1		K1		/* track 1 cmd (K1,K1N,M1,M1N) */
#define LIB_MAGS_MRDTR2		K2		/* track 2 cmd (K2,K2N,M2,M2N) */
#define LIB_MAGS_MRDTR3		K3		/* track 3 cmd (K3,K3N,M3,M3N) */
/* specify mag stripe read timeout in seconds */
#define LIB_MAGS_MRDTMR		0		/* mag timeout (secs, 0 = none) */
#include "field.h"

/*******/

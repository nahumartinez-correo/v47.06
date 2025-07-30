/****x* Pendientes de agrupación/DEF:include:TMPLT
* NOMBRE
*   DEF:include:TMPLT
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

/* Total list element defs */
#define SRC_ELEMENT    0
#define ADDSUB_ELEMENT 1
#define BUCKET_ELEMENT 2
#define MOD_ELEMENT    3
#define CORR_ELEMENT   4

/* Array item defs */
#define SRC_ARRAY    0
#define BUCKET_ARRAY 1
#define MOD_ARRAY    2
#define CORR_ARRAY   3
#define ADDSUB_ARRAY 4

#define MAX_MEMBERS    128

#define MAX_DBFM_MEMBERS 150
#include "field.h"

/*******/

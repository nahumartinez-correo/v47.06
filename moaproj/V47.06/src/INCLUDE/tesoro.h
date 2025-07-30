/****x* Pendientes de agrupación/DEF:include:tesoro
* NOMBRE
*   DEF:include:tesoro
* DESCRIPCION
*   Defines para CAJAS y TESORO
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


#define ENTRADA     "+"
#define SALIDA      "-"
#define NO_AFECTA   "N"
#define NADA        ""

#ifndef TOTAL_TESO
#define TOTAL_TESO "** TOTAL **"
#endif

#define IDX_MONEDA 39
#define OPERADOR_LIST   1
#define SUCURSAL    0

/* Cuando se define una nueva transaccion se agrega un
   un nuevo total y este numero aumenta en uno */
#define IDX_TOTAL 196
#define IDX_ARRAY 197

#define TODOS                  0
#define ITEMIZADO              1
#define GLOBAL                 2
#define GLOBAL_NO_MONETARIO    3
#define GLOBAL_MONETARIO       4
#define ITEMIZADO_BANCARIO     5
#define ITEMIZADO_NO_MONETARIO 6
#define AGREGA_SI              0
#define AGREGA_NO              1

#define MP_ARRAY      40

#define MAX_DEN 14
#define DETERIO     "DETERIOR"
#define EFECTIVO 0
#define EFECTIVO_INICIAL 1
#define ENTRADA_CAJA 2
#define SAL_CAJA 3
#define EFVO_SAL 4
#define EFVO_ENT 5
#define ENT_BASE 6
#define SAL_BASE 7
#define TX_CORREO 8
#define USO_INT 9
#define TIPO_ARRAY 10

#define TOT_CORREO  200
#define TOT_INTERNO  200
#define ENT_INVERSA "E"
#define SAL_INVERSA "S"

#define INICIAL			    0
#define CONSULTA_MOTIVOS    1
#define	RENDICION_ESPECIES  2
#define SALIDA_CAJA		    4
#define CIERRE_LOTE			3
/* Se comenta xq no se usa mas #define FALLO_CAJA_POSITIVO 4 */
#define FALLO_CAJA_NEGATIVO 5
#define SALIDA_VALORES      6
#define CONSULTA_TOTALES	7
#define CONSULTA_FRANQUEADORAS 8
#define CIERRE_CAJA			9

/* El ultimo es uno menos porque va desde 0*/
#define IDX_USOINT 162 //cgw: Las posiciones 125 y 126 (anteriormente trx's 6660 y 6860) estan disponibles para reutilizar.

/*Usado para Compromisos de Pago*/
#define CONCEPTO	1
#define ITEM		2
#define SUBITEM		3

#define SUMA_ENVIO 1
#define RESTA_ENVIO 2
#define SALIDA_SUCURSAL     "A"

#define DIRECTA		"D"
#define REVERSA		"R"

/*******/

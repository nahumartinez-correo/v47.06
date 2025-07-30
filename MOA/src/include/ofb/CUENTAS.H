/****x* Pendientes de agrupación/DEF:include:CUENTAS
* NOMBRE
*   DEF:include:CUENTAS
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
# ifndef	CUENTAS_H
# define	CUENTAS_H

/* Tipos de Cuenta */
#define	CC_NORMAL	10101
#define	AC_NORMAL	10201

/* Monedas */
#define	MON_PESOS	0
#define	MON_DOLARES_L	1
#define	MON_DOLARES	2

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif

/* Tiempo de Los Pedidos */
#define	DIAS_PEDIDO_BOLETAS	5
#define	DIAS_PEDIDO_CHEQUERAS	5
#define	DIAS_PEDIDO_EXTRACTO	2

#define CANT_CHEQUE     100
#define NUEVA           "N"
#define EN_USO          "U"
#define TERMINADA       "T"
#define MAX_CHEQUE      99999999

#define CREDITO         "C"
#define DEBITO          "D"
#define NO_AFECTA_SALDO "N"

#define ANULADO "A"
#define ORDEN_NO_PAGAR "N"
#define EMITIDO "E"
#define SIN_EMITIR " "

# endif


/*******/

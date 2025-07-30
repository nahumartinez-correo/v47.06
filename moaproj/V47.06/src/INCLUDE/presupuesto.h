/****x* Pendientes de agrupación/DEF:include:presupuesto
* NOMBRE
*   DEF:include:presupuesto
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
#define RECHAZADO 		3
#define ACEPTADO  		2
#define SCONFIRMACION 	1
#define ERROR_BORRADO         "No es posible Borrar una Transaccion Aceptada!!!"
#define ERROR_MODIFICADO      "No es posible Modificar una Transaccion Aceptada!!!"
#define MODO_PRESUPUESTO(x)    (( x == 2) || (x == 3) || (x == 4))
#define  IndiceMaximo  5
#define TotalMedioPermitidos 6


/* Reingenieria                         */
/* para la captura de medios de pago... */
/*                                      */
#define PRIMER_PASADA		1
#define SEGUNDA_PASADA		2
#define CORRECCION			3
#define ANULACION			4

/* Reingenieria                               */
/* para la nueva (i)logica del presupuesto... */
/*                                            */
#define BUDGET_STARTED	"I"  /* Inicio presupuesto */
#define CLOSING_BUDGET	"C" /* Cerrando presupuesto */
#define BUDGET_CLOSED	"F" /* presupuesto Finalizado */
#define CONFIRMATION	"E" /* Proceso de Efectivizacion */
#define FORM_PRINTING	"P" /* imPresion form fiscal - fin de sesion */
#define READYTOSTART	"R" /* listo para comenzaR */ 
#define RECOVERY	"Y" /* se caYo el sistema */

/* Esta transaccion marca el fin del presupuesto */
#define END_SESSION_TX	LST:post:TRANimpre_fiscal

/* Esta pantalla se utiliza para la confirmacion del presupuesto */
#define SCR_CONFIRMA 	SCR:plataf:SCRNproc_confirma

/* Cancela presupuesto - Captura */
#define BUDGET_CANCEL	"L"

#define SESION 1
#define PROFORMA 2

/*******/

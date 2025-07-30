/****x* Pendientes de agrupación/DEF:include:giros
* NOMBRE
*   DEF:include:giros
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

#define GIROS "SI"
#define CH_BAJA "B"
#define CH_VENCIDO "V"
#define CH_RECEPCIONADO "E"
#define CH_RECHAZADO "R"
#define CH_ACREDITADO "A"
#define CH_EMITIDO "X"
#define NO_ES_REVERSA 1
#define    MAX_VEC_OPC  6

/****************************************************************/
/* PARAMETROS DE LA RUTINA OFB:giros:CDSsval_chkpoint           */
/* Los siguientes son los valores utilizados para recuperar el  */
/* estado donde quedo la emision de un giro al momento de caer_ */
/* se la aplicacion mientras se confirmaba la transaccion.      */
/****************************************** FGB 13.6.2001 *******/
#define	ANTES_JNL		"J"
#define	ANTES_ANULACION 	"A"
#define ANTES_IMPRESION 	"I"
#define ANTES_TOTAL_TRANSAC_1	"T"
#define ANTES_TOTAL_TRANSAC_2	"O"
#define ANTES_PRESUDET		"P"
#define ANTES_IMPRE_SUB_1 	"1"
#define ANTES_IMPRE_SUB_2 	"2"
#define ANTES_IMPRE_SUB_3 	"3"
#define ANTES_IMPRE_SUB_4 	"4"
#define ANTES_IMPRE_SUB_5 	"5"
#define ANTES_IMPRE_SUB_6 	"6"
#define ANTES_IMPRE_SUB_7 	"7"

/*Importe máximo permitido en una Transferencia Internacional*/
#define MAX_INTER_LIMITE  10000

/* Descripcion que identifica las cancelaciones por ruptura de cheque" */
#define DSCERR_PRT_CHQ		"PRTERROR"

/*Giros Masivos*/
#define GM_SOBRANTE "SOBRANTE"
#define GM_FALTANTE "FALTANTE"
#define GM_FACTURA "LOTE_A_FACTURAR"
#define GM_NO_EMI  "LOTE_NO_EMITIDO"
#define GM_LOTE    "LOTE GIROS MASIVOS"

/*Giros Masivos*/
#define GM_EMITIDO  "EM"
#define GM_ANULADO  "AN"

// Transaccion de Giros Masivos JNL.
#define JNL_TRX     "U"

/* Listados de Giros Emitidos Y Pagados*/
#define DSC_EMI	"E"
#define DSC_PAG "P"

/* Tiempo Maximo (arbitrario) considerado para que un registro este */
/* bloqueado para ser recuperado ante una caida en vuelo            */
/* El tiempo es expresado en hhmmss (ej: 00h 30m 00s = "003000")    */
#define FREE_LOCKED_REG_TIME	"003000"

/*Giros Masivos*/
#define GM_SIN_SESION		"I"

#define SET_MARCA_REV     1
#define RESET_MARCA_REV   0

/* usados para la TotalTransac */
#define SOLOSUMA "S"
#define SOLORESTA "T"
#define SUMADISCRIMINADO "D"
#define SUMARESTA "R"

/*******/

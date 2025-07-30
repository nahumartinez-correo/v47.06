/****x* Pendientes de agrupación/DEF:include:HCOMMSTD
* NOMBRE
*   DEF:include:HCOMMSTD
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
/* This file contains miscellaneous standard defines for the host comm    */
/*  PCL scripts.                                                          */


/* The following defines are used for the hostcomm status's.              */
/* A one character number is used to identify the status.                 */

/* Accepted status's have a range of 1 - 99 */

#define UNPOSTED          1       /* unposted transaction */
#define MEMO              2       /* memo transaction */
#define HOST_ACCEPTED     3       /* host accepted transaction */

#define BULL_NO_HOST     98       /* transaccion que volvio ok del bull y
             rechazada del host (utilizado en el TDRVprocess_bull) Bettina */
#define CANCEL_REENVIE    99       /* Debe reversar la transaccion */

/* Rejected status's have a range of 100-199 */

#define HOST_REJECTED     100     /* host rejected transaction */

#define HC_ERR_BEGIN      101     /* start of error range */
#define SEND_ERROR        101     /* error in sending to host  */
#define SEND_RESET        102     /* reset while sending to host */
#define RCV_RESET         103     /* reset while receiving */
#define RCV_ERROR         104     /* error in receiving from host */ 
#define HOST_RCV_DOWN     105     /* host went down during receive */
#define SESSION_LOST      106     /* session was lost to host */
#define HOST_NEG_RESP     107     /* negative response from host */
#define HOST_COMMSRVR_DOWN 108    /* comm server went down (aci) */
                                  /* insert new error defines here */
#define ENVIADA_CONSULTA  110     /* Respuesta de Transac. Enviada a Consulta */
#define HC_ERR_END        199     /* end of error range */


/* The following defines are used for pinnacle plus service requests */
/* such as, set host mode all, set nohost mode all.                  */

/* mosaic service request codes */
#define SET_HOSTMODE_ALL         "100"       
#define SET_NOHOSTMODE_ALL       "101"
#define SET_HOSTMODE_WS          "102"
#define SET_NOHOSTMODE_WS        "103"
#define MSG_BROADCAST			 "104"
#define MSG_OPERATOR			 "105"
#define REM_OVR_REQ				 "106"
#define REM_OVR_RESP			 "107"
#define REM_OVR_CANCEL			 "108"
#define REM_OVR_ACKN			 "109"
#define MSG_WORKSTATION			 "110"
#define MSG_CONFIG     			 "111"
#define CB_REQUEST_NOW           "200"
#define CB_REQUEST_QUE           "201"
#define CB_STATUS                "202"
#define CB_STATUS_RCV            "203"
#define REENTRY_BEG              "300"
#define REENTRY_CMPL             "301"
#define START_REENTRY_REQ        "302"
#define REN_UPT_FLAGS            "303"
#define HALT_REENTRY             "304"

#define PPLUS_RSP   "OK"
#define MOA_RSP PPLUS_RSP
#define PPLUS_ERR	"ERR"
#define MOA_ERR PPLUS_ERR

#define OK      1
#define NOTOK   0

#define DEFAULT_WAIT_TIME 15
#define OVR_WAIT_TIME 240

/* Para Giros */
#define TRAN_CAIDA_VUELO -5
#define RECH_REV_AUTO 111

/*******/

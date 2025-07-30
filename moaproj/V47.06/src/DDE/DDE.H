/****x* Pendientes de agrupación/DEF:include:DDE_DDE
* NOMBRE
*   DEF:include:dde
* DESCRIPCION
*  ddeml.h -    Header file to contain constants used by  PCL proc while
*               dealing with DDE                                        
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

;*****************************************************************************\
;*                                                                             *
;*  ddeml.h -    Header file to contain constants used by  PCL proc while 	  *
;*               dealing with DDE                                              *
;*                                                                             *
;*****************************************************************************/

;******** public types ********/

#define DDE_FBUSY               0x4000
#define DDE_FDEFERUPD           0x4000
#define DDE_FACKREQ             0x8000
#define DDE_FRELEASE            0x2000
#define DDE_FREQUESTED          0x1000
#define DDE_FAPPSTATUS          0x00ff
#define DDE_FNOTPROCESSED       0x0000

#define DDE_FACKRESERVED        (~(DDE_FACK | DDE_FBUSY | DDE_FAPPSTATUS))
#define DDE_FADVRESERVED        (~(DDE_FACKREQ | DDE_FDEFERUPD))
#define DDE_FDATRESERVED        (~(DDE_FACKREQ | DDE_FRELEASE | DDE_FREQUESTED))
#define DDE_FPOKRESERVED        (~(DDE_FRELEASE))

;***** message filter hook types *****/

#define     MSGF_DDEMGR             0x8001

;***** codepage constants ****/

#define CP_WINANSI      1004    /* default codepage for windows & old DDE convs. */
#define CP_WINUNICODE   1200

;***** transaction types *****/

#define     XTYPF_NOBLOCK            0x0002  /* CBR_BLOCK will not work */
#define     XTYPF_NODATA             0x0004  /* DDE_FDEFERUPD */
#define     XTYPF_ACKREQ             0x0008  /* DDE_FACKREQ */

#define     XCLASS_MASK              0xFC00
#define     XCLASS_BOOL              0x1000
#define     XCLASS_DATA              0x2000
#define     XCLASS_FLAGS             0x4000
#define     XCLASS_NOTIFICATION      0x8000

#define     XTYP_ERROR               0x8002
#define     XTYP_ADVDATA             0x4010
#define     XTYP_ADVREQ              0x2022
#define     XTYP_ADVSTART            0x1030
#define     XTYP_ADVSTOP             0x8040
#define     XTYP_EXECUTE             0x4050
#define     XTYP_CONNECT             0x1062
#define     XTYP_CONNECT_CONFIRM     0x8072
#define     XTYP_XACT_COMPLETE       0x8080
#define     XTYP_POKE                0x4090
#define     XTYP_REGISTER            0x80A2
#define     XTYP_REQUEST             0x20B0
#define     XTYP_DISCONNECT          0x80C2
#define     XTYP_UNREGISTER          0x80D2
#define     XTYP_WILDCONNECT         0x20E2

#define     XTYP_MASK                0x00F0
#define     XTYP_SHIFT               4  /* shift to turn XTYP_ into an index */

;***** Timeout constants *****/

#define     TIMEOUT_ASYNC           0xFFFFFFFF

;***** Transaction ID constants *****/

#define     QID_SYNC                0xFFFFFFFF

;***** public strings used in DDE *****/


;**
; * Callback filter flags for use with standard apps.
; */

#define     CBF_FAIL_SELFCONNECTIONS     0x00001000
#define     CBF_FAIL_CONNECTIONS         0x00002000
#define     CBF_FAIL_ADVISES             0x00004000
#define     CBF_FAIL_EXECUTES            0x00008000
#define     CBF_FAIL_POKES               0x00010000
#define     CBF_FAIL_REQUESTS            0x00020000
#define     CBF_FAIL_ALLSVRXACTIONS      0x0003f000

#define     CBF_SKIP_CONNECT_CONFIRMS    0x00040000
#define     CBF_SKIP_REGISTRATIONS       0x00080000
#define     CBF_SKIP_UNREGISTRATIONS     0x00100000
#define     CBF_SKIP_DISCONNECTS         0x00200000
#define     CBF_SKIP_ALLNOTIFICATIONS    0x003c0000

;/
; * Application command flags
; */
; **
;
;#define     APPCMD_CLIENTONLY            0x00000010L
;#define     APPCMD_FILTERINITS           0x00000020L
;#define     APPCMD_MASK                  0x00000FF0L
;*/

#define     APPCMD_CLIENTONLY            0x00000010
#define     APPCMD_FILTERINITS           0x00000020
#define     APPCMD_MASK                  0x00000FF0

;**
; * Application classification flags
; */
;**
;#define     APPCLASS_STANDARD            0x00000000L
;#define     APPCLASS_MASK                0x0000000FL
;*/
#define     APPCLASS_STANDARD            0x00000000
#define     APPCLASS_MASK                0x0000000F




#define DNS_REGISTER        0x0001
#define DNS_UNREGISTER      0x0002
#define DNS_FILTERON        0x0004
#define DNS_FILTEROFF       0x0008

;*
; *data transfer functions
; */

#define     HDATA_APPOWNED          0x0001



#define     DMLERR_NO_ERROR                    0       ;** must be 0 */

#define     DMLERR_FIRST                       0x4000

#define     DMLERR_ADVACKTIMEOUT               0x4000
#define     DMLERR_BUSY                        0x4001
#define     DMLERR_DATAACKTIMEOUT              0x4002
#define     DMLERR_DLL_NOT_INITIALIZED         0x4003
#define     DMLERR_DLL_USAGE                   0x4004
#define     DMLERR_EXECACKTIMEOUT              0x4005
#define     DMLERR_INVALIDPARAMETER            0x4006
#define     DMLERR_LOW_MEMORY                  0x4007
#define     DMLERR_MEMORY_ERROR                0x4008
#define     DMLERR_NOTPROCESSED                0x4009
#define     DMLERR_NO_CONV_ESTABLISHED         0x400a
#define     DMLERR_POKEACKTIMEOUT              0x400b
#define     DMLERR_POSTMSG_FAILED              0x400c
#define     DMLERR_REENTRANCY                  0x400d
#define     DMLERR_SERVER_DIED                 0x400e
#define     DMLERR_SYS_ERROR                   0x400f
#define     DMLERR_UNADVACKTIMEOUT             0x4010
#define     DMLERR_UNFOUND_QUEUE_ID            0x4011

#define     MF_SENDMSGS                  0x02000000
#define     MF_POSTMSGS                  0x04000000
#define     MF_CALLBACKS                 0x08000000
#define     MF_ERRORS                    0x10000000
#define     MF_LINKS                     0x20000000
#define     MF_CONV                      0x40000000

#define     MF_MASK                      0xFF000000

/*******/



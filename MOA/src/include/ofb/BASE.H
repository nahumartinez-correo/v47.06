/****x* Pendientes de agrupación/DEF:includes:base
* NOMBRE
*   DEF:includes:base
* DESCRIPCION
* 	The following defines are for PCL object types               
*  	These may be used when testing for the object type contained 
*  	in an indirect object.                                       
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

   /********************  WARNING  ****************************/
   /* The "XXX_TYPE" defines MUST match the values for the    */
   /* "XXX" defines in /moa/src/include/dataman.h       */
   /***********************************************************/

#define SCR_TYPE      0x0F
#define OFB_TYPE      0x0A
#define LST_TYPE      0x06

/*  The following defines are for fyi message and error message  */
/*  keyaction codes.                                             */

#define NOWAIT_EXPLICIT_CLR   0
#define NOWAIT_KEY_CLR        1
#define WAIT_DISCARD_KEY      2
#define WAIT_ACT_ON_KEY       3

/*  The following defines are for return values, generally useable */
/*  throughout the system.                                         */

#define RET_SUCCESS           0
#define RET_FAILURE          -1   /* Error status, not handled */
#define RET_ERROR            -2   /* Error status, handled by called routine */
#define RET_FOUND             1
#define RET_NOTFOUND          2

/* The following define is for transaction status.  Hostcomm tran */
/* status defines are maintained in hcomm_std.h.  The "pending"   */
/* status, however, is not related to a host response, so does    */
/* not belong in the hcomm_std.h file.  The value for PENDING     */
/* should not match any of the status defines in the hcomm_std.h  */
/* file.                                                          */

#define PENDING       200

/*--------------------------------------------------------------*/
/* Global Pattern Tailoring                                                                             */
/*--------------------------------------------------------------*/
/* specify date edit pattern */
#define BASE_DATE_PAT   dd/mm/yyyy
#define BASE_OLD_DATE   dd/mm/yy
/*--------------------------------------------------------------*/
/* Global Field Tailoring                                                                               */
/*--------------------------------------------------------------*/
/* specify type, length, and pattern for branch_id field */
#define BASE_BRID_TYPE  ASCII
#define BASE_BRID_LEN   5
/* este deja de usarse .MARTIN */
#define BASE_BRID_PAT   lib:zerosig
/* specify type, length, and pattern for operator_id field */
#define BASE_OPID_TYPE  NUMERIC
#define BASE_OPID_LEN   5
#define BASE_OPID_PAT   lib:zerosig
/*--------------------------------------------------------------*/
/* Global Configuration Tailoring                                                               */
/*--------------------------------------------------------------*/
/* specify whether various applications are present */
#define BASE_CFIG_CL    0               /* 0=absent, 1=present */
#define BASE_CFIG_CB    0               /* 0=absent, 1=present */
#define BASE_CFIG_DDS   0               /* 0=absent, 1=present */
#define BASE_CFIG_NA    0               /* 0=absent, 1=present */
#define BASE_CFIG_NA_CALC       0       /* 0=absent, 1=present */
#define BASE_CFIG_QUE   0               /* 0=absent, 1=present */
#define BASE_CFIG_TLR   1               /* 0=absent, 1=present */
#define BASE_CFIG_BATCH 0               /* 0=absent, 1=present */
#define BASE_CFIG_COEX  0               /* 0=absent, 1=present */
#define BASE_CFIG_OVR   1               /* 0=absent, 1=present */
#define BASE_CFIG_CTR   0               /* 0=absent, 1=present */
#define BASE_CFIG_XSL   0               /* 0=absent, 1=present */
#define BASE_CFIG_NAUSER   1               /* 0=absent, 1=present */
#define BASE_CFIG_BIO   0               /* 0=absent, 1=present */
#define BASE_CFIG_HIST  0               /* 0=absent, 1=present */
#define BASE_CFIG_FF    0               /* 0=absent, 1=present */
#define BASE_CFIG_GWS   0               /* 0=absent, 1=present */
#define BASE_CFIG_ODBC   0              /* 0=absent, 1=present */
#define BASE_NA_SIDEBAR  0              /* 0=absent, 1=present */
#define BASE_CFIG_OFFICE  0             /* 0=absent, 1=present */


#define HCOMM_EVENT_FLAG hcomm:event_flags
#define BATCH_FLAG batch:function_flags

#ifndef LOOK
	#define LOOK 2
#endif

#define TLR_SCR_SIZE 640,406
#define TLR_SIZE 0,68,640,340
;#define TLR_SCR 0,0,640,408
#define TLR_FIELD_FONT "Courier New"
#define TLR_FIELD_SIZE  12
#define TLR_TEXT_FONT "Arial"
#define TLR_TEXT_SIZE  14

#define CL_SCR_SIZE 0,68,640,340
#define CL_NOTE_SIZE 4,113,620,292
#define CL_FIELD_FONT "Monaco"
#define CL_FIELD_SIZE  12
#define CL_TEXT_FONT "Geneva"
#define CL_TEXT_SIZE  12


#define CTR_TEXT_FONT "Pullman"
#define CTR_TEXT_SIZE  18

#define CL_ADT_SIZE 0,45,640,363

#include "field.h"

/* Averiguar si lo siguiente se usa, estaba en el base_talr.h */

; RT VERSIONS 3 OR 4 ARE SUPPORTED for CL and CB and NA calcs
;#define		APP_VER3
#define		APP_VER4

#ifdef APP_VER3
#define CDSRETVAL  RETVAL
#define COMMRETVAL  RETVAL
#define PRTRETVAL  RETVAL
#endif

#ifdef APP_VER3
#define HCOMM_EVENT_FLAG common:pp_event_flags
#define BATCH_FLAG common:batch_func_flags
#endif
#ifdef APP_VER4
#define HCOMM_EVENT_FLAG hcomm:event_flags
#define BATCH_FLAG batch:function_flags
#endif

/*******/

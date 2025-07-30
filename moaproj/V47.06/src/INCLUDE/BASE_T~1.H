/****x* Pendientes de agrupación/DEF:includes:base_t
* NOMBRE
*   DEF:includes:base_t
* DESCRIPCION
* 	Header files contain #defines for use by the pre-processor	
* 	with "pc" and the import utilities.  The #defines have the	
* 	basic form "#define <name> <value>".  There are three basic	
* 	categories of header files, distinguished by whether the		
* 	<name>s and/or <value>s in the file are standard or custom.	
*																
*   file name		use					<name>s		<value>s	
*   ----------		------------------	--------	--------	
*   xxx_std.h		standard defines	standard	standard	
*   xxx_talr.h		tailoring std fcns	standard	custom		
*   xxx_cust.h		custom use			custom		custom		
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

/*--------------------------------------------------------------*/
/* Global Pattern Tailoring										*/
/*--------------------------------------------------------------*/
/* specify date edit pattern */
#define BASE_DATE_PAT	mm/dd/yyyy
/*--------------------------------------------------------------*/
/* Global Field Tailoring										*/
/*--------------------------------------------------------------*/
/* specify type, length, and pattern for branch_id field */
#define BASE_BRID_TYPE	NUMERIC
/* cambiado para que el op. tenga long 3 (pedido por B.SUD) bettina*/
#define BASE_BRID_LEN	3
#define BASE_BRID_PAT	lib:zerosig
/* specify type, length, and pattern for operator_id field */
#define BASE_OPID_TYPE	NUMERIC 
#define BASE_OPID_LEN	5
#define BASE_OPID_PAT	lib:zerosig
/*--------------------------------------------------------------*/
/* Global Configuration Tailoring								*/
/*--------------------------------------------------------------*/
/* specify whether various applications are present */
#define BASE_CFIG_CL	0				/* 0=absent, 1=present */
#define BASE_CFIG_CB    0               /* 0=absent, 1=present */
#define BASE_CFIG_DDS   0               /* 0=absent, 1=present */
#define BASE_CFIG_NA	0				/* 0=absent, 1=present */
#define BASE_CFIG_NA_CALC	0				/* 0=absent, 1=present */
#define BASE_CFIG_QUE	0				/* 0=absent, 1=present */
#define BASE_CFIG_TLR	1				/* 0=absent, 1=present */
#define BASE_CFIG_BATCH 0               /* 0=absent, 1=present */
#define BASE_CFIG_COEX  0               /* 0=absent, 1=present */
#define BASE_CFIG_OVR   1               /* 0=absent, 1=present */
#define BASE_CFIG_CTR   0               /* 0=absent, 1=present */
#define BASE_CFIG_XSL   0               /* 0=absent, 1=present */
#define BASE_CFIG_NAUSER   1               /* 0=absent, 1=present */

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

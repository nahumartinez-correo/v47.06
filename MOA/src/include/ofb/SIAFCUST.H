/****x* Pendientes de agrupación/DEF:include:siafcust
* NOMBRE
*   DEF:include:siafcust
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

# ifndef	SIAF_CUST_H
# define	SIAF_CUST_H

/*--------------------------------------------------------------*/
/* Valores de Init particulares 				*/
/*--------------------------------------------------------------*/
# define	INIT_ERRFYI_2		40
# define	INIT_ERRFYI_3		41
/*--------------------------------------------------------------*/
/* Main Module Tailoring					*/
/*--------------------------------------------------------------*/
/* specify name of custom teller keys routine */
# define SIAF_TECLAS			KEYScustom_siaf
/*--------------------------------------------------------------*/
/* Menu Module Tailoring					*/
/*--------------------------------------------------------------*/
/* MENUES */
# define SIAF_MENU_ACTIV		teller_actv
# define SIAF_MENU_TRAN			main_menu
# define SIAF_MENU_TRAN_R		main_menu2
# define SIAF_MENU_TECLAS_RAP		speed_key1
# define SIAF_MENU_TECLAS_RAPR		speed_key2

/* TITULO DE LOS MENUES */
# define SIAF_TIT_MENU_TRAN		"TRANSACCIONES DE CAJA"
# define SIAF_TIT_MENU_TRAN_R		"TRANSACCIONES ADMINISTRATIVAS"
# define SIAF_TIT_MENU_ACTIV		"ACTIVIDADES DE CAJERO/ADMINISTRATIVO"
# define SIAF_TIT_TECLAS_RAP		"MENU TECLAS RAPIDAS"

/* Lista de menues posibles */
# define L_SIAF_MENU_COMUN		"MENU TECLAS RAPIDAS"
/*****************************************************************/
/*		COMUNICACIONES					 */
/*****************************************************************/


 /* definicion de mascaras */

# define	BBI_MSK		0x80
# define	EBI_MSK		0x40

# define	ACCEPT_MSK	0x40
# define	SUPERV_MSK	0x20
# define	DATEREQ_MSK	0x04
# define	PRINT_MSK	0x02
# define	PANTALLA_MSK	0x01
# define	MSG_MSK		0x02
# define	PASSBOOK_MSK	0x01

# define	SUPMODE_MSK	0x10
/* mascara que indica 0620 en reentrada */
# define	REENCLOC_MSK	0x02

# ifndef	TRUE
# define	TRUE	1
# define	FALSE	0
# endif

# define	HOST_CRT	host1
# define	HOST_SIAF	host2

/************************************************************************/
/*			EQUS						*/
/************************************************************************/

# define 	ascii		ASCII
# define 	ebcdic		EBCDIC
# define 	bcd		BCD
# define 	binary		BINARY
# define 	asciienchex	ASCIIENCHEX
# define 	zpd		ZPD
# define 	ascii_bulk	ASCII_BULK
# define 	sbinary		SBINARY
# define 	alc		ALC
# define 	sebc		SEBC
# define 	flag		FLAG
# define 	user		USER
# define 	bulk		BULK

/*******/

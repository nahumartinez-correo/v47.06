/****x* Pendientes de agrupación/DEF:include:ISCD
* NOMBRE
*   DEF:include:ISCD
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
# ifndef	ISCD_H
# define	ISCD_H


/*
# define	H1	10
# define	V1	180
# define	H2	590
# define	V2	330
*/

# define	H1	30
# define	V1	240
# define	H2	590
# define	V2	380


/*
# define	BFRP			/* */
# define	BSUD			/* */
# define	CON_P8			/* */
# define	CON_PICKLIST		/* */
/*
# define	CON_NETMAN		/* */
# define	CON_PACKBITS		/* */


# define	ACCT_BSUD		/* */
# define	ID_BSUD			/* */

/*
# define	CON_OTRO_DBMS		/* */

# define	ISCD_ERROR_BASE		9000

/*
# define	CON_DUMP	/* */


# define	ORDEN_CONJUNTA		2

# ifdef	BFRP
# 	define	PP_VERSION		8	
# 	ifdef	POR_DOCUMENTO
# 		undef	POR_DOCUMENTO
# 	endif
# else
# 	define	PP_VERSION		8
# 	define	POR_DOCUMENTO
# 	define	ORDEN_SOCIEDADES	3
# endif

# ifdef	CENTRO_CAPTURA
# 	define	POR_DOCUMENTO
# endif

# ifdef	CON_P8
# 	define	ISC_P8		"1"
# 	define	ISC_BITOBJ	"2"
# endif

# define	PADD_L1		868
# define	PADD_L2		872
# define	PADD_SIZE	20

# define	PROD_TIMEOUT	30
# define	TEST_TIMEOUT	120
# define	RESET		"S"


# endif

/*******/

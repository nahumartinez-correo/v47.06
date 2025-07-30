/****x* Pendientes de agrupación/DEF:include:PICKDRV
* NOMBRE
*   DEF:include:PICKDRV
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
# ifndef	PICKDRV_H
# define	PICKDRV_H

# define	PL_GETITEM	0x8000	
# define	PL_NUMITEMS	0x1FF

# define	LINEA		"L-"
# define	TITULO		"T-"
# define	TOTAL 		"t-"
# define	BLANCO 		"B-"
# define	LINEA_SALDO	"LS"
# define	ACUERDO 	"A-"
# define	MOVIMIENTO 	"M-"
# define	VALORES    	"V-"
# define	EXTRACCIONES   	"E-"

# ifndef	TRUE
# define	TRUE	1
# define	FALSE	0
# endif

# define	SALDOS_POR_PANTALLA		10
# define	MOVIMIENTOS_POR_PANTALLA	10

# endif


/*******/

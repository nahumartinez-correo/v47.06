/****x* Pendientes de agrupación/DEF:include:NAUSER
* NOMBRE
*   DEF:include:NAUSER
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
# ifndef	NAUSER_H
# define	NAUSER_H

# define	PRUEBA	"N"
# define	LE	"1"
# define	LC	"2"
# define	DNI	"3"
# define	CI_PF	"4"
# define	CUIT	"C"
# define	GRADO	"G"
# define	CI_PP	"5"
# define	PPTE	"6"
# define	INAC	"7"
# define	SOC	"8"

# define	ES_DOC	1
# define	ES_TJ	2

/*
# define	TESTMODE	/* */

/*
# define	CON_SERVER_DEBUG	/* */

/*
# define	CON_DISPLAY	/* */

/*
# define	UN_MES_ATRAS	/* */

# ifdef	TESTMODE
# define	UDP_SERVICE	"udp_tst"
# else
# define	UDP_SERVICE	"udp_sna"
# endif

# define	SRV_SERVICE	"/netphys/dsacli_"

# define	AS400LINE_HDR_S	10
# define	CON_SIGNO

/* Valores de status */
# define	TRAN_ACEPTADA	"A"
# define	TRAN_ACEPTMSG	"M"
# define	TRAN_RECHAZADA	"R"
# define	TRAN_TIMED_OUT	"T"
# define	TRAN_PENDIENTE	"P"
# define	TRAN_RECHLOCAL	"Q"

# define	SIAF_FMH_S	5

# define	CON_TARJETA	/* */


# define	IMPOR_MAX_PESOS		300000
# define	IMPOR_MAX_DOLARES	300000

# define	ERR_DOCUMENTO_INVALIDO	600
# define	ERR_TARJETA_INVALIDA	601
# define	ERR_CLAVE_INVALIDA	603
# define	ERR_CTA_SIN_MOVIM	605

# define	ERR_CLAVE_BLOQUEADA	608
# define	CLAVE_CAMBIADA  	610

# endif

# define	HORASCHEQUERA		72
# define	HORASBOLETA		96

/*******/

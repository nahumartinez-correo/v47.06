/****x* Pendientes de agrupación/DEF:includes:comm
* NOMBRE
*   DEF:includes:comm
* DESCRIPCION
*   Comportamiento del objeto (obligatorio)
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

# ifndef	COMM_H
# define	COMM_H

# ifndef	TRAN_COD_S
# define	TRAN_COD_S	4
# endif

# ifndef	DATE8_S
# define	DATE8_S		8
# endif

# ifndef	TIME_S
# define	TIME_S		6
# endif

# ifndef	BRANCH_S
# define	BRANCH_S	3
# endif

# ifndef	TELLER_S
# define	TELLER_S	2
# endif


# ifndef	SUPMARK_S
# define	SUPMARK_S	1
# endif

# ifndef	FILLER1_S
# define	FILLER1_S	1
# endif

# ifndef	NERR
# define	NERR		4
# endif

# ifndef	ERR_S
# define	ERR_S		3
# endif

# ifndef	RET_S
# define	RET_S	2
# endif

# ifndef	ERRSTAT_S
# define	ERRSTAT_S	1
# endif

# ifndef	NETMAN_S
# define	NETMAN_S
# endif

# ifndef	TRUE
# define	TRUE	1
# define	FALSE	0
# endif

# ifndef	TRAN_ACEPTADA
# define	TRAN_ACEPTADA	0
# define	TRAN_RECHAZADA	1
# define	REQ_SUPERVISOR	2
# define	TRAN_OBSERVADA	3
# endif

# define	CON_ORDEN
# define	ISCD_SERV_NAME	"/netphys/sig_srv_"

# ifndef	SIS_S
# define	SIS_S	2
# endif

# ifndef	FIRST_NAME_S
# define	FIRST_NAME_S	25
# endif

# ifndef	LAST_NAME_S
# define	LAST_NAME_S	25
# endif

# ifndef	CIFNUM_S
# define	CIFNUM_S	15
# endif

# ifndef	CIFNAME_S
# define	CIFNAME_S	35
# endif

# ifndef	ACCOUNT_S
# define	ACCOUNT_S	20
# endif

# ifndef	ACCT_TYPE_S
# define	ACCT_TYPE_S	3
# endif

# ifndef	BRANCH_S
# define	BRANCH_S	3
# endif

# ifndef	COUNT_S
# define	COUNT_S		2
# endif

# ifndef	CLINUM_S
# define	CLINUM_S	9
# endif

# ifndef	CODE_S
# define	CODE_S		1
# endif

# ifndef	IDTYPE_S
# define	IDTYPE_S	3
# endif

# ifndef	CARDNO_S
# define	CARDNO_S	IDTYPE_S
# endif

# ifndef	TOTAL_S
# define	TOTAL_S		3
# endif

# ifndef	AMOUNT_S
# define	AMOUNT_S	13
# define	AMTINT_S	11
# define	AMTDEC_S	2
# endif

# ifndef	FREC_S
# define	FREC_S		8
# endif

# ifndef	TIPOCLI_S
# define	TIPOCLI_S	2
# endif

# ifndef	CHAR_S
# define	CHAR_S		1
# endif

# ifndef	TYPE_S
# define	TYPE_S		CHAR_S
# endif

# endif

/*******/

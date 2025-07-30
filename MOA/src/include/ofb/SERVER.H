/****x* Pendientes de agrupación/DEF:include:SERVER
* NOMBRE
*   DEF:include:SERVER
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
/*
**
**	Macros para ocultar la implementacion de la interfase
**	del 'rt' con el 'server'
*/
# ifndef	SERVER_H	/* pa no incluir 2 veces vio? */ 
# define	SERVER_H

# define  UDP_OPEN	call CSR:udp_connect	/* abre conexion al server */
# define  TCP_OPEN	call CSR:tcp_connect	/* abre conexion al server */
# define  SRV_OPEN	call CSR:server_open	/* abre conexion al server */
# define  SRV_CLOSE	call CSR:server_close	/* cierra conexion al server */
# define  SRV_SEND	call CSR:server_send	/* Mandar al server */
# define  SRV_RECV	call CSR:server_recv	/* Recibir del server */
# define  SRV_SNDRCV	call CSR:server_send_recv /* Mandar al server y recibir */
# define  SRV_PUT	call CSR:server_putflds /* Poner campos buff */
# define  SRV_GET	call CSR:server_getflds	/* Extraer campos */
# define  SRV_SETPTR	call CSR:server_set_ptr	/* Posicionar pointer */
# define  SRV_CLRBUF    call CSR:server_clear_buffer /* Limpiar buffer */
# define  SRV_DEBUG	call CSR:toggle_server_debug	/* toggle del debugger
							   de la interfase */
# define  TCP_CLOSE	SRV_CLOSE
# define  UDP_CLOSE	SRV_CLOSE

# define	RCVBUF		"R"		/* buffer de receive  */
# define	SNDBUF		"S"		/* buffer de send */

# define	ABS		"a"		/* posicionamiento absoluto */
# define	REL		"r"		/* posicionamiento relativo */

# define	SND_SIZE	512
# define	RCV_SIZE	12288

# endif

/*******/

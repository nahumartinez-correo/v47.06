/****x* Pendientes de agrupación/DEF:include:GSP
* NOMBRE
*   DEF:include:GSP
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

	/* Functions */
#define GSP_OPEN		0
#define GSP_CLOSE		1
#define GSP_GET_TERMIO	2
#define GSP_SET_TERMIO	3
#define GSP_GETC		4
#define GSP_PUTC		5
#define GSP_READ		6
#define GSP_WRITE		7
#define GSP_PURGE		8
#define GSP_SET_TIMEOUT	9
#define GSP_DUMP_BUFFER	10
#define GSP_GET_LASTC	11
#define GSP_DUMP_TERMIO	12
#define GSP_SET_LRC		13
#define GSP_COMPUTE_LRC	14
#define GSP_HISTORY		15
#define GSP_GETFCNTL	16
#define GSP_SETFCNTL	17
#define GSP_SETIOCTL	18
#define GSP_READ_NW		19
#define GSP_CANCEL		20

	/* ERROR codes */

#define GSPERR_OPENFAILED	-1
#define GSPERR_LOCKFAILED	-2
#define GSPERR_MAXCHARS		-3
#define GSPERR_NO_EOD		-4
#define GSPERR_TIMEOUT		-5

#define GSPERR_BAD_ARGUMENT	-9
#define GSPERR_NOTOPEN		-10
#define GSPERR_SHORTWRITE	-11
#define GSPERR_NOCHAR		-12
#define GSPERR_RESET		-13
#define	GSPERR_IO_FAIL		-14
#define	GSPERR_KEY_OUT		-15
#define GSPERR_API_FAIL		-16
	/* ASCII control character names */
#define GSP_STX		0x02
#define GSP_ETX		0x03
#define GSP_EOT		0x04
#define GSP_EOM		0x19
#define GSP_SI 		0x0F
#define GSP_SO 		0x0E
#define GSP_ACK		0x06
#define GSP_NAK		0x15
#define GSP_FS		0x1C
#define GSP_SUB		0x1A


#ifndef GSP_ITSELF		/* DO NOT DEFINE THIS SYMBOL! */
/*
 *	Input modes
 */
#define IGNBRK	0x0001
#define BRKINT	0x0002
#define IGNPAR	0x0004
#define PARMRK	0x0008
#define INPCK	0x0010
#define ISTRIP	0x0020
#define INLCR	0x0040
#define IGNCR	0x0080
#define ICRNL	0x0100
#define IUCLC	0x0200
#define IXON	0x0400
#define IXANY	0x0800
#define IXOFF	0x1000

/*
 *	Output modes
 */
#define OPOST	0x0001
#define OLCUC	0x0002
#define ONLCR	0x0004
#define OCRNL	0x0008
#define ONOCR	0x0010
#define ONLRET	0x0020
#define OFILL	0x0040
#define OFDEL	0x0080
#define NLDLY	0x0100
#define NL0		0x0000
#define NL1		0x0100
#define CRDLY	0x0600
#define CR0		0x0000
#define CR1		0x0200
#define CR2		0x0400
#define CR3		0x0600
#define TABDLY	0x1800
#define TAB0	0x0000
#define TAB1	0x0800
#define TAB2	0x1000
#define TAB3	0x1800
#define XTABS	0x1800
#define BSDLY	0x2000
#define BS0		0x0000
#define BS1		0x2000
#define VTDLY	0x4000
#define VT0		0x0000
#define VT1		0x4000
#define FFDLY	0x8000
#define FF0		0x0000
#define FF1		0x8000

/* 
 *	Control modes
 */
#define CBAUD	0x000F
#define CSIZE	0x0030
#define CS5		0x0000
#define CS6		0x0010
#define CS7		0x0020
#define CS8		0x0030
#define CSTOPB	0x0040
#define CREAD	0x0080
#define PARENB	0x0100
#define PARODD	0x0200
#define HUPCL	0x0400
#define CLOCAL	0x0800
#define RCV1EN	0x1000
#define XMT1EN	0x2000
#define LOBLK	0x4000

/* 
 *	Baud Rates
 */
#define B0		0x0000
#define B50		0x0001
#define B75		0x0002
#define B110	0x0003
#define B134	0x0004
#define B150	0x0005
#define B200	0x0006
#define B300	0x0007
#define B600	0x0008
#define B1200	0x0009
#define B1800	0x000A
#define B2400	0x000B
#define B4800	0x000C
#define B9600	0x000D
#define B19200	0x000E
#define EXTA	0x000E
#define B38400	0x000F
#define EXTB	0x000F

/*
 *	Line discipline 0 modes
 */
#define ISIG	0x0001
#define ICANON	0x0002
#define XCASE	0x0004
#define ECHO	0x0008
#define ECHOE	0x0010
#define ECHOK	0x0020
#define ECHONL	0x0040
#define NOFLSH	0x0080

#endif
#define FAILURE RT_ERROR

/*******/

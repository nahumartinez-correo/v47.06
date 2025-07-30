
#define	NCC	8

/* control characters */
#define	VINTR	0
#define	VQUIT	1
#define	VERASE	2
#define	VKILL	3
#define	VEOF	4
#define	VEOL	5
#define	VEOL2	6
#define	VMIN	4
#define	VTIME	5
#define	VCEOF	NCC		/* true EOF char (V7 compatability) */
#define	VCEOL	(NCC + 1)	/* true EOL char */

#define	CNUL	0
#define	CDEL	0377
/* default control chars */
#define	CESC	'\\'
#define	CINTR	0177	/* DEL */
#define	CQUIT	034	/* FS, cntl | */
#define CERASE  '\010'  /* backsp */
#define CKILL   '\025'  /* cntl u */
#define	CEOF	04	/* cntl d */
#define	CSTART	021	/* cntl q */
#define	CSTOP	023	/* cntl s */


/* input modes */
#define	IGNBRK	0000001
#define	BRKINT	0000002
#define	IGNPAR	0000004
#define	PARMRK	0000010
#define	INPCK	0000020
#define	ISTRIP	0000040
#define	INLCR	0000100
#define	IGNCR	0000200
#define	ICRNL	0000400
#define	IUCLC	0001000
#define	IXON	0002000
#define	IXANY	0004000
#define	IXOFF	0010000

/* output modes */
#define	OPOST	0000001
#define	OLCUC	0000002
#define	ONLCR	0000004
#define	OCRNL	0000010
#define	ONOCR	0000020
#define	ONLRET	0000040
#define	OFILL	0000100
#define	OFDEL	0000200
#define	NLDLY	0000400
#define	NL0	0
#define	NL1	0000400
#define	CRDLY	0003000
#define	CR0	0
#define	CR1	0001000
#define	CR2	0002000
#define	CR3	0003000
#define	TABDLY	0014000
#define	TAB0	0
#define	TAB1	0004000
#define	TAB2	0010000
#define	TAB3	0014000
#define	BSDLY	0020000
#define	BS0	0
#define	BS1	0020000
#define	VTDLY	0040000
#define	VT0	0
#define	VT1	0040000
#define	FFDLY	0100000
#define	FF0	0
#define	FF1	0100000

/* control modes */
#define	CBAUD	0000017
#define	B0	0
#define	B50	0000001
#define	B75	0000002
#define	B110	0000003
#define	B134	0000004
#define	B150	0000005
#define	B200	0000006
#define	B300	0000007
#define	B600	0000010
#define	B1200	0000011
#define	B1800	0000012
#define	B2400	0000013
#define	B4800	0000014
#define	B9600	0000015
#define	B19200	0000016
#define	B38400	0000017
#define	CSIZE	0000060
#define	CS5	0
#define	CS6	0000020
#define	CS7	0000040
#define	CS8	0000060
#define	CSTOPB	0000100
#define	CREAD	0000200
#define	PARENB	0000400
#define	PARODD	0001000
#define	HUPCL	0002000
#define	CLOCAL	0004000

/* line discipline 0 modes */
#define	ISIG	0000001
#define	ICANON	0000002
#define	XCASE	0000004
#define	ECHO	0000010
#define	ECHOE	0000020
#define	ECHOK	0000040
#define	ECHONL	0000100
#define	NOFLSH	0000200
#define	XCLUDE	0100000		/* *V7* exclusive use */

#define SSPEED  13              /* default speed: 7=300, 13=9600 baud */

/*
 * Serial line discipline definitions
 */
#define LDTTY		0	/* TTY line discipline */
#define	LDSLIP		1	/* Slip line discipline */
				/* (2-7) reserved */
#define LDZUMA		8	/* Zuma mouse line discipline */
#define	LDSAMPLE	9	/* Sound Sampler line discipline */
				/* (10-15) reserved */
#define LDX25DTE	16	/* X.25/LAPB DTE line discipline */
#define LDX25DCE	17	/* X.25/LAPB DCE line discipline */
#define	LDSDLCSEC	18	/* SDLC/SNA secondary line discipline */
#define	LDSDLCPRI	19	/* SDLC/SNA primary line discipline */
#define	LDBYSNSEC	20	/* IBM BISYNC secondary line discipline */
#define	LDBYSNPRI	21	/* IBM BISYNC primary line discipline */

/*
 * Ioctl control packet
 */
struct termio {
	unsigned short	c_iflag;	/* input modes */
	unsigned short	c_oflag;	/* output modes */
	unsigned short	c_cflag;	/* control modes */
	unsigned short	c_lflag;	/* line discipline modes */
	char		c_line;		/* line discipline */
//  FRANK: changed to unsigned char. uchar_t not defined
//	uchar_t         c_ccs[NCC];     /* control chars */
	unsigned char   c_ccs[NCC];     /* control chars */
	char		c_pad;		/* Padding..... */
};

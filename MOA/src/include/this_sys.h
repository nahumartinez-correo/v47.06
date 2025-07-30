#ifndef THIS_SYS_H
#define THIS_SYS_H

#define	A_I86_L
#ifndef i386
#	define i386 1
#endif
#define	LITTLE_ENDIAN 1
#define	ENV_TZ
#define	IO_TERMIO
#define	LCK_S5
#define	OPN_S5

#define	SYSZBLOCK	1024
#define FSYNC_B4_2

#ifndef NT
#define NT 1
#endif
#define VOID_KEYWORD /* our compiler supports void */
#ifndef LINTING
#define ANSI_C_COMPILER
#endif

#endif


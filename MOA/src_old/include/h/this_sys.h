#ifndef THIS_SYS_H
#define THIS_SYS_H

#ifndef NT
#define	NT 1
#endif

#ifndef i386
#	define i386 1
#endif

#ifndef UNIX
#define UNIX 1
#endif

#ifndef SVR4
#define SVR4 1
#endif


#ifndef LINTING
#define ANSI_C_COMPILER
#endif

#define	A_I86_L
#define	ENV_TZ
#define	IO_TERMIO
#define	LCK_S5
#define	OPN_S5
#define	SYSZBLOCK	1024
#define FSYNC_B4_2
#define VOID_KEYWORD /* our compiler supports void */

#endif

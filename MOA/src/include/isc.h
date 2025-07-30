#ifndef ISC_H
#define ISC_H
#ifndef THIS_SYS_H
#	include <this_sys.h>
#endif

#if DNIX == 0
typedef void catch_t;
#endif

#ifdef NT
#define iscsignal signal
	/* bjo nt - includes dont offer the SYS_SIGNAL_H entry. */
typedef catch_t (*signal_handler)(int);
#endif

#ifndef NT
#ifndef	_SYS_SIGNAL_H
extern int iscsignal(int sig, int (*func)());
#else
typedef catch_t (*signal_handler)(int);
extern  signal_handler iscsignal (int sig, signal_handler);
#endif
#endif

extern char *iscgetcwd(char *buf, int size);
#ifndef CONVERSION_H
extern char *cvxl(long *longptr, char *x);	/* atol+ */
#endif
extern int flflush(int fd);	/* dnix SF_FLUS or unix fsync()*/
#endif

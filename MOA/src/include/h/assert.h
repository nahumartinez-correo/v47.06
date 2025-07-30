#include	<assert.h>

#ifndef XPDATA
#define XPDATA	extern
#endif
XPDATA    int debugsw;
void myprintf(const char *format, ...);

#define DBG_BUFFERS 4
#define DBG_LOGGING 8
#define DBG_COMM	16
#define DBG_CONNECT	32
#define DBG_FUNCTIONS 64
#define DBG_MAIN	128
#define DBG_NULL_UPDATE 256
#define DBG_2PHC 512  /* two phase commit */
#define DBG_PQL 1024  /* Pretty Quick (Query) Language */
#define DBG_INDEXING 2048 /* cdsv indexing */
#define DBG_UPDATE   4096
#define DBG_HEX_DUMP 8192
#define DBG_CANCEL_SCAN 16384

#ifdef DEBUG
//#define LOG(x,y) if (debugsw) {myprintf(x,y);}
# define LOG(L,X) {if (L & debugsw) myprintf X;}
#else
#define LOG(x,y)
#endif

//extern	FILE	*cryfile ;

#ifdef	DEBUG
extern char Asrtmsg[] ;
/*lint -e607 */
#define	ASSERT(condition)	{if (!(condition)) {\
		extern char *Progname ;\
		cry(Asrtmsg,Progname,__LINE__,__FILE__, " condition ");\
		abort() ;\
	}\
}
#else
#define	ASSERT(condition)
#endif


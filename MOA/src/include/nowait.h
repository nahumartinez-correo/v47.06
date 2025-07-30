#ifndef NOWAIT_H
#define NOWAIT_H

#define TIMED_OUT 		2
#define RESET_OUT 		3
#define KEY_OUT   		4		/* used in no-wait I/O */

#define	RESET_TIMED_OUT	10
#define	KEY_TIMED_OUT	11

#define	MSGFOUND		100		/* return value from rt_wait() */
	
	extern int Trapqfd;			/* Trap Queue File Descriptor */

	int rt_wait(HWND hWnd, UINT uMsgFilterMin,
								UINT uMsgFilterMax, UINT uRemoveMsg,
								int timeout, int reset_timeout,
								int key_timeout);

#include <rttimers.h>
#endif

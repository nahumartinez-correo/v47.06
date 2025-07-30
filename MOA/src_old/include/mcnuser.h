
//
//  MNCUSER.H  user header file for Message Callbacks and Network Library
//

#ifndef MCNUSER_H
#define MCNUSER_H

//
// Includes
//
#include <winsock.h>
#include <typedefs.h>
#include <onaport.h>
#ifndef	XDLL
#define XDLL
// #define XDLL	_declspec (dllexport)   // To export from DLL
#endif

//
// Externs
//
extern	XDLL int	mcnsockerror;
#define es_error	mcnsockerror

//
// Error Codes
//
#define MCNSOCKERROR	0

#define	MCNBASE			4000
#define	MCNENOCCB	   	(MCNBASE + 1)	// couldn't find channel control block
#define	MCNENORCB		(MCNBASE + 2)	// couldn't find request control block
#define	MCNETSDUHDR		(MCNBASE + 3)	// error handling the TSDU header
#define	MCNEREMOVED		(MCNBASE + 4)	// no longer used
#define	MCNECANCEL		(MCNBASE + 5)	// the request was canceled
#define	MCNEDUPLICATE	(MCNBASE + 6)	// couldn't add CCB -- duplicate in list
#define	MCNENOTIMER		(MCNBASE + 7)	// couldn't create timer

//
// MCNAsync Function "flag" Bits
// CCB cflags bits
// RCB uflags bits
//
#define	MCN_TSDU					0x1
#define MCN_TSDU_BUFFER_PROVIDED	0x10
#define MCN_HEARTBEAT				0x20
#define MCN_ASYNC_GETXBY			0x40
#define MCN_LOCATE_SERVICE			0x80
#define	MCN_MORE					0x100
#define MCN_DATAGRAM				0x200
#define MCN_BROADCAST				0x400
#define MCN_REUSEADDR				0x800 // honored only MCNOpenDatagram()

//
// Log Levels
//
#define	MCNMIN			0x1
#define	MCNIO			0x2
#define	MCNTRACE		0x4
#define	MCNEVENTS		0x08
#define	MCNTSDU			0x10
#define	MCNMAX			0x100
#define MCNTIME			0x200		// tick count printed with every log if this bit is on.

//
// Transport Defines
//
#define MCN_TCP			0
#define MCN_UDP			1

//
//	MCN Socket Message ID
//
#define	WM_SOCKETMSG	(0xabcd)
#define	WM_GETXBYMSG	(0xabce)

//
//	MCN Async Event ID
//
#define	FD_MCNGETXBY	(0x8000)

//
//	UDP Message definitions
//
#define	MCN_UDPM_TIMEOUT	2			// seconds to wait for UDP response
#define	MCN_UDPM_RETRIES	2			// times to retry UDP request

//
//	MCN UDP Message definitions
//
#define	MCN_UDPM_MAX_LEN			512		// max UDP packet size

#define	MCN_UDPF_SERVER_LOCATE		1		// server locate function code
#define	MCN_UDPF_SERVER_LOCATION	2		// server location function code

#define	MCN_UDPN_SERVER_LOCATE		3		// server locate number of parameters
#define	MCN_UDPN_SERVER_LOCATION	4		// server location number of parameters

struct	mcn_udp {
	u_long	len;							// UDP message length
	u_short	func;							// UDP message function code
	char	msg[MCN_UDPM_MAX_LEN-sizeof(u_long)-sizeof(u_short)];	// message buffer
};

typedef	struct mcn_udp	MCNUDP;
typedef	struct mcn_udp	*PMCNUDP;

//
//	Function Prototypes
//
XDLL int	MCNInit(int ccbnum, int rcbnum);
XDLL int	MCNCleanup();
XDLL SOCKET	MCNOpenListen(HWND hwnd, int transport, char *hostname, char *servicename, 
						int qlen, int flags);
XDLL SOCKET	MCNOpenListenEx(HWND hwnd, int transport, char *hostname, char *servicename, 
						int qlen, int flags, short *port);
XDLL SOCKET	MCNConnect(HWND hwnd, int transport, char *hostname, char *servicename, 
						int flags);
XDLL SOCKET	MCNConnectService(HWND hwnd, int transport, char *hostname, 
						char *servicename, u_short serviceport, u_long timeout, 
						int retries, int flags);
XDLL int MCNPingService(HWND hwnd, int transport, char *hostname, char *servicename,
				  u_short serviceport, u_long timeout, int retries);
XDLL int	MCNAsyncConnect(HWND hwnd, int transport, void (*func)(), char *hostname, 
						char *servicename, int flags, void *userp);
XDLL int	MCNAsyncConnectService(HWND hwnd, int transport, void (*func)(), 
						char *hostname, char *servicename, u_short serviceport, 
						u_long timeout, int retries, int flags, void *userp);
XDLL int	MCNAsyncListen(HWND hwnd, void (*func)(), SOCKET sock, void *userp);
XDLL int	MCNAsyncRecv(HWND hwnd, void (*func)(), SOCKET sock, UCHAR *buf, int len, 
						int flags, void *userp);
XDLL int	MCNAsyncSend(HWND hwnd, void (*func)(), SOCKET sock, UCHAR *buf, int len, 
						UINT flags, void *userp);
XDLL SOCKET	MCNOpenDatagram(HWND hwnd, char *hostname, char *servicename, 
						u_short serviceport, int flags);
XDLL int	MCNAsyncRecvDatagram(HWND hwnd, void (*func)(), SOCKET sock, UCHAR *buf, int len, 
						 struct sockaddr *from, int *fromlen, int flags, void *userp);
XDLL int	MCNAsyncSendDatagram(HWND hwnd, void (*func)(), SOCKET sock, UCHAR *buf, int len, 
						 struct sockaddr *to, int tolen, int flags, void *userp);
XDLL int	MCNGetAddress(int transport, char *hostname, char *servicename,
						u_short serviceport, struct sockaddr *saddrp, int *addrlen);
XDLL int	MCNBuildServerLocate(MCNUDP *udpip, char *servicename);
XDLL int	MCNBuildServerLocation(MCNUDP *udpip, char *servicename);
XDLL int	MCNBuildServerLocationMsg(MCNUDP *udpip, char *servicename, short port);
XDLL int	MCNMessage(HWND hwnd, UINT message, UINT wparam, LONG lparam);
XDLL int	MCNClose(HWND hwnd, SOCKET sock);
XDLL int	MCNAddSocket(HWND hwnd, int transport, SOCKET sock, int flags);
XDLL int	MCNRemoveSocket(HWND hwnd, SOCKET sock);
XDLL int	MCNAddMessage(HWND hwnd, UINT msgid, void (*func)());
XDLL int	MCNRemoveMessage(HWND hwnd, UINT msgid);
XDLL int	MCNCancel(int reqid);
XDLL int	MCNbeatHeart(HWND hwnd, SOCKET sock);
XDLL int	MCNSend(HWND hwnd, SOCKET sock, UCHAR *buf, int ulen, int uflags);
XDLL int	MCNRecv(HWND hwnd, SOCKET sock, UCHAR *buf, int ulen, int *uflags);
XDLL void	MCNULog();
XDLL void	MCNSetULog(int level, int console, const char *filename, HANDLE *file);

#endif

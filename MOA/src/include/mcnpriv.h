
//
//  MCNPRIV.H  private header file for Message Callback and Network Library
//
#define MCN_MAX_ONE_SEND 1024   // maximum buffer to copy internally									   

//
//	Channel Control Block  One of these is used for
//	each client connection, the Listen channel, and the
//	Alarm and Wait queues.
//
typedef struct ccb {
	struct ccb	*next;			// listmgr requirment
	struct ccb	*prev;			// listmgr requirment
	short		flags;			// listmgr requirment
	struct rcb	*rcbhead;		// request control block list for this channel
	void		*userp;			// user pointer returned in event callback
	void		(*func)();		// event callback function pointer
	HWND		hwnd;			// window handle
	UINT		msgid;			// message id
	WPARAM		wparam;			// wparam - socket
	LPARAM		lparam;			// lparam - event
	UINT		cflags;			// channel flags, MCN_TSDU, MCN_MORE (recv)
	int			transport;		// specifies transport
	int			rseglen;		// tsdu recv segment length left
	int			sseglen;		// tsdu send segment length left
	int			rtsduhbytes;	// tsdu header recv bytes
	UCHAR		rtsduhbuf[4];	// tsdu header recv buffer
	int			stsduhbytes;	// tsdu header send bytes
	UCHAR		stsduhbuf[4];	// tsdu header send buffer
	unsigned	msgs_in_period;	// for MCN_HEART_BEAT
	CRITICAL_SECTION beat;
} CCB;

//
//	NoWait Request Control Block  One of these is used for
//	each nowait request
//
typedef struct	rcb {
	struct rcb	*next;			// listmgr requirment
	struct rcb	*prev;			// listmgr requirment
	short		flags;			// listmgr requirment
	struct ccb	*ccbp;			// channel control block ptr
	void		*userp;			// user pointer returned in request callback
	void		(*func)();		// request callback function pointer
	UINT		uflags;			// request flags, MCN_MORE, MCN_EXPIDITED
	UINT		msg;			// message id number for this event
	long		operation;		// io operation, read, write...
	int			nwnum;			// no wait request number
	UCHAR		*data;			// data storage malloced/freed by user
	UCHAR		*current;		// pointer into data used for cat
	int			len;			// number of bytes requested
	int			left;			// number of bytes left to complete request 
	char		*cudp;			// pointer to call-user-data buffer
	int			cudlen;			// call-user-data length
	int			reqid;			// nowait request id
	int 		*fromlen;		// address len of sender in MCNAsyncRecvFrom()
	int			tolen;			// address len of destination in MCNAsyncSendTo()
	struct sockaddr *fromto;	// address of sender/destiation in MCNAsyncRecvFrom() MCNAsyncSendTo() 
} RCB;

//
//	Async Connect Control Block  Only one of these is used for
//	the async connect request
//
typedef struct	accb {
	RCB			*rcbp;			// request control block ptr
	void		*userp;			// user pointer returned in request callback
	void		(*func)();		// request callback function pointer
	SOCKET		sock;			// connection socket
	int			transport;		// specifies transport
	char		*hostname;		// host name
	char		*servicename;	// service name
	char		*proto;			// protocol name
	u_short		serviceport;	// default service port
	SOCKET		servicesock;	// find service socket
	u_long		timeout;		// find service timeout
	int			retries;		// find service retries
	UINT		idtimer;		// timer id (=accbp)
	SOCKADDR_IN	serviceaddr;	// service address (recv)
	int			addrlen;		// size of service address buffer (recv)
	MCNUDP		recvudp;		// UDP server location buffer (recv)
	union {
		BYTE		buf[MAXGETHOSTSTRUCT];	// max host/service entry buffer
		HOSTENT		heb;		// host entry buffer
		SERVENT		seb;		// service entry buffer
		struct {
			SOCKADDR_IN	addr;	// service address (send)
			MCNUDP		info;	// UDP server locate buffer (send)
		} udp;
	} data;
} ACCB;
																 

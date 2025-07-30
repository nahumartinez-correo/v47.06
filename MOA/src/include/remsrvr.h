//	remsrvr.h
// $Revision: 1 $ $Date: 10/31/95 11:39a $
// $Locker:  $

/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*
*	Structure used to receive and send wingsp messages
*/
typedef struct
	{
		unsigned char	header[4];
		int				cmd;		// command/status from/to client
		DWORD			err;		// error code to client
		int				status;		// completion status to client
		unsigned char	msg[256];
	} MYMSG, *PMYMSG;

/*
*   Structure passed into the accept thread
*/
typedef struct
	{
		int			nThread;  		// Thread number.
		int 		count;
		int			numchars;
		int			size;
		int			command;
		DWORD		open_flags;
		char		comm_port[6];
		char		eodchars[20];
		BOOL		have_eods;
		BOOL		readinprogress;
		SOCKET  	sock;
		SOCKET  	connsock;
		HANDLE  	fd;
		FD_SET		readfds;
		OVERLAPPED	overlapped;
		MYMSG		msg;
	} THREADPACK, *PTHREADPACK;

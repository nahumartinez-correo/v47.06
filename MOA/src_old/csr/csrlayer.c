// ==================================================================
// MODULENAME - cslaryer.c
//
// $Name: R1V4C13 R1V4C12 $
// $Revision: 1.24 $
// $Date: 1998/10/16 16:45:57 $
//
// CSRLAYER --   Provide an interface layer between CSR's which must
//               invoke a function in the CSR.DLL but whose actual
//               function to be executed resides in the RT.DLL.
//
//	External Function List:
//
//		In RT.DLL:
//			manual()
//			undup_fld_list()
//
//	Notes:
//
//
//  (c) Copyright by Olivetti North America, 1994
//        All rights reserved.
//
//  Author: Chuck Hartshorn
//
// ==================================================================

#include "pch.h"

#include <typedefs.h>
#include <defines.h>
#include <ppdefs.h>
#include <pclwords.h>
#include <dpwords.h>
#include <errdefs.h>
#include <dsplist.h>
#include <tdspl.h>
#include <scrlist.h>
#include <rt.h>
#include <rtprotos.h>
#include <convers.h>
#include <misc.h>
#include <dsplist.h>
#include <textedit.h>
#include <ncontrol.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <commpro.h>
#include <picklist.h>
#include <nowait.h>
#include <csr.h>
#include <winuser.h>
#include <msgrtn.h>

	HANDLE	hRTdll;	// global so other CSR's may access this handle

	static byte		trick[4] = {ICN, 0, 0, 0};
	static int		icon_not_loaded_yet = TRUE;
	void			*mydrawfield;
	void			*myredrawcontrol;



//----------------------------
//		manual()
//----------------------------
XDLL int
f_manual(int argc,rtobj *argv[])
{
	return(manual(argc,argv));
}




//----------------------------
//		undup_fld_list()
//----------------------------
XDLL int
f_undup_fld_list(int argc,rtobj *argv[])
{
	undup_fld_list();
	return(RT_SUCCESS);
}



//------------------------------------------------
//		Called by DataMan when he can't find a CSR
//------------------------------------------------
XDLL int
CSRForwardReference(int argc,rtobj *argv[])
{
	ofb_msgbox(hWnd,"Forward Referenced CSR object\n"
				"Did you forget to IMPORT the COMMON.CSR file?", "Warning",
	           MB_OK + MB_ICONEXCLAMATION);
	return(RT_SUCCESS);
}


// ---------------
// xlate_filename
// ---------------
XDLL int
csr_xlate_filename(int argc,rtobj *argv[])
{
	char	filename[128];
	field	*fp;

	if (rtobj2str(argv[0], filename, sizeof(filename)) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (argv[1]->obj.object != CSRARG_FLD)
		return(CSR_FAIL_ARGTYPE);
	fp = (field *) argv[1]->ptr;
	str2field(xlate_filename(filename),fp);
	return(RT_SUCCESS);
}


// --------------------------------------------------
// csr_wait - crude synchronization point for events
// 
// argv[0] = MsgMin
// argv[1] = MsgMax
// argv[2] = RemoveMsg
// argv[3] = timeout
// argv[4] = reset_lockout
// argv[5] = key_lockout
// --------------------------------------------------
XDLL int
csr_wait(int argc,rtobj *argv[])
{
	int	status, timeout, reset_lockout, key_lockout;
	int MsgMin, MsgMax, RemoveMsg;
	int	sv_RTsuspend_reset;

	if (rtobj2int(argv[0], &MsgMin) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[1], &MsgMax) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[2], &RemoveMsg) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[3], &timeout) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[4], &reset_lockout) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[5], &key_lockout) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	status = 0;
	while (status != MSGFOUND) {
		sv_RTsuspend_reset = RTsuspend_reset;
		RTsuspend_reset    = -1;	// set to -1 for proper reset handling
		// -------------------------------
		// wait for message, key or timer
		// -------------------------------
		status = rt_wait(hWnd,(UINT)MsgMin,(UINT)MsgMax,(UINT)RemoveMsg,
							timeout,reset_lockout,key_lockout);
		RTsuspend_reset = sv_RTsuspend_reset;
		if (status == RESET_OUT && reset_lockout != 0)
			break;
		if (status == KEY_OUT && key_lockout != 0)
			break;
		if (status == TIMED_OUT && timeout > 0)
			break;
		if (status == RESET_TIMED_OUT && reset_lockout > 0)
			break;
		if (status == KEY_TIMED_OUT && key_lockout > 0)
			break;
		}
	return(status);
}

// -----------------------------------------------------------
// create_process - create child process
//	
// 
// argv[0] = cmd
// argv[1] = window_type (hidden,minimized, etc.. from winuser.h) -optional-
// argv[2] = wait|nowait (csr will optionally wait for process to complete)
// -----------------------------------------------------------
XDLL int
create_process(int argc,rtobj *argv[])
{
	char		Buf[256];
	int	retval;
	int window_state = SW_SHOWNORMAL;
	int waitflag = CSR_NOWAIT;
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	if (argc < 1)	{
		mre(-1,"Invalid Number of arguments passed");
		return(CSR_FAIL_ARGCOUNT);
		}
	if (rtobj2str(argv[0],Buf,sizeof(Buf)) == RT_ERROR) {
		mre(-1,"CSR:exec_hidden, Invalid Command");
		return(CSR_FAIL_ARGTYPE);
		}
	if ( argc >= 2 ) {
		if (rtobj2int(argv[1],&window_state) == RT_ERROR) {
			mre(-1,"CSR:exec_hidden, Invalid Window State");
			return(CSR_FAIL_ARGTYPE);
			}
		}
	if ( argc == 3 ) {
		if (rtobj2int(argv[2],&waitflag) == RT_ERROR) {
			mre(-1,"CSR:exec_hidden, Invalid Wait Flag");
			return(CSR_FAIL_ARGTYPE);
			}
		}
		

	memset(&si,0,sizeof(si) );
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = window_state;
	if (!CreateProcess(0,Buf,0,0,0,0,0,0,&si,&pi))	// start process
		return(GetLastError()*-1);
	if (waitflag == CSR_NOWAIT) {
		return(RT_SUCCESS);
		}
	//wait until process exits
	WaitForSingleObject(pi.hProcess,INFINITE);
	//get exit status from process
	GetExitCodeProcess(pi.hProcess,&retval);
	// Close process and thread handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return(retval);
}

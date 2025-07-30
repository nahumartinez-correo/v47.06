/*$
** MODULENAME - ctccsr.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.1 $
** $Date: 1996/04/23 15:41:17 $
**
**	Synopsis:
**
**	External Function List:
**
**	Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/
#include "pch.h"

#include <defines.h>
#include <typedefs.h>
#include <rtdef.h>
#include <csrdef.h>
#include <rtprotos.h>
#include <convers.h>

// max DDE buffer size excluding "Service|Topic!Item" info
#define CTC_MAX_DDE_NAME	64			// max length of DDE name
#define CTC_MAX_BUF_SIZE	4096		// max size of CTC API call buffer
#define CTC_MAX_DDE_BUFF	(CTC_MAX_BUF_SIZE-(3*(CTC_MAX_DDE_NAME+1)))

static HANDLE rtdll()
{
	static HANDLE hrtdll;
	// this is so the makefile does not have to link the world in 
	if ( ! hrtdll )
		hrtdll = LoadLibrary( "rt.dll" );
	return hrtdll;
}

// OFB: CALL csr:CTC_SEND_DATA <no_parameters>
// This takes the buffer built using OFB COMM verbs and sends it 
// to the host via the CTC Transport (for moa-only environments) 
// or via the CTC T27 Emulation (for shared environments)
int ctc_send_data( int argc, rtobj *argv[] )
{
	static int (*pctcrt_send_data)();

	if ( ! pctcrt_send_data ) {
		if ( !(pctcrt_send_data = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_send_data" ) ) )
			return(-1);
		}
	return pctcrt_send_data();
}

// OFB: CALL csr:CTC_SET_MODE <ctc_window> <ctc_mode>
// This sets the CTC-Bridge/MosaicOA operating mode
//  - ctc_window is either CTC_FRONT, CTC_BACK, CTC_CURRENT
//  - ctc_mode is either CTC_NO_NOTIFY, CTC_NOTIFY, CTC_PASSTHRU, or 
//    CTC_STATUS_ONLY
int ctc_set_mode( int argc, rtobj *argv[] )
{
	static int (*pctcrt_set_mode)();
	int win, mode;

	if ( ! pctcrt_set_mode ) {
		if ( !(pctcrt_set_mode = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_set_mode" ) ) )
			return(-1);
		}
	if (rtobj2int(argv[0], &win) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[1], &mode) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	return pctcrt_set_mode( win, mode );
}

// OFB: CALL csr:CTC_SET_WINDOW <ctc_window>
// This positions the CTC-Bridge window front or back
//  - ctc_window is either CTC_FRONT or CTC_BACK
int ctc_set_window( int argc, rtobj *argv[] )
{
	static int (*pctcrt_set_window)();
	int win;

	if ( ! pctcrt_set_window ) {
		if ( !(pctcrt_set_window = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_set_window" ) ) )
			return(-1);
		}
	if (rtobj2int(argv[0], &win) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	return pctcrt_set_window( win );
}

// OFB: CALL csr:CTC_DDE_SERVICE <service_name>
// This sets up the DDE service name for subsequent 
// CTC_EXECUTE, CTC_POKE, and CTC_REQUEST calls
int ctc_dde_service( int argc, rtobj *argv[] )
{
	static int (*pctcrt_service)();
	char service[CTC_MAX_DDE_NAME + 1];

	if ( ! pctcrt_service ) {
		if ( !(pctcrt_service = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_service" ) ) )
			return(-1);
		}
	if (rtobj2str(argv[0], service, sizeof(service)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return pctcrt_service( service );
}

// OFB: CALL csr:CTC_DDE_TOPIC <topic_name>
// This sets up the DDE topic name for subsequent 
// CTC_EXECUTE, CTC_POKE, and CTC_REQUEST calls
int ctc_dde_topic( int argc, rtobj *argv[] )
{
	static int (*pctcrt_topic)();
	char topic[CTC_MAX_DDE_NAME + 1];

	if ( ! pctcrt_topic ) {
		if ( !(pctcrt_topic = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_topic" ) ) )
			return(-1);
		}
	if (rtobj2str(argv[0], topic, sizeof(topic)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return pctcrt_topic( topic );
}

// OFB: CALL csr:CTC_EXECUTE <command>
// This calls CTC-Bridge to Execute the DDE command string
int ctc_execute( int argc, rtobj *argv[] )
{
	static int (*pctcrt_execute)();
	char command[CTC_MAX_DDE_BUFF];

	if ( ! pctcrt_execute ) {
		if ( !(pctcrt_execute = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_execute" ) ) )
			return(-1);
		}
	if (rtobj2str(argv[0], command, sizeof(command)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return pctcrt_execute( command );
}

// OFB: CALL csr:CTC_POKE <item> <value>
// This calls CTC-Bridge to Poke the DDE item value
int ctc_poke( int argc, rtobj *argv[] )
{
	static int (*pctcrt_poke)();
	char item[CTC_MAX_DDE_NAME + 1];
	char value[CTC_MAX_DDE_BUFF];

	if ( ! pctcrt_poke ) {
		if ( !(pctcrt_poke = (int (*)())GetProcAddress( rtdll(), 
			"ctcrt_poke" ) ) )
			return(-1);
		}
	if (rtobj2str(argv[0], item, sizeof(item)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2str(argv[1], value, sizeof(value)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return pctcrt_poke( item, value );
}

// OFB: CALL csr:CTC_REQUEST <item> <value>
// This calls CTC-Bridge to Request the DDE item value
int ctc_request( int argc, rtobj *argv[] )
{
	static char *(*pctcrt_request)();
	char item[CTC_MAX_DDE_NAME + 1];
	char *value;

	if ( ! pctcrt_request ) {
		if ( !(pctcrt_request = (char *(*)())GetProcAddress( rtdll(), 
			"ctcrt_request" ) ) )
			return(-1);
		}
	if (rtobj2str(argv[0], item, sizeof(item)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (argv[1]->obj.object != CSRARG_FLD)		// not a field error out
		return(CSR_FAIL_ARGTYPE);

	value = pctcrt_request( item );
	if (!value)
		return RTerrno;
	if (str2field(value, argv[1]->ptr) == RT_ERROR)	// put string back in the field
		return(CSR_FAIL_ARGTYPE);
	return(1);
}


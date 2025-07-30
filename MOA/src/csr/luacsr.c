/*$
** MODULENAME - luacsr.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.4 $
** $Date: 1996/03/08 19:38:10 $
**
** Synopsis:
**   ??
**
** External Function List:
**
** Notes:
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

// This is the first callout from OFB for LUA send
// It takes 2 parameters
//  param1 message_type see luaofb.h
//  param2 message_flags see luaofb.h

int lua_send_data( int argc,rtobj *argv[])
{
	static int (*pcomm_lua_send)();
	static HANDLE rtdll;
	unsigned flags, msgtype;

// this is some BS so the makefile does not have to link the world in 
	if ( ! rtdll ) {
		rtdll = LoadLibrary( "rt.dll" );
		if ( !(pcomm_lua_send = (int (*)())GetProcAddress( rtdll, "comm_lua_send" ) ) )
			return(-1);
		}
	if (rtobj2int(argv[0], &msgtype) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[1], &flags) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	return pcomm_lua_send( msgtype, flags );
}


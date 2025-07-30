//
// MODULENAME - dbcsr.c
//
// $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
// $Revision: 1.9 $
// $Date: 1996/03/08 19:37:32 $
//
// Synopsis:
//   Load Dynamic Link Library for DataBase
//
#include	"pch.h"

#include	<ppdefs.h>
#include	<defines.h>
#include	<typedefs.h>
#include	<rtdef.h>
#include	<csrdef.h>
#include	<rtprotos.h>
#include	<convers.h>
#include	<misc.h>
#include	<rtdef.h>
#include	<dbuser.h>
#include 	<er.h>
#include	<dberrs.h>

static 	FARPROC DBconnect_procaddr;

XDLL void
dbstart(int max_connects)
{
	static 	HANDLE hDBdll;
	FARPROC procaddr;
	if (hDBdll == NULL) {
		hDBdll = LoadLibrary("db.dll");
		if (hDBdll == NULL) {
			DWORD dwError = GetLastError();
			char szBuff[25];
			wsprintf(szBuff,"%d",dwError);
			MessageBox(NULL,szBuff,"Error",MB_OK);
			ersysn(DBDLLNOTLOADED); // "dbstart: unable to locate \"db.dll\"\n");
		}
	}
	procaddr = GetProcAddress(hDBdll,"DBstartup");
	if (procaddr == NULL)
		erbugn(DBDLLNOPROCADDRESS,"DBstartup", GetLastError());
	DBconnect_procaddr = GetProcAddress(hDBdll,"DBconnect");
	if (DBconnect_procaddr == NULL)
		erbugn(DBDLLNOPROCADDRESS,"DBconnect", GetLastError());
	//DBstartup(max_connects, NULL /* error handler function here*/);
	(*procaddr)(max_connects, NULL /* error handler function here*/);
}

XDLL int
odbc_start(argc,argv)
int argc;
rtobj *argv[];
{	// Arguments are environment_handle_field (output), dll_name (input), max_connects (input)
	field *result_env;
	char dll_name[32];
	int max_connects, argn=0;
	int retcode;
	static 	HANDLE hDBdll;
	FARPROC procaddr;

	if (argc != 3)
		return (CSR_FAIL_ARGCOUNT);

	/* make sure the target field is usable	*/
	if (argv[0]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	result_env = argv[argn++]->ptr;
	if (result_env->flags.type != FT_INT)
		return(CSR_FAIL_ARGTYPE);		
	if (rtobj2str(argv[argn++], dll_name, sizeof(dll_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[argn++], &max_connects) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (hDBdll == NULL) {
		hDBdll = LoadLibrary("odb.dll");
		if (hDBdll == NULL) {
			DWORD dwError = GetLastError();
			char szBuff[25];
			wsprintf(szBuff,"%d",dwError);
			MessageBox(NULL,szBuff,"Error",MB_OK);
			ersysn(DBDLLNOTLOADED); // "dbstart: unable to locate \"db.dll\"\n");
		}
	}
	procaddr = GetProcAddress(hDBdll,"DBstartup");
	if (procaddr == NULL)
		erbugn(DBDLLNOPROCADDRESS,"DBstartup", GetLastError());
	if (DBconnect_procaddr == NULL)
		DBconnect_procaddr = GetProcAddress(hDBdll,"DBconnect");
	if (DBconnect_procaddr == NULL)
		erbugn(DBDLLNOPROCADDRESS,"DBconnect", GetLastError());
	//DBstartup(max_connects, NULL /* error handler function here*/);
	if (retcode = (*procaddr)(result_env, max_connects, dll_name, NULL /* error handler function here*/))
		return(retcode);
	return(0);
}


XDLL int
odbc_connect(int argc, rtobj *argv[])
{  /*Arguments are:
  out: connection_handle_field,  
  in: environment_handle, dbms_product_name, server_name, server_machine, user_name, password */
	int argn=0;
	field *result_dbc_handle_field;
	int environment_handle;
	char dbms_name[32], server_name[32];
	char server_machine_name[32],database_name[32];
	char user_name[32];
	char password[32]; 

	if (argc != 8)
		return (CSR_FAIL_ARGCOUNT);

	/* make sure the target field is usable	*/
	if (argv[argn]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	result_dbc_handle_field = argv[argn++]->ptr;   // connection handle result field
	if (result_dbc_handle_field->flags.type != FT_INT)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[argn++], &environment_handle) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (environment_handle == 0)
		return(DBDLLSETUPFIRST);
	if (rtobj2str(argv[argn++], dbms_name, sizeof(dbms_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2str(argv[argn++], server_name, sizeof(server_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2str(argv[argn++], server_machine_name, sizeof(server_machine_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2str(argv[argn++], database_name, sizeof(database_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2str(argv[argn++], user_name, sizeof(user_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (argv[argn]==NULL)
		password[0] = 0;
	else if (rtobj2str(argv[argn++], password, sizeof(password)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (DBconnect_procaddr == NULL)
		return(DBDLLSETUPFIRST);

	return((*DBconnect_procaddr)(result_dbc_handle_field,environment_handle,RTddh,dbms_name,server_name,server_machine_name,database_name,user_name,password));
}

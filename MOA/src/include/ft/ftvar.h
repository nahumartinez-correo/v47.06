#ifndef FTVAR_H
#define FTVAR_H
/*
 *	copyright (c) 1989	ISC System Corporation
 */

#ifndef _Hc
#include	<c.h>
#endif
#ifndef _Ffttypes
#include	<ft/ftop.h>
#endif

/*
 *	various ftdes 
 */

global_const	ftdes	fchar;
global_const	ftdes	fint;
global_const	ftdes	flong;
global_const	ftdes	faddr;
global_const	ftdes	fstring;
global_const	ftdes	farray;
global_const	ftdes	fdtpar;
global_const	ftdes	fnull;

/*
 *	various fpardes function parameter tables
 */

global_const	fpardes	psvsetup[];
global_const	fpardes	pCds_Select[];
global_const	fpardes	pCds_SelectnFetch[];
global_const	fpardes	pCds_Join[];
global_const	fpardes	pCds_End_Select[];
global_const	fpardes	pCds_Insert[];
global_const	fpardes	pBatchInsert[];
global_const	fpardes	pCds_Update[];
global_const	fpardes	pCds_Delete[];
global_const	fpardes	pCds_Find[];
global_const	fpardes	pCds_Find_n[];
global_const	fpardes	pCds_Unlock[];
global_const	fpardes	pCds_Compress[];
global_const	fpardes	pCds_CancelScan[];
global_const	fpardes	psvstop[];
global_const	fpardes psv_who[];
global_const	fpardes	pCds_Server_Names[];
global_const	fpardes pWork[];
global_const	fpardes p2pc[];
global_const	fpardes pCds_Lock_Table[];
global_const	fpardes pCds_Unlock_Table[];
global_const	fpardes pCds_Files[];
global_const	fpardes	pCds_Reset_Log[];
global_const	fpardes	pCds_Create_Table[];
global_const	fpardes	pCds_Alter_Table[];
global_const	fpardes	pCds_Rename_Table[];
global_const	fpardes	pCds_Drop_Table[];
global_const	fpardes	pCds_Drop_Index[];
global_const	fpardes	pCds_Create_Index[];
global_const	fpardes	pCds_Alter_Field[];
global_const	fpardes pCds_Table_Size[];
global_const	fpardes psv_stop[];
global_const	fpardes pshadow_setup[];
global_const	fpardes pshadow_verify[];
global_const	fpardes pBecome_Primary[];
global_const	fpardes	pCds_Tables[];
global_const	fpardes pcdsync[];
global_const	fpardes pPhase2[];
global_const	fpardes pPrepare[];
global_const	fpardes pCds_Update_All[];
global_const	fpardes pCds_Delete_All[];
global_const	fpardes	pCds_Project[];
global_const	fpardes	pBe_Shadow[];
global_const	fpardes	pGetConnectOption[];
global_const	fpardes	pSetConnectOption[];

#endif

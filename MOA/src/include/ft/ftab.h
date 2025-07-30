#ifndef FTAB_H
#define FTAB_H
/* FUNCTION TABLE FOR BOTH CLIENT AND SERVER *
*
*	Copyright 1989	ISC Systems Corporation 
*/
# ifndef	FTOP_H
#	include <ft/ftop.h>
# endif

#ifdef SERVER
#define FUNC(fn)	(FCAST) fn
#else
#define FUNC(fn)	FUNCNIL
#endif

	/* the following table has one entry per supported function.

		*************   IMPORTANT    *************

	   the function code should be the index into the table for this
	   function.

	   The first available code is '1'.
	   '0' is reserved to begin and end the list.
	   For example,  ftab [SVSETUP]  is the entry for `svsetup()'
	   and SVSETUP is 1. 

	   The function codes are in <ft/ftop.h>
	*/
GLOBAL_CONST	fdes	ftab1[]={
	{ 0, 0, (char *) 0, FUNCNIL, (fpardes *)0 },
	{ SVSETUP, 5, "CONNECT (set_up)", FUNC(svsetup), psvsetup},
	{ SELECT, 5, "SELECT", FUNC(Cds_Select), pCds_Select },
	{ END_SELECT, 1, "ENDSELECT", FUNC(Cds_End_Select), pCds_End_Select },
	{ SVINSERT, 4,	"INSERT", FUNC(Cds_Insert), pCds_Insert },
	{ REPLACE, 4,	"REPLACE", FUNC(Cds_Replace), pCds_Update },
	{ UPDATE_FLDS, 4, "UPDATE", FUNC(Cds_Update_Flds), pCds_Update },
	{ FIND_FIRST, 3, "FINDFIRST", FUNC(Cds_Find_First), pCds_Find },
	{ FIND_LAST, 3, "FINDLAST", FUNC(Cds_Find_Last), pCds_Find },
	{ FIND_NEXT, 3, "FINDNEXT", FUNC(Cds_Find_Next), pCds_Find },
	{ FIND_PREV, 3, "FINDPREV", FUNC(Cds_Find_Prev), pCds_Find },
	{ FIND_N, 3, "FINDN", FUNC(Cds_Find_n), pCds_Find_n },
	{ CANCEL_SCAN, 1, "cancel_scan", FUNC(Cds_Cancel_Scan), pCds_End_Select},
	{ SVDELETE, 2, "DELETE", FUNC(Cds_Delete), pCds_Delete },
	{ SVWHO, 1, "who", FUNC(sv_who), psv_who },
	{ UNLOCK, 1, "UNLOCK", FUNC(Cds_Unlock), pCds_End_Select },
	{ LOCK_TABLE, 2, "LOCKTABLE", FUNC(Cds_Lock_Table), pCds_Lock_Table},
	{ UNLOCK_TABLE, 2, "UNLOCKTABLE", FUNC(Cds_Unlock_Table), pCds_Unlock_Table},
	{ LIST_FILES, 0, "list_files", FUNC(Cds_Files), pCds_Files},
	{ LIST_TABLES, 0, "list_tables", FUNC(Cds_Tables), pCds_Tables},
	{ LIST_SV_NAMES, 1, "list_server_names", FUNC(Cds_Server_Names), pCds_Server_Names},
	{ RESET_LOG, 0, "reset_log", FUNC(Cds_Reset_Log), pCds_Reset_Log},
	{ COMPRESS, 2, "COMPRESS", FUNC(Cds_Compress), pCds_Compress },
	{ CREATE_TABLE, 4, "CREATETABLE", FUNC(Cds_Create_Table), pCds_Create_Table},
	{ ALTER_TABLE, 4, "ALTERTABLE", FUNC(Cds_Alter_Table), pCds_Alter_Table},
	{ RENAME_TABLE, 2, "RENAMETABLE", FUNC(Cds_Rename_Table), pCds_Rename_Table},
	{ COPY_TABLE_DEF, 2, "Copy_table_definition", FUNC(Cds_Copy_Table_Def), pCds_Rename_Table},
	{ DROP_TABLE, 1, "DROPTABLE", FUNC(Cds_Drop_Table), pCds_Drop_Table},
	{ CREATE_INDEX, 5, "Create_Index", FUNC(Cds_Create_Index), pCds_Create_Index},
	{ DROP_INDEX, 3, "Drop_Index", FUNC(Cds_Drop_Index), pCds_Drop_Index},
	{ CDS_COMMIT_WORK, 0, "COMMITWORK", FUNC(Cds_Commit), pWork},
	{ CDS_2_PHASE_COMMIT, 2, "2PhaseCommit", FUNC(Cds_2_Phase_Commit), p2pc},
	{ CDS_ROLLBACK, 0, "ROLLBACKWORK", FUNC(Cds_Rollback), pWork},
	{ CDS_TABLE_SIZE, 1, "Table_Size", FUNC(Cds_Table_Size), pCds_Table_Size },
	{ FETCH_AGAIN, 2, "FINDAGAIN", FUNC(Cds_Fetch_Again), pCds_Find },
	{ CDSYNC,3, "cdsync",FUNC(cdsync), pcdsync },
	{ UPDATE_ALL, 3, "Update_all", FUNC(Cds_Update_All), pCds_Update_All },
	{ SVDELETE_ALL, 2, "Delete All", FUNC(Cds_Delete_All), pCds_Delete_All },
	{ CDS_PROJECT, 2, "Project Context", FUNC(Cds_Project), pCds_Project },
	{ SVSTOP, 0, "stop", FUNC(sv_stop), psv_stop},
	{ BECOME_PRIMARY,0, "Become_Primary",FUNC(become_primary), pBecome_Primary },
	{ CDS_COMPLETE_PHASE2, 2, "Phase 2 completion", FUNC(complete_phase2), pPhase2},
	{ CDS_JOIN, 5, "JOIN", FUNC(Cds_Join), pCds_Join },
	{ CDS_SELECT_FETCH, 5, "SelectnFetch", FUNC(Cds_SelectnFetch), pCds_SelectnFetch },
	{ CDS_TEST_CONNECTION,0,"IsConnected", FUNC(svIsConnected), pWork },
	{ CDS_BATCH_INSERT,3,"batchInsert", FUNC(svBatchInsert), pBatchInsert },
	{ CDS_TABLE_SIZES,0,"cdsTableSizes", FUNC(cdsTableSizes), pCds_Tables },
	{ CDS_OOB,0,"out of band", FUNC(sv_oob), pWork },
	{ CDS_UNION, 5, "UNION", FUNC(sv_union), pCds_Select },
	{ OPEN_CDS_CAPTURE,0,"open capture", FUNC(open_capture), pBecome_Primary },
	{ CLOSE_CDS_CAPTURE,0,"close capture", FUNC(close_capture), pBecome_Primary },

#ifdef INCLUDE_SERVER_ONLY_MESSAGES
	{ SHADOW_SETUP, 5, "Shadow_setup", FUNC(shadow_setup), pshadow_setup },
	{ SHADOW_VERIFY, 1, "Shadow_verify", FUNC(shadow_verify), pshadow_verify },
	{ SHADOW_WATCH, 0, "Shadow_watch", FUNC(shadow_watch), psv_stop },
	{ BE_SHADOW,2, "Be_Shadow",FUNC(be_shadow), pBe_Shadow },
	{ CDS_PREPARE, 2, "Prepare", FUNC(prepare), pPrepare},
	{ CDS_PREPARED, 2, "Prepare response", FUNC(prepared), pPhase2 },
	{ CDS_COMPLETED_PHASE2, 2, "Phase 2 completed", FUNC(completed_phase2), pPhase2},
	{ CDS_PARTNER_SETUP, 5, "partner set_up", FUNC(partner_setup), psvsetup},
#endif
	{ 0, 0, (char *) 0, FUNCNIL, (fpardes *)0 }
};
GLOBAL_CONST	fdes	ftab2[]={
	{ 0, 0, (char *) 0, FUNCNIL, (fpardes *)0 },
	{ SVSETUP, 5, "CONNECT (set_up)", FUNC(svsetup), psvsetup},
	{ SELECT, 5, "SELECT", FUNC(Cds_Select), pCds_Select },
	{ END_SELECT, 1, "ENDSELECT", FUNC(Cds_End_Select), pCds_End_Select },
	{ SVINSERT, 4,	"INSERT", FUNC(Cds_Insert), pCds_Insert },
	{ REPLACE, 4,	"REPLACE", FUNC(Cds_Replace), pCds_Update },
	{ UPDATE_FLDS, 4, "UPDATE", FUNC(Cds_Update_Flds), pCds_Update },
	{ FIND_FIRST, 3, "FINDFIRST", FUNC(Cds_Find_First), pCds_Find },
	{ FIND_LAST, 3, "FINDLAST", FUNC(Cds_Find_Last), pCds_Find },
	{ FIND_NEXT, 3, "FINDNEXT", FUNC(Cds_Find_Next), pCds_Find },
	{ FIND_PREV, 3, "FINDPREV", FUNC(Cds_Find_Prev), pCds_Find },
	{ FIND_N, 3, "FINDN", FUNC(Cds_Find_n), pCds_Find_n },
	{ CANCEL_SCAN, 1, "cancel_scan", FUNC(Cds_Cancel_Scan), pCds_End_Select},
	{ SVDELETE, 2, "DELETE", FUNC(Cds_Delete), pCds_Delete },
	{ SVWHO, 1, "who", FUNC(sv_who), psv_who },
	{ UNLOCK, 1, "UNLOCK", FUNC(Cds_Unlock), pCds_End_Select },
	{ LOCK_TABLE, 2, "LOCKTABLE", FUNC(Cds_Lock_Table), pCds_Lock_Table},
	{ UNLOCK_TABLE, 2, "UNLOCKTABLE", FUNC(Cds_Unlock_Table), pCds_Unlock_Table},
	{ LIST_FILES, 0, "list_files", FUNC(Cds_Files), pCds_Files},
	{ LIST_TABLES, 0, "list_tables", FUNC(Cds_Tables), pCds_Tables},
	{ LIST_SV_NAMES, 1, "list_server_names", FUNC(Cds_Server_Names), pCds_Server_Names},
	{ RESET_LOG, 0, "reset_log", FUNC(Cds_Reset_Log), pCds_Reset_Log},
	{ COMPRESS, 2, "COMPRESS", FUNC(Cds_Compress), pCds_Compress },
	{ CREATE_TABLE, 4, "CREATETABLE", FUNC(Cds_Create_Table), pCds_Create_Table},
	{ ALTER_TABLE, 4, "ALTERTABLE", FUNC(Cds_Alter_Table), pCds_Alter_Table},
	{ RENAME_TABLE, 2, "RENAMETABLE", FUNC(Cds_Rename_Table), pCds_Rename_Table},
	{ COPY_TABLE_DEF, 2, "Copy_table_definition", FUNC(Cds_Copy_Table_Def), pCds_Rename_Table},
	{ DROP_TABLE, 1, "DROPTABLE", FUNC(Cds_Drop_Table), pCds_Drop_Table},
	{ CREATE_INDEX, 5, "Create_Index", FUNC(Cds_Create_Index), pCds_Create_Index},
	{ DROP_INDEX, 3, "Drop_Index", FUNC(Cds_Drop_Index), pCds_Drop_Index},
	{ CDS_COMMIT_WORK, 0, "COMMITWORK", FUNC(Cds_Commit), pWork},
	{ CDS_2_PHASE_COMMIT, 2, "2PhaseCommit", FUNC(Cds_2_Phase_Commit), p2pc},
	{ CDS_ROLLBACK, 0, "ROLLBACKWORK", FUNC(Cds_Rollback), pWork},
	{ CDS_TABLE_SIZE, 1, "Table_Size", FUNC(Cds_Table_Size), pCds_Table_Size },
	{ FETCH_AGAIN, 2, "FINDAGAIN", FUNC(Cds_Fetch_Again), pCds_Find },
	{ CDSYNC,3, "cdsync",FUNC(cdsync), pcdsync },
	{ UPDATE_ALL, 3, "Update_all", FUNC(Cds_Update_All), pCds_Update_All },
	{ SVDELETE_ALL, 2, "Delete All", FUNC(Cds_Delete_All), pCds_Delete_All },
	{ CDS_PROJECT, 2, "Project Context", FUNC(Cds_Project), pCds_Project },
	{ SVSTOP, 0, "stop", FUNC(sv_stop), psv_stop},
	{ BECOME_PRIMARY,0, "Become_Primary",FUNC(become_primary), pBecome_Primary },
	{ CDS_COMPLETE_PHASE2, 2, "Phase 2 completion", FUNC(complete_phase2), pPhase2},
	{ CDS_JOIN, 5, "JOIN", FUNC(Cds_Join), pCds_Join },
	{ CDS_SELECT_FETCH, 5, "SelectnFetch", FUNC(Cds_SelectnFetch), pCds_SelectnFetch },
	{ CDS_TEST_CONNECTION,0,"IsConnected", FUNC(svIsConnected), pWork },
	{ CDS_BATCH_INSERT,3,"batchInsert", FUNC(svBatchInsert), pBatchInsert },
	{ CDS_TABLE_SIZES,0,"cdsTableSizes", FUNC(cdsTableSizes), pCds_Tables },
	{ CDS_OOB,0,"out of band", FUNC(sv_oob), pWork },
	{ CDS_UNION, 5, "UNION", FUNC(sv_union), pCds_Select },
	{ OPEN_CDS_CAPTURE,0,"open capture", FUNC(open_capture), pBecome_Primary },
	{ CLOSE_CDS_CAPTURE,0,"close capture", FUNC(close_capture), pBecome_Primary },
	{ CDS_PREPARE, 2, "Prepare", FUNC(prepare), pPrepare},
	{ CDS_PREPARED, 2, "Prepare response", FUNC(prepared), pPhase2 },
	{ CDS_COMPLETED_PHASE2, 2, "Phase 2 completed", FUNC(completed_phase2), pPhase2},
	{ CDS_PARTNER_SETUP, 5, "partner set_up", FUNC(partner_setup), psvsetup},
	{ CDS_SetConnectOption, 2, "SetConnectOption", FUNC(svSetConnectOption), pSetConnectOption},
	{ CDS_GetConnectOption, 1, "GetConnectOption", FUNC(svGetConnectOption), pGetConnectOption},
	{ CDS_SetStmtOption, 2, "SetStmtOption", FUNC(svSetStmtOption), pSetConnectOption},
	{ CDS_GetStmtOption, 1, "GetStmtOption", FUNC(svGetStmtOption), pGetConnectOption},
#ifdef INCLUDE_SERVER_ONLY_MESSAGES
	{ SHADOW_SETUP, 5, "Shadow_setup", FUNC(shadow_setup), pshadow_setup },
	{ SHADOW_VERIFY, 1, "Shadow_verify", FUNC(shadow_verify), pshadow_verify },
	{ SHADOW_WATCH, 0, "Shadow_watch", FUNC(shadow_watch), psv_stop },
	{ BE_SHADOW,2, "Be_Shadow",FUNC(be_shadow), pBe_Shadow },
#endif
	{ 0, 0, (char *) 0, FUNCNIL, (fpardes *)0 }
};
GLOBAL_CONST	short	Interface_Version=37;
/* changed to 37 for SQL cursor logic */
/* changed to 36 for longer table names */
/* changed to 35 for more information about dictionary identity */
/* changed to 34 for new functions open_capture and close_capture */
/* changed to 33 for new function sv_oob() */
/* changed to 32 for new function cdsTableSizes() */
/* changed to 31 for new parameters for BE_SHADOW */
#endif

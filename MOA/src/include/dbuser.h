#ifndef _DBUSER_H
#define _DBUSER_H

#ifdef WITHIN_ODB
#define DBXDLL __declspec(dllexport) 
#else
#define DBXDLL __declspec(dllimport) 
#endif
#ifndef _DBERRS_H
#	include <dberrs.h>
#endif
#define NOTSUPPORTEDYET 5620
#define NULL_INDICATOR -1
#define FT_IND_COLUMN_LENGTH (3+1 + NAME_KEYSIZE +1 + NAME_KEYSIZE +1)
		// for type_name +comma + application_name + colon + item name + null terminator

typedef enum {DB_DYNASET,DB_QUERY_DEF,DB_SNAPSHOT,DB_NO_OUTPUT} qtype ;

typedef int (*error_handler_function)(void *dbc_handle, int error_number, int native_error, char *msg_text); 

DBXDLL int DBstartup(field *env_fld, unsigned max_connects, char *name, error_handler_function catastrophe);
DBXDLL int DBsetDebug(int mask);  // always returns 0.
// see mask value macros in dberrs.h
DBXDLL int DBsetTrace(void *dbc, bool opt);  /* opt == 0 means turn off, else on */
// sets ODBC trace, usually to \sql.log

DBXDLL int DBgetInfo(void *dbc, unsigned int fInfoType, rtobj *obj);

DBXDLL int DBconnect(field *dbc_handle, int envHandle, int ddhandle, const char *rdbms_type,
	const char *server_name,const char *server_machine,	const char *database_name,
	 const char *user_name, char *password);
// max_connects is used only on the very first call to DBconnect to determine
// the size of the dbc free list.  If you pass zero or 1, 1 is used.

DBXDLL int DBdisconnect(void *dbc);

DBXDLL int DBbegin(void *dbc);	 // start Logical Unit of Work  (LUW)
DBXDLL int DBcommit(void *dbc);	 // commit LUW
DBXDLL int DBrollback(void *dbc);	 // rollback LUW
DBXDLL int DBinLUW(void *dbc);     // return 1 if in LUW, 0 if not, other values indicate error
DBXDLL int DBsetTimeoutDefaults(void *dbc, int a_time_out_seconds, int r_time_out_seconds);
DBXDLL int DBsetConnectOption(void *dbc, int fOption, int vParam);

DBXDLL char *DBmsg(void *dbc);	 // return message text for last error
DBXDLL int DBlastmsg(rtobj *msgfld, void *dbc); // put text for last error in msg field

#define ODB_LIST_MEMBER_MAX_ELEMENTS 32
DBXDLL int DBretreive(void *dbc, int element_index, rtobj *projection_list, char *tables, char *where_clause, rtobj *inparms);

DBXDLL int DBsqlExecute(void *dbc, char *sql_text);
DBXDLL int DBdoQuery(void *dbc, unsigned rows, const char *sql_text, ...);
DBXDLL int DBdefineQuery(rtobj *q_def_handle,void *dbc, const char *name, unsigned rows, const char *sql_text, ...);
DBXDLL int dbdefine_query_inner(void **rq, void *dbc, const char *query_name, qtype query_type,
	unsigned rows, const char *sql_text, bool bookmarks, field *input_flds[], field *output_flds[]);

DBXDLL int DBcreateQueryLst(rtobj *fldobj,void *dbc, const char *tables, const char *name, unsigned buffer_rows, 
	bool bookmark, rtobj *projection_list, int element_index, bool do_ofb, const char *where_clause, rtobj *inparms, ...); 
DBXDLL int DBmultiDisplay(rtobj *qfieldobj, void *dbc, const char *query_name, int flags, rtobj *list, LPSTR tables, LPSTR where_clause, rtobj *inparms, ...);
DBXDLL void *DBqdefOwner(void *query_def); // returns dbc that owns query_def
DBXDLL int DBqBufferRows(void *query_def); // returns number of buffer rows or negative error
DBXDLL int DBqRowsFound(void *query_def); // returns number of rows currently in buffer or negative error
DBXDLL int DBrowsAffected(void *query_def);  // returns number of rows affected or negative on error
DBXDLL int DBsetQTimeout(void *query_def, int time_out_seconds);

DBXDLL int DBqExecute(void *query_definition);
DBXDLL int DBqclose(void *query_definition);
DBXDLL int DBsetCurrentRowSubscript(void *query_definition, int new_row);
DBXDLL int DBCurrentRowSubscript(void *query_definition);

DBXDLL int DBcreateDynaset(rtobj *dynaset_handle,void *dbc, const char *name, unsigned buffer_rows, 
	bool bookmark, const char *sql_text, ...);   // a dynaset may have book marks
//DBXDLL int DBpositionToBookmark(field *dynaset_handle,void *bookmark_value);
	// Set current record to the previously saved bookmark.
DBXDLL int DBgetBookmark(void *query_def,rtobj *fld_to_hold_bookmark);
    // Save the bookmark of the current record.
DBXDLL int DBfetch(void *dynaset_handle);   // Fetch next buffer of record(s)
DBXDLL int DBexFetch(void *dynaset_handle, unsigned short int fetch_type, long irow);   // Fetch buffer of record(s)
DBXDLL int DBxFetch(void *dynaset_handle, unsigned short int fetch_type);   // Fetch buffer of record(s)
DBXDLL int DBextractRow(void *dynaset_handle, int row_index);   // extract row's column values into fields

DBXDLL int DBupdate(void *dynaset_handle);  // Update current record fields with dataentered flag on using field list of dynaset
DBXDLL int DB_update(void *dynaset_handle);  // Update current record using field list of dynaset (IGNORE DATA ENTERED FLAG)
DBXDLL int DBupdateFlds(void *dynaset_handle,const char *column_names, /*field *f1,*/ ...);
			  // Update current record from field list.
			  // column_names are optional if field names match column names.

DBXDLL int DBdelete(void *dynaset_handle);  // Delete current record
    // Note that update and delete of sets of records can be done via define_query and execute.
#define DBdsClose( dynaset ) DBqclose(dynaset)
DBXDLL int DBupdatable(void *dynaset_handle);  // returns updatable? property
DBXDLL int DBbookmarkable(void *query_def);  // returns Bookmarkable? property

DBXDLL int DBgetColumnValue(void *query_def, const char *column_name, rtobj *into_field);


// TABLES
DBXDLL int DBinsert(int envHandle,const char *table_name,const char *column_names,/*field *fld1,*/ ...);
//                       column_names may be NULL pointer

DBXDLL int DBinsert1(int envHandle, const char *table_name,byte applno, byte alternate_applno);
//        uses all column/field names defined for table at server. 
DBXDLL int DBinsertLst(int envHandle, const char *table_name, int element_index, rtobj *lst);

DBXDLL int DBcreateTable(void *dbconnect, int drop_existing, char *table_name, rtobj *fld_or_list,  ...);
DBXDLL int DBdropTable(const char *table_name, int environment);
// DBEXISTSTABLE return 0 if table exists
DBXDLL int DBexistsTable(const char *table_name, int envhandle);
#endif

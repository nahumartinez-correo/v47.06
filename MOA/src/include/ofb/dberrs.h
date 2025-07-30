#ifndef _DBERRS_H
#define _DBERRS_H

#ifndef __SQLEXT
/* from sqlext.h */
#define SQL_PARAM_INPUT                  1
#define SQL_PARAM_INPUT_OUTPUT           2
#define SQL_RESULT_COL                   3
#define SQL_PARAM_OUTPUT                 4
#endif

// FETCH TYPES	for DBexFetch()
#ifndef SQL_FETCH_NEXT
/* from sql.h or sqlext.h */
#define SQL_FETCH_NEXT			 1
#define SQL_FETCH_FIRST 		 2
#endif

#ifndef __SQLEXT
#define SQL_FETCH_LAST			 3
#define SQL_FETCH_PRIOR			 4
#define SQL_FETCH_ABSOLUTE		 5
#define SQL_FETCH_RELATIVE		 6
#define SQL_FETCH_BOOKMARK		 8
#endif

#define MOA_PUT_EMPTY_STRING_AS_NULL 2202 // option ID for DBsetConnectOption()

// LOGGING MASK VALUES FOR DBsetDebug()
#define DBLOGEX 1
#define DBLOGERRS 2
#define DBLOGLUW  4
#define DBLOGQD   8
#define DBLOGCONNECT 16
#define DBLOGQD_TEXT 32
#define DBLOGQD_FIELD_VALUE 64
#define DBAPI 128
#define DBLOGDYN 256
#define DBLOGFETCH 512
#define DBLOG_COLUMN_VALUE 1024	
#define DBLOG_MULTIDISPLAY 2048
#define DBLOG_ODBC_TRACE 4096    /* use driver manager trace, usually to \sql.log */
#define DB_TOKEN 8192
#define DBLOG_BUILDLIST 16384	 /* in multi-display */
#define DBLOG_CREATE 0x8000
#define DBUG_LIST_COLUMNS		0x10000
#define DBLOG_COLUMN_DIF		0x20000
#define DBLOG_CRASH_ON_BUG		0x40000  /* default is exit on encountering a bug */

#ifndef CDS_DEFAULT_APP_ID_OPTION

#define CDS_DEFAULT_APP_ID_OPTION 2200
#endif

// SQL result values
#define DB_NO_ROWS 1403
#define DB_NO_DATA 100   /* means no rows */

// error result values
#define DB_ERRS_BASE			5600
#define DB_NULL_HANDLE			DB_ERRS_BASE
#define DB_BAD_CONNECT_HANDLE	(DB_ERRS_BASE+1)
#define DB_BAD_HQD (DB_ERRS_BASE+2) // Invalid query_def handle to %s
#define DB03  (DB_ERRS_BASE+3) // query_def to %s, (%s) has no bound variables 
#define DB_NOT_FT_INT 	(DB_ERRS_BASE+4) //field to recieve object handle must be type FT_INT
#define DB05  (DB_ERRS_BASE+5)	//bad first character of input field name %s
#define DB06  (DB_ERRS_BASE+6)	//output field names don't match	//%s, %s
#define DB07  (DB_ERRS_BASE+7)	//field %s length %d < column maximum %d
#define DB_NO_BEGIN (DB_ERRS_BASE+8)	//Tried to commit without a begin
#define DB09  (DB_ERRS_BASE+9)	//Tried to rollback when not in LUW
#define DB10 (DB_ERRS_BASE+10)	//%s Unknown table '%s'
#define DB11  (DB_ERRS_BASE+11)	//Called %s without any connections
#define DB12  (DB_ERRS_BASE+12)	//Field parameter required for %s('%s', fld, ...)
#define DB13  (DB_ERRS_BASE+13)	//Bad %dth field parameter to %s(%s,... %x)
#define DB14  (DB_ERRS_BASE+14)	//Too many parameters to %s(%s, ....) Out of space.
#define DBDLLSETUPFIRST (DB_ERRS_BASE+15)
#define DB16  (DB_ERRS_BASE+16)	//Not enough input field parameters to %s(.,%s,'%s',..)
#define DB17  (DB_ERRS_BASE+17)	//Table '%s' not in connection '%s'
#define DB18  (DB_ERRS_BASE+18)	//zero columns from table '%s" on '%s'
#define DB19  (DB_ERRS_BASE+19)	//can't convert to string from decimal type field '%s' applno %d
#define DB20  (DB_ERRS_BASE+20)	//Not supported yet
#define DB21  (DB_ERRS_BASE+21)	//End of list of columns too soon in %s line %d
#define DB22  (DB_ERRS_BASE+22)	//max_column_length = %d in %s
#define DB23  (DB_ERRS_BASE+23)	//%s ocon returned %s
#define DB24  (DB_ERRS_BASE+24)	//%s call to get_tables() failed %s
#define DB25  (DB_ERRS_BASE+25)	//unsupported FT_ field data type %s
#define DB26  (DB_ERRS_BASE+26)	//corrupt list of table names on dbc %x, entry %d
#define DB27  (DB_ERRS_BASE+27)	//Only '%s' servers are supported by the DDL that is linked.
#define DB28  (DB_ERRS_BASE+28)	//Field '%s' which is type '%s' incompatible with DB type '%s'
#define DB29  (DB_ERRS_BASE+29)	//Number of rows in client side buffer must be at least 1
#define DB30  (DB_ERRS_BASE+30)	// %s, %s is Invalid database column type name
#define DB31  (DB_ERRS_BASE+31)	//Unsupported SQL external type '%s' for field '%s'
#define DB32  (DB_ERRS_BASE+32)	//Bulk field %s must be at least 16 bytes long to hold a row ID, or bookmark.
#define DB33  (DB_ERRS_BASE+33)	//Free_buf %x == bp %x
#define DB34  (DB_ERRS_BASE+34)	//qbuffer at %x has finder corrupted
#define DB35  (DB_ERRS_BASE+35)	//qbuffer at %x was already released
#define DB36  (DB_ERRS_BASE+36)	//out of buffer space in %s
#define DB37  (DB_ERRS_BASE+37)	//Can't get field name from %d and %x rterrno=%d, DDerrno=%d
#define DB38  (DB_ERRS_BASE+38)	//%s query '%s' rows > 0 and no output binding is non-sensical, sql='%s'
#define DB39  (DB_ERRS_BASE+39)	//Can't store column '%s' value '%s' in field %d '%s'
#define DB40  (DB_ERRS_BASE+40)	//Can't find table name for update of dynaset '%s'
#define DB41  (DB_ERRS_BASE+41)	//%s updated %d rows with '%s'
#define DB42  (DB_ERRS_BASE+42)	//bad name parameter %x to %s
#define DB43  (DB_ERRS_BASE+43)	//rows to %s must be 1 or zero.  query_name=%s, sql=%s
#define DB44  (DB_ERRS_BASE+44)	//%s rows must be <= %d
#define DB45  (DB_ERRS_BASE+45)	//%s requires a field paramter of type FT_ASCII
#define DB46  (DB_ERRS_BASE+46)	//can't convert to decimal from string type column '%s'
#define DB47  (DB_ERRS_BASE+47)	//Only dynasets can be updated, not '%s'
#define DB48  (DB_ERRS_BASE+48)	//Dynaset '%s' is not updatable.  For Oracle, use "for update" clause.
#define DB49  (DB_ERRS_BASE+49)	//Too many field parameters or not enough column names for %s(%s, ...)
#define DB50  (DB_ERRS_BASE+50)	//Invalid column name or number for %(%s,'%s', fld-%s)
#define DBDLLNOTLOADED (DB_ERRS_BASE+51)	//ODB.DLL not loaded
#define DBDLLNOPROCADDRESS (DB_ERRS_BASE+52)	// function address for %s() within .DLL not found
#define DB53  (DB_ERRS_BASE+53)	//row %d bigger than %s buffer rows %d
#define DB54  (DB_ERRS_BASE+54)	//%s finds that record set %s has no current row
#define DB55  (DB_ERRS_BASE+55)	//%s (%s, %d) row is greater than rows_found %d
#define DB56  (DB_ERRS_BASE+56)	//Max number of buffered rows in query is too small for multidisplay
#define DB57  (DB_ERRS_BASE+57)	//max items in multidisplay must be an even multiple of query buffer rows
#define DB_BAD_HENV (DB_ERRS_BASE+58)	//Invalid ODBC environment handle %lx.  Did you call DBstartup()?
#define DB59  (DB_ERRS_BASE+59)	//Can't set timeout option
#define DB_SQLALLOCCONNECT	(DB_ERRS_BASE+60)	//Can't SQLAllocConnect
#define DB61  (DB_ERRS_BASE+61)	//This set is not bookmarked
#define DB62  (DB_ERRS_BASE+62)	//ODBC interface bug	//%s
#define DB63  (DB_ERRS_BASE+63)	//SQLError failed, retval = %d
#define DB64  (DB_ERRS_BASE+64)	//ODBC max_connections must be in range 1 to %d
#define DB65  (DB_ERRS_BASE+65)	//Bad object type for parameter %d of %s
#define ODB_BAD_PARM 5665
#define DB66  (DB_ERRS_BASE+66)	//%s List_member_element_index must be in range 0 to %d
#define DB67  (DB_ERRS_BASE+67)	//%s Projection_List is for CDS_ONLY
#define DB68  (DB_ERRS_BASE+68)	//%s sees bad member at %x within compiled list at %x
#define DB69  (DB_ERRS_BASE+69)	//%s sees bad %dth member in compiled list. %s
#define DB70  (DB_ERRS_BASE+70)	//index element 1 must not be used for the OFB field reference 
#define DB71  (DB_ERRS_BASE+71)	//Dynasets must have bound output fields
#define DB72  (DB_ERRS_BASE+72)	// Invalid fetch type
#define DB73  (DB_ERRS_BASE+73)	// Parameter row less than rows_found
#define DB74  (DB_ERRS_BASE+74)	// no fields in compiled list
#define DB75  (DB_ERRS_BASE+75)	// Invalid negative row number for %s(%s, %d)
#define DB76  (DB_ERRS_BASE+76)	// Input variables in sql text must be fields, not constants.
#define DB77  (DB_ERRS_BASE+77)	// %s query '%s' rows < 1 with output binding is non-sensical, SQL='%s'.
#define DB78  (DB_ERRS_BASE+78)	// Can't decipher indirect value %d,%d : %d in field %d : '%s' 
#define DB79  (DB_ERRS_BASE+79)	// %s:OFB routine must be the first element of a list not %dth
#define DB80  (DB_ERRS_BASE+80)	// Execution of routine %d:%d was not successful
#define DB81  (DB_ERRS_BASE+81)	// Execution of routine %d:%d returned %d
#define DB82  (DB_ERRS_BASE+82)	// %fname SQLGetConnectOption(dbc->hdbc, SQL_CURRENT_QUALIFIER, '%s') returned %s
#define DB83  (DB_ERRS_BASE+83)	// The ODBC driver must be for ODBC version 2.0 or later. Your driver is version %s.
#define DB84  (DB_ERRS_BASE+84)	// Can't find corresponding type for FT_NUMERIC
#define DB85  (DB_ERRS_BASE+85)	// unknown or unsupported FT datatype %d on field %s
#define DB86  (DB_ERRS_BASE+86)	// Unsupported field definition modifier in '%s'
#define DB87  (DB_ERRS_BASE+87)	// Only up to %d input fields per table are allowed
#define DB88  (DB_ERRS_BASE+88)	// Primary key designation required on table '%s'
#define DB89  (DB_ERRS_BASE+89)	// can't convert CSRARG_STRING parameter in %s
#define DB90  (DB_ERRS_BASE+90)	// bad parameter_type value %d in %s for parm #%d
#define DB91  (DB_ERRS_BASE+91)	// Can't find corresponding SQL column type for field %s of data type %s, length %d, scale %d
#define DB92  (DB_ERRS_BASE+92)	// can't set trace option
#define DB93  (DB_ERRS_BASE+93)	// Time out parameters (seconds) must be greater than or equal to zero.
#define DB94  (DB_ERRS_BASE+94)	// out of buffer space in %s, for column %d, field %s, sql='%s'
#define DB95  (DB_ERRS_BASE+95)	// Bad date value %s from field %s, param %d
#define DB96  (DB_ERRS_BASE+96)	// reserved
#define DB97  (DB_ERRS_BASE+97)	// reserved
#define DB98  (DB_ERRS_BASE+98)	// reserved
#define DB99  (DB_ERRS_BASE+99)	// reserved
#endif							

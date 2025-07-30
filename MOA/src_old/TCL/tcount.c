/*****************
*  Test Client for counting record types
*
*/
#include "pch.h"
#include "rtdef.h"
#include <sql.h>
#include <sqlext.h>
#ifndef APIENTRY	// Windows NT defines APIENTRY, but 3.x doesn't
# define APIENTRY far pascal
#endif
#include <getopt.h>
#include "c.h"
#include "cdserror.h"
#include "clconn.h"
#include "cdsfuncs.h"

#include "defines.h"
#include "dataman.h"
#include "memalloc.h"
#include "clientdf.h"
#include "tcl.h"
#include "swerror.h"
#include "cmgetenv.h"
#include "misc.h"
#include "er.h"

int		wm_argc = 3;
char	*wm_argv[24];
	static clenv *env;
static char *column_record;


GLOBAL_PROC char	*type_name(char type)
{
	char	*type_string;
	static	char  type_number[5];

	switch(type)
	{
		case FT_ASCII: type_string="A   "; break;
		case FT_NUMERIC:	type_string="Num "; break;
		case FT_DATE:		type_string="Date"; break;
		case FT_TIME:		type_string="Time "; break;
		case FT_TIMESTAMP:	type_string="Timst"; break;
		case FT_INT:		type_string="Int "; break;
		case FT_FIXED:		type_string="Fixd"; break;
		case FT_FLAGS:		type_string="Flgs"; break;
		case FT_IND:		type_string="Ind "; break;
		case FT_BULK:		type_string="Bulk"; break;
		case FT_NULL:		type_string="NULL"; break;
		default:			
			sprintf(type_number,"%d  ",type);
			type_string = type_number;
			break;
	}
	return(type_string);
}

HINSTANCE	globhprevinstance;
int APIENTRY WinMain(
	HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{

	void	my_main(int, char **);
	int		lastwaswhite = 1;
	char	*parmptr;

	globhinstance = hInstance;
	globhprevinstance = hPrevInstance;
	globncmdshow = nCmdShow;

	wm_argc = 1;
	wm_argv[0] = "testluw";
	parmptr = lpCmdLine;
	while (*parmptr) {
		if (*parmptr == ' ' || *parmptr == '\t') {
			lastwaswhite = 1;
			*parmptr = '\0';
			}
		else {
			/* non-white, new parm if last char was white	*/
			if (lastwaswhite) {
				wm_argv[wm_argc++] = parmptr;
				lastwaswhite = 0;
				}
			}
		parmptr++;
		}

	my_main(wm_argc, wm_argv);

	return 0;
	lpCmdLine; // This will prevent 'unused formal parameter' warnings
}

struct rlist {
	struct rlist *next;
	int count;
	char rec[2];
} *types_list;

bool
same_record_type(addr old_rec, addr test_rec)
{
	char	*Optr = old_rec + sizeof(reclen);
	char	*Tptr = test_rec + sizeof(reclen);
	char	*Oend, *Tend;
//	char	*type_string;
	fieldv	f, Tf;

	Oend = old_rec + RECLEN(old_rec);
	Tend = test_rec + RECLEN(test_rec);

	while (Optr < Oend)
	{
		if (!(Tptr < Tend))
			return FALSE;
		Optr = cmfield(&f, (byte *)Optr);
		Tptr = cmfield(&Tf, (byte *)Tptr);
		if (f.appl_id != Tf.appl_id
			|| f.field_id != Tf.field_id)
			return FALSE;
	}
	if (Tptr < Tend)
		return FALSE;
	return TRUE;
}

int
print_diff(HANDLE OutFH, int ddh, addr old_rec, addr test_rec)
{
	char	*Optr = old_rec + sizeof(reclen);
	char	*Tptr = test_rec + sizeof(reclen);
	char	*Oend, *Tend;
	char	*type_string;
	fieldv	f, Tf;
	bool found;
	int result=0;

	Oend = old_rec + RECLEN(old_rec);
	Tend = test_rec + RECLEN(test_rec);

	while (Tptr < Tend)
	{
		Tptr = cmfield(&Tf, (byte *)Tptr);
		found = FALSE;
		Optr = old_rec + sizeof(reclen);
		for (Optr = cmfield(&f, (byte *)Optr); Optr <= Oend; Optr = cmfield(&f, (byte *)Optr)) {
			if (f.appl_id == Tf.appl_id
				&& f.field_id == Tf.field_id) {
				found = TRUE;
				break;
			}
			if (Optr == Oend)
				break;
		}
		if (!found) {
			type_string = type_name(Tf.type);
			result++;
			if (ddh < 0 
			|| ptrnil(Tf.name =ddname(ddh, FLD, Tf.appl_id, Tf.field_id)))
				myprintf(OutFH, "ap#%2d fld#%-4d type:%s \n",
					Tf.appl_id,	Tf.field_id,	type_string);
			else
				myprintf(OutFH, "%d:%-20s %s \n",
					Tf.appl_id,	Tf.name,	type_string);
			if (Tf.appl_id == 0) {
				myprintf(OutFH, "\r\ninvalid field ID (appl == 0)\r\n");
				return result;
			}
		}
	}
	myprintf(OutFH, "\r\n");
	return(result);
}

int
store_record_type(addr rec)
{
	static int space_used=0;
	static int record_types=0;
	struct rlist *rec_list, *prev=NULL;

	for (rec_list=types_list; rec_list; rec_list = rec_list->next) {
		if (same_record_type(rec_list->rec, rec)) {
			rec_list->count++;
			return space_used;
		}
		prev = rec_list;
	}
	rec_list = mem_alloc(RECLEN(rec)+sizeof(struct rlist));
	rec_list->next = NULL;
	rec_list->count = 1;
	memcpy(rec_list->rec, rec, RECLEN(rec));
	space_used += (RECLEN(rec)+sizeof(addr));
	record_types++;
	if (prev)
		prev->next = rec_list;
	else
		types_list = rec_list;
	return space_used;
}

static int
BuildBaseRecord(const char *table_name, char *service_name)
{
	int result, len;
	char szScript[256];
	UDWORD row=0, rowCount, rowTotal=0, buffRow;
	field	*ddf;		/* a PCL/rt field structure */
	CDS_column_info col[MAX_REQUIRED_FIELDS+1];
	static const char fname[]="get_columns";
	HANDLE hdbc, henv, hstmt;

	len = sprintf(szScript, 
"select CdsFldName, CdsApplNo, CdsFldNo from fields colF, tables colT where colT.Cds_table_name = '%s' and colF.Cds_table_no = colT.Cds_table_no", table_name);

	if (SQLAllocEnv(&henv) != SQL_SUCCESS)
		erspace(" SQLAllocEnv");
	if (SQLAllocConnect(henv, &hdbc) != SQL_SUCCESS)
		erbug("AllocConnect");
	result = SQLConnect(hdbc, service_name, SQL_NTS, /* Connect to data source */
	                     "", SQL_NTS, "", SQL_NTS); 
	if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO){
		ersys("bad connect with %x, %s", hdbc, service_name);
	}
	if (SQLAllocStmt(hdbc, &hstmt))
		erbugn(5409, fname);

	//SQLSetStmtOption(hstmt, CDS_DEFAULT_APP_ID_OPTION, env->common_id);
	if (result = SQLExecDirect(hstmt, szScript, len))
		goto err_exit;
	result = SQLSetStmtOption(hstmt, SQL_ROWSET_SIZE, MAX_REQUIRED_FIELDS);
	while (result == 0) {
		result = SQLExtendedFetch(hstmt,SQL_FETCH_NEXT,0,&rowCount,NULL);
		if (!result) {
			rowTotal += rowCount;
			for (buffRow=1; buffRow <= rowCount; buffRow++, row++) {
				result = SQLSetPos(hstmt, (ushort) buffRow, SQL_POSITION, SQL_LOCK_NO_CHANGE);
				if (result)
					break;
				if (row > MAX_REQUIRED_FIELDS)
					erbugn(5438, row, fname,__LINE__);

				if (result = SQLGetData(hstmt, 1, SQL_C_CHAR, col[row].name,NAME_KEYSIZE+1, NULL))
					break;
				if (result = SQLGetData(hstmt, 2, SQL_C_UTINYINT, &col[row].appl_id,1, NULL))
					break;
				if (result = SQLGetData(hstmt, 3, SQL_C_SSHORT, &col[row].field_id,2, NULL))
					break;
				if (!(ddf = ddget(env->RTddh,FLD,col[row].appl_id,col[row].field_id))) {
					result =  CDS_BAD_DDGET_FIELD;
					goto err_exit;
				}
				if (ddf->flags.array) {
					result = 5332; //Attempted to index on an array field %s:%s
					goto err_exit;
				}
				col[row].type = ddf->flags.type;	/* type is 4 bit element of flags */
				col[row].length = cdsFieldLength(ddf);
			}
		}
	}
	if (result == SQL_NO_DATA_FOUND && rowTotal) {
		char *ptr;

		result = 0;
		column_record = mem_alloc (sizeof(CDS_column_info) * (++rowTotal));
		col[row].appl_id = 0; // terminate list

		ptr = column_record+ sizeof(reclen);
		for (row=0;row<rowTotal  && col[row].appl_id != 0;row++) {
			put_appl(ptr, col[row].appl_id);
			put_field(ptr, col[row].field_id);
			*ptr++ = col[row].type;
			put_length(ptr,0);
		}
		SETRECLEN(column_record, (ptr-column_record));

		//memcpy(table->column, col, sizeof(CDS_column_info) * rowTotal);
		store_record_type(column_record);
	}
err_exit:
	SQLFreeStmt(hstmt, SQL_DROP);
	return result;	
}




/*******************************************
* main() of test program
********************************************/
void my_main(argc, argv)
int	argc;	 /* argument count */
char	*argv[]; /* arg[0] is program name */
/* arg[3] is name of table to read */
/* arg[4] is beginning search date yyyymmdd for context3 */
/* arg[5] is ending search date yyyynndd for context3 */
{
	const  char Usage[]="Usage: %s [-V][-nproject][-Sserver_name] [table_name [ODBC_DSN]]\r\n";
	char *table_name="journal";
	clconnect *ccb = 0;
	int log_level=0;
	char *debug_string;
	char *envfile;
	bool print_all=FALSE;
	cdsselect	 * context1;
	short	good=0, bad=0, totals_records=0;
	foundset *found;
	int space_used=0;
	reclen	record_length;
	ushort  record_count=0, i;
	char	*buffer, *server_name = 0;
	char	*start_date, *end_date;
	int	 per_buffer_request_count;
	char	*project=get_moacurproj();
	int		argn=1, opt;
	int record_types=0;
	long	time_now=time(NULL);
	struct	tm 	*now=localtime(&time_now);
	static	char	s_date[9]="19890415";
	static	char	e_date[9]="19890516";
	struct rlist *rec_list, *prev=NULL;
	char *service_name=NULL;
	
	OutFH = GetStdHandle(STD_OUTPUT_HANDLE);
	ErrFH = GetStdHandle(STD_ERROR_HANDLE);
	
	while ((opt = getopt(argc, argv, "Vn:r:S:")) != EOF)
	{
	/*	char	option, *options = argv[opt];	* for sdb debugging */
	/*	while (option = *options++)*/
			switch (opt)
			{
			case 'V':
				PrintRelInfo(NULL);
				return;
			case 'n':
				project=optarg;
				break;
			case 'S':
				server_name = optarg;
				break;
			default:
				printf(Usage,argv[0]);
				exit(15);
			}
	}
	argn = optind;
		
	if (argc > argn)
		table_name = argv[argn++];

	if (argc > argn)
		service_name = argv[argn++];

	//init_fields();	/* initialize fields*/

	if (argc > argn)
		start_date = argv[argn++];
	else
	{
   		sprintf(&s_date[0],"%04d",now->tm_year+1900);
 		sprintf(&s_date[4],"%02d",now->tm_mon);
 		sprintf(&s_date[6],"%02d",now->tm_mday-1);
		start_date = s_date;
	}
	if (argc > argn)
		end_date = argv[argn++];
	else
	{
 		sprintf(&e_date[0],"%04d",now->tm_year+1900);
 		sprintf(&e_date[4],"%02d",now->tm_mon);
 		sprintf(&e_date[6],"%02d",now->tm_mday+1);
		end_date = e_date;
	}

	if (strlen(end_date) !=8)
	{
		myprintf(OutFH,"end_date must be in yyyymmdd format\r\n");
		exit(2);
	}
	if (strlen(start_date) !=8)
	{
		myprintf(OutFH,"start_date must be in yyyymmdd format\r\n");
		exit(2);
	}
	env = clAllocEnv(project, QL);
	if (!env) {
		myprintf(OutFH,"Fatal error can't allocate CDS client environment\r\n");
		exit(13);
	}
	envfile = cmenvfile_name(project,server_name);
	if ((debug_string=cmgetenv("CDS_NWLOG", envfile))!=NULL) {
		log_level = atoi(debug_string);
		// Set Logging
		//MCNSetULog(log_level,1,0,0);
	}
	if (!Cds_Initial (env, server_name)) { /* initialize client/server or linked in server */
		myprintf(OutFH,"Fatal error %s",cdsErrMsg(env));
		exit(12);
	}
	if (env->cds_errno == CDS_FLD_DD_INCOMPATIBLE) {
		puts(cdsErrMsg(env));
		exit(1);
	}

	if (!(context1 = Cds_Select(env, NULL, table_name, (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	if (service_name)
		BuildBaseRecord(table_name,service_name);
	per_buffer_request_count = 64565/context1->tbl->buffsize + 1;
	for (found =Cds_Find_n(context1, per_buffer_request_count, 0); found && space_used < 4000000;
		found = Cds_Find_n(context1, per_buffer_request_count, 0))
	{
		record_count = (found->count);
		if (log_level)
			myprintf(ErrFH, "Find_n returned %d records\n",record_count);
		buffer = found->recs;
		for (i=1; i<=record_count; i++)	{
			space_used = store_record_type(buffer);
			if (space_used > 4000000) {
				myprintf(ErrFH, "space_used = %d\n", space_used);
				break;
			}
			record_length = RECLEN(buffer);
			buffer += record_length;
		}
	}
	if (env->cds_errno != CDS_RECORD_NOT_FOUND)	{
		myprintf(ErrFH, "Fetch on context1 failed on %s\r\n", cdsErrMsg(env));
		exit(1);
	}
	if (!Cds_End_Select(context1))
		erbug("End select failed on context1\r\n%s\r\n",cdsErrMsg(env));

	close_all_clconnects(env);

	for (rec_list=types_list; rec_list; rec_list = rec_list->next) {
		if (rec_list == types_list || print_all) {
			cdsprint(OutFH, rec_list->rec);
			myprintf(OutFH, "at least %d of these\n", rec_list->count);
		} 
		else {
			if (print_diff(OutFH, env->RTddh, types_list->rec, rec_list->rec)) {
				record_types++;
				myprintf(OutFH, "%d of these\n", rec_list->count);
			}
			else
				types_list->count += rec_list->count;
		}
	}

	if (record_count) {
		record_types++;
				myprintf(OutFH, "%d of the first record type\n", types_list->count);
	}
	myprintf(OutFH,"Table '%s' has %d record type%c\r\n",table_name,record_types, record_types == 1 ? ' ' : 's');
	for (rec_list=types_list; rec_list; rec_list = rec_list->next) {
		if (prev)
			mem_free(prev);
		prev = rec_list;
	}
	exit(0);
}

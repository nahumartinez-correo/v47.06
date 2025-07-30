/*****************
*  Test Client for cdserver Data Definition Language
*
*/
#include "pch.h"
#include "rtdef.h"
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
#include "er.h"
#include "swerror.h"
#include "tcl.h"
#ifndef SYSTEM_H
# include "h/system.h"
#endif
#include "mcnuser.h"
#include "misc.h"

int		wm_argc = 3;
char	*wm_argv[24];

char	criteria4[40] = "__f2:7 rmatch \".*F\"";
char	criteria7[60] = "__f2:1 == 1 && f2:2 == 1 && f2:4 >= 2 && f2:4 <= 3";
/*                     012345678901234567890123456 */

static table_options profile_opts={sizeof(table_options),6,0,2564,64,1,0,0};
static void create_serial(void)
{
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
	wm_argv[0] = "testddl";
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

#if 0
	return (msg.wParam); // Returns the value from PostQuitMessage
#else
	return 0;
#endif
	lpCmdLine; // This will prevent 'unused formal parameter' warnings
}

/*******************************************
* main() of test program
********************************************/
void my_main(argc, argv)
int	argc;	 /* argument count */
char	*argv[]; /* arg[0] is program name */
/* arg[1] is project name
** arg[2] is project level */
/* arg[3] is number of records to insert */
/* arg[4] is beginning search date yyyymmdd for context3 */
/* arg[5] is ending search date yyyynndd for context3 */
{
	static const  char Usage[]="Usage: %s [-V][-n project -S server_name -M maxlength -x debugsw] [limit [start_date [end_date]]]\r\n";
	int		found, int_result;
	static char	Number[10];
	cdsselect	 * context1,
			   *context2,
			   *context3,
			   *context4,
			   *context5,
			   *contextp,
			   *context7;
	char	*criteria2;
	char	*result, *server_name = 0;
	char	*number = Number;
	short	i, n, limit, good=0, bad=0;
	reclen	len;
	char	*buffer, *ptr, cx;
	char	*start_date, *end_date;
	int	 seq_int;
	char	*project = get_moacurproj(), *level="01";
	int		argn=1, opt;
	long	time_now;
	struct	tm 	*now;
	long	tics, start_tics;
	int			clock_tics;
	float	seconds;
	char *rbuffer;
	char *rbuff2;
	char	readbuffer[4];
	bool testpause=false;
	char index_list[128];
	static clenv *env;
	clconnect *ccb = 0;

	static	char	s_date[9]="19890415";
	static	char	e_date[9]="19890516";

	static table_options journal_opts={sizeof(table_options),94,46,3000,64,0,0,0};

	clock_tics = tics_per_second();
	time_now=time(NULL);
	now=localtime(&time_now);

	while ((opt = getopt(argc, argv, "VtM:n:r:S:x:")) != EOF)
	{
	/*	char	option, *options = argv[opt];	* for sdb debugging */
	/*	while (option = *options++)*/
		switch (opt){
		  case 'V':
			PrintRelInfo(NULL);
			return;
		  case 't':
			testpause=true;
			break;
		  case 'M': /* Max_length option */
			Max_length = (unsigned short) atol (optarg);
			if (Max_length > journal_opts.buffer_size)
			    journal_opts.buffer_size = Max_length;
			break;
		  case 'n':
			project=optarg;
			break;
		  case 'r':		/*revision level */
			level=optarg;
			break;
		  case 'S':
			server_name = optarg;
			break;
		  case 'x' :	/* the higher the 'arg' (1-9) the more trace	*/
			  /* information gets displayed on stderr and stdout*/
				debugsw = atoi(optarg);
				MCNSetULog(debugsw,1,0,0);
				break ;
		  default:
			myprintf(OutFH,Usage,argv[0]);
			exit(15);
		}
	}
	argn = optind;
		
	if (argc > argn)
		limit = atoi(argv[argn++]);
	else
		limit = 4;

	init_fields();	/* initialize fields*/

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
	if (!Cds_Initial (env, server_name)) { /* initialize client/server or linked in server */
		myprintf(OutFH,"Fatal error %s",cdsErrMsg(env));
		exit(12);
	}

	if (!translate_field_names(env))
		exit(2);

	build_criteria3(criteria3,start_date,end_date);

/*lint -e606 */
	sprintf(criteria4, "__f%d:%d rmatch \".*F\"",
		List[6].application, List[6].field_number);
	len = strlen(criteria4);
	SETARECLEN(criteria4,len);

	build_criteria5(criteria5,start_date,end_date);

	build_criteria6(criteria6);

	sprintf(criteria7,
			"__f%d:%d == 1 && f%d:%d == 1 && f%d:%d >= 2 && f%d:%d <= 3",
			Branch->application,Branch->field_number,
			Operator->application,Operator->field_number,
			Sequence_Number->application,Sequence_Number->field_number,
			Sequence_Number->application,Sequence_Number->field_number);
	len = strlen(criteria7);
	SETARECLEN(criteria7, len);

	if (!(Cds_Drop_Table(env, "journal"))){
		if (env->cds_errno != CDS_NO_SUCH_FILE)
			myprintf(OutFH,"Can't drop journal because %s",cdsErrMsg(env));
	}
	if (testpause) {
		myprintf(OutFH,"Paused .  Hit return key to continue.");
		fflush(stdout);
		read(STDIN,readbuffer,1);
	}

	if ((int_result = isConnected(env, NULL))== CDS_SERVICE_CONNECTED)
		myprintf(OutFH, "Still connected\r\n");
	else {
		myprintf(OutFH, "Not now connected to cdserver, result = %d\r\n", int_result);
		exit(1);
	}
	
	start_tics = GetTickCount();		/* start of function  */
	sprintf(index_list,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
		Branch->appl_name,Branch->field_name,"a",
		Operator->appl_name,Operator->field_name,"a",
		Bus_Date->appl_name,Bus_Date->field_name,"a",
		Sequence_Number->appl_name,Sequence_Number->field_name,"a");
	if (!(Cds_Create_Table(env,"journal",index_list, &journal_opts,	(char *) 0 ))){
		myprintf(OutFH,"Create journal table failed:\r\n\r%s",cdsErrMsg(env));
		exit(12);
	}
	if (testpause) {
		myprintf(OutFH,"Paused after create journal.  Hit return key to continue.");
		fflush(stdout);
		read(STDIN,readbuffer,1);
	}

	tics = (GetTickCount()) - start_tics;	/* end of function  */
/*lint -e736 */
	seconds = (float) 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for Create_Table journal\r\n", seconds);

	create_serial();

	start_tics = GetTickCount();		/* start of function  */
	if (!(Cds_Rename_Table(env, "journal", "fab")))
	{
		myprintf(OutFH,"rename of journal failed\r\n%s",cdsErrMsg(env));
		exit(13);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = (float) 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for Rename_Table \r\n", seconds);

	if (!(Cds_Rename_Table(env, "fab","journal")))
	{
		myprintf(OutFH,"rename of fab to journal failed\r\n%s",cdsErrMsg(env));
		exit(13);
	}
	
	if (!(Cds_Drop_Table(env,"profiles"))){
		if (env->cds_errno != CDS_NO_SUCH_FILE)
			myprintf(OutFH,"Can't drop profiles because %s",cdsErrMsg(env));
	}
	if (testpause) {
		myprintf(OutFH,"Paused after drop profiles.  Hit return key to continue.");
		fflush(stdout);
		read(STDIN,readbuffer,1);
	}

	sprintf(index_list,"%s,%s,%s,%s,%s,%s",
		Branch->appl_name,Branch->field_name,"a",
		Operator->appl_name,Operator->field_name,"a");
	if (!(Cds_Create_Table(env, "profiles", index_list, &profile_opts, NULL)))
	{
		myprintf(OutFH,"Create profiles table failed:\r\n\r%s",cdsErrMsg(env));
		exit(12);
	}
	if (testpause) {
		myprintf(OutFH,"Paused after create profiles.  Hit return key to continue.");
		fflush(stdout);
		read(STDIN,readbuffer,1);
	}

		
	if (!(Cds_Drop_Table(env, "totals"))){
		if (env->cds_errno != CDS_NO_SUCH_FILE)
			myprintf(OutFH,"Can't drop totals because %s",cdsErrMsg(env));
	}
	if (!(Cds_Create_Table(env, "totals", index_list, &profile_opts, NULL )))
	{
		myprintf(OutFH,"Create totals table failed:\r\n\r%s",cdsErrMsg(env));
		exit(12);
	}
		
	if (!(context1 = Cds_Select(env, NULL,"journal", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH,  cdsErrMsg(env));
		exit(11);
	}
	myprintf(ErrFH,  "context from select=%x\r\n", (VPTR)context1);

	/* build and insert as many records as desired for test */

	result = "";
	seq_int = atoi(&Sequence_number[0]) + 1;
	limit += seq_int;
	for (n =  seq_int; n <= limit && !ptrnil(result); n++) {
		/* put record number into record key */
		sprintf(number, "%d", n);
		Sequence.storage = (byte * ) number;
		Sequence.length = strlen(number);
		buffer = build_buffer();
		cdsprint(ErrFH,buffer);
		if (!(result = Cds_Insert(context1, buffer,WITH_LOCK, WORK))) {
			myprintf(ErrFH,  "Insert error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			myprintf(OutFH, "Insert error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			bad++;
			mem_free(buffer);
			if (good == 0)
				exit(12);
		}
		else
		{
			good++;
			mem_free(buffer);
		}
	}
/*     TESTBIG */
	myprintf(OutFH,"starting big record test\r\n"); fflush(stdout);
	sprintf(number, "%d", n);
	Sequence.storage = (byte * ) number;
	Sequence.length = strlen(number);
	Stuff.storage = (byte * ) mem_alloc(2200);
	memset(Stuff.storage,'x',2199);
	for (cx='0', ptr = (char *) Stuff.storage; ptr < (char *)Stuff.storage+ 2198; )
	{
		*ptr++ = cx++;
		if (cx > '9')
			cx = '0';
	}
	Stuff.storage[2199] = 0;
	Stuff.length = 2199;
	buffer = build_buffer();
	if (!(result = Cds_Insert(context1, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH,  "Insert error\r\n %s on big record\r\n", cdsErrMsg(env));
		myprintf(OutFH, "Insert error\r\n %s on big record\r\n", cdsErrMsg(env));
		bad++;
			exit(12);
	}
	else
		good++;
	
	myprintf(OutFH,"Sucessfully inserted %d records\r\n", good);

	/* prepare to update HostStat on last inserted record */
	HostStat.storage = (byte * ) "1";
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH,  "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*hoststat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH,  "buffer build failed");
		exit(12);
	}
	len = Jnlptr - Jnlbuf; /*set length in front*/
	SETARECLEN(Jnlbuf,len);

	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,WITHOUT_LOCKING,WORK))) {
		myprintf(ErrFH,  "at line %d Update_Flds context1 failed on %s\r\n", 
					__LINE__, cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH,  "Update_Flds succeeded\r\n");
		dmpbulk(ErrFH, Jnlbuf);
	}

	if (!(context3 = Cds_Select(env, NULL,"journal", "context3", criteria3,Ascending))) {
		myprintf(ErrFH,  cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context3,WITH_LOCK, false))) {
		myprintf(ErrFH,  "at line %d Find_Next on context 3 failed on:\r\n %s\r\n", 
			__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next context 3 succeeded\r\n");
		myprintf(ErrFH,  "Find_Next context 3 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (!(context7 = Cds_Select(env, NULL,"profiles", "context3", criteria7,Ascending))) {
		myprintf(ErrFH,  "GOOD %s\r\n",cdsErrMsg(env));
	}
	else
	{
		myprintf(ErrFH,  "ERROR context selection allowed at line %d\r\n",__LINE__-3);
		++bad;
	}

	if (!(context7 = Cds_Select(env, NULL,"journal", "context7", criteria7,Ascending))) {
		myprintf(ErrFH,  cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context7,WITH_LOCK, false))) {
		myprintf(ErrFH,  "at line %d Find_Next on context 7 failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next context 7 succeeded\r\n");
		myprintf(ErrFH,  "Find_Next context 7 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (! (Cds_End_Select(context7)))
	{
		myprintf(ErrFH,  "End_Select context 7 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	}

	if (ptrnil(buffer = Cds_Find_Next(context3,WITHOUT_LOCK, false))) {
		myprintf(ErrFH,  "Find_Next context3 2nd failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next context 3 2nd succeeded\r\n");
		myprintf(ErrFH,  "Find_Next context 3 2nd succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (!(context4 = Cds_Select(env, NULL,"journal", "context4",criteria4,Ascending))) {
		myprintf(ErrFH,  cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context4,WITH_LOCK, false))) {
		myprintf(ErrFH,  "at line %d Find_Next on context 4 failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next in context 4 succeeded\r\n");
		myprintf(ErrFH,  "Find_Next in context 4 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (ptrnil(buffer = Cds_Find_Next(context4,WITHOUT_LOCK, false))) {
		myprintf(ErrFH,  "Find_Next context4 line %d failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next succeeded\r\n");
		myprintf(ErrFH,  "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	criteria2 = build_criteria2();	/* using current date */
	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Descending))) {
		myprintf(ErrFH,  cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context2, WITHOUT_LOCK, false))) {
		if (env->cds_errno == CDS_RECORD_NOT_FOUND)
			strcpy(Sequence_number,"0");
		else
		{
			myprintf(ErrFH,  "line %d, Find_Next context2 failed on %s\r\n",__LINE__, cdsErrMsg(env));
			exit(1);
		}
	} else
	{
	 	cdsprint(ErrFH,buffer);
		if(!get_seq_num(buffer))
			myprintf(ErrFH, "Find_Next - did not find sequence number in buffer\r\n");
		else
		{
			myprintf(OutFH,"Find_Next context2 succeeded\r\n");
			myprintf(OutFH,"Find_Next - sequence number->%s\r\n",Sequence_number);
			myprintf(ErrFH,  "Find_Next context2 succeeded\r\n");
			myprintf(ErrFH, "Find_Next - sequence number->%s\r\n",Sequence_number);
			/* now test Find_Prev */
			if (!ptrnil(buffer = Cds_Find_Prev(context2, WITHOUT_LOCK, false))) {
				myprintf(ErrFH,  "CDS ERROR line %d,Find_Prev context2 succeeded!\r\n",__LINE__);
			 	cdsprint(ErrFH,buffer);
				bad++;
			}
		}
	}

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Ascending))) {
		myprintf(ErrFH,  cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Last(context2, WITHOUT_LOCK, false))) {
		if (env->cds_errno == CDS_RECORD_NOT_FOUND)
			strcpy(Sequence_number,"0");
		else
		{
			myprintf(ErrFH,  "Find_Last context2 failed on %s\r\n", cdsErrMsg(env));
			exit(1);
		}
	} else
	{
	 	cdsprint(ErrFH,buffer);
		if(!get_seq_num(buffer))
			myprintf(ErrFH, "Find_Last - did not find sequence number in buffer\r\n");
		else
		{
			myprintf(OutFH,"Find_Last context2 succeeded\r\n");
			myprintf(OutFH,"Find_Last - sequence number->%s\r\n",Sequence_number);
			myprintf(ErrFH,  "Find_Last context2 succeeded\r\n");
			/* now test Find_Prev */
			if (ptrnil(buffer = Cds_Find_Prev(context2, WITHOUT_LOCK, false))) {
				myprintf(ErrFH,  "Find_Prev context2 failed on:\r\n %s\r\n",cdsErrMsg(env));
				bad++;
			}
		}
	}

	/****** test get functions ******/

	if (ptrnil(buffer = Cds_Find_First(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_First at line %d failed on %s\r\n",__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH,  "Find_First succeeded\r\n");
		myprintf(ErrFH,  "%x, %x, %x, %x, %x\r\n", *buffer, *(buffer + 1),
			 *(buffer + 2), *(buffer + 3), *(buffer +4));
		cdsprint(ErrFH,buffer);
	}
	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,WITHOUT_LOCKING, WORK))) {
		myprintf(ErrFH,  "Update_Flds context1 after Find_First failed on:\r\n %s\r\n", cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH,  "Update_Flds succeeded\r\n");
		dmpbulk(ErrFH, Jnlbuf);
		cdsprint(ErrFH, Jnlbuf);
	}
	if (ptrnil(buffer = Cds_Find_First(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_First at line %d failed on %s\r\n", __LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH,  "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	/* prepare to update HostStat on last inserted record */
	HostStat.storage = (byte * ) "2";
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH,  "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*hoststat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH,  "buffer build failed");
		exit(12);
	}
	len = Jnlptr - Jnlbuf; /*set length in front*/
	SETARECLEN(Jnlbuf,len);

	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,WITHOUT_LOCKING,WORK))) {
		myprintf(ErrFH,  "Update_Flds failed on %s\r\n", cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH,  "Update_Flds succeeded\r\n");
		cdsprint(ErrFH, Jnlbuf);
	}
	if (ptrnil(buffer = Cds_Find_First(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_First at line %d failed on %s\r\n",__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH,  "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}


	if (ptrnil(buffer = Cds_Find_Last(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_Last failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Last succeeded\r\n");
		myprintf(ErrFH,  "Find_Last succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (ptrnil(buffer = Cds_Find_Prev(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_Prev failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Prev succeeded\r\n");
		myprintf(ErrFH,  "Find_Prev succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (ptrnil(buffer = Cds_Find_Next(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "line %d, Find_Next context1 failed on %s\r\n", __LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next succeeded\r\n");
		myprintf(ErrFH,  "Find_next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (ptrnil(Cds_Unlock(context1))) {
		myprintf(ErrFH,  "Unlock failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Unlock succeeded\r\n");
	}
	if (ptrnil(buffer = Cds_Find_Next(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_Next at EOF failed on (good) %s\r\n", cdsErrMsg(env));
		++good;
	} else
	 {
	 	++bad;
		myprintf(OutFH,"Find_Next at line %d succeeded and was supposed to fail EOF\r\n", __LINE__);
		myprintf(ErrFH,  "Find_Next succeeded and was supposed to fail EOF\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (ptrnil(buffer = Cds_Find_First(context1,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_First at line %d failed on %s\r\n",__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH,  "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (!(result = Cds_End_Select(context1))) {
		++bad;
		myprintf(ErrFH,  "Line %d End select failed on:\r\n%s",__LINE__,cdsErrMsg(env));
	}
	myprintf(OutFH,"End_Select is OK\r\n\r\n");



	if (ptrnil(buffer = Cds_Find_First(context2,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_First on context 2 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		if(!get_seq_num(buffer)) {
			myprintf(ErrFH, "Find_First - did not find sequence number in buffer\r\n");
			++bad;
		}
		else
		{
			myprintf(OutFH,"Find_First succeeded\r\n");
			myprintf(OutFH,"Find_First - sequence number->%s\r\n",Sequence_number);
			myprintf(ErrFH,  "Find_First succeeded\r\n");
			cdsprint(ErrFH,buffer);
			if (ptrnil(buffer = Cds_Find_Next(context2,WITH_LOCK, false))) {
				myprintf(ErrFH,  "Line %d, Find_Next failed on:\r\n %s\r\n", __LINE__, cdsErrMsg(env));
				++bad;
			} else
			 {
				myprintf(OutFH,"Find_Next succeeded\r\n");
				myprintf(ErrFH,  "Find_Next succeeded\r\n");
				dmpbulk(ErrFH, buffer);
				cdsprint(ErrFH,buffer);
			}
		}
	}

	if (!ptrnil(buffer))
	{
		len = ARECLEN(buffer);
		rbuffer = mem_alloc(len);
		memcpy(rbuffer,buffer,len);
		myprintf(ErrFH, "Buffer for Replace\r\n");
		cdsprint(ErrFH,rbuffer);
		if (ptrnil(result = Cds_Replace(context2, rbuffer,WITH_LOCK,WORK))) {
			myprintf(ErrFH,  "Replace failed on %s\r\n", cdsErrMsg(env));
			++bad;
		} else {
			myprintf(OutFH,"Replace first succeeded\r\n");
			myprintf(ErrFH,  "Replace first succeeded\r\n");
			//	Cds_Unlock(context2);  // test only one time
		}
		Br.storage = (byte *) ""; 	/*set null */
		Teller.storage = (byte *) "";
		rbuff2 = build_buffer();
		myprintf(ErrFH, "Buffer for Replace\r\n");
		cdsprint(ErrFH,rbuff2);
		if (ptrnil(result = Cds_Replace(context2, rbuff2,WITHOUT_LOCKING,WORK))) {
			myprintf(ErrFH,  "NULL Replace failed on: good %s\r\n", cdsErrMsg(env));
			++good;
		} else
		 {
			myprintf(ErrFH,  "ERROR - null replace succeeded\r\n");
			++bad;
		}
		Br.storage = (byte *) "1";
		Teller.storage = (byte *) "1";
	}

	/* now there should be no locked record held.	But Unlock shouldn't
	** complain */
	if (ptrnil(Cds_Unlock(context2))) {
		myprintf(ErrFH,  "Unlock failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Unlock succeeded\r\n");
	}

	if (!(result = Cds_End_Select(context2))) {
		++bad;
		myprintf(ErrFH,  "Line %d End select failed on:\r\n%s\r\n",__LINE__,cdsErrMsg(env));
	}
	myprintf(OutFH,"End_Select is OK\r\n\r\n");

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Ascending))) {
		myprintf(ErrFH,  "\r\nERROR at line %d\r\n%s\r\n",__LINE__,cdsErrMsg(env));
		abort();
	}

	if (found = get_n(context2,18,0))
	{
		good++;
		myprintf(OutFH,"Find_n succeeded\r\n");
	}
	else
		bad++;
	
	if (found = get_n(context2,8,-((found>8 ? 8 : found))))
	{
		good++;
		myprintf(OutFH,"Find_n succeeded\r\n");
	}
	else
	{
		myprintf(ErrFH, "get_n returned only %d records\r\n",found);
		bad++;
	}

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",(char *) criteria6,Ascending))) {
		myprintf(ErrFH,  "at line %d\r\n%s\r\n",__LINE__,cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context2,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_Next on context2 at line %d failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
		exit(12);
	} else {
		myprintf(OutFH,"Find_Next on context2  succeeded\r\n");
		myprintf(ErrFH,  "Find_Next on context2 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (!(result = Cds_End_Select(context2))) {
		++bad;
		myprintf(ErrFH,  "Line %d end_select failed:\r\n%s\r\n",__LINE__, cdsErrMsg(env));
	}
	else
		myprintf(OutFH,"End_Select is OK\r\n\r\n");
	if (!(context5 = 
		Cds_Select(env, NULL,"journal","context5",(char *)criteria5,(char *) 0)))
	{
		++bad;
		myprintf(ErrFH, "Line %d, select 5 failed \r\n %s", __LINE__,cdsErrMsg(env));
	}
	if (!(contextp = 
		Cds_Select(env, NULL,"profiles",(char *) 0,(char *)0,(char *) 0)))
	{
		++bad;
		myprintf(ErrFH, "Line %d, select profiles failed \r\n %s", __LINE__,cdsErrMsg(env));
	}
	else
	{
		Stuff.length = 2;
		buffer = build_buffer();
		if (!Cds_Insert(contextp, buffer,WITH_LOCK,WORK)) {
			myprintf(ErrFH,  "Insert error\r\n %s on profiles\r\n", cdsErrMsg(env));
			myprintf(OutFH, "Insert error line %d\r\n %s on profiles\r\n", __LINE__,cdsErrMsg(env));
			bad++;
			if (good == 0)
				exit(12);
		}
		else
		{
			good++;
			if (!Cds_Delete(contextp, WORK)) {
				myprintf(ErrFH,  "Delete error at line %d\r\n %s on record %d\r\n", __LINE__,cdsErrMsg(env), n);
				bad++;
			}
			else
			{
				good++;
				if (!Cds_Update_Flds(contextp,buffer,WITHOUT_LOCKING,WORK))
					++good;
				else
				{
					myprintf(ErrFH,  "ERROR updated deleted record");
					++bad;
				}
			}
		}

	}
	if (!(context5 = Cds_Select(env, NULL,"journal", "context5",(char *) criteria5,Ascending))) {
		myprintf(ErrFH,  "at line %d\r\n%s",__LINE__,cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context5,WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_Next on context 5 line %d,failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next succeeded\r\n");
		myprintf(ErrFH,  "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (ptrnil(buffer = Cds_Find_Next(context5, WITH_LOCK, false))) {
		myprintf(ErrFH,  "Find_Next context5 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next succeeded\r\n");
		myprintf(ErrFH,  "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (ptrnil(Cds_End_Select(context5)))
	{
		myprintf(OutFH,"end_select context5 failed\r\n%s",cdsErrMsg(env));
		exit(13);
	}
#ifdef	CDSVSTATS
	if (!ptrnil(ccb = gl_getfirst(env->ccb_head))) 
		cmstats_print(ccb->ftab);
#endif
	close_all_clconnects(env);
	if (bad) {
		myprintf(OutFH,"Sorry Charlie, %d errors.\r\n",bad);
		exit(1);
	}
	else
	{
		myprintf(ErrFH,  "Nice Job!\r\n");
		myprintf(OutFH,"Nice Job!\r\n");
		exit(0);
	}
}

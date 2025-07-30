/*****************
*  Test Client for cdserver
*
*/
#include "pch.h"
#include "rtdef.h"
#ifndef APIENTRY	// Windows NT defines APIENTRY, but 3.x doesn't
# define APIENTRY far pascal
#endif
#include <getopt.h>
#include "c.h"
#include "er.h"
#include "cdserror.h"
#include "clconn.h"
#include "cdsfuncs.h"
#include "clcdsfn.h"
#include "defines.h"
#include "dataman.h"
#include "memalloc.h"
#include "clientdf.h"
#include "cdsfuncs.h"
#include "ppdefs.h"
#include "swerror.h"
#include "misc.h"
#include "rtprotos.h"
#include "cmgetenv.h"
#include "mcnuser.h"
#include "tcl.h"
#ifndef SYSTEM_H
#	include "h/system.h"
#endif

int		wm_argc = 3;
char	*wm_argv[24];

char	criteria4[40] = "__f2:7 rmatch \".*F\"";
char	criteria7[60] = "__f2:1 == 1 && f2:2 == 1 && f2:4 >= 2 && f2:4 <= 3";
/*                     012345678901234567890123456 */
char	criteria8[] = {0,11,0,CDS_FIELD,2,0,3,(char)PCLle,FT_INT,1,0};
char	criteria9[20] = "__f2:4 >= 100";

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
	wm_argv[0] = "testcl";
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
static void
build_BT_order(byte *order_spec)
{
	byte *ord=  &order_spec[sizeof(reclen)];

	*ord++ ='A';
	*ord++ = CDS_ORDER_FIELD_LIST;
	*ord++ ='A';
	*ord++ = Branch->application;
	*ord++ = Branch->field_number >> 8;
	*ord++ = Branch->field_number % 256;
	*ord++ ='A';
	*ord++ = Operator->application;
	*ord++ = Operator->field_number >> 8;
	*ord++ = Operator->field_number % 256;
	SETRECLEN(order_spec, (ord- order_spec));
}


/*******************************************
* main() of test program
********************************************/
void my_main(argc, argv)
int	argc;	 /* argument count */
char	*argv[]; /* arg[0] is program name */
/* arg[3] is number of records to insert */
/* arg[4] is beginning search date yyyymmdd for context3 */
/* arg[5] is ending search date yyyynndd for context3 */
{
	static const  char Usage[]="Usage: %s [-V][-kls][-Sserver_name][-nproject][-M maxlength] [limit [start_date [end_date]]]\r\n";
	int		found;
	static clenv *env;
	reclen len, big_length;
	foundset *found_recs;
	static char	Number[10];
	clconnect *ccb = 0;
	const fdes *ftab;
	byte bt_order[32];
	cdsselect	 * context1,
			   *context2,
			   *context3,
			   *context4,
			   *context5,
			   *context5a,
			   *contextp,
			   *context7,
			   *context8,
			   *context9,
			   *contextL;
	char	*criteria2;
	char	*result;
	char	*number = Number;
	short	i, n, limit, good=0, bad=0, count=0;
	char	*buffer, *ptr, cx;
	char	*start_date, *end_date;
	int	 	seq_int;
	char	*project=get_moacurproj();
	char	readbuffer[4], *server_name = 0;
	int		argn=1, opt;
	long	m_time=time(NULL);
	struct	tm 	*now=localtime(&m_time);
	unsigned long	tics, start_tics, beginning_tics, ending_tics;
	unsigned clock_tics;
	double	seconds;
	bool	want_buff_print=false, kill_it=false, lockopt=false, want_dumps=0;
	int log_level=0;
	char *debug_string;
	char *envfile;
	bool	want_stop=false;
	static	char	s_date[9]="19890415";
	static	char	e_date[9]="19890516";

	clock_tics = tics_per_second();
	while ((opt = getopt(argc, argv, "VbklsM:n:S:")) != EOF) {
		switch (opt) {
		  case 'V':
			PrintRelInfo(NULL);
			return;
		  case 'n':
			project=optarg;
			break;
		  case 'b':		/* print bulk dumps*/
			want_dumps = true;
			break;
		  case 'k':		/*kill_it on error */
			kill_it = true;
			break;
		  case 'l':		/*pause to test locking */
			lockopt = true;
			break;
		  case 'M': /* Max_length option */
			Max_length = (ushort) atol (optarg);
			break;
		  case 'S':
			server_name = optarg;
			break;
		  case 's':
			want_stop = true;
			break;
		  default:
			printf(Usage,argv[0]);
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
	else {
    		sprintf(&s_date[0],"%04d",now->tm_year+1900);
 		sprintf(&s_date[4],"%02d",now->tm_mon);
 		sprintf(&s_date[6],"%02d",now->tm_mday-1);
		start_date = s_date;
	}
	if (argc > argn)
		end_date = argv[argn++];
	else {
 		sprintf(&e_date[0],"%04d",now->tm_year+1900);
 		sprintf(&e_date[4],"%02d",now->tm_mon);
 		sprintf(&e_date[6],"%02d",now->tm_mday+1);
		end_date = e_date;
	}

	if (strlen(end_date) !=8) {
		myprintf(OutFH,"end_date must be in yyyymmdd format\r\n");
		exit(2);
	}
	if (strlen(start_date) !=8)	{
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
	start_tics = GetTickCount();		/* start of function  */
	if (!Cds_Initial (env, server_name)) { /* initialize client/server or linked in server */
#ifndef NT
		if (env->cds_errno == CDS_SHADOW_WAITING) {
			printf (cdsErrMsg(env));
			myprintf(OutFH," Do you want the shadow to take over? ");
			if (getchar() == 'y') {
				become_primary(server_name);
				continue;
			}
			else
				exit(13);
		}
#endif
		myprintf(OutFH,"Fatal error %s",cdsErrMsg(env));
		exit(12);
	}
	if (env->cds_errno == CDS_FLD_DD_INCOMPATIBLE)
		puts(cdsErrMsg(env));
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for Cds_Init\r\n", seconds);

	if (!translate_field_names(env))
		exit(2);
	build_BT_order(bt_order);

	build_criteria3(criteria3,start_date,end_date);
	sprintf(criteria4, "__f%d:%d rmatch \".*F\"",
		List[6].application, List[6].field_number);
	len = strlen(criteria4);
	SETARECLEN(criteria4,len);
	build_criteria5(criteria5,start_date,end_date);
	build_criteria6(criteria6);
	sprintf(criteria7, "__f%d:%d == 1 && f%d:%d == 1 && f%d:%d >= 2 && f%d:%d <= 3",
				Branch->application,Branch->field_number,
				Operator->application,Operator->field_number,
				Sequence_Number->application,Sequence_Number->field_number,
				Sequence_Number->application,Sequence_Number->field_number);
	len  = strlen(criteria7);
	SETARECLEN(criteria7,len);

	sprintf(criteria9, "__f%d:%d >= 100",
				Sequence_Number->application,Sequence_Number->field_number);
	len  = strlen(criteria9);
	SETARECLEN(criteria9,len);

	start_tics = GetTickCount();		/* start of function  */
	if (!(found_recs = Cds_SelectnFetch(env, NULL,"journal",(char *) criteria5,bt_order,2))) {
		myprintf(ErrFH, "line %d, %s\r\n",__LINE__,cdsErrMsg(env));
		exit(11);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for Cds_SelectnFetch\r\n", seconds);

	beginning_tics = start_tics = GetTickCount();		/* start of function  */
	if (!(context1 = Cds_Select(env, NULL,"ugly", (char *) 0, (char *) 0,NULL))) {
		myprintf(ErrFH,"good %s\r\n",cdsErrMsg(env));
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for failed Cds_Select\r\n", seconds);

	start_tics = GetTickCount();		/* start of function  */
	if (!(found_recs = Cds_SelectnFetch(env, NULL,"journal",(char *) criteria5,Ascending,2))) {
		myprintf(ErrFH, "line %d, %s\r\n",__LINE__,cdsErrMsg(env));
		exit(11);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for Cds_SelectnFetch\r\n", seconds);
	buffer = found_recs->recs;
	cdsprint(ErrFH,buffer);

	MCNSetULog(log_level,1,0,0);
	start_tics = GetTickCount();		/* start of function  */
	if (!(context5 = Cds_Select(env, NULL,"journal", "context5",(char *) criteria5,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for Cds_Select\r\n", seconds);
	
	buffer = mem_alloc(8);
	len = sizeof(reclen);
	SETARECLEN(buffer, len); /*set bogus length in front*/
	if (Cds_Update_All(context5, buffer,0)==false) {
		myprintf(ErrFH, "At line %d, Update_All context5 failed on GOOD %s\r\n", 
				__LINE__, cdsErrMsg(env));
		good++;
	} else
	{
		myprintf(ErrFH, "ERROR At line %d, Update_All context5 succeeded but shouldn't have\r\n", 
				__LINE__);
		++bad;
	}
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_Next(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next on context 5 line %d,failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.3f seconds for Fin_Next\r\n", seconds);
		myprintf(ErrFH, "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it){
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
	
	start_tics = GetTickCount();		/* start of function  */
	cdsPush(env);
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for Push\r\n", seconds);

	if (!(context5a = Cds_Select(env, NULL,"journal", "context5",(char *) criteria5,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	if (ptrnil(buffer = Cds_Find_Next(context5a, WITH_LOCK, false))) {
		myprintf(ErrFH,"Find_Next on context 5a line %d,failed on:\r\nGOOD %s\r\n",
				__LINE__, cdsErrMsg(env));
	} else	 {
		time_t time_now=time(NULL);
		myprintf(OutFH,"BAD Find_Next succeeded with LOCK at %s\r\n",ctime(&time_now));
		myprintf(ErrFH, "ERROR Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
		++bad;
	}
	if (bad && kill_it){
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	start_tics = GetTickCount();		/* start of function  */
	cdsPop(env);
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for POP\r\n", seconds);
	if (!(context8 = Cds_Select(env, NULL,"journal", "context8",(char *) criteria8,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_Next(context8, WITH_LOCK, false))) {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.3f seconds for Find_Next(context8)\r\n", seconds);
		myprintf(ErrFH, "Find_Next on context 8 line %d,failed on:\r\n good %s\r\n",
				__LINE__, cdsErrMsg(env));
		++good;
	} else {
		myprintf(OutFH,"Find_Next succeeded at %d\r\n",time(NULL));
		myprintf(ErrFH, "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
		++bad;
	}
	
	start_tics = GetTickCount();		/* start of function  */
	if (!(contextL = Cds_Select(env, NULL,"journal", "contextL",(char *) criteria5,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	if (ptrnil(buffer = Cds_Find_Next(contextL, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next on context L line %d,failed on good:\r\n%s\r\n",
				__LINE__, cdsErrMsg(env));
	} else {
		myprintf(OutFH,"ERROR Find_Next succeeded at line %d\r\n", __LINE__);
		myprintf(ErrFH, "ERROR line %d Find_Next succeeded and should not have\r\n",__LINE__);
		cdsprint(ErrFH,buffer);
		++bad;
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for Find_Next(contextL)\r\n", seconds);
	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
	
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_Last(context5, READ_LOCK, false))) {
		myprintf(ErrFH, "Find_Last on context 5 line %d,failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.3f seconds for Find_Last line %d\r\n", seconds, __LINE__);
		myprintf(ErrFH, "Find_Last succeeded\r\n");
	}
	
	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_Last(contextL, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Last on context L line %d,failed on good:\r\n%s\r\n",
				__LINE__, cdsErrMsg(env));
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.3f seconds for Find_Last line %d\r\n", seconds,__LINE__);
		
		if (ptrnil(buffer = Cds_Find_Last(contextL, READ_LOCK, false))) {
			myprintf(ErrFH, "ERROR Find_Last on context L line %d,failed on:\r\n%s\r\n",
				__LINE__, cdsErrMsg(env));
			++bad;
		} else
			;
		
	} else
	 {
		myprintf(OutFH,"ERROR Find_Last succeeded at line %d\r\n", __LINE__);
		myprintf(ErrFH, "ERROR line %d Find_Last succeeded and should not have\r\n",__LINE__);
		cdsprint(ErrFH,buffer);
		++bad;
	}
	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
	Cds_Begin_Work(env);
	Cds_End_Select(contextL);
	if (!Cds_Commit(env)) {
		myprintf(ErrFH, "At line %d, Commit failed on %s\r\n",__LINE__, cdsErrMsg(env));
		++bad;
		goto badout;
	}

	if (lockopt) {
		myprintf(OutFH,"Paused for lock test.  Hit return key to continue.");
		fflush(stdout);
		read(STDIN,readbuffer,1);
	}

	Cds_Begin_Work(env);

	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_First(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_First context5 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.3f seconds for Find_First line %d\r\n", seconds, __LINE__);
		myprintf(ErrFH, "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH, "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*HostStat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH, "buffer build failed");
		exit(12);
	}
	len = (Jnlptr - Jnlbuf);
	SETARECLEN(Jnlbuf, len); /*set length in front*/
	/* test update function */
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(result = Cds_Update_Flds(context5, Jnlbuf,WITH_LOCK,0))) {
		myprintf(ErrFH, "At line %d, Update_Flds context5 failed on %s\r\n", 
				__LINE__, cdsErrMsg(env));
		bad++;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Update_Flds succeeded in %2.3f seconds\r\n",seconds);
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
	}
	if (!Cds_Commit(env)) {
		myprintf(ErrFH, "At line %d, Commit failed on %s\r\n",__LINE__, cdsErrMsg(env));
		++bad;
		goto badout;
	}
	if (!(contextL = Cds_Select(env, NULL,"journal", "contextL",(char *) criteria5,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	if (ptrnil(buffer = Cds_Find_First(contextL, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_First on context L line %d,failed on good:\r\n%s\r\n",
				__LINE__, cdsErrMsg(env));
	} else
	 {
		myprintf(OutFH,"ERROR Find_First succeeded at line %d\r\n", __LINE__);
		myprintf(ErrFH, "ERROR line %d Find_First succeeded and should not have\r\n",__LINE__);
		cdsprint(ErrFH,buffer);
		++bad;
	}
	Cds_Begin_Work(env);

	if (ptrnil(buffer = Cds_Find_First(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First context5 failed on:\r\n %s\r\n",
				__LINE__,  cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH, "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH, "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*HostStat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH, "buffer build failed");
		exit(12);
	}
	len = (Jnlptr - Jnlbuf);
	SETARECLEN(Jnlbuf,len); /*set length in front*/

	/* test update function */
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(result = Cds_Update_Flds(context5, Jnlbuf,WITH_LOCK,0))) {
		myprintf(ErrFH, "At line %d, Update_Flds context5 failed on %s\r\n",
				__LINE__,  cdsErrMsg(env));
		bad++;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Update_Flds succeeded in %2.3f seconds\r\n",seconds);
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
	}
	if (!Cds_Commit(env)) {
		myprintf(ErrFH, "At line %d, Commit failed on %s\r\n",__LINE__, cdsErrMsg(env));
		++bad;
		goto badout;
	}
	if (!(contextL = Cds_Select(env, NULL,"journal", "contextL",(char *) criteria5,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	if (ptrnil(buffer = Cds_Find_First(contextL, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_First on context L line %d,failed on good:\r\n%s\r\n",
				__LINE__, cdsErrMsg(env));
	} else
	 {
		myprintf(OutFH,"ERROR Find_First succeeded at line %d\r\n", __LINE__);
		myprintf(ErrFH, "ERROR line %d Find_First succeeded and should not have\r\n",__LINE__);
		cdsprint(ErrFH,buffer);
		++bad;
	}
	if (ptrnil(Cds_End_Select(contextL)))
	{
		myprintf(OutFH,"line %d,end_select contextL failed\r\n%s",__LINE__,cdsErrMsg(env));
		exit(13);
	}
	

	myprintf(OutFH,"starting end_select context5 line %d\r\n",__LINE__);
	if (ptrnil(Cds_End_Select(context5)))
	{
		myprintf(OutFH,"end_select context5 failed\r\n%s",cdsErrMsg(env));
		exit(13);
	}
	if (!(context1 = Cds_Select(env, NULL,"journal", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	myprintf(ErrFH, "context from select=%x\r\n", (VPTR)context1);

	if (!(context3 = Cds_Select(env, NULL,"journal", "context3", criteria3,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context3, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_Next on context 3 failed on:\r\n %s\r\n",
				__LINE__,  cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next context 3 succeeded\r\n");
		myprintf(ErrFH, "Find_Next context 3 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (!(context7 = Cds_Select(env, NULL,"journal", "context7", criteria7,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	start_tics = GetTickCount();		/* start of function  */
	buffer = Cds_Find_Next(context7, WITH_LOCK, false);
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for find_next context 7\r\n", seconds);
	if (ptrnil(buffer)) {
		myprintf(ErrFH, "Find_Next on context 7 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next context 7 succeeded\r\n");
		myprintf(ErrFH, "Find_Next context 7 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}


	if (ptrnil(buffer = Cds_Find_Next(context3, WITHOUT_LOCK, false))) {
		myprintf(ErrFH, "Find_Next context 3 2nd failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next context 3 2nd succeeded\r\n");
		myprintf(ErrFH, "Find_Next context 3 2nd succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (!(context4 = Cds_Select(env, NULL,"journal", "context4",criteria4,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context4, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next on context 4 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next in context 4 succeeded\r\n");
		myprintf(ErrFH, "Find_Next in context 4 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (ptrnil(buffer = Cds_Find_Next(context4, WITH_LOCK, false))) {
		if (env->cds_errno == CDS_LOCK_FAILURE)
			++good;
		else {
			myprintf(ErrFH, "Find_Next context4 line %d failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
			++bad;
		}
	} else
	 {
		myprintf(OutFH,"Find_Next succeeded\r\n");
		myprintf(ErrFH, "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}


	criteria2 = build_criteria2();	/* using current date */
	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Descending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context2, WITHOUT_LOCKING, false))) {
		if (env->cds_errno == CDS_RECORD_NOT_FOUND)
			strcpy(Sequence_number,"0");
		else
		{
			myprintf(ErrFH, "line %d, Find_Next context2 failed on %s\r\n",__LINE__, cdsErrMsg(env));
			exit(1);
		}
	} else
	{
	 	cdsprint(ErrFH,buffer);
		if(!get_seq_num(buffer))
			myprintf(ErrFH,"Find_Next - did not find sequence number in buffer\r\n");
		else
		{
			myprintf(OutFH,"Find_Next context2 succeeded\r\n");
			myprintf(OutFH,"Find_Next - sequence number->%s\r\n",Sequence_number);
			myprintf(ErrFH, "Find_Next context2 succeeded\r\n");
			myprintf(ErrFH,"Find_Next - sequence number->%s\r\n",Sequence_number);
			/* now test Find_Prev */
			if (ptrnil(buffer = Cds_Find_Prev(context2, WITHOUT_LOCKING, false))) {
				myprintf(ErrFH, "line %d,Find_Prev context2 failed on:\r\ngood %s\r\n",__LINE__,cdsErrMsg(env));
				good++;
			}
		}
	}

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Last(context2, WITHOUT_LOCKING, CDSRESET))) {
		if (env->cds_errno == CDS_RECORD_NOT_FOUND)
			strcpy(Sequence_number,"0");
		else
		{
			myprintf(ErrFH, "Find_Last context2 failed on %s\r\n", cdsErrMsg(env));
			exit(1);
		}
	} else
	{
	 	cdsprint(ErrFH,buffer);
		if(!get_seq_num(buffer))
			myprintf(ErrFH,"Find_Last - did not find sequence number in buffer\r\n");
		else
		{
			myprintf(OutFH,"Find_Last context2 succeeded\r\n");
			myprintf(OutFH,"Find_Last - sequence number->%s\r\n",Sequence_number);
			myprintf(ErrFH, "Find_Last context2 succeeded\r\n");
			/* now test Find_Prev */
			if (ptrnil(buffer = Cds_Find_Prev(context2, WITHOUT_LOCKING, CDSRESET))) {
				myprintf(ErrFH, "Find_Prev context2 failed on:\r\n %s\r\n",cdsErrMsg(env));
				bad++;
			}
		}
	}
	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	/* build and insert as many records as desired for test */

	result = "";
	seq_int = atoi(&Sequence_number[0]) + 1;
	limit += seq_int;
	start_tics = GetTickCount();		/* start of function  */
	for (n =  seq_int; n <= limit && !ptrnil(result); n++) {
		/* put record number into record key */
		sprintf(number, "%d", n);
		Sequence.storage = (byte * ) number;
		Sequence.length = strlen(number);
		buffer = build_buffer();
		if (want_buff_print)
			cdsprint(ErrFH,buffer);
		if (!(result = Cds_Insert(context1, buffer, WITH_LOCK, WORK))) {
			myprintf(ErrFH, "Insert error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
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
			count++;
		}
	}
	if (bad)
		fflush(stdout);
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for %d inserts, %2.3f average\r\n", seconds, count, seconds/count);

/*     TESTBIG */
	myprintf(OutFH,"starting big record test\r\n"); fflush(stdout);
	sprintf(number, "%d", n);
	Sequence.storage = (byte * ) number;
	Sequence.length = strlen(number);
	big_length = Max_length - 80;
	Stuff.storage = (byte * ) mem_alloc(big_length+4);
	Stuff.storage[big_length -1] = 'x';
	for (cx='0', ptr = (char *) Stuff.storage; ptr < (char *)Stuff.storage+ big_length - 1; )
	{
		*ptr++ = cx++;
		if (cx > '9')
			cx = '0';
	}
	Stuff.storage[big_length] = 0;
	Stuff.length = big_length;
	buffer = build_buffer();
	start_tics = GetTickCount();		/* start of function  */
	if (!(result = Cds_Insert(context1, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH, "insert error\r\n %s on big record\r\n", cdsErrMsg(env));
		myprintf(OutFH, "insert error\r\n %s on big record\r\n", cdsErrMsg(env));
		bad++;
			exit(12);
	}
	else
		good++;
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%2.3f seconds for big insert\r\n", seconds);
	
	myprintf(OutFH,"sucessfully inserted %d records\r\n", ++count);

	if (!(result = Cds_Insert(context1+4, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH, "insert error\r\ngood %s\r\n", cdsErrMsg(env));
		good++;
	}
	else
		bad++;
	
	if (!(result = Cds_Insert((cdsselect *) 0, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH, "insert error\r\ngood %s\r\n", cdsErrMsg(env));
		good++;
	}
	else
		bad++;
	

	/* prepare to update HostStat on last inserted record */
	HostStat.storage = (byte * ) "1";
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH, "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*HostStat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH, "buffer build failed");
		exit(12);
	}
	len = (Jnlptr - Jnlbuf);
	SETARECLEN(Jnlbuf,len); /*set length in front*/

	/* test update function */
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,WITHOUT_LOCKING,0))) {
		myprintf(ErrFH, "Update_Flds context1 failed on %s\r\n", cdsErrMsg(env));
		bad++;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Update_Flds succeeded in %2.3f seconds\r\n",seconds);
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, Jnlbuf);
		cdsprint(ErrFH, Jnlbuf);
	}
	if (bad && kill_it)
	{
		myprintf(OutFH,"after update bad=%d\r\n",bad);
		exit(1);
	}

	if (ptrnil(buffer = Cds_Fetch_Again(context2, WITH_LOCK))) {
		myprintf(ErrFH, "Fetch_Again at line %d failed on:\r\n %s\r\n",__LINE__,cdsErrMsg(env));
		bad++;
		if (kill_it)	{
			myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
			exit(1);
		}
	}
	else
	{
		myprintf(OutFH,"Fetch_Again succeeded\r\n");
		myprintf(ErrFH, "Fetch_Again succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}

	if (ptrnil(buffer = Cds_Find_First(context2, WITH_LOCK, 0))) {
		myprintf(ErrFH, "Fetch_First at line %d failed on:\r\n %s\r\n",__LINE__,cdsErrMsg(env));
		bad++;
		sv_who(env, server_name,MEMORY_STATS);
	}
	else
	{
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH, "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	if (!Cds_Replace(context2,buffer, 0,0)) {
		myprintf(ErrFH, "Replace at line %d failed on:\r\n %s\r\n",__LINE__,cdsErrMsg(env));
		bad++;
	}
	else
	{
		char *fetched;
		if (ptrnil(fetched = Cds_Fetch_Again(context2, WITHOUT_LOCK))) {
			myprintf(ErrFH, "Fetch_Again at line %d failed on:\r\n %s\r\n",__LINE__,cdsErrMsg(env));
			bad++;
		}
		else
		{
			if (memcmp(buffer,fetched,RECLEN(buffer))) {
				myprintf(ErrFH, "Fetch_Again at line %d failed on:\r\n %s\r\n",__LINE__,"NOT SAME RECORD");
				bad++;
			}
		}
	}
		
	/****** test get functions ******/

	if (ptrnil(buffer = Cds_Find_First(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First failed on %s\r\n", __LINE__,  cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH, "Find_First succeeded\r\n");
		myprintf(ErrFH, "%x, %x, %x, %x, %x\r\n", *buffer, *(buffer + 1), *(buffer + 2), *(buffer + 3), *(buffer +
			4));
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,false,0))) {
		myprintf(ErrFH, "At line %d, Update_Flds context1 after Find_First failed on:\r\n %s\r\n",
			__LINE__, cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, Jnlbuf);
		cdsprint(ErrFH, Jnlbuf);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
	if (ptrnil(buffer = Cds_Find_First(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First failed on %s\r\n",
				__LINE__,  cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH, "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	/* prepare to update HostStat on last inserted record */
	HostStat.storage = (byte * ) "2";
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH, "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*HostStat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH, "buffer build failed");
		exit(12);
	}
	len = (Jnlptr - Jnlbuf);
	SETARECLEN(Jnlbuf,len); /*set length in front*/

	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,false,0))) {
		myprintf(ErrFH, "At line %d, Update_Flds failed on %s\r\n",
				__LINE__,  cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
		cdsprint(ErrFH, Jnlbuf);
	}
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_First(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First failed on %s\r\n",
				__LINE__,  cdsErrMsg(env));
		++bad;
	} else	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Find_First at line %d succeeded in %2.3f seconds\r\n",
		       __LINE__, seconds);
		myprintf(ErrFH, "Find_First succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}


	if (ptrnil(buffer = Cds_Find_Last(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_Last failed on %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Last succeeded\r\n");
		myprintf(ErrFH, "Find_Last succeeded\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, buffer);
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	if (ptrnil(buffer = Cds_Find_Prev(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_Prev failed on %s\r\n",
				__LINE__,  cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Prev succeeded\r\n");
		myprintf(ErrFH, "Find_Prev succeeded\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, buffer);
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	if (ptrnil(buffer = Cds_Find_Next(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "line %d, Find_Next context1 failed on %s\r\n", __LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next succeeded\r\n");
		myprintf(ErrFH, "Find_next succeeded\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, buffer);
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}
 
	if (ptrnil(Cds_Unlock(context1))) {
		myprintf(ErrFH, "Unlock failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Unlock succeeded\r\n");
	}

	if (ptrnil(buffer = Cds_Find_Next(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next at EOF failed on %s\r\n", cdsErrMsg(env));
		++good;
	} else
	 {
	 	++bad;
		myprintf(OutFH,"Find_Next succeeded and was supposed to fail EOF\r\n");
		myprintf(ErrFH, "Find_Next succeeded and was supposed to fail EOF\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, buffer);
		cdsprint(ErrFH,buffer);
	}

	if (ptrnil(buffer = Cds_Find_First(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First failed on %s\r\n", __LINE__,  cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH, "Find_First succeeded\r\n");
		if (want_dumps)
			dmpbulk(ErrFH, buffer);
		cdsprint(ErrFH,buffer);
	}
	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	if (!(result = Cds_End_Select(context1))) {
		++bad;
		myprintf(ErrFH, "Line %d End select failed on:\r\n %s\r\n",__LINE__,cdsErrMsg(env));
	} else
		myprintf(OutFH,"End_Select is OK\r\n\r\n");

	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	if (ptrnil(buffer = Cds_Find_First(context2, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First on context 2 failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		if(!get_seq_num(buffer)) {
			myprintf(ErrFH,"Find_First - did not find sequence number in buffer\r\n");
			++bad;
		}
		else
		{
			myprintf(OutFH,"Find_First succeeded\r\n");
			myprintf(OutFH,"Find_First - sequence number->%s\r\n",Sequence_number);
			myprintf(ErrFH, "Find_First succeeded\r\n");
			if (want_dumps)
				dmpbulk(ErrFH, buffer);
			cdsprint(ErrFH,buffer);
			if (ptrnil(buffer = Cds_Find_Next(context2, WITH_LOCK, false))) {
				if (env->cds_errno == CDS_LOCK_FAILURE)
					++good;
				else {
					myprintf(ErrFH, "Line %d, Find_Next failed on:\r\n %s\r\n", __LINE__, cdsErrMsg(env));
					++bad;
				}
			} else
			 {
				myprintf(OutFH,"Find_Next succeeded\r\n");
				myprintf(ErrFH, "Find_Next succeeded\r\n");
				if (want_dumps)
					dmpbulk(ErrFH, buffer);
				cdsprint(ErrFH,buffer);
			}
		}
	}

	Stuff.length = 3;
	Stuff.storage[3] = 0;
	if (!ptrnil(buffer))
	{
		char *rbuffer = mem_alloc(ARECLEN(buffer));
		memcpy(rbuffer,buffer,ARECLEN(buffer));
		myprintf(ErrFH,"Buffer for Replace\r\n");
		cdsprint(ErrFH,rbuffer);
		if (ptrnil(result = Cds_Replace(context2, rbuffer,WITH_LOCK,WORK))) {
			myprintf(ErrFH, "Replace failed on %s\r\n", cdsErrMsg(env));
			++bad;
		} else
		 {
			myprintf(OutFH,"Replace first succeeded\r\n");
			myprintf(ErrFH, "Replace first succeeded\r\n");
		}
		mem_free(rbuffer);
		Br.storage = (byte *) ""; /* set null */
		Teller.storage = (byte *) "";
		rbuffer = build_buffer();
		myprintf(ErrFH,"Buffer for Replace\r\n");
		cdsprint(ErrFH,rbuffer);
		if (ptrnil(result = Cds_Replace(context2, rbuffer,WITHOUT_LOCKING,0))) {
			myprintf(ErrFH, "NULL Replace failed on %s\r\n", cdsErrMsg(env));
			++good;
		} else
		 {
			myprintf(ErrFH, "ERROR - null replace succeeded\r\n");
			++bad;
		}
		mem_free(rbuffer);
		Br.storage = (byte *) "1";
		Teller.storage = (byte *) "1";
	}

	/* now there should be no locked record held.	But Unlock shouldn't
	** complain */
	if (ptrnil(Cds_Unlock(context2))) {
		myprintf(ErrFH, "Unlock failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Unlock succeeded\r\n");
	}

	if (!(result = Cds_End_Select(context2))) {
		++bad;
		myprintf(ErrFH, "Line %d End select failed on:\r\n %s\r\n",__LINE__,cdsErrMsg(env));
	}
	myprintf(OutFH,"End_Select is OK\r\n\r\n");

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	if (ptrnil(buffer = Cds_Find_First(context2, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_First on context 2 failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else	
		cdsprint(ErrFH,buffer);
	start_tics = GetTickCount();		/* start of function  */
	if (found = get_n(context2,18,-1)) {
		good++;
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Find_n at line %d succeeded in %2.3f seconds\r\n",
		       __LINE__, seconds);
	}
	else
		bad++;
	
	start_tics = GetTickCount();		/* start of function  */
	if (found = get_n(context2,8,-((found>8 ? 8 : found)))) {
		good++;
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Find_n page back at line %d succeeded in %2.3f seconds\r\n",
		       __LINE__, seconds);
	}
	else
		bad++;

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",(char *) criteria6,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context2, WITH_LOCK, false))) {
		myprintf(ErrFH, "At line %d, Find_Next on context 2 failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next on context2  succeeded\r\n");
		myprintf(ErrFH, "Find_Next on context2 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}


	if (!(result = Cds_End_Select(context2))) {
		++bad;
		myprintf(ErrFH, "Line %d end_select failed:\r\n %s\r\n",__LINE__, cdsErrMsg(env));
	}
	else
		myprintf(OutFH,"End_Select is OK\r\n\r\n");
	if (!(context5 = 
		Cds_Select(env, NULL,"journal","context5",(char *)criteria5,(char *) 0)))
	{
		++bad;
		myprintf(ErrFH,"Line %d, select 5 failed \r\n %s", __LINE__,cdsErrMsg(env));
	}
	if (!(contextp = 
		Cds_Select(env, NULL,"profiles",(char *) 0,(char *)0,(char *) 0)))
	{
		++bad;
		myprintf(ErrFH,"Line %d, select profiles failed \r\n %s", __LINE__,cdsErrMsg(env));
	}
	else
	{
		Stuff.length = 2;
		Stuff.storage[2] = 0;
		buffer = build_buffer();
		if (!Cds_Insert(contextp, buffer, WITHOUT_LOCK, WORK)) {
			myprintf(ErrFH, "Insert error\r\n %s on profiles\r\n", cdsErrMsg(env));
			myprintf(OutFH, "Insert error\r\n %s on profiles\r\n", cdsErrMsg(env));
			bad++;
			if (good == 0)
				exit(12);
		}
		else
		{
			good++;
			if (!Cds_Fetch_Again(contextp, WITH_LOCK)) {
				myprintf(ErrFH, "Fetch_Again error\r\n %s on profiles\r\n", cdsErrMsg(env));
				myprintf(OutFH, "Fetch_Again error\r\n %s on profiles\r\n", cdsErrMsg(env));
				bad++;
				exit(12);
			}
				
			if (!Cds_Delete(contextp, WORK)) {
				myprintf(ErrFH, "Insert error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
				bad++;
			}
			else
			{
				good++;
				if (!Cds_Update_Flds(contextp,buffer,WITHOUT_LOCKING,0))
					++good;
				else
				{
					myprintf(ErrFH, "ERROR updated deleted record");
					++bad;
				}
			}
		}

	}
	myprintf(OutFH,"journal table consumes %d bytes\r\n",Cds_Table_Size(env, "journal"));
	if (!(context9 = Cds_Select(env, NULL,"journal", "context9", criteria9,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	start_tics = GetTickCount();		/* start of function  */
	buffer = Cds_Find_Next(context9, WITH_LOCK, false);
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.3f seconds for find_next context 9\r\n", seconds);
	if (ptrnil(buffer)) {
		myprintf(ErrFH, "Find_Next on context 9 failed on:\r\n %s\r\n", cdsErrMsg(env));
	} else
	 {
		myprintf(OutFH,"Find_Next context 9 succeeded\r\n");
		myprintf(ErrFH, "Find_Next context 9 succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	if (want_stop) {
		if (cdsStop(env, server_name) == false) {
			myprintf(ErrFH,"%s\r\n",cdsErrMsg(env));
			exit(11);
		}
		myprintf(OutFH,"good cdsStop()\r\n");
	}
	if (!ptrnil(ccb = gl_getfirst(env->ccb_head))) 
		ftab = ccb->ftab;
	close_all_clconnects(env);
	ending_tics = GetTickCount();		/* end of functions */
	seconds = (1.0 * ((ending_tics - beginning_tics))/clock_tics);
#ifdef	CDSVSTATS
	cmstats_print(ftab);
#endif
badout:
	if (bad) {
		myprintf(OutFH,"Sorry Charlie, %d errors.\r\n",bad);
		exit(1);
	} else
	{
		myprintf(OutFH,"%3.3f seconds after cdsInit\r\n", seconds);
		myprintf(ErrFH, "Nice Job!\r\n");
		myprintf(OutFH,"Nice Job!\r\n");
		exit(0);
	}
}


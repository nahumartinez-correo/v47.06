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
#include "h/system.h"

#include "cdserror.h"
#include "clconn.h"
#include "cdsfuncs.h"
#include "clientdf.h"
#include "defines.h"
#include "dataman.h"
#include "memalloc.h"
#include "clcdsfn.h"
#include "tcl.h"
#include "er.h"
#include "misc.h"
#include "swerror.h"
#include "cmgetenv.h"
#include "mcnuser.h"

int		wm_argc = 3;
char	*wm_argv[24];


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
	wm_argv[0] = "testnw";
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
/* arg[3] is number of records to insert */
/* arg[4] is beginning search date yyyymmdd for context3 */
/* arg[5] is ending search date yyyynndd for context3 */
{
	const  char Usage[]="Usage: %s [-V][-nproject][-Sserver_name] [limit [start_date [end_date]]]\r\n";
	int		found;
	static clenv *env;
	static char	Number[10];
	int log_level=0;
	char *debug_string;
	char *envfile;
	cdsselect	 * context1,
			   *context2,
			   *context5,
			   *contextp;
	char	*criteria2;
	char	*result;
	char	*number = Number;
	short	i, n, limit, good=0, bad=0, count=0;
	reclen  len;
	char	*buffer, *rbuffer=NULL;
	char	*start_date, *end_date;
	int	 seq_int;
	char	*project, *server_name = 0;
	int		argn=1, opt;
	long	time_now=time(NULL);
	struct	tm 	*now=localtime(&time_now);
	long	tics, start_tics;
	int			clock_tics;
	double  seconds;
	bool	want_buff_print=false, kill_it=false;
	clrb	*rb, *rb2;
	clconnect *ccb = 0;
	const fdes *ftab;
	static	char	s_date[9]="19890415";
	static	char	e_date[9]="19890516";

	clock_tics = tics_per_second();

	project = get_moacurproj();  // get default project
	while ((opt = getopt(argc, argv, "Vkn:S:")) != EOF)
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
			case 'k':		/*kill on error */
				kill_it = true;
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
	if (env->cds_errno == CDS_FLD_DD_INCOMPATIBLE)
		puts(cdsErrMsg(env));

	if (!translate_field_names(env))
		exit(2);
	build_criteria5(criteria5,start_date,end_date);
	build_criteria6(criteria6);

	start_tics = GetTickCount();		/* start of function  */
	if (!(context5 = Cds_Select(env, NULL, "journal", "context5",(char *) criteria5,Ascending))) {
		myprintf(ErrFH,"%s\r\n", cdsErrMsg(env));
		abort();
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for blocked select context5\r\n", seconds);

	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_Next(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next on context 5 line %d,failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.2f seconds for blocked find_next context5\r\n", seconds);
		cdsprint(ErrFH,buffer);
	}
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(buffer = Cds_Find_Next(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next context5 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"%3.2f seconds for blocked find_next context5\r\n", seconds);
		myprintf(ErrFH, "Find_Next succeeded\r\n");
		cdsprint(ErrFH,buffer);
	}
	myprintf(OutFH,"starting end_select context5 line %d\r\n",__LINE__);
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(Cds_End_Select(context5)))
	{
		myprintf(OutFH,"end_select context5 failed\r\n%s",cdsErrMsg(env));
		exit(13);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for blocked end_select context5\r\n", seconds);

	start_tics = GetTickCount();		/* start of function  */
	if (!(rb = cdsnwSelect(env,NULL,"journal", "context5",(char *) criteria5,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for nowait select context5\r\n", seconds);

	start_tics = GetTickCount();		/* start of function  */
	if (!(context5 = clwait(rb))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds to wait for completion of select context5\r\n", seconds);

	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(rb = cdsnwFind_Next(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next on context 5 line %d,failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++bad;
	} else if (ptrnil(cdsnwFind_Next(context5, WITH_LOCK, false))) {
		myprintf(ErrFH, "GOOD ERROR:Find_Next on context 5 line %d,failed on:\r\n %s\r\n",
				__LINE__, cdsErrMsg(env));
		++good;
	} else {
		myprintf(ErrFH, "Second cdsnwFind_Next(context5,... was allowed and shouldn't have been\r\n");
		exit(12);
	}
	//tics = (GetTickCount()) - start_tics;	/* end of function  */
	//seconds = 1.0 * tics/clock_tics;
	//myprintf(OutFH,"%3.2f seconds for nw find_next context5\r\n", seconds);
	SleepEx(2000,1);
	start_tics = GetTickCount();		/* start of function  */
	if (!(buffer = clwait(rb))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds to wait for completion of find_next context5\r\n", seconds);

	myprintf(OutFH,"starting end_select context5 line %d\r\n",__LINE__);
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(rb = cdsnwEnd_Select(context5)))
	{
		myprintf(OutFH,"end_select context5 failed\r\n%s",cdsErrMsg(env));
		exit(13);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for nw end_select start context5\r\n", seconds);
	start_tics = GetTickCount();		/* start of function  */
	if (!(clwait(rb))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds to wait for completion of end_select context5\r\n", seconds);

	
	if (!(context1 = Cds_Select(env, NULL,"journal", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	myprintf(ErrFH, "context from select=%x\r\n", (VPTR) context1);

	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}


	criteria2 = build_criteria2();	/* using current date */

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Last(context2, WITHOUT_LOCKING, false))) {
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
			if (ptrnil(buffer = Cds_Find_Prev(context2, WITHOUT_LOCKING, false))) {
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
		if (!(rb = cdsnwInsert(context1, buffer, WITH_LOCK, n==seq_int ? BEGIN_WORK : WORK)))
			erbug("Insert with nowait at line %d failed to start on\r\n%s\r\n",__LINE__,cdsErrMsg(env));
		mem_free(buffer);
		if (!(result = clwait(rb))) {
			myprintf(ErrFH, "Insert error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			myprintf(OutFH, "Insert error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			bad++;
			if (good == 0)
				exit(12);
		}
		else
		{
			good++;
			count++;
		}
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%3.2f seconds for %d inserts, %2.2f average\r\n", seconds, count, seconds/count);

	Cds_Commit(env);

/*     TESTBIG */
	myprintf(OutFH,"starting big record test\r\n"); fflush(stdout);
	sprintf(number, "%d", n);
	Sequence.storage = (byte * ) number;
	Sequence.length = strlen(number);
	Stuff.storage = (byte * ) mem_alloc(2200);
	memset(Stuff.storage,'x',2199);
	Stuff.storage[2199] = 0;
	Stuff.length = 2199;
	buffer = build_buffer();
	start_tics = GetTickCount();		/* start of function  */
	if (!(rb =cdsnwInsert(context1, buffer, WITH_LOCK, WORK)))
		erbug("Big Insert with nowait failed to start on\r\n%s\r\n",cdsErrMsg(env));
	if (!(result = clwait(rb))) {
		myprintf(ErrFH, "Insert error\r\n %s on big record\r\n", cdsErrMsg(env));
		myprintf(OutFH, "Insert error\r\n %s on big record\r\n", cdsErrMsg(env));
		bad++;
			exit(12);
	}
	else
		good++;
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = 1.0 * tics/clock_tics;
	myprintf(OutFH,"%2.2f seconds for big insert\r\n", seconds);
	
	myprintf(OutFH,"Sucessfully inserted %d records\r\n", ++count);

	/* prepare to update HostStat on last inserted record */
	HostStat.storage = (byte * ) "1";
	Jnlptr = mem_calloc(1024, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH, "no memory for journal buffer");
		exit(10);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = 1024 - sizeof(reclen);

	i = 4;	/*hoststat*/
	if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
		 != RT_SUCCESS)
	{
		myprintf(ErrFH, "buffer build failed");
		exit(12);
	}
	len =  Jnlptr - Jnlbuf; /*set length in front*/
	SETARECLEN(Jnlbuf,len);

	/* test update function */
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(rb = cdsnwUpdate_Flds(context1, Jnlbuf,WITHOUT_LOCKING,0)))
		erbug("Can't start no wait update\r\n%s",cdsErrMsg(env));
	if (ptrnil(result = clwait(rb))) {
		myprintf(ErrFH, "Update_Flds context1 failed on %s\r\n", cdsErrMsg(env));
		bad++;
	} else
	 {
		tics = (GetTickCount()) - start_tics;	/* end of function  */
		seconds = 1.0 * tics/clock_tics;
		myprintf(OutFH,"Update_Flds succeeded in %2.2f seconds\r\n",seconds);
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
	}
	if (bad && kill_it)
	{
		myprintf(OutFH,"after update bad=%d\r\n",bad);
		exit(1);
	}



	if (!(result = Cds_End_Select(context1))) {
		++bad;
		myprintf(ErrFH, "Line %d End select failed on:\r\n%s\r\n",__LINE__,cdsErrMsg(env));
	}
	myprintf(OutFH,"End_Select is OK\r\n\r\n");

	if (bad && kill_it)
	{
		myprintf(OutFH,"after line %d, bad=%d\r\n",__LINE__,bad);
		exit(1);
	}

	if (ptrnil(buffer = Cds_Find_First(context2, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_First on context 2 failed on:\r\n %s\r\n", cdsErrMsg(env));
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
				myprintf(ErrFH, "%x, %x, %x, %x, %x\r\n", *buffer, *(buffer + 1), *(buffer + 2), *(buffer + 3), *(buffer +
					4));
				cdsprint(ErrFH,buffer);
			}
		}
	}

	if (!ptrnil(buffer))
	{
		char *replbuff;
		len = RECLEN(buffer);
		replbuff = mem_alloc(len);
		memcpy(replbuff,buffer,len);
		if (ptrnil(rb = cdsnwReplace(context2, replbuff,WITH_LOCK,WORK)))
			erbug("Can't start no wait Replace\r\n%s",cdsErrMsg(env));
		myprintf(ErrFH,"Buffer for Replace\r\n");
		cdsprint(ErrFH,replbuff);
		mem_free(replbuff);
		if (ptrnil(result = clwait(rb))) {
			myprintf(ErrFH, "Replace failed on %s\r\n", cdsErrMsg(env));
			++bad;
		} else
		 {
			myprintf(OutFH,"Replace first succeeded\r\n");
			myprintf(ErrFH, "Replace first succeeded\r\n");
		}
	}

	if (ptrnil(rb = cdsnwUnlock(context2))) {
		myprintf(ErrFH, "Unlock nw failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} 
	if (!(result = Cds_End_Select(context2))) {
		++good;
		myprintf(ErrFH, "GOOD ERROR at line %d End select failed on:\r\n%s\r\n",__LINE__,cdsErrMsg(env));
	}
	myprintf(OutFH,"End_Select is OK\r\n\r\n");
	if (ptrnil(clwait(rb))) {
		myprintf(ErrFH, "Unlock failed on %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Unlock succeeded\r\n");
		myprintf(ErrFH,"Unlock succeeded\r\n");
	}


	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",criteria2,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
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
		myprintf(OutFH,"Find_n page back succeeded\r\n");
	}
	else
		bad++;

	if (!(context2 = Cds_Select(env, NULL,"journal", "context2",(char *) criteria6,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	if (ptrnil(buffer = Cds_Find_Next(context2, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_Next on context 2 failed on:\r\n %s\r\n", cdsErrMsg(env));
		++bad;
	} else
	 {
		myprintf(OutFH,"Find_Next on context2  succeeded\r\n");
		myprintf(ErrFH, "Find_Next on context2 succeeded\r\n");
		cdsprint(ErrFH,buffer);
		rbuffer = mem_alloc(RECLEN(buffer));
		memcpy(rbuffer,buffer,RECLEN(buffer));
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
		buffer = build_buffer();
		if (!Cds_Insert(contextp, buffer, WITH_LOCK, WORK)) {
			myprintf(ErrFH, "Insert error\r\n %s on profiles\r\n", cdsErrMsg(env));
			myprintf(OutFH, "Insert error\r\n %s on profiles\r\n", cdsErrMsg(env));
			bad++;
			if (good == 0)
				exit(12);
		}
		else
		{
			good++;
			start_tics = GetTickCount();		/* start of function  */
			if (!(rb = cdsnwUpdate_Flds(contextp, buffer, WITH_LOCK, WORK))) {
				erbug("Update error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			}
			if (!(rb2 = cdsnwReplace(context2, rbuffer, WITHOUT_LOCKING,WORK))) {
				erbug("Delete error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			}
			tics = (GetTickCount()) - start_tics;	/* end of function  */
			seconds = 1.0 * tics/clock_tics;
			myprintf(OutFH,"%3.2f seconds for 2 nw updates to start\r\n", seconds);
			start_tics = GetTickCount();		/* start of function  */
			if (!clwait(rb)) {
				myprintf(ErrFH, "Update error at line %d\r\n %s\r\n", __LINE__,
					cdsErrMsg(env));
				bad++;
			}
			if (!clwait(rb2)) {
				myprintf(ErrFH, "Replace error at line %d.\r\n%s\r\n", __LINE__,cdsErrMsg(env));
				bad++;
			}
			tics = (GetTickCount()) - start_tics;	/* end of function  */
			seconds = 1.0 * tics/clock_tics;
			myprintf(OutFH,"%3.2f seconds for 2 nw updates to complete\r\n", seconds);
			if (!(rb = cdsnwDelete(contextp, WORK))) {
				erbug("Delete error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
			}
			if (!clwait(rb)) {
				myprintf(ErrFH, "Delete error\r\n %s on record %d\r\n", cdsErrMsg(env), n);
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
	if (!(result = Cds_End_Select(context2))) {
		++bad;
		myprintf(ErrFH, "Line %d end_select failed:\r\n %s\r\n",__LINE__, cdsErrMsg(env));
	}
	else
		myprintf(OutFH,"End_Select is OK\r\n\r\n");
	if (!ptrnil(ccb = gl_getfirst(env->ccb_head))) 
		ftab = ccb->ftab;	// for cmstats_print
	close_all_clconnects(env);
#ifdef	CDSVSTATS
	cmstats_print(ftab);
#endif
	if (bad) {
		myprintf(OutFH,"Sorry Charlie, %d errors.\r\n",bad);
		exit(1);
	}
	else
	{
		myprintf(ErrFH, "Nice Job!\r\n");
		myprintf(OutFH,"Nice Job!\r\n");
		exit(0);
	}
}

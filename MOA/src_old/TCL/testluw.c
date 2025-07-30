/*****************
*  Test Client for cdserver LUW testing
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
#include "tcl.h"
#include "swerror.h"
#include "cmgetenv.h"
#include "misc.h"
#include "er.h"

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
	static clenv *env;
	clconnect *ccb = 0;
	int log_level=0;
	char *debug_string;
	char *envfile;
	static char	Number[10];
	cdsselect	 * context1,
			   *context2;
	char	*criteria2;
	char	*result;
	char	*number = Number;
	short	i, n, limit, good=0, bad=0, totals_records=0;
	reclen  len;
	char	*buffer, *server_name = 0;
	char	*start_date, *end_date;
	int	 seq_int;
	char	*project=get_moacurproj();
	int		argn=1, opt;
	long	time_now=time(NULL);
	struct	tm 	*now=localtime(&time_now);
	static	char	s_date[9]="19890415";
	static	char	e_date[9]="19890516";
	static fieldv fld;
	char *ptr;
	byte *array_length;

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

	build_criteria3(criteria3,start_date,end_date);

	open_capture(env);
	Cds_Begin_Work(env);
	if (Cds_Begin_Work(env))
	{
		++bad;
		myprintf(ErrFH, "ERROR. 2nd begin work allowed\r\n");
	}
	else
	{
		myprintf(ErrFH,"good %s",cdsErrMsg(env));
		++good;
	}


	if (!(context1 = Cds_Select(env, NULL, "journal", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	myprintf(ErrFH, "context from select=%lx\r\n", (long) context1);

	if (!(Cds_Select(env, NULL,"journal", "context3", criteria3,Ascending))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
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
	if (!Cds_End_Select(context2))
		erbug("End select failed on context2\r\n%s\r\n",cdsErrMsg(env));

	/* build and insert as many records as desired for test */

	result = "";
	seq_int = atoi(&Sequence_number[0]) + 1;
	limit += seq_int;
	for (n=seq_int; n <= limit && !ptrnil(result); n++) {
		/* put record number into record key */
		sprintf(number, "%d", n);
		Sequence.storage = (byte * ) number;
		Sequence.length = strlen(number);
		buffer = build_buffer();
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
		}
	}
/*     TESTBIG */
	//close_capture();
	myprintf(OutFH,"starting big record test\r\n"); fflush(stdout);
	sprintf(number, "%d", n);
	Sequence.storage = (byte * ) number;
	Sequence.length = strlen(number);
	Stuff.storage = (byte * ) mem_alloc(2200);
	memset(Stuff.storage,'x',2199);
	Stuff.storage[2199] = 0;
	Stuff.length = 2199;
	buffer = build_buffer();
	if (!(result = Cds_Insert(context1, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH, "Insert error\r\n %s on big record\r\n", cdsErrMsg(env));
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
	len = Jnlptr - Jnlbuf;
	SETRECLEN(Jnlbuf, len); /*set length in front*/

	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,WITHOUT_LOCKING,0))) {
		myprintf(ErrFH, "Update_Flds context1 failed on %s\r\n", cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
		dmpbulk(ErrFH, Jnlbuf);
	}

	myprintf(ErrFH,"\r\nstarting rollback\r\n");

	if (Cds_Rollback(env))
		++good;
	else
	{
		myprintf(ErrFH, "rollback failed on %s\r\n", cdsErrMsg(env));
		++bad;
	}
	if (Cds_Rollback(env))
	{
		++bad;
		myprintf(ErrFH, "ERROR. rollback succeeded at LINE %d\r\n", __LINE__);
	}
	else
	{
		++good;
		myprintf(ErrFH, "rollback should fail good %s\r\n", cdsErrMsg(env));
	}

	if (!(context1 = Cds_Select(env, NULL, "journal", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}

	seq_int = atoi(&Sequence_number[0]) + 1;
	n = seq_int;
	sprintf(number, "%d", n++);
	Sequence.storage = (byte * ) number;
	Sequence.length = strlen(number);
	buffer = build_buffer();
	ptr=buffer+RECLEN(buffer);
	fld.length = 13;
	fld.value = NULL;
	fld.appl_id = List[7].application;
	fld.field_id = List[7].field_number;
	fld.type = FT_ASCII;
	fld.name="field_a";
	fld.dimension = 1;
	fld.is_member =0;
	fld.sub1=0;
	fld.sub2=1;
	
	if ((ptr = cm_append_array_hdr((byte *) ptr, &fld, &Jnlblen,&array_length))==NULL)
		erbug("can't append array hdr for field_a");

	for (i = 1; i<=2; i++) {
		sprintf(number,"%d",i);
		fld.value=(byte *)&number[0];
		fld.length = strlen(number);
		ptr = cm_append_element((byte *)ptr,&fld, &Jnlblen,1);
	}
	SETRECLEN(array_length,((ptr - (addr)array_length) - 2));
	SETARECLEN(buffer,(ptr-buffer));

	cdsprint(ErrFH,buffer);
	if (!(result = Cds_Insert(context1, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH, "Insert error\r\n %s on array record %d\r\n",cdsErrMsg(env));
		myprintf(OutFH,"Insert error\r\n %s on array record %d\r\n",cdsErrMsg(env));
		exit(9);
	}
	ptr = buffer+sizeof(reclen);
	if ((ptr = cm_append_array_hdr((byte *) ptr, &fld, &Jnlblen,&array_length))==NULL)
		erbug("can't append field_a on line %d",__LINE__);

	for (; i<=4; i++) {
		sprintf(number,"%d",i);
		fld.value=(byte *)&number[0];
		fld.length = strlen(number);
		ptr = cm_append_element((byte *)ptr,&fld, &Jnlblen,1);
	}
	SETRECLEN(array_length,((ptr - (addr)array_length) - 2));
	SETARECLEN(buffer,(ptr-buffer));

	cdsprint(ErrFH,buffer);

	if (Cds_Begin_Work(env))
	{
		++good;
		myprintf(ErrFH, "begin work allowed\r\n");
	}
	else
	{
		myprintf(ErrFH,"begin work at line %d failed\r\n%s",__LINE__,cdsErrMsg(env));
		++bad;
	}

	if (!(result = Cds_Update_Flds(context1, buffer, WITH_LOCK, WORK))) {
		myprintf(ErrFH, "Update error\r\n %s on array record %d\r\n",cdsErrMsg(env));
		myprintf(OutFH,"Update error\r\n %s on array record %d\r\n",cdsErrMsg(env));
		exit(9);
	}

	if (Cds_Rollback(env))
		++good;
	else
	{
		myprintf(ErrFH, "rollback failed on %s\r\n", cdsErrMsg(env));
		++bad;
	}



	if (ptrnil(buffer = Cds_Find_First(context1, WITH_LOCK, false))) {
		myprintf(ErrFH, "Find_First failed on GOOD %s\r\n ", cdsErrMsg(env));
		++good;
	} else
	 {
		myprintf(OutFH,"Find_First succeeded\r\n");
		myprintf(ErrFH, "ERROR Find_First succeeded\r\n");
		++bad;
		dmpbulk(ErrFH, buffer);
		cdsprint(ErrFH,buffer);
	}


	if (!(result = Cds_End_Select(context1))) {
		++good;
		myprintf(ErrFH, "Line %d End select failed on:\r\nGOOD %s\r\n",__LINE__,cdsErrMsg(env));
	}
	else
	{
		++bad;
		myprintf(ErrFH, "ERROR last End_Select succeeded and should not have\r\n");
	}
	if (Cds_Begin_Work(env))
	{
		++good;
		myprintf(ErrFH, "begin work allowed\r\n");
	}
	else
	{
		myprintf(ErrFH,"begin work at line %d failed\r\n%s",__LINE__,cdsErrMsg(env));
		++bad;
	}


	if (!(context1 = Cds_Select(env, NULL,"totals", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, "at line %d, %s", __LINE__, cdsErrMsg(env));
		exit(11);
	}
	myprintf(ErrFH, "context from select=%lx\r\n", (long) context1);

	if (!(Cds_Select(env, NULL,"journal", "context3", criteria3,Ascending))) {
		myprintf(ErrFH, "at line %d, %s", __LINE__, cdsErrMsg(env));
		exit(11);
	}

	if (!(context2 = Cds_Select(env, NULL,"totals", "context2",criteria2,Ascending))) {
		myprintf(ErrFH, "at line %d, %s", __LINE__, cdsErrMsg(env));
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
		}
	}

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
			totals_records++;
			mem_free(buffer);
		}
		break;	/* only doing one record */
	}
	myprintf(OutFH,"Sucessfully inserted %d records into totals\r\n", totals_records);

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
	len = Jnlptr - Jnlbuf;
	SETRECLEN(Jnlbuf, len); /*set length in front*/

	/* test update function */
	if (ptrnil(result = Cds_Update_Flds(context1, Jnlbuf,WITHOUT_LOCKING,0))) {
		myprintf(ErrFH, "Update_Flds context1 failed on %s\r\n", cdsErrMsg(env));
		bad++;
	} else
	 {
		myprintf(OutFH,"Update_Flds succeeded\r\n");
		myprintf(ErrFH, "Update_Flds succeeded\r\n");
		dmpbulk(ErrFH, Jnlbuf);
	}

	myprintf(ErrFH,"\r\nstarting commit\r\n");
	if (Cds_Commit(env))
		++good;
	else
	{
		myprintf(ErrFH, "commit failed on %s\r\n", cdsErrMsg(env));
		++bad;
	}
	if (!(context1 = Cds_Select(env, NULL,"totals", (char *) 0,NULL,NULL))) {
		myprintf(ErrFH, cdsErrMsg(env));
		exit(11);
	}
	myprintf(ErrFH, "context from select=%lx\r\n",(long) context1);
	if (Cds_Find_Last(context1,EXCLUSIVE_LOCK,BEGIN_WORK))
	{
		myprintf(OutFH,"starting delete from totals\r\n");
		if (Cds_Delete(context1,COMMIT_WORK)) /* this small luw is bad practice for performance */
			++good;
		else
		{
			++bad;
			myprintf(ErrFH,"Delete at line %d failed\r\n%s",
					__LINE__,cdsErrMsg(env));
		}
	}
	else
	{
		++bad;
		myprintf(ErrFH,"Find_Last at line %d failed\r\n%s",
				__LINE__, cdsErrMsg(env));
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
		myprintf(ErrFH, "Nice Job!\r\n");
		myprintf(OutFH,"Nice Job!\r\n");
		exit(0);
	}
}

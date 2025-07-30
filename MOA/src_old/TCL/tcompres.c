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
#include "clconn.h"
#include "cdserror.h"
#include "cdsfuncs.h"
#include "dataman.h"
#include "clientdf.h"
#include "tcl.h"
#include "misc.h"
#include "swerror.h"
#include "er.h"
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
	wm_argv[0] = "tcompres";
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
	const  char Usage[]="Usage: %s [-V][-nproject][-Sserver_name] [start_date [end_date]]\r\n";
	static clenv *env;
	clconnect *ccb = 0;
	short	bad=0;
	char	*start_date, *end_date;
	char	*project=get_moacurproj(), *server_name = 0;
	int		argn=1, opt;
	long	m_clock=time(NULL);
	struct	tm 	*now=localtime(&m_clock);
//	struct	tms	t_tics;
	long	tics, start_tics;
	int			clock_tics;
	float	seconds;
	struct recset *return_buf;
	static	char	s_date[9]="19890415";
	static	char	e_date[9]="19890516";

	clock_tics = tics_per_second();

	while ((opt = getopt(argc, argv, "Vn:S:")) != EOF)
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
		
	init_fields();	/* initialize fields*/

	if (argc > argn)
	    argn++;

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
	myprintf(OutFH,"starting %s, ending %s\r\n", start_date, end_date);

	env = clAllocEnv(project, QL);
	if (!env) {
		myprintf(OutFH,"Fatal error can't allocate CDS client environment\r\n");
		exit(13);
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

	myprintf(OutFH,"Compress Started\r\n");
	fflush(stdout);
	start_tics = GetTickCount();		/* start of function  */
	if (ptrnil(return_buf = (struct recset *) Cds_Compress(env, "journal", (char *) criteria5))) {
		myprintf(ErrFH,  "Compress failed on %s\r\n", cdsErrMsg(env));
		bad++;
	} 
	else {
		if (return_buf->count == 0)
			myprintf(OutFH,"Compress succeeded\r\n");
		else {
			myprintf(ErrFH,  "Compress failed to acquire exclusive lock\r\n");
			bad++;
		}
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	seconds = (float)( 1.0 * tics/clock_tics);
	myprintf(OutFH,"%3.2f seconds for Compress\r\n", seconds);
#ifdef	CDSVSTATS
	if (!ptrnil(ccb = gl_getfirst(env->ccb_head))) 
		cmstats_print(ccb->ftab);
#endif
	close_all_clconnects(env);
	if (bad) {
		myprintf(OutFH,"Sorry Charlie, %d errors.\r\n",bad);
		exit(1);
	} else
	{
		myprintf(ErrFH,  "Nice Job!\r\n");
		myprintf(OutFH,"Nice Job!\r\n");
		exit(0);
	}
}

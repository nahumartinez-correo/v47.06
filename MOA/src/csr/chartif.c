// Note: all calls to pcl2xxx() routines assume no OFB arglist...

/*
** MODULENAME - chartif.c
**
**	$Name: R1V4C13 R1V4C12 $
**	$Revision: 1.11 $
**	$Date: 1998/10/16 16:55:44 $
**
**	P C L   I N T E R F A C E   T O   C H A R T
**
**
**	Written by: Dennis Weaver 5-19-89
**	Modified by: Lynette Walsh 3-2-90
**	Misc shrinkages & pipe conversion by: Jim Cathey 11-19-90
**	Misc shrinkages & cleanup by: Mike Telford 3-15-91
**	Conversion to NT - removed pipe, save data to a file
**
**	Synopsis:
**
**    NOTE! (1) the "chart" program must be in "/usr/user/pplus/bin"
**          (2) the "chart.h" file must be in "/usr/lib/pplus" and
**              must be "#included" in the PCL list file and the file
**              that calls the list.
**          (3) A chart must be completed (initialization, data, &
**              plot) before a new chart may be initialized.
**          (4) the number of decimal places will be determined from
**              the first member of the first CALL CSR:chart CHART_DATA.
**
**    To use the chart interface do the following steps
**
**    1.  Create a PCL list to initialize the chart.
**
**    2.  Create a PCL list to create the data set(s).
**
**    3.  Do a CALL CSR:chart CHART_INIT, LST:initilization list name.
**        See RETVAL table for posible errors returned.
**
**    4.  Loop doing CALL CSR:chart CHART_DATA, LST:data set list
**        name, one call for each DATA set. If each data set will be
**        using different fields, then each data set may be included
**        in one list as separate members and the call need be made
**        only once.
**        See RETVAL table for posible errors returned.
**
**    5.  Do a final CALL  CSR:chart CHART_PLOT to kick-off the chart
**        program. The CHART will become a separate process.
**        See RETVAL table for posible errors returned.
**
**    RETVAL table:
**                          INIT   DATA   PLOT
**        WRONGOPERANDS      X      X      X
**        OUTOFORDER         X      X      X
**        WRONGARG           X      X
**        BADMEMBERFORMAT    X      X
**        BADLISTOBJ         X      X
**        BADELEMENTCOUNT    X      X
**        OBJNOTFOUND        X      X
**        ERRPWRITE          X      X
**        ERRPOPEN           X
**        ERREXEC            X
**        OUTOFPROCESSES     X
**        MALLOCFAILED              X
**        MATHOVERFLOW                     X
**
**
**      WRONGOPERANDS   - Incorrect number of operands sent to CSR:chart
**      OUTOFORDER      - CSR:chart called out of order - INIT DATA PLOT
**      WRONGARG        - Wrong argument type(s) passed to CSR:chart
**      BADMEMBERFORMAT - Invalid list member format
**      BADLISTOBJ      - Invalid object in member of list
**      BADELEMENTCOUNT - Incorrect number of elements for list member
**      OBJNOTFOUND     - Object not found in data dictionary
**      ERRPWRITE       - Can't write on pipe
**      ERRPOPEN        - Can't open pipe
**      ERREXEC         - Unable to exec program chart
**      OUTOFPROCESSES  - System out of processes
**      MALLOCFAILED    - Error allocating memory buffer
**      MATHOVERFLOW    - Math overflow resulted in bad data for chart
**
**
**      INITIALIZATION LIST
**
**      LIST application:listname
**      member1:  CHART_VERBOSE level
**      member2:  CHART_INIT_1 x,y,w,h
**      member3:  CHART_INIT_2 f,a,s,C,c,k,
**      member4:  CHART_INIT_3 m,M,T,p,g,l,F
**      member5:  CHART_TITLE  title
**      member6:  CHART_VNAMES name1,name2,name3,....,nameN
**      member7:  CHART_SNAMES name1,name2,name3,....,nameN
**      member8:  CHART_TOPTXT justify, text
**      member9:  CHART_BOTTXT justify, text
**      member10: CHART_LEFTTXT text
**      Where,
**
**         Note:  All member items with the execption of the chart type
**                (CHART_INIT_1 ... CHART_LEFTTXT) may be a descriptor,
**                a field or a string constant.
**
**      member1(optional):
**         CHART_VERBOSE   specifies verbose mode wanted in stand alone
**                         chart program.  This should be the first
**                         member of your LST.
**
**         level    This specifies how much detail will be displayed.  
**                  The higher the number, the more detail.
**
**      member2:
**         CHART_INIT_1 specifies that this is the 1st initialization
**                      member.
**
**         x   specifies the upper left column in pixels to position
**             the display window.  If x = SCR:applname:scrn_name, then
**             the x, y, w, and h coordinates of that screen will be
**             used.
**
**         y   specifies the upper left row in pixel to position the
**             display window.
**
**         w   specifies the width of the display window in pixels.
**
**         h   specifies the length of the display window in pixels.
**
**      member3:
**         CHART_INIT_2 specifies that this is the 2nd initialization
**                      member.
**
**         f   font, the windowman font number.
**
**         a   attributes, windoman "face".
**
**         s   font size in points.
**
**         C   WP6 forground color.
**
**         c   WP6 background color.
**
**      member4:
**         CHART_INIT_3 specifies that this is the 3rd initialization
**                      member.
**
**         m   matrix,  0 = normal, 1 = inverted.
**
**         M   menus, 0 = MENUS_ON, 1 = MENUS_OFF, This specifies whether
**             the pull down menus that allow the chart to be modified
**             will be displayed.
**
**         T   type of chart, 1 = COLUMN, 2 = LINE, 3 = SCATTER, 4 = PIE.
**
**         p   pattern, 0 = NORMALSHADE, any other value will be put in
**             the pattern shift variable and will alter the shading
**             accordingly.
**
**         g   grid, 0 = GRID_ON, 1 = GRID_OFF.
**
**         l   legend, 0 = LEGEND_ON, 1 = LEGEND_OFF.
**
**         F   format, 1=NORMAL, 2=COMMAS, 3=DOLLAR, 4=DOLLARCOMMAS,
**                     5=PERCENT.
**             Defaults to DOLLARCOMMAS if 0 is entered in this postion.
**
**
**      member5(optional):
**         CHART_TITLE   specifies that this is the title.
**
**         title   title string which may be a descriptor, a field or a
**                 string constant.  The title string is centered in the
**                 horizontal bar at the top of the screen and describes
**                 the type of screen this is.
**
**      member6(optional):
**         CHART_VNAMES   specifies that this is the value names member.
**
**         name1...N   These are the names of the values.
**
**      member7(optional):
**         CHART_SNAMES   specifies that this is the set names member.
**
**         name1...N   These are the names of the sets.
**
**      member8(optional):
**         CHART_TOPTXT   specifies that this is a top text member.
**
**         justify  0 = JUST_CENTER, 1 = JUST_LEFT, 2 = JUST_RIGHT
**                  This specifies whether center, left, or right
**                  justification of the text is desired.
**
**         text     This is the text that will be displayed at the top
**                  of the chart.  There may be more than one top
**                  text member if more than one line of text is
**                  desired on the top of the chart.  The lines will
**                  appear in the same order on the chart as the
**                  members appear in the list.
**
**      member9(optional):
**         CHART_BOTTXT   specifies that this is a bottom text member.
**
**         justify  0 = JUST_CENTER, 1 = JUST_LEFT, 2 = JUST_RIGHT
**                  This specifies whether center, left, or right
**                  justification of the text is desired.
**
**         text     This is the text that will be displayed at the
**                  bottom of the chart.  There may be more than
**                  member if more than one line of text is desired on the
**                  one bottom text bottom of the chart.  The lines will
**                  appear in the same order on the chart as the members
**                  appear in the list.
**
**      member10(optional):
**         CHART_LEFTTXT   specifies that this is a left text member.
**
**         text     This is the text that will be displayed at the left
**                  of the chart.  There may be more than one left text
**                  member if more than one line of text is desired on the
**                  left of the chart.  The lines will appear in the same
**                  order on the chart as the members appear in the list.
**
**      DATA SET LIST
**
**      LIST application:listname
**      member1: value1, value2, ... valueN
**
**      Where,
**
**      member1:
**         value1..N  specifies the values to be displayed in the chart.
**                    Each member should contain the number of values
**                    contained in one data set.  If more than one data
**                    set is to be displayed using different fields, then
**                    additional members may be added to this list, one
**                    for each set.  The values may be fields, integer
**                    constants or floating point constants.
**
*/
/*
**   INCLUDES
*/
/*lint -e537 */

#include "pch.h"

#include <typedefs.h>
#include <defines.h>
#include <ofbdefs.h>
#include <pclwords.h>
#include <dsplist.h>
#include <tdspl.h>
#include <rtdef.h>
#include <dataman.h>
#include <csrdef.h>
#include <chart.h>
#include <errdefs.h>
#include <convers.h>
#include <rtprotos.h>
#include <misc.h>
#include <msgrtn.h>

#define	CI_IDLE			0
#define	INIT_COMPLETE	1
#define	DATA_SENT		2

/*
**	GLOBALS
*/
int		chart_retval;
int		chart_retcode;
int		x;
int		y;
int		w;
int		h;
char	fontname[48];
int		a;
int		s;
int		C;
int		c;
int		k;
int		m;
int		M;
int		T;
int		p;
int		g;
int		l;
int		F;
int		v;
int		chart_status = CI_IDLE;
char	tempname[256];
static	int		filectr = 0;			// incremented for each call

byte	fldcnt;
int		fd = -1;					/* Fd for writing to chart input file	*/
byte	ano;
index	ino;

static	char	chartprog[] = {"/usr/users/pplus/bin/chart"};
static	char	*unable2 = "write error, errno=%d\n";
static	char	*wrongarg = "Wrong argument type(s) passed to CSR:chart";
static	char	*outofprocesses = "System out of processes.";
static	char	*mathoverflow = "Math overflow resulted in bad data for chart";
static	char	*outoforder = "CSR:chart called out of order - INIT DATA PLOT";
static	char	*objnotfound = "Object not found in data dictionary";
unsigned char	wrtbuf[1000];
unsigned char	*bp;
unsigned short numitems;
unsigned short numdecimals;
unsigned short decimals_done;
unsigned short chart_open = 0;

void chart_init(byte *, unsigned short);
byte *chart_init_1(byte *);
byte *chart_init_2(byte *);
byte *chart_init_3(byte *);
byte *chart_names(int, byte *);
byte *chart_text(int, byte *);
byte *chart_verbose(int, byte *);
void chart_data(byte *, unsigned short);
static void chart_plot(void);
static void dudley(void);
void *getobj(byte,byte,index);
static byte *get_ano_ino(register byte *lp);
void chart_error_a(int,int,int);
void chart_error_b(char *,int,int);
void chart_defaults(void);
/*eject*/
/*
**
**	C H A R T   I N T E R F A C E   C S R
**
*/
XDLL int
chart(int argc, rtobj *argv[])
{
	complist	*chartlist;
	byte	*lp;
	int		fc;

	if  (argc > 0) {
		if  (rtobj2int(argv[0],&fc) != RT_ERROR) {
			if  ((fc == CHART_INIT || fc == CHART_DATA) && argc == 2) {
				if  (argv[1]->obj.object == CSRARG_LST) {
					ano = argv[1]->obj.applno;
					ino = argv[1]->obj.itemno;
					if  (chartlist = (complist *) getobj(LST,ano,ino)) {
						lp = (byte *) chartlist;
						lp += sizeof(complist);
						numitems = chartlist->numitems;
						if  (numitems > 0) {
							if  (fc == CHART_INIT)
								chart_init(lp,numitems);
							else
								chart_data(lp,numitems);
							}
						else
							/* Invalid list memeber format */
							chart_error_a(BADMEMBERFORMAT,BADMEMBERFORMAT,BADMEMBERFORMAT);
						}
					}
				else
					/* Wrong argument type(s) passed to CSR:chart */
					chart_error_b(wrongarg,CSR_FAIL_ARGTYPE,CSR_FAIL_ARGTYPE);
				}
			else
				if  (fc == CHART_PLOT)
					chart_plot();
				else
					/* Wrong argument type(s) passed to CSR:chart */
					chart_error_b(wrongarg,CSR_FAIL_ARGTYPE,CSR_FAIL_ARGTYPE);
			}
		else
			/* Wrong argument type(s) passed to CSR:chart */
			chart_error_b(wrongarg,CSR_FAIL_ARGTYPE,CSR_FAIL_ARGTYPE);
		}
	else
		/* Incorrect number of operands */
		chart_error_a(WRONGOPERANDS,WRONGOPERANDS,WRONGOPERANDS);

	Retval = chart_retval;
	return(chart_retcode);
}


/*
**	I N I T I A L I Z E   C H A R T
*/
static void chart_init(byte *lp, unsigned short numitems)
{
	int		op;

	/* reset globals	*/
	chart_defaults();

	chart_retval  = RT_SUCCESS;
	chart_retcode = RT_SUCCESS;

	if  (chart_open) {
		/* CSR:chart called out of order - INIT DATA PLOT */
		chart_error_b(outoforder,-1,-1);
		return;
		}
	sprintf(tempname, "%schart%d.dat", tempdir(),++filectr);
	if (filectr == 999)
		filectr = 0;
	fd = open(tempname, O_CREAT | O_WRONLY | O_TRUNC, S_IREAD | S_IWRITE);

	while (numitems-- > 0) {
		lp++;				/* move past MEMBER byte	*/
		fldcnt = *lp++;		/* number of fields in this line */
		lp++;				/* move past member length */
		lp++;
		pcl2int(lp,&op,NULL);	/* first fld in each list is operation */
		fldcnt--;			/* reduce field count by one for operation */
		lp += tsize(lp);
		switch(op) {
			case CHART_INIT_1:
				lp = chart_init_1(lp);
				break;
			case CHART_INIT_2:
				lp = chart_init_2(lp);
				break;
			case CHART_INIT_3:
				lp = chart_init_3(lp);
				break;
			case CHART_VNAMES:
			case CHART_SNAMES:
				if (op == CHART_SNAMES && fldcnt > MAXSETS) {
					chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,
					              BADELEMENTCOUNT);
					return;
					}
				lp = chart_names(op,lp);
				break;
			case CHART_TITLE:
			case CHART_TOPTXT:
			case CHART_BOTTXT:
			case CHART_LEFTTXT:
				lp = chart_text(op,lp);
				break;
			case CHART_VERBOSE:
				lp = chart_verbose(op,lp);
				break;
			default:
				/* Invalid object in MEMBER of list */
				chart_error_a(BADLISTOBJ,BADLISTOBJ,BADLISTOBJ);
				break;
			}
		if  (chart_retcode != RT_SUCCESS) /* one of the init routines has errored out */
			break;
		}
	numdecimals = 0;
	decimals_done = 0;
	if  (chart_retcode == RT_SUCCESS) /* initialization completed ok */
		chart_status = INIT_COMPLETE;
}


static byte * chart_init_1(lp)
byte	*lp;
{
	scrdef	*scrp;

	if  (fldcnt == 1) {
		if  (*lp == SCR) {
			/* correct number of arguments for screen object */
			/* get rectangle coordinates from screen */
			lp++;
			lp = get_ano_ino(lp);
			if  (scrp = (scrdef *) getobj(SCR,ano,ino)) {
				x = scrp->x;
				y = scrp->y;
				w = scrp->width;
				h = scrp->length;
				}
			}
		else {
			/* Invalid object in MEMBER of list */
			chart_error_a(BADLISTOBJ,BADLISTOBJ,BADLISTOBJ);
			}
		}
	else {
		if  (fldcnt == INIT1MAX) {
			/* correct number of arguments for x, y, width, length */
			/* get rectangle coordinates from LST member */
			/* no error checking done so they get whats in the LST */
			pcl2int(lp,&x,NULL);
			lp += tsize(lp);
			pcl2int(lp,&y,NULL);
			lp += tsize(lp);
			pcl2int(lp,&w,NULL);
			lp += tsize(lp);
			pcl2int(lp,&h,NULL);
			lp += tsize(lp);
			}
		else {
			/* Incorrect number of elements for list member */
			chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,BADELEMENTCOUNT);
			}
		}

	/* this is the spot for changing x, y, width, length */
	/* to match rt's relative positoning of screens      */
	return(lp);
}


static byte * chart_init_2(lp)
byte	*lp;
{
	char	*cp;
	if  (fldcnt != INIT2MAX) {
		/* Incorrect number of elements for list member */
		chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,BADELEMENTCOUNT);
		}
	else {
		pcl2str(lp, fontname, sizeof(fontname),NULL);
		/* translate spaces to tildes so that names with spaces	*/
		/* can be passed as command line args to chart			*/
		cp = fontname;
		while (*cp) {
			if (*cp == ' ')
				*cp = '~';
			cp++;
			}
		lp += tsize(lp);
		pcl2int(lp,&a,NULL);
		lp += tsize(lp);
		pcl2int(lp,&s,NULL);
		lp += tsize(lp);
		/* these options not supported by NT!!!	*/
		pcl2int(lp,&C,NULL);
		lp += tsize(lp);
		pcl2int(lp,&c,NULL);
		lp += tsize(lp);
		pcl2int(lp,&k,NULL);
		lp += tsize(lp);
		}
	return(lp);
}


static byte * chart_init_3(lp)
byte	*lp;
{
	if  (fldcnt != INIT3MAX) {
		/* Incorrect number of elements for list member */
		chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,BADELEMENTCOUNT);
		return(0);
		}

	pcl2int(lp,&m,NULL);
	lp += tsize(lp);
	pcl2int(lp,&M,NULL);
	lp += tsize(lp);
	M ^= 1;
	pcl2int(lp,&T,NULL);
	lp += tsize(lp);
	pcl2int(lp,&p,NULL);
	lp += tsize(lp);
	pcl2int(lp,&g,NULL);
	lp += tsize(lp);
	g ^= 1;
	pcl2int(lp,&l,NULL);
	lp += tsize(lp);
	l ^= 1;
	pcl2int(lp,&F,NULL);
	lp += tsize(lp);

	return(lp);
}


/*
**	G E T   N A M E S
*/
static byte * chart_names(op,lp)
int		op;
byte	*lp;
{
	char	namebuf[200];

	bp = wrtbuf;
	if  (op == CHART_SNAMES)
		bp += sprintf((char *)bp, "set names:");
	else
		bp += sprintf((char *)bp, "value names:");

	/* Get Each Name */
	while (fldcnt-- > 0) {
		pcl2str(lp,namebuf,sizeof(namebuf),NULL);
		lp += tsize(lp);

		/* Format the data */
		if  (StringLen(namebuf) > 0)
			bp += sprintf((char *)bp, "%s,", namebuf);
		}
	bp[-1] = '\n'; /* Overwrite last comma with newline. */

	/* Write a line Value Names to the file */
	dudley();	/* Do-write!  Hyuck, hyuck.  */
	return(lp);
}


/*
**	G E T   T I T L E,   T O P,   B O T T O M   O R   L E F T
**  O F   C H A R T   T E X T
*/
static byte * chart_text(op,lp)
int		op;
byte	*lp;
{
	int		justify;
	char	namebuf[200];

	if  ((op == CHART_TOPTXT || op == CHART_BOTTXT) && fldcnt != 2) {
		/* Incorrect number of elements for list member */
		chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,BADELEMENTCOUNT);
		return(0);
		}
	else
		if  ((op == CHART_LEFTTXT || op == CHART_TITLE) && fldcnt != 1) {
			/* Incorrect number of elements for list member */
			chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,BADELEMENTCOUNT);
			return(0);
			}

	bp = wrtbuf;
	if  (op == CHART_TOPTXT) {
		bp += sprintf((char *)bp, "top text:");
		pcl2int(lp,&justify,NULL);
		lp += tsize(lp);
		}
	else
		if  (op == CHART_BOTTXT) {
			bp += sprintf((char *)bp, "bottom text:");
			pcl2int(lp,&justify,NULL);
			lp += tsize(lp);
			}
		else
			if  (op == CHART_LEFTTXT)
				bp += sprintf((char *)bp, "left text:");
			else
				if  (op == CHART_TITLE)
					bp += sprintf((char *)bp, "window title:");

	pcl2str(lp,namebuf,sizeof(namebuf),NULL);
	lp += tsize(lp);

	/*	Format the data */
	if  (op == CHART_TOPTXT || op == CHART_BOTTXT)
		bp += sprintf((char *)bp, "j%d:%s\n", justify, namebuf);
	else
		bp += sprintf((char *)bp, "%s\n", namebuf);

	/* Write a line to the file */
	dudley();	/* Do-write!  Hyuck, hyuck.  */
	return(lp);
}


/*
**	S E T U P   'v'   S W I T C H   F O R   D E B U G I N G
*/
static byte * chart_verbose(op,lp)
int		op;
byte	*lp;
{
	if  (fldcnt != 1) {
		/* Incorrect number of elements for list member */
		chart_error_a(BADELEMENTCOUNT,BADELEMENTCOUNT,BADELEMENTCOUNT);
		return(0);
		}
	pcl2int(lp,&v,NULL);
	lp += tsize(lp);
	return(lp);
}


/*
**	G E T   D A T A   F R O M   P C L
*/
static void chart_data(byte *lp, unsigned short numitems)
{
	char	decimalstr[20];
	char	tmpdata[30];
	char	*tmpptr;
	int		decimal_found;

	if  (chart_status < INIT_COMPLETE) {
		/* CSR:chart called out of order - INIT DATA PLOT */
		chart_error_b(outoforder,-1,-1);
		return;
		}

	decimal_found = 0;
	while (numitems-- > 0) { /* numitems = # of members in list */
		lp++;				/* move past MEMBER byte	*/
		fldcnt = *lp++;		/* number of fields in this line */
		lp++;				/* move past member length */
		lp++;
		bp = wrtbuf;
		while (fldcnt-- > 0) {
			memset(tmpdata,0,sizeof(tmpdata));
			pcl2str(lp,tmpdata,sizeof(tmpdata),NULL);
			if  (decimals_done == 0) {
				decimals_done = 1;
				tmpptr = tmpdata;
				while(*tmpptr) {
					if  (*tmpptr == '.') {
						decimal_found = 1;
						tmpptr++;
						}
					if  (decimal_found)
						numdecimals += 1;
					tmpptr++;
					}
				sprintf(decimalstr, "decimals: %d\n", numdecimals);
				write(fd, decimalstr, StringLen(decimalstr));
				}
			lp += tsize(lp);
			/* add string to the write buffer */
			if  (StringLen(tmpdata) < 1) {
				tmpdata[0] = '\0';
				tmpdata[1] = '\0';
				}
			bp += sprintf((char *)bp, "%s ", tmpdata);
			}
		bp[-1] = '\n';	/* Overwrite last space with a newline. */

		/* Write a line (one set) of data to the file. */
		dudley();	/* Do-write!  Hyuck, hyuck.  */
		}
	chart_status = DATA_SENT;
}


/*
**	C H A R T   T H E   D A T A
*/
static void chart_plot(void)
{
	char	cmdline[256];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	if  (chart_status != DATA_SENT) {
		/* CSR:chart called out of order - INIT DATA PLOT */
		chart_error_b(outoforder,-1,-1);
		return;
		}

	/* all done writing to chart file	*/
	close(fd);

	/* invoke the program	*/
	sprintf(cmdline,	"chart -D"
						" -v%d -x%d -y%d -w%d -h%d -m%d -M%d -T%d -p%d"
						" -f%s -s%d -a%d"
						" -g%d -l%d -F%d %s",
		v,				/* verbose level				*/
		x, y, w, h,		/* screen location and width	*/
		m,				/* data matrix					*/
		M,				/* menus						*/
		T,				/* chart type					*/
		p,				/* pattern number				*/
		fontname,		/* font name					*/
		s,				/* font size					*/
		a,				/* font attribute				*/
		g,				/* grid							*/
		l,				/* legend						*/
		F,				/* format (comma, dollar, etc.)	*/
		tempname);
	memset(&si,0,sizeof(si) );
	if (!CreateProcess(0,cmdline,0,0,0,0,0,0,&si,&pi)) {	// start process
		mre(-1,"Unable to load chart.exe");
	}
	else {
		// Close process and thread handles
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	chart_defaults();
	chart_open = 0;
	decimals_done = 0;
}


/* Do the write to the chart data file. */
static void dudley(void)
{
	if (write(fd, wrtbuf, bp-wrtbuf) < 0) {
		/* */
		printf(unable2, errno);
		close(fd);
		fd = -1;
		chart_error_a(ERRPWRITE,ERRPWRITE,ERRPWRITE);
		}
}


void *getobj(byte obj, byte ano, index	ino)
{
	byte	itemstat;

	itemstat = ddgetstat(RTddh,obj,ano,ino);
	if  ((itemstat & DDFWDREFD) || (itemstat & DDDELETED)) {
		/* Object not found in data dictionary */
		chart_error_b(objnotfound,-1,-1);
		return(0);
		}
	else
		return(ddget(RTddh,obj,ano,ino));
}


static byte * get_ano_ino(byte *lp)
{
	ano = *lp++;
	ino = *lp++ << 8;
	ino |= *lp++;
	return lp;
}


static void chart_error_a(chart_rterrorbox, chart_Retval, chart_return)
int		chart_rterrorbox;
int		chart_Retval;
int		chart_return;
{
	rterrorbox(swerrormsg(chart_rterrorbox));
	chart_retval  = chart_Retval;
	chart_retcode = chart_return;
	chart_status = CI_IDLE;
	if  (chart_open) {
		close(fd);
		fd = -1;
		chart_open = 0;
		}
}


static void chart_error_b(chart_rterrorbox, chart_Retval, chart_return)
char	*chart_rterrorbox;
int		chart_Retval;
int		chart_return;
{
	rterrorbox(chart_rterrorbox);
	chart_retval  = chart_Retval;
	chart_retcode = chart_return;
	chart_status = CI_IDLE;
	if  (chart_open) {
		close(fd);
		fd = -1;
		chart_open = 0;
		}
}

void
chart_defaults(void)
{
	/* return chart globals to default values	*/
	x	=	40;
	y	=	40;
	w	=	200;
	h	=	200;
	strcpy(fontname, "Arial");
	a	=	0;
	s	=	12;
	C	=	0;
	c	=	0;
	k	=	0;
	m	=	0;
	M	=	MENUS_ON;
	T	=	COLUMN;
	p	=	0;
	g	=	1;
	l	=	1;
	F	=	NORMAL;
	v	=	0;
	chart_status = CI_IDLE;
}


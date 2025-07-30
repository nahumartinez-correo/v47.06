/*$
**
** SAPL -- Picklist-related routines built for System Admin special needs
**
** $Name: R1V4C13 $
** $Revision: 1.24 $
** $Date: 1999/03/05 17:25:16 $
**
**	Synopsis:
**		Assorted CSR functions and supporting code for making on-the-fly
**		type picklists.  Used by many SysAdm PCL programs.
**
**	External Function List:
**		int sacmdpl
**		char *sacmdparse
**		int sanewpl
**		int saaddpl
**		int sadopl
**		int sadroppl
**		int sacombopl		Added on 12/23/96
**
**	Notes:
**      None.
**
**  (C) Copyright 1989 by ISC Systems Corp.  All rights reserved.
**
*/
#include "pch.h"

#include <msgrtn.h>
#include <typedefs.h>
#include <convers.h>
#include <defines.h>
#include <rtdef.h>
#include <csrdef.h>
#include <rtprotos.h>
#include <memalloc.h>
#include <tdspl.h>
#include <picklist.h>
#include <misc.h>

#define	MAXPICKS			350			/* max items for a "sacmdpl" list */
#define	DEFAULT_BOX_WIDTH	80			/* about 10 columns in font 0 */
#define	DEFAULT_BOX_LENGTH	22			/* about 1 row in font 0 */
#define	DEFAULT_FONT		4			/* the "Woodland" font */
#define	DEFAULT_FACE		0
#define	DEFAULT_SIZE		12
#define	DEFAULT_X			40			/* default horizontal position */
#define	DEFAULT_Y			40			/* default vertical position */

#define	SCREEN_WIDTH	640		/* pixel-width of screen */
#define	SCREEN_HEIGHT	360		/* pixel-length of video */

#define	MAX_FIELDS	16					/* max number of fields in cmd output */
#define	MAX_LISTS	32					/* number of active dynamic picklists */
#define	LISTSIZE_INCR	256				/* amount to grow a list */

#define WKBUFLEN	256			/* length of various working buffers */
#define	LINE_LEN	WKBUFLEN			/* max len of cmd output line */

#define EOS '\0'

char *sacmdparse(char *cmd, char *flds, char *dlm, int colwidth);
int c_sanewpl(char *name, int x, int y, int w_width, int w_height, int font, int face, int size, 
  int forecolor, int backcolor, int boxwidth, int boxheight, int boxtype, 
  rtobj *title, index itemdepth, short flags, rtobj *key_filter);

int c_saaddpl(char *name, char *val);
int c_sadopl(char *name, char *target, int targlen);
int c_sadroppl(char *name);
static int findname(char *name, int *empty);
static	void	decide_output_fields (char *arg, byte output_fields[]);
byte	*_get_mem(register short i, register byte *bp);
int c_saitempl(struct plwork *pl);
int c_sagetpl(struct plwork *pl, int item, char *onval, char *outfield);
int c_samodpl(struct plwork *pl, char *onval, char *offval, int pos, int item);
int c_sadopv(char *name, field *target, int update);
int SetLeftTop(unsigned long tag, picheader *pic);
int c_sacombopl(char *name, char *target, int targlen, int iu);
int sacombopl(int argc, rtobj *argv[]);
int sacombopv(int argc, rtobj *argv[]);
int c_sacombopv(char *name, field *target, int iu, int update);
static void *redrawcontrolfunc;

/*%
**
**  STATIC SPACE USED BY VARIOUS ROUTINES IN THIS MODULE
**
*/

static	short	Curpicks;		/* current count */
static	int fontwidth = 8;		/* width of current font in pixels */
static	int fontlength = 18;	/* length (height) of current font in pixels */

static char *listnames[MAX_LISTS];	/* Symbolic names for dynamic picklists. */
									/* Nil pointer indicates that this index */
									/* is available; use same subscripts for */
									/* the following arrays. */
static int listw_width[MAX_LISTS],
		   listw_height[MAX_LISTS];
static int listsizes[MAX_LISTS];
static int listbufsizes[MAX_LISTS];
static struct plwork *listpl[MAX_LISTS];

static picbody salist = {	0,			/* number of items in list */
							0			/* size of list in bytes */
							};

static picheader sahdr = 
{
	DEFAULT_X,					/* upper left */
	DEFAULT_Y,
	DEFAULT_BOX_WIDTH,			/* width */
	DEFAULT_BOX_LENGTH,			/* length */
	DEFAULT_FONT,				/* font */
	DEFAULT_FACE,
	DEFAULT_SIZE,
	0,							/* forecolor */
	0,							/* backcolor */
	1,							/* box type */
	1,							/* itemdepth */
	0,							/* flags */
	{							/* list...*/
		&salist,				/* pointer */
		{						/* item */
			0,					/* object type */
			0,					/* application number */
			0,					/* item number */
		}
	},							/* rest of structure init */
	{							/* list...*/
		0,						/* pointer */
		{						/* item */
			0,					/* object type */
			0,					/* application number */
			0					/* item number */
		}
	}
};

/*%
**
**  SACMDPL - "do" a system admin picklist based on output of a shell command
**
**	Usage:
**		CALLC sacmdpl plhdr,colwidth,command,fldlist,delim
**
**	Parameters:
**		parm1	-	picklist header defining x,y,etc.
**		parm2	-	column width
**					controls "columnization" of fields returned from 
**					the system command.  Relevant only when parsing is
**					requested.  0 indicates width-1 columns; other values
**					cause fields to be aligned at the next even multiple
**					of the column width.
**		parm3	-	Pointer to a shell command line w/arguments (for popen)
**      parm4   -   Target field (for results output)
**		parm5	-	(otional) list of fields from cmd to be in picklist
**					format: "1,3,6-9,14"  note: 16 is highest recognized field
**		parm6	-	(optional) Delimiter string (strtok) used to parse 
**					command output into fields.  The first character of this
**					string is used as a delimiter in the picklist display.
**
**	Returns:
**		ret		-	A zero or positive result means that choice was
**                  made.  i.e. a 0 means the 1st picklist selection
**                  was chosen.  A negative result is the negated
**                  key selected, i.e. -K_ESC for <RESET>.
**
**	Synopsis:
**      Sets up a variable-box-size picklist, calls popen() and reads
**      the output of the command as picklist items.  Lets the user
**      select one of these lines, returns its index.  Copies the text
**		of the item into the "common:csr_string" field, to the limit
**		of the size of that field.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**      none
*/

XDLL int 
sacmdpl(int argc, rtobj *argv[])
{
	char *sp;
	int colwidth;
	char cmd[WKBUFLEN], flds[WKBUFLEN], dlm[WKBUFLEN], c_width[5], c_height[5];
	int count, retval;
	int width, length, i;
	int max_string_length = 0, w_width = 0, w_height = 0;
	struct plwork sapl;
	picheader		*plist;
	field	*target;
	TEXTMETRIC tm;

	if (argc < 4)
		return CSR_FAIL_ARGCOUNT;
	if (argv[0]->obj.object != PIC || argv[0]->ptr == NULL)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[1],&colwidth) != RT_SUCCESS ||
	    rtobj2str(argv[2],cmd,sizeof(cmd)) != RT_SUCCESS)
		return CSR_FAIL_ARGTYPE;
	target = (field *)argv[3]->ptr;
	if (argv[3]->obj.object != FLD || target->flags.type != FT_ASCII)
		return(CSR_FAIL_FIELDTYPE);

	plist = (picheader *)argv[0]->ptr;
	plist->list.ptr = &salist;
	flds[0] = 0;
	if (argc > 4)
	{
		rtobj2str(argv[4],flds,sizeof(flds));
		if (flds[0] == 'w')
		{
			w_width = atoi(&flds[1]);
			flds[0] = 0;
			if (argc > 5)
			{
				rtobj2str(argv[5],c_height,sizeof(c_height));
				if (c_height[0] == 'h')
				{
					w_height = atoi(&c_height[1]);
				}
			}
		}
	}

	dlm[0] = 0;
	if (argc > 5)
	{
		rtobj2str(argv[5],dlm,sizeof(dlm));
		if (dlm[0] == 'w')
		{
			w_width = atoi(&dlm[1]);
			dlm[0] = 0;
			if (argc > 6)
			{
				rtobj2str(argv[6],c_height,sizeof(c_height));
				if (c_height[0] == 'h')
				{
					w_height = atoi(&c_height[1]);
				}
			}
		}
	}
	if (argc > 7)
	{
		if (rtobj2str(argv[6],c_width,sizeof(c_width)) != RT_SUCCESS ||
		    rtobj2str(argv[7],c_height,sizeof(c_height)) != RT_SUCCESS)
			return CSR_FAIL_ARGTYPE;

			if (c_width[0] == 'w')
			{
				w_width = atoi(&c_width[1]);
			}
			if (c_height[0] == 'h')
			{
				w_height = atoi(&c_height[1]);
			}
	}
	TextFont(plist->font);
	TextFace(plist->face);
	TextSize(plist->size);
	getfont(&tm);
	fontwidth = tm.tmMaxCharWidth;
	fontlength = tm.tmHeight;
	/* add 12.5% of chararacter height for inter-line spacing */
	fontlength += (fontlength + 7) >> 3; /* adjust for interline spacing */
#ifdef DEBUGCODEWANTED
if (Debug)
	fprintf(stdout, "sacmdpl: col width %d  flds = <%s>, dlm = <%s>\n", colwidth, flds, dlm);
#endif

#ifdef DEBUGCODEWANTED
if (Debug)
fprintf(stdout, "sacmdpl: command was '%s', fldlist was '%s', dlm was '%s'\n",
   cmd, (flds == 0)?"<NULL>":flds, (dlm == 0)?"<NULL>":dlm);
#endif

	count = 0;

/*
**  Build the list of pick items from data provided by sacmdparse()
*/
	sapl.pickitem = (char **)alloca(sizeof(char *) * (MAXPICKS + 1));
	while (0 != (sp = sacmdparse(cmd, flds, dlm, colwidth))) /* get next item */
	{
		if (*sp) {	/* is it a non-empty string? */
			sapl.pickitem[count] = sp;
			++count;							/* count items in list */

			if (max_string_length < (i = StringLen(sp) + 2)) /* 2 for cosmetics */
				max_string_length = i;

			if (count > MAXPICKS) {
				char emsg[50];
				sprintf(emsg,"sacmdpl(): Picklist has too many items, max allowed is %d",
					MAXPICKS);
				rterrorbox(emsg);
				return -1;
			}
		}
	}

	/* make sure we've got things to pick from */
	if (count < 1) {
		rterrorbox("sacmdpl(): No items to pick from");
		return -1;
	}

	/* make box wide enough for biggest item */
	width = (fontwidth * max_string_length);
	if (width + 3 > plist->width)
	{
		plist->width = width + 3;
	}
	if (w_width != 0)
	{
		if (plist->width > w_width)
		{
			plist->width = w_width;
		}
	}
	if (plist->width > (SCREEN_WIDTH - plist->x))
	{
		plist->width = SCREEN_WIDTH - plist->x;
	}

	/* make box long enough for list... */
	length = (fontlength * count) + 4;	/* about 4 pixels for border */
	if (length + 2 > plist->length)
	{
		plist->length = length + 2;
	}
	if (w_height != 0)
	{
		if (plist->length > w_height)
		{
			plist->length = w_height;
		}
	}
	if (plist->length > SCREEN_HEIGHT - plist->y)
	{
		plist->length = SCREEN_HEIGHT - plist->y;
	}
	
	sapl.pickitem[count] = 0;			/* make last pointer NULL */
	salist.numitems = count;

	sapl.picklist = plist;
	sapl.plscreen = PCLfields;
	sapl.foldflag = FALSE;
	retval = show_picklist (&sapl, TRUE);

	if (retval >= 0)	/* if valid, return string value in common:csr_string */
	{
		str2field(sapl.pickitem[retval], target);
	}
	
	for (i=0; i < count; ++i)
		mem_free(sapl.pickitem[i]);		/* clean up memory malloc'd by sacmdparse */
	
	sahdr.width = DEFAULT_BOX_WIDTH;	/* Restore standard box size for */
	sahdr.length = DEFAULT_BOX_LENGTH;	/* other routines in this module */
	sahdr.x = DEFAULT_X;
	sahdr.y = DEFAULT_Y;
	sahdr.font = (byte)theFont;
	sahdr.face = (byte)theFace;
	sahdr.size = (byte)theSize;
	sahdr.forecolor = (byte)theForeColor;
	sahdr.backcolor = (byte)theBackColor;

	return retval;
}

/*%
**
**  SACMDPARSE - handle system command activation and parsing 
**
**	Usage:
**		val = sacmdparse(cmd, flds, dlm);
**
**	Parameters:
**		parm1	-	Pointer to a shell command line w/arguments (for popen)
**		parm2	-	(otional) list of fields from cmd to be included in 
**					the returned string, in the format: "1,3,6-8,11,14-"  
**					note: 16 is highest recognized field.
**		parm3	-	(optional) Delimiter string (strtok) used to parse 
**					command output into fields
**		parm4	-	column width.  See description in sacmdpl().
**
**	Returns:
**		ret		-	A null pointer means no more lines can be read from
**                  the specified system command.  Subsequent calls will
**                  activate a new popen().  Any valid pointer is a 
**                  formatted string as returned by the system command,
**					with the specified fields.
**
**	Synopsis:
**      Keeps track of the command string passed to it.  The first time
**      it sees a string, it "remembers" it, does a popen() of that command,
**		parses the field-list argument (if present), and returns the first
**		line of output from the command (suitably formatted).  On subsequent
**		invocations with the same command, returns subsequent output lines.
**		When a command completes, this routine does a pclose(), "forgets"
**      the remembered command, returns a nil pointer, and is ready to 
**		be invoked again with a new (or same) command string.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**      Returns strings in malloc()'d space.  Caller is responsible for
**		free()ing these strings.
**
**		Notice that the field list and delimiter arguments are inspected
**		ONLY when a "new" command string is encountered.
**
**		Max size of one output line from 'cmd' is LINE_LEN characters; longer 
**		lines are broken into LINE_LEN-character chunks (see: fgets).
**
**		When parsing is taking place (flds and dlm are non-null), a maximum
**		of 16 fields per line are processed (additional fields are dropped).
**		The returned line is delimited by the FIRST of the delimiter
**		characters specified in the 'dlm' argument, regardless of input.
**		When parsing is not specified, the entire line is returned, regardless
**		of the number of fields present.
*/
char *
sacmdparse(char *cmd, char *flds, char *dlm, int colwidth)
{
	static char *last1 = "";
	static FILE *infile;
	static byte output_fields[MAX_FIELDS];
	char in_buff[LINE_LEN];
	char *ret_buff, *tok;
	int i;

	if (0 != strcmp(last1, cmd))
	{
#ifdef DEBUGCODEWANTED
if (Debug)
fprintf(stdout, "sacmdparse: cmd '%s', fldlst '%s', delimiter '%s'\n",
   cmd, (flds == 0)?"":flds, (dlm == 0)?"":dlm);
#endif
		if (*last1 != '\0')			/* Previous cmd didn't terminate properly */
		{
			mem_free(last1);
			_pclose(infile);
		}
		last1 = (char *)mem_alloc(StringLen(cmd) + 1);
		if (0 == last1)
		{
			rterrorbox("sacmdparse(): out of memory");
			return (char *)-1;
		}
		strcpy(last1,cmd);
		infile = _popen(cmd,"r");
		if (0 == infile)
		{
			char emsg[50];
			sprintf(emsg, "popen() failed in sa picklist - cmd = '%s', errno %d",
				cmd, errno);
			rterrorbox(emsg);
			return (char *)-1;
		}
		if (0 != flds && '\0' != *flds && 0 != dlm && '\0' != *dlm)		
			/* if we got field spec and delimiter */
		{
#ifdef DEBUGCODEWANTED
if (Debug)
	printf("calling decide_output_fields with flds = <%s>\n", flds);
#endif
			decide_output_fields(flds, output_fields);
		}
#ifdef DEBUGCODEWANTED
if (Debug)
fprintf(stdout,"popen succeeded\n");
#endif
	}

	if (0 == fgets(in_buff, LINE_LEN, infile))	/* Get a line from cmd stdout */
	{
		_pclose(infile);
		mem_free(last1);
		last1 = "";
		return (char *)0;
	}
#ifdef DEBUGCODEWANTED
if (Debug)
fprintf(stdout,"fgets succeeded\n");
#endif

 	if (in_buff[0] == '\n' || in_buff[0] == '\0')
		return "";
	in_buff[StringLen(in_buff) - 1] = '\0';	/* Replace CR */

	ret_buff = (char *)mem_alloc(LINE_LEN);
	if (0 == ret_buff)
	{
		rterrorbox("sacmdparse(): Out of memory 2");
		return (char *)-1;
	}

	if (0 != flds && '\0' != *flds && 0 != dlm && '\0' != *dlm)		
		/* if we need to parse...*/
	{
		for (i = LINE_LEN - 1; i >= 0; --i)
			ret_buff[i] = ' ';					/* wipe buffer clean */

		if (0 >= colwidth)
			colwidth = 1;
		if (0 == (tok = strtok(in_buff, dlm)))	/* first token */
		{
			*ret_buff = '\0';
		}
		else
		{
			int align = 0;
			char * bufp = ret_buff;				/* point at return buffer */

			for (i = 0; i < MAX_FIELDS; ++i)
			{
				if (output_fields[i] == 'y')	/* return this field? */
				{
					bufp += align;				/* offset to next column */
					while (*tok != '\0')	/* copy string, point at end */
					{
						*bufp++ = *tok++;
					}
					align = colwidth - ((bufp - ret_buff) % colwidth);
				}
				if (0 == (tok = strtok((char *)0, dlm)))
					break;
			}
			bufp += align;
			*bufp = '\0';			/* wipe out trailing delimiter */
		}
	}
	else
		/* no parsing required */
	{
		strcpy(ret_buff, in_buff);
	}

	return ret_buff;
}


/***********************************************************************
*		*** decide_output_fields ***		local
*
* Creator:	Carl Paukstis	Thu Sep 29  1988
*
* Purpose: Set up the output_fields array, given a command-line
*          specification for desired fields and desired order.
*
* Modifications: none
*
* Imports:	par1: field spec
*			par2: array of yes/no flags
*
* Exports:	Sets output_fields[] (par2)
*
* Comments:	none
*
***********************************************************************/
static	void	
decide_output_fields(char *arg, byte output_fields[])
{
	int i;
	int low_field, high_field;

#ifdef DEBUGCODEWANTED
if (Debug)
	printf("decide_output_fields: flds = <%s>\n", arg);
#endif
	for (i = 0; i < MAX_FIELDS; ++i)
		output_fields[i] = 'n';

	while ('\0' != *arg)
	{
		low_field = atoi(arg);
#ifdef DEBUGCODEWANTED
if (Debug)
	printf("after atoi, low_field = %d\n", low_field);
#endif
		while (isdigit(*arg))
			++arg; 				/* look for comma, hyphen or null terminator */
		if (0 >= low_field)
			low_field = 1;		/* allow incorrect zero-based numbering */
		if (MAX_FIELDS <= low_field)
			continue;
		--low_field;
#ifdef DEBUGCODEWANTED
if (Debug)
{
	printf ("putting 'y' in out_put[%d]f for field %d\n", low_field,
	  low_field + 1);
	printf ("remaining flds = <%s>\n", arg);
}
#endif
		output_fields[low_field] = 'y';
/*
   We got a field number, check for a range specification
*/
		if ('-' == *arg)
			{
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("found a hyphen\n");
#endif
			arg++;
			if ('n' == *arg || ',' == *arg || '\0' == *arg)
				{
				high_field = MAX_FIELDS;
				++arg;
				}
			else
				{
				while (!isdigit(*arg) 
					&& '\0' != *arg 
					&& ',' != *arg)
						++arg;
				if ('\0' == *arg)
					break;
				if (',' == *arg)
					high_field = MAX_FIELDS;
				else
					{
					high_field = atoi(arg);
					while (isdigit(*arg))
						++arg;
					if (0 >= high_field || low_field > high_field)
						high_field = low_field;
					}
				}
			for (i = low_field; i < high_field; ++i)
				{
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("putting 'y' in out_put[%d]f for field %d\n", i, i + 1);
#endif
				output_fields[i] = 'y';
				}
			}
		while (!isdigit(*arg) && '\0' != *arg)
			++arg;
#ifdef DEBUGCODEWANTED
if (Debug)
	printf (" at end of loop remaining flds = <%s>\n", arg);
#endif
		}
#ifdef DEBUGCODEWANTED
if (Debug)
{
	int i;
	fprintf(stdout, "decide_output: output_fields list: ");
	for (i=0; i < MAX_FIELDS; ++i)
		fprintf(stdout, "%d:%c, ", i, output_fields[i]);
	fprintf(stdout, "\n");
}
#endif
}




/*%
**
**  findname - internal routine to find a list name in the table
**
**	Usage:
**		list = findname(char *name, int *empty);
**
**	Parameters:
**		parm1 - name of list to search for
**		parm2 - pointer to int to store index of an empty slot
**
**	Returns:
**		index of name found.  Index of empty slot returned in area
**		pointed to by second parm.  Negative values are failures.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
*/
static int 
findname(char *name, int *empty)
{
	int i;

	*empty = -1;
#ifdef DEBUGCODEWANTED
if (Debug)
fprintf(stdout, "findname: searching for %s\n", name);
#endif
	for (i=0; i < MAX_LISTS; ++i)
	{
#ifdef DEBUGCODEWANTED
if (Debug)
fprintf(stdout, "finaname: listnames[%d] = %X (%s)\n", i, listnames[i], 
   (listnames[i] == 0) ? "<NULL>" : listnames[i]);
#endif
		if (0 == listnames[i])	/* nil pointer is empty slot */
			*empty = i;
		else if (0 == strcmp(listnames[i], name))
			return i;
	}
	return -1;
}


/*%
**
**  SANEWPL - initialize for a picklist to be built line-at-a-time
**
**	Usage:
**		CALLC sanewpl "name",plhdr
**
**	Parameters:
**		parm1 - symbolic name of new picklist
**		parm2 - picklist header specifying size, position, etc.
**		parm3 - max width
**		parm4 - max length
**		parm5 - file name which contains picklist items
**
**	Returns:
**		0 if all is well
**		-1 if name is null.
**
**	Synopsis:
**      Sets up some static stuff to initialize for building a pciklist
**		"on the fly" from within a PCL program.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**		sensible values are supplied for any omitted parameters except name
*/

XDLL int 
sanewpl(int argc, rtobj *argv[])
{
	char name[WKBUFLEN];
	char file[128];
	int result, width, height;
	picheader *plist;
	FILE	*fc_hdl = ((FILE *)0);
	char	line[300],*fgets();

	file[0] = '\0';
	if (argc < 2)
	{
		rterrorbox("sanewpl(): requires at least 2 arguments");
		return CSR_FAIL_ARGCOUNT;
	}
	if ((result=rtobj2str(argv[0],name,sizeof(name))) != RT_SUCCESS ||
		name[0] == '\0' ||
		argv[1]->obj.object != PIC || 
		argv[1]->ptr == NULL)
		return(CSR_FAIL_ARGTYPE);

	if (argc >= 4)
	{
		if (rtobj2int(argv[2], &width) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
		if (rtobj2int(argv[3], &height) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
		if (argc == 5)
		{
			if ((result=rtobj2str(argv[4],file,sizeof(file))) != RT_SUCCESS ||
				file[0] == '\0')
				return(CSR_FAIL_ARGTYPE);
		}
	}
	else
	{
		width = height = 0;
	}


	
   	plist = (picheader *)argv[1]->ptr;
	c_sanewpl(name, plist->x, plist->y, width, height,
					 plist->font, plist->face, plist->size, 
					 plist->forecolor, plist->backcolor, 
					 plist->width, plist->length,
					 plist->boxtype, &plist->title,
					 plist->itemdepth, plist->flags, &plist->key_filter);
	if (argc != 5)
	{
		return (0);
	}
	fc_hdl = fopen(file,"r");
	if (fc_hdl == (FILE *) 0 )
	{
		return(-99);
	}
	while (fgets (line,255,fc_hdl) != (char *)0)
	{
		line[strlen(&line[0])-1] = '\0';
		if (line[0] == '\0')
		{
			line[0] = ' ';
			line[1] = '\0';
		}
		c_saaddpl(name, line);
	}
	fclose(fc_hdl);
	return (0);
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_sanewpl(char *name, int x, int y, int w_width, int w_height, int font, 
		int face, int size, int forecolor, int backcolor, int boxwidth, 
		int boxheight, int boxtype, rtobj *title, index itemdepth, 
		short flags, rtobj *key_filter)
{
	struct plwork *ourpl;
	int index;
	int emptyslot;
	TEXTMETRIC tm;

#ifdef DEBUGCODEWANTED
if (Debug)
	printf("sanewpl: name = %s\n", name);
#endif

 	c_sadroppl(name);	/* seek and destroy current 'name' picklist */
/*
**  We don't have a list by this symbolic name, so make some memory and
**  init some non-varying places.
*/
	if (0 > (index = findname(name, &emptyslot)))	/* it's really new! */
	{
		if (0 > emptyslot)					/* no empty slots */
		{
			rterrorbox("sanewpl(): too many dynamic picklists");
			return -1;
		}
		index = emptyslot;	/* index used later outside this IF */

	/*  memory for: plwork; picheader; list struct + buffer; symbolic name */

		if (0 == (ourpl = (struct plwork *)mem_alloc(sizeof(struct plwork))) ||
			0 == (ourpl->picklist = (picheader *)mem_alloc(sizeof(picheader))) ||
			0 == (ourpl->picklist->list.ptr = (picbody *)mem_alloc(sizeof(picbody) + LISTSIZE_INCR)) ||
			0 == (listnames[index] = (char *)mem_alloc(StringLen(name) + 1)))
		{
			rterrorbox("sanewpl(): no memory for another picklist");
			return -1;
		}
		listpl[index] = ourpl;
		strcpy(listnames[index], name);
		listbufsizes[index] = LISTSIZE_INCR;	/* initial size of list buf */
		ourpl->picklist->itemdepth = itemdepth;
		ourpl->picklist->boxtype = boxtype;
		ourpl->picklist->flags = flags;
		ourpl->picklist->list.obj.object = 0;
		ourpl->picklist->list.obj.applno = 0;
		ourpl->picklist->list.obj.itemno = 0;
		if (title)
		{
			ourpl->picklist->title = *title; /* structure assignment */
		}
		else
		{
			ourpl->picklist->title.ptr = NULL;
			ourpl->picklist->title.obj.object = 0;
			ourpl->picklist->title.obj.applno = 0;
			ourpl->picklist->title.obj.itemno = 0;
		}
		if (key_filter)
		{
			ourpl->picklist->key_filter = *key_filter; /* structure assign */
		}
		else
		{
			ourpl->picklist->key_filter.ptr = NULL;
			ourpl->picklist->key_filter.obj.object = 0;
			ourpl->picklist->key_filter.obj.applno = 0;
			ourpl->picklist->key_filter.obj.itemno = 0;
		}
	}
	else
	{
#ifdef DEBUGCODEWANTED
if (Debug)
	printf("sanewpl: name already exists, index = %d\n", index);
#endif
  		rterrorbox("sanewpl(): ERROR, unexpected symbolic name");
  		return VAL_FAIL_NO_MSG;
	}

	listsizes[index] = 0;

	if (0 == boxwidth)
		ourpl->picklist->width = DEFAULT_BOX_WIDTH;	
	else
		ourpl->picklist->width = boxwidth;

	if (0 == boxheight)
		ourpl->picklist->length = DEFAULT_BOX_LENGTH;
	else
		ourpl->picklist->length = boxheight;
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("box height = %d\n", ourpl->picklist->length);
#endif

	((picbody *)ourpl->picklist->list.ptr)->numitems = 0;
	((picbody *)ourpl->picklist->list.ptr)->size = 0;

	ourpl->picklist->font = font;
	ourpl->picklist->face = face;
	ourpl->picklist->size = size;
	ourpl->picklist->forecolor = forecolor;
	ourpl->picklist->backcolor = backcolor;
	TextFont((short)font);
	TextFace((short)face);
	TextSize((short)size);
	getfont(&tm);
	fontwidth = tm.tmMaxCharWidth;
	fontlength = tm.tmHeight;
	fontlength += ((fontlength + 7) >> 3); /* adjust for interline spacing */
#ifdef DEBUGCODEWANTED
if (Debug)
	fprintf(stdout, "sanewpl: font %d (max = %d), height %d, width %d\n",
		font, Maxfontnum, fontwidth, fontlength);
#endif

	if (0 == x)
		ourpl->picklist->x = DEFAULT_X;
	else
		ourpl->picklist->x = x;

	if (0 == y)
		ourpl->picklist->y = DEFAULT_Y;
	else
		ourpl->picklist->y = y;


	listw_width[index] = w_width;
	listw_height[index] = w_height;
	return 0;
}




/*%
**
**  SAADDPL - Add an item to the on-the-fly picklist being built
**
**	Usage:
**		CALLC saaddpl name value
**
**	Parameters:
**		parm1 - 	symbolic name of the picklist
**		parm2 - 	ASCII value or list
**		parm3 - 	optional leader
**
**	Returns:
**		none
**
**	Synopsis:
**      Appends the field contents to the list being built for the
**		named picklist, growing the list buffer if necessary.
**		Used in building a picklist "on the fly" from within a PCL program.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
**		Uses malloc() to make copies of the field data, placing the
**		buffer addresses in the pickitem array.  Somebody (normally
**		the sadopl() routine) must free() these spaces.
*/

XDLL int 
saaddpl(int argc, rtobj *argv[])
{
	char name[WKBUFLEN], val[WKBUFLEN], leader[WKBUFLEN];
	complist	*cl;
	byte		*mem_ptr,
				mem_cnt;
	short		i;
	
	if (argc < 2) {
		rterrorbox("saaddpl(): requires atleast 2 arguments");
		return CSR_FAIL_ARGCOUNT;
	}
	
	rtobj2str(argv[0],name,sizeof(name));
	if ('\0' == name[0]) {
		rterrorbox("saaddpl(): requires symbolic name as first argument");
		return CSR_FAIL_ARGTYPE;
	}
	
	if (argv[1]->obj.object != CSRARG_LST) {
		rtobj2str(argv[1],val,sizeof(val));
		if (argc == 3) {
			rtobj2str(argv[2],leader,sizeof(leader));
			strcat (&leader[0],&val[0]);
			c_saaddpl(name, leader);
		}
		else {
			c_saaddpl(name, val);
		}
	}
	else {
		cl = (complist *)ddget(RTddh, argv[1]->obj.object,
				argv[1]->obj.applno, argv[1]->obj.itemno);
		for (i = 0; i < cl->numitems; i++) { 
			mem_ptr = _get_mem(i,(byte *)(cl + 1));
			mem_cnt = mem_ptr[1];
			mem_ptr += 4;
	
			pcl2str (mem_ptr,&val[0],WKBUFLEN-1,NULL);
			if (val[0] != '\0') {
				if (argc == 3) {
					rtobj2str(argv[2],leader,sizeof(leader));
					strcat (&leader[0],&val[0]);
					c_saaddpl(name, &leader[0]);
				}
				else {
					c_saaddpl(name, &val[0]);
				}
			}
		}
	}
	return (VAL_PASS);
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_saaddpl(char *name, char *val)
{
	int index, i;
	picbody *ourlist;
	picheader *ourhdr;
	char *ptr;
	int prevsize;				/* number of chars in list before adding this */
	int width, length;
	int msglen;
	SIZE	xy;
	int	textlen;
	HDC		hdc;
	int		ret;


	if (0 == val || '\0' == *val)
	{
		rterrorbox("saaddpl(): second argument must evaluate to a string");
		return CSR_FAIL_ARGTYPE;
	}

	if (0 > (index = findname(name, &i)))
	{
		return -1;
	}
	ourhdr = listpl[index]->picklist;
	ourlist = (ourhdr->list).ptr;

#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "saaddpl: listname '%s', index = %d, ourlist = %d\n", name, index, ourlist);
#endif
	msglen = StringLen(val) + 1;
	prevsize = listsizes[index];
	listsizes[index] += msglen;

#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "saaddpl: oldlen = %d, addlen = %d, newlen = %d\n", 
    prevsize, msglen, listsizes[index]);
#endif
	if (listsizes[index] > listbufsizes[index])	/* no room in buffer */
	{
#ifdef DEBUGCODEWANTED
		int oldsize = listbufsizes[index];
#endif
		listbufsizes[index] += (LISTSIZE_INCR > msglen) ? LISTSIZE_INCR : msglen;
#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "saaddpl: realloc'ing: size was %d, new size is %d\n",
    oldsize, listbufsizes[index]);
#endif
		if (0 == (ourlist = (picbody *)mem_realloc(ourlist, sizeof(picbody) + listbufsizes[index])))
		{
			rterrorbox("saaddpl():: no memory to copy field contents");
			return -1;
		}
		(ourhdr->list).ptr = ourlist;
	}

	ptr = (char *)ourlist+sizeof(picbody)+prevsize;
	memcpy(ptr, val, msglen);
	*(ptr + msglen - 1) = '\0';
	++ourlist->numitems;							/* count items in list */

#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "saaddpl: did memcpy, new numitems = %d\n", ourlist->numitems);
#endif
	/*
	** caculate the length of the text plus one of the widest characters
	*/
	hdc = GetDC( hWnd );
	ret = GetTextExtentPoint32(hdc, val, msglen, &xy);
	textlen = xy.cx;
	ReleaseDC(hWnd, hdc);

	/* First check the width limits */
	width = textlen;
	/* set the bounding box 3 wider than the list width */
	if (width + 3 > (int)ourhdr->width) 
	{
		ourhdr->width = width + 3;	
	}
	/* check the bounding box width against any width value passed in */
	if (listw_width[index] != 0)
	{
		if ((int)ourhdr->width > listw_width[index]) 
		{
			/* don't exceed the limit */
			ourhdr->width = listw_width[index];
		}
	}
	/* protect from exceeding screen space */
	if (ourhdr->width > SCREEN_WIDTH - ourhdr->x)
	{
		ourhdr->width = SCREEN_WIDTH - ourhdr->x;
	}

	/* Now check the length limits */
	length = (fontlength * ourlist->numitems) + 4;
	if (ourhdr->title.ptr)
	{
		length += fontlength + 8;
	}
	/* set the bounding box 2 longer than the list length */
	if (length + 2 > (int)ourhdr->length)
	{
		ourhdr->length = length + 2;
	}
	/* check the bounding box length against any length value passed in */
	if (listw_height[index] != 0)
	{
		if ((int)ourhdr->length > listw_height[index])
		{
			/* don't exceed the limit */
			ourhdr->length = listw_height[index];
		}
	}
	/* protect from exceeding screen space */
	if (ourhdr->length > SCREEN_HEIGHT - ourhdr->y)
	{
		ourhdr->length = SCREEN_HEIGHT - ourhdr->y;
	}

#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("length = %d\n", ourhdr->length);
#endif

	return 0;
}




/*%
**
**  SADOPL - Display/process the "on-the-fly" picklist
**
**	Usage:
**		CALLC sadopl name, target
**
**	Parameters:
**		name	-	Symbolic name of picklist previously built by 
**					sanewpl()/saaddpl().
**      target  -   The field in which the result is to be placed
**
**	Returns:
**		ret		-	A zero or positive result means that choice was
**                  made.  i.e. a 0 means the 1st picklist selection
**                  was chosen.  A negative result is the negated
**                  key selected, i.e. -K_ESC for <RESET>.
**
**	Synopsis:
**      Finishes up from building, then displays and processes the static
**		picklist built "on the fly" from within a PCL program.  Returns
**		The index of the item selected, and copies (as much as possible of)
**		the text for the item into the supplied field.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
*/


XDLL int 
sadopl(int argc, rtobj *argv[])
{
	static char name[42];
	field		*target;
	char buf[WKBUFLEN];
	int result;

	rtobj2str(argv[0],name,sizeof(name));
	target = (field *)argv[1]->ptr;
	if (argv[1]->obj.object != FLD || target->flags.type != FT_ASCII)
	{
		rterrorbox("sadopl(): requires an ASCII field as the second argument");
		return(CSR_FAIL_ARGTYPE);
	}
	result = c_sadopl(name, buf, sizeof(buf));
	if (result >= 0)
		str2field(buf, target);
	return result;
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_sadopl(char *name, char *target, int targlen)
{
	int retval, index, i;

	if (0 == name || '\0' == *name)
	{
		rterrorbox("sadopl(): requires symbolic name as first argument");
		return CSR_FAIL_ARGTYPE;
	}
	if (0 > (index = findname(name, &i)))
	{
		rterrorbox("sadopl(): couldn't find symbolic name");
		return VAL_FAIL_NO_MSG;
	}
#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "sadopl: symname = %s, index = %d\n", name, index);
#endif
	if (((picbody *)listpl[index]->picklist->list.ptr)->numitems == 0)
	{	/* No items for this pick list! */
		return -1;	
	}
	listpl[index]->plscreen = PCLfields;
	listpl[index]->foldflag = FALSE;
	retval = show_picklist (listpl[index], FALSE);

	/* if valid, return string value in target */
	if (target && retval >= 0)
	{
		strncpy(target,listpl[index]->lastpick,targlen);
		*(target+targlen-1) = '\0';
	}

	return retval;
}




/*%
**
**  SADOPV - Validate field against the "on-the-fly" picklist
**
**	Usage:
**		CALLC sadopv name, target, update
**
**	Parameters:
**		name	-	Symbolic name of picklist previously built by 
**					sanewpl()/saaddpl().
**      target  -   The field to validate against the picklist
**      update  -   1=show picklist/update field if invalid, 0=no update
**
**	Returns:
**		ret		-	0 == successful validation.
**                 -1 == validation failed.
**                 +n == Picklist item 'n' was selected via show_picklist.
**                       In this case, the number is 1-relative rather
**                       than 0-relative, so 1 means the 1st element.
**
**	Synopsis:
**      Finishes up from building, then verifies the target field
**      against the picklist build "on the fly" from within a PCL program.
**      If the field matches one of the items in the pick list, returns
**      0.  If the field does not match any item in the pick list, and
**      update is 0, returns -1.  If field does not match, and update is
**      1, display pick list and wait for operator selection.  If a
**      selection is made, field is updated to contain selection and index
**	 	(1-relative) is returned.  If no selection is made, return -1.
**
**	Errors:
**
**  Author:
**		Greg Eitzen - June, 1991
**
**	Notes:
*/


XDLL int 
sadopv(int argc, rtobj *argv[])
{
	static char name[42];
	field		*target;
	int update;

	rtobj2str(argv[0],name,sizeof(name));
	target = (field *)argv[1]->ptr;
	if (argv[1]->obj.object != FLD || target->flags.type != FT_ASCII)
	{
		rterrorbox("sadopv(): requires an ASCII field as the second argument");
		return(CSR_FAIL_ARGTYPE);
	}
	rtobj2int(argv[2],&update);
	return c_sadopv(name, target, update);
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_sadopv(char *name, field *target, int update)
{
	int index, i;
 	int retval;

	if (0 == name || '\0' == *name)
	{
		rterrorbox("sadopv(): requires symbolic name as first argument");
		return CSR_FAIL_ARGTYPE;
	}
	if (0 > (index = findname(name, &i)))
	{
		rterrorbox("sadopv(): couldn't find symbolic name");
		return VAL_FAIL_NO_MSG;
	}
#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "sadopv: symname = %s, index = %d\n", name, index);
#endif
	if (((picbody *)listpl[index]->picklist->list.ptr)->numitems == 0)
	{	/* No items for this pick list! */
		return -1;	
	}
	listpl[index]->plscreen = PCLfields;
	retval = val_picklist (target, listpl[index],
		(short)((update == 1) ? TRUE : FALSE));
	if (retval >= 0)	/* if valid, return string value in target field */
	{
		str2field(listpl[index]->pickitem[retval], target);
	}
	return retval;
}




/*%
**
**  SADROPPL - Dismiss a dynamic picklist
**
**	Usage:
**		CALLC sadroppl "plname"
**
**	Parameters:
**		parm1	-	Symbolic name of picklist
**		parm2	-	Optional file name to save picklist to
**
**	Returns:
**		ret		-	0 = success
**                  -1 = invalid name
**
**	Synopsis:
**      Frees up table slot and various memory associated with the named
**		dynamic picklist.
**
**	Errors:
**
**  Author:
**		Carl Paukstis - May, 1989
**
**	Notes:
*/


XDLL int 
sadroppl(int argc, rtobj *argv[])
{
	static char name[42], file[128], line[300];
	FILE	*fc_hdl = ((FILE *)0);
	int index, i;
	picbody *ourlist;
	picheader *ourhdr;
	char *ptr;

	if (argc > 2)
	{
		rterrorbox("sadroppl(): requires 1 or 2 argument");
		return CSR_FAIL_ARGCOUNT;
	}
	rtobj2str(argv[0],name,sizeof(name));
	if (argc == 1)
	{
		return c_sadroppl(name);
	}
	file[0] = '\0';

	if ((rtobj2str(argv[1],file,sizeof(file))) != RT_SUCCESS ||
		file[0] == '\0')
		return(CSR_FAIL_ARGTYPE);

	fc_hdl = fopen(file,"w");
	if (fc_hdl == (FILE *) 0 )
	{
		return(-99);
	}

	index = findname(&name[0], &i);

	ourhdr = listpl[index]->picklist;
	ourlist = (ourhdr->list).ptr;
	ptr = (char *)ourlist+sizeof(picbody);
	for (i = 0; i < ourlist->numitems; i++)
	{
		memcpy(line, ptr, StringLen(ptr)+1);
		strcat(line,"\n");
		ptr = ptr + StringLen(ptr) + 1;

		fputs(line,fc_hdl);
	}
	fclose(fc_hdl);
	return c_sadroppl(name);
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_sadroppl(char *name)
{
	int index, i;
	picheader *hdr;

	if (0 == name || '\0' == *name)
	{
		rterrorbox("sadroppl(): requires symbolic name as first argument");
		return CSR_FAIL_ARGTYPE;
	}
#ifdef DEBUGCODEWANTED
if (Debug)
	fprintf (stdout, "sadroppl: symname = %s\n", (name) ? name : "<NULL>");
#endif
	if (0 > (index = findname(name, &i)))
	{
  		return -1;		/* only a warning, not an rterrorbox() call */
	}
#ifdef DEBUGCODEWANTED
if (Debug)
	printf ("sadroppl: index = %d\n", index);
#endif
	
	hdr = listpl[index]->picklist;
	mem_free((char *)hdr->list.ptr);	/* the list */
	mem_free(hdr);						/* the header */
	mem_free(listnames[index]);			/* the name */
	mem_free(listpl[index]);			/* the structure */
	listpl[index] = 0;
	listnames[index] = 0;		/* nil name pointer means this slot is free */
	listbufsizes[index] = 0;
	listsizes[index] = 0;
	listw_width[index] = 0;
	listw_height[index] = 0;

	return 0;
}




/*%
**
**  SAMODPL - modify item to the on-the-fly picklist
**
**	Usage:
**		CALLC samodpl name value
**
**	Parameters:
**		parm1 - 	symbolic name of the picklist
**		parm2 - 	On value
**		parm3 - 	Off value
**		parm4 - 	starting position
**		parm5 - 	item to modify [optional] defualt = current
**
**	Returns:
**		none
**
**	Synopsis:
**
**	Errors:
**
**  Author:
**		Randy Geer
**
*/

XDLL int 
samodpl(int argc, rtobj *argv[])
{
	char onval[80], offval[80], name[WKBUFLEN];
	int index, i, pos,item;
	
	if (argc < 4) {
		rterrorbox("samodpl(): requires atleast  4 agruments");
		return CSR_FAIL_ARGCOUNT;
	}
	rtobj2str(argv[0],name,sizeof(name));
	rtobj2str(argv[1],onval,sizeof(onval));
	rtobj2str(argv[2],offval,sizeof(offval));
	rtobj2int(argv[3],&pos);
	if (argc == 5) {
		rtobj2int(argv[4],&item);
	}
	else {
		item = -1;
	}
		
	if (0 > (index = findname(&name[0], &i))) {
		rterrorbox("samodpl(): invalid name");
		return -1;
	}
	return (c_samodpl(listpl[index], onval, offval, pos,item) == -1);
}



/* extra function layer needed so we can call from C instead of PCL */
int 
c_samodpl(struct plwork *pl, char *onval, char *offval, int pos, int item)
{
	char 	*ptr, *item_ptr;
	int		rc;
	int		i, index;

	if (item == -1)
		index = pl->lp;
	else
		index = item;

	// point to the first piclist entry
	ptr = (char *)pl->picklist->list.ptr + sizeof(picbody);

	// advance to [index] entry
	for (i = 0; i < index; i++) {
		while (*ptr++)
			;
	}	 	

	// advance to desired char position
	ptr = ptr + pos;
	rc = strncmp(ptr,offval,strlen(offval));
	if (rc == 0) {
		item_ptr = onval;
	} else {
		item_ptr = offval;
	}
	while (*item_ptr != EOS) {
		*ptr = *item_ptr;
		ptr++;
		item_ptr++;
	}
	// --------------------------------------------------------------------
	// To insure that the item we just modified gets re-rendered correctly,
	// we will simply invalidate the RECT enclosing it and let the refresh
	// logic (in RT) deal with it.  This works both for selected and non-
	// selected items...
	// --------------------------------------------------------------------
	if (item == -1) {
		if (pl->cp < pl->lastline &&
			pl->cp >= (pl->lastline - pl->items_in_box)) {
			drawstring(pl->pickloc[pl->cp]);
			//hilite(pl->selbar);
			InvalidateRect(hWnd,&pl->pickloc[pl->cp]->bounds,0);
		}
	} else {
		if (item < pl->lastline &&
			item >= (pl->lastline - pl->items_in_box)) {
			drawstring(pl->pickloc[item]);
			InvalidateRect(hWnd,&pl->pickloc[item]->bounds,0);
		}
	}
return (TRUE);
}




/*%
**
**  SAGETPL - get item to the on-the-fly picklist
**
**	Usage:
**		CALLC sagetpl name value
**
**	Parameters:
**		parm1 - 	symbolic name of the picklist
**		parm2 - 	item
**		parm3 - 	On value
**		parm4 - 	position of value
**		parm5 - 	position of string
**		parm6 - 	return string
**
**	Returns:
**		none
**
**	Synopsis:
**
**	Errors:
**
**  Author:
**		Randy Geer
**
*/

XDLL int 
sagetpl(int argc, rtobj *argv[])
{
	char onval[80],  name[WKBUFLEN], outfield[WKBUFLEN];
	int index, i, item, result, pos, p_str;
	
	if (argc != 6)
	{
		rterrorbox("sagetpl(): requires 6 arguments");
		return CSR_FAIL_ARGCOUNT;
	}
	rtobj2str(argv[0],name,sizeof(name));
	rtobj2int(argv[1],&item);
	rtobj2str(argv[2],onval,sizeof(onval));
	rtobj2int(argv[3],&pos);
	rtobj2int(argv[4],&p_str);
	
	if (0 > (index = findname(&name[0], &i)))
	{
		rterrorbox("sagetpl(): invalid name");
		return -2;
	}
	result = c_sagetpl(listpl[index], item, onval, outfield);
	if (result == -2)
	{
		return (result);
	}
	str2field(&outfield[p_str],argv[5]->ptr);
	if (outfield[pos] == onval[0])
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_sagetpl(struct plwork *pl, int item, char *onval, char *outfield)
{
	char *ptr;

	if (item >= pl->items_in_list) {
		return (-2);
	}
	
	ptr = pl->pickitem[item];
	memcpy(outfield, ptr, StringLen(ptr)+1);
	return (VAL_PASS);
}



/*%
**
**  SAITEMPL - get current item to the on-the-fly picklist
**
**	Usage:
**		CALLC saitempl name value
**
**	Parameters:
**		parm1 - 	symbolic name of the picklist
**		parm2 - 	item
**
**	Returns:
**		none
**
**	Synopsis:
**
**	Errors:
**
**  Author:
**		Randy Geer
**
*/

XDLL int 
saitempl(int argc, rtobj *argv[])
{
	char name[WKBUFLEN];
	int index, i;
	
	if (argc != 1)
	{
		rterrorbox("saitempl(): requires 1 argument");
		return CSR_FAIL_ARGCOUNT;
	}
	rtobj2str(argv[0],name,sizeof(name));
	if (0 > (index = findname(&name[0], &i)))
	{
		rterrorbox("saitempl(): invalid name");
		return -1;
	}
	return (c_saitempl(listpl[index]));
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_saitempl(struct plwork *pl)
{
	int ch;
	ch = pl->lp;
 	if (ch > 0)
 	{
		ch = (pl->lp / pl->picklist->itemdepth);
	}
	return (ch);
}



/*%
**
**  SAADDPL_m - Add a menu picklist
**
**	Usage:
**		CALLC saaddpl_m name value
**
**	Parameters:
**		parm1 - 	symbolic name of the picklist
**		parm2 - 	List
**
**	Returns:
**		none
**
*/

XDLL int 
saaddpl_m(int argc, rtobj *argv[])
{
	complist	*cl;
	field			*fp;
	byte		*mem_ptr,
				mem_cnt;
	short		i;
	int		    flag1,
	   			flag2;
	index			itemno;
	char name[WKBUFLEN], val[WKBUFLEN];
	
	if (argc < 2) {
		rterrorbox("saaddpl_m(): requires 2 arguments");
		return CSR_FAIL_ARGCOUNT;
	}
	
	rtobj2str(argv[0],name,sizeof(name));
	if ('\0' == name[0]) {
		rterrorbox("saaddpl_m(): requires symbolic name as first argument");
		return CSR_FAIL_ARGTYPE;
	}
	
	fp = argv[1]->ptr;
	itemno = (fp->storage[2] << 8) + fp->storage[3];
	cl = (complist *)ddget(RTddh, fp->storage[0],fp->storage[1], itemno);

	if (argv[1]->obj.object == CSRARG_LST) {
		cl = (complist *)ddget(RTddh, argv[1]->obj.object,
					argv[1]->obj.applno, argv[1]->obj.itemno);
	}
	else {
		if (argv[1]->obj.object == CSRARG_FLD) {
			fp = argv[1]->ptr;
			itemno = (fp->storage[2] << 8) + fp->storage[3];
			cl = (complist *)ddget(RTddh, fp->storage[0],fp->storage[1], itemno);
		}
		else {
			rterrorbox("saaddpl_m(): requires first argument to be a list");
			return(-2);
		}
	}
	
	if (!cl) {
		rterrorbox("saaddpl_m(): Invalid list");
		return(-2);
	}

	for (i = 1; i < cl->numitems; i++) { 
		mem_ptr = _get_mem(i,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;

		pcl2int (mem_ptr,&flag1,NULL);
		mem_ptr += tsize(mem_ptr); /* skip elm 0 */
		pcl2int (mem_ptr,&flag2,NULL);
		mem_ptr += tsize(mem_ptr); /* skip elm 1 */

		pcl2str (mem_ptr,&val[0],WKBUFLEN-1,NULL);
		c_saaddpl(name, &val[0]);
	}
	return (VAL_PASS);
}



byte *
_get_mem(short i, byte *bp)
{
	short	len;

	if (i == 0)
		return(bp);
	while (i--) {
		bp += 2;			/* move past member type byte & field count */
		len = *bp++ << 8;	/* 2-byte length */
		len |= *bp++;
		bp += len;			/* point to next list element */
		}
	return(bp);
}



			/* find list using name  (alw) */
picbody *
c_saflist(char *name)
{
	int					index, i;

	index = findname(name, &i);
	if(index < 0) return((picbody *) 0);
	return((picbody *)(listpl[index]->picklist->list.ptr));
}



int 
sadebug(int argc, rtobj *argv[])
{
 	FILE		*fc_hdl = ((FILE *)0);
	int			i,j,x;
	char		*ptr;
	char		line[128];
	picbody		*ourlist;
	picheader	*ourhdr;
	char		filename[256];

    sprintf(filename, "%sdebug_pic", tempdir());
	fc_hdl = fopen(filename,"w");
	if (fc_hdl == (FILE *) 0 ) {
		return(-103);
	}

	j = 0;
	for (i=0; i < MAX_LISTS; i++) {
		if (listnames[i] != 0) {
			j++;
		}
	}

	sprintf(&line[0],"Dynamic Picklists  = %d\n",j);
	if (fputs(line,fc_hdl) == EOF) {
		return(-100);
	}


	for (i=0; i < MAX_LISTS; i++) {
		if (listnames[i] != 0) {
			sprintf(&line[0],"%2d  %s\n",i,listnames[i]);
		}
		else {
			sprintf(&line[0],"%2d\n",i);
		}
		
		if (fputs(line,fc_hdl) == EOF) {
			return(-100);
		}
		
		if (argc > 2 && listnames[i] != 0) {
			ourhdr = listpl[i]->picklist;
			ourlist = (ourhdr->list).ptr;
			ptr = (char *)ourlist+sizeof(picbody);
			for (x = 0; x < ourlist->numitems; x++) {
				sprintf(&line[0],"        %s\n",ptr);
				ptr = ptr + StringLen(ptr) + 1;
				fputs(line,fc_hdl);
			}
		}
	}
	
	fclose(fc_hdl);
	return(VAL_PASS);
}



/*% ** Author: Nagesh Nookala    Dec 1996
**
**  SACOMBOPL - Display/process the "on-the-fly" picklist. See the note below
**
**  Usage:
**      CALLC sacombopl name, target, tag
**
**  Parameters:
**      name    -   Symbolic name of picklist previously built by 
**                  sanewpl()/saaddpl().
**      target  -   The field in which the result is to be placed
**
**      tag     -   tag name which is used in the combofield statement
**
**	Note:	This functions is same as sadopl except it displays the
**	picklist under the combofield and turns it upside down if it is going to
**	exceed the rt window boundaries. This code uses the rt_comboverify logic.
*/


XDLL int 
sacombopl(int argc, rtobj *argv[])
{
    static char name[42];
    field       *target;
    char buf[WKBUFLEN];
    int result;
    int iu;
    
// Get the tag
    if (argv[2]->obj.object == TAG) {
        iu  = TAG << 24;
        iu |= argv[2]->obj.applno << 16;
        iu |= argv[2]->obj.itemno;
	}


    rtobj2str(argv[0],name,sizeof(name));
    target = (field *)argv[1]->ptr;
    if (argv[1]->obj.object != FLD || target->flags.type != FT_ASCII) {
        rterrorbox("sacombo(): requires an ASCII field as the second argument");
        return(CSR_FAIL_ARGTYPE);
    }
    
	result = c_sacombopl(name, buf, sizeof(buf), iu);
    if (result >= 0)
        str2field(buf, target);
    return result;
}


/* extra function layer needed so we can call from C instead of PCL */

int 
c_sacombopl(char *name, char *target, int targlen, int iu)
{
    int retval, index, i;
	picheader TempPH;

    if (0 == name || '\0' == *name)
    {
        rterrorbox("sacombopl(): requires symbolic name as first argument");
        return CSR_FAIL_ARGTYPE;
    }
    if (0 > (index = findname(name, &i)))
    {
        rterrorbox("sacombopl(): couldn't find symbolic name");
        return VAL_FAIL_NO_MSG;
    }
#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "sacombopl: symname = %s, index = %d\n", name, index);
#endif
    if (((picbody *)listpl[index]->picklist->list.ptr)->numitems == 0)
    {   /* No items for this pick list! */
        return -1;  
    }
    listpl[index]->plscreen = PCLfields;
    listpl[index]->foldflag = FALSE;
	TempPH = *listpl[index]->picklist;
    SetLeftTop (iu, (picheader *)listpl[index]->picklist);
    retval = show_picklist (listpl[index], FALSE);

    /* if valid, return string value in target */
    if (target && retval >= 0)
    {
        strncpy(target,listpl[index]->lastpick,targlen);
        *(target+targlen-1) = '\0';
    }
	*listpl[index]->picklist = TempPH;
    return retval;
}


//******************************************************************
//*********This code has been copied from rt_comboverify. *****************/
// It gets the field pointer and co-ordinates to which a given tag is attached
// and sets the pick-list x and y co-ordinates accordingly. The pick-list will
// be displayed upside down if the co-ordinates are going to cross the
// rt window bounadries.
// 
//	Nagesh Nookala 12/23/96
//**************************************************************************


int SetLeftTop(unsigned long tag, picheader *pic)
{
    field       *fp = NULL;
    dlist       *dlp, *fdp;
    rtcontrol   *crp;
	static HANDLE hRTdll;
	int			piclen;
	int			lentest;
	picbody		*ourlist;

	if (hRTdll == NULL)		// Get real address we need
		hRTdll = LoadLibrary("rt.dll");


    //field_focus(0,Current_screen,Currentdlp); // turn off current field focus
    //refreshfield(0,0,Currentdlp->obj);
    // ----------------------------------------------------------
    // Now get pointers to:
    //   crp    - and RTCONTROL struct for the pushbutton itself
    //   dsp    - the pushbutton's display list entry
    //   fdp    - the associated FIELD's display list
    //   pic    - the picklist object pointer
    //   fp     - Pointer to the associated field
    // ----------------------------------------------------------
    dlp = dl_getfirst(PCLscreen);
    while (dlp) {
        // ----------------------------------------------------------
        // Search the current screen for the object with the matching
        // TAG, and if we don't find one then put up an error message
        // ----------------------------------------------------------
        crp = dlp->obj;
		if (redrawcontrolfunc == NULL)
			redrawcontrolfunc = GetProcAddress(hRTdll,"redrawcontrol");

        if (crp != NULL && dlp->dspfunc == redrawcontrolfunc 
                && tag == (unsigned long)crp->tag) {
            // Get FIELD pointer
            if (crp->dlp2->dspfunc == drawTEfield)
                fp = ((struct RT_TEFIELD *)(crp->dlp2->obj))->fp;
            else
                fp = crp->dlp2->obj;       // Get FIELD pointer
            fdp = crp->dlp2;
            break;
            }
        dlp = dl_getnext(PCLscreen,dlp);
        }
    if (fp == NULL || dlp == NULL || crp == NULL) {
        mre(1,"Unknown tag");
        return(0);
        }


    // -----------------------------------------------------------
    //  Now grab the picklist header and adjust it's X,Y location
    //  so it lines up just below and aligned with the left of the
    //  associated field (unless the picklist would be off-screen)
    // -----------------------------------------------------------
    pic->x = fdp->bounds.left + 1;
#ifdef DEBUGCODEWANTED
if (Debug)
    printf("picklist width %d height %d\n", pic->width, pic->length);
#endif
    if ((pic->x + pic->width) > RTrect.right) {
		if ( crp->rect.right > (RTrect.right - crp->rect.left) ) {
        	// Then make it right-side aligned
			if( pic->width <= crp->rect.right ) {
        		pic->x = crp->rect.right - pic->width - 1;
				}
			else {
				pic->x = 1;
				}
			}
		}
	pic->y = crp->rect.bottom + 1;
	if ((pic->y + pic->length) > (RTrect.bottom
					- RTstatus_y - 1 - 20)) {	// extra 20 for titlebar
		// which space is larger
		if( (crp->rect.top - 1 ) > 
 			(RTrect.bottom - RTstatus_y - 21 - crp->rect.bottom) ){
			// above the field
			piclen = crp->rect.top - 2; // available space
			lentest = ((piclen / fontlength) * fontlength ) + 4;
			if( piclen > lentest ) {
				piclen = lentest - fontlength;
				}
			if( piclen > pic->length ) {
				pic->y = crp->rect.top - 1 - pic->length;
				}
			else {
				pic->y = crp->rect.top - 1 - piclen;
				}									   
			}
		else {
			// below the field
			pic->y = crp->rect.bottom + 1;
			piclen = RTrect.bottom - RTstatus_y - 21 - pic->y; // available space
			lentest = ((piclen / fontlength) * fontlength ) + 4;
			if( piclen > lentest ) {
				piclen = lentest - fontlength;
				}

			}

		pic->length = (pic->length > piclen)? piclen : pic->length;
		}
	pic->boxtype = 1;		// Force a single-line box type
	// see if a scroll bar will be added and adjust
	ourlist = (picbody*)pic->list.ptr;
	if( ourlist->numitems * fontlength > pic->width ) {
		if( ((int)pic->x + (int)pic->width + 17) <	SCREEN_WIDTH ) {
			pic->width += 17;	// added for scroll bar
			}
		else {
			pic->width = SCREEN_WIDTH - 1 - pic->x;	// provide maximum space
			}
		}
	return (0);
}


/*%
**
**  SACOMBOPV - Validate field against the "on-the-fly" picklist and display
**				the picklist just like sacombopl if there is no match
**
**	Usage:
**		CALLC sacombopv name, target, tag, update
**
**	Parameters:
**		name	-	Symbolic name of picklist previously built by 
**					sanewpl()/saaddpl().
**      target  -   The field to validate against the picklist
**      tag     -   tag name which is used in the combofield statement
**      update  -   1=show picklist/update field if invalid, 0=no update
**
**	Returns:
**		ret		-	0 == successful validation.
**                 -1 == validation failed.
**                 +n == Picklist item 'n' was selected via show_picklist.
**                       In this case, the number is 1-relative rather
**                       than 0-relative, so 1 means the 1st element.
**
**	Synopsis:
**      Finishes up from building, then verifies the target field
**      against the picklist build "on the fly" from within a PCL program.
**      If the field matches one of the items in the pick list, returns
**      0.  If the field does not match any item in the pick list, and
**      update is 0, returns -1.  If field does not match, and update is
**      1, display pick list under the combo field whose tag is passed
** 		as parameter 3 and wait for operator selection.  If a
**      selection is made, field is updated to contain selection and index
**	 	(1-relative) is returned.  If no selection is made, return -1.
**
*/


XDLL int 
sacombopv(int argc, rtobj *argv[])
{
	static char name[42];
	field		*target;
	int update;
    int iu;
    
// Get the tag
    if (argv[2]->obj.object == TAG) {
        iu  = TAG << 24;
        iu |= argv[2]->obj.applno << 16;
        iu |= argv[2]->obj.itemno;
        }


	rtobj2str(argv[0],name,sizeof(name));
	target = (field *)argv[1]->ptr;
	if (argv[1]->obj.object != FLD || target->flags.type != FT_ASCII)
	{
		rterrorbox("sacombopv(): requires an ASCII field as the second argument");
		return(CSR_FAIL_ARGTYPE);
	}
	rtobj2int(argv[3],&update);
	return c_sacombopv(name, target, update, iu);
}




/* extra function layer needed so we can call from C instead of PCL */
int 
c_sacombopv(char *name, field *target, int update, int iu)
{
	int index, i;
 	int retval;
	picheader TempPH;

	if (0 == name || '\0' == *name)
	{
		rterrorbox("sacombopv(): requires symbolic name as first argument");
		return CSR_FAIL_ARGTYPE;
	}
	if (0 > (index = findname(name, &i)))
	{
		rterrorbox("sacombopv(): couldn't find symbolic name");
		return VAL_FAIL_NO_MSG;
	}
#ifdef DEBUGCODEWANTED
if (Debug)
  fprintf(stdout, "sacombopv: symname = %s, index = %d\n", name, index);
#endif
	if (((picbody *)listpl[index]->picklist->list.ptr)->numitems == 0)
	{	/* No items for this pick list! */
		return -1;	
	}
	listpl[index]->plscreen = PCLfields;

    listpl[index]->plscreen = PCLfields;
    listpl[index]->foldflag = FALSE;
	TempPH = *listpl[index]->picklist;
    SetLeftTop (iu, (picheader *)listpl[index]->picklist);

	retval = val_picklist (target, listpl[index],
		(short)((update == 1) ? TRUE : FALSE));
	if (retval >= 0)	/* if valid, return string value in target field */
	{
		str2field(listpl[index]->pickitem[retval], target);
	}
	*listpl[index]->picklist = TempPH;
	return retval;
}


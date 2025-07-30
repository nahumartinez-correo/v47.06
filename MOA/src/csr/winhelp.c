/*$
** winhelp  --
**
** $Name: R1V4C13 $
** $Revision: 1.10 $
** $Date: 1999/03/11 08:29:30 $
**
**	Synopsis:
**		CSR interface to windows help
**
**	PCL EXAMPLES:
**
*/

#include "pch.h"
#include <winhelp.h>
#include <share.h>

#include <defines.h>
#include <typedefs.h>
#include <rtdef.h>
#include <csrdef.h>
#include <convers.h>
#include <rtprotos.h>
#include <memalloc.h>

#define NAMESIZE		128
#define	INDEXNOSIZE		5		/* 5 byte index number in keys	*/
#define	ASCIIKEYSIZE	128		/* max bytes in rt style key	*/

/* open modes */
#define	CLOSED		0
#define	READONLY	1
#define	READWRITE	2

extern void rterrorbox(char *, ...);

/* Function Prototypes */
static	int		whclose(int shutdown);
static	int		whstart(char *helpfile, char *keyfile, char *topicfile);
static	int		whindex(char whoption, int searchtype, char *userdefinedkey);
static	int		currentenvir(void);
static int		correctargs(int argcount, int min, int max);
static void		wherrmsg(char *text);
static	int		skip_white(char *from);
static	int		next_line(char *from);
static	void	term_line(char *from);
static int		currentenvir_drv (byte appl);
static int		currentenvir_std(void);
static int		whnextpickline(rtobj *arg);
static int		whlinktopic(rtobj *argv[]);
static void		make_key(int searchtype, char *asciikey, char *userdefinedkey);
static int		whtopictitle(char whoption, int searchtype,
						rtobj *rto, char *userdefinedkey);
static int count_includes(char *includefile, char *topicfile);
static int get_include_filesize(char *includefile, char *topicfile);
char *read_file(char *includefile, char *topicfile, char *buffer);


/* Globals */
static	char	editmode;
static	char	msgbuf[256];
static	short	msgsize = 256;
static	char	*msg = msgbuf;
static	char	*msg_ptr;
unsigned short whopened = CLOSED;
static char *CSRNAME = "winhelp";
static	int	wh_popup=0;
static	int	wh_finder=0;
/* topic flags	*/
#define	TOPIC_INUSE		0x01
#define	TOPIC_MALLOC	0x02

typedef struct HELPTOPIC {
	char	*topname;
	int		topicno;
	} helptopic;
typedef struct HELPKEY {
	char	*key;
	int		topicno,
			flags;
	} helpkey;
static helpkey		*HelpKeys;
static helptopic	*HelpTopics;

/* book name & key info storage for windows help link	*/
static char		HelpFileName[NAMESIZE],			/* windows .hlp file	*/
				KeyFileName[NAMESIZE],			/* rt keys + topic no's	*/
				TopicFileName[NAMESIZE];		/* windows .hpj file	*/
static char		*KeyBuff,
				*TopicBuff,
				*IncludeTopicBuff;
static int		KeyCount,
				TopicCount,
				IncludeTopicCount,
				KeyFD = -1;

/* currentenvir information */
static	byte	fld_appl_no,
				scr_appl_no,
				appl_no,
				mnu_appl_no,
				lst_appl_no;
static 	short	fld_item_no,
				scr_item_no,
				item_no,
				mnu_item_no,
				lst_item_no;
static	char	*ptrapplname, *ptritemname;


XDLL int
winhelp(int argc, rtobj * argv[])
{
	char	whoption;
	char	keyfile[NAMESIZE + 1],
			helpfile[NAMESIZE + 1],
			topicfile[NAMESIZE + 1];
	char	userdefinedkey[ASCIIKEYSIZE + 1];
	int 	searchtype,
			status;
	int	wh_option,wh_option_value;
	
	if (argc > 0) {
		if (rtobj2char(argv[0], &whoption) != RT_ERROR) {
			switch(whoption) {
				case _WHSTART:
					if (!correctargs(argc, 3, 4))
						return(CSR_FAIL_ARGCOUNT);
					/* get help file name	*/
					if (rtobj2str(argv[1], helpfile,
						sizeof(helpfile)) == RT_ERROR)
						return(CSR_FAIL_ARGTYPE);
					/* get keyfilename */
					if (rtobj2str(argv[2], keyfile,
					    sizeof(keyfile)) == RT_ERROR)
						return(CSR_FAIL_ARGTYPE);
					if (argc == 4) {
						/* have a help project file name too	*/
						if (rtobj2str(argv[3], topicfile,
							sizeof(topicfile)) == RT_ERROR)
							return(CSR_FAIL_ARGTYPE);
						}
					else {
						/* no topic file name	*/
						topicfile[0] = '\0';
						}
					status = whstart(helpfile, keyfile, topicfile);
					break;
				case _WHEXIT:
					if (!correctargs(argc, 1, 1))
						return(CSR_FAIL_ARGCOUNT);
					status = whclose(1);
					break;
				case _WHGETENV:
					if (!correctargs(argc, 1, 1))
						return(CSR_FAIL_ARGCOUNT);
					status = currentenvir();
					break;
				case _WHGETTOPICTITLE:
					if (!correctargs(argc, 3, 4))
						return(CSR_FAIL_ARGCOUNT);

					/* get search type	*/
					rtobj2int(argv[1], &searchtype);
					if (searchtype == WH_STRING && argc != 4)
						return(CSR_FAIL_ARGCOUNT);
					if (searchtype != WH_STRING && argc == 4)
						return(CSR_FAIL_ARGCOUNT);

					/* validate out target field */
					if (argv[2]->obj.object != FLD)
						return(CSR_FAIL_ARGTYPE);

					if (searchtype == WH_STRING) {
						/* get user defined key	*/
						if (rtobj2str(argv[3], userdefinedkey,
							sizeof(userdefinedkey)) == RT_ERROR)
							return(CSR_FAIL_ARGTYPE);
						}
					status = whtopictitle(whoption, searchtype,
						argv[2], userdefinedkey);
					break;
				case _WHINQKEY:
				case _WHSELKEY:
					/* inquire if there's a link to a key (WHINQKEY)	*/
					/* select the topic linked to a key (WHINQKEY)		*/
					if (!correctargs(argc, 2, 3))
						return(CSR_FAIL_ARGCOUNT);
					/* get searchtype for inquiry */
					rtobj2int(argv[1], &searchtype);

					/* Check to see if it is a user defined search		*/
					/* and get the userdefinedkey i.e. search string	*/
					if (searchtype == WH_STRING && argc != 3)
						return(CSR_FAIL_ARGCOUNT);
					if (searchtype != WH_STRING && argc != 2)
						return(CSR_FAIL_ARGCOUNT);
					if (searchtype == WH_STRING) {
						if (rtobj2str(argv[2], userdefinedkey,
							sizeof(userdefinedkey)) == RT_ERROR)
							return(CSR_FAIL_ARGTYPE);
						}

					status = whindex(whoption, searchtype, userdefinedkey);
					break;
				case _WHNEXTPICKLINE:
					/* next pick list entry	*/
					if (!correctargs(argc, 2, 2))
						return(CSR_FAIL_ARGCOUNT);
					if (argv[1]->obj.object != FLD)
						return(CSR_FAIL_ARGTYPE);
					status = whnextpickline(argv[1]);
					break;
				case _WHLINKTOPIC:
					/* make (or break) a link to a topic	*/
					if (!correctargs(argc, 4, 4))
						return(CSR_FAIL_ARGCOUNT);
					status = whlinktopic(argv);
					break;
				case _WHOPTION:
					if (rtobj2int(argv[1], &wh_option)) return CSR_FAIL_ARGTYPE;
					if (rtobj2int(argv[2], &wh_option_value)) return CSR_FAIL_ARGTYPE;
					if ( wh_option_value != 0 && wh_option_value != 1) return -2;
					switch(wh_option){
						case WH_POPUP:
							wh_popup = wh_option_value;
							break;
						case WH_FINDER:
							wh_finder = wh_option_value;
							break;
						default:
							return -2;
							break;
					}
					return 0;
					break;
				default:
					status = WH_UNKNOWN;
					break;
				}
			}
		else {
			/* Wrong argument type passed to winhelp */
			status = CSR_FAIL_ARGTYPE;
			}
		}
	else {
		/* Incorrect number of operands */
		status = CSR_FAIL_ARGCOUNT;
		}
	return(status);
}



static void
whsavekeys(void)
{
	int		idx;
	char	keyline[256];

	if (editmode) {
		/* need to save out the key  file	*/

		/* back to the beginning of the file	*/
		_lseek(KeyFD, 0L, SEEK_SET);
		for (idx = 0; idx < KeyCount; idx++) {
			if (HelpKeys[idx].flags & TOPIC_INUSE &&
				HelpKeys[idx].topicno != -1) {
				/* save out each key	*/
				sprintf(keyline, "%s|%d\r\n", 
					HelpKeys[idx].key, HelpKeys[idx].topicno);
				write(KeyFD, keyline, strlen(keyline));
				}
			}
		close(KeyFD);
		KeyFD = -1;
		*KeyFileName = '\0';
		editmode = 0;
		}
}



static int
whclose(int shutdown)
{
	int		idx;

	if (whopened) {
		/* write key file if it's open	*/
		whsavekeys();
		if (shutdown) {
			/* close help system	*/
			WinHelp(hWnd, HelpFileName, HELP_QUIT, 0L);
			}
		}
	else {
		return(WH_NOTOPEN);
		}
	if (KeyBuff) {
		mem_free(KeyBuff);
		KeyBuff = NULL;
		}
	if (HelpKeys) {
		for (idx = 0; idx < KeyCount; idx++) {
			if (HelpKeys[idx].flags & TOPIC_MALLOC)
				mem_free(HelpKeys[idx].key);
			}
		mem_free(HelpKeys);
		HelpKeys = NULL;
		}
	if (HelpTopics) {
		mem_free(HelpTopics);
		HelpTopics = NULL;
		}
	if (KeyFD != -1) {
		close(KeyFD);
		KeyFD = -1;
		}
	TopicFileName[0] = '\0';
	HelpFileName[0] = '\0';
	KeyFileName[0] = '\0';
	KeyCount = 0;
	TopicCount = 0;
	editmode = 0;
	whopened = 0;
	return(RT_SUCCESS);
}



static int
str_nocase_cmp(char *s1, char *s2)
{
	char	c1,
			c2;

	/* perform a case-insensitive string compare	*/
	while (*s1 && *s2) {
		if (*s1 != *s2) {
			c1 = *s1;
			if (isupper(c1))
				c1 = tolower(c1);
			c2 = *s2;
			if (isupper(c2))
				c2 = tolower(c2);
			if (c1 > c2)
				return(1);
			if (c2 > c1)
				return(-1);
			}
		s1++;
		s2++;
		}
	if (*s1)
		return(1);
	if (*s2)
		return(-1);
	return(0);
}



static int
topic_name_compare(helptopic *top1, helptopic *top2)
{
	return(str_nocase_cmp(top1->topname, top2->topname));
}



static int
topic_key_compare(helpkey *key1, helpkey *key2)
{
	return(str_nocase_cmp(key1->key, key2->key));
}



static void
sortkeys(void)
{
	qsort(HelpKeys, KeyCount, sizeof(helpkey),
		(int(__cdecl *)(const void *, const void *))topic_key_compare);
}



static void
sorttopics(void)
{
	qsort(HelpTopics, (TopicCount + IncludeTopicCount), sizeof(helptopic),
		(int(__cdecl *)(const void *, const void *))topic_name_compare);
}



static helptopic *
find_topic_by_no(int topicno)
{
	int		idx;
	for (idx = 0; idx < (TopicCount + IncludeTopicCount); idx++) {
		if (HelpTopics[idx].topicno == topicno)
			return(&HelpTopics[idx]);
		}
	return(NULL);
}



static helptopic *
find_topic(char *name)
{
	int		idx;
	/* a binary search would be faster here, the topics are sorted by name	*/
	for (idx = 0; idx < (TopicCount + IncludeTopicCount); idx++) {
		if (HelpTopics[idx].topname[0] == *name) {
			/* first char matches, go through the overhead of strcmp	*/
			if (!strcmp(name, HelpTopics[idx].topname)) {
				/* names match, return the number of this one	*/
				return(&HelpTopics[idx]);
				}
			}
		}
	return(NULL);
}



static helpkey *
find_key(char *name)
{
	int		idx;
	for (idx = 0; idx < KeyCount; idx++) {
		if (HelpKeys[idx].key[0] == *name) {
			/* first char matches, go through the overhead of strcmp	*/
			if (!strcmp(name, HelpKeys[idx].key)) {
				/* names match, return a pointer to this struct	*/
				if (HelpKeys[idx].flags & TOPIC_INUSE)
					return(&HelpKeys[idx]);
				else
					return(NULL);
				}
			}
		}
	return(NULL);
}



static int
topic_no(char *name)
{
	helptopic	*thistop;
	if (thistop = find_topic(name))
		return(thistop->topicno);
	return(-1);
}



static int
whopentopic(char *topicfile)
{
	struct stat	statbuf;
	int			bytes,
				found,
				idx,
				fd,
				includes,IncludeSize,offset;
	char		*srcptr,*i,
				*tmpptr,
				*startptr,
				*includeptr;


	char IncludeFile[100][256];
	if (HelpTopics) {
		mem_free(HelpTopics);
		HelpTopics = NULL;
		TopicFileName[0] = '\0';
		}
	TopicCount = 0;
	IncludeTopicCount = 0;
	editmode = 0;

	/* get the help topics from the help project file	*/
	if (*topicfile) {
		strcpy(TopicFileName, topicfile);
		/* make sure the requested file is there	*/
		if (stat(TopicFileName, &statbuf)) {
			return(WH_NOPROJFILE);
			}
		if (!(statbuf.st_mode & _S_IFREG) || !(statbuf.st_mode & _S_IREAD)) {
			/* not a regular file or no read permission	*/
			return(WH_NOPROJFILE);
			}

		if ((fd = open(TopicFileName, O_RDONLY | O_BINARY, 0)) == 0) {
			return(WH_NOPROJFILE);
			}

		TopicBuff = mem_alloc(statbuf.st_size + 1);
		*(TopicBuff + statbuf.st_size) = '\0';

		bytes = read(fd, TopicBuff, statbuf.st_size);
		close(fd);
		if (bytes != statbuf.st_size) {
			/* can't get the file	*/
			whclose(1);
			return(WH_NOPROJFILE);
			}

		srcptr = TopicBuff + skip_white(TopicBuff);
		TopicCount = 0;
		IncludeTopicCount = 0;
		IncludeSize = 0;
		includes = 0;
		found = 0;
		IncludeTopicBuff = 0;
		while (*srcptr && found < 2) {
			tmpptr = srcptr;
			srcptr += next_line(srcptr);
			/* find the MAP section	*/
			if (!found && (strncmp(tmpptr, "[MAP]", 5) == 0 ||
				strncmp(tmpptr, "[map]", 5) == 0)) {
				found++;
				startptr = srcptr;
				}
			else if (found && *tmpptr == '[') {
				/* next option section found, all done	*/
				found++;
				}
			else if (found == 1) {
				/* it's a keeper in the MAP section	*/
				// let's look for a #include
				if ( strstr(srcptr,"#include") == srcptr) {
					memset(IncludeFile[includes],0,256);
					i = strstr(srcptr,"<");
					if (i > 0) srcptr = i + 1;
					i = strstr(srcptr,">");
					if (i>0){
						memcpy(&IncludeFile[includes][0],srcptr,(i - srcptr));
						// get the file size so we can allocate
						// a buffer later to read all the files into
						IncludeSize += get_include_filesize(&IncludeFile[includes][0],TopicFileName);
						// get the number of #defines in the .hh file
						IncludeTopicCount += count_includes(&IncludeFile[includes][0],TopicFileName);
						includes++;						
					}
				}
				else
					TopicCount++;
				}
			}
		/* set up pointer array	*/
		HelpTopics = mem_calloc(TopicCount + IncludeTopicCount, sizeof(helptopic));

		// if we had include files, allocate a buffer for these
		// and read the files into the buffer
		if (includes) {
			IncludeTopicBuff = mem_alloc(IncludeSize);
			includeptr = IncludeTopicBuff;
			for (idx=0;idx<includes;idx++) {
				includeptr = read_file(IncludeFile[idx],TopicFileName,includeptr);
			}
		}
		/* set pointers to topic names	(directly in the map file)*/
		srcptr = startptr;
		for (idx = 0; idx < TopicCount; idx++) {
			/* set the pointer to the topic line	*/
			if ( strstr(srcptr,"#include") != srcptr) {
				HelpTopics[idx].topname = srcptr;
				/* terminate and extract the topic number	*/
				while (*srcptr && *srcptr != ' ' && *srcptr != '\t' &&
					*srcptr != '\r' && *srcptr != '\n')
					srcptr++;
				if (*srcptr) {
					*srcptr++ = '\0';
					HelpTopics[idx].topicno = atoi(srcptr);
				}
			}
			else {
				idx--;
				srcptr++;
			}
			srcptr += next_line(srcptr);
		}
		// now include the topics from the include files
		srcptr = IncludeTopicBuff;
		offset = idx;
		for (idx = 0; idx < IncludeTopicCount; idx++) {
			/* set the pointer to the topic line	*/
			if ( strstr(srcptr,"#define") == srcptr) {
				srcptr += 8;
				HelpTopics[offset].topname = srcptr;
				/* terminate and extract the topic number	*/
				while (*srcptr && *srcptr != ' ' && *srcptr != '\t' &&
					*srcptr != '\r' && *srcptr != '\n')
					srcptr++;
				if (*srcptr) {
					*srcptr++ = '\0';
					HelpTopics[offset].topicno = atoi(srcptr);
					offset++;
				}
			}
			else {
				idx--;
				srcptr++;
			}
			srcptr += next_line(srcptr);
		}
		sorttopics();
		editmode = 1;
		return(RT_SUCCESS);
		}
	return(WH_NOPROJFILE);
}



/*****************************************************************************
*	FUNCTION -		whstart - Send a message to winhelp to setup the
*					current help file.
*
*	PARAMETERS -	bookname (passed as a parameter to winhelp)
*
*   RETURN -		0     on success
*					-201  WH_NOKEYFILE
*
*****************************************************************************/
static int
whstart(char *helpfile, char *keyfile, char *topicfile)
{
	int		sametopic,
			samekey,
			samehelp,
			idx,
			keys,
			topics,
			result,
			newkeyfile;
	char	*srcptr,
			*tmpptr;
	struct stat	statbuf;

#	ifdef DEBUGCODEWANTED
		if(Debug)
			printf("%s: Book selection, helpfile='%s', keyfile='%s', topicfile='%s'\n", CSRNAME, helpfile, keyfile, topicfile);
#	endif

	if (whopened) {
		samehelp = strcmp(HelpFileName, helpfile);
		samekey = strcmp(KeyFileName, keyfile);
		sametopic = strcmp(TopicFileName, topicfile);
		if (samehelp != 0 || samekey != 0) {
			/* different help file or key file, close and re-open	*/
			if (samehelp == 0) {
				/* help file hasn't changed, don't need to close win help	*/
				whclose(0);
				}
			else {
				/* need to shut everything down	*/
				whclose(1);
				}
			}
		else if (sametopic != 0) {
			/* project file has changed	*/
			if (*topicfile == '\0') {
				/* no topic file, going from edit mode to read only mode	*/
				whsavekeys();
				*TopicFileName = '\0';
				return(RT_SUCCESS);
				}
			else if (*TopicFileName == '\0') {
				/* added topic file, going from read to edit mode	*/
				result = whopentopic(topicfile);
				if (result != RT_SUCCESS) {
					whclose(1);
					return(result);
					}
				}
			}
		else {
			/* already open and nothing changed, return OK	*/
			return(RT_SUCCESS);
			}
		}

	whopened = 0;

	/* make sure the requested help file is available	*/
	if (stat(helpfile, &statbuf)) {
		whclose(1);
		return(WH_NOHELPFILE);
		}
	if (!(statbuf.st_mode & _S_IFREG) || !(statbuf.st_mode & _S_IREAD)) {
		/* not a regular file or no read permission	*/
		return(WH_NOHELPFILE);
		whclose(1);
		}

	/* read the key file	*/
	strcpy(KeyFileName, keyfile);

	/* make sure the requested key file is there	*/
	newkeyfile = 0;
	if (stat(KeyFileName, &statbuf)) {
		if (*topicfile) {
			/* in edit mode and no key file, need to create a new one	*/
			newkeyfile = 1;
			statbuf.st_size = 0;
			}
		else {
			whclose(1);
			return(WH_NOKEYFILE);
			}
		}
		
	if (!newkeyfile) {
		if (!(statbuf.st_mode & _S_IFREG) || !(statbuf.st_mode & _S_IREAD)) {
			/* not a regular file or no read permission	*/
			whclose(1);
			return(WH_NOKEYFILE);
			}
		}

	if (*topicfile) {
		/* hpj file requested, we're in edit mode	*/
		KeyFD = sopen(KeyFileName, O_RDWR | O_BINARY | O_CREAT,
			_SH_DENYRW, _S_IREAD | _S_IWRITE);
		}
	else {
		/* read only mode	*/
		KeyFD = sopen(KeyFileName, O_RDONLY | O_BINARY, _SH_DENYRW, 0);
		}
	if (KeyFD == -1) {
		whclose(1);
		return(WH_NOKEYFILE);
		}

	KeyBuff = mem_alloc(statbuf.st_size + 1);
	*(KeyBuff + statbuf.st_size) = '\0';

	if (read(KeyFD, KeyBuff, statbuf.st_size) != statbuf.st_size) {
		/* can't get the file	*/
		close(KeyFD);
		KeyFD = -1;
		*KeyFileName = '\0';
		mem_free(KeyBuff);
		KeyBuff = NULL;
		whclose(1);
		return(WH_NOKEYFILE);
		}
	if (*topicfile == '\0') {
		/* read only mode, don't need to keep the key file locked	*/
		close(KeyFD);
		KeyFD = -1;
		}

	srcptr = KeyBuff + skip_white(KeyBuff);
	strncpy(HelpFileName, helpfile, sizeof(HelpFileName));

	/* count number of entries	*/
	KeyCount = 0;
	tmpptr = srcptr;
	while (*tmpptr) {
		KeyCount++;
		tmpptr += next_line(tmpptr);
		}
	/* set up pointer array	*/
	HelpKeys = mem_calloc(KeyCount, sizeof(helpkey));

	/* loop back through and set pointers to key entries	*/
	keys = 0;
	topics = 0;
	tmpptr = srcptr;
	while (*srcptr) {
		HelpKeys[keys].flags = TOPIC_INUSE;
		HelpKeys[keys++].key = srcptr;
		tmpptr = srcptr;
		srcptr += next_line(srcptr);
		term_line(tmpptr);
		}

	/* Get topic numbers for rt keys	*/
	for (idx = 0; idx < KeyCount; idx++) {
		/* get a pointer to the last byte in the key	*/
		srcptr = HelpKeys[idx].key;
		srcptr += strlen(srcptr) - 1;

		/* back up past white space	*/
		while (srcptr > HelpKeys[idx].key &&
			(*srcptr == ' ' || *srcptr == '\t'))
			*srcptr-- = '\0';

		/* back up to the delimiter ('|')	*/
		while (srcptr > HelpKeys[idx].key && *srcptr != '|')
			srcptr--;
		if (srcptr > HelpKeys[idx].key && *srcptr == '|') {
			/* delimiter found	*/
			*srcptr = '\0';
			HelpKeys[idx].topicno = atoi(srcptr + 1);
			}
		else {
			HelpKeys[idx].topicno = -1;
			}
		}
	sortkeys();

	if (*topicfile) {
		/* get the help topics from the help project file	*/
		result = whopentopic(topicfile);
		if (result != RT_SUCCESS) {
			whclose(1);
			return(result);
			}
		}

	whopened = 1;
	return(RT_SUCCESS);
}



static void
term_line(char *from)
{
	/* find first cr or lf & plug it with a null	*/
	while (*from && *from != '\r' && *from != '\n')
		from++;
	*from = '\0';	
}



static int
skip_white(char *from)
{
	int		howmany = 0;
	while (*from == ' ' || *from == '\t' || *from == '\n' || *from == '\r') {
		from++;
		howmany++;
		}
	return(howmany);
}



static int
next_line(char *from)
{
	int		howmany = 0;

	/* skip to end of current line	*/
	while (*from && *from != '\r' && *from != '\n') {
		from++;
		howmany++;
		}

	/* skip return and newline	*/
	while (*from == '\r' || *from == '\n') {
		from++;
		howmany++;
		}
	return(howmany + skip_white(from));

}



static int
whtopictitle(char whoption, int searchtype, rtobj *rto, char *userdefinedkey)
{
	char		asciikey[ASCIIKEYSIZE + 1];
	helpkey		*thiskey;
	helptopic	*thistop;
	field		*target;

	if (!whopened)
		return(WH_NOTOPEN);
	if (!editmode)
		return(WH_NOTEDITMODE);

	/* get pointer to target field */
	target = (field *)rto->ptr;

	/* build the key that matches the lookup option	*/
    memset(asciikey, 0, sizeof(asciikey));
	make_key(searchtype, asciikey, userdefinedkey);

	/* next, get the key structure we're interested in for this key	*/
	thistop = NULL;
	if (thiskey = find_key(asciikey)) {
		if (thistop = find_topic_by_no(thiskey->topicno)) {
			/* copy the topic title to the supplied field	*/
			str2field(thistop->topname, target);
			}
		}
	if (thistop == NULL) {
		/* couldn't match things up	*/
		str2field("", target);
		return(WH_NOMATCH);
		}
	return(RT_SUCCESS);
}



/*****************************************************************************
*	FUNCTION -	whindex -- inquiry if a message code of WH_INQKEY "a",
*				and select if a message code of WH_SELKEY "A".
*				inquiry index types include:
*					Current FLD and SCR (C)
*					Current FLD (F)
*					Current SCR (S)
*					Current PCL routine/Tran (T)
*					Current Menu (M)
*					User Defined Key (U)
*
*	PARAMETERS -	whoption(WH_INQKEY|WH_SELKEY)
*
*					searchtype 
*						WH_FLD
*						WH_SCR
*						WH_SCRFLD
*						WH_PCL		(transactions are PCL routines)
*						WH_STRING
*						WH_MNU
*						WH_ALL		(do all index searches)
*
*					userdefinedkey	(The search string)  
*
*   RETURN -		0 on success
*					WH_NOMATCH
*
*	REV. HISTORY -	Date		Name	Description
*					05/17/91	CDH		Created
*****************************************************************************/
static int
whindex(char whoption, int searchtype, char *userdefinedkey)
{
	char	asciikey[ASCIIKEYSIZE + 1];
	helpkey	*thiskey;

	if (!whopened)
		return(WH_NOTOPEN);

	if (whoption == _WHSELKEY && searchtype == 0) {
		if (wh_finder){
			if (WinHelp(hWnd, HelpFileName, HELP_FINDER, 0L))
				return(RT_SUCCESS);
		}
		else {		
			if (WinHelp(hWnd, HelpFileName, HELP_CONTENTS, 0L))
				return(RT_SUCCESS);
		}
		return(WH_WINHELPERROR);
		}

    memset(asciikey, 0, sizeof(asciikey));

	/* build the key we're interested in based on searchtype	*/
	make_key(searchtype, asciikey, userdefinedkey);

	if (thiskey = find_key(asciikey)) {
		if (whoption == _WHINQKEY) {
			return(RT_SUCCESS);
		}
		else {
			/* _WHSELKEY */
			if ( searchtype != WH_FLD && searchtype != WH_SCRFLD) {
				if (WinHelp(hWnd, HelpFileName, HELP_CONTEXT, thiskey->topicno))
					return(RT_SUCCESS);
			}
			else {
				if (wh_popup){
					if (WinHelp(hWnd, HelpFileName, HELP_CONTEXTPOPUP, thiskey->topicno))
						return(RT_SUCCESS);
				}
				else {
					if (WinHelp(hWnd, HelpFileName, HELP_CONTEXT, thiskey->topicno))
						return(RT_SUCCESS);
				}
			}
				
		}
				
	}
		
	return(WH_NOMATCH);
}



static int
currentenvir()
{
	byte appl;

	appl = ddapplindex(RTddh,"drv");
	if (Tran_current->appl_no == appl)
		return currentenvir_drv(appl);
	return currentenvir_std();
}



static int
currentenvir_std()
{
	int		status;
	itemidx	*fld_itemidx;
	static	char	applname[20 + 1],
					itemname[20 + 1];
	int		val;
	status = 0;
	/* get application and item number for current field	*/
	if (Currentfield && (fld_itemidx = ddgetitemidx(RTddh,FLD, Currentfield))) {
		/* field's itemidx located	*/
		fld_item_no = fld_itemidx->flapit & ITEMNO;
		fld_appl_no = (fld_itemidx->flapit & APPLNO) >> APPLSHIFT;
		if (fld_item_no && fld_appl_no) {
			status |= WH_FLD;
			}
		strcpy(applname, ddapplname(RTddh,fld_appl_no));
		strcpy(itemname, ddname(RTddh,FLD,fld_appl_no,fld_item_no));
		ptrapplname = applname;
		ptritemname = itemname;
#		ifdef DEBUGCODEWANTED
			if (Debug && fld_appl_no && fld_item_no)
				printf(": Current field is %s:%s\n", CSRNAME, ddapplname(RTddh,fld_appl_no),ddname(RTddh,FLD,fld_appl_no,fld_item_no));
#		endif
		}
	else {
		fld_appl_no = 0;
		fld_item_no = 0;
#		ifdef DEBUGCODEWANTED
			if (Debug)
				printf("%s: No current field\n", CSRNAME);
#		endif
		}	

	/* get application and item number for current screen	*/
	scr_appl_no = (byte)Current_screen->appl_no;
	scr_item_no = Current_screen->item_no;
	if (scr_appl_no && scr_item_no) {
		status |= WH_SCR;
		if (status & WH_FLD)
			status |= WH_SCRFLD;
#		ifdef DEBUGCODEWANTED
			if (Debug && scr_appl_no && scr_item_no)
				printf("%s: Current screen is %s:%s\n", CSRNAME,
					ddapplname(RTddh,scr_appl_no),ddname(RTddh,SCR,scr_appl_no,
						scr_item_no));
#		endif
		}
	else {
		scr_appl_no = 0;
		scr_item_no = 0;
#		ifdef DEBUGCODEWANTED
			if (Debug)
				printf("%s: No current screen\n", CSRNAME);
#		endif
		}

	/* get application and item number for current pcl routine (TRAN)	*/
	lst_appl_no = 0;
	lst_item_no = 0;
	appl_no = 0;
	item_no = 0;
	if (!(Tran_current->flags & MENUTRAN) && *RTflags & RTFLAG_intran) {
		if (*RTflags & RTFLAG_listtran) {
			/* in a list-driven transaction	*/
			field2int(List_tran_ano, &val);
			lst_appl_no = val;
			field2int(List_tran_ino, &val);
			lst_item_no = val;
			if (lst_appl_no && lst_item_no) {
				status |= WH_PCL;
#				ifdef DEBUGCODEWANTED
					if (Debug)
						printf("%s: Current tran list is %s:%s\n", CSRNAME,
							ddapplname(RTddh,lst_appl_no),
								ddname(RTddh,LST,lst_appl_no,lst_item_no));
#				endif
				}
			}
		else {
			field2int(Tran_ano,&val);
			appl_no = val;
			field2int(Tran_ino,&val);
			item_no = val;
			if (appl_no && item_no) {
				status |= WH_PCL;
#				ifdef DEBUGCODEWANTED
					if (Debug)
						printf("%s: Current pcl routine is %s:%s\n", CSRNAME,
							ddapplname(RTddh,appl_no),
								ddname(RTddh,PCL,appl_no,item_no));
#				endif
				}
			}
		}

	/* get application and item number for current menu	*/
	if (!scr_appl_no && (Tran_current->flags & MENUTRAN)) {
		/* menu's itemidx located	*/
		mnu_item_no = Tran_current->item_no;
		mnu_appl_no = (byte)Tran_current->appl_no;
		if (mnu_item_no && mnu_appl_no)
			status |= WH_MNU;
#		ifdef DEBUGCODEWANTED
			if (Debug && mnu_item_no && mnu_appl_no)
				printf("%s: Current menu is %s:%s\n", CSRNAME,
					ddapplname(RTddh,mnu_appl_no),
						ddname(RTddh,MNU,mnu_appl_no,mnu_item_no));
#		endif
		}
	else {
		mnu_appl_no = 0;
		mnu_item_no = 0;
#		ifdef DEBUGCODEWANTED
			if (Debug)
				printf("%s: No current menu\n", CSRNAME);
#		endif
		}	

	return(status);
}



static int
currentenvir_drv (byte appl)
{
	int		status;
	itemidx	*fld_itemidx;
	char	applname[20 + 1],
			itemname[20 + 1];
	field	*tfp,
			*sfp,
			*mfp;
	byte 	*ind;

	status = 0;
	lst_appl_no = 0;
	lst_item_no = 0;
	appl_no = 0;
	item_no = 0;
	fld_appl_no = 0;
	fld_item_no = 0;

	/* get application and item number for current field	*/
	if (Currentfield && (fld_itemidx = ddgetitemidx(RTddh,FLD, Currentfield))) 
	{
		/* field's itemidx located	*/
		fld_item_no = fld_itemidx->flapit & ITEMNO;
		fld_appl_no = (fld_itemidx->flapit & APPLNO) >> APPLSHIFT;
		if (fld_item_no && fld_appl_no) 
		{
			status |= WH_FLD;
			status |= WH_SCRFLD;
		}
		strcpy(applname, ddapplname(RTddh,fld_appl_no));
		strcpy(itemname, ddname(RTddh,FLD,fld_appl_no,fld_item_no));
		ptrapplname = applname;
		ptritemname = itemname;
	}
	
	tfp = ddgetbyname(RTddh,FLD,appl,"INDcur_tran");
	sfp = ddgetbyname(RTddh,FLD,appl,"INDcur_scr");
	mfp = ddgetbyname(RTddh,FLD,appl,"INDcur_menu");
	if (tfp == NULL || sfp == NULL || mfp == NULL)
		return (-1);

	/* get application and item number for current screen	*/
	if (sfp->storage != NULL) 
	{
		ind = (byte *) sfp->storage;
		ind++;
		scr_appl_no = *ind++;        /* application number */;
		scr_item_no = *ind++ << 8;	  /* index */
		scr_item_no |= *ind;
		if (scr_appl_no && scr_item_no)
			status |= WH_SCR;
	}

	/* get application and item number for current pcl routine (TRAN)	*/
	if (mfp->storage == NULL) 
	{
		if (tfp->storage != NULL) 
		{
			ind = (byte *) tfp->storage;
			ind++;
			appl_no = *ind++;        /* application number */;
			item_no = *ind++ << 8;	  /* index */
			item_no |= *ind;
			if (appl_no && item_no)
				status |= WH_PCL;
		}
	}
	else 
	{
			ind = (byte *) mfp->storage;
			ind++;
			appl_no = *ind++;        /* application number */;
			item_no = *ind++ << 8;	  /* index */
			item_no |= *ind;
			if (appl_no && item_no)
				status |= WH_PCL;
	}

	return(status);
}



static int
correctargs(int argcount, int min, int max)
{
	if (argcount < min || argcount > max) {
		/* argument count isn't correct	*/
		wherrmsg("Incorrect number of operands");
		return(0);
		}
	return(1);
}



static void
wherrmsg(char *text)
{
	char	msgbuf[256];
	sprintf(msgbuf, "%s: %s!", CSRNAME, text);
	rterrorbox(msgbuf);
}



static int
whnextpickline(rtobj *arg)
{
	static int	count;
	field		*target;

	target = (field *)arg->ptr;

	if (count == (TopicCount + IncludeTopicCount)) {
		/* end of topics, reset and return	*/
		count = 0;
		return(RT_ERROR);
		}

	str2field(HelpTopics[count++].topname, target);
	return(RT_SUCCESS);
}



static int
whlinktopic(rtobj *argv[])
{
	int			type,
				opcode;
	char		topicname[NAMESIZE],
				asciikey[NAMESIZE];
	helpkey		*thiskey;
	helptopic	*thistop;

	if (!whopened)
		return(WH_NOTOPEN);
	if (!editmode)
		return(WH_NOTEDITMODE);

	if (rtobj2str(argv[1], topicname, sizeof(topicname)) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[2], &opcode) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2int(argv[3], &type) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	/* build the key we're interested in	*/
	make_key(opcode, asciikey, "");
#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("whlinktopic, type %d, opcode 0x%02x, name '%s', key '%s'\n",
				type, opcode, topicname, asciikey);
#	endif

	/* find the entry for this key	*/
	thiskey = find_key(asciikey);

	if (thiskey) {
		/* already an entry for this key	*/
		/* get the entry for this topic name	*/
		if (type) {
			if (!(thistop = find_topic(topicname)))
				return(WH_NOMATCH);

			/* changing an entry	*/
#			ifdef DEBUGCODEWANTED
				if (Debug)
					printf("Changing topic for key '%s' from %d to %d\n",
						asciikey, thiskey->topicno, thistop->topicno);
#			endif
			thiskey->topicno = thistop->topicno;
			return(RT_SUCCESS);
			}
		else {
			/* removing an entry	*/
#			ifdef DEBUGCODEWANTED
				if (Debug)
					printf("removing entry '%s'\n", thiskey->key);
#			endif
			thiskey->topicno = -1;
			thiskey->flags &= ~TOPIC_INUSE;
			if (thiskey->flags & TOPIC_MALLOC) {
				mem_free(thiskey->key);
				thiskey->flags &= ~TOPIC_MALLOC;
				}
			return(RT_SUCCESS);
			}
		}
	else {
		/* adding a new key	*/
		if (!(thistop = find_topic(topicname)))
			return(WH_NOMATCH);

		HelpKeys = mem_realloc(HelpKeys, sizeof(helpkey) * (KeyCount + 1));
		HelpKeys[KeyCount].topicno = thistop->topicno;
		HelpKeys[KeyCount].key = mem_alloc(strlen(asciikey) + 1);
		strcpy(HelpKeys[KeyCount].key, asciikey);
		HelpKeys[KeyCount].flags = TOPIC_INUSE | TOPIC_MALLOC;
		KeyCount++;
		sortkeys();
		return(RT_SUCCESS);
		}
	return(WH_NOMATCH);
}



static void
make_key(int searchtype, char *asciikey, char *userdefinedkey)
{
	*asciikey = '\0';
	currentenvir();
	/* Format ascii key to send to winhelp based on searchtype */
	if (fld_appl_no && fld_item_no && scr_appl_no && scr_item_no
		&& (searchtype & WH_SCRFLD)) {
		/* NOTE: have to call ddname & ddapplname twice because	*/
		/* they'll return a pointer to the same static buffer	*/
		/* for screen and field									*/
		sprintf(asciikey, "%c:%s:%s,", 'C',
			ddapplname(RTddh,scr_appl_no),
			ddname(RTddh,SCR,scr_appl_no,scr_item_no));
		sprintf(asciikey + strlen(asciikey), "%s:%s",
			ddapplname(RTddh, fld_appl_no),
			ddname(RTddh, FLD, fld_appl_no, fld_item_no));
			}
	else if (fld_appl_no && fld_item_no && (searchtype & WH_FLD))
		sprintf(asciikey, "%c:%s:%s", 'F',
			ddapplname(RTddh,fld_appl_no),
			ddname(RTddh,FLD,fld_appl_no,fld_item_no));
	else if (scr_appl_no && scr_item_no && (searchtype & WH_SCR))
		sprintf(asciikey, "%c:%s:%s", 'S',
			ddapplname(RTddh,scr_appl_no),
			ddname(RTddh,SCR,scr_appl_no,scr_item_no));
	else if ((searchtype & WH_PCL) &&
		((item_no && appl_no) || (lst_item_no && lst_appl_no))) {
		/* do list driven transaction first if there is one	*/
		if (lst_item_no && lst_appl_no) {
			/* it's a list, build a list tran key	*/
			sprintf(asciikey, "%c:%s:%s", 'L',
				ddapplname(RTddh, lst_appl_no),
					ddname(RTddh, LST, lst_appl_no, lst_item_no));
			}
		else {
			/* it's a tran, but not a list driven tran, build a tran key */
			sprintf(asciikey, "%c:%s:%s", 'T',
				ddapplname(RTddh,appl_no),
				ddname(RTddh,PCL,appl_no,item_no));
			}
		}
	else if (mnu_item_no && mnu_appl_no && (searchtype & WH_MNU))
		sprintf(asciikey, "%c:%s:%s", 'M',
			ddapplname(RTddh,mnu_appl_no),
			ddname(RTddh,MNU,mnu_appl_no,mnu_item_no));
	else if (searchtype & WH_STRING)
		sprintf(asciikey, "%s", userdefinedkey);
#	ifdef DEBUGCODEWANTED
		if (Debug)
			printf("%s: asciikey =%s\n", CSRNAME, asciikey);
#	endif
}

// Get the filesize of the .hh file
// includefile is the name of the .hh file
// topicfile is the full path to the .hpj file
int get_include_filesize(char *includefile, char *topicfile)
{
	int i;
	char FullPath[256];
	struct stat	statbuf;
	memset(FullPath,0,sizeof(FullPath));
	i = strlen(topicfile);
	while(i){
		// search backwards for the '\'
		if (topicfile[i] == '\\') break;
		i--;
	}
	memcpy(FullPath,topicfile,i+1);
	sprintf(FullPath,"%s%s",FullPath,includefile);
	if (stat(FullPath, &statbuf) == -1) {
		return(0);
	}
	if (!(statbuf.st_mode & _S_IFREG) || !(statbuf.st_mode & _S_IREAD)) {
		/* not a regular file or no read permission	*/
		return(0);
	}

	return statbuf.st_size + 1;
}
// Count the # of #defines in the file passed
// as argument one.  Argument 2 will be used to
// obtain the path from
int count_includes(char *includefile, char *topicfile)
{
	int i;
	char FullPath[256];
	FILE *fd;
	char ReadLine[512];

	memset(FullPath,0,sizeof(FullPath));
	i = strlen(topicfile);
	while(i){
		// search backwards for the '\'
		if (topicfile[i] == '\\') break;
		i--;
	}
	if (!i) return 0;
	memcpy(FullPath,topicfile,i+1);
	sprintf(FullPath,"%s%s",FullPath,includefile);
	if ((fd = fopen(FullPath,"r")) == 0) {
		return(0);
	}
	i=0;
	while (fgets(ReadLine,sizeof(ReadLine),fd)){
		if (strstr(ReadLine,"#define") == ReadLine){
			i++;
		}
	}
	fclose(fd);
	return i;
}

// read a file into a pre-allocated buffer
// includefile is the file to read
// topicfile is where it will grab the path from
// buffer is a pointer to the buffer to place the
// file in
// returns a pointer to the end of the read data
char *read_file(char *includefile, char *topicfile, char *buffer)
{
	int i;
	char FullPath[256];
	struct stat	statbuf;
	int fd,bytes;
	char *ptr;

	ptr = buffer;

	memset(FullPath,0,sizeof(FullPath));
	i = strlen(topicfile);
	while(i){
		// search backwards for the '\'
		if (topicfile[i] == '\\') break;
		i--;
	}
	memcpy(FullPath,topicfile,i+1);
	sprintf(FullPath,"%s%s",FullPath,includefile);
	if (stat(FullPath, &statbuf) == -1) {
		return(ptr);
	}
	if (!(statbuf.st_mode & _S_IFREG) || !(statbuf.st_mode & _S_IREAD)) {
		/* not a regular file or no read permission	*/
		return(ptr);
	}

	if ((fd = open(FullPath, O_RDONLY | O_BINARY, 0)) == 0) {
		return(ptr);
	}

	bytes = read(fd, buffer, statbuf.st_size);
		close(fd);
	return (char *)buffer + statbuf.st_size;
}

/*$
** MODULENAME - csrprint.c
**
** $Name: R1V4C13 $
** $Revision: 1.14 $
** $Date: 1999/03/11 08:25:16 $
**
**	CCSR for Pinnacle Plus List Printing
**
**  Copyright (c) 1988 ISC Systems Corporation.
**  All rights reserved.
**
**	Written by: Dennis Weaver 5-19-89
**
**	Internal Functions called from PCL
**
**		printlist()		 front-end func for list printing
**
**	External Functions (in rt/fldsubs.c) called from PCL
**
**		copy_fld_list()	 make a copy of array of fields used 
**                       in current tran
**		undup_fld_list() move list fields data from dup to storage
**		clear_fld_list() clear list fields data
**
**	Other Internal support functions
**
**		parselst()		 parse list into array of structs
**		initcollpcl()	 get data collect pcl routine ano and ino
**		initcollflds()	 get pointers for data collect fields
**		stuffflds()		 stuff data collect fields from list
**		docollect()		 call pcl data collect proc
**		getflddata()	 get qty from collect fields
**		printlst()		 print requested number of copies of each
**                       doc in list
**
*/

#include "pch.h"

#include <typedefs.h>
#include <defines.h>
#include <pclwords.h>
#include <rtdef.h>
#include <csrdef.h>
#include <rt.h>
#include <rtprotos.h>
#include <convers.h>

#define DEBUGCODEWANTED	1
#define	MAXP	9
#define BYTE	unsigned char
#define DUP		1
#define NON_DUP 2
#define TITLELEN 40

/*
**	Document List Struct
**
**	An array of these structs is created by parselst().  One struct
**	for each list item.
*/
typedef struct pa PA;
struct pa {
	byte	Dtype;		/* document type (DOC or PCL) */
	byte	Dano;		/* PDOC application number */
	index	Dino;		/* PDOC index number */
	byte	Pano;		/* pre proc application number */
	index	Pino;		/* pre proc index number */
	char	dev[20];	/* logical printer */
	byte	qty;		/* number of copies */
	char	dup;		/* dup flag */
	field	*Mfp;		/* message field pointer	*/
	char	title[TITLELEN];	/* title of document */
};
/*
**	Generalized Collect Struct
*/
typedef struct gc GC;
struct gc {
	field	*title;	/* title of document */
	field	*qty;	/* number of copies */
};

	static void	parselst(PA *sp, byte *lp, int numlistitems);
	static byte *initcollpcl(byte *dc_list, byte *DCano, index *DCino);
	static int	initcollflds(byte *dc_list, int num_items);
	static int	stuffflds(PA *sp);
	static void docollect(byte DCano, index DCino);
	static void getflddata(PA *sp, int numdocs);
	static void printlst(PA *ap);

/*
**	Global Variables
*/
int			DeBug;
static GC	gca[MAXP];
static int	display;


/*eject*/
/*$
**
**	Get the named list from the field, parse the list, do collect
**	for dup/recall, print items in list
**
**	Args
**		argc	- number of args
**		argv	- array of pointers to structures
**
**		arg0	- indirect object containing the name of a list 
**		arg1	- int, 0 - no collect, 1 -  dup collect, 2 - non-dup collect
**		arg2    - list containing PCL data collect routine and field
**				  names for dup document screen
**
**	Return
**		none
**
*/

XDLL int
printlist(int argc, rtobj *argv[])
{
	complist	*lp;
	PA			ptr[MAXP];
	int			numdocs;
	int			num_items;
	char		*rlp;
	byte		Lano;
	byte		*dc_list;
	index		Lino;
	byte		*ind;
	complist*	dc_list_hdr;
	byte		DCano;		/* data collect routine application number */
	index		DCino;		/* data collect routine index number */
	byte		*initcollpcl();
	field		*indfp;

	/* check arguments */
	if (argc < 2 || argc > 3) {
		rterrmsg_via_list(0,RTprtmsg_nowait,
							"printlist: wrong number of arguments",-1);
		return(CSR_FAIL_ARGCOUNT);
	}

	/* verify the the first argument is a type FT_IND field	*/
	if (argv[0]->obj.object != CSRARG_FLD) {
		rterrmsg_via_list(0,RTprtmsg_nowait,
						"1st arg to printlist must be a field of type IND",-1);
		return(CSR_FAIL_ARGTYPE);
	}
	indfp = argv[0]->ptr;
	if (indfp->flags.type != FT_IND) {
		rterrmsg_via_list(0,RTprtmsg_nowait,
						"1st arg to printlist must be a field of type IND",-1);
		return(CSR_FAIL_ARGTYPE);
	}
	ind = (byte *) indfp->storage;
	if (ind == NULL || *ind == 0)
		return(0);		/* empty IND object means do nothing gracefully */

	if (*ind++ != LST) {
		rterrmsg_via_list(0,RTprtmsg_nowait,
			"1st arg to printlist must be IND field which references a LIST",-1);
		return(CSR_FAIL_ARGTYPE);
	}
	if (argv[1]->obj.object != CSRARG_INT) {
		rterrmsg_via_list(0,RTprtmsg_nowait,"2nd arg to printlist must be INT",-1);
		return(CSR_FAIL_ARGTYPE);
	}
	display = (int)argv[1]->ptr;

	if (display) {
		if (argc != 3) {
			rterrmsg_via_list(0,RTprtmsg_nowait,
				"3rd arg to printlist must be specified when 2nd arg = 1 or 2",-1);
			return(CSR_FAIL_ARGCOUNT);
		}
		if (argv[2]->obj.object != CSRARG_LST) {
			rterrmsg_via_list(0,RTprtmsg_nowait,
								"3rd arg to printlist must be LIST",-1);
			return(CSR_FAIL_ARGTYPE);
		}
	}

	/* Get documents list loaded into memory */
	Lano = *ind++;        /* application number */
	Lino = *ind++ << 8;	  /* index */
	Lino |= *ind;
	if ((lp = (complist *)ddget(RTddh,LST,Lano,Lino)) == 0) {
		char temp[128];
		sprintf(temp,"ERROR, list (%d:%04d) not found", Lano,Lino);
		rterrmsg_via_list(0,RTprtmsg_nowait,temp,-1);
		return(0);
		}

	/* Initialize array of structs & point at begining of list */
	memset(ptr,0,sizeof(PA)*MAXP);
	rlp = (char *)lp;
	rlp += sizeof(complist);
	parselst((PA *)&ptr[0],(byte *)rlp,lp->numitems);

	/* if requested, Do data collect */
	if (display) {
#ifdef DEBUGCODEWANTED
if (Debug) printf("printlist: doing display code\n");
#endif

		/* get data collect routine from list */
#ifdef DEBUGCODEWANTED
if (Debug) printf("printlist: getting data collect LIST\n");
#endif
		dc_list_hdr = (complist *)(argv[2]->ptr);
		num_items = dc_list_hdr->numitems;
		dc_list = (byte *)dc_list_hdr + sizeof(complist);
		if ((dc_list = initcollpcl(dc_list, &DCano, &DCino)) == NULL) {
			return -2;
		}
		num_items--;

		/* Initialize pointers to collect fields */
#ifdef DEBUGCODEWANTED
if (Debug) printf("printlist: getting data collect fields\n");
#endif
		if (initcollflds(dc_list, num_items) == -1) {
			return -2;
		}
		numdocs = stuffflds(&ptr[0]);
		if (numdocs > 0) {
			docollect(DCano, DCino);
			getflddata(&ptr[0],numdocs);
			printlst(&ptr[0]);
		}
	}
	else {
		printlst(&ptr[0]);
	}
	return(RT_SUCCESS);
}
/*eject*/
/*$
**
**	Parse the List into an array of structs of pointers
**	into the list
**
**	Args
**		ap		 - points to array of structs (pointers into list)
**		rlp		 - points to the beginning of list (past header)
**		numitems - number of items in the list
**
**	Return
**		none
**
*/

static void
parselst(PA *sp, byte *lp, int numlistitems)
{
	int		i;
	short	objsize;

	for (i=0; i<MAXP && i<numlistitems; i++, sp++) {
		/*
		**	Parse each record in the list
		*/
		if (*lp != MEMBER)
			break;
		lp += 4;		/* skip item count & byte count */
		/*
		**	Get document applno & indexno
		*/
		if (*lp != DOC && *lp != OFB)
			break;
		sp->Dtype = *lp++;
		sp->Dano  = *lp++;
		sp->Dino  = *lp++ << 8;
		sp->Dino |= *lp++;
		/*
		**	Get Pre Document Proc applno & indexno
		*/
		if (*lp++ != OFB)
			break;
		sp->Pano  = *lp++;
		sp->Pino  = *lp++ << 8;
		sp->Pino |= *lp++;
		/*
		**	Get logical device string
		*/
		if (*lp++ != TYPE_SCON)
			break;
		strncpy((char *)sp->dev, (char *)lp+1, *lp);
		lp += (*lp + 1);
		/*
		**	Get number of copies
		*/
		if (*lp++ != TYPE_ICON1)
			break;
		sp->qty = (byte)*lp++;
		/* 
		**	Get dup flag
		*/
		if (*lp == TYPE_SCON) {
			if (*++lp != 1)
				break;			/* string must be a single char */
			lp++;
			sp->dup = *lp++;
			}
		else if (*lp == TYPE_CCON) {
			lp++;
			sp->dup = *lp++;	/* character constant is also okay */
			}
		else
			break;				/* bad item in list */
		if (sp->dup == 'Y') 
			sp->dup = 'y';  /* translate upper case to lower */
		/*
		**	Get fyi message field
		*/
		if (*lp != FLD && *lp != TYPE_ARRAY)
			break;
		sp->Mfp = rt_get_fp(lp, &objsize, NULL);
		if (!sp)
			break;
		lp += objsize;
		/*
		**	Get document title string
		*/
		if (*lp++ != TYPE_SCON)
			break;
		/* truncate title if too long */
		strncpy((char *)sp->title, (char *)(lp+1),*lp>TITLELEN?TITLELEN:*lp); 
		lp += (*lp + 1);
#ifdef DEBUGCODEWANTED
	if (DeBug > 0) {
	printf("%2X:%d:%d %d:%d %s %d %c %x %s\n",sp->Dtype,sp->Dano,sp->Dino,
				sp->Pano,sp->Pino,sp->dev,sp->qty,sp->dup,sp->Mfp,
				sp->title);
	}
#endif
	}
}
/*eject*/
/*$
**
**	Get Ano and Ino for data collect PCL routine  from list
**
**	Args
**		dc_list	- pointer to list
**		DCano   - pointer to application number
**		DCino   - pointer to index number
**
**	Return
**		updated dc_list
**
*/
static byte	*
initcollpcl(byte *dc_list, byte *DCano, index *DCino)
{
	/* skip MEMBER byte */
	dc_list++;
		
	/* if more than one element in member, error */
	if (*dc_list++ != (byte) 1) {
		rterrmsg_via_list(0,RTprtmsg_nowait,"printlist: Data Collect List member has more than one element",-1);
		return (NULL);
	}
	dc_list += 2;				/* move past length */
#ifdef DEBUGCODEWANTED
if (Debug) printf("This should be OFB type %x: %x\n", OFB, *dc_list);
#endif

	if (*dc_list++ != OFB) {		/* member must be PCL */
		rterrmsg_via_list(0,RTprtmsg_nowait,"printlist: Data Collect List firt member should be PCL routine name",-1);
		return (NULL);
	}
	*DCano = *dc_list++;				 /* application number */
	*DCino = *dc_list++ <<8; 
	*DCino |= *dc_list++; /* field number */
#ifdef DEBUGCODEWANTED
if (Debug) printf("PCL ano = %d, ino = %d\n", *DCano, *DCino);
#endif

	return (dc_list);
}

/*eject*/
/*$
**
**	Stuff array of collect structs w pointers to collect fields
**
**	Args
**		sp	- pointer to array of structs (of pointers to list)
**
**	Return
**		none
**
*/
static int
initcollflds(byte *dc_list, int num_items)
{
	short	i;
	GC		*gcp;
	field	*fields[2];
	int		sa_get_fptr();

	gcp = &gca[0];
	for (i=0; i<MAXP && num_items > 0; i++, gcp++) {
		/* Get title and quantity fields */
		if (sa_get_fptr(&dc_list, &num_items, 2, fields) == -1)
			return -1;
		gcp->qty = fields[0];
		gcp->title = fields[1];
	}
	return 0;
}
/*eject*/
/*$
**
**	Stuff the data collect 'title' & 'qty' fields from the
**	parsed list into the general collect fields
**
**	Args
**		sp		- pointer to array of structs (of pointers to list)
**
**	Return
**		numdocs	- number of docs stuffed in the collect fields
**
*/

static int
stuffflds(PA *sp)
{
	short	i, j;
	GC		*gcp;
	int		numdocs=0;

	gcp = &gca[0];
	for (i=0; i<MAXP; i++, gcp++) {
		/*
		**	Mark all fields as hidden
		*/
		if (gcp->title != NULL){
			gcp->title->flags.hidden = 1;
			gcp->qty->flags.hidden = 1;
		}
	}
	gcp = &gca[0];
	for (i=0; i<MAXP; i++, sp++) {
		/*
		**	Stuff general collect fields until we
		**	run out of gc fields or items in list
		**	and unhide all used fields
		*/
		if (gcp->title != NULL) {

			/* if in non-dup mode or in dup mode and document is dupable */
			/* put the document on the screen */
			if (display == NON_DUP || sp->dup == (char)'y') {
				memset(gcp->title->storage,' ',gcp->title->length);
				for (j=0; j<gcp->title->length && sp->title[j] != '\0'; ++j)
					gcp->title->storage[j] = sp->title[j];
				sprintf((char *)gcp->qty->storage,"%*d",gcp->qty->length,
												sp->qty);
				gcp->qty->numchars = strlen(gcp->qty->storage);
				gcp->title->numchars = j;
				if (j > 0) {
					gcp->title->flags.hidden = 0;
					gcp->qty->flags.hidden = 0;
				}
				else {
					gcp->title->flags.hidden = 1;
					gcp->qty->flags.hidden = 1;
				}
				gcp->qty->flags.dataentered = 1;
				gcp++;
				numdocs++;
			}
		}
	}
	return(numdocs);
}
/*eject*/
/*$
**
**	Build and run the generalized data collect proc for
**	dup/recall
**
**	Args
**		none
**
**	Return
**		none
**
*/

static void
docollect(byte DCano, index DCino)
{
	int		retval;
	char	temp[128];

	/*	Call pcl to do data collect */
	if ((retval = pcl(0,PCL,DCano,DCino,0,NULL)) != RT_SUCCESS) {
		sprintf(temp,"pcl() for data collect function %d:%d failed code %d",
								DCano,DCino,retval);
		rterrmsg_via_list(0,RTprtmsg_nowait,temp,-1);
		}
}
/*eject*/
/*$
**
**	Get data from data collect fields and put number of copies
**	in array of structs (of pointers into list)
**
**	Args
**		sp		- pointer to array of structs (of pointers to list)
**		numdocs	- number of docs in data collect fields
**
**	Return
**		none
**
*/

static void
getflddata(PA *sp,int numdocs)
{
	int		i,j,result;
	GC		*gcp;

	gcp = &gca[0];
	for (i=0,j=0; i<MAXP && i<numdocs; i++, gcp++, j++) {
		if (gcp->title != NULL) {
			if (display == DUP) {
				while(sp->dup != 'y') {
					/* Skip the non-dupable fields */
					sp->qty = 0;
					sp++;
					j++;
					}
				}
			/* following routine sets 'result' to 0 if error */
			field2int(gcp->qty, &result);
			sp->qty = result;
			sp++;
			}
		}
	/* Skip trailing non-dupable documents */
	if (gcp->title != NULL) {
		if (display == DUP) {
			for (;j < MAXP; j++, sp++)
				sp->qty = 0;
			}
		}
}
/*eject*/
/*$
**
**	For each docment in the list
**		Get the pre document proc and run it
**		Build the 'PRINTDOC' proc and run it once for each copy
**
**	Args
**		ap	- pointer to array of structs of pointers into list
**
**	Return
**		none
**
*/

static void
printlst(PA *ap)
{
	int		i, j;
	int		retval;
	byte	*bp;		/* pointer to where we are in the buffer*/
	pcldef	*pclp;		/* pointer to the pcl proc */
	byte	space[sizeof(pcldef) + 100];

	/*
	**	Get memory for pcl header & proc
	*/
	pclp = (pcldef *)space;
	pclp->PPip = (byte *)(pclp + 1);
	bp = pclp->PPip;	
	/*
	**	Print each document in the list
	*/
	/* clear RTFLAG_prtreset bit before 1st document */
	*RTflags &= ~RTFLAG_prtreset;
	for (i=0; i<MAXP; i++, ap++) {
		if (!ap->Dano || !ap->Dino || !ap->dev)
			continue;
		/*
		**	Print requested number copies of document
		*/
		for (j=0; j<ap->qty; j++) {
#ifdef DEBUGCODEWANTED
			if (DeBug > 1) {
				printf("%s\n",ap->title);
				if (ap->Pano) printf("PRE PROC exists\n");
			}
#endif
			/*
			**	Call pcl to run document preprocessing proc
			*/
			if (ap->Pano) {
				if ((retval =
					pcl(0,PCL,ap->Pano,ap->Pino,0,NULL)) != RT_SUCCESS) {
					char temp[128];
					sprintf(temp,"pcl() for pre proc %d:%d failed code %d",
								ap->Pano,ap->Pino,retval);
					rterrmsg_via_list(0,RTprtmsg_nowait,temp,-1);
					}
				}
			if (Retval != RT_SUCCESS) {
				/*
				**	Predoc proc says don't print this doc
				*/
				continue;
			}
			/*
			**	Call print package to print the document
			*/
			if ((retval = print_package(0,ap->Dtype,ap->Dano,ap->Dino,
				(ap->dev),(char *)(ap->Mfp->storage),(void *)0,
				(void *)0,NULL)) != RT_SUCCESS) {
				char temp[128];
				sprintf(temp,"print_package() for doc %02X:%d:%d failed code %d"
							,ap->Dtype,ap->Dano,ap->Dino,retval);
				rterrmsg_via_list(0,RTprtmsg_nowait,temp,-1);
				}
			if (RTflags && (*RTflags & RTFLAG_prtreset))
				break;
		}
		if (RTflags && (*RTflags & RTFLAG_prtreset))
			break;
	}
}
/*eject*/
/*$
**
**	Convert NULL terminated ascii string (app:obj) to ano & ino
**	and store ano & ino where requested
**
**	Args
**		app		- ascii application name
**		obj		- ascii object name
**		anop	- store app number at this address
**		inop	- store obj number at this address
**
**	Return
**		none, but ano & ino will be '0' if error
**
*/


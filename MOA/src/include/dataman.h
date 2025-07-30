/*****************************************************************************
* Header File For Dictionary Manager
*****************************************************************************/
#ifndef DATAMAN_H
#define	DATAMAN_H

#ifndef THIS_SYS_H
#   include  <this_sys.h>
#endif
#ifndef TYPEDEFS_H
#	include <typedefs.h>	/* 'cause of references to 'byte' herein */
#endif

#define		MOADIR			"MOADIR"
#define		MOAPROJ			"MOAPROJ"
#define		MOACURPROJ		"MOACURPROJ"
#define		MOAREGISTRYKEY	"SOFTWARE\\Olivetti North America\\mosaic OA"

#define adjlen(i) (i&1)

#include <ddrevno.h>
#include <dictobjs.h>

#define		MAXAPPL			127		/* max number of applications */
#define		MAX_OPEN_DICTS	4		/* max dictionaries opened at same time	*/

#define	HDR_SIZE		sizeof(long)	/* revision # at top of cfig file	*/

#define		RUN				0
#define		MAINTAIN		1
#define		RUN_UNLOCKED	2
#define		NOTE_BIT		0x80
#define		NOTE_RUN		(RUN | NOTE_BIT) 
#define		NOTE_MAINTAIN	(MAINTAIN | NOTE_BIT)

/* types for ddgetlist	*/
#define		LISTINIT		0	/* use one of these 3 for the type when		*/
#define		LISTCONT		1	/* calling ddgetlist						*/
#define		LISTSTOP		2
#define		LISTNONDELETED	4	/* or with LISTINIT or LISTCONT	to get a	*/
								/* list of only NON DELETED objects			*/
#define		LISTDELETED		8	/* or with LISTINIT or LISTCONT to get a	*/
								/* list of only DELETED objects				*/
#define		LISTTYPEMASK	(LISTINIT | LISTCONT | LISTSTOP)
#define		LISTDELMASK		(LISTNONDELETED | LISTDELETED)

#define		MAXINDEXES		20

#define		DDFIRSTTYPE		'A'		/* data type used as first byte of key	*/
#define		DDFORCELOAD		0
#define		DDLOADIFMAINT	1

#define		TYPE_KEYSIZE	1	/* dictionary type included in key		*/
#define		APPL_KEYSIZE	3	/* 3 byte application number in key		*/
#define		ITEM_KEYSIZE	5	/* 5 byte item number in key			*/
#define		NAME_KEYSIZE	20	/* 20 byte name in key					*/
#define		STAT_KEYSIZE	1	/* 1 byte status in key					*/
#define		MAX_PATH_NAME	(size_t) 256
//#ifndef MAX_CDS_HOST
//#define		MAX_CDS_HOST	(size_t) MACHINE_NAME_LEN  // see also cdslmts.h
//#endif
#define		MAX_PROJ_NAME	(size_t) 256
#ifndef MAX_REV_LEVEL
#define		MAX_REV_LEVEL	(size_t) 2
#endif
#define		MAX_DICT_EXT	(size_t) 3
#define		MAX_FILE_NAME	(size_t) 12
#define		TOTAL_KEYSIZE	(size_t) (TYPE_KEYSIZE + NAME_KEYSIZE + APPL_KEYSIZE + \
							ITEM_KEYSIZE + STAT_KEYSIZE)

#define		BTREENOINIT		0
#define		BTREEINIT 		1

/* Defines Used In Index Structures */
#define ITEMNO		0x0000ffff	/* AND With flapit To Mask Itemno		*/
#define APPLNO		0x00ff0000	/* AND With flapit To Mask Applno		*/
#define	STATMASK	0xff000000	/* Mask for status byte					*/
#define APPLSHIFT	16			/* Number Of Bits To Shift For Applno	*/
#define	STATSHIFT	24			/* # bits to shift to get status byte	*/

/* status bits used in objects' status bytes	*/
#define		DDNORMALSTAT	0x0000	/* no bits set for normal status		*/
#define		DDMODIFIED		0x0001	/* status bit for modified object		*/
#define		DDFWDREFD		0x0002	/* status bit for fwd ref'd object		*/
#define		DDDELETED		0x0004	/* status bit for deleted object		*/
#define		DDCOMPERROR		0x0008	/* pcl object compiled with error		*/
#define		DDTEMP			0x0010	/* temporary stat bit used by utilities	*/
#define 	DDOCCUPIED		0x0020	/* AND With flapit To Test, OR To Set	*/
#define		DDMAKEASCII		DDOCCUPIED	/* this bit doubles as in indicator	*/
									/* that an object in memory is occupied,*/
									/* and as the bit to make sure the key	*/
									/* in the btree is ASCII.  It is ALWAYS	*/
									/* on, except when dddel turns it off,	*/
									/* but ddput turns it right back on.	*/
#define		DDLOADED		0x0040	/* AND With flapit To Test, OR To Set 	*/
#define		DDSTATFAIL		0x00ff	/* result when status request failed	*/

/*eject*/
/* Typedefs Used By Dictionary Manager */
typedef struct ITEMIDX {
	void		*itemptr;		/* Pointer To Item Storage */
	long		daddr;			/* Disk Offset */
	int			size;			/* Size Of Item On Disk */
	long		flapit;			/* FLags, APplication, and ITem number */
	} itemidx;

typedef struct DDOVERFLOW {
	struct DDOVERFLOW	*next,	
						*prev;
	short				flags;
	itemidx				item;
	} ovrflo;

typedef struct ARRAYHDR {
		short	cols;
		short	rows;
		long	sizeperfield;
		long	mallocsize;
	} arrayhdr;

typedef struct APPL0REC {
		int		item_count;
		int		appl_count;
		int		dict_revision;
		long	modtime[DICT_COUNT];
		long	createtime;
	} appl0rec;

typedef struct APPLMAN {
	char			applname[NAME_KEYSIZE + 1];	/* application name			*/
	itemidx			*idx[DICT_COUNT];	/* Index Heads For This Application */
	unsigned short	cnt[DICT_COUNT];	/* Item Counts This Application 	*/
	ovrflo			*overflow[DICT_COUNT];	/* Items added this session		*/
	byte			applstat;			/* application status byte			*/
	} applman;

typedef struct DD_NOTE {
	short			note_len;				// Length of Note text
	short			path_len;				// Length of PATH to source file
	short			note3_len;				// Length of Note 3
	} dd_note;

#pragma pack (push, 2)
typedef struct SYSCFIG {
	char	Projname[MAX_PROJ_NAME + 1];
	char	DD_path[MAX_PATH_NAME + 1];
	} syscfig;
#pragma pack (pop)

/* dataman globals	*/
extern	char		*dict_names[];
extern	void		*bitbuffer;
#define MAX_DDOPENERROR_LEN 512   /* length of DDopenerror */
extern	char		DDopenerror[];
extern	int			lastproj;
extern	byte		lastdict;
extern	byte		lastappl;
extern	void		*lastptr;

#ifndef XPDATA
#define XPDATA extern
#endif
XPDATA	int	DDerrno;
XPDATA	syscfig		config;		/* system configuration vars	*/

/* functions in ddmaint.c */
index	ddput(int, byte, byte, void *, const char *, byte);
bool	dddel(int, byte, byte, index);
index	ddindex(int, byte, byte, const char *);
char	*ddname(int, byte, byte, index);
int		csr_lookup(const char *);
byte	ddaddappl(int, const char *);
char	*dddictname(byte);
byte	dddictindex(const char *);
int		legalname(const char *);
bool	ddidentical(int, byte, void *, void *, byte, byte);
bool	ddputstat(int, byte, byte, index, byte);
byte	ddgetstat(int, byte, byte, index);
bool	ddrename(int, byte, byte, index, const char *);
itemidx	*ddgetitemidx(int, byte, void *);
int		ddapplcount(int);
long	ddarraysize(int thisproj, byte applno, index itemno);
long	ddgetrowcol(int thisproj, byte applno, index itemno, void *objptr);

void	*ddget(int, byte, byte, index);
void	*ddaget(int, byte, index, short, short);
bool	ddgetrec(int, byte, long, void *, int);
void	*ddgetbyname(int, byte, byte, const char *);
int		ddgetlist(int, byte, byte, int, const char*, char***);
bool	btreecountitems(int, int *, int *);
byte	*flddupptr(register field *);
int	fldobjcount(field *);
int	fldstosize(register field *);

/* cfgrun.c functions */
bool	cfig_load(char *, syscfig *);
int		get_moakeyvalue(char *key, char *buf, int len);
char	*get_moadir(void);
char	*get_moaproj(void);
char	*get_moacurproj(void);

/* Functions In fmxxx.c */
bool	btreeopen(int, bool);
bool	btreeclose(int);
unsigned short	btreebldindex(int, byte, byte, itemidx *);
bool	btreeinsertitem(int, byte, itemidx *, char *, byte);
long	btreeinsert(int, byte, byte, index, char *, void *, unsigned short, byte);
bool	btreedeleteitem(int, byte, itemidx *);
bool	btreedelete(int, byte, byte, index);
unsigned short	btreegetbyindex(int, byte, byte, index, void *, unsigned short);
char	*btreeitemname(int, byte, byte, index);
bool	btreequickndx(int, itemidx *, int);
bool	btreeputstat(int, byte, byte, index, byte);
byte	btreegetstat(int, byte, byte, index);
bool	btreerename(int, byte, byte, index, char *);
int	fmread(int, char *, unsigned);

char *ddpath(int);

/* applman.c functions */
byte	ddapplindex(int, const char *);
char	*ddapplname(int, byte);

/* dataman.c functions */
int	ddopen(char *, int);
int	ddopenp(char *, int, char *);
bool	ddclose(int);
long	ddlastupdate(int, byte);
long	ddhashID(int ddhandle);
bool	dictconfig(int);
bool	dictbldindex(int);
void	dict_release(int);
void	dict_release_item(int, byte, itemidx *);
bool	ddreload(int);
void	dd_inc_update(void);
void	dd_dec_update(void);
void	ddsetsignal(void);
long	ddcreatetime(int);
void	DDspillguts(void);
void	*ddgetdd(int);
void 	dd_cleanup(int thisproj);

/* functions in fmindex.c	*/
index	btreeitemindex(int, byte, byte, char *);
long	btreedaddr(int, byte, byte, index);

/* function in ddobjcnt.c */
int ddobjcnt(int thisproj, byte dictno, byte applno);

/* functions in ddread.c */
itemidx *ddclosestobj(int thisproj, byte dictno, void *ptr);
char *ddininame(char *project);
void *load_refs(int, byte, void *, int);

/* function in ddbytrev.c	*/
void *convert_ints(char *nw, byte dictno, ushort maxlen, \
	void *itdata, byte applno, int mode);

/*  in FMINDEX.C */
index	btreeitemindex(int, byte, byte, char *);
long	btreedaddr(int, byte, byte, index);

/* Function in DDOBJCNT.C  */
int 	ddobjcnt(int thisproj, byte dictno, byte applno);

/* Functions in DDREAD.C */
itemidx	*ddclosestobj(int thisproj, byte dictno, void *ptr);
char 	*ddininame(char *project);
void 	*load_refs(int, byte, void *, int);

/* Function in DDBYTREV.C */
void *convert_ints(char *nw, byte dictno, ushort maxlen, void *itdata,
					byte applno, int mode);

/* Function in DDLOCALS.C */
field *local_get_by_name(int local_idx, char *name);

/* Function in DDfldTYP.C */
const char *ddfieldtypeName(int field_type);
#endif

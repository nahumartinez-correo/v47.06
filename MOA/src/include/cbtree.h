/*  cbtree.h      v 3.00   last mod 05/01/89       */

#ifndef CBTREE_H      /* prevent multiple inclusion */
#define CBTREE_H 1

#define MIN_CBT

#ifndef NULL
#define NULL  ((void *)0)
#endif

#ifndef NUL
#define NUL  '\0'            /* define NUL character */
#endif

#include <cbtrdef.h>

    /******************************************************************/
   /*                                                               */
  /* recompile cbtree.c and application pgm if #defines are changed */
 /*                                                                */
/******************************************************************/

/* #define CBVER 0x0304 * CBTREE version 304, initiated 07/24/90 */
/* #define CBVER 0x0305 * CBTREE version 305, initiated 05/12/91 Satyabodhi */
#define CBVER 0x0306 /* CBTREE version 306, initiated 11/19/91 Satyabodhi */
                          /*   version 306 has bytecount in bthdr */
#define CBTREE_CORRUPTED (-32676)

/* for fixed-length records: */
#ifdef WIN32
#else
# define btseek(fd, btloc, reclen)  lseek(fd, ((btloc) - 1L) * (reclen), 0)
#endif
/* compatible form for variable length records: reclen can be a dummy */
#ifdef WIN32
# define btseekv(fd, btloc, reclen) _lseek(fd, (long)(btloc), 0)
# define btseek(fd, btloc, reclen)  _lseek(fd, ((btloc) - 1L) * (reclen), 0)
# define btwrite(fd, buffer, count) _write(fd, buffer, count)
# define btread(fd, buffer, count)  _read(fd, buffer, count)
# define btclose(fd) _close(fd)
#if 0
# define btseek(fd, btloc, reclen)  SetFilePointer((HANDLE)fd, ((btloc) - 1L) * (reclen), NULL, FILE_BEGIN)
int btwrite(int fd, const void *buffer, int count);
int btread(int fd, void* buffer, int count);
int btclose(int fd);
#endif
#else
# define btseek(fd, btloc, reclen)  lseek(fd, ((btloc) - 1L) * (reclen), 0)
# define btseekv(fd, btloc, reclen) lseek(fd, (long)(btloc), 0)
# define btwrite(fd, buffer, count) write(fd, buffer, count)
# define btread(fd, buffer, count) read(fd, buffer, count)
# define btclose(fd) close(fd)
#endif
#define HDRTRYS   4  /* number of times to try to lock header record */

/* MAXBTREE is NOT a cbtree limitation.  btsetup.c and btrinit.c use this  */
/* value to process the btparms.btr file.  Feel free to increase MAXBTREE. */

#if AmigaDOS
#define MAXBTREE   18   /* up to MAXBTREE B+trees    */
#endif
#if (MSDOS | WIN32)
#define MAXBTREE   20
#endif
#if (XENIX | UNIX | DNIX)
#define MAXBTREE   19
#endif

#define MAXFNAME 32 /* Maximun # chars in index file name including PATH.  */
					/* This can be increased as needed. Increasing MAXFNAME*/
					/* will increase the amount of memory required by each */
					/* btcommo structure. */

#define MAXCACHE  0 /* upto 0 cached index blocks. increase if more needed*/

#define LINWIDTH   82   /* max line length for btparms.btr file */
#define DELIMITER '^'   /* in BTPARMS file */

#ifndef ERR
#define ERR -1
#endif

#define repeat   do
#define until(x) while(!(x))
#if (DNIX | UNIX)
//#	ifndef _SYS_TYPES_H
//#		include <sys/types.h>
//#	endif
#endif

#define SZCHAR   sizeof(char)

#ifndef MIN
#define MIN(i, j) ((i) < (j) ? i : j)
#endif

#ifndef MAX
#define MAX(i, j) ((i) < (j) ? j : i)
#endif
#define GETREC   1
#define GETNXT   2
#define GETPRV   3
#define GETALL   4
#define INSERT   5
#define CB_DELETE   6
#define DELTKY   7
#define ISRTKY   8
#define GETSEQ   9
#define GETFRST   10
#define GETLAST   11
#define GETPAR   12
#define GETKEYS   13
#define NEWLOC   14
#define GETGT      15
#define GETGE      16
#define GETLT      17
#define GETLE      18
#define INSERTU  19
#define ISRTKYU  20
#define GETNBLK  21
#define GETPBLK  22

#define BTCALLOK   0
#define BTERROR   -1
#define BTNOTFND   -2
#define BTCOMMO   -3
#define NOMEM      -4
#define BTCPYBRO   -5
#define BTLOCK   -6
#define BTUNLOCK   -7
#define BTABORT   -9

#define CK_NOMEM     1
#define CK_BTRIN     2
#define CK_OPEN   3
#define CK_SEEK   4
#define CK_READ   5
#define CK_WRIT   6
#define CK_CBTR   7
#define CK_STAK   8
#define CK_PUSH   9
#define CK_INDX  10

#define LOCKR    2   	/* Non-blocking lock */
#define UNLOCKR  0     /* Unlock record */

#if ((XENIX == 0) & (DNIX|UNIX | MS | WIN32 | TC | MX) | (LC & (LC > 299) ))
#	ifndef _STDLIB_H
#		include <stdlib.h>
#	endif

#	ifndef _STRING_H
#		include <string.h>
#	endif

#	if (DNIX)
#		ifndef MEM_ALLOC_H
#			include <memalloc.h>
#		endif
#		ifndef srcCopy	/* if isc/window is not already included */
int StringLen(register char *string);
#		endif
#		define STRLEN(s)	StringLen((char *)s)
#	else
#		define STRLEN(s) strlen(s)
#		ifndef MEM_ALLOC_H
#			include <memalloc.h>
#		endif
#	endif

#else
#	if XENIX == 1
#		include <string.h>
#		include <malloc.h>
#	endif
#endif

#if (XENIX == 0) & (MS | TC | MX || NT)
#include <io.h>
#else 
#	ifdef DNIX
#		ifndef _UNISTD_H
#			include <unistd.h>
#		endif
#	endif
#endif

      /* cbtree structure templates */

/* the following is used by ckerror.c */
#define BTOPTYPE struct _btoptype
BTOPTYPE
{
   char *op_name;
   int   op_value;
};

/* offset to base block for variable length records */
#define BASEBLK   32    /* reserved space at beginning of file */

#define BTBLKHDR struct btblkhdr
struct btblkhdr {
   long freelst;
   long eoflst;
   long	lastrec;   /* displacement of last valid record when variable length*/
   long rec_count;	/* record count for variable length .dat files */
   long bytecount;	/* byte count for variable length CDS .dat files */
};
#define SZBLKHDR sizeof(BTBLKHDR)

#pragma pack (push, 2)

#define BT_BASE struct _bt_base    /* first 20 bytes after BTBLKHDR */
#define SZ_BT_BASE     (BASEBLK-SZBLKHDR)
BT_BASE {
   uword   bb_cbver;      /* CBTREE version number */
   sword   bb_negver;     /* negative CBTREE version number */
   uword   bb_numbtree;   /* # B+tree slots already in this file */
   uword   bb_szinfo;     /* size of each info block */
   uword   bb_blksz;      /* index block size */
   uword   bb_varlen;     /* if variable length records are used */
   /* char    bb_fill[SZ_BT_BASE-2-2-2-2-2-2];*/
};

#define BT_INFO     struct _bt_info   /* B+tree parameters */
#define SZ_BT_INFO  32
#define SZ_BNAME     8
BT_INFO
{
   long  bi_rootp;      /* root for this tree */
   char  bi_bname[SZ_BNAME];     /* btree name */
   uword bi_blksz;      /* length of index block parameters */
   uword bi_kylen;      /* key length (w/o NUL) */
   uword bi_cells;      /* cells per index node */
   uword bi_varlen;     /* variable length record flag */
   char  bi_fill[SZ_BT_INFO-SZ_BNAME-4-2-2-2-2];
};

#define BT_LINK     struct _bt_link
BT_LINK
{
   long  bl_link;
   char  bl_tag[SZ_BNAME];   /* make it look like BT_INFO */
   uword bl_blksz;           /* make it look like BT_INFO */
   uword bl_more;           /* # BT_INFO blocks that will fit here */
   uword bl_total;           /* # BT_INFO blocks per index block */
};
#define SZ_BT_LINK  sizeof(BT_LINK)

#define BTIDX struct btidx
struct btidx {
   long   btptr;     /* record location */
   char  *skeynme;   /* key */
};
#define SZBTIDX sizeof(BTIDX)

#define BTIDXBLK struct btidxblk
struct btidxblk {
   uword blkalloc;    /* count of cells in block w/ RAM allocated */
   uword blktype;     /* block type... 1 = data; 0 = idx         */
   uword cellicnt;    /* count of cells in use                 */
   long  fwdpage;     /* ptr to next ordered page; (blktype = 1) */
   long  bwdpage;     /* ptr to prev. ordered page; (blktype = 1) */
   BTIDX btpage[1];   /* idx node with # of cells + 1 NULL. */
};
#define SZIDXBLK sizeof(BTIDXBLK)

#define BTC struct btcommo
struct btcommo {
#ifndef MIN_CBT
   char   *dtaname;      /* Data File Name */
#endif
   uword   btdtalen;     /* Data File Record Length */
   uword   btvarlen;     /* variable-length record mode */
   uword   btmulti;      /* muti-user mode (record locking) */
#ifndef MIN_CBT
   uword   btcrash;      /* crash-protection mode */
   BTBLKHDR freeptrs;   /* freelst and end-of-file pointers */
#endif
   long    findroot;     /* where to find the root block pointer */
   long    btroot;       /* record offset for root block */
   char   *idxname;      /* Index File Name */
   uword   btidxlen;     /* Index File Record Length */
   uword   btkeylen;     /* max strlen(key) */
   uword   btcells;      /* max # Cells per Index Block */
   char   *btname;       /* btree name */
   uword   btoptype;     /* CBTREE operation type */
   char   *btkey;        /* search key for operation */
   long    btloc;        /* data file record location */
#ifndef MIN_CBT
   uword   btoptyp2;     /* CBTREE secondary operation */
   char   *btkey2;       /* secondary key */
   long    btloc2;       /* secondary operation */
   uword   btmax2;       /* maximum size of btrecnm2 array */
   long   *btrecnm2;     /* for logical operations on record ranges */
   uword   btsave;       /* save key is in use  */
   long   *locklist;     /* tree traversal list */
   uword   lockptr;      /* locklist index */
   char *(*btmakeky)();  /* function to build the key */
#endif
   int  (*btcdslog)(void *ccb, unsigned char action, void *context, char *buf, int luw);
		 /* function for recovery logging (pre-images) */
   void	*btkeydef;		 /* pointer to key segments definition array */
   int   (*btxcmpky)(const char *, const char *, const void *);  /* function to do exact key comparisons */
   int   (*btpcmpky)(const char *, const char *, int, const void *);  /* function to do partial key comparisons */
   void  (*btsethi)(char *key, void *btkeydef);
   void  (*btsetlow)(char *key, void *btkeydef);
   uword   btmax;        /* maximum size of btrecnum array */
   long   *btrecnum;     /* array of record numbers     */
   char   *btsvkey;      /* save key for use with GETALL, et. al. */
   short btkeytyp;       /* key types: 0 = char; 1 = long; 2 = float; */
   short bt_no_reclaim;	 /* 1 if true, default = 0 */
   long bthival;         /* highest value for binary key */
   uword btvcount;       /* count of cache index blocks to allocate */
   long *btvptrs;        /* record pointers for cached index blocks */
   void *user_ccb;		/* pointer to user's connection control block - used in cdsv.exe */
#if (MAXCACHE > 0)
   BTIDXBLK *vidxblk[MAXCACHE]; /* up to MAXCACHE cached index blocks. */
#endif
};

/* The following added to support btrinit1 function call */

typedef struct BTPARMS {
	BTC *btcp;         				/* btcommo structure tag. */
	char idxfname[16];				/* index file name. */
	char idxname[16];				/* index btree name. */
	unsigned ilen;  				/* index block size. */
	unsigned klen;					/* index maximum key size. */
	unsigned dlen;					/* data file record size, 0 for varible. */
	unsigned szmax;					/* # of cells to calloc for block calls. */
	} *btparms;

/*lint +e514 */
#define SZ_UB  12   /* sizeof(USE_BLOCK) */

#pragma pack (pop)

/* function declarations */
#include <cbproto.h>	/* cbtree prototype declarations. */
                      	/* Xenix, EC and DC: no args in function prototypes. */

#endif /*CBTREE_H*/


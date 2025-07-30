/* modify compiler type flag to match specific compiler */

#ifndef CBTRDEF_H   /* prevent multiple inclusion */
#define CBTRDEF_H 1

#ifndef THIS_SYS_H
#	include <this_sys.h>
#endif
#ifdef LITTLE_ENDIAN
#	define CANNONICAL_INT
#endif

  /*******************************/
 /* Step One: Choose a compiler */
/*******************************/
#ifndef NT
#   define NT  1
#endif
#define MS     0     /* Microsoft C                   */
#define MSVER  510

#define TC     0     /* Borland's TURBO C             */
#define TCVER  200

#define MX     0     /* Aztec C                       */
#define MXVER  410

#define CI     0     /* Computer Innovations C86      */
#define CIVER  230

#define DC     0     /* Desmet C                      */
#define DCVER  250

#define LC     0     /* Lattice C & Datalight C       */
#define LCVER  300   /* version number */

#define DL     0     /* Datalight C       */
                     /* Must also define LC 1 & LCVER 215 above!!! */

#define MW     0     /* Mark Williams C               */
#define MWVER  311

#define EC     0     /* Ecosoft C                     */
#define ECVER  201

#define DCC    0     /* Diab C                        */
#define DCCVER 235	/* version 2.35bX */

#if ((TC + LC + MS + MX + MW + CI + DC + EC + DCC + NT) != 1)
cbtrdef_1_error;      /* will generate an error */
#endif

  /****************************************/
 /* Step Two: Choose an Operating System */
/****************************************/
#ifndef MSDOS           /* Lattice pre-defines this */
#define MSDOS    0
#endif
#define AmigaDOS 0
#define XENIX    0      /* Unix, AIX, etc */
#ifndef CDS_MACRO_H
#	include <cdsmacro.h>  /* for ntoh and hton macros */
#endif
#ifndef UNIX
#	define UNIX     0      /* UNIX, SVR4 */
#endif
#ifndef DNIX
#	define DNIX     0      /* Dnix */
#endif
#define ALLOCA   1		/* function alloca() is available */


#define CDSKEY   9		/* key type for CDS composite keys that aren't ASCII */

#if ((MSDOS + AmigaDOS + XENIX + DNIX +UNIX + NT) != 1)
cbtrdef_2_error;   /* select only ONE operating system */
#endif

  /**********************************/
 /* Step Three: Choose a processor */
/**********************************/
#define I808x   0   /* Intel 8086 286 family */
#define I386x   1   /* Intel 386 family */
#define M680x0  0   /* Motorola 68000 family */

#if ((I808x + I386x + M680x0) != 1)
cbtrdef_3_error;    /* select only ONE processor */
#endif

      /*******************************************************************/
     /*                                                                 */
    /* The following should NOT be modified by application programmer. */
   /* Please notify Peacock Systems if compiler incompatibility is    */
  /* discovered.  Phone (703) 847 - 1743.                            */
 /*                                                                 */
/*******************************************************************/

/* prototypes with parameters. */
/*lint -e514 */
#if ((XENIX == 0) & ((MS | NT | TC | LC) & (LCVER > 299)) | DNIX |UNIX) 
#define _CBPROTO 1
#else					/* prototypes without parameters. */
#define _CBPROTO 0
#endif

#if (MSDOS & ((LC & (LCVER < 300)) | CI))
#define void int
#define short 
#endif


#define LCAST(p)  (long ) *((long *)p)  /* cast char pointer into long int */

#if I808x
#define LOHI    1    /* low-byte high-byte order */
#define NOALIGN 1    /* no alignment is required */
#endif

#if I386x			/* 32 bit processors */
#define LOHI    1
#define NOALIGN 0
#endif

#if M680x0			/* 32 bit processors */
#define LOHI    0
#define NOALIGN 0
#endif

#if (I386x | M680x0)
#define uword unsigned short
#define sword short int
#else
#define uword unsigned short
#define sword short int
#endif

#if CI
#define atol atoi
#endif

#define FREE(p)  cb_free(p),(p) = NULL   /* all malloc is thru cb_malloc() */

#if AmigaDOS
#define PRINTER "PRT:"
#endif
#if MSDOS
#define PRINTER "prn"
#endif
#if (XENIX | UNIX)
#define PRINTER "/dev/lp"
#endif

#if AmigaDOS
#define scr_clr() printf("\14");
#endif
#if MSDOS
#define scr_clr() printf("\33[2J");
#endif
#if XENIX
#define scr_clr() printf("\033H\033J");
/* #define scr_clr() printf("\033*"); */
#endif
#if DNIX
#define scr_clr() printf("\033[;H\033[2J");
#define scr_curs(y,x)   printf("\033[%d;%dH", (y)+1, (x)+1)   /* mv cur */
#endif

#if XENIX
#define scr_curs(r,c)   printf("\033&a%dr%dC", (r)+1, (c)+1) /* move cursor */
/* #define scr_curs(y,x)   printf("\33=%c%c", (y)+' ', (x)+' ') */  /* mv cur */ 
#endif
#if MSDOS | AmigaDOS | NT
#define scr_curs(r,c)   printf("\33[%d;%dH", (r)+1, (c)+1) /* move cursor */
#endif

#if AmigaDOS
#define PRE_EXTEND 0    /* normally set to 0 */
                        /* set to 1 if you want your data records and */
                        /* index tree in the SAME file. */
#endif

#if AmigaDOS
#if LC
#define cbwopen(s) {int _s_f;freopen(s,"r+",stdout);_s_f=stdin->_file;stdin->_file=stdout->_file
#define cbwclose() printf("\n  Press <RETURN> ... ");getchar();freopen("*","w",stdout);stdin->_file=_s_f;}
#endif
/* DEH 10/20/87 */
#if MX
#define cbwopen(s) {int _s_f;freopen(s,"r+",stdout);_s_f=stdin->_unit;stdin->_unit=stdout->_unit
#define cbwclose() printf("\n  Press <RETURN> ... ");getchar();freopen("*","w",stdout);stdin->_unit=_s_f;}
#endif
#else
#define cbwopen(s)
#define cbwclose()
#endif

#define CBTREE_BAD_KEYTYP 1859
#define CBTREE_IDX_CORRUPT 1860
#define CBTREE_BAD_OPCODE 1861
#define CBTREE_BASE_CORRUPT 1862
#define CBTREE_DELETED_ALREADY 1863
#define CBTREE_UB_SEEK 1864
#define CBTREE_UB_READ 1865
#define CBTREE_UB_CIRCLE 1866
#define CBTREE_REMOVING_CORRUPT_FILE 1867
#define CBTREE_DELTREE_SEEK 1868
#define CBTREE_BAD_SPLIT_LOG 5867
#define CBTREE_BAD_MERGE_LOG 5868

#endif /* CBTRDEF_H */

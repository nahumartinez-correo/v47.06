#if _CBPROTO
#	ifndef REGP 
#		define REGP register
#	endif
#define PASS_THRU 0
#if (PASS_THRU == 0)
#ifndef size_t
#	include <malloc.h>
#endif
void *cb_calloc(REGP size_t, REGP  size_t);
void *cb_malloc(REGP  size_t size);
void cb_free(REGP  void *);
#else
#	ifdef CKREGPAR
#		define cb_calloc(a,b) calloc(a,b)
#		define cb_malloc(a) malloc(a)
#		define cb_free(p) free(p)
#	else
#		define cb_calloc(a,b) mem_calloc(a,b)
#		define cb_malloc(a) mem_alloc(a)
#		define cb_free(p) mem_free(p)
#		define CKREGPAR	/* for ckerror parameters */
#	endif
#endif
#	ifndef CKREGPAR 
#		define CKREGPAR	/* for ckerror parameters */
#	endif

#define XREGP	/* for external register variables */

int alloc(int fd, long size_in_bytes);
uword btcellspi(struct btcommo *btcp);	/* return cells per index */
int btcheck(char *name, int fd);   /* verify file's CBTREE version number */
void adjstack(REGP int, REGP struct btidxblk *, REGP long, REGP  struct btcommo *);
void blkshft(REGP char *,REGP char *,REGP unsigned short,REGP unsigned short);
int binarysr(REGP struct btidxblk *, REGP char *, REGP struct btcommo *);
int binarydt(int,REGP struct btidxblk *, REGP struct btcommo *,unsigned int,unsigned int *,unsigned int *);
int binarydu(REGP struct btidxblk *, REGP struct btcommo *,unsigned int *);
int binvalsr(REGP struct btidxblk *, REGP char *);
int binvaldt(int,struct btidxblk *,struct btcommo *,unsigned int *,unsigned int *);
int binvaldu(REGP struct btidxblk *,REGP struct btcommo *,unsigned int *);
void insertky(REGP struct btidxblk *,int,char *,unsigned short int,int);
void spltnode(REGP struct btidxblk *,REGP struct btidxblk *,unsigned short int);
int bt_open(XREGP const char *,int,int);
int btrdelet(int,int,REGP struct btidxblk *, REGP int, long, REGP struct btcommo *);
int cmptnode(REGP int,REGP int,REGP struct btidxblk *,REGP struct btcommo *);
int btrgets(REGP int,REGP struct btidxblk *,int,uword, unsigned int *,struct btcommo *);
int btrinit(char *,struct btcommo *);
int   btrinit1(btparms btpptr);		/* created by ISC-BR */
int btrisrt(REGP int,int,struct btidxblk *,int,long,long,REGP struct btcommo *);
long btgivvar(BTC *btcp, REGP int fd);
uword btdelvar(BTC *btcp, REGP int,REGP long);
long btnxtvar(BTC *btcp, REGP int,REGP long,REGP long);
long btnxtrecv(int,long,long);
/* unsigned short int btsyssiz(REGP int,REGP long);*/
unsigned short btsiz_var(BTC *btcp, int fd, long offset); /* gets sizeof record */
unsigned short int btsizvar(XREGP int,XREGP long);
void writzeros(REGP int, REGP long, REGP unsigned short int);
int btcinit(REGP struct btcommo *,char *,char *,unsigned int,unsigned int,unsigned int,unsigned int);
void btrterm(struct btcommo *);
int cbtree( int, int,  struct btcommo *);
void ckerror(CKREGPAR int, CKREGPAR char *);
void cberror(int,char *,struct btcommo *);
int ckbtree(struct btidxblk *, struct btcommo *, int);
int cpyltbro(REGP int,REGP struct btidxblk *,REGP struct btcommo *);
int cpyrtbro(REGP int,REGP struct btidxblk *,REGP struct btcommo *);
void creatbtr(struct btcommo *);
void vcreatbtr(struct btcommo *btcp, unsigned size);
void delaycb(REGP long);
int deletbtr(int,long,struct btcommo *);
void delnode(REGP int,long,unsigned short int,unsigned short int,unsigned short int,REGP struct btcommo *);
void deleteky(REGP struct btidxblk *,REGP int);
int deletrec(REGP int, REGP long, unsigned short int);
char *fdgets(REGP char *,REGP int,REGP int);
int fdgetc(REGP int);
long findroot(REGP int,REGP char *);
void freekeys(REGP struct btidxblk *);
void getbase(REGP struct _bt_base *,REGP int);
long getroot(REGP int, REGP long);
void getidxr(REGP int, REGP long, REGP struct btidxblk *, REGP struct btcommo *);
void addvidx(REGP struct btidxblk *,REGP long,REGP struct btcommo *);
void mvidxblk(REGP struct btidxblk *,REGP struct btidxblk *,REGP unsigned short int,int);
void updvidxp(long,long,int, REGP struct btcommo *);
void vidxdel(REGP long, REGP struct btcommo *);
char *strsave(REGP char *,REGP unsigned short int,REGP int);
void getfhdr(REGP struct btblkhdr *,REGP int);
long btlastvrec(int fd);	/* returns size of user's last record in file */
void getinfo(REGP struct _bt_info *,REGP int,REGP long);
long giveidxr(REGP int,REGP unsigned short int,REGP int,REGP int);
void initdat(XREGP int,XREGP struct btcommo *);
void remove_corrupt_file(BTC *btcp, char *key);

/*lint +fvr
  The following functions exhibit variable return modes.
 */
int lockunlk(REGP int, REGP long, unsigned short int, REGP int);
void putroot(REGP int,long,REGP long);
void putidxr(int,long,REGP struct btidxblk *,REGP struct btcommo *);
char *keyadd(REGP char *, REGP char *);
void putfhdr(REGP int,REGP struct btblkhdr *);
long pushdown(REGP long);
long poptop(void );
void relstack(void );
void strnncpy(REGP char *,REGP char *,REGP unsigned short int);
int updilvl(REGP int,long,REGP char *,REGP struct btcommo *);
int mtch_loc(REGP int,REGP struct btidxblk *,int,REGP struct btcommo *);
int wrap_hdr(REGP int,REGP struct btidxblk *,int,REGP struct btcommo *, long *);
void free_svkey(XREGP struct btcommo *);
void free_gtkey(XREGP struct btcommo *);
int cbgetblk(REGP int, REGP struct btcommo *);
int btrgtnxt(REGP int, REGP struct btidxblk *,int, REGP struct btcommo *);
int btrgtprv(REGP int, REGP struct btidxblk *,int, REGP struct btcommo *);
void fasttree(long,char *,int,struct btidxblk *,long *,REGP struct btcommo *);
void copyleft(int,REGP struct btidxblk *,REGP struct btcommo *,unsigned int);
void uplvlbld(int,REGP struct btidxblk *,REGP struct btcommo *,unsigned int);
int cpystr(REGP char *,REGP char *);
unsigned short int strnlcpy(REGP char *,REGP char *,REGP uword);
int cacheld(REGP int,REGP struct btcommo *);
int cklvlptr(REGP int,REGP struct btidxblk *,REGP struct btcommo *);
void btstrcpy(REGP char *,REGP char *,REGP int);
int btstrcmp(REGP char *,REGP char *,REGP int);
int btstrncm(REGP char *,REGP char *,REGP uword, REGP struct btcommo *);
#ifdef SZ_PGSTRUCT
int pageidx(REGP int fdi, REGP struct btcommo *btc, REGP struct pgstruct *pgvar);
#endif
#else
extern  void adjstack();
extern  void blkshft();
extern  int binarysr();
extern  int binarydt();
extern  int binarydu();
extern  int binvalsr();
extern  int binvaldt();
extern  int binvaldu();
extern  void insertky();
extern  void spltnode();
extern  int bt_open();
extern  int btrdelet();
extern  int cmptnode();
extern  int btrgets();
extern  int btrinit();
extern  int btrisrt();
extern  long btgivvar();
extern  void btdelvar();
extern  long btnxtvar();
extern  unsigned short int btsyssiz();
extern  unsigned short int btsizvar();
extern  void writzeros();
extern  int btcinit();
extern  void btrterm();
extern  int cbtree();
extern  void ckerror();
extern  void cberror();
extern  int ckbtree();
extern  int cpyltbro();
extern  int cpyrtbro();
extern  void creatbtr();
extern  int newindex();
void delaycb();
extern  int deletbtr();
extern  void delnode();
extern  void deleteky();
extern  int deletrec();
extern  char *fdgets();
extern  int fdgetc();
extern  long findroot();
extern  void freekeys();
extern  void getbase();
extern  long getroot();
extern  void getidxr();
extern  void addvidx();
extern  void mvidxblk();
extern  void updvidxp();
extern  void vidxdel();
extern  char *strsave();
extern  void getfhdr();
extern  void getinfo();
extern  long giveidxr();
extern  void initdat();
extern  int lockunlk();
extern  void putroot();
extern  void putidxr();
extern  char *keyadd();
extern  void putfhdr();
extern  long pushdown();
extern  long poptop();
extern  void relstack();
extern  char *strnncpy();
extern  int updilvl();
extern  int mtch_loc();
extern  int wrap_hdr();
extern  void free_svkey();
extern  void free_gtkey();
extern  int cbgetblk();
extern  int btrgtnxt();
extern  int btrgtprv();
extern  void fasttree();
extern  void copyleft();
extern  void uplvlbld();
extern  int cpystr();
extern  unsigned short int strnlcpy();
extern  int cacheld();
extern  int cklvlptr();
extern  void btstrcpy();
extern  int btstrcmp();
extern  int btstrncm();
extern int pageidx();
#endif


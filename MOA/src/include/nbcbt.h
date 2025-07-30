
#ifndef _NBCBT_H
#define _NBCBT_H

#ifndef MAXPATH
#define MAXPATH 256
#endif

#define NBCB_KEYLNG KEYDATA_L  /*  Size of cbtree key */

struct NBcntxt
	{
	struct NBcntxt *next_context;	/* link for free list */
	CSS_NBS_DBREQ *out_rec;			/*  record returned from first/next */
	CSS_NBS_DBREQ *in_rec;			/*  ??? */
	short	mkey_length;
	short	mdata_length;
	long	rloc;
	char	part_key[KEYDATA_L + 1];
	char	*match[NBCB_KEYCOUNT + 1]; /* pointers to match criteria */
	char	match_type;				/*  set to 2 when regular expression */
									/*  should be used, set to 1 when  */
									/*  partial key should be used, 0 */
									/*  if match exact */
	};

typedef struct NBcntxt NBcntxt;

/*    Following arguments used at cbtree initialization  */
#define LEAVE_DATA 0			/* Open "as is" */
#define CLEAR_DATA 1			/* Clear existing data before initializing */
#define CREATE_DB  2			/* Create files (error if they exist)  */


#ifdef GLOBALS_BELONG_HERE
#define	GLOBAL	/* nothing */
#define INIT(v) = (v)
#else
#define GLOBAL extern
#define INIT(v)	/* nothing */
#endif

GLOBAL  struct btcommo nb_btcommo;

int initnbcbt(register char *dirpath, register int mode);
int nbcbt(server *sp, register CSS_NBS_DBREQ *nb_buf, register CSS_NBS_DBREQ *return_buf);

int NBcbinit(register int mode, register char *dname, register char *fname );
int NBcbinsert(register CSS_NBS_DBREQ *nb_buf, char **ptr);
int NBcbdelete(register CSS_NBS_DBREQ *nb_buf, char **ptr);
int NBcbfirst(register NBcntxt *ctx);
int NBcbnext(register NBcntxt *ctx);
#endif

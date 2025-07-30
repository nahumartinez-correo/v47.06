/* Miscellaneous defines for compile and execution modules */

#define MAXTAG	10
#define MAXDFA  1024
 
#define OKP     1
#define NOP     0
 
#define CHR     1
#define ANY     2
#define CCL     3
#define NCL     4
#define BOL     5
#define EOL     6
#define BOT     7
#define EOT     8
#define BOW		9
#define EOW		10
#define REF     11
#define CLO     12
/* #define BIN     13 does not seem to be used */
#define BIT		14
#define REPEAT	15		/* REPEAT PATTERN ON '(R)*'  */ 
#define END     0
 
/*
 * The following defines are not meant
 * to be changeable. They are for readibility
 * only.
 *
*/
#define MAXCHR	256
#define CHRBIT	8
#define BITBLK	MAXCHR/CHRBIT
#define BLKIND	0370
#define BITIND	07
 
/*
 * skip values for CLO XXX to skip past the closure
 *
*/
#define ANYSKIP	2 	/* CLO ANY END ...	   */
#define CHRSKIP	3	/* CLO CHR chr END ...	   */
#define CCLSKIP BITBLK + 2	/* CLO CCL BITBLK bytes END ... */

#define isinset(x,y) 	((x)[((y)&BLKIND)>>3] & (1<<((y)&BITIND)))

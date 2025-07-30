/*
** The following define the various opcodes and typecodes used by the
** "wclist" CSR for use by programmers writing compiled list edit
** procedures.
*/

/* csr:wclist opcodes */
#define WORKINIT 1
#define ELGET 2
#define ELUPDATE 3
#define MEMDELETE 4
#define LISTLOAD 5
#define LISTSAVE 6
#define TEXTEDIT 7

/* csr:wclist type codes */
#define DICTOBJ 1			/* any dictionary object reference */
#define STROBJ 2			/* Double quoted string  constant */
#define NUMOBJ 3			/* Fixed or Integer constant */
#define CHAROBJ 4			/* Character constant (single quoted) */

#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H
#ifndef THIS_SYS_H
#	include <this_sys.h>
#endif
#ifndef TYPEDEFS_H
#	include <typedefs.h>
#endif
/* #define REAL_MALLOC */
#ifdef REAL_MALLOC
extern void *malloc(size_t s);
extern void free(void *s);
extern void *realloc(void *s, size_t n);
extern void *calloc(size_t n, size_t e);
#define mem_alloc(n) malloc(n)
#define mem_free(p) free(p)
#define mem_realloc(p,n) realloc(p,n)
#define mem_calloc(n,i) calloc(n,i)

#define memtofree(a)	/* do nothing */
#define mem2free(a) 	/* do nothing */
#define memdofree()		/* do nothing */

#else
XDLL	void	*mem_alloc(unsigned);			/* allocate a buffer		*/
XDLL	bool	mem_free(void *);				/* release allocated buffer	*/
XDLL	void	*mem_realloc(void *, unsigned);	/* reallocate a buffer		*/
XDLL	void	*mem_calloc(unsigned, unsigned);/* clear & allocate a buffer*/
extern	void	mem2free(void **ptr);		/* put in list for memdofree() */
extern 	void	memdofree( void );

#define memtofree(a) mem2free(a)
#endif

/* memory management with optimized realloc	*/
extern	void	*memr_alloc(unsigned);			/* allocate a buffer		*/
extern	bool	memr_free(void *);				/* release allocated buffer	*/
extern	void	*memr_realloc(void *, unsigned);	/* reallocate a buffer		*/
extern	void	*memr_calloc(unsigned, unsigned);/* clear & allocate a buffer*/
#define memclr(dest,nbytes) memset(dest,0,nbytes)

#ifndef __STDC__IS_FALSE
	/* then the prototypes are in memory.h which is included with string.h */
#else
extern	char *memset(void *dest, int fill, int nbytes);	/* fill for length */
extern	void *memcpy(void *dest, const void *src, int nbytes); /* copy for length */
	/* memcpy is in libc.a */
#endif

#ifdef MDEBUG
extern char *memr_check(char *base, char *next);
extern char *Chain_base;
#endif
extern short Do_memr_check;	/* memr_check() exits with out checking if 0 */

#endif

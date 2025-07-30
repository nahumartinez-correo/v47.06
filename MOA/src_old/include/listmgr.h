
#ifndef LISTMGR_H
#define LISTMGR_H

#ifndef CQUE
#include <cque.h>
#endif

#include <typedefs.h>

typedef unsigned long ULONG;

typedef struct LIST list;

#define	GLPOOLSIZE	100
#define GLGROWTH	20

struct LIST {			/* all lists must begin with these three elements */
	list	*next;	
	list	*prev;
	short	flags;			/* INUSE, PROTECTED, etc. */
							/* Used for element size in freelist */
};

/*
** define for list flags
*/
#define GL_INUSE 0x0001

	 XDLL void	*gl_createfree(int element_size);
	 XDLL void	*gl_createfreev(int element_size, int poolsize);
					/* poolsize is number of elements in initial free list */
	 XDLL void	*gl_create(void);
	 XDLL void	*gl_create_head(void *space);
	 XDLL void	gl_free_head(void *glhead,void *glfree);  // use with gl_create_head()
	 XDLL void	*gl_append(void *glhead,void *glfree);
	 XDLL void	*gl_insertbefore(void *glhead,void *glbefore,void *glfree);
	 XDLL void	*gl_insertafter(void *glhead,void *glafter,void *glfree);
	 XDLL void	gl_remove(void *glhead,void *glptr,void *glfree);
	 XDLL void	gl_free(void *glhead,void *glfree);

#define gl_getnext(glhd,glp) (nextq(glhd,glp))
#define gl_getprev(glhd,glp) (prevq(glhd,glp))
#define gl_getfirst(glhd) (nextq(glhd,glhd))
#define gl_getlast(glhd) (lastq(glhd))
#endif


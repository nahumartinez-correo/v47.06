/*
 *	copyright (c) 1989	ISC SYSTEMS CORPORATION
 */

#ifndef  _Hvarargs
#	define  _Hvarargs

#include <varargs.h>

# define  vinit		va_start(argp)
#define vargs va_alist
# define  vdeclare	int 	va_alist
global_var	va_list	argp;
# define  vword()	(va_arg (argp, int))
# define  vlong()	(va_arg (argp, long))
# define  vaddr()	va_arg (argp, char *)
# define  vtaddr()	va_arg (argp, char *)

#endif

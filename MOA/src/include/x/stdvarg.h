/*
 *	copyright (c) 1989	ISC SYSTEMS CORPORATION
 */

#ifndef  _Hvarargs
#	define  _Hvarargs

#ifndef _rhSTDARG
#	include <stdarg.h>
#endif

# define  vinit(parmn)	va_start(argp, parmn)
#define vargs ...
# define  vdeclare	must_fix_for_stdarg 	va_alist
global_var	va_list	argp;
# define  vword()	(va_arg (argp, int))
# define  vlong()	(va_arg (argp, long))
# define  vaddr()	va_arg (argp, char *)
# define  vtaddr()	va_arg (argp, char *)

#endif

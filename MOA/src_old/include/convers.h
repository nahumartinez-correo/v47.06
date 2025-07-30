/* ------------------------------------------------------------------ */
/*  Prototypes for conversion routines found in misc.a(conversion.o)  */
/*  These routines may be used by any C program, but this file        */
/*  *MUST* be #included else the generated C code will not run        */
/*  correctly (although it probably won't get a compile error)        */
/* ------------------------------------------------------------------ */

#ifndef CONVERSION_H
#define CONVERSION_H

#ifndef TYPEDEFS_H
#include <typedefs.h>	/* these #includes are needed here... */
#endif
#ifndef DECMATH_H
#include <decmath.h>
#endif


	XDLL int	field2decwork(register field *from, register decwork *to);
	XDLL int	decwork2field(register decwork *from, register field *to);

	XDLL int	str2decwork(register char *from, register decwork *to);
	XDLL int	decwork2str(register decwork *from, register char *to,
						register short maxlen);
	XDLL int	strn2decwork(register char *from, register short stringlength,
						register decwork *to);

	XDLL int	str2int(register const char *from, register int *to);
	XDLL int	int2str(register int from, register char *to,
						register short maxlen);

	XDLL int	decwork2int(register decwork *from, register int *to);
	XDLL int	int2decwork(register int from, register decwork *to);

	XDLL int	str2field(register char *from, register field *to);
	XDLL int	strn2field(register char *from, register short stringlength,
						register field *to);
	XDLL int	field2str(register field *from, register char *to,
						register short maxlen);

	XDLL int	field2int(register field *from, register int *to);
	XDLL int	int2field(register int from, register field *to);

	XDLL int	float2decwork(double, register decwork *to);
	XDLL int	decwork2float(register decwork *from, register double *to);

	XDLL bool	isASCIIfield(register field *fp);
	XDLL bool	isNUMERICfield(register field *fp);
	XDLL bool	isDISPLAYfield(register field *fp);

	XDLL int	field_datalen(register field *fp);
	XDLL void	clr_fld_storage(register field *fp);
	XDLL void 	remove_digits(decwork *dp,short len,short flag);

	void		dumpdecwork(register decwork *num);

	/* FROM CV.O */
	char		*cvxl(long *lptr, char *x); /* convert string to long int */
                          /* return pointer to past last char used if OK */
                          /* else return NULL */

#endif





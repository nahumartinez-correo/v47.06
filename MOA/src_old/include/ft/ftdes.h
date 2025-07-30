
/*
 *	copyright (c) 1989	ISC Systems Corporation
 */

# ifndef	_Fftdes
#	define	_Fftdes

#ifndef _Hc
#include	<c.h>
#endif
/*
 *	data type of a function argument or return value
 */
#pragma pack (push, 2)

typedef	char	tiny_int;
typedef	struct
{
	byte ftid;		/* the type id */
	byte type;		/* the type of the argument
				   INTVAL  - an integer
				   CHARVAL - a char
				   ADDR    - a pointer
				   STRING  - a string
				   ARRAY   - an array 
				   etc.   */
	tiny_int	elemsize;	/* if the argument is an array, then
				   this is the size of each element
				   - else it is 0 */
}	ftdes;

/*
 *	parameter descriptor for a function argument or return value
 */

typedef struct
{
	tiny_int	io;		/* INPARG - the argument is read only
				   OUTARG - the argument is write only
				   RETURN - it is the return value of the
						function 
				   VOID   - returns nothing
				   NORET  - does not return */
	tiny_int	where;		/* LOCAL - if it is in the client address space 
				   REMOTE - if it belongs to the server
				   NOWHERE - it corresponds to VOID or NORET */
	const ftdes	*ftype;	 /* the data type of the argument/result */

}	fpardes;

/*
 *	function descriptor
 *	these functions constitute the interface with the server
 */

 /* define what is the type of the generic parameter or return value. It will be
    dependent on the implentation and guaranteed to hold all valid
    return data types. It will be used at the server side. */

typedef		addr		fgpar;

typedef	struct
{
	tiny_int	fid;		/* function code */
	tiny_int	nargs;		/* number of parameters */
	char	*fname;		/* function name */
	fgpar	(*func)();	/* the actual function on server side */
	const fpardes	*fpar;		/* an array of arg descriptor structures.
				   the first entry is for the return value
				   the rest are the arguments */
}	fdes;

extern const fdes ftab1[], ftab2[];

# define	FTDESNIL	((ftdes *)0)
# define	FPARDESNIL	((fpardes *)0)
# define	FDESNIL		((fdes *)0)
# define	FGPARNIL	((fgpar)0)

# define	argtype(i)	( f->fpar[i].ftype->type )
# define	argvalue(i)	( f->fpar[i].ftype->elemsize )
# define	argwhere(i)	( f->fpar[i].where )
# define	argio(i)	( f->fpar[i].io )
# define	isresult(i)	(i == 0)
	
#pragma pack (pop)
# endif

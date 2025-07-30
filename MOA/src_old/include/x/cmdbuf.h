
# ifndef	_Fdbufx
#	define	_Fdbufx

#ifdef	CDSVDEBUG
global_proc	void	prbuf (HANDLE, addr buf, const fdes	*ftab, pos fcode, int towhere);

#endif
global_proc	void	proutargs ();

# ifndef	_Fdbg
#	define	_Fdbg

# define	TOSERVER	1
# define	TOCLIENT	0
# define	TOCLIENT_REAL 2	/* for driver */

# endif
# endif

#ifdef CDSVDEBUG
global_proc	void	chkmtype (const char *caller, regpar const fdes *f, regpar pos i, pos type);
global_proc	void	chkmwhere ();
global_proc	void	chkmptr ();
#else
#define chkmtype(a,b,c,d)	/*do naught*/
#define chkmwhere(a,b,c,d)
#define chkmptr(a,b,c,d)
#endif
global_proc	int	arraysize ();
global_proc	void	noprbuf(HANDLE outFH, addr msgbuf, const fdes *ftab, pos function_code,int towhere);
global_proc void print_Contexts(HANDLE outFH);
 

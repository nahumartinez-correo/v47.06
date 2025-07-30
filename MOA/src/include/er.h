// ==========================================================
//   Copyright (c) 1990	ISC-Bunker Ramo
// ==========================================================

#ifndef ER_H
#define ER_H

	enum fatal {EXBUG=25,
				EXFILE,
				EXSPACE,
				EXSYS,
				EXUSAGE,
				EXUSER
				};
	extern const	char Not_Run[];
	extern	int		(**Clean_up)(void);

/* VARARG 1 */
	extern void	erbugn (int n, ... );
	extern void	erspace (const char *fmt, ... );
	extern void	ersysn (int n, ... );

	extern void		erbug (const char *fmt, ... );
	extern void		erfile (const char *fmt, ... );
	extern void		ersys (const char *fmt, ... );
	extern void		eruser (const char *fmt, ... );
	extern void		erusern (int n, ... );
	extern void		erusage (const char *format, ... );
	extern void		must_fix(const char *format, ... );
	extern void		must_fixn(int error_code_N, ... );
	extern void		must_stop(int error_code_N, ... );
	extern void		must_fixe(const char *format, ... );
	extern void		must_fixen(int error_code_N, ... );
	extern void		eraction (register enum fatal code);

#endif

/****************************************************************************
 *
 *	BT_FIO.H  --  header for btree file I/O functions
 */

/* This file should be included AFTER "cbtrdef.h" for EC compatibility */

#ifndef BT_FIO_H
#define BT_FIO_H 1

/* see also btseek() macro in cbtree.h */
#define bt_read( fd, buf, sz) read( fd, buf, sz)	/* read routine */
#define bt_write(fd, buf, sz) write(fd, buf, sz)	/* write routine */
#define bt_close(fd)			close(fd)				/* close routine */

/****************************************************************************
 *
 *   the following bt_open() mode flags ONLY are used by CBTREE:
 *
 *   O_RDONLY   file is opened for reading ONLY,
 *   O_RDWR     file is opened for reading and writing,
 *   O_RAW	file is opened in binary (CR/LF untranslated) mode,
 *   O_CREAT	only if file does not exist: create file
 *   O_NEW	make new: if file does exist, truncate; else create.
 *
 *	O_CREAT and O_NEW, if not directly supportable by compiler,
 *		should be a non-zero value which does not conflict with
 *		other open mode flags so that it can be detected by
 *		bt_open())
 *
 *   with O_NEW, the following protection mode must be specified.
 *   S_IRDWR  file is available for reading and writing
 *
 */

#ifdef NT
#	define	S_IRDWR		(S_IREAD | S_IWRITE)
#	define	O_NEW		(O_CREAT|O_TRUNC)
#endif

#if 0
#if TC
#include <fcntl.h>
//#include <stat.h>		/* this is found in sys subdirectory and requires */
								/* an additional -i\include\sys\ compilation flag */
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
#define	O_RAW		O_BINARY
/*	O_CREAT		is already defined */
#define O_NEW		(O_CREAT|O_TRUNC)
#define S_IRDWR		(S_IREAD|S_IWRITE)
#endif

#if MS
#include <fcntl.h>
#if MSDOS
//#include <sys\types.h>
//#include <sys\stat.h>		/* this is found in sys subdirectory and requires */
								/* an additional -i\include\sys\ compilation flag */
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
#define O_RAW		_O_BINARY
/*	O_CREAT		is already defined */
#define O_NEW	  	(O_CREAT|O_TRUNC)
#define S_IRDWR  	(S_IREAD|S_IWRITE)
#endif

#if XENIX
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
#define O_RAW		0
/*	O_CREAT		is already defined */
#define O_NEW	  (O_CREAT|O_TRUNC)
#define S_IRDWR	0xffff				  /* feel free to adjust this */
#endif
#endif /* MS */

#if (DCC | UNIX)
# ifndef _FCNTL_H
#   include <fcntl.h>
# endif
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
#define O_RAW 0
/*	O_CREAT		is already defined */
#define O_NEW	  	(O_CREAT|O_TRUNC)
#define S_IRDWR 0x01b6	/* read/write for all, execute for none	*/
#endif

#if MX
#include <fcntl.h>
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
#define O_RAW		0
/*	O_CREAT		is already defined */
#define O_NEW	  	(O_CREAT|O_TRUNC)
#define S_IRDWR	0
#endif

#if MW
#define O_RDONLY	0	/* open for reading ONLY */
#define O_RDWR		2
#define O_RAW		0	/* translation not implemented */
#define O_CREAT		0x4000	/* CBTREE local */
#define O_NEW		0x8000	/* CBTREE local */
#define S_IRDWR	0
#endif

#if DC
#define O_RDONLY	0	/* open for reading ONLY */
#define O_RDWR		2
#define O_RAW		0	/* translation not implemented */
#define O_CREAT		0x4000	/* CBTREE local */
#define O_NEW		0x8000	/* CBTREE local */
#define S_IRDWR	0
#endif


#if CI
#include <stdio.h>
#define O_RDONLY	0	/* open for reading ONLY */
#define O_RDWR		2	/* (xUPDATE) read and write */
#define O_RAW		4	/* (Bxxxxxx) binary mode */
#define O_CREAT		0x4000	/* CBTREE local */
#define O_NEW		0x8000	/* CBTREE local */
#define S_IRDWR	0
#endif

#if LC
#include <fcntl.h>
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
/*	O_RAW		is already defined */
/*	O_CREAT		is already defined */
#define O_NEW	  	(O_CREAT|O_TRUNC)
#if (LCVER < 300) | AmigaDOS
#define S_IRDWR	0
#else
#define S_IRDWR  (S_IREAD|S_IWRITE)
#endif
#endif

#if EC
#include <fcntl.h>
/*	O_RDONLY	is already defined */
/*	O_RDWR		is already defined */
#define O_RAW		0			/* binary mode */
#define O_CREAT		0x4000	/* CBTREE local */
#define O_NEW		0x8000	/* CBTREE local */
#define S_IRDWR	0
#endif
#endif
#endif /* BT_FIO_H */

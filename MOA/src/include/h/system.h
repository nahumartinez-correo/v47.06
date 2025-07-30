#ifndef  SYSTEM_H
#	define  SYSTEM_H

/*
 *	system parameters:
 *
 *	the operating system:
 *	(this is actually handled in c.h)
 *	  OS_PCDOS	IBM PC/DOS
 *	  OS_VMS	DEC VAX/VMS
 *	  (OS_UNIX)	Unix in all its flavours
 *
 *	machine architecture:
 *	  A_BELLMAC     Bellmac 32-bit Processor
 *	  A_GEC63	GEC Series 63
 *	  A_GOULD	Gould 32-bit Processor
 *	  A_HP32	HP 32-bit Microprocessor
 *	  A_IBM		IBM 360/370, Perkin-Elmer
 *	  A_IBM_RT	IBM RT RISC architecture
 *	  A_I86_S	Intel iAPX 86, 64K text, 64K data (not usable)
 *	  A_I86_M	Intel iAPX 86, >64K text, 64K data
 *	  A_I86_L	Intel iAPX 86, >64K text, >64K data
 *	  A_M68_I2	Motorola 68000 2-byte integer
 *	  A_M68_I4	Motorola 68000 4-byte integer
 *	  A_NS16	National Semiconductor NS16032
 *	  A_NS32	National Semiconductor NS32032
 *	  A_PDP11	DEC PDP-11 (not usable)
 *	  A_PYRAMID	Pyramid RISC architecture
 *	  A_VAX		DEC VAX
 *	  A_Z8K_NON	Zilog Z-8000 non-segmented (not usable)
 *	  A_Z8K_SEG	Zilog Z-8000 segmented
 *
 *	machine floating point format:
 *	  FLT_NONE	no floating point
 *	  FLT_DEC	DEC PDP-11/VAX compatible
 *	  FLT_GEC63	GEC Series 63
 *	  FLT_IBM	IBM 360/370 compatible
 *	  FLT_IEEE	IEEE standard
 *	  FLT_INTEL	Intel iAPX 086/186 compatible
 *	  FLT_RIEEE	IEEE standard with bytes swapped
 *	  FLT_UNISOFT	Unisoft Crap; like DEC but more limited
 *	  FLT_XIEEE	IEEE standard where compiler generates bad code
 *
 *	file system block size in bytes:
 *	  SYSZBLOCK
 *
 *	special labels used in the source:
 *
 *	  ENV_TZ	timezone info in environment variable "TZ"
 *
 *	  IO_BSD	Berkeley line discipline for ttys
 *	  IO_TERMIO	termio entry for ttys
 *	  IO_XENIX	like termio, but different include files
 *	  (IO_V7)	if none of above 3
 *
 *	  LCK_B4_2	Berkeley 4.2 file locking
 *	  LCK_S5	System 5 type file locking
 *	  LCK_XENIX	Xenix file locking
 *	  LCK_ZEUS	ZEUS file locking
 *	  (LCK_NONE)	if none of above 4
 *
 *	  OPN_B4_2	Berkeley 4.2 "open" w. append
 *	  OPN_S5	System 5 "open" w. append
 *	  (OPN_NORMAL)	if none of above 2
 *
 *	  DIR_B4_2	Berkeley 4.2 "opendir", "closedir", "readdir"
 *	  GETWD_B4_2	Berkeley 4.2 "getwd"
 *	  RENAME_B4_2	Berkeley 4.2 "rename"
 *	  FSYNC_B4_2		Berkeley 4.2 "fsync"
 *	  VFORK_B4_2		Berkeley 4.2 "vfork"
 *	  HOST_B4_2	Berkeley 4.2 "gethostname"
 *	  SYS_B4_2	A short hand for the symbol DIR_B4_2, GETWD_B4_2,
 *			RENAME_B4_2, FSYNC_B4_2, VFORK_B4_2, HOST_B4_2
 *
 *	  SUN_NFS	Runing SUN OS with NFS
 *
 *	  PAR_GEC63	GEC reverses its parameter stack with 16-byte alignment
 *	  PAR_PYR	Pyramid passes parameters in registers
 *	  PAR_REV	parameter stack is reversed
 *	  PAR_FZEUS	ZEUS passes parameters in regular + float registers
 *	  PAR_ZEUS	ZEUS passes parameters in regular registers
 *	  (PAR_NORMAL)	if none of above 5
 *
 *	the following are not recommended for default use:
 *
 *	  SP_NON_MACRO	compiler cannot handle the macros in the SP stuff
 *
 *	  SYS_PW	use a system procedure to get the user name
 *
 *	customization (optional):
 *	  CUS_GREEK	greek support
 */

/*
 *	to handle the none of the above
 */

#ifndef  IO_BSD
#ifndef  IO_TERMIO
#ifndef  IO_XENIX
# define  IO_V7
#endif
#endif
#endif
#ifndef  LCK_B4_2
#ifndef  LCK_S5
#ifndef  LCK_XENIX
#ifndef  LCK_ZEUS
# define  LCK_NONE
#endif
#endif
#endif
#endif
#ifndef  OPN_B4_2
#ifndef  OPN_S5
# define  OPN_NORMAL
#endif
#endif
#ifndef  PAR_GEC63
#ifndef  PAR_PYR
#ifndef  PAR_REV
#ifndef  PAR_FZEUS
#ifndef  PAR_ZEUS
# define  PAR_NORMAL
#endif
#endif
#endif
#endif
#endif

/*
 *	machine architecture parameters:
 *
 *	WORD32BIT	if a word is 32 bits; else
 *	  WORD16BIT
 *	ADDR32BIT	if an addr is 32 bits; else
 *	  ADDR16BIT
 *	LONGISINT	if long int is the same as int; else
 *	  LONGNOTINT
 *	ADDRISINT	if addr is the same as int; else
 *	  ADDRNOTINT
 *	TADDRISINT	if taddr is the same as int; else
 *	  TADDRNOTINT
 *	CHARISSIGNED	if char is signed; else
 *	  CHARNOTSIGNED
 *	ALIGN_n		data alignment on N-byte boundary
 */

/*
 *	various machine architectures
 */

#ifdef  A_BELLMAC
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARNOTSIGNED
# define  ALIGN_4
#endif

#ifdef  A_GEC63
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARNOTSIGNED
# define  ALIGN_8
#endif

#ifdef  A_GOULD
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_8
#endif

#ifdef  A_HP32
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_4
#endif

#ifdef  A_IBM
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARNOTSIGNED
# define  ALIGN_8
#endif

#ifdef  A_IBM_RT
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARNOTSIGNED
# define  ALIGN_4
#endif

#ifdef  A_I86_S
# define  WORD16BIT
# define  ADDR16BIT
# define  LONGNOTINT
# define  CHARISSIGNED
# define  ADDRISINT
# define  TADDRISINT
# define  ALIGN_2
#endif

#ifdef  A_I86_M
# define  WORD16BIT
# define  ADDR16BIT
# define  LONGNOTINT
# define  CHARISSIGNED
# define  ADDRISINT
# define  TADDRNOTINT
# define  ALIGN_2
#endif

#ifdef  A_I86_L
# define  WORD16BIT
# define  ADDR32BIT
# define  LONGNOTINT
# define  CHARISSIGNED
# define  ADDRNOTINT
# define  TADDRNOTINT
# define  ALIGN_2
#endif

#ifdef  A_I86
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_2
#endif

#ifdef  A_M68_I2
# define  WORD16BIT
# define  ADDR32BIT
# define  LONGNOTINT
# define  ADDRNOTINT
# define  TADDRNOTINT
# define  CHARISSIGNED
# define  ALIGN_2
#endif

#ifdef  A_M68_I4
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_2
#endif

#ifdef  A_NS16
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_1
#endif

#ifdef  A_NS32
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_1
#endif

#ifdef  A_PDP11
# define  WORD16BIT
# define  ADDR16BIT
# define  LONGNOTINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_2
#endif

#ifdef  A_PYRAMID
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_4
#endif

#ifdef  A_VAX
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_1
#endif

#ifdef  A_Z8K_NON
# define  WORD16BIT
# define  ADDR16BIT
# define  LONGNOTINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARISSIGNED
# define  ALIGN_2
#endif

#ifdef  A_Z8K_SEG
# define  WORD16BIT
# define  ADDR32BIT
# define  LONGNOTINT
# define  ADDRNOTINT
# define  TADDRNOTINT
# define  CHARISSIGNED
# define  ALIGN_2
#endif

#ifdef	A_RIDGE
# define  WORD32BIT
# define  ADDR32BIT
# define  LONGISINT
# define  ADDRISINT
# define  TADDRISINT
# define  CHARNOTSIGNED
# define  ALIGN_8
#endif

#ifdef	SYS_B4_2
# define  DIR_B4_2
# define  GETWD_B4_2
# define  RENAME_B4_2
# define  FSYNC_B4_2
# define  VFORK_B4_2
# define  HOST_B4_2
#endif

/*
 *	system dependent functions
 */

#ifdef  CHARISSIGNED
# define  ch_int(ch)	(int)(ch)
# define  uch_int(ch)	((ch) & BYTEMASK)
#else
global_var	int	_chx_;
# define  ch_int(ch)	(_chx_ = (ch), \
		((_chx_ & BYTEMSB) == 0) ? _chx_ : (_chx_ | ~BYTENOMSB))
# define  uch_int(ch)	(int)(ch)
#endif

#ifdef ADDR16BIT
#	define VPTR short
#else
#	ifdef WORD16BIT
#		define VPTR long
#	else
#		define VPTR int
#	endif
#endif


#endif

#ifndef CDSMACRO_H
#define CDSMACRO_H
#ifndef THIS_SYS_H
#   include <this_sys.h>
#endif
#ifndef CDS_RECLEN
#define CDS_RECLEN
typedef	 unsigned short	reclen;	/* size of record length specifier */
#endif

// THIS is an NT only version
//#if (DNIX == 0 && !defined(A_M68_I4))
//#	if (defined(__GNUC__) || NT == 1)
#	 define	ntohl(u) (((uint)u<<24)|(((uint)u&0x0000ff00)<<8)|(((uint)u&0x00ff0000)>>8)|((uint)u>>24))
#	 define	ntohs(u)	((ushort)(((ushort)u<<8)|((ushort)u>>8)))
#	 define	htonl(u) ntohl(u)	
#	 define	htons(x)	ntohs(x)
//#	elif (defined _lint)
//		extern unsigned short ntohs(unsigned short);
//		extern unsigned short htons(unsigned short);
//		extern unsigned  ntohl(unsigned );
//		extern unsigned  htonl(unsigned );

#define RECLEN(p) ((*(byte *)p << 8) | (*(byte *)((byte *)p+1)))
#define SETRECLEN(p,len) *(byte *)p = (byte)(len >> 8), *(byte *)((byte *)p+1)= (byte)len
#define put_appl(p,a) *(byte *)p++ = a	/* put application number into buffer, advance pointer*/
#define PUT_USHORT(p,ushrt) *(byte *)p++ = (ushrt >> 8), *(byte *)p++ = (byte)ushrt
#define put_field(p,f) *(byte *)p++ = f >> 8, *(byte *)p++ = (byte)f
#define put_length(p,len) PUT_USHORT(p,len)
#define set_length(p,len) SETRECLEN(p, len)
#define get_appl(a, p) a = *(byte *)p++		/* get appl num from buffer */
#define GET_FIELD_ID(a, f, p) {regvar r; a= *(byte *) p++; r= *(byte *)p++ << 8; r |= *(byte *)p++; f=r;}	/* get field num */
#define GET_USHORT(j, p) j = *(byte *)p++ << 8, j |= *(byte *)p++
#define get_length(len, p) GET_USHORT(len,p)
#define CDSFLENP(len, p) GET_USHORT(len,p)
#define CDSFLEN(p) RECLEN(p)
#define get_field(f,p)     GET_USHORT(f,p)
#define GET_SUBSCRIPT(s,p) GET_USHORT(s,p)

#define CDS_STRING 'A'
#define CDS_NUMBER 'N'
#define CDS_INTEGER 'I'
#define CDS_INDIRECT (byte) 0xDD
#define CDS_FIELD 'f'
#define CDS_JOIN_FIELD 'j'

#define CDS_INDIRECT_MASK	(byte) 0x80;

#ifdef A_M68_I4
#	define GET_INT(p) (*(int *)p)
#	define PUT_INT(p,i) *(int *)p = i
#	define SETARECLEN(p,len) *(reclen *)p = len
#	define ARECLEN(p) (*(reclen *)p )
#else
# define GET_INT(p) ((*(byte *)p << 24) | (*(byte *)(((byte *)p+1)))<<16 | (*(byte *)((byte *)p+2))<< 8 | *(byte *) (((byte *)p)+3))

# define PUT_INT(p,i) *(byte *)p = i >> 24, *(byte *)((byte *)p+1) = i >> 16 & 0x00ff, *(byte *)(((byte *)p+2))= i >> 8 & 0x00000ff, *(byte *)((byte *)p+3) = i & 0x000000ff

# define SETARECLEN(p,len) *(byte *)p = (byte)(len >> 8), *(byte *)((byte *)p+1)= (byte)len
/* # define SETARECLEN(p,len) *(reclen *)p = htons(len)*/
# define ARECLEN(p) RECLEN(p)
#endif
/* bit switch values for sv_who() options */
#define CONTEXT_LISTS 1
#define TABLE_SIZES   2
#define MEMORY_STATS  4
#define CHECK_TABLES  8
#define CDS_SET_DEBUGSW 32
#define CDS_SET_NWLOG	64

#define NOT_UNIQUE 0
#define UNIQUE 1
#define SEMI_DISTINCT 2

/* Out of Band Messages */
#define SHADOW_WENT_DOWN 1
#define SHADOW_CAME_UP 2
#define CATALOG_CHANGE 4

/* server optimizer options */
#define FIND_INDEX_ON_NULL_ORDER 1
#define FIND_INDEX_ON_EMPTY_ORDER_INDEX_NAME 2
#define TRIM_ON_COMPRESS 8
#define CDS_NOT_NULL 'N'
#define CDS_NULL_OK ' '

/* connection options */
#ifndef	CDS_MAX_STMT_OPTIONS
#define CDS_MAX_STMT_OPTIONS 5
#endif
# define CDS_CURSOR_FETCH_BEHAVIOUR 0
# define CDS_CURSOR_FETCH_BEHAVIOUR_OLD 0
# define CDS_ANSI_SQL_CURSOR_BEHAVIOR 1
# define CDS_TRACE 1
# define CDS_TRACE_ON 1
# define CDS_TRACE_OFF 0
# define CDS_NULL_PROJECTION_BEHAVIOR 2
# define CDS_NULLS_PROJECT_AS_DEFAULT 0
# define CDS_NULLS_PROJECT_AS_NULL 1
# define CDS_TIMESTAMP_TYPE_SUPPORT   3
# define CDS_TIMESTAMP_SUPPORTED 1
# define CDS_TIMESTAMP_NOT_SUPPORTED 0
# define CDS_SQL_TXN_ISOLATION   4	// see option flag definitions for SQL_TXN_ISOLATION
#define CDS_DEFAULT_APP_ID_OPTION 2200  // number assigned by Jim Melton of X/Open
#endif

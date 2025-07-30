#ifndef CDSDEFS_H
#define CDSDEFS_H

#pragma pack (push, 2)

struct ddid {
	reclen len;
	char system; /* O/S type */
	long ddtime;
	long ddhash;
	long threadID;
};

typedef struct
 {
	unsigned short	len; 		/*always = sizeof(table_options) */
	unsigned short	base_size;
	unsigned short	idx_size;
	unsigned short	buffer_size;			/* for rt's buffers */
	unsigned short	minimum_record_size;
	byte			allow_delete;			/* TRUE if allowed */
	byte			allow_key_change;		/* TRUE if allowed */
	byte			allow_new_field;		/* TRUE if allowed */
} table_options;

typedef struct table_information
 {
 	char	name[TBLEN+1];
 	reclen  rt_buffer_size;
 } table_info;

struct table_sizes_hdr
	{
		reclen	len;
		long	time_stamp;
		long	log_length;
		unsigned short	table_count;
		long	check_sum;
	} ;
struct table_size
	{
		char	name[TBLEN+1];
		long	rec_count;
		long	bytecount;
	};


typedef struct table_join_entry
 {
 	char	table_name[CLTBLEN+1];
 	char	preserve;
 	char	as_name[CNTXTLEN+1];
 	char	index_name[ALTIDXLEN+1];
 	char	scan_direction;
 } join_table_entry;

typedef struct jlp
{
	reclen len;
	join_table_entry join_list[2];	/* may be more than two */
} join_list_param;

struct recset
{
	reclen	length;
	unsigned short	count;
	char	recs[2];	/* this is variable length */
};

typedef struct fndset	/* for return value from Find_n */
{
	reclen	length;
	unsigned short	more_flag;	/* true if count is less than asked for
              * (because buffer overflowed and more records are available. */
	unsigned short	count;
	char	recs[2];	/* this is variable length */
} foundset;

#define CDS_ASCENDING_SEGMENT  'A'   /* ascending key segment */
#define CDS_DESCENDING_SEGMENT 'D'   /* descending key segment */
#define CDS_SPARSE_INDEX       'S'   /* ommit from index if segment is null */
#define CDS_GENERATED_KEY      'G'   /* the serial integer data type */

#define	CDS_ORDER_INDEX_NAME 0
#define	CDS_ORDER_FIELD_LIST 1
typedef struct cds_order_ref {	  /* order by field reference */
	byte	direction;	/* A= acsending, D=descending */
	byte	appl_no;	/* application number */
	index	field_no;   /* in normal format */
} cds_order_ref;
#pragma pack (pop)

typedef long luw_flag;
#define	WORK			0 /* This is the default for Integra */
#define BEGIN_WORK		1
#define COMMIT_WORK		3
#define IMMEDIATE_COMMIT 4
#define PHASE1_COMMIT	8

/* defines anded with the "begin" LUW byte for additional state info */
#define	CDSRESET	(0x02)
extern byte LUW_State;
#define	NOT_IN_LUW 0
#define LUW_BEGIN_PENDING 1
#define LUW_STARTING LUW_BEGIN_PENDING /* for server side only */
#define IN_LUW 2
#define COMMITTING 3
#define ROLLING_BACK 4				/* for server side only */
#define PREPARED PHASE1_COMMIT
#define POLLING_PARTICIPANTS 16
#define POLLING_COMPLETED    32
#define COMMITTING_PHASE2 64
#define ROLLING_BACK_PHASE2 65


typedef enum
{
	WITHOUT_LOCK=0,
	WITH_LOCK,			/* establish an update lock & permit view by others */
	EXCLUSIVE_LOCK,		/* prohibit view by others */
	READ_LOCK=4 		/* prohibit update lock by others */
} lock_flag;
#define WITHOUT_LOCKING 0

/* defines used with table level locking */
#define TABLE_UPDATE_LOCK 1
#define EXCLUSIVE_TABLE_LOCK 2
#define ALL_TABLE_LOCKS 3
#define TABLE_LOCK_INQUIRY 8

/* aggregate function types */
typedef enum
{
	CDS_NO_AGG=0,
	CDS_COUNT,
	CDS_SUM,
	CDS_AVG,
	CDS_MIN,
	CDS_MAX
} agg_type;

#endif

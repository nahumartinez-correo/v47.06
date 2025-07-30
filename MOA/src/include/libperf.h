/*
**	"$Header: X:/moa/src/include/rcs/libperf.h 1.0 1994/03/11 15:55:35 PPLUS Exp $";
*/

#ifndef LIBPERF_H
#define LIBPERF_H

#define	PERFLIB_EXTRA	16
#define MAX_PERFREQS	20

#pragma pack (push, 2)
/*
**	This structure definition is used for the open, read or write command
**	that is sent to the server
*/
struct perf_orw {
	unsigned char	spacer1[14];
	unsigned char	MoreByte;
	unsigned char	FunctionCode;
};

typedef struct perf_orw perf_orw;

/*
**	This structure definition is used for the return of the server data.
**	It defines the data returned for an open, write or error.
*/
struct perf_owe_rtn {
	unsigned char	spacer1;
	unsigned char	MoreByte;
	int				ReturnValue;
	unsigned char	spacer2[2];
	unsigned char	FunctionCode;
	unsigned char	spacer3[7];
};

typedef struct perf_owe_rtn perf_owe_rtn;

/*
**	This structure definition is used for the return of the server data.
**	It defines the data returned for a read.
*/
struct perf_read_rtn {
	unsigned char	spacer1;
	unsigned char	MoreByte;
	int				ReturnValue;
	unsigned char	spacer2[2];
	unsigned char	FunctionCode;
	unsigned char	spacer3;
	unsigned char	DeviceType;
	unsigned char	DataType;
	int				DataLen;
};

typedef struct perf_read_rtn perf_read_rtn;

/*
**	This union brings together the three structures defined above to support
**	communication with the perf_man server.
*/
union perf_lib_header {
	perf_orw		perf_lib_orw;
	perf_owe_rtn	perf_lib_owe;
	perf_read_rtn	perf_lib_read;
};

typedef union perf_lib_header perf_lib_header;


/*
**	This structure definition is used to pass information between the
**	no-wait I/O completion functions.  This allows the perf_lib routines to
**	do an extra no-wait operation before completing back to the clients
**	completion function.
*/
struct	perfstruct {
	void		*our_fcptr;			/* this points us to our next function */
	void		**their_strucptr;	/* this is the clients structure pointer */
	int			trapq_fd;			/* if this is set, they want no-wait I/O */
	int			first_rn;			/* the first I/O request number */
	int			second_rn;			/* the intermediate I/O request number */
	int			file_des;			/* the file descriptor returned from open */
	int			type;				/* type designator of SERVER or HANDLER */
	char		*clientdata;		/* pointer to clients data buffer */
	int			clientlen;			/* length of client/server buffer */
	int			state;				/* state of activities in progress */
	int			function;			/* function in progress */
};

typedef struct	perfstruct	PERFSTRUCT;

typedef struct	perfreqs	PERFREQS;

/*
**	This structure definition contains the necessay elements for the list
**	manager routines to properly handle the requests for the handler/server.
**	It also contains the perfstruct to be used as described above.
*/
struct	perfreqs {
	PERFREQS	*next;				/* pointer to next structure */
	PERFREQS	*prev;				/* pointer to previous structure */
	short		flags;				/* flags used by list handler */
	PERFSTRUCT	perfstruct;			/* the libperf structure */
};

#pragma pack (pop)

/*
** Function Prototypes for perflib.c
*/
	int  perf_open(char *system_id, char *device, int oflag, int mode, int trapque, void *strucptr);
	int  perf_read(int filed, char *buffer, int nbyte, int trapque, void *strucptr);
	int  perf_write(int filed, char *buffer, int nbyte, int trapque, void *strucptr);
	int  perf_close(int filed, int trapque, void *strucptr);

#endif

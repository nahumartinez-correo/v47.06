#include "pch.h"
#include "rtdef.h"

#ifndef APIENTRY	// Windows NT defines APIENTRY, but 3.x doesn't
# define APIENTRY far pascal
#endif
#include <getopt.h>
#include "c.h"
#include "er.h"
#include "cdserror.h"
#include "clconn.h"
#include "cdsfuncs.h"
#include "clcdsfn.h"
#include "defines.h"
#include "dataman.h"
#include "memalloc.h"
#include "clientdf.h"
#include "cdsfuncs.h"
#include "ppdefs.h"
#include "swerror.h"
#include "rtprotos.h"
#include "cmgetenv.h"
#include "mcnuser.h"
#include "tcl.h"
#ifndef SYSTEM_H
#	include "h/system.h"
#endif
#include "cmfield.h"
#include "errdefs.h"
HANDLE OutFH=INVALID_HANDLE_VALUE;
HANDLE ErrFH=INVALID_HANDLE_VALUE;

char	*Jnlptr, *Jnlbuf;
int	Jnlblen = 1024;
GLOBAL_VAR unsigned short Max_length=3000;
GLOBAL_VAR char	Sequence_number[8];

GLOBAL_VAR short TruncateFlag;

field Br, Teller, Business_date, Sequence, Amount, HostStat, Stuff, Field_a;

GLOBAL_VAR struct field_list List[] = {
	{ &Br, 0,0,"common","Cds_num2" },	/* normally the internal and external */
	{ &Teller, 0,0,"common","Cds_num3" }, /* names should agree, this is not a good example */
	{ &Business_date,0,0,"common","Cds_date"   },
	{ &Sequence, 0,0,"common","Cds_num4" },
	{ &HostStat, 0,0,"common","Cds_table_no"  },
	{ &Amount, 0,0,"common","Cds_table_indexes"  },
	{ &Stuff, 0,0,"common","Cds_table_name"  },
	{ &Field_a, 0,0,"common","field_a" },
	{ (field * ) 0, 0, 0,"","" }
};	

GLOBAL_VAR struct field_list *Branch=&List[0], *Operator=&List[1],
    *Bus_Date= &List[2], *Sequence_Number= &List[3],
    *Field_Array=&List[7];

char	criteria3[60];
/*lint -e569 */
char	criteria5[] = {0,36,0,CDS_FIELD,2,0,3,(char)PCLle,FT_DATE,0,8,
								'1','9','8','9','0','4','2','4',
								(char)PCLand,
								CDS_FIELD,2,0,3,(char)PCLge,
								FT_DATE,0,8,
								'1','9','8','9','0','4','2','1'
							};

char	criteria6[] = {0,42,0,CDS_FIELD,2,0,1,(char)PCLeq,FT_INT,0,4,0,0,0,
								1,
								(char)PCLand,
								CDS_FIELD,2,0,2,(char)PCLeq,
								FT_NUMERIC,0,1,
								'1',
								(char)PCLand,
								CDS_FIELD,2,0,3,(char)PCLge,
								FT_DATE,0,8,
								'1','9','8','9','0','1','0','1'
							};

/*lint +e569 */

/*********************************
* init_fields - set up field memory
*	of fields to be used to compose record buffer
*********************************/
GLOBAL_PROC void	init_fields()
{
	long	machine_time=time(NULL);
	struct	tm 	*now=localtime(&machine_time);
	static	char	bdate[9]="19890415";

	OutFH = GetStdHandle(STD_OUTPUT_HANDLE);
	ErrFH = GetStdHandle(STD_ERROR_HANDLE);

	memset((addr) & Br, 0, sizeof(field));
	Br.storage = (byte * ) "1";
	Br.flags.type = FT_NUMERIC;
	Br.length = 1;

	memset((addr) & Teller, 0, sizeof(field));
	Teller.storage = (byte * ) "01";
	Teller.flags.type = FT_NUMERIC;
	Teller.length = strlen((char *) Teller.storage);

	memset((addr) & Business_date, 0, sizeof(field));
	Business_date.storage = (byte * ) bdate;
	sprintf(&bdate[0],"%04d",now->tm_year+1900);
	sprintf(&bdate[4],"%02d",++(now->tm_mon));
	sprintf(&bdate[6],"%02d",now->tm_mday);
	Business_date.flags.type = FT_DATE;
	Business_date.length = 8;

	memset((addr) & Sequence, 0, sizeof(field));
	Sequence.storage = (byte * ) Sequence_number;
	Sequence.flags.type = FT_NUMERIC;
	Sequence.length = strlen(Sequence_number);

	memset((addr) & Amount, 0, sizeof(field));
	Amount.storage = (byte * ) "1000";
	Amount.flags.type = FT_NUMERIC;
	Amount.length = 4;

	memset((addr) & HostStat, 0, sizeof(field));
	HostStat.flags.type = FT_NUMERIC;
	HostStat.storage = (byte * ) "0";
	HostStat.length = 1;

	Stuff.flags.type = FT_ASCII;
	Stuff.storage = (byte * ) "STUFF";
	Stuff.length = 5;

	memset((addr) & Field_a, 0, sizeof(field));
	Field_a.flags.type = FT_ASCII | (1 << 6);
	Field_a.storage = (byte * ) 0;
	Field_a.length = 0;

}


/*********
* applendJnlbuff - append one field to journal buffer
*
* Returns: success or failure
**********/

GLOBAL_PROC int	appendJnlbuff(field *fp, byte application, short field_number)
							/*application number, field index number*/
{
	static fieldv fld;
	register char *ptr=Jnlptr;

	fld.length = strlen((char *) fp->storage);
		/* calculation of length is different for FT_FIXED, FT_INT and FT_BULK */
	fld.value = fp->storage;
	fld.appl_id = application;
	fld.field_id = field_number;
	fld.type = fp->flags.type;
	fp->length = fld.length;
	if ((ptr = cm_append_field((byte *) ptr, &fld, &Jnlblen,1))==NULL) {
		// RTerrno = JNLBUFFOFLOW;
		return(RT_ERROR);
	}
	Jnlptr = ptr;
	return(RT_SUCCESS);
}

GLOBAL_PROC void myprintf(HANDLE fh, const char *format, ...)
{
	va_list ap;
	int nbytes, nbytes_written;
	char buffer[1024];

	if (fh == INVALID_HANDLE_VALUE) {
		//	fh = GetStdHandle(STD_ERROR_HANDLE);
		fh= CreateFile("CONOUT$",GENERIC_WRITE,FILE_SHARE_READ,NULL,
		// Append to current file unless debugging
						0 ? OPEN_ALWAYS : CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_WRITE_THROUGH*/,0);
		if (fh == INVALID_HANDLE_VALUE)
			erbug("can't get stderr handle, error = %d",nbytes = GetLastError());
	}

	va_start(ap, format);
	nbytes = vsprintf(buffer, format, ap);
	if (!WriteFile(fh,buffer,nbytes,&nbytes_written,NULL))
		erbug("Can't WriteFile(%x, %x, %d, %x,NULL) in prnt(fh, ) GetLastError = %d",fh,buffer,nbytes,&nbytes_written,
			GetLastError());
} 

/************************
* build_buffer	- allocate and fill record buffer for insert() or replace()
* 
* Returns pointer to buffer aquired via malloc()
***********/
GLOBAL_PROC char	*build_buffer()
{
	short	i;
	reclen  len;

	Jnlptr = mem_calloc(Max_length, 1);
	if (!(Jnlbuf = Jnlptr)) {
		myprintf(ErrFH,  "no memory for journal buffer");
		return(0);
	}
	Jnlptr += sizeof(reclen);
	Jnlblen = Max_length - sizeof(reclen);

	for (i = 0; List[i].fp != (field * ) 0 && List[i].fp->storage; i++) {
		if (appendJnlbuff(List[i].fp, List[i].application, List[i].field_number)
			 != RT_SUCCESS)
			myprintf(ErrFH,  "buffer build failed");
	}
	len = Jnlptr - Jnlbuf; /*set length in front*/
	SETARECLEN(Jnlbuf,len);

	return(Jnlbuf);
}

/* BUILD_CRITERIA2 from current date   to get last record with current date
*  for current operator.  This is used to find the sequence number for the
*  next insert.
*
*	Returns: a pointer to the criteria string (with leading length).
*/
GLOBAL_PROC	char	*build_criteria2()
{
	static	char	criteria[256];
	char	*criteria_ptr = &criteria[0] + sizeof(reclen);
	reclen len;


	sprintf(criteria_ptr,"f%d:%d == 1 && f%d:%d == \"",
		Branch->application, Branch->field_number,
		Operator->application, Operator->field_number);
	criteria_ptr += strlen(criteria_ptr);
	memcpy(criteria_ptr, Teller.storage, (Teller.length));
	criteria_ptr += Teller.length;
	sprintf(criteria_ptr,"\" && f%d:%d == ",
		Bus_Date->application, Bus_Date->field_number);
	criteria_ptr +=  strlen(criteria_ptr);
	memcpy(criteria_ptr,Business_date.storage,Business_date.length);
	criteria_ptr+= Business_date.length;
	*criteria_ptr = '\0';
	len = criteria_ptr - &criteria[0];
	SETARECLEN(&criteria[0],len);
	return(criteria);
}
GLOBAL_PROC void build_criteria3(char criteria_3[],char *start_date, char *end_date)
{
	reclen len;
	sprintf(criteria_3, "__f%d:%d < %s && f%d:%d > %s",Bus_Date->application,
		Bus_Date->field_number,end_date,
		Bus_Date->application,
		Bus_Date->field_number,start_date);
	len = strlen(criteria_3);
	SETARECLEN(criteria_3,len);
}
GLOBAL_PROC void build_criteria5(char criteria[],char *start_date, char *end_date)
{
	criteria[4] = Bus_Date->application;
	criteria[5] = Bus_Date->field_number >> 8;
	criteria[6] = Bus_Date->field_number % 256;
	criteria[21] = Bus_Date->application;
	criteria[22] = Bus_Date->field_number >> 8;
	criteria[23] = Bus_Date->field_number % 256;
	memcpy(criteria + 11, end_date, 8);
	memcpy(criteria + 28, start_date, 8);
}
GLOBAL_PROC void	build_criteria6(char criteria[])
{
	criteria[4] = Branch->application;
	criteria[5] = Branch->field_number >> 8;
	criteria[6] = Branch->field_number % 256;
	criteria[17] = Operator->application;
	criteria[18] = Operator->field_number >> 8;
	criteria[19] = Operator->field_number % 256;
	criteria[27] = Bus_Date->application;
	criteria[28] = Bus_Date->field_number >> 8;
	criteria[29] = Bus_Date->field_number % 256;
}

GLOBAL_PROC bool translate_field_names(clenv *env)
{
	short i;
	byte appl_id;

	for (i = 0; List[i].fp != (field * ) 0; i++) {
		if ((appl_id = ddapplindex(env->RTddh,List[i].appl_name))==0) {
			myprintf(ErrFH, "Unknown application name '%s'\n",List[i].appl_name);
			return(false);
		}
		List[i].application = appl_id;
		if ((List[i].field_number = ddindex(env->RTddh,FLD,appl_id,List[i].field_name))==0) {
			myprintf(ErrFH, "Unknown field name '%s'\n",List[i].field_name);
			return(false);
		}
	}
	return(true);
}

/* GET_SEQ_NUM - Get sequence number from table record buffer
* Put the field value into global var Sequence_number.
*	return true if sucesseful
*		else false
*/
GLOBAL_PROC bool	get_seq_num(char *buffer)
{
	fieldv sequence;
	fieldv buff_fld;
	char *buff_ptr,
		 *buff_end;
	bool found_it = false;

	sequence.appl_id = Sequence_Number->application;
	sequence.field_id = Sequence_Number->field_number;
	sequence.type = FT_NUMERIC;
	sequence.length = 4;

	buff_end = buffer + ARECLEN(buffer);
	buff_ptr = buffer + sizeof(reclen);

	buff_ptr = cmfield(&buff_fld,(byte *) buff_ptr);

	while(buff_ptr < buff_end)
	{
		if(buff_fld.appl_id == sequence.appl_id)
		{
			if(buff_fld.field_id == sequence.field_id)
			{
				memcpy(Sequence_number,buff_ptr - buff_fld.length,buff_fld.length);
				Sequence_number[buff_fld.length]= '\0';  /*terminate string */
				found_it = true;
				break;
			}
		}
		buff_ptr = cmfield(&buff_fld,(byte *)buff_ptr);
	}
	return(found_it);
}


/* GET_N() get n records from context and print their contents
**
** return number of records found.
*/
int	get_n(client_context *context, int n, int skip)
{
	foundset *found;
	char *buffer;
	reclen	record_length;
	ushort  record_count, i;
	unsigned long	tics, start_tics;

	start_tics = GetTickCount();		/* start of function  */
	if (!(found = Cds_Find_n(context, n, skip)))
	{
		myprintf(ErrFH, "get_n() line %d, n=%d, skip=%d,\n%s\n",__LINE__,
					n, skip, cdsErrMsg(context->tbl->ccb->env));
		return(0);
	}
	tics = (GetTickCount()) - start_tics;	/* end of function  */
	/*seconds = 1.0 * tics/clock_tics;*/
	myprintf(OutFH, "Find_n within get_n succeeded in %d tics\n",tics);
	record_count = (found->count);
	myprintf(ErrFH, "Find_n returned %d records\n",record_count);
	buffer = found->recs;
	for (i=1; i<=record_count; i++)
	{
		myprintf(ErrFH, "Record #%d, ",i);
		cdsprint(ErrFH,buffer);
		record_length = RECLEN(buffer);
		buffer += record_length;
	}
	return(record_count);
}

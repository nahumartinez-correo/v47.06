#ifndef TCL_H
#define TCL_H
#ifndef CMFIELD_H
#	include <cmfield.h>
#endif
extern HANDLE OutFH;
extern HANDLE ErrFH;
void myprintf(HANDLE fh, const char *format, ...);

global_var char	*Jnlptr, *Jnlbuf;
global_var int	Jnlblen;
global_var unsigned short Max_length;

global_var field Br, Teller, Business_date, Sequence, Amount, HostStat, Stuff;

global_var struct field_list {
	field *fp;
	byte	application;
	index field_number;
	char appl_name[8];
	char field_name[18];
} List[];

global_var struct field_list *Branch, *Operator,
							*Bus_Date, *Sequence_Number;

global_var char	Sequence_number[8];

global_var char criteria3[], criteria5[], criteria6[];

global_proc void my_itoa(char *s, int n);
global_proc void init_fields(void);
global_proc int	appendJnlbuff(field *fp, byte application, short field_number);
global_proc char	*build_buffer(void);
global_proc	char	*build_criteria2(void);
global_proc void build_criteria3(char criteria3[],char *start_date, char *end_date);
global_proc void build_criteria5(char criteria5[],char *start_date, char *end_date);
global_proc void build_criteria6(char criteria6[]);
global_proc unsigned tics_per_second(void);
global_proc char *cds_get_fld(fieldv *fld_struct, char *buffer);
global_proc bool translate_field_names(clenv *env);
global_proc	bool	get_seq_num(char *buffer);
global_proc	int		get_n(cdsselect *context, int n, int skip);
#endif

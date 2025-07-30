#ifndef SVMODE_H
#define SVMODE_H
#ifndef	CDSLIMITS_H
#	include <cdslmts.h>
#endif

typedef enum
{
	NORMAL=0,
	PRIMARY,
	SHADOW
} run_mode;

#define CDS_MAX_SERVICE_NAME 64
#define SHADOW_SUFFIX "_SHADOW"

struct my_identity 
{
	reclen	len;
	char machine[MACHINE_NAME_LEN+2];
	int	process_id;
};

struct setup_return
{
	reclen	length;
	long	prime_time;
	long	prime_log_length;
	long	prime_log_time;		/* timestamp on first record in log for this run */
};
#ifdef NT
#define NPROTOCOLS 1
#endif

struct net_protocol {
	//struct t_info   T_info;		  for SVR4 only
	int fd;
	short port;
	char name[8];
}; 
extern struct net_protocol Protocol[];

global_var char *Alarmed_function;
global_var bool Beginning;
global_var	short Shutdown_Sleep_Period;
global_var	int   My_pid;
global_var	run_mode	Run_Mode, Original_Mode;
global_var	int	Primary_fd;
global_var char *Project, *Level;
global_var	short	Flush_Trigger;
global_var	short	Nice;
global_var	unsigned	Updates_as_primary;
global_var	boolchar	Primary_Up;
global_var	bool	Im_alone, Put_Shadow_Died;
global_var long	Log_Dif_Limit; /* Max difference between shadow log *
									* and primary log that still allows shadow
									* to synchronize with primary */
global_const char Dont_B_Primary[];

global_proc void shadow(long log_length);
global_proc void start_shadow(bool exists_other_primary);
global_proc bool shadow_verify(addr shadows_list);
global_proc bool get_lan_status(void);
global_proc void MakeMyName(void);

#endif

#ifndef CBFUNCS_H
#define CBFUNCS_H
global_proc void call_remote(Connectcb *connectcb, pos code, ...);
global_proc Connectcb *createconnectcb(void);
global_proc void closeconnectcb(Connectcb *);
global_proc unsigned short client_count(void);
global_proc Connectcb *find_user(Connectcb *user);
global_proc Connectcb *find_user_by_name(const char *user);
global_proc void	close_connects(void);
global_proc void	checkHearts(void);
global_proc	void addtopendingque(Connectcb *);
global_proc boolean executepending( void );
global_proc	boolean	executescan(Connectcb *connectcb);
global_proc	bool	addtoscanque(Connectcb *connectcb);
/*global_proc	char	*sv_who(int options);*/
global_proc Connectcb	*in_LUW( void );
global_proc	bool	is_LUW( void );
global_proc void ask_coordinator(Connectcb *user);
global_proc void set_oob_bit(unsigned short bits);

#endif

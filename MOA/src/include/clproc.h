#ifndef CLPROC_H
#define CLPROC_H
global_proc void add_to_tab_list (regpar short count, table_info *tbl_info,
								 regpar clconnect *connect);
global_proc	void *no_table(clenv *env, regpar const char *tab_name);
global_proc void *complete_the_action(regpar clconnect *ccb, regpar void *response, regpar luw_flag luw);
global_proc	clconnect *find_coordinator( clenv *env );
global_proc void remove_clcontexts (regpar clconnect *ccb);
global_proc clconnect *find_ccb(clenv *env, regpar char *service_name);
global_proc	clconnect	*new_clconnect (clenv *env, char *server_id);
global_proc void	remove_clconnect (regpar clconnect *connect);
global_proc	bool	remove_cltab(regpar client_table *cltab);
global_proc	void	end_clluw( clenv *env );
global_proc bool two_phase_commit(regpar clconnect *coordinator);
global_proc short	close_clconnect(regpar clconnect *ccb);
global_proc	void	clseterr(clenv *env, int error_no, const char *msg);
#ifdef	CMXBUF_H
global_proc int clunpack(cmbuffer *bp, addr *cloutargs, pos code);
global_proc int clpack(cmbuffer *bp, addr *cloutargs, pos code);
#endif
int Cds_Remote_Cancel_Scan(clconnect *ccb, void *remote_cntxt);
client_table *cltab_list (clenv *env, client_table *cltab, const char *server_id);
bool	cdsInitCcb (clconnect *ccb,	char *cds_host,	long dict_time, long dict_hash,	short retry);
#endif

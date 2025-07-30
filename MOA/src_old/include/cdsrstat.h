extern	short Run_State;
extern	boolchar Sv_running;	/* set to false to terminate server */
#define INIT_STATE 0
#define TABLE_OPENING 1
#define INDEX_BUILDING 2
#define TABLE_COMPRESSING 3
#define TABLE_ARCHIVING 4
#define SHADOW_PROCESSING 5
#define TRANSACTION_PROCESSING 6

char *cds_run_state_name(void);
void respond_if_event(void);

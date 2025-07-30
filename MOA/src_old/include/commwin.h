
// this is the stuff needed by top level WndProc for process using
// cslib or any of the NOOSE comm libraries ie LUA CTC CPIC X25 etc 

#define WMU_PPCOMMBASE		(WM_USER+6666)		// seems good to me

// This section is used by our PPLUS/Z80 ACI comm stuff non NOOSE
#define WMU_PPCOMMEVENT 	WMU_PPCOMMBASE+0		// Winsock Comm event
#define WMU_PPCOMMREL   	WMU_PPCOMMBASE+1		// dummy event for releasing events
#define WMU_PPCRTEVENT 		WMU_PPCOMMBASE+2		// PP CRT event
#define WMU_PPCOMMSTATUS	WMU_PPCOMMBASE+3		// drive status msg

#define WMU_LUA_EVENT		WMU_PPCOMMBASE+100		//LUA message

#define WMU_CTC_EVENT		WMU_PPCOMMBASE+200		// CTC unisys message

#define WMU_CPIC_EVENT		WMU_PPCOMMBASE+300		// CPIC message

#define WMU_X25_EVENT		WMU_PPCOMMBASE+400		// X25 message

extern long comm_pplus_event(void *hwnd,unsigned int message,unsigned int wParam,long lParam);
extern long comm_lua_event(void *hwnd,unsigned int message,unsigned int wParam,long lParam);
extern long comm_ctc_event(void *hwnd,unsigned int message,unsigned int wParam,long lParam);
extern long comm_cpic_event(void *hwnd,unsigned int message,unsigned int wParam,long lParam);
extern long comm_x25_event(void *hwnd,unsigned int message,unsigned int wParam,long lParam);

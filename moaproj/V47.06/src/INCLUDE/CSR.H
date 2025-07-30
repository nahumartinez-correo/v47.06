// ==========================================================================
//  This file contains various #define symbols used by Custom C subroutines
//  as distributed with 'mosaic OA'.
//
//  You may add your own pre-processor code here, but we suggest you do so
//  at the end of the file so that in subsequent releases you can simply
//  cut your additions from this file and append them to the new file.
// ==========================================================================

	// ------------------------
	// Function codes for TAG.C
	// ------------------------
#define TAG_REDRAW		1
#define TAG_REDRAW_LIST	2
#define TAG_STATUS		3
#define TAG_REDRAW_ALL	4
#define TAG_SAVE_LIST	5
#define TAG_GET_MAX_BUTS	6
#define REDRAW_TAG_LIST	7
#define TAG_SAVE	0
#define TAG_RESTORE	1

	// --------------------------
	// Function codes for DT_TRAN
	// --------------------------
#define DT_TRAN_INIT        1
#define DT_TRAN_START       2
#define DT_TRAN_RESTART     3
#define DT_TRAN_END         4
#define DT_TRAN_RESET       5
#define DT_TRAN_SAVE_CURFLD 6
#define DT_FLD				101
#define DT_STACK			102
#define DT_PIC				104

	// --------------------------
	// Function codes for create_process
	// --------------------------
#define CSR_WAIT 	1	// wait for process to complete then return
#define CSR_NOWAIT 	0	// don't wait for completion

// window_state flags for create_process (taken from winuser.h)
/*
 * ShowWindow() Commands
 */
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_MAX              10

	// --------------------------
	// Function codes for print_utility
	// --------------------------
#define PUT_DESKTOP 	1
#define PUT_WINDOW 	2

#define PUT_BESTFIT 	1
#define PUT_STRETCHED 	2
#define PUT_SCALED 	3

// ================= add Customer specific stuff after this line ============
#define WM_SYSCOMMAND 0x0112
#define SC_RESTORE  0xF120

	// ----------------------------
	// Function codes for PROC_LIST
	// ----------------------------
#define F_COPY          120
#define F_CPTODUP       130
#define F_COMPARE       140
#define F_SWAP          150
#define F_CPFROMDUP     160
#define F_CLEAR         170
#define F_CLEARARRAY    180
#define F_REFRESH       190
#define F_PROTECT       200
#define F_UNPROTECT     210
#define F_EMPTY         220
#define F_DATAPRESENT   230
#define F_HIDEFIELD     240
#define F_SHOWFIELD     250
#define F_CURSCR	    260
#define F_APPEND	    270
#define F_GETFIELDS_LIST	280
#define F_SETVALDONE	290
#define F_HIGHLITE	    300
#define F_NOHIGHLITE	310
#define F_CLEARALL  	320
#define F_PROTECTALL    330
#define F_UNPROTECTALL  340
#define F_REFRESHALL    350      
#define F_HIDEFIELDALL  360
#define F_SHOWFIELDALL  370      
#define F_SETVALDONEALL 380
#define F_HIGHLITEALL   390
#define F_NOHIGHLITEALL 400



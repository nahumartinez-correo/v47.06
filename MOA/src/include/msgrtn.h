// ====================================
//
// Message Routine Library Header File
//
// ====================================

#ifndef MSGRTN_H
#define MSGRTN_H

// ================
// General defines
// ================
#define MR_INFO			(0)		// informational msg type
#define MR_WARN			(1)		// warning msg type
#define MR_ERROR		(2)		// error msg type
#define MR_NUMTYPES		(3)		// number of msg types
#define MR_STDOUT		(101)	// direct msg to stdout
#define MR_STDERR		(102)	// direct msg to stderr
#define MR_WINDOW		(103)	// display msg in MessageBox() WDW
#define MR_FILE			(104)	// direct msg to file

// ==================================================================
// Values returned from the mr?() family of functions
//
// Values greater than 0 indicate that a MessageBox() function
// was invoked and the button identity is returned as one of the set
// {1, 2, 3} respectfully.
// ==================================================================
#define MR_NOWINDOW		(0)		// no return value expected for non-WDW msg
#define MR_MASKFAIL		(-1)	// did not pass mask for this msg type
#define MR_NODLL		(-2)	// unable to load DLL, invalid path or name
#define MR_NOPROCADDR	(-3)	// unable to resolve function name in DLL
#define MR_BADHANDLE	(-4)	// bad window or file handle

// ============================================
// Mesage Routine Library Structure definition
// ============================================
typedef struct MRS {
	int			mask;
	int			where;
	int			style;	
	const char	*title;
	HANDLE		hmrl;
	int 		(*func)(struct MRS *, const int, const char *, va_list);
	} mrs;

// ===============================================================
// Message Routine Library interface routine function prototypes.
// ===============================================================
void	msgrtn_init(int);
int		mri(int keymask, const char *str, ...);
int		mrw(int keymask, const char *str, ...);
int		mre(int keymask, const char *str, ...);
void	get_mri(mrs *mrp);
void	get_mrw(mrs *mrp);
void	get_mre(mrs *mrp);
int		set_mri(mrs *mrp, const char *dllname, const char *procname);
int		set_mrw(mrs *mrp, const char *dllname, const char *procname);
int		set_mre(mrs *mrp, const char *dllname, const char *procname);

#endif

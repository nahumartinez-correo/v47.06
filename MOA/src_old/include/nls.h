// ====================================
//
// National Language Support Header File
//
// ====================================

#ifndef NLS_H
#define NLS_H

// =============
// D E F I N E S
// =============
#define NLS_DLL_OPEN_ERROR   	"Can't Open the Resource DLL: %s\nConsult the Mosaic RunTime System Reference Manual\nfor Localization Instructions."
#define NLS_LOADSTRING_ERROR	"Resource %d Not Found in %s\nConsult the Mosaic RunTime System Reference Manual\nfor Localization Instructions."

// These defines are used by RT_keyboard_mode set by INIT 69
#define DisableIME 0	// IME is disabled in WM_GETFOCUS event
#define LATIN  2		// Default
#define EnableIME  2	// IME controlled by user
#define AutoIME    4	// IME followes field.AutoIME flag

// These defines are used by the IME routines in CSR/ime.h
// 
#define WM_CONVERTREQUESTEX             0x0109  // DBCS
#define WM_CONVERTREQUEST               0x010A  // DBCS
#define WM_CONVERTRESULT                0x010B  // DBCS
#define WM_INTERIM                      0x010C  // DBCS
#define IME_WINDOWUPDATE          0x16

// ===============
// T Y P E D E F S
// ===============
typedef int (*IMEPROC)(HWND,int);
typedef LRESULT (CALLBACK* IMEWNDPROC)(HWND,UINT,WPARAM,LPARAM);

// =======================================
// F U N C T I O N S   P R O T O T Y P E S
// =======================================
//  Prototypes are in rtprotos.h

// =============================
// D A T A   S T R U C T U R E S
// =============================
#endif

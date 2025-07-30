
#define  VERSION    "1.00.0000"
#define  BUILDDATE  "July 14, 1994"

#ifdef DEBUG
#define  STFABOUTMSG  "mosaic OA Setup for Windows NT\n\nJune 30, 1993\nVersion 2.20.0000 - Debug\n\nCopyright (C) 1991, 1993 Olivetti NA ."
#else  /* !DEBUG */
#define  STFABOUTMSG  "mosaic OA Setup for Windows NT\nVersion 1.0\n\nCopyright (C) 1994, 1996 Olivetti NA ."
#endif /* !DEBUG */



#ifdef RC_INVOKED

#if defined(WIN16)
#include <ver.h>
#else 
#include <winver.h>
#endif

#define VER_FILEVERSION_STR      "2.2\0"
#define VER_FILEVERSION          2,20,0,0

#define VER_PRODUCTNAME_STR      "mosaic OA Setup for Windows NT\0"
#define VER_COMPANYNAME_STR      "Olivetti NA \0"
#define VER_LEGALTRADEMARKS_STR  "mosaic OA\256 is a registered trademark of Olivetti NA . Windows NT(TM) is a trademark of Olivetti NA \0"
#define VER_LEGALCOPYRIGHT_STR   "Copyright \251 Olivetti NA Corp. 1994, 1996\0"
#define VER_PRODUCTVERSION_STR   "2.1\0"
#define VER_PRODUCTVERSION       2,10,0,45
#define VER_COMMENT_STR          "Windows NT Setup Toolkit (Poof)\0"
#define VER_FILETYPE             VFT_DLL
#define VER_FILESUBTYPE          0
#define VER_FILEFLAGSMASK        VS_FFI_FILEFLAGSMASK
#define VER_FILEFLAGS            0L
#define VER_FILEOS               VOS_NT_WINDOWS32

#endif /* RC_INVOKED */

//-----------------------------------------------------------------------------
//
// UBRAM.H  Ubr/M Access Manager Include File Version 2.1
//
//-----------------------------------------------------------------------------
#define     UBRMVERSION             0x0210

//-----------------------------------------------------------------------------
// Constants
//
#define  RC_OUT_OF_SEQUENCE 0x8321
#define  RC_BAD_PARAMETERS  0x8421
#define  RC_UNKNOWN_COMMAND 0x8121
#define  RC_INTERNAL_ERROR  0x8621

#define  PEEKMESSAGE  0
#define  GETMESSAGE   1

#define  LL_ERROR00   0
#define  LL_ERROR02   2
#define  LL_ERROR03   3
#define  LL_ERROR05   5
#define  LL_ERROR06   6
#define  LL_ERROR08   8
#define  LL_ERROR10  10
#define  LL_ERROR11  11
#define  LL_ERROR12  12
#define  LL_ERROR13  13
#define  LL_ERROR14  14
#define  LL_ERROR15  15
#define  LL_ERROR16  16
#define  LL_ERROR19  19
#define  LL_ERROR20  20
#define  LL_ERROR21  21

#define  ID_UBRINI              LL_ERROR21+1
#define  ID_PROTWND             LL_ERROR21+2
#define  ID_UNINSTALLED_DEVICE  LL_ERROR21+3
#define  ID_UNKNOWN_REQUEST     LL_ERROR21+4
#define  ID_LISTFULL            LL_ERROR21+5
#define  ID_UNABLETOFREELEVEL1  LL_ERROR21+6

#define  IDS_INIELOGSECTENABLE  LL_ERROR21+7
#define  IDS_INIELOGNAME        LL_ERROR21+8
#define  IDS_INIELOGFOV         LL_ERROR21+9
#define  ID_ERRWRONGVER         LL_ERROR21+10

#define SIZECLASSNAME       26
#define SIZEMODULENAME      80
#define MAXTASKCOUNT         8
#define MAXDEVICE            5
#define MAXPORT              2
#define MAXSUBPROT           2
#define MAXENTRYSTRING      80
#define MAXCLASSNAME        80

#define ALREADY_WAITING      0
#define LIST_FULL            1
#define WAIT_FOR_PROCESS     2
#define NO_WAITING           3
#define LOCKED_BY_MYSELF     4
#define UNLOCKABLE_DEVICE    5

#define NOT_IN_LIST          6
#define ALREADY_IN_LIST      7

#define LEVEL1               0
#define LEVEL2               1

#define UBRMWIN              1
#define LOCK                 2
#define UNLOCK               3

#define NO_WAIT              0

//#define WRONGDRIVER         -1
#define BUSYDRIVER           1
#define USABLEDRIVER         0
#define FREEDRIVER           USABLEDRIVER         

#define PROC_GETVERSION      MAKEINTRESOURCE(9)
#define SEQWNDPROCORDINAL    MAKEINTRESOURCE(6)
#define SEQPARAMPROCORDINAL  MAKEINTRESOURCE(5)

#define UBRAPI 

//-----------------------------------------------------------------------------
// Types
//
typedef int (FAR PASCAL *ERRORLOG) (LPVOID);

typedef LPSTR  *LPLPSTR;
typedef LPWORD *LPLPWORD;

typedef BOOL (FAR * LPGETMSG) (MSG FAR*, HWND, UINT, UINT);

typedef struct tagDEVINFO {
    WORD        wDevice;
    char        szSeqClassName [SIZECLASSNAME];
    int         iSubProtocol;
    HINSTANCE   hSequencer;
    HINSTANCE   hProtocol;
//    WNDPROC     lpfnSeqWndProc;
    LPFNGETPARM lpfnSeqWndProc;
    LPFNGETPARM lpfnParamProc;
    HWND        hwndProtocol;
    HWND        hwndSequencer;
    WORD        wL2Index;
} DEVINFO, * LPDEVINFO;

typedef struct tagLISTITEM {
    HTASK   hTask;
    BYTE    bAPI;
    PARMINFO cdData;
} LISTITEM, *LPLISTITEM;

typedef struct _LOADPARMS {
    WORD      segEnv;                   // child environment
    LPSTR     lpszCmdLine;              // child command tail
    UINT FAR* lpShow;                   // how to show child
    UINT FAR* lpReserved;               // must be NULL
} LOADPARMS, *LPLOADPARMS;

//typedef MODULEENTRY *LPMODULEENTRY;

extern ERRORLOG  ErrorLogFunc;
extern HINSTANCE hAccessMan;


//---------------------------------------------------------------------------
// Prototypes
//
BOOL    GetDeviceProperties  (LPSTR, LPSTR, LPSTR, LPINT, LPWORD);
void    ErrorMsgBox          (int, LPSTR);
void    FreeAccessResources  (int);
BOOL    LoadSequencer        (LPSTR, int);
BOOL    LoadProtocol         (LPSTR, int);
                             
//---------------------------------------------------------------------------

int     IsDeviceInstalled    (BYTE);
int     WaitLockedDevice     (int);
int     WaitLockingDevice    (int);
int     WaitExecutingRequest (int);
int     CheckUnlockingDevice (int);
BOOL    AddToList            (int, BYTE, int, LPPARMINFO, LPLPWORD);
BOOL    CreateSEQWindow      (int);
int     IsTaskInTaskTable    (void);
void    DeleteFromLevel2     (int);
BOOL    DeleteFromTaskTable  (void);
BOOL    FreeLockList         (int);
BOOL    MoveToLevel2         (int, int);
void    SetReplyCode         (LPPARMINFO, LPWORD, WORD, WORD);
void    AddToTaskTable       (void);
                             
//---------------------------------------------------------------------------

LPSTR     GetInitializationString   (LPCSTR, LPCSTR, LPSTR, int);
LPLPSTR   GetInitializationSection  (LPCSTR, LPINT);
void      FreeInitializationSection (LPLPSTR);
void FAR *UbrAlloc                  (DWORD);
void      UbrFree                   (void FAR *);
// LPSTR     GetResourceString         (HINSTANCE, int);
void      MessageLoop               (BYTE, int);
// int       _fatoi                    (LPSTR);
HWND      GetProtocolWindow         (LPSTR);
void      UbrDebugExit              (int, LPBYTE, short);
                             
BOOL      HIEldInit                 (void);
HINSTANCE HIGetEldHinst             (void);


//---------------------------------------------------------------------------

int CALLBACK _WEP (int bSystemExit);
// int CALLBACK wienpr (int bSystemExit);

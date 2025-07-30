/******************************************************************************
*
*   EEHLLAPI.H - definitions / declarations for the Wall Data HllApi
*
* $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
* $Revision: 1.2 $
* $Date: 1996/03/08 19:27:52 $
*
*	(C) Wall Data Incorporated, 1994
*
*******************************************************************************/
#ifndef _EEHLLAPI_H_
#define _EEHLLAPI_H_

/*----------------------------------------------------------------------------*/
/* Wall Data Hllapi type definitions                                          */
/*----------------------------------------------------------------------------*/
#ifdef OS2_20
    typedef INT USHORT;
    typedef INT FAR *LPINT;
    typedef INT PID;
    typedef BYTE FAR *PSZ;
    typedef BYTE UCHAR;
    typedef DWORD   HWND_CLIENT;
#else
    typedef HWND    HWND_CLIENT;
#endif

//#ifndef WINVT
//typedef INT FAR *LPINT;
//#endif

/* Query Session structure */
typedef struct
{
    char    ShortName;      /* Session Shortname       */
    char    LongName[8];    /* Session LongName        */
    char    Type;           /* Session Type            */
    WORD    PSSize;         /* Presentation space size */
}   QuerySession[26];
typedef QuerySession FAR *LPQUERYSESSION;

/*----------------------------------------------------------------------------*/
/* SetSessionParameters definitions                                           */
/*----------------------------------------------------------------------------*/
#define SSP_NEWRET      (DWORD)0x00000001
#define SSP_OLDRET      (DWORD)0x00000002
#define SSP_ATTRB       (DWORD)0x00000004
#define SSP_NOATTRB     (DWORD)0x00000008
#define SSP_NWAIT       (DWORD)0x00000010
#define SSP_LWAIT       (DWORD)0x00000020
#define SSP_TWAIT       (DWORD)0x00000040
#define SSP_EAB         (DWORD)0x00000080
#define SSP_NOEAB       (DWORD)0x00000100
#define SSP_AUTORESET   (DWORD)0x00000200
#define SSP_NORESET     (DWORD)0x00000400
#define SSP_CONPHYS     (DWORD)0x00000800
#define SSP_SRCHALL     (DWORD)0x00001000
#define SSP_SRCHFROM    (DWORD)0x00002000
#define SSP_SRCHFRWD    (DWORD)0x00004000
#define SSP_SRCHBKWD    (DWORD)0x00008000
#define SSP_FPAUSE      (DWORD)0x00010000
#define SSP_IPAUSE      (DWORD)0x00020000
#define SSP_XLATE       (DWORD)0x00040000
#define SSP_NOXLATE     (DWORD)0x00080000
#define SSP_CONLOG      (DWORD)0x00100000
#define SSP_DEFESC      (DWORD)0x40000000
    // Items for dwParamEx. This allows us to duplicate values above.
#define SSP_WRSUPER     (DWORD)0x00000001
#define SSP_WRWRITE     (DWORD)0x00000002
#define SSP_WRREAD      (DWORD)0x00000004
#define SSP_WRNONE      (DWORD)0x00000008
#define SSP_SUPERWRITE  (DWORD)0x00000010
#define SSP_RDWRITE     (DWORD)0x00000020
#define SSP_QUIET       (DWORD)0x00000040
#define SSP_NOQUIET     (DWORD)0x00000080
    // new items should be added between these. Note the values.
#define SSP_NOSO        (DWORD)0x00100000
#define SSP_SO          (DWORD)0x00200000
#define SSP_SPACESO     (DWORD)0x00400000
#define SSP_EAD         (DWORD)0x00800000
#define SSP_NOEAD       (DWORD)0x01000000


/*----------------------------------------------------------------------------*/
/* Convert Row or Column flags                                                */
/*----------------------------------------------------------------------------*/
#define CONVERT_POSITION 'P'
#define CONVERT_ROW      'R'

/*----------------------------------------------------------------------------*/
/* Api Function Codes.                                                        */
/*----------------------------------------------------------------------------*/
#define OEMFUNCTION             0   /* OEM Function.                        */
#define CONNECTPS               1   /* Connect Presentation Space.          */
#define DISCONNECTPS            2   /* Disconnect Presentation Space.       */
#define SENDKEY                 3   /* Send Key.                            */
#define WAIT                    4   /* Wait.                                */
#define COPYPS                  5   /* Copy Presentation Space.             */
#define SEARCHPS                6   /* Search Presentation Space.           */
#define QUERYCURSORLOC          7   /* Query Cursor Location.               */
#define COPYPSTOSTR             8   /* Copy Presentation Space To String.   */
#define SETSESSIONPARAMETERS    9   /* Set Session Parameters.              */
#define QUERYSESSIONS           10  /* Query Sessions.                      */
#define RESERVE                 11  /* Reserve.                             */
#define RELEASE                 12  /* Release.                             */
#define COPYOIA                 13  /* Copy OIA Information.                */
#define QUERYFIELDATTRIBUTE     14  /* Query Field Attribute.               */
#define COPYSTRTOPS             15  /* Copy String To Presentation Space.   */
#define PAUSE                   18  /* Pause.                               */
#define QUERYSYSTEM             20  /* Query System.                        */
#define RESETSYSTEM             21  /* Reset System.                        */
#define QUERYSESSIONSTATUS      22  /* Query Session Status.                */
#define STARTHOSTNOTIFICATION   23  /* Start Host Notification.             */
#define QUERYHOSTUPDATE         24  /* Query Host Update.                   */
#define STOPHOSTNOTIFICATION    25  /* Stop Host Notification.              */
#define SEARCHFIELD             30  /* Search Field.                        */
#define FINDFIELDPOSITION       31  /* Find Field Position.                 */
#define FINDFIELDLENGTH         32  /* Find Field Length.                   */
#define COPYSTRINGTOFIELD       33  /* Copy String To Field.                */
#define COPYFIELDTOSTRING       34  /* Copy String To Field.                */
#define DISPLAYCURSOR           37  /* Display Cursor.                      */
#define DISPLAYPS               38  /* Display Presentation Space           */
#define DELETEPS                39  /* Delete Presentation Space            */
#define WDSETCURSOR             40  /* Set Cursor                           */
#define STARTKSINTERCEPT        50  /* Start Keystroke Intercept            */
#define GETKEY                  51  /* Get Key                              */
#define POSTINTERCEPTSTATUS     52  /* Post Intercept Status                */
#define STOPKSINTERCEPT         53  /* Stop  Keystroke Intercept            */
#define SENDFILE                90  /* Send File.                           */
#define RECEIVEFILE             91  /* Receive File.                        */
#define CONVERT                 99  /* Convert Position or RowCol           */
#define ISINITIALIZED          100  /* Get Rumba initialization status      */

#define CONNECTWINDOWSERVICES    101  /* Connect Window Services              */
#define DISCONNECTWINDOWSERVICES 102  /* Disconnect Window Services           */
#define QUERYWINDOWCOORDINATES   103  /* Query Window Coordinates             */
#define WINDOWSTATUS             104  /* Window Status                        */
#define CHANGELISTNAME           105  /* Change Switch List/Task List Name    */
#define CHANGEWINDOWNAME         106  /* Change Window Name                   */

/*----------------------------------------------------------------------------*/
/* Api Extended Function Codes.                                               */
/*----------------------------------------------------------------------------*/
#define WINEXEC               1000  /* Execute HllApi Application           */
#define SHOWSESSION           1001  /* Show HllApi Window                   */
#define GETSESSIONHWND        1002  /* Get HllApi Window Handle             */
#define ADVISEHOSTUPDATE      1003  /* Advise when Host Update occurs       */
#define RUNMACRO              1004  /* Run Rumba Macro                      */
#define SETSESSIONPARAMEX     1005  /* Set Session Parameters Extended      */

/*----------------------------------------------------------------------------*/
/* Api Return Codes.                                                          */
/*----------------------------------------------------------------------------*/
#define APIOK                   0   /* Successful.                          */
#define APINOTCONNECTED         1   /* Not Connected To Presentation Space. */
#define APIPARAMETERERROR       2   /* Parameter Error/Invalid Function.    */
#define APIFTXCOMPLETE          3   /* File Transfer Complete.              */
#define APIFTXSEGMENTED         4   /* File Transfer Complete / segmented   */
#define APIPSBUSY               4   /* Presentation Space is Busy           */
#define APIINHIBITED            5   /* Inhibited/Keyboard Locked.           */
#define APITRUNCATED            6   /* Data Truncated.                      */
#define APIPOSITIONERROR        7   /* Invalid Presentation Space Position. */
#define APINOTAVAILABLE         8   /* Unavailable Operation.               */
#define APISYSERROR             9   /* System Error.                        */
#define APINOTSUPPORTED         10  /* Function not supported.              */
#define APIUNAVAILABLE          11  /* Resource is unavailable.             */
#define APISESSIONSTOPPED       12  /* Session stopped.                     */
#define APIUNDEFINEDKEY         20  /* Undefined Key Combination            */
#define APIOIAUPDATE            21  /* OIA Updated.                         */
#define APIPSUPDATE             22  /* PS Updated.                          */
#define APIBOTHUPDATE           23  /* Both PS And OIA Updated.             */
#define APINOFIELD              24  /* No Such Field Found.                 */
#define APINOKEYSTROKES         25  /* No Keystrokes are available.         */
#define APIPSCHANGED            26  /* PS or OIA changed.                   */
#define APIFTXCANCELLED         27  /* Verbose mode FTX was cancelled       */
#define APIZEROLENFIELD         28  /* Field Length was Zero                */
#define APIINVALIDTYPE          30  /* Invalid Cursor Type.                 */
#define APIKEYOVERFLOW          31  /* Keystroke overflow.                  */
#define APIINVALIDPSID        9998  /* Invalid Presentation Space ID        */
#define APIINVALIDRC          9999  /* Invalid Row or Column Code           */

#define APIFILENOTFOUND        302  /* File Not Found.                      */
#define APIACCESSDENIED        305  /* Access Denied.                       */
#define APIMEMORY              308  /* Insufficient Memory.                 */

#ifdef WINVT
#define OIALENGTH              132
#endif // WINVT

#if 0
/*----------------------------------------------------------------------------*/
/* Api Tracing Function Prototypes                                            */
/*----------------------------------------------------------------------------*/
Moved to APITR.DLL (and renamed)
#endif

/*----------------------------------------------------------------------------*/
/* Api Function Prototypes                                                    */
/*----------------------------------------------------------------------------*/
INT WINAPI hllapi(short *, LPSTR, short *, short *);
INT WINAPI WD_ConnectPS(HWND_CLIENT, LPSTR);
INT WINAPI WD_DisconnectPS(HWND_CLIENT);
INT WINAPI WD_SendKey(HWND_CLIENT, LPSTR);
INT WINAPI WD_Wait(HWND_CLIENT);
INT WINAPI WD_CopyPS(HWND_CLIENT, LPSTR, INT);
INT WINAPI WD_SearchPS(HWND_CLIENT, LPINT, INT, LPSTR);
INT WINAPI WD_QueryCursorLocation(HWND_CLIENT, LPINT);
INT WINAPI WD_CopyPSToString(HWND_CLIENT, INT, LPSTR, INT);
INT WINAPI WD_SetSessionParameters(HWND_CLIENT, DWORD);
INT WINAPI WD_QuerySessions(HWND_CLIENT, LPINT, LPQUERYSESSION);
INT WINAPI WD_Reserve(HWND_CLIENT);
INT WINAPI WD_Release(HWND_CLIENT);
INT WINAPI WD_CopyOIA(HWND_CLIENT, LPSTR, INT);
INT WINAPI WD_QueryFieldAttribute(HWND_CLIENT, LPINT, INT);
INT WINAPI WD_CopyStringToPS(HWND_CLIENT, INT, LPSTR, INT);
INT WINAPI WD_Pause(HWND_CLIENT, INT);
INT WINAPI WD_QuerySystem(HWND_CLIENT, LPSTR);
INT WINAPI WD_ResetSystem(HWND_CLIENT);
INT WINAPI WD_QuerySessionStatus(HWND_CLIENT, LPSTR);
INT WINAPI WD_StartHostNotification(HWND_CLIENT, LPSTR);
INT WINAPI WD_QueryHostUpdate(HWND_CLIENT, LPSTR);
INT WINAPI WD_StopHostNotification(HWND_CLIENT, LPSTR);
INT WINAPI WD_SearchField(HWND_CLIENT, LPINT, INT, LPSTR);
INT WINAPI WD_FindFieldPosition(HWND_CLIENT, LPINT, INT, LPSTR);
INT WINAPI WD_FindFieldLength(HWND_CLIENT, LPINT, INT, LPSTR);
INT WINAPI WD_CopyStringToField(HWND_CLIENT, INT, LPSTR);
INT WINAPI WD_CopyFieldToString(HWND_CLIENT, INT, LPSTR, INT);
INT WINAPI WD_DisplayCursor(HWND_CLIENT, INT, LPSTR);
INT WINAPI WD_DisplayPS(HWND_CLIENT, LPSTR);
INT WINAPI WD_DeletePS(HWND_CLIENT, LPSTR);
INT WINAPI WD_SetCursor(HWND_CLIENT, INT);
INT WINAPI WD_StartKSIntercept(HWND_CLIENT, LPSTR);
INT WINAPI WD_GetKey(HWND_CLIENT, LPSTR);
INT WINAPI WD_PostInterceptStatus(HWND_CLIENT, LPSTR);
INT WINAPI WD_StopKSIntercept(HWND_CLIENT, LPSTR);
INT WINAPI WD_SendFile(HWND_CLIENT, LPSTR);
INT WINAPI WD_ReceiveFile(HWND_CLIENT, LPSTR);
INT WINAPI WD_Convert(HWND_CLIENT, INT, LPPOINT, LPSTR);

#ifdef OS2_20
USHORT WINAPI WD_ConnectWindowServices( PID, PSZ );
USHORT WINAPI WD_DisconnectWindowServices( PID, PSZ );
USHORT WINAPI WD_QueryWindowCoordinates( PID, PSZ, LONG FAR *, LONG FAR *, LONG FAR *, LONG FAR * );
USHORT WINAPI WD_WindowStatus( PID, PSZ, UCHAR, USHORT FAR *, USHORT FAR *, USHORT FAR *, USHORT FAR *, USHORT FAR *, LONG );
USHORT WINAPI WD_ChangeListName( PID, PSZ, UCHAR, PSZ );
USHORT WINAPI WD_ChangeWindowName( PID, PSZ, UCHAR, PSZ );
#endif

/*----------------------------------------------------------------------------*/
/* Api Wall Data Extended Function Prototypes                                 */
/*----------------------------------------------------------------------------*/
INT WINAPI WD_RunProfile(LPSTR, INT);
BOOL WINAPI WD_ShowSession(HWND_CLIENT, INT);
HWND_CLIENT WINAPI WD_GetSessionHWND(HWND_CLIENT);
INT WINAPI WD_AdviseHostUpdate(HWND_CLIENT, HWND_CLIENT, INT);
INT WINAPI WD_RunMacro(HWND_CLIENT, LPSTR);
INT WINAPI WD_SetSessionParamEx(HWND_CLIENT, LPSTR);

/*--------------------------------------------------------------------------*/
/* Register Message Strings.                                                */
/*--------------------------------------------------------------------------*/
#define API_SESSION_ACTIVATE    "ApiSessionActivate"

/*--------------------------------------------------------------------------*/
/* Get task ID                                                              */
/*--------------------------------------------------------------------------*/
#ifdef WIN32
#define GetCurrentTask()        (HANDLE)GetCurrentThreadId()
#endif

/*----------------------------------------------------------------------------*/
#endif

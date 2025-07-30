/* $Revision: 1 $ $Date: 10/31/95 11:40a $
 * $Locker: $
 */

//---------------------------------------------------------------------------
//
//  UBRMAMS.H
// 
//  Access Manager - Sequencers Interface Include File
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macros
//
//#define MAKEWORD(low,high)  (((WORD)(low)) | (((WORD)(high)) << 8))


// struttura di interfaccia numero di parametri del cmd e presenza RC
typedef struct tagPARMINFO {
    WORD wCmd;
    WORD wStatus;	   		// Special device status
    BYTE cbParam;
    BYTE    fReadInProgress;
    BYTE    bType;          // transmission type
    BYTE    bChannel;       // channel
    LPBYTE  lpbTx;          // transmission buffer pointer
    DWORD    wcTx;          // transmission bytes number
    LPBYTE  lpbRx;          // reception buffer pointer
    DWORD    wcRx;          // reception buffer size
    HWND    hwndSequencer;  // sequencer window handle 
    LPWORD  lpPar1;	    	// Data Buffer
    LPWORD  lpPar2;	    	// Data Buffer Count
    LPWORD  lpPar3;
    LPWORD  lpPar4;
    LPWORD  lpPar5;
    LPWORD  lpPar6;
    BOOL fRC;
    BOOL fNoWaitRead_Active;
    BOOL fNoWaitRead_InProgress;
    BOOL fCancel_NoWaitRead;

} PARMINFO, *LPPARMINFO;


// funzione determinazione parametri UbrWin  (AMS)
typedef BOOL (* LPFNGETPARM) (LPPARMINFO);
typedef WORD (* LPFNGETVER)  (void);

/*
// elemento dati del comando
typedef struct tagCMDDATA {
    WORD    wCmd;
    HWND    hwndProtocol;
    int     iChannel;
    LPWORD  lpPar1;
    LPWORD  lpPar2;
    LPWORD  lpPar3;
    LPWORD  lpPar4;
    LPWORD  lpPar5;
    LPWORD  lpPar6;
    LPWORD  lpPar7;
    LPWORD  lpPar8;
    LPWORD  lpPar9;
    LPWORD  lpPar10;
    BOOL    fRC;
} CMDDATA, *LPCMDDATA;
*/


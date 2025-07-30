/*
** $Name: R1V4C13 $
** $Revision: 1.9 $
** $Date: 1999/03/11 08:29:27 $
*/

//---------------------------------------------------------------------------
// Mael - OLIVETTI
// 
// UBR for Windows.
// 
// Access Manager, DLL di interfaccia verso le applicazioni.
// 
// 3 Marzo 1994, prima stesura
// Santino Troiani
// 
// 13 Gennaio 1995      Piero Taliani
//      Prevista la gestione di due periferiche con una unica dll di
//      sequencer

// 8 June 1995	Frank Albrecht
//	Converted to a csr for Mosaic OA
// 
//---------------------------------------------------------------------------

#define STRICT
#define UBM_ITSELF		// to make <ubm.h> include differently

#include "pch.h"

#include <typedefs.h>
#include <convers.h>
#include <defines.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <csrdef.h>			// Must be *AFTER* <rtdef.h>
#include <ubr.h>
#include <ubrmspl.h>
#include <ubrmams.h>
#include <ubram.h>
#define DEBUG 1
#ifdef DEBUG
#define UBRM_LOG(L,X) printf X;
//#define UBRM_LOG(L,X) if (L >= Gverbose) printf X;
#else
#define UBRM_LOG(L,X)
#endif

static int
is_not_INT_field(struct RTOBJ *, field **);
//-----------------------------------------------------------------------------
// Variabili globali
//
// Puntatore alla Tabella dei Task (lista di livello 0); 
// La lista, allocata dinamicamente al caricamento di Access Manager, consta di
// un array di MAXTASKCOUNT elementi di tipo HTASK.
HTASK *lpTaskTable;
//
// Puntatore alla Tabella di Accesso;
// La tabella, allocata dinamicamente al caricamento di Access Manager, consta 
// un array di elementi di tipo DEVINFO in numero pari al numero di periferiche
// installate, così come risulta dalla sezione [DEVICES] di UBRM.INI.
LPDEVINFO  lptblDevInfo;            
//
// Puntatori alle liste di Livello 1 e Livello 2;
// La lista di Livello 1, Lista di Acquisizione Esclusiva, viene allocata 
// dinamicamente al caricamento di Acces Manager e consta di un array di tanti
// elementi quante sono le periferiche installate; questi elememti sono, a loro
// volta, array di MAXTASKCOUNT elementi di tipo LISTITEM.
//
// La lista di Livello 2, Lista di Serializzazione Comandi, viene allocata 
// dinamicamente al caricamento di Acces Manager e consta di un array di tanti
// elementi quanti sono i canali di comunicazione; questi elememti sono, a loro
// volta, array di MAXTASKCOUNT elementi di tipo LISTITEM.
LPLISTITEM lpLev1List, lpLev2List;
//
// Nomi degli eseguibili del generico Sequencer e Protocol Layer
char szProtName [SIZEMODULENAME], szSeqName [SIZEMODULENAME];
//
// Numero di periferiche installate
int CntDevice = -1;
//
// Instanza di Access Manager
//HINSTANCE hAccessMan;
//
// Variabili di appoggio
LPLPSTR lplpDeviceGroup;
char szUbrIni [13];

WORD      CntTx         = 0;
char TRData[4000];
// gestione cursore
HCURSOR hcurSave = NULL;
BOOLEAN ubrminit(void);

static char szfBoxTitle [] = "ACCESS MANAGER FATAL ERROR";


//-----------------------------------------------------------------------------
// WEP
// 
// Exit point di Access Manager.
//
// Termina l'esecuzione dei Protocol Layer.
//-----------------------------------------------------------------------------


// #pragma alloc_text(FIXEDSEG, WEP)


int CALLBACK WEP (int bSystemExit)
{

    return 1;
}

//-----------------------------------------------------------------------------
// UBRMWIN
//
// Funzone usata dalle applicazioni per ottenere i servizi offerti dalle 
// periferiche.
//
// - Se la periferica non è installata termina con errore
// - Determina il numero dei suoi parametri e li registra
// - Se il task richiedente ha una richiesta in corso termina con errore
// - Se la periferica richiesta è in uso esclusivo di altri accoda il task
//          richiedente in livello 0 e 1
// - Accoda il task richedente in livello 0 e 2
// - Attende che il task richiedente arrivi in testa al livello 2
// - Avvia il Sequencer interessato ed attende la fine del servizio
// - Decoda il task richiedente dal livello 0 e 2
// - Se richiesto, attiva Error Logging
//-----------------------------------------------------------------------------

HINSTANCE   hInst;

/* ------------------------------------------ */
/*  This is the entry point from PCL code...  */
/* ------------------------------------------ */
XDLL int
ubrmwin(int argc, struct RTOBJ *argv[])
{
    static int		vf201fd = -1;
    static int iDev_nowait;
    int				rcmd, rdev, iDev;
    field			*fpsx, *fptxrx;
// FROM ubrm.c
    static PARMINFO pi, pi_nowait;

    UBRM_LOG(0, ("ubrmwin(), argc = %d\n", argc));
    if (argc < 2) {
		return(CSR_FAIL_ARGCOUNT);
    }
    rtobj2int(argv[1], &rcmd);
    rtobj2int(argv[0], &rdev);
    if (argc > 2)
    {
    	if (is_not_INT_field(argv[2], &fpsx)) {
	    return(CSR_FAIL_ARGTYPE);
	}
    }
    if (argc == 4)	// if fourth argument (transmit/receive buffer pointer)
	fptxrx = (field *) argv[3]->ptr;
    UBRM_LOG(0, ("\nubrmwin(), dev = %d cmd = %d\n", rdev, rcmd));
    pi.lpbTx = (LPBYTE)(TRData + 1);

    // l'area di ricezione punta a SIZETXRXBUFFER bytes da quella di tx
    pi.lpbRx = pi.lpbTx + SIZETXRXBUFFER;

     // dimensione dell' area di ricezione
     pi.wcRx  = SIZETXRXBUFFER;


    if (CntDevice == -1)	// Needs initializing.
	ubrminit();

    if ((iDev = IsDeviceInstalled ((BYTE) rdev)) == -1) {
        ErrorMsgBox (ID_UNINSTALLED_DEVICE, NULL);
        return 0xFF;
    }    

    pi.wCmd = MAKEWORD (rcmd, rdev);
    

		
/* Need to know more about the serial printer before this is required
    if (rdev == BADGEREADER)
    {
*/
        if (argc == 4)
        {
    	    pi.lpPar1 = (WORD *)fptxrx->storage;
    	    pi.lpPar2 = &fptxrx->numchars;
        }
        else
        {
            pi.lpPar1 = NULL;
            pi.lpPar2 = NULL;
        }
    if (pi_nowait.fNoWaitRead_Active == TRUE)
    {
		pi_nowait.fCancel_NoWaitRead = TRUE;
		while (pi_nowait.fNoWaitRead_Active == TRUE)	Sleep(500);
	}
    if (rdev == BADGEREADER && (rcmd & NOWAIT_READ) == NOWAIT_READ)
    {
		pi.fNoWaitRead_Active = TRUE;
		pi.fNoWaitRead_InProgress = TRUE;
		MoveMemory(&pi_nowait, &pi, sizeof(PARMINFO));
		iDev_nowait = iDev;
    		if (lptblDevInfo [iDev_nowait].lpfnParamProc (&pi_nowait) == FALSE) {
    	    		ErrorMsgBox (ID_UNKNOWN_REQUEST, NULL);
    	    		return 0xFF;
    		}
    		if (lptblDevInfo [iDev_nowait].lpfnSeqWndProc (&pi_nowait) == FALSE) {
        		ErrorMsgBox (ID_UNKNOWN_REQUEST, NULL);
        		return 0xFF;
    		}
    		return 1;
	}
/* Need to know more about the serial printer before this is required
    }
    else if (rdev == SERIALPRINTER)
    {

// Serial printer
	switch(rcmd)
	{
		case PRINTST:
		case CUTST:
		case GETSTST:
		case CKRCTR:
		case GETINTST:

		case SETDOTFEED:
		case SETDEFFONT:
		case LDGRAPHPAGE:
		case LDFONT:
		case PRINTBC:
		case PRINTGT:
		case PRINTGP:
		case LDLOGO:

		case SETVMODE:
		case SETHMODE:
		case SETFONTTYPE:
		case LINEFEED:
		case STEPPER:
		case SETVTAB:
		case SETFONT:
		case PRINTLOGO:
		case PAPERCUT:
		default:
			return(CSR_FAIL_ARGCOUNT);
	}



    }
*/
	if (pi_nowait.fNoWaitRead_Active == FALSE)
    {
    	if (lptblDevInfo [iDev].lpfnParamProc (&pi) == FALSE) {
    	    ErrorMsgBox (ID_UNKNOWN_REQUEST, NULL);
    	    return 0xFF;
    	}
    	if (lptblDevInfo [iDev].lpfnSeqWndProc (&pi) == FALSE) {
        	ErrorMsgBox (ID_UNKNOWN_REQUEST, NULL);
        	return 0xFF;
    	}
    	if (argc > 2)
        	int2field(pi.wStatus, fpsx);
//    	return 1;
	}
	if (pi_nowait.fNoWaitRead_InProgress == TRUE && pi_nowait.fNoWaitRead_Active == FALSE)
	{
		pi_nowait.fNoWaitRead_Active = TRUE;
    	if (lptblDevInfo [iDev_nowait].lpfnParamProc (&pi_nowait) == FALSE) {
    	    ErrorMsgBox (ID_UNKNOWN_REQUEST, NULL);
    	    return 0xFF;
    	}
    	if (lptblDevInfo [iDev_nowait].lpfnSeqWndProc (&pi_nowait) == FALSE) {
        	ErrorMsgBox (ID_UNKNOWN_REQUEST, NULL);
        	return 0xFF;
    	}
    	return 1;
	}
	return RT_SUCCESS;
}

BOOL 
GetDeviceProperties (LPSTR lpDeviceName, LPSTR lpSequencerName, 
            LPSTR lpProtocolName, LPINT lpSubProtocol, LPWORD lpDeviceCode)
{
    LPSTR lpszTemp;
    WORD wAppo = 0;
    LPSTR p = (LPSTR) malloc ((WORD) MAXENTRYSTRING);

    if ((p = GetInitializationString ("DEVICES", lpDeviceName,
                                    p, MAXENTRYSTRING)) == NULL) {
        free (p);
        return FALSE;
    }

//    *lpDeviceCode = GetDeviceCode (p);
    if (lpszTemp = strrchr (p, (int) ',')) {
        *lpszTemp = 0;
        lpszTemp++;
        wAppo = (WORD) atoi (lpszTemp);
    }

    *lpDeviceCode = (WORD) ((wAppo << 8) | ((WORD) atoi (p)));

    if ((p = GetInitializationString (lpDeviceName, "SEQUENCER",
                                    p, MAXENTRYSTRING)) == NULL) {
        free (p);
        return FALSE;
    }

    lstrcpy (lpSequencerName, p);

    if (lpszTemp = strrchr (lpSequencerName, (int) '.'))
        *lpszTemp = 0x00;

    lstrcat (lpSequencerName, (LPSTR)".DLL");

    if ((p = GetInitializationString (lpDeviceName, "PROTOCOL",
                                    p, MAXENTRYSTRING)) == NULL) {
        free (p);
        return FALSE;
    }

    if ((p = GetInitializationString (lpDeviceName, "MPP2CHANNEL",
                                    p, MAXENTRYSTRING)) == NULL)
        * lpSubProtocol = 0;
    else
        * lpSubProtocol = atoi (p);

    free (p);

    return TRUE;
}


void ErrorMsgBox (int iExitCode, LPSTR lpszMsg)
{
    static char szString [256];
	
  	sprintf(szString, "ExitCode = %d", iExitCode);


    MessageBox (GetActiveWindow (), (LPSTR) szString,
                (LPSTR) szfBoxTitle,
                /*MB_ICONSTOP | */MB_SYSTEMMODAL | MB_OK);

    return;
}


void FreeAccessResources (int cnDev)
{                            
    int i;
    
    // rilascio l'area usata per leggere la sezione [DEVICES] di UBRM.INI
    if (lplpDeviceGroup)
        FreeInitializationSection (lplpDeviceGroup);
    
    // rilascio l'area usata per la Tabella dei Task
    if (lpTaskTable)
        free ((LPBYTE) lpTaskTable);
    
    // rilascio l'area usata per la Tabella di Accesso
    if (lptblDevInfo) {
    
        for (i = 0; i < cnDev; i++) {                    
        
            if (lptblDevInfo [i].hSequencer) {
                FreeModule  (lptblDevInfo [i].hSequencer);
                lptblDevInfo [i].hSequencer = NULL;
            }
        
            if (lptblDevInfo [i].hProtocol)
                PostMessage (lptblDevInfo [i].hwndProtocol, WM_CLOSE, 0, 0L);
                lptblDevInfo [i].hProtocol = NULL;
        }
        
        free ((LPBYTE) lptblDevInfo);
    }    

    // rilascio l'area usata per le liste di livello 1
    if (lpLev1List)
        free ((LPBYTE) lpLev1List);
        
    // rilascio l'area usata per le liste di livello 2
    if (lpLev2List)
        free ((LPBYTE) lpLev2List);

    // reload old cursor
    if (hcurSave)
        SetCursor (hcurSave);

    return;    
}


BOOL LoadSequencer (LPSTR lpszName, int index)
{
    LPFNGETPARM lpfnWndProc;
    LPFNGETPARM lpfnGetParam;
    char        szClass [MAXCLASSNAME];
    WNDCLASS    wc;
    LPSTR       lpszPunto;

    if ((hInst = 
        LoadLibrary (lpszName)) == 0) {
        ErrorMsgBox ((int) GetLastError(), lpszName);
        return FALSE;
    }

            
    if ((lpfnWndProc = (LPFNGETPARM) GetProcAddress (hInst, 
                                    (LPCSTR) SEQWNDPROCORDINAL)) == NULL)
        return FALSE;            
        
    if ((lpfnGetParam = (LPFNGETPARM) GetProcAddress (hInst, 
                                    (LPCSTR) SEQPARAMPROCORDINAL)) == NULL)
        return FALSE;            
        
    lstrcpy (szClass, "class");
    lstrcat (szClass, lpszName);
    
    if ((lpszPunto = strchr (szClass, '.')) != NULL)
        *lpszPunto = 0;


    if (GetClassInfo (hInst, (LPSTR) szClass, (LPWNDCLASS) &wc))
        UnregisterClass ((LPSTR) szClass, hInst);

    wc.style         = CS_VREDRAW | CS_HREDRAW | CS_GLOBALCLASS;
   // wc.lpfnWndProc   = lpfnWndProc;
    wc.lpfnWndProc   = (WNDPROC) GetProcAddress(hInst, (LPCSTR) SEQWNDPROCORDINAL);
    wc.cbClsExtra    = wc.cbWndExtra = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION); // HICON) 0;
    wc.hCursor       = LoadCursor (NULL, IDC_ARROW);     // (HCURSOR) 0;
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);      // 0;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = (LPSTR) szClass;

    if (!RegisterClass ((LPWNDCLASS) &wc))
        return FALSE;
        
    lptblDevInfo [index].hSequencer = hInst;
    lstrcpy (lptblDevInfo [index].szSeqClassName, szClass);
    
    lptblDevInfo [index].lpfnSeqWndProc = lpfnWndProc;
    lptblDevInfo [index].lpfnParamProc = lpfnGetParam;
    
    return TRUE;    
}
                  

//-----------------------------------------------------------------------------
// Funzioni di servizio usate in ubrmwin
//-----------------------------------------------------------------------------


int IsDeviceInstalled (BYTE bDevice)
{
    int i;
    
    for (i = 0; i < CntDevice; i++)
        if (LOBYTE (lptblDevInfo [i].wDevice) == bDevice ||
            HIBYTE (lptblDevInfo [i].wDevice) == bDevice)
            return i;

    return -1;
}

/*
BOOL CreateSEQWindow (int iDevice)
{
    LPLISTITEM lpList = lpLev2List +
                            MAXTASKCOUNT * lptblDevInfo [iDevice].wL2Index;
    char szBuffer [MAXCLASSNAME];
    
    lstrcpy (szBuffer, "wnd");
    lstrcat (szBuffer, lptblDevInfo [iDevice].szSeqClassName + 5);
#if _OUTDEBUG
    wsprintf (szBuffer, "Window Sequencer: %4X\n", 
                                        lptblDevInfo [iDevice].hwndSequencer);
    OutputDebugString (szBuffer);
#endif
      return TRUE;
}
     
*/

//---------------------------------------------------------------------------
// LPSTR *  GetInitializationSection (LPCSTR lpszSection, 
//                                          LPINT  lpCntKeys);
//
//    lpszSection, address of section
//    lpCntKeys,   number of entries in section
//
// Return the key names of a whole section of UBR.INI.
//
// N.B. Free memory allocated calling FreeInitializationSection ().
//---------------------------------------------------------------------------
LPSTR *  
GetInitializationSection (LPCSTR lpszSection, LPINT  lpCntKeys)
{
    int cbReturnBuffer;         // size of destination buffer
    LPSTR lpszReturnBuffer;     // address of destination buffer
    LPSTR * lpszReturn;     // address of return list of strings
    int iLen;
    LPSTR lpBuf;
    LPSTR lpszToken;

    *lpCntKeys = 0;
    cbReturnBuffer = 4096;
    
    if ((lpszReturnBuffer = (LPSTR) malloc (cbReturnBuffer)) == NULL)
        return NULL;

    if (* szUbrIni == 0)
        LoadString (hInst, ID_UBRINI, szUbrIni, sizeof szUbrIni);
    
    iLen = GetPrivateProfileString (lpszSection, (LPCSTR) NULL, (LPCSTR) "", 
                        lpszReturnBuffer, cbReturnBuffer, "ubrm.ini");
                        
    if (iLen == 0 || lpszReturnBuffer [0] == 0) {
        free (lpszReturnBuffer);
        return NULL;
    }
    
    for (lpszToken = lpszReturnBuffer; *lpszToken != 0; (*lpCntKeys)++)
        lpszToken += lstrlen (lpszToken) + 1;
    
    if ((lpBuf = (LPSTR) malloc (iLen + 1)) == NULL) {
        free (lpszReturnBuffer);
        return NULL;
    }
    
    MoveMemory (lpBuf, lpszReturnBuffer, iLen + 1);
    
    free (lpszReturnBuffer);
    
    if ((lpszReturn = (LPSTR *) malloc ((*lpCntKeys) * 
                                            sizeof (LPSTR))) == NULL) {
        free (lpBuf);
        return NULL;
    }
        
    for (lpszToken = lpBuf, *lpCntKeys = 0; *lpszToken != 0; (*lpCntKeys)++) {
        lpszReturn [*lpCntKeys] = lpszToken;
        lpszToken += lstrlen (lpszToken) + 1;
    }
    
    return lpszReturn;
}

//---------------------------------------------------------------------------
// VOID  FreeInitializationSection (LPSTR * lpSection);
//    lpSection, section key names
//
// Free memory allocated by GetInitializationSection ().
//---------------------------------------------------------------------------
void  FreeInitializationSection (LPLPSTR lplpSection)
{
    free (lplpSection [0]);
    free (lplpSection);
}


BOOLEAN ubrminit(void)
{
    int i;
    WORD l;
    int iSubProt;
    WORD wDevCode;
    LPSTR lpDeviceName;
    
    hcurSave = SetCursor (LoadCursor (NULL, IDC_WAIT));
        
    if ((lplpDeviceGroup = GetInitializationSection ("DEVICES", 
                                            (LPINT) &CntDevice)) == NULL)
        return 0;
                               
    if ((lpTaskTable = (HTASK *) malloc ((WORD) (MAXTASKCOUNT * 
                                            sizeof (HTASK)))) == NULL) {
        FreeAccessResources (CntDevice);
        return 0;                                   
    }

    if ((lptblDevInfo = (LPDEVINFO)
                malloc ((WORD) (CntDevice * sizeof (DEVINFO)))) == NULL) {
        FreeAccessResources (CntDevice);
        return 0;                                                          
    }
                        
    for (lpDeviceName = lplpDeviceGroup [i = 0]; 
                        lpDeviceName; lpDeviceName = lplpDeviceGroup [++i]) {
                                       
        if (GetDeviceProperties (lpDeviceName, szSeqName, 
                szProtName, (LPINT) &iSubProt, (LPWORD) &wDevCode) == FALSE)
            break;
            
        if (LoadSequencer (szSeqName, i) == FALSE)
            break;

        lptblDevInfo [i].wDevice = (WORD) wDevCode;
        lptblDevInfo [i].iSubProtocol = iSubProt;
    }                                                                   
    
    if (i < CntDevice) {
        FreeAccessResources (CntDevice);
        return 0;       
    }
    
    FreeInitializationSection (lplpDeviceGroup);
    lplpDeviceGroup = NULL;

    if ((lpLev1List = (LPLISTITEM) malloc (CntDevice * 
                            (DWORD)sizeof (LISTITEM) * MAXTASKCOUNT)) == NULL) {
        FreeAccessResources (CntDevice);
        return 0;
    }   
        
    for (i = 0, l = 0; i < CntDevice; i++)
        l = max (l, lptblDevInfo [i].wL2Index);
        
    if ((lpLev2List = (LPLISTITEM) malloc ((l + 1) * 
                            (DWORD)sizeof (LISTITEM) * MAXTASKCOUNT)) == NULL) {
        FreeAccessResources (CntDevice);
        return 0;
    }   
       
    SetCursor (hcurSave);

    return 1;
}

char szUbrIni [13];

//---------------------------------------------------------------------------
//  GetInitializationString
//  Return the string associated to the key name from UBR.INI.
//
//  N.B. Free of memory allocated will be caller responsability.
//---------------------------------------------------------------------------
LPSTR  
GetInitializationString (LPCSTR lpszSection, LPCSTR lpszEntry,
                                                LPSTR  lpszRet, int cbRet)
{
    int   cbReturnBuffer;           // size of destination buffer
    int   iLen;
    LPSTR lpszReturnBuffer;         // address of destination buffer
    LPSTR lpszReturn;               // address of return buffer

    cbReturnBuffer = max (cbRet, 256);

    if ((lpszReturnBuffer = (LPSTR) malloc ((WORD) cbReturnBuffer)) == NULL)
        return NULL;

    if (* szUbrIni == 0)
        LoadString (hInst, ID_UBRINI, szUbrIni, sizeof szUbrIni);

    iLen = GetPrivateProfileString (lpszSection, lpszEntry, (LPCSTR) "",
                        lpszReturnBuffer, cbReturnBuffer, "ubrm.ini");

    if (iLen == 0 || * lpszReturnBuffer == 0)
        lpszReturn = NULL;

    else {

        lpszReturn = lpszRet ? lpszRet :
                            (LPSTR) malloc ((WORD)(cbRet = iLen + 1));

        if (lpszReturn) {

            if (cbRet < iLen + 1) {
                lstrcpyn (lpszReturn, lpszReturnBuffer, cbRet - 1);
                lpszReturn [cbRet - 1] = 0;
            }
            else
                lstrcpy (lpszReturn, lpszReturnBuffer);
        }
    }

    free (lpszReturnBuffer);

    return lpszReturn;
}
static int
is_not_INT_field(struct RTOBJ *rp, field **fp)
{
	if (rp->obj.object != CSRARG_FLD) {
		return(CSR_FAIL_ARGTYPE);
	}
	*fp = (field *) rp->ptr;
	if ((*fp)->flags.type != FT_INT) {
		return(CSR_FAIL_ARGTYPE);
	}
	return(0);
}


#include <windows.h>
#include <winreg.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <ddeml.h>
#include <nddeapi.h>
#include <nddesec.h>
#include "setupapi.h"
#include "msdetect.h"
#include "msregdb.h"
#include "msshared.h"
#include <stdio.h>
#include "sample.h"
#include "dialogs.h"
#include "bldcui\dialogs.h"
#include <winver.h>
#include <malloc.h>
#include <process.h>
#include "..\setupcmn\setupini.h"
//#include <ctl3d.h>

// Global Data
#define cbSymBuf   1024
#define MAXLEN 256

#define INITTAB_FILE    "inittab.ini"
#define DEFAULT_STR     "Default" 
#define DEFAULT_MOA_DIRECTORY  "C:\\MOA\\"
#define DEFAULT_MOA_PROJECT_DIRECTORY "C:\\MOAPROJ\\"
// defines for printer types
#define DP3_PRINTER 0
#define PR50_PRINTER 1
// defines for COM ports
#define COM1 0
#define COM2 1
#define COM3 2
#define com4 3


int installType;
static int FileIndex = 0;
char    szInstallPath[cchMax],szWinDir[_MAX_PATH],szMachine[50] = "\0";
char    szMoaProjDir[_MAX_PATH];
char    szSrcDir[cchMax],szONPServerName[50],szOldServer[50];
SMDINFO smdInfo[50];
FILESINFO *FileInfo;
HANDLE  hInst;
char szServSearchStr[] = "MOA/AUTO-ADDED SERVICES";
WORD wCopyRunTimeFiles, wCopyDevFiles,wInstServices,wInstWincom,
    wInstSerial,wUpdatePath,wUpdateServ,wUpdatePrinter,wSetFonts;
WORD wInstAuxPrint, wInstRDateAt, wInstRDateInitTab, wInstRemServer,wPrinter = DP3_PRINTER,wComPort = COM1;
WORD wRemoveDirTree=0;
HANDLE hWnd;
BOOL bNetInstall = FALSE,bShutDown = FALSE, bWin95=FALSE;
HKEY hRootKey = HKEY_LOCAL_MACHINE;
char* pInfFileName = "moa.inf";
int iCustomSetup = 0;
int iVersion; 		// os version * 100

// Flags to specify whether the wInstallServices related checkbox
// has just been cleared or not.
#define ENABLE 1
#define DISABLE 0
BOOL bCheckBoxEnabled;
BOOL bUnInstall = FALSE;

//Function protptypes
BOOL bRunningWin95();
BOOL CreateBatchFiles();
BOOL CheckIfTCPInstalled();
void UninstallRDateAt ( void );
void InstallServices();
void InstallDrivers();
DWORD GetValueType (char *pValue);
void ShutDownComputer ();
void CopyDLLs();
void CheckServicesFile();
void CopyRTExe();
void DeleteServices();
BOOL FOnpServerNameDialog();
void ServicesDialog (char ch);
BOOL CALLBACK EnDisServicesButton ( HWND hwnd, LPARAM l );
void AddONPServerName (char * szFile,char *szTmpFile);
void SetPath (char * szPath,char ch);
void CopyLastMinuteFiles();
void UpdatePrinterInfo();
void StopServices();
BOOL InstallOptionsDialog(char);
void DeleteDrivers ();
VOID WelcomeUnInstall();
VOID AskUnInstallQuit();
VOID MyReadInfFile(LPSTR szFile);
char * StripOfLeadingSpaces (char *szLine);
void BuildInfo ( char *pInfo);
BOOL FUnInstallPathDialog();
void UnInstall();
void SetUnInstallText(char *pText);
void GetModelessDialog(HWND*);
void InstallFonts(char ch);
void UpdateServerName(char*,char*);
BOOL GetONPServerName(char*);
void InitNetInstall(char* szCommand);
char* CopyWord(char *pCommand,char *pRecieveBuffer);
void MyGetWindowsDirectory(char* pRecieveBuff,int iSize);
void SetAllOptions();
void CreateProgGroupOnRemote();
HDDEDATA CALLBACK DdeCallback(UINT,UINT,HCONV ,HSZ ,HSZ,HDDEDATA,DWORD,DWORD);
BOOL UpdateFontsInRegistry(char ch, int i);
char* GetRealPath(char* pNewPath, char* pPath);
void CopySharedDLL();
void AppendOverSizeFiles();
BOOL GetMagicPointers(char* pLine,char** pSection,char** pFileName,char** pOriName);
char* GetNextWord(char* pLine);
void FixFileName(char* pString);
void GetFileTimeStamp(char*,FILETIME*);
void SetFileTimeStamp(char*,FILETIME*);
BOOL FlipCheckBox ( LPSTR, int nButton );
void DumpServicesFlags ( void ) ;
void DumpGlobalFlags ( void ) ;
void DoService ( LPCSTR, LPSTR, LPCSTR ) ;
void UpdateInitTabFile ( void ) ;
void UninstallInitTabFile ( void ) ;
void Usage ( void ) ;
BOOL bCopyAnyFiles ( void ) ;
BOOL bCopyAllFiles ( void ) ;
BOOL bFileExists ( char * );
BOOL IsInterruptTwoOK(void);
void ErrorHandler(char* szBuff);
BOOL bSplitFileName ( CHAR szFileName[] );
CHAR *szReturnUnsplitName ( CHAR szFileName[] );
BOOL bZerothSplitFile ( CHAR szFileName[] );
VOID SaveSetupOptions(VOID);
BOOL bIsInstalled ( LPCSTR szSection );
void PrinterTypesDialog (char ch);
void RemoveDLLs();
VOID StripSlash(LPSTR sz1);
VOID AppendSlash(LPSTR sz1);
BOOL CopyPrinterDLLs();


// These headers are functions to perform the recursive directory tree deletion.
BOOL bRemoveTree ( char * szDir ) ;
HANDLE FindFirstDirectory(LPTSTR lpszSearchFile, LPWIN32_FIND_DATA lpffd) ;
BOOL bFindRealFile ( void ) ;
BOOL FindNextDirectory(HANDLE hSearch, LPWIN32_FIND_DATA lpffd) ;


// Array holding the names of services to be stopped before starting Installation
// Even driver names are included in this list. Drivers can also be started and
// stopped in the same way as that of Services
typedef struct  tagSERVICE {
    char* serviceName;
    char* exeName;
    WORD  wFlag;
} SERVICE;

// Following array is used by InstallServices to install services.
// InstallServcies function will install the service, if wFlag is set and
// contains valid exeName.i.e, if exeName is NULL service is not installed.

SERVICE services[] = { {"Auxprint","auxcli.exe",1},
                       {"RemServer","remsrvr.exe",1},
                       {"Inittab","inittab.exe",1},
                       {"wincom",NULL,1},
                       {"serial",NULL,1}
                     };

typedef struct  tagDRIVER {
    char* DriverKey;
    char* DriverName;
    char* DriverIni;
    char* DriverDir;
    WORD  wFlag;
} DRIVER;

// Array containing the  info about drivers
DRIVER DriverInfo[] = { {"wincom","wincom.sys","wincom.ini","SRC\\DRIVER\\FILES\\",1},
                        { "serial","serial.sys","serial.ini","SRC\\DRIVER\\FILES\\",1}
                      };
// Array containing fontnames, to install more fonts add names here,
// may be it's a good idea if there is no explicit reference to font names
// and it gets name of fonts from *.inf file.. a kludge for now
char *szFonts[] = { "ISC_BR1.FON","ISC_BR2.FON","ISC_BR3.FON","ISC_BR4.FON"};
char *szFontNames[] = { "ISC_BR1","ISC_BR2","ISC_BR3","ISC_BR4"};
CHAR *szSharedDLLs[] = { "\\system32\\ctl3d32.dll" };

char *szPrinterDLLs[] = {
							"olipr46.dll",
							"olipr50.dll",
							"windoc.dll",
							"winval.dll"
						};

/********************************************************************** */
VOID BuildLists()
{
    BuildCopyList();
    BuildBillboardList();
}


/* ********************************************************************* */
VOID BuildCopyList()
{
    ClearCopyList();

    if(bCopyAnyFiles())
    {
        AddSectionFilesToCopyList("Shared", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy shared/common files", "Setup Message", MB_OK );
        #endif
    }

    if (wCopyDevFiles)
    {
        AddSectionFilesToCopyList("Development files", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy dev files", "Setup Message", MB_OK );
        #endif
    }

    if (wCopyRunTimeFiles)
    {
        AddSectionFilesToCopyList("Runtime files", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy Runtime Files", "Setup Message", MB_OK );
        #endif
    }


	if (wInstSerial)
    {
		AddSectionFilesToCopyList("Serial Sharing", szSrcDir, szInstallPath);		
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy Serial Sharing Files", "Setup Message", MB_OK );
        #endif
    }

   	if (wSetFonts)
    {
		AddSectionFilesToCopyList("Fonts", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy fonts", "Setup Message", MB_OK );
        #endif
    }

	if ((wInstAuxPrint) || (wInstWincom))
    {
		AddSectionFilesToCopyList("Auxprint and Wincom", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy auxprint and wincom", "Setup Message", MB_OK );
        #endif
    }

	if (wInstRemServer)
    {
	 	AddSectionFilesToCopyList("Remote COM Server", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy RemServer", "Setup Message", MB_OK );
        #endif
    }


	// Note - if the sections for RDate running as an AT service, and RDate running
	// as an InitTab service ever differ, just modify the files file to contain the 
	// separate file lists, and break up this if-section into two chunks.
	if (wInstRDateAt || wInstRDateInitTab)
	{
	 	AddSectionFilesToCopyList("RDate", szSrcDir, szInstallPath);
        #ifdef DEBUG_LEVEL_2
        MessageBox ( NULL, "Will copy RDateAT", "Setup Message", MB_OK );
        #endif
    }
}


/* ********************************************************************* */
VOID BuildBillboardList()
{
    AddToBillboardList("tsetuph.dll", DLG_BOARD1, "FModelessDlgProc", 5);
    AddToBillboardList("tsetuph.dll", DLG_BOARD2, "FModelessDlgProc", 5);
    AddToBillboardList("tsetuph.dll", DLG_BOARD3, "FModelessDlgProc", 5);
    AddToBillboardList("tsetuph.dll", DLG_BOARD4, "FModelessDlgProc", 5);

    SetCopyGaugePosition(155, 130);
}


/* ************************************************************************/
VOID Install()
{
    CHAR rgchT[16],szBuff[256];
    INT  iCursorSav;
    INT iRet;

    iCursorSav = ShowWaitCursor();
    CopyDLLs ();
    StopServices();
    RestoreCursor(iCursorSav);

	// Only copy files if any of these flags is set.  Really, we should give the user 
	// an error message if this occurs.   
    if(bCopyAnyFiles() || wSetFonts)
	{
        CopyFilesInCopyList();
	}

    UIStartDlg("tsetuph.dll", DLG_REGINI, "FModelessDlgProc", 0, NULL,
            (LPSTR)rgchT, 16);
    #ifdef DO_SERVICES
    if(wUpdateServ)
        FOnpServerNameDialog();	
	#endif

    iCursorSav = ShowWaitCursor();
/*  if(wCopyFiles)
        CopyRTExe();*/ // If rt.exe too big to fit on one disk, do some dirty
                        // work in moa.inf and then call CopyRTExe.
    if (CheckIfTCPInstalled())
        CheckServicesFile();
    //wsprintf (szBuff,"%sbin\\auxcli.exe",szInstallPath);
    if (wInstServices)
        InstallServices();
    InstallDrivers();

	#ifdef DO_SERVICES
	// If we're installing rdate, but not updating the services file, we still 
	// need the onp server name so rdate knows where to go.
	if((wInstRDateAt || wInstRDateInitTab) && !wUpdateServ)
	{
		FOnpServerNameDialog();
	}
	#endif

    if (wInstRDateAt)
    {
        // Start the "at" services
        DoService ( "Schedule", "Schedule", "Schedule" ) ;

        Assert ( szONPServerName != NULL );
        wsprintf ( szBuff, "AT 00:01 /every:m,t,w,th,f,s,su rdate %s", szONPServerName ) ;
        iRet = _spawnlp ( _P_DETACH, "AT.EXE", szBuff, NULL ) ;

        if ( iRet == -1 ) 
        {
            wsprintf ( szBuff, "Error starting RDate as an AT service - run `at 00:01 rdate %s' by hand", szONPServerName ) ;   
            MessageBox (GetActiveWindow(), szBuff, "Setup Message", MB_OK | MB_ICONEXCLAMATION);    
        }         
    }
    if (wInstRDateInitTab)
    {
        // Add RDATE stuff to the inittab.ini in the correct format.
         UpdateInitTabFile ( ) ;
    }


    InstallFonts('i');
    lstrcpy (szBuff,szInstallPath);
    lstrcat (szBuff,"BIN");
    SetPath (szBuff,'i');
	
	// Call putreg.exe here.
	if (wCopyRunTimeFiles)
	{
		char szCmdBuff[_MAX_PATH];
		StripSlash(szInstallPath);
		StripSlash(szMoaProjDir);
	    wsprintf ( szBuff, "%sbin\\putreg.exe -d%s -p%s", szInstallPath, szInstallPath, szMoaProjDir ) ;
		AppendSlash(szInstallPath);
		AppendSlash(szMoaProjDir);
	  	wsprintf(szCmdBuff,"%sbin\\putreg.exe",szInstallPath);

	    iRet = _spawnlp ( _P_DETACH, szCmdBuff, szBuff, NULL ) ;
	
	    if ( iRet == -1 ) 
	    {
	        MessageBox (GetActiveWindow(), "Error starting PUTREG.EXE - run PUTREG -d<mOA Install Directory> -p<mOA Projects Directory> by hand",
	                             "Setup Message", MB_OK | MB_ICONEXCLAMATION);    
	    }         
	}
    TerminateRegDb();

    if (bNetInstall)
        CreateProgGroupOnRemote();
    else
    {

		if ((wCopyDevFiles) || (wCopyRunTimeFiles))
		{
			CreateProgmanGroup("Mosaic OA", "", cmoVital);
        	ShowProgmanGroup("Mosaic OA",  1, cmoVital);
		}

        if (wCopyDevFiles)
        {

			char szBuffer[128];


            CreateProgmanItem("Mosaic OA", "IBAE",
                SzCatStr(szInstallPath, "bin\\ibae.exe"), "", cmoOverwrite);

            CreateProgmanItem("Mosaic OA", "Mosaic OA Investigator",
                SzCatStr(szInstallPath, "bin\\mi.exe"), "progman.exe,13", cmoOverwrite);

			// Csrhelp.hlp
			wsprintf ( szBuffer, "Winhlp32 %sbin\\csrhelp.hlp", szInstallPath );
            CreateProgmanItem("Mosaic OA", "Custom C Subroutine Help",
                szBuffer, SzCatStr(szInstallPath,"bin\\csrhelp.ico,1"), cmoOverwrite);

			// fields.hlp
			wsprintf ( szBuffer, "Winhlp32 %sbin\\fields.hlp", szInstallPath );
            CreateProgmanItem("Mosaic OA", "Mosaic OA Fields Help",
                szBuffer, "", cmoOverwrite);

			// ofbhelp.hlp
			wsprintf ( szBuffer, "Winhlp32 %sbin\\ofbhelp.hlp", szInstallPath );
            CreateProgmanItem("Mosaic OA", "OFB Help",
                szBuffer, SzCatStr(szInstallPath,"bin\\ofbhelp.ico,1"), cmoOverwrite);

			// suppinfo.hlp
			wsprintf ( szBuffer, "Winhlp32 %sbin\\suppinfo.hlp", szInstallPath );
            CreateProgmanItem("Mosaic OA", "Supplemental OFB Statement Information", 
                szBuffer, "", cmoOverwrite);
		}
		

		// Add a program icon for the release notes file.
		if (wCopyRunTimeFiles)
		{
			char szBuffer[128];
			wsprintf ( szBuffer, "%sbin\\wordview.exe %sdoc\\relnotes.doc", szInstallPath, szInstallPath );

            CreateProgmanItem("Mosaic OA", "Runtime and Tools Release Notes",
                szBuffer, "", cmoOverwrite);
		}
		


		// Now, create two batch files to change the user's environment between 
	    // debug and release builds (fat vs slim)
		if (wCopyDevFiles || wCopyRunTimeFiles )
		{
			if (CreateBatchFiles())
			{
				char szBuffer[cchMax];

				CreateProgmanItem("Mosaic OA", "Run Release mOA Binaries",
	                SzCatStr(szInstallPath, "bin\\useslim.cmd"), "", cmoOverwrite);

				CreateProgmanItem("Mosaic OA", "Run Debug mOA Binaries",
	                SzCatStr(szInstallPath, "bin\\usefat.cmd"), "", cmoOverwrite);


				// for some reason, spawning the batch file here fails.  I've used 
				// both system() and _spawnlp(), and get error returns on both.
				//
				// So I'm copying these files on the fly.

				// Run the batch file to copy the FAT files into BIN, so that the user
				// can run debug files by default.
				//wsprintf ( szBuffer, "%sBIN\\USEFAT.CMD", szInstallPath ) ;
    		    //iRet = _spawnlp ( _P_DETACH, "CMD.EXE", szBuffer, NULL ) ;
				//iRet = system ( szBuffer );

				{
				char szBuffer2[cchMax], szBuffer3[cchMax];
				HANDLE hFile;
				int d;
				WIN32_FIND_DATA file;

				wsprintf ( szBuffer2, "%sbin_fat\\*.*", szInstallPath ) ;
				hFile = FindFirstFile ( szBuffer2, &file );
				
				if (hFile==INVALID_HANDLE_VALUE)
					GetLastError();


				while (FindNextFile ( hFile, &file ))
				{
					if ( (strcmp (file.cFileName, ".")!=0) && (strcmp ( file.cFileName, "..")!=0))
					{
						wsprintf ( szBuffer3, "%sbin_fat\\%s", szInstallPath, file.cFileName ) ;
						wsprintf ( szBuffer2, "%sbin\\%s", szInstallPath, file.cFileName ) ;
						
						// Also, CopyFile will always overwrite the target file if the 
						// 3rd paramater==FALSE, so we can update older DLL's/etc.
						if (!CopyFile ( szBuffer3, szBuffer2, FALSE ) )
						{
							d = GetLastError();

							wsprintf ( szBuffer, "Warning!  Setup failed to copy %s from %sbin_fat to %sbin (ERROR:%d)\n"
							 					 "You must copy this file by hand", file.cFileName, szInstallPath, szInstallPath, d, szInstallPath ) ;

							MessageBox ( NULL, szBuffer, "Setup Message", MB_OK|MB_ICONEXCLAMATION );
				
							// Restart the loop and find the next file.
							break;
						}	// end if
						
					}	// end if 
				}	// end while
				FindClose(hFile);
				// End copy files
				}	// end block
	        	
				if (iRet==-1)
				{
					GetLastError();
				
					MessageBox (NULL, "Error Copying <moa dir>\\bin_fat into <moa dir>\\bin!\n"
									  "Please run the 'Run Debug mOA Binaries' program icon.", 
									  "Setup Message", MB_OK|MB_ICONEXCLAMATION ) ;
				}

			}
			else
			{
				MessageBox ( NULL,  "Setup was unable to create one or both of the batch files which "
									"change between the debug and release builds of the mOA binaries.\n"
									"You must create these files by hand." , "Setup Message", MB_OK|MB_ICONEXCLAMATION );
			}
	 	}
    }
	CopyLastMinuteFiles();
    UpdatePrinterInfo();
    CopySharedDLL();
    AppendOverSizeFiles();
	CopyPrinterDLLs();
    RestoreCursor(iCursorSav);
    UIPop(1);
	RemoveDLLs();
}





/* **************************************************************** */
VOID AskQuit()
{
    CHAR rgchT[16];

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_QUIT, "FQuitDlgProc", HELP_ASKQUIT,
            "FHelpDlgProc", (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        default:
            continue;

        case IDC_QUITEXIT:
            UIPopAll();
            EndSetupToolkit();
            exit(0);

        case IDC_CANCEL:
        case IDC_QUITCONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}

/* **************************************************************** */
VOID WelcomeDialog()
{
        CHAR rgchT[16];

        while (TRUE)
            {
        UIStartDlg("tsetuph.dll", DLG_WELCOME, "FInfoDlgProc",
                HELP_APPLICATION, "FHelpDlgProc", (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        case IDC_CANCEL:
        case IDC_TYPEEXIT:
            AskUnInstallQuit();
            continue;

        default:
            continue;

        case IDC_CONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}

/* **************************************************************** */
VOID InstallTypeDialog()
{
    CHAR rgchT[16];

    while (TRUE)
        {
        SetSymbolValue("RadioDefault", "1");
        UIStartDlg("tsetuph.dll", DLG_INSTALLTYPE, "FRadioDlgProc",
                HELP_INSTALLTYPE, "FHelpDlgProc", (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        case IDC_CANCEL:
        case IDC_TYPEEXIT:
            AskQuit();
            continue;

        default:
            continue;

        case IDC_CONTINUE:
            GetSymbolValue("ButtonChecked", rgchT, 15);
            installType = (*rgchT == '1') ? itComplete : itMinimum;
            break;
            }

        UIPop(1);
        return;
        }
}


/* **************************************************************** */
VOID BlankPathDialog()
{
    CHAR rgchT[16];

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_BLANK, "FInfo0DlgProc", 0, NULL,
                (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        default:
            continue;

        case IDC_BLANKEXIT:
            AskQuit();
            continue;

        case IDC_CANCEL:
        case IDC_BLANKCONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}


/* **************************************************************** */
VOID DestIsSrcDialog()
{
    CHAR rgchT[16];

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_DESTISSRC, "FInfo0DlgProc", 0, NULL,
                (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        default:
            continue;

        case IDC_DESTISSRCEXIT:
            AskQuit();
            continue;

        case IDC_CANCEL:
        case IDC_DESTISSRCCONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}


/* **************************************************************** */
VOID InvalidPathDialog()
{
    CHAR rgchT[16];

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_INVALIDPATH, "FInfo0DlgProc", 0, NULL,
                (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        default:
            continue;

        case IDC_INVALIDEXIT:
            AskQuit();
            continue;

        case IDC_CANCEL:
        case IDC_INVALIDCONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}


/* **************************************************************** */
VOID AppendSlash(LPSTR sz1)
{
    LPSTR sz2 = sz1;

    while (*sz2)
        {
        sz1 = sz2;
        sz2 = AnsiNext(sz1);
        }

    if (sz2 != sz1 + 1 || *sz1 != '\\')
        {
        *sz2++ = '\\';
        *sz2 = '\0';
        }
}


/* **************************************************************** */
VOID StripSlash(LPSTR sz1)
{
    LPSTR sz2 = sz1;

    while (*sz2)
        {
        sz1 = sz2;
        sz2 = AnsiNext(sz1);
        }

    if (sz2 == sz1 + 1 && *sz1 == '\\')
        *sz1 = '\0';
}


/* **************************************************************** */
BOOL FInstallPathDialog()
{
    CHAR rgchT[16];
    BOOL fRet;
    CHAR sz[cchMax], sz2[_MAX_PATH];

    lstrcpy(sz, szInstallPath);
	lstrcpy(sz2, szMoaProjDir ) ;

    StripSlash(sz);
	StripSlash(sz2);

    SetSymbolValue("EditTextIn", sz);
	SetSymbolValue("Edit2TextIn", sz2 ) ;
    SetSymbolValue("EditFocus", "END");

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_INSTALLPATH, "F2EditDlgProc",
            HELP_INSTALLPATH, "FHelpDlgProc", (LPSTR)rgchT, 15);

        if ((WORD)(rgchT[0]+rgchT[1]) == IDC_PATHEXIT)
            {
            AskQuit();
            continue;
            }
        else if ((WORD)(rgchT[0]+rgchT[1]) == IDC_PATHCANCEL
                || (WORD)(rgchT[0]+rgchT[1]) == IDC_CANCEL
                || (WORD)(rgchT[0]+rgchT[1]) == IDC_BACK)
            {
            fRet = FALSE;
            break;
            }
        else if (rgchT[0]+rgchT[1] != IDC_CONTINUE)
            continue;

        GetSymbolValue("EditTextOut", sz, cchMax);
		GetSymbolValue("Edit2TextOut", sz2, cchMax ) ;

        if ((lstrlen(sz) == 0) || (lstrlen(sz2)==0))
        {
            BlankPathDialog();
            continue;
        }

        if ((!FValidDir(sz)) || (!FValidDir(sz2)))
        {
            InvalidPathDialog();
            continue;
        }

        lstrcpy(szInstallPath, sz);
        AppendSlash(szInstallPath);

		lstrcpy (szMoaProjDir, sz2);
		AppendSlash(szMoaProjDir);

        if (lstrcmp(szInstallPath, szSrcDir) == 0)
            {
            DestIsSrcDialog();
            continue;
            }

        fRet = TRUE;
        break;
        }

    UIPop(1);
    return fRet;
}






/* ********************************************************************* */
VOID SetupError(int error)
{
    CHAR rgchT[16];
    BOOL fLoopForDialog = TRUE;
    static BOOL fInsideSetupError = FALSE;

    if (!fInsideSetupError)
        {
        fInsideSetupError = TRUE;

        UIPopAll();

        while (fLoopForDialog)
            {
            UIStartDlg("tsetuph.dll", DLG_ERROR, "FInfo0DlgProc", 0, NULL,
                (LPSTR)rgchT, 15);

            switch ((WORD)(rgchT[0]+rgchT[1]))
                {
            default:
                continue;

            case IDC_CANCEL:
            case IDC_ERROREXIT:
                fLoopForDialog = FALSE;
                break;
                }
            }

        UIPop(1);
        }

    EndSetupToolkit();
    exit(0);
}


/* **************************************************************** */
INT PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR szCmdLineArgs,
        INT nCmdShow)
{
    DWORD dwSize = 50;
	DWORD dwOSVersion;
    int  iCursorSav;
    CHAR rgchT[16];

    hInst = hInstance;
    if (szCmdLineArgs)
	{
		if ( (!lstrcmp ( szCmdLineArgs, "-?")) || 
			 (!lstrcmp ( szCmdLineArgs, "/?")) || 
			 (!lstrcmp ( szCmdLineArgs, "/U")) || 
		     (!lstrcmp ( szCmdLineArgs, "-U")) )
		{
			Usage();
			exit(1);
		}

        if ( (!lstrcmp ( szCmdLineArgs, "-u" )) ||
             (!lstrcmp ( szCmdLineArgs, "/u" )) )
        {
            bUnInstall = TRUE;
            szCmdLineArgs[0] ='\0';
        }
        else
		{
        	// Check if net work installation
        	InitNetInstall(szCmdLineArgs);
		}
	}
//  Ctl3dRegister(hInstance);
//  Ctl3dAutoSubclass(hInstance);

	dwOSVersion = GetVersion () ;
	iVersion = 100 * LOBYTE(LOWORD(dwOSVersion)) +  HIBYTE(LOWORD(dwOSVersion)) ;

	if (InitSetupToolkit(szCmdLineArgs) > 0)        // always should come first
        {
        if (FInitRegDb())
            {
                SetBitmap("tsetuph.dll", 1);
                SetTitle("Mosaic OA setup");
                iCursorSav = ShowWaitCursor();
                hWnd = FindWindow (NULL,"Mosaic OA setup");
                //Initialize 3DCtl lib
                RestoreCursor(iCursorSav);

                if (bNetInstall) {
                    char szBuff[100];
                    wsprintf(szBuff,"Mosaic OA Setup -%s",szMachine);
                    SetTitle(szBuff);
                    if (iCustomSetup)
                        InstallOptionsDialog('i');
                    else
                        SetAllOptions();
                    GetSymbolValue("STF_SRCDIR", szSrcDir, cchMax);
                    ReadInfFile(SzCatStr(szCurDir, pInfFileName));
                    BuildLists();
                    Install();
                    RegCloseKey(hRootKey);
                    }
                else {

//VOID __declspec (dllexport) GetSetupDirectory ( LPCSTR szWhichSetup, LPSTR szPath, DWORD dwLength );
				
				// Pick up the previous path settings from the store (.ini/registry) if they exist;
				// if they don't, then stuff in default pathnames to fill in this dialog.

				GetSetupDirectory ( "Runtime and Development Tools", szInstallPath, _MAX_PATH ) ;
									
				if (lstrlen (szInstallPath) == 0)
                	lstrcpy(szInstallPath, DEFAULT_MOA_DIRECTORY);
				
				GetSetupDirectory ( "mOA Projects", szMoaProjDir, _MAX_PATH ) ;
				
				if (lstrlen (szMoaProjDir) == 0)
					lstrcpy(szMoaProjDir, DEFAULT_MOA_PROJECT_DIRECTORY);

                GetSymbolValue("STF_SRCDIR", szSrcDir, cchMax);
                if (!bUnInstall) {
                    WelcomeDialog();
					// handles the back button
                    do {
						FInstallPathDialog();
                    } while (!InstallOptionsDialog('i'));
                    ReadInfFile(SzCatStr(szCurDir, pInfFileName));
                    BuildLists();
                    Install();
                }
                else{
                        WelcomeUnInstall();
                        do  {
							FUnInstallPathDialog();
                        } while (!InstallOptionsDialog('u'));
                        UnInstall();
                }
            }
            }


			if (wCopyDevFiles && !bUnInstall)
				MessageBox(hWnd,"The Development Tools rely upon the version of CTL3D32.DLL dated 4/4/95 and 27136 bytes in size.  "
				"If a different version is found first in the path, the development tools may not operate properly.\n\n"
				"To determine the version of CTL3D32.DLL which your machine is running, you can either run the "
				"Mosaic Investigator utility, or type 'set' from a command prompt and look for CTL3D32.DLL in the directories "
				"listed in the PATH environment variable.  CTL3D32.DLL will be loaded from the first directorory in the PATH "
				"which contains CTL3D32.DLL.\n\n"
				"Setup will copy this file from the installation floppies to <windows dir>\\system32.  If a directory earlier "
				"in your path than <windows dir>\\system32 contains a different version of CTL3D32.DLL, either update the path or delete the different version. "
				"You can also use the NT 'expand' command to copy CTL3D32.DLL from the mosaic floppies to the correct location.","Setup Message",MB_OK | MB_ICONINFORMATION);

            if (bShutDown)
                if ( IDOK == MessageBox(hWnd,"Changes made to the Registry will not be effective until the computer is restarted.\r\nRestart now ?","Setup Message",MB_OKCANCEL | MB_ICONQUESTION))
                    ShutDownComputer();
		    
		    if ( bUnInstall )
				UIStartDlg("tsetuph.dll", EXITUNINSTALL, "FInfoDlgProc", 0, NULL,
        	    	(LPSTR)rgchT, 16);
			else
		    	UIStartDlg("tsetuph.dll", EXITSUCCESS, "FInfoDlgProc", 0, NULL,
        	    	(LPSTR)rgchT, 16);


//          Ctl3dUnregister(hInstance);
            EndSetupToolkit();

        }

    exit(0);
    return(0);
}


BOOL CheckIfTCPInstalled()
{
    HKEY hKey = 0;
    long lRetValue;

    if (bRunningWin95())
		lRetValue = RegOpenKey (hRootKey,"Enum\\Network\\MSTCP",&hKey);
	else
		lRetValue = RegOpenKey (hRootKey,"SOFTWARE\\Microsoft\\Tcpip",&hKey);

	if (!lRetValue == ERROR_SUCCESS)
        MessageBox (hWnd,"TCP/IP is not Installed!","Setup Message", MB_OK);
    if (hKey)
        RegCloseKey (hKey);
    if (lRetValue == ERROR_SUCCESS)
        return TRUE;
    return FALSE;
}

void
InstallServices()
{
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
	QUERY_SERVICE_CONFIG qsc;
	DWORD cbBytesNeeded;	
    DWORD dwError;
    char szMessageBuff[256];
    int i,j;
    char szDestBuff[MAX_PATH],szRealDest[_MAX_PATH],szSrcBuff[_MAX_PATH];
    LPCTSTR lpszBinaryPathName;
    char* serviceName;
    j = sizeof (services) / sizeof (SERVICE);
    for (i = 0; i < j; i++){
        if(!services[i].wFlag || !services[i].exeName)
            continue;
        serviceName = services[i].serviceName;

		// Leave auxcli.exe file in c:\moa\bin, e.g.		
		if ( _stricmp ( serviceName, "Auxprint" ) != 0 ) 
		{
	        MyGetWindowsDirectory(szDestBuff,MAX_PATH);
	        lstrcat(szDestBuff,"\\system32\\drivers\\");
	        wsprintf (szSrcBuff,"%sbin\\%s",szInstallPath,services[i].exeName);
	        lstrcat(szDestBuff,services[i].exeName);
			CopyAFile(szSrcBuff,szDestBuff,cmoOverwrite,0);        
	    }
		else {
			if (DP3_PRINTER == wPrinter)
	        	wsprintf (szDestBuff,"%sbin\\%s",szInstallPath,services[i].exeName);
			else
	        	wsprintf (szDestBuff,"%sbin\\%s",szInstallPath,"verifone.exe");
			}


		/* Following code installs the above copied service image.szDestBuff
		 * should contain the path where the file can be found for service 
		 * manager */
	    /* If network installation szDestbuff differs from lpszBinaryPathName*/
        lpszBinaryPathName = GetRealPath(szRealDest,szDestBuff);
        schSCManager = OpenSCManager(
                            szMachine,                   // machine (NULL == local)
                            NULL,                   // database (NULL == default)
                            SC_MANAGER_ALL_ACCESS   // access required
                            );

		// PSR #398 - Auxprint and PR50 don't get along.  If they both are requested, 
		// then don't start up Auxprint automatically. PML 7/6/95
		if (( _stricmp ( serviceName, "Auxprint" ) == 0 ) && (wPrinter==PR50_PRINTER))
		{
	        schService = CreateService(
	                schSCManager,               // SCManager database
	                serviceName,                // name of service
	                serviceName,                // name to display
	                SERVICE_ALL_ACCESS,         // desired access
	                SERVICE_WIN32_OWN_PROCESS,  // service type
	                SERVICE_DEMAND_START,/**/   // start type
	                SERVICE_ERROR_NORMAL,       // error control type
	                lpszBinaryPathName,         // service's binary
	                NULL,                       // no load ordering group
	                NULL,                       // no tag identifier
	                "serial",                   // depends only on serial.sys driver
	                NULL,                       // LocalSystem account
	                NULL);                      // no password
		}
		else
		{
			// Auxprint and Verifone both depend upon the serial service to start.
			if (_stricmp (serviceName, "Auxprint"))
			{
			    schService = CreateService(
		                schSCManager,               // SCManager database
		                serviceName,                // name of service
		                serviceName,                // name to display
		                SERVICE_ALL_ACCESS,         // desired access
		                SERVICE_WIN32_OWN_PROCESS,  // service type
		                SERVICE_AUTO_START,/**/     // start type
		                SERVICE_ERROR_NORMAL,       // error control type
		                lpszBinaryPathName,         // service's binary
		                NULL,                       // no load ordering group
		                NULL,                       // no tag identifier
		                "serial",/**/               // no dependencies
		                NULL,                       // LocalSystem account
		                NULL);                      // no password
			}
			else
			{
		        schService = CreateService(
		                schSCManager,               // SCManager database
		                serviceName,                // name of service
		                serviceName,                // name to display
		                SERVICE_ALL_ACCESS,         // desired access
		                SERVICE_WIN32_OWN_PROCESS,  // service type
		                SERVICE_AUTO_START,/**/     // start type
		                SERVICE_ERROR_NORMAL,       // error control type
		                lpszBinaryPathName,         // service's binary
		                NULL,                       // no load ordering group
		                NULL,                       // no tag identifier
		                NULL,                       // no dependencies
		                NULL,                       // LocalSystem account
		                NULL);                      // no password
			}
		}

		if (schService == NULL)
		{	 
			dwError = GetLastError ();

			if (dwError == ERROR_SERVICE_EXISTS)
			{
				schService = OpenService(schSCManager, serviceName, SERVICE_ALL_ACCESS);

				// PSR #960, 1291:  SCREG.EXE experiences a protection fault when making this call on NT 3.1, even
				// with service pack 2 installed.  An MSDN article led us to believe that this was only a problem
				// on NT 3.1 without any SP's, but this is not the case.
				//
				// This code circumvents the fault by not making the ChangeServiceConfig() call iff the current
				// service already is started automatically (This is settable via ctrl panel->services).
				// All machines running 3.1 and being set up from scratch will be this way for the schedule service,
				// which is the one causing the problem in this PSR report..  The only time this will NOT work is
				// if somebody fiddled with that control panel setting to make the schedule service start manually.
				// So we'll put up a message box saying "Please change this to autostart" by hand, if running on 
				// 3.1, and if the service is not autostart.

				QueryServiceConfig ( schService, &qsc, sizeof(qsc), &cbBytesNeeded ) ;				

				if (TRUE /*qsc.dwStartType != SERVICE_AUTO_START*/ )
				{
					if (iVersion == 310)
					{ 
						wsprintf ( szMessageBuff,  "The %s service is not currently configured to start automatically.\n"
										"Please start Control Panel, select Services, and configure the %s service to start automatically.", serviceName, serviceName ) ;
						MessageBox ( NULL, szMessageBuff, "Setup Message", MB_OK ) ;
					}
					else 
					{			
						// PSR #398 - Auxprint and PR50 don't get along.  If they both are requested, 
						// then don't start up Auxprint automatically. PML 7/6/95
						if (( _stricmp ( serviceName, "Auxprint" ) == 0 ) && (wPrinter==PR50_PRINTER))
						{
							if (!ChangeServiceConfig (schService,SERVICE_WIN32_OWN_PROCESS,
												SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,
												lpszBinaryPathName,NULL,NULL,NULL,NULL,NULL,
												serviceName))
							{

								dwError = GetLastError ();

								if (!dwError)
								{
									wsprintf(szMessageBuff,"Failed to install %s service",services[i].serviceName);
									MessageBox (hWnd,szMessageBuff,"Setup Message",MB_OK);
								}
							}
						}
						else
						{
							if (!ChangeServiceConfig (schService,SERVICE_WIN32_OWN_PROCESS,
												SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
												lpszBinaryPathName,NULL,NULL,NULL,NULL,NULL,
												serviceName))
							{

								dwError = GetLastError ();

								if (!dwError)
								{
									wsprintf(szMessageBuff,"Failed to install %s service",services[i].serviceName);
									MessageBox (hWnd,szMessageBuff,"Setup Message",MB_OK);
								}
							}
						}
					}
				}
			}	// end if service exists
			else
			{
				wsprintf(szMessageBuff,"Failed to install %s service",services[i].serviceName);
				MessageBox (hWnd,szMessageBuff,"Setup Message",MB_OK);
			}
		}
		if (schService)
			CloseServiceHandle(schService);
		if (schSCManager)
			CloseServiceHandle(schSCManager);
    }
	// If Auxprint is installed, we need to add some more registry keys to the 
	// Auxprint services key. These keys are used if the verifone.exe is 
	// the exe of Auxprint, but we add for other case too.
	if (services[0].wFlag)
		{
		HKEY hKey;
		char szBuff[25];
		INT i = 2;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Auxprint",0,KEY_ALL_ACCESS,&hKey);
		wsprintf(szBuff,"COM%d",wComPort + 2);
		RegSetValueEx(hKey,"Port",0,REG_SZ,szBuff,lstrlen(szBuff) + 1);
		RegSetValueEx(hKey,"IdLen",0,REG_DWORD,(CONST BYTE*)&i,sizeof(i));
		RegCloseKey(hKey);
		}

}

void DoService ( LPCSTR serviceName, LPSTR serviceExe, LPCSTR lpszBinaryPathName ) 
{
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
    char szMessageBuff[128];
    DWORD dwError;
    DWORD cbBytesNeeded;
    QUERY_SERVICE_CONFIG qsc;

    schSCManager = OpenSCManager(
                        szMachine,                   // machine (NULL == local)
                        NULL,                   // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS   // access required
                        );

    schService = CreateService(
            schSCManager,               // SCManager database
            serviceName,                // name of service
            serviceName,                // name to display
            SERVICE_ALL_ACCESS,         // desired access
            SERVICE_WIN32_OWN_PROCESS,  // service type
            SERVICE_AUTO_START,       // start type
            SERVICE_ERROR_NORMAL,       // error control type
            lpszBinaryPathName,         // service's binary
            NULL,                       // no load ordering group
            NULL,                       // no tag identifier
            NULL,                       // no dependencies
            NULL,                       // LocalSystem account
            NULL);                      // no password

    
    if (schService == NULL)
    {
        dwError = GetLastError ();
        if (dwError == ERROR_SERVICE_EXISTS)
        {
            schService = OpenService(schSCManager, serviceName, SERVICE_ALL_ACCESS);

            QueryServiceConfig ( schService, &qsc, sizeof(qsc), &cbBytesNeeded ) ;


            if (!ChangeServiceConfig (schService,SERVICE_WIN32_OWN_PROCESS,
                                 SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
                                 NULL/*Don't modify pathname*/,NULL,NULL,NULL,NULL,NULL,
                                 serviceName))
            {

                dwError = GetLastError ();

                if (!dwError)
                {
                    wsprintf ( szMessageBuff, "Failed to install %s service", serviceName ) ;
                    MessageBox (hWnd, szMessageBuff, "Setup Message", MB_OK | MB_ICONEXCLAMATION);
                }
            }
        }
        else
        {
            wsprintf ( szMessageBuff, "Failed to install %s service", serviceName ) ;   
            MessageBox (hWnd, szMessageBuff, "Setup Message", MB_OK | MB_ICONEXCLAMATION);
        }
    }
    
    // If we're setting up the schedule service, we need to actually
    // start it, so that the "at" command can be run.
    if ( stricmp ( serviceName, "Schedule" ) == 0)
    {
        // Start the schedule service
        if (StartService ( schService, 0, NULL ) == FALSE )
        {
            if ( GetLastError() != ERROR_SERVICE_ALREADY_RUNNING ) 
                MessageBox ( hWnd, "Couldn't start the Schedule service; the AT service may fail now", "Setup Message", MB_OK | MB_ICONEXCLAMATION ); 
        }        
    } 
    if (schService)
    {
        CloseServiceHandle(schService);
    }
    if (schSCManager)
    {
        CloseServiceHandle(schSCManager);
    }
}


void
DeleteServices()
{
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
    DWORD dwErrorCode;
    int i,j;
    char szMessageBuff[256];

        schSCManager = OpenSCManager(
                        szMachine,                   // machine (NULL == local)
                        NULL,                   // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS   // access required
                        );

    j = sizeof (services) / sizeof (SERVICE);
    for (i = 0; i < j; i++){
        if(!services[i].wFlag)
            continue;
        wsprintf(szMessageBuff,"Deleting Service   : %s",services[i].serviceName);
        SetUnInstallText(szMessageBuff);
        schService = OpenService(schSCManager, services[i].serviceName, SERVICE_ALL_ACCESS);
        if (schService != NULL) {
            if (!DeleteService (schService)){
                dwErrorCode = GetLastError();
                switch(dwErrorCode) {
                    case  ERROR_ACCESS_DENIED:
                        wsprintf(szMessageBuff,"%s service could not be deleted. \r\n Access denied",\
                                    services[i].serviceName);
                        MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK |MB_ICONINFORMATION);
                    default:
                        wsprintf(szMessageBuff,"%s service could not be deleted \r\n \
                        Service is not responding to delete request. ",services[i].serviceName);
                        MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK |MB_ICONINFORMATION);
                        break;
                    }
                }
            }

        }
    CloseServiceHandle(schSCManager);
}

typedef struct KEYVALUES{
    char    *szName;
    DWORD   dwValueType;
    char    *szValue;
}KEYVALUES;
typedef struct KEYINFO{
    int         nSize;
    char        *szKeyName;
    KEYVALUES   KeyValues[1];
}KEYINFO;

//*********************************************************************************
// FUNCTION     InstallDrivers
//
// PURPOSE      This routine reads *.ini file and gets the info and updates
//         registry database.It also copies the driver to winnt\system32\driver
//         directory.For example to install wincom.sys driver, it reads
//         wincom.ini file, updates registry.Then copies the file.
//**********************************************************************************

void
InstallDrivers ()
{
	int iRet; 				// Return value from function(s)
	char szBuff[256];		// Temporary buffer for MessageBox() output
    HKEY hKey = 0;
    HFILE hFile;
    DWORD Constant;
    OFSTRUCT ofStruct;
    KEYINFO KeyInfo;
    KEYVALUES   KeyValue;
    DWORD dwBytesRead;
    char *pCurrentPtr,*pEndPtr,*pTmpPtr,*pFileInfo;
    char szDestBuff[MAX_PATH],szSrcBuff[MAX_PATH],szBackupBuff[_MAX_PATH];
    int i,j;
	char* pTmp;

    j = sizeof (DriverInfo) / sizeof (DRIVER);
    for (i = 0; i < j; i++){
        if(!DriverInfo[i].wFlag)
            continue;
        MyGetWindowsDirectory (szDestBuff,MAX_PATH);
        lstrcat (szDestBuff,"\\system32\\drivers\\");
        lstrcat (szDestBuff,DriverInfo[i].DriverName);
		lstrcpy(szBackupBuff,szDestBuff);
        lstrcpy (szSrcBuff,szInstallPath);
        lstrcat (szSrcBuff,DriverInfo[i].DriverDir);
        lstrcat (szSrcBuff,DriverInfo[i].DriverName);
		// Rename the driver file with .bak extension.
		// This should take care of the case , if the file is 
		// in use.
		pTmp = strrchr(szBackupBuff,'.');
		if (pTmp)
			lstrcpy(pTmp,".bak");
		else
			lstrcat(szBackupBuff,".bak");
		// We donot check if the below function fails, because
		// we need to keep the original copy of driver.
		// So backup first time, then don't for later installations.
		// This helps in restoring back the original drivers after
		// uninstall.

		rename(szDestBuff,szBackupBuff);

        CopyAFile (szSrcBuff,szDestBuff,cmoOverwrite,0);

        lstrcpy (szSrcBuff,szInstallPath);
        lstrcat (szSrcBuff,DriverInfo[i].DriverDir);
        lstrcat (szSrcBuff,DriverInfo[i].DriverIni);

        hFile = OpenFile (szSrcBuff,&ofStruct,OF_READ);
        if (HFILE_ERROR == hFile){
            MessageBox (hWnd, "Could not find the file containing drivers info","Setup Message",MB_OK);
            return;
        }
        pFileInfo = (char*)GlobalAlloc (GPTR, GetFileSize ((HANDLE)hFile,NULL) + 10);
        ReadFile ((HANDLE)hFile,pFileInfo,GetFileSize ((HANDLE)hFile,0),&dwBytesRead,NULL);
        pCurrentPtr = pFileInfo;
        do {
            pEndPtr = strstr (pCurrentPtr,"\r\n");
            if (pEndPtr)
                *pEndPtr = '\0';
            KeyInfo.szKeyName = strstr (pCurrentPtr,"\\registry\\machine");
            if (KeyInfo.szKeyName){
				// Note the trailing backslashes on line below.  If we don't have 
				// these, the API fails on an INVALID_PATH error.  The path assumes
				// no leading slashes.
                KeyInfo.szKeyName += lstrlen ("\\registry\\machine\\");
                iRet = RegCreateKeyEx(hRootKey,KeyInfo.szKeyName,0,0,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hKey,&Constant);

				// Handle failure in a relatively graceful manner
				if (iRet != ERROR_SUCCESS)
				{
					wsprintf ( szBuff, "Failed RegCreateKeyEx - Registry key %s creation for the %s driver failed, returning %d!", KeyInfo.szKeyName, DriverInfo[i].DriverName, iRet ) ;
					MessageBox ( NULL, szBuff, "Setup Message", MB_OK|MB_ICONEXCLAMATION ) ;
					return;
				}

                pCurrentPtr = pEndPtr + 2;
            }
            else{
                pTmpPtr = strstr (pCurrentPtr," = ");
                if (pTmpPtr){
                    *pTmpPtr = '\0';
                    pTmpPtr += 3;
                    while( *pCurrentPtr == ' ' || *pCurrentPtr == '\t')
                        pCurrentPtr++;
                    KeyValue.szName = pCurrentPtr;
                    pCurrentPtr = pTmpPtr;
                    pTmpPtr = strstr(pCurrentPtr," ");
                    *pTmpPtr = '\0';
                    pTmpPtr++;
                    KeyValue.szValue = pTmpPtr;
                    KeyValue.dwValueType = GetValueType (pCurrentPtr);
                    if (KeyValue.dwValueType == REG_DWORD){
                        dwBytesRead = atol(pTmpPtr);
                        RegSetValueEx (hKey,(LPCTSTR)KeyValue.szName,0,KeyValue.dwValueType ,(CONST BYTE*)&dwBytesRead,sizeof(dwBytesRead));
                    }
                    else
                        RegSetValueEx (hKey,KeyValue.szName,0,KeyValue.dwValueType ,pTmpPtr,lstrlen(pTmpPtr) +1);
                    pCurrentPtr = pEndPtr + 2;
                }
            }
        }while (pEndPtr );
    }
}
DWORD
GetValueType (char *pValue)
{

    if (!lstrcmp (pValue,"REG_SZ"))
        return REG_SZ;
    else
    if (!lstrcmp (pValue,"REG_DWORD"))
        return REG_DWORD;
    else
    if (!lstrcmp (pValue,"REG_BINARY"))
        return REG_BINARY;
    else
    if (!lstrcmp (pValue,"REG_EXPAND_SZ"))
        return REG_EXPAND_SZ;
    else
    if (!lstrcmp (pValue,"REG_LINK"))
        return REG_LINK;
    else
    if (!lstrcmp (pValue,"REG_DWORD_LITTLE_ENDIAN"))
        return REG_DWORD_LITTLE_ENDIAN;
    else
    if (!lstrcmp (pValue,"REG_DWORD_BIG_ENDIAN"))
        return REG_DWORD_BIG_ENDIAN;
    else
    if (!lstrcmp (pValue,"REG_MULTI_SZ"))
        return REG_MULTI_SZ;
    else
    if (!lstrcmp (pValue,"REG_RESOURCE_LIST"))
        return REG_RESOURCE_LIST;
    else
        return REG_NONE;
}



//********************************************************************************************

void ShutDownComputer ()
{
    HANDLE hToken;              /* handle to process token */
    TOKEN_PRIVILEGES tkp;        /* ptr. to token structure */


    // If not necessary to Shutdown then don't
    if(!bShutDown)
        return;
    /*
     * Get the current process token handle
     * so we can get shutdown privilege.
    */

    if (!OpenProcessToken(GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    // ErrorHandler("OpenProcessToken failed.");
    ;

    /* Get the LUID for shutdown privilege. */

    LookupPrivilegeValue(szMachine, SE_SHUTDOWN_NAME,
            &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  /* one privilege to set    */
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    /* Get shutdown privilege for this process. */

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES) NULL, 0);

    /* Cannot test the return value of AdjustTokenPrivileges. */

    //if (GetLastError() != ERROR_SUCCESS)
        //ErrorHandler("AdjustTokenPrivileges enable failed.");

    /* Display the shutdown dialog box and start the time-out countdown. */

	InitiateSystemShutdown (szMachine,"Your system will be shut down in 30 seconds.  Please save your work and logoff.", 30, TRUE, TRUE);
}

void
CopyDLLs()
{
    char szBuff[MAX_PATH];
    char szBuffDest[MAX_PATH],szBuffSrc[MAX_PATH];
    char* szSrcDLLs[] = { "\\MSCOMSTF.DLL","\\tsetuph.DLL","\\MSDETSTF.DLL","\\MSINSSTF.DLL",
                           "\\MSSHLSTF.DLL","\\MSUILSTF.DLL","\\MSVCRT20.DLL"
                         }  ;
    int i;
    GetWindowsDirectory (szBuff,MAX_PATH);
    lstrcat ( szBuff, "\\system32" ) ;
    for (i = 0 ; i < 7; i++){
        lstrcpy (szBuffDest,szBuff);
        lstrcpy (szBuffSrc,szCurDir);
        lstrcat (szBuffSrc,szSrcDLLs[i] + 1);
        lstrcpy (szBuffDest,szBuff);
        lstrcat (szBuffDest,szSrcDLLs[i]);
        CopyFile (szBuffSrc,szBuffDest,FALSE);
    }
}

void
RemoveDLLs()
{
    char szBuff[MAX_PATH];
    char szBuffDest[MAX_PATH],szBuffSrc[MAX_PATH];
    char* szSrcDLLs[] = { "\\MSCOMSTF.DLL","\\tsetuph.DLL","\\MSDETSTF.DLL","\\MSINSSTF.DLL",
                           "\\MSSHLSTF.DLL","\\MSUILSTF.DLL"
                         }  ;
    char* szDestDLLs[] = { "\\MSCOMSTF.DL_","\\tsetuph.DL_","\\MSDETSTF.DL_","\\MSINSSTF.DL_",
                           "\\MSSHLSTF.DL_","\\MSUILSTF.DL_"
                         }  ;
    int i;
    GetWindowsDirectory (szBuff,MAX_PATH);
    for (i = 0 ; i < 6; i++){
        lstrcpy (szBuffDest,szBuff);
        lstrcpy (szBuffSrc,szBuff);
        lstrcat (szBuffSrc,szSrcDLLs[i]);
        lstrcpy (szBuffDest,szBuff);
        lstrcat (szBuffDest,szDestDLLs[i]);
        MoveFileEx(szBuffSrc,szBuffDest,MOVEFILE_REPLACE_EXISTING | MOVEFILE_DELAY_UNTIL_REBOOT);
    }
}




void
CheckServicesFile()
{
    char szDestBuff[MAX_PATH], *pBuff,*pTmp,szSrcBuff[MAX_PATH],szTmpFile[MAX_PATH];
    HFILE hFile;
    OFSTRUCT of;
    DWORD dwSize;

    if(!wUpdateServ)
        return;
    MyGetWindowsDirectory (szDestBuff,MAX_PATH);
    lstrcat (szDestBuff,"\\system32\\drivers\\etc\\services");
    lstrcpy (szSrcBuff,szInstallPath);
    lstrcat (szSrcBuff,"src\\driver\\files\\Services.ini");
    lstrcpy (szTmpFile,szInstallPath);
    lstrcat (szTmpFile,"Services.tmp");

    AddONPServerName(szSrcBuff,szTmpFile);
    hFile = OpenFile (szDestBuff,&of,OF_READWRITE);
    if (hFile == HFILE_ERROR){
        MessageBox (hWnd,"Couldn't open Services file\r\nServices Info is not updated","Setup Message",MB_OK | MB_ICONEXCLAMATION);
        return;
    }
    dwSize = GetFileSize ((HANDLE)hFile,0) ;
    pBuff = (char*)GlobalAlloc (GPTR,dwSize + 10);
    ReadFile ((HANDLE)hFile,pBuff,dwSize,&dwSize,0);
    pTmp = strstr (pBuff,szServSearchStr);
    _lclose (hFile);
    if (!pTmp)
        CopyAFile (szSrcBuff,szDestBuff,cmoAppend,1);
    else
        UpdateServerName(szDestBuff,pBuff);
    remove (szSrcBuff);
    CopyAFile (szTmpFile,szSrcBuff,cmoOverwrite,0);
    GlobalFree((HGLOBAL)pBuff);
}
void
CopyRTExe()
{
    char szDestBuff[MAX_PATH],szSrcBuff[MAX_PATH];

    lstrcpy (szDestBuff,szInstallPath);
    lstrcat (szDestBuff,"bin\\rt.exe");
    lstrcpy (szSrcBuff,szInstallPath);
    lstrcat (szSrcBuff,"rt.exe");
    CopyAFile (szSrcBuff,szDestBuff,cmoOverwrite,0);
    lstrcpy (szSrcBuff,szInstallPath);
    lstrcat (szSrcBuff,"rt.e2_");
    CopyAFile (szSrcBuff,szDestBuff,cmoAppend,1);
}

/* **************************************************************** */
BOOL FOnpServerNameDialog()
{
    CHAR rgchT[16];
    BOOL fRet;
    CHAR sz[cchMax];

    if(!GetONPServerName(sz))
        lstrcpy(sz, "ONPSERVER");
    if (!wUpdateServ) // GetOnpServerName sets this flag to false if failed
                      // to open services file in system32/drivers/etc dir
        return FALSE;
    lstrcpy(szOldServer,sz);
    StripSlash(sz);
    SetSymbolValue("EditTextIn", sz);
    SetSymbolValue("EditFocus", "END");

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_SERVICENAME, "FEditDlgProc",
            HELP_INSTALLPATH, "FHelpDlgProc", (LPSTR)rgchT, 15);

        if ((WORD)(rgchT[0]+rgchT[1]) == IDC_PATHEXIT)
            {
            AskQuit();
            continue;
            }
        else if ((WORD)(rgchT[0]+rgchT[1]) == IDC_PATHCANCEL
                || (WORD)(rgchT[0]+rgchT[1]) == IDC_CANCEL
                || (WORD)(rgchT[0]+rgchT[1]) == IDC_BACK)
            {
            fRet = FALSE;
            break;
            }
        else if (rgchT[0]+rgchT[1] != IDC_CONTINUE)
            continue;

        GetSymbolValue("EditTextOut", sz, cchMax);

        if (lstrlen(sz) == 0 || strchr(sz,':') || strchr(sz,'\\') || strchr(sz,'/'))
            {
            BlankPathDialog();
            continue;
            }

        fRet = TRUE;
        break;
        }
    lstrcpy (szONPServerName,sz);
    UIPop(1);
    return fRet;
}

void
AddONPServerName (char * szFile,char * szTmpFile)
{
    HFILE hFile;
    OFSTRUCT of;
    DWORD dwSize;
    char *pBuff,*pTmp;
    hFile = OpenFile (szFile,&of,OF_READWRITE);
    if (hFile == HFILE_ERROR){
        MessageBox (hWnd,"Couldn't open Services.ini file\r\nServices Info is not updated","Setup Message",MB_OK | MB_ICONEXCLAMATION);
        return;
    }
    dwSize = GetFileSize ((HANDLE)hFile,0) ;
    pBuff = (char*)GlobalAlloc (GPTR,dwSize + 50);
    ReadFile ((HANDLE)hFile,pBuff,dwSize,&dwSize,0);
    _lclose (hFile);
    remove (szTmpFile);
    rename (szFile,szTmpFile);
    hFile = OpenFile (szFile,&of,OF_CREATE);
    pTmp = strstr (pBuff,"ONPNAME");
    if (pTmp){
        *pTmp = '\0';
        pTmp += lstrlen ("ONPNAME");
    }
    _lwrite (hFile,pBuff, lstrlen (pBuff));
    if (pTmp){
        _lwrite (hFile,szONPServerName,lstrlen (szONPServerName));
        _lwrite (hFile,pTmp,lstrlen(pTmp));
    }
    _lclose (hFile);
}
void
SetPath (char * szPath,char ch)
{
    HKEY hKey;
    DWORD dwSize,dwType;
    char  szBuff[_MAX_PATH * 100] = "\0",*pTmp,*pPath,szPathAddon[_MAX_PATH];

    if(!wUpdatePath)
        return;
    dwSize = sizeof (szBuff);
    RegOpenKey (hRootKey,"System\\CurrentControlSet\\Control\\Session Manager\\Environment",&hKey);
    RegQueryValueEx (hKey,"Path",0,&dwType,szBuff,&dwSize);
    /* Incase of network installation we will have install path as z:\ppnt, where is z is the
     * network drive, and it may be equal to either 'c' or 'd' on remote machine.
     * so get the info of drive and modify install path and update
     */
    pPath = GetRealPath(szPathAddon,szPath);
    if (ch =='i') {
        if (!strstr (szBuff,pPath)){
            lstrcat (szBuff,";");
            lstrcat (szBuff,pPath);
            RegSetValueEx (hKey,"Path",0,REG_EXPAND_SZ,szBuff,lstrlen (szBuff)+1);
            }
        }
    else
        if (pTmp = strstr(szBuff,pPath)) {
            *pTmp = '\0';
            /* Strip of the ';' added to path */
            // This check is necessary to find if path we are looking is
            // not in the begining.If it is then we will not have ';'.
            if (szBuff != pTmp)
                if (';' == *(pTmp-1))
                    *(pTmp-1) = '\0';
            pTmp += lstrlen(pPath);
            lstrcat(szBuff,pTmp);
            RegSetValueEx (hKey,"Path",0,REG_EXPAND_SZ,szBuff,lstrlen (szBuff)+1);
            }

    RegCloseKey (hKey);
}
//************************************************************************************
// Following Function was added in the last minute
// paths are hard code, later on change, the files got installed to
// contain the path. Right now move them into the place where wanted
//*****************************************************************************
void
CopyLastMinuteFiles()
{
    char szSrcBuff[MAX_PATH],szDestBuff[_MAX_PATH];

/*    lstrcpy (szSrcBuff,szInstallPath);
     if ( lstrcmpi (szInstallPath,"c:\\MOA\\")){
    }
    lstrcpy (szSrcBuff,szInstallPath);
    lstrcat (szSrcBuff,"olivett3.lgo");
    CopyAFile (szSrcBuff,"c:\\jetform4\\logos\\olivett3.lgo",cmoOverwrite,0);

    lstrcpy (szSrcBuff,szInstallPath);
    lstrcat (szSrcBuff,"bin\\fin.ini");
    CopyAFile (szSrcBuff,"c:\\winnt\\fin.ini",cmoOverwrite,0);*/

	MyGetWindowsDirectory(szDestBuff,_MAX_PATH);
	strcat(szDestBuff,"\\system32");
	strcat(szDestBuff, "\\moamsg.dll");
    lstrcpy (szSrcBuff,szInstallPath);
    lstrcat (szSrcBuff,"bin\\moamsg.dll");

	MyGetWindowsDirectory(szDestBuff, _MAX_PATH);
	lstrcat(szDestBuff, "\\ubrm.ini");

	lstrcpy(szSrcBuff, szInstallPath);
	lstrcat (szSrcBuff, "bin\\ubrm.ini");
	CopyFile(szSrcBuff, szDestBuff, TRUE);
    //CopyAFile(szSrcBuff,szDestBuff,cmoOverwrite,0);
 
}
void UpdatePrinterInfo()
{
    char szSectionBuff[256],szKeyBuff[256],szValueBuffer[256];
    char szSourceFile[_MAX_PATH],szDestFile[_MAX_PATH];
    char *pSectionBuff,*pKeyBuff;

    if(!wUpdatePrinter)
        return;
    // Initialize source and destination file names
    lstrcpy (szSourceFile,szInstallPath);
    lstrcat (szSourceFile,"src\\driver\\files\\rtprint.inf");
    MyGetWindowsDirectory (szDestFile,MAX_PATH);
    lstrcat (szDestFile,"\\system32\\printer.inf");

    GetPrivateProfileString ( NULL,NULL,"default",szSectionBuff,256,szSourceFile);
    pSectionBuff = szSectionBuff;
    while ( lstrlen (pSectionBuff)){
            pKeyBuff = szKeyBuff;
            GetPrivateProfileString ( pSectionBuff,NULL,"default",szKeyBuff,256,szSourceFile);
            while (lstrlen (pKeyBuff)){
                GetPrivateProfileString (pSectionBuff,pKeyBuff,"default",szValueBuffer,256,szSourceFile);
                WritePrivateProfileString (pSectionBuff,pKeyBuff,szValueBuffer,szDestFile);
                pKeyBuff += lstrlen (pKeyBuff) +1 ;
            }
            pSectionBuff += lstrlen (pSectionBuff) +1;
    }

    WritePrivateProfileString (NULL,NULL,NULL,szDestFile);

}

void
StopServices ()
{
    SERVICE_STATUS serv_stat;
    SC_HANDLE   schSCManager;
    SC_HANDLE   schService;
    int i,j;
    DWORD dwErrorCode;
    char szMessageBuff[100];

    schSCManager = OpenSCManager(
                        szMachine,              // machine (NULL == local)
                        NULL,                   // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS   // access required
                        );
    j = sizeof (services) / sizeof (SERVICE);
    for (i = 0; i < j; i++){
        if(!services[i].wFlag)
            continue;
        schService = OpenService(schSCManager, services[i].serviceName, SERVICE_ALL_ACCESS);
        if (schService != NULL) {
            if (!ControlService (schService,SERVICE_CONTROL_STOP,&serv_stat)){
                dwErrorCode = GetLastError();
                switch(dwErrorCode) {
                    case  ERROR_ACCESS_DENIED:
                        wsprintf(szMessageBuff,"%s service will not be installed. Access denied",\
                                    services[i].serviceName);
                        MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK |MB_ICONINFORMATION);
                        services[i].wFlag = (WORD) 0;
                        break;
                    case ERROR_DEPENDENT_SERVICES_RUNNING:
                        wsprintf(szMessageBuff,"%s service will not be installed \r\n \
                        There are other running services dependent on it. ",services[i].serviceName);
                        MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK |MB_ICONINFORMATION);
                        services[i].wFlag = (WORD) 0;
                        break;
        /*          case ERROR_INVALID_SERVICE_CONTROL:
                        wsprintf(szMessageBuff,"%s service will not be installed \r\n \
                        Donot have permission to stop the service. ",services[i].serviceName);
                        MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK |MB_ICONINFORMATION);
                        services[i].wFlag = (WORD) 0;
                        break;
                    case ERROR_SERVICE_CANNOT_ACCEPT_CTRL:
                        wsprintf(szMessageBuff,"%s service will not be installed \r\n \
                        Service is not responding to stop request. ",services[i].serviceName);
                        MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK |MB_ICONINFORMATION);
                        services[i].wFlag = (WORD) 0;
                        break;*/
                    case ERROR_SERVICE_NOT_ACTIVE:
                        break;
                    default:
                        break;
                    }
                }
            }

        }
    CloseServiceHandle(schSCManager);
}

BOOL InstallOptionsDialog(char ch)
{
	BOOL bRet=TRUE;
	BOOL bWin95;
    CHAR rgchT[16];
    WORD wCheck[10];
    char szSymBuf[cbSymBuf];
    int i,cItems,cb;
    int nButton;

	// There's a new checkbox item for "delete directory tree" in uninstall.
    if ( ch == 'u' || ch == 'U' )
    	cItems = 7;
	else
		cItems = 6;

	bWin95 = bRunningWin95();

	for ( i = 1; i <= cItems; i++ )
    {
		// if win95 turn off and disable disable button3 and 4.
		if ( bWin95 && ((i == 3) || (i == 4)) )
		{
    		FAddListItem ( "CheckItemsState", "OFF" ) ;
		}
		else
		{
			FAddListItem ( "CheckItemsState", "ON" ) ;
		}
    }

	if ( bWin95 )
	{
		FAddListItem ( "OptionsGreyed", "3" ) ;
		FAddListItem ( "OptionsGreyed", "4" ) ;			
	}

    wInstAuxPrint = wInstWincom = wRemoveDirTree = /*wInstRDateAt = wInstRDateInitTab = */wInstRemServer = 1;
    // At one point, I saw that a=b=c was assigning c to a, but not to b.  Really.  I swear. (PL)
    Assert (( wInstAuxPrint == wInstWincom ) && ( wInstAuxPrint == wInstRemServer ));

    FAddListItem ( "CheckItemsIn", wInstAuxPrint?"ON":"OFF" );
    FAddListItem ( "CheckItemsIn", wInstRemServer?"ON":"OFF" );
   
   #if 0
    FAddListItem ( "CheckItemsIn", wInstRDateInitTab?"ON":"OFF" );
    FAddListItem ( "CheckItemsIn", wInstRDateAt?"ON":"OFF" );
   #endif

    while (TRUE)
    {
        if (ch == 'i')
            UIStartDlg("tsetuph.dll", DLG_CHECK, "FCustInstDlgProc",
                HELP_INSTALLTYPE, "FHelpDlgProc", (LPSTR)rgchT, 15);
        else
            UIStartDlg("tsetuph.dll", DLG_UNINSTALL_CHECK, "FCustInstDlgProc",
                HELP_INSTALLTYPE, "FHelpDlgProc", (LPSTR)rgchT, 15);

        // This dialog function is re-called whenever user input happens - to a button, 
        // a checkbox, etc.  When we get back to this point, the rgchT contains a string
        // referencing the id which was hit - e.g. EXIT, CONTINUE, CHK4, etc.
        switch ((WORD)(rgchT[0]+rgchT[1]))
        {
            case IDC_TYPEEXIT:
                AskQuit();
                continue;

			case IDC_CANCEL:
			case IDC_BACK:
            	bRet = FALSE;
            	break;

            // User hit a pushbutton
            case ('B'+'T'):
                // There's only one button the user could have pushed in this dialog, so 
                // call the handler.
                ServicesDialog ( ch ) ;

                // If the user deselected all the checkboxes in the dialog, then we will also 
                // deselect the services checkbox out here, and disable the pushbutton.
                if (!(wInstAuxPrint || wInstRDateAt || wInstRDateInitTab || wInstRemServer))
                {
                    FlipCheckBox ( "CheckItemsState", 3 ) ;
                    bCheckBoxEnabled = !bCheckBoxEnabled;
                    CheckDlgButton ( GetActiveWindow(), IDC_B2, FALSE ) ;
                    EnumChildWindows ( GetActiveWindow(), EnDisServicesButton, DISABLE ) ;
                }
                continue;    


            // User hit a checkbox
            case ('C'+'H'):
            {
                nButton = rgchT[3] - '0';
                Assert (nButton <= cItems);

                switch (nButton)
                {
                    case 1: 
                    case 2: 
                    case 4: 
                    case 5: 
                    case 6: 
                    case 7: 
					case 8:
                        FlipCheckBox ( "CheckItemsState", nButton ) ;
                        break;

                    case 3:                        
                        //EnumChildWindows ( GetActiveWindow(), FlipCheckBox, nButton ) ;
                        FlipCheckBox ( "CheckItemsState", nButton ) ;
                        bCheckBoxEnabled = !bCheckBoxEnabled;

                        // if the checkbox is off, do this.  If you turned it back on, 
                        // re-enable the button.
                        // if checkbox was just cleared
                        if ( bCheckBoxEnabled ) 
                        {
                            EnumChildWindows ( GetActiveWindow(), EnDisServicesButton, DISABLE ) ;
                            wInstAuxPrint = wInstWincom = /*wInstRDateAt = wInstRDateInitTab = */wInstRemServer = 0;
                            Assert ( (wInstAuxPrint==wInstRDateAt  ) && ( wInstAuxPrint == wInstRemServer ));
                        }
                        else
                        {
                            EnumChildWindows ( GetActiveWindow(), EnDisServicesButton, ENABLE ) ;    
                            wInstAuxPrint = /*wInstRDateAt = wInstRDateInitTab = */wInstRemServer = 1;
                            Assert ( wInstAuxPrint == wInstRemServer );
                        }
                        break;
                        

                } // end switch nbutton
                continue;

            }  // end case checkboxes
            continue; 



            default:
                continue;

            case IDC_CONTINUE:
				// Check if IRQ on network Card.
				if (IsInterruptTwoOK())
                	break;
				else {
					char szBuff[256];
					wsprintf(szBuff,"%s\r\n%s\r\n%s","Setup detected that Interrupt 2 is already in use", 
									"by a network card. If you continue, the Wincom",
									"driver will not be installed."
									);
									
					if (IDOK == MessageBox(GetActiveWindow(),szBuff,"Setup Message",MB_OKCANCEL|MB_ICONINFORMATION)){
						wInstAuxPrint = 0;
						wInstWincom = 0;
						break;
						}
					else
						continue;					

					}
        } // end switch
            
        for (i = 1; i <= cItems; i++)
        {
            cb = CbGetListItem("CheckItemsState", i, szSymBuf, cbSymBuf);
            Assert(cb < cbSymBuf);
            if (lstrcmp(szSymBuf, "ON") == 0)
                wCheck[i-1] = (WORD)1;
            else
                wCheck[i-1] = (WORD)0;
        } // end for

        wCopyRunTimeFiles = wCheck[0];
        wCopyDevFiles =  wCheck[1];
        wInstServices = wCheck[2];
        //Services file is being updated by demo application setup         
        //wUpdateServ = wCheck[3];
        wInstSerial = wCheck[3];
        wUpdatePath = wCheck[4];
		wSetFonts = wCheck[5];
		if ( ( ch == 'u' ) || ( ch == 'U' ))
		{
			wRemoveDirTree = wCheck[6];
        }

        wUpdatePrinter = wInstAuxPrint || wInstWincom; 
        
        services[0].wFlag = services[3].wFlag= DriverInfo[0].wFlag = wInstWincom;
        // At one point, a=b=c was assigning c to b, but not to a.  Really.  I swear. (PL)
        Assert ((services[0].wFlag == DriverInfo[0].wFlag ) && (services[0].wFlag == wInstWincom )) ;

        services[1].wFlag = wInstRemServer;
        
        services[4].wFlag = DriverInfo[1].wFlag = wInstSerial;
        Assert ((services[4].wFlag == DriverInfo[1].wFlag ) && (services[4].wFlag == wInstSerial )) ;        

        #ifdef DEBUG_LEVEL_2
        DumpGlobalFlags();
        #endif

		//if ((ch != 'u') && (ch != 'U')) 		
		SaveSetupOptions();

        if (wInstServices || wInstSerial)
            bShutDown = TRUE;

        UIPop(1);
        return bRet;
    }  // end while (TRUE)
}


BOOL CALLBACK EnDisServicesButton ( HWND hwnd, LPARAM l )
{
    //  if the id of the current window isn't the button's ID, continue enumeration
   
    if ( IDC_SP1 == GetWindowLong ( hwnd, GWL_ID))
    {
        // o or enable the button control
        EnableWindow ( hwnd, (BOOL)l );
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}




BOOL FlipCheckBox ( LPSTR szSymbol, int nButton )
{
    int cb;
    char szSymBuf[cbSymBuf];

    //  if the id of the current window isn't the button's ID, continue enumeration
    cb = CbGetListItem(szSymbol, nButton, szSymBuf, cbSymBuf);
    Assert ( cb != 0 );

    if (lstrcmp (szSymBuf, "ON") != 0)
    {
        return (FReplaceListItem(szSymbol, nButton, "ON"));
    }
    else
    {
        return (FReplaceListItem (szSymbol, nButton, "OFF"));
    }
}

void ServicesDialog (char ch)
{
    CHAR rgchT[16];
    WORD wCheck[10];
    int i,cItems,cb;
    char szSymBuf[cbSymBuf];

    cItems = 2;


    // ---------------------------------------------------------------------------
    // This code sets the initial values of the "services options" dialog to ON.
    // I would do this in the ServicesDialog() function, but the user can go into
    // that particular dialog multiple times, so the checkbox states need to be
    // saved across dialog invocations.
    // 
    // ---------------------------------------------------------------------------

    FReplaceListItem ( "CheckItemsIn", 1, wInstAuxPrint?"ON":"OFF" );
    FReplaceListItem ( "CheckItemsIn", 2, wInstRemServer?"ON":"OFF" );
   
   #if 0    
    FReplaceListItem ( "CheckItemsIn", 3, wInstRDateInitTab?"ON":"OFF" );
    FReplaceListItem ( "CheckItemsIn", 4, wInstRDateAt?"ON":"OFF" );
   #endif

  #ifdef __DEBUG 
    DumpServicesFlags();
  #endif

    while ( TRUE )
    {
        if (ch == 'i')
            UIStartDlg ( "tsetuph.dll", DLG_SERVICES, "FCheckDlgProc", 
                HELP_INSTALLTYPE, "FHelpDlgProc", (LPSTR) rgchT, 15 ) ;
        else
            UIStartDlg ( "tsetuph.dll", DLG_SERVICES_UNINSTALL, "FCheckDlgProc", 
                HELP_INSTALLTYPE, "FHelpDlgProc", (LPSTR) rgchT, 15 ) ;
                
        switch ((WORD)(rgchT[0]+rgchT[1]))
        {

            case IDC_CANCEL:
                break;

            case IDC_CONTINUE:
                // Save the checkbox settings.
                for (i = 1; i <= cItems; i++)
                {
                    cb = CbGetListItem("CheckItemsOut", i, szSymBuf, cbSymBuf);

                    Assert ((cb < cbSymBuf) && (cb > 0));
                    
                    if (lstrcmp(szSymBuf, "ON") == 0)
                        wCheck[i-1] = (WORD)1;
                    else
                        wCheck[i-1] = (WORD)0;
                } // end for
            
                wInstWincom = wInstAuxPrint = wCheck[0];
                // At one point, a=b=c was assigning c to b, but not to a.  Really.  I swear. (PL)
                Assert ((wInstWincom == wInstAuxPrint) && (wInstWincom == wCheck[0]));
                wInstRemServer =  wCheck[1];
                #if 0
                wInstRDateInitTab = wCheck[2];
                wInstRDateAt = wCheck[3];
				#endif
                break;
            case ('B'+'T'):
                // There's only one button the user could have pushed in this dialog, so 
                // call the handler.
                PrinterTypesDialog ( ch ) ;
                continue;    


        }   // end switch


        UIPop(1);
        return;
    }
}

void PrinterTypesDialog (char ch)
{
    CHAR rgchT[16];
    char szSymBuf[cbSymBuf];

	// Set symbols for initializing radio buttons
	wsprintf(szSymBuf,"%d",wPrinter);
    SetSymbolValue("RadioDefault",szSymBuf);
	wsprintf(szSymBuf,"%d",wComPort);
	SetSymbolValue("RadioDefault1",szSymBuf);
    while ( TRUE )
    {
        if (ch == 'i')
            UIStartDlg ( "tsetuph.dll", DLG_PRINTER_TYPE, "FPrinterDlgProc", 
                HELP_INSTALLTYPE, "FHelpDlgProc", (LPSTR) rgchT, 15 ) ;
		else 
			return;
        switch ((WORD)(rgchT[0]+rgchT[1]))
        {

            case IDC_CANCEL:
                break;

            case IDC_CONTINUE:
				// Get the symbol values set
				GetSymbolValue("ButtonChecked",szSymBuf,cbSymBuf);
				wPrinter = atoi(szSymBuf);
				GetSymbolValue("ButtonChecked1",szSymBuf,cbSymBuf);
				wComPort = atoi(szSymBuf);
                break;
			default :
				break;
 
        }   // end switch


        UIPop(1);
        return;
    }
}




//*********************************************************************************
// FUNCTION     DeleteDrivers
//
//**********************************************************************************

void
DeleteDrivers ()
{

    int i,j;
    char szKeyBuff[256];

    j = sizeof (DriverInfo) / sizeof (DRIVER);
    for (i = 0; i < j; i++)
    {
        if(!DriverInfo[i].wFlag)
            continue;
        wsprintf(szKeyBuff,"System\\CurrentControlSet\\Services\\%s",DriverInfo[i].DriverKey);
		// patl - todo - access denied.
        RegDeleteKey(hRootKey,szKeyBuff);
    }
}
/* **************************************************************** */
VOID WelcomeUnInstall()
{
        CHAR rgchT[16];

        while (TRUE)
            {
        UIStartDlg("tsetuph.dll", DLG_WELCOME_UNINSTALL, "FInfoDlgProc",
                HELP_APPLICATION, "FHelpDlgProc", (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        case IDC_CANCEL:
        case IDC_TYPEEXIT:
            AskQuit();
            continue;

        default:
            continue;

        case IDC_CONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}
/* **************************************************************** */
VOID AskUnInstallQuit()
{
    CHAR rgchT[16];

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_UNINSTALL_QUIT, "FQuitDlgProc", HELP_ASKQUIT,
            "FHelpDlgProc", (LPSTR)rgchT, 15);

        switch ((WORD)(rgchT[0]+rgchT[1]))
            {
        default:
            continue;

        case IDC_QUITEXIT:
            UIPopAll();
            EndSetupToolkit();
            exit(0);

        case IDC_CANCEL:
        case IDC_QUITCONTINUE:
            break;
            }

        UIPop(1);
        return;
        }
}

/* **************************************************************** */
BOOL FUnInstallPathDialog()
{
    CHAR rgchT[16];
    BOOL fRet;
    CHAR sz[cchMax];

    lstrcpy(sz, szInstallPath);

    StripSlash(sz);
    SetSymbolValue("EditTextIn", sz);
    SetSymbolValue("EditFocus", "END");

    while (TRUE)
        {
        UIStartDlg("tsetuph.dll", DLG_UNINSTALLPATH, "FEditDlgProc",
            HELP_INSTALLPATH, "FHelpDlgProc", (LPSTR)rgchT, 15);

        if ((WORD)(rgchT[0]+rgchT[1]) == IDC_PATHEXIT)
            {
            AskQuit();
            continue;
            }
        else if ((WORD)(rgchT[0]+rgchT[1]) == IDC_PATHCANCEL
                || (WORD)(rgchT[0]+rgchT[1]) == IDC_CANCEL
                || (WORD)(rgchT[0]+rgchT[1]) == IDC_BACK)
            {
            fRet = FALSE;
            break;
            }
        else if (rgchT[0]+rgchT[1] != IDC_CONTINUE)
            continue;

        GetSymbolValue("EditTextOut", sz, cchMax);

        if (lstrlen(sz) == 0)
            {
            BlankPathDialog();
            continue;
            }

        if (!FValidDir(sz))
            {
            InvalidPathDialog();
            continue;
            }

        lstrcpy(szInstallPath, sz);
        AppendSlash(szInstallPath);

        if (lstrcmp(szInstallPath, szSrcDir) == 0)
            {
            DestIsSrcDialog();
            continue;
            }

        fRet = TRUE;
        break;
        }

    UIPop(1);
    return fRet;
}

void
UnInstall()
{

    INT  iCursorSav,i=0;
    HWND hModelessDlg;
    CHAR rgchT[16];
    char szTextBuff[356];
    DWORD dwErrorCode;
    MSG msg;


    iCursorSav = ShowWaitCursor();
    StopServices();
    MyReadInfFile(pInfFileName);
    RestoreCursor(iCursorSav);
    UIStartDlg("tsetuph.dll", DLG_UNINSTALL, "FUnInstallDlgProc", 0, NULL,
            (LPSTR)rgchT, 16);
    GetModelessDialog(&hModelessDlg);
    if (wInstServices)
        DeleteServices();
    DeleteDrivers();
    InstallFonts('u');
    lstrcpy (szTextBuff,szInstallPath);
    lstrcat (szTextBuff,"BIN");
    SetPath (szTextBuff,'u');

	// The AT table must be cleaned of RDATE entries before you try to 
	// actually delete rdate.exe, or NT will deny access to the .exe, 
	// failing the deletion.
    if (wInstRDateAt)
    {            
        UninstallRDateAt();
    }
    if (wInstRDateInitTab)
    {
        UninstallInitTabFile ( ) ;
    }


    while (i < FileIndex)
    {
        wsprintf(szTextBuff,"Deleting file : %s%s",szInstallPath,FileInfo[i++].szFileName);
        SetUnInstallText(szTextBuff);
        if(!DeleteFile(szTextBuff +16)) 
        {
            char szError[256];
            char szMessageBuff[256];
            dwErrorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwErrorCode,
                            GetSystemDefaultLangID(),szError,255,NULL);
            wsprintf(szMessageBuff,"Failed to delete file \r\n%s",szError);
            MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK|MB_ICONINFORMATION);
        }
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
            if (!IsDialogMessage(hModelessDlg,&msg)) 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
		}
    }


	#if 0
	lstrcpy ( szTextBuff, szInstallPath ) ;
	lstrcat ( szTextBuff, "ctl3d32.dll" ) ;
	DeleteFile ( szTextBuff ) ;
	#endif

	// Lose the added trailing backslash.
	szInstallPath[strlen(szInstallPath)-1] = '\0';

	// Now delete the empty tree
	if ( wRemoveDirTree ) 
	{	
		SetCurrentDirectory ( szInstallPath ) ;
		bRemoveTree ( szInstallPath ) ;
		SetCurrentDirectory ( ".." ) ;
		RemoveDirectory ( szInstallPath ) ;
	}

 	// Finally, delete the Demo Appplication sections of the moasetup.ini/registry
	bDeleteSetupSection ( "Demo Application" ) ;
	bDeleteSetupDirectory ( "Demo Application" ) ;
}

//-************************************************************************
VOID MyReadInfFile(LPSTR szFile)
{
    BuildInfo (szFile);
}
char * StripOfLeadingSpaces (char *szLine)
{
    while (' ' ==  *szLine || '\t' == *szLine || '\r' == *szLine || '\n' == *szLine || '\\' == *szLine || '/' == *szLine) {
        szLine++;
        if (!*szLine)
            break;
        }
    return szLine;
}


//*************************************************************************

void BuildInfo ( char *pFileName)
{
 char *pLine,*pTmp;
 char szLine [400];
 int smdIndex = 0;
 int ReadState,i;
 FILE *fileP;

 FileInfo = (FILESINFO *) GlobalAlloc (GPTR, sizeof (FILESINFO) * 5000);
 fileP = fopen(pFileName,"r");

 	// This check was added because BuildINfo would GPFault if moa.inf didn't exist.
	if ( fileP == NULL )
	{
		wsprintf ( szLine, "Error!  Could not open %s, please correct problem and re-run setup!", pFileName ) ;
		MessageBox ( NULL, szLine, "Setup Error", MB_OK | MB_ICONEXCLAMATION ) ;

		UIPopAll();
		EndSetupToolkit();
		exit(0);
	}


  while (fgets(szLine,399,fileP)){
    pLine = StripOfLeadingSpaces (szLine);
    if ( '[' == *pLine){
        ReadState = FALSE;
        if (!lstrcmpi (pLine , "[Source Media Descriptions]\n"))
			// Patl - should this be 1?  Comments seem to match, but don't do it now.
            ReadState = 1 ;//SMDINFO; // was 3
        else
        if (strstr (pLine, "[Runtime files]") && wCopyRunTimeFiles)
		{
			if (bIsInstalled ( "Runtime Files" ))
				ReadState = 2;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Runtime Files but they " 
									"are not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}
				
		}

#if 0
		else if (strstr (pLine, "[Runtime SLIM files]") && wCopyRunTimeFiles)
		{
			if (bIsInstalled ( "Runtime SLIM Files" ))
				ReadState = 2;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Runtime Files but they " 
									"are not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}				
		}
		
		else if (strstr (pLine, "[Runtime FAT files]") && wCopyRunTimeFiles)
		{
			if (bIsInstalled ( "Runtime FAT Files" ))
				ReadState = 2;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Runtime Files but they " 
									"are not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}			
		}
#endif		

        else if(wCopyDevFiles && strstr (pLine, "[Development files]"))
        {
			if ( bIsInstalled ("Development Environment"))
				ReadState = 2;//FILESINFO;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Development Files but they " 
									"are not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}
			
		}
		
		else if (strstr (pLine, "[Serial Sharing]" ) && wInstSerial)
        {
			if ( bIsInstalled ("Serial Device Sharing"))
				ReadState = 2;//FILESINFO;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Serial Sharing but it " 
									"is not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}
			
		}

		else if (strstr (pLine, "[Fonts]" ) && wSetFonts)
        {
			if ( bIsInstalled ("Fonts"))
				ReadState = 2;//FILESINFO;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Fonts but they " 
									"are not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}
			
		}

		else if (strstr (pLine, "[Auxprint and Wincom]" ) && (wInstAuxPrint || wInstWincom))
		{
			if ( bIsInstalled ("Auxprint and Wincom"))
				ReadState = 2;//FILESINFO;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Auxprint and Wincom but they " 
									"are not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}
			
		}

		else if (strstr (pLine, "[Remote COM Server]" ) && wInstRemServer)
		{
			if ( bIsInstalled ("Remote COM Server"))
				ReadState = 2;//FILESINFO;
			else
			{	
				MessageBox ( NULL, "Warning!  You asked to uninstall Remote COM Server but it " 
									"is not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
			}
			
		}

		else if ( strstr ( pLine, "[RDate]" ) && (wInstRDateAt || wInstRDateInitTab ))
		{
			if (wInstRDateAt)
			{
				if ( bIsInstalled ( "RDate At" ) )
		    		ReadState = 2;
				else
				{	
					MessageBox ( NULL, "Warning!  You asked to uninstall RDate running as a scheduled AT service but it " 
									"is not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
				}
			}

			if (wInstRDateInitTab)
			{
				if ( bIsInstalled ( "RDate Inittab" ) )
		    		ReadState = 2;
				else
				{	
					MessageBox ( NULL, "Warning!  You asked to uninstall RDate running as an InitTab entry but it" 
									"is not installed.  Setup will ignore this selection.", 
									"Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
				}
			}

		}
		else if (bCopyAnyFiles() && !bUnInstall && strstr (pLine, "[Shared]" ) && bCopyAnyFiles())
		    ReadState = 2;
        else
            ReadState = 3;
        continue;
    }

    switch (ReadState){
        case 1://SMDINFO:
            pTmp = strchr (pLine,',');
			if (!pTmp)
				break;
            *pTmp = '\0';
            i = (int)atol (pLine);
            smdInfo[smdIndex].DiskNo = atol (pLine);
            pLine = ++pTmp;
            pTmp = strchr (pLine,',');
            *pTmp = '\0';
            lstrcpy (smdInfo[smdIndex].szDiskDesc,pLine);
            pLine = ++pTmp;
            pTmp = strchr (pLine,',');
            *pTmp = '\0';
            lstrcpy (smdInfo[smdIndex].szDiskStampFile,pLine);
            break;
        case 2://FILEINFO:
            pTmp = strchr (pLine,',');
			if (!pTmp)
				break;
            *pTmp = '\0';
            FileInfo[FileIndex].DiskNo = atol (pLine);
            pLine = ++pTmp;
            pTmp = strchr (pLine,',');
            *pTmp = '\0';
            pLine = StripOfLeadingSpaces (pLine);
            lstrcpy (FileInfo[FileIndex].szFileName,pLine);
            pLine = ++pTmp;
            for (i = 0; i < 14;i++){
                pTmp = strchr (pLine,',');
                if (pTmp == pLine)
                    pLine++;
                else
                    pLine = pTmp;
            }
            pTmp = strchr (pLine,',');
            *pTmp = '\0';
            FileInfo[FileIndex].dwFileSize = atol (pLine);
			if ( bSplitFileName ( FileInfo[FileIndex].szFileName ) )
			{
				//if you're on foo.d0l, add the glued filename, otherwise don't.
				// And don't increment the index
				// Add the split glued name
				if ( bZerothSplitFile ( FileInfo[FileIndex].szFileName ) )
				{
					lstrcpy (FileInfo[FileIndex].szFileName,szReturnUnsplitName(FileInfo[FileIndex].szFileName));
					FileIndex++;
				}
				// else don't increment the file index, and just overwrite the 
				// existing entry on the next file - if it's not a split name.
			}
            else
			{
            	FileIndex++;
			}

            if (FileIndex == 5000)
                FileInfo = GlobalReAlloc (FileInfo, GlobalSize ((HANDLE)FileInfo) * 2 ,GMEM_ZEROINIT);
            break;
        default:
            break;
    }
 }
}


//*********************************************************************************
// FUNCTION     InstallFonts
//
// PURPOSE      This routine either adds or deletes the fonts
//**********************************************************************************

void
InstallFonts (char ch)
{

    int i,j;
    char szDestBuff[_MAX_PATH],szSrcBuff[_MAX_PATH];
    char szError[256];
    char szMessageBuff[256],*pSrc,*pDest;
    DWORD dwErrorCode;
    OFSTRUCT    ofstruct;
    HFILE hFile;

    if (!wSetFonts)
        return;
    MyGetWindowsDirectory(szDestBuff,MAX_PATH);
    lstrcat(szDestBuff,"\\system\\");
    pDest = szDestBuff + lstrlen(szDestBuff);


    j = sizeof (szFonts) / sizeof (char*);
    if ('i' == ch) {
        wsprintf(szSrcBuff,"%sfonts\\",szInstallPath);
        pSrc = szSrcBuff + lstrlen(szSrcBuff);
        for (i = 0; i < j; i++) {
            // If successfully added fonts to to registry then proceed
            if (UpdateFontsInRegistry(ch,i) && !bNetInstall) {
                lstrcat(szDestBuff,szFonts[i]);
                lstrcat(szSrcBuff,szFonts[i]);
                hFile = OpenFile(szDestBuff,&ofstruct,OF_READWRITE);
                if (HFILE_ERROR == hFile) {
                    dwErrorCode = GetLastError();
                    if (dwErrorCode == ERROR_SHARING_VIOLATION) {
                            dwErrorCode = RemoveFontResource(szDestBuff);
                            SendMessage(HWND_BROADCAST,WM_FONTCHANGE,(WPARAM)0,(LPARAM)0);
                        }
                    }
                _lclose(hFile);
                CopyAFile(szSrcBuff,szDestBuff,cmoOverwrite,0);
                if (!AddFontResource(szDestBuff)) {
                    dwErrorCode = GetLastError();
                    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwErrorCode,
                            GetSystemDefaultLangID(),szError,255,NULL);
                    wsprintf(szMessageBuff,"Failed to add font %s \r\n%s",\
                            szFonts[i],szError);
                    MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK|MB_ICONINFORMATION);
                    }
                }
                *pDest = '\0';
                *pSrc = '\0';

            }
        }
    else {
        for (i = 0; i < j; i++){
            lstrcat(szDestBuff,szFonts[i]);
            // If successfully deleted fonts from registry then proceed
            if (UpdateFontsInRegistry(ch,i) && !bNetInstall) {
                if (!RemoveFontResource(szDestBuff)) {
                    dwErrorCode = GetLastError();
                    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwErrorCode,
                                GetSystemDefaultLangID(),szError,255,NULL);
                    wsprintf(szMessageBuff,"Failed to remove font file %s \r\n%s",\
                                szFonts[i],szError);
                    MessageBox(hWnd,szMessageBuff,"Setup Message",MB_OK|MB_ICONINFORMATION);
                    }
                *pDest = '\0';
                }
            }
        }
    SendMessage(HWND_BROADCAST,WM_FONTCHANGE,(WPARAM)0,(LPARAM)0);

}

void
UpdateServerName (char *pServFile,char *pBuff)
{
    char *pTmp;
    char szDestBuff[MAX_PATH];
    HFILE hFile;
    OFSTRUCT of;

    pTmp = strstr(pBuff,szOldServer);
    if (pTmp) {
        /* create a backup of services file,incase some thing goes wrong*/
        *pTmp = '\0';
        lstrcpy(szDestBuff,pServFile);
        lstrcat(szDestBuff,".bak");
        CopyAFile(pServFile,szDestBuff,cmoOverwrite,0);
        remove(pServFile);
        hFile = OpenFile (pServFile,&of,OF_CREATE);
        _lwrite(hFile,pBuff, lstrlen (pBuff));
        _lwrite(hFile,szONPServerName,lstrlen(szONPServerName));
        pTmp += lstrlen(szOldServer);
        _lwrite(hFile,pTmp,lstrlen(pTmp));
        }
}

/*
 * GetONPServerName : This funciton gets the name of the ONPSERVER
 *          from services file located in /winnt/system32/drivers/etc
 *          This function greatly depends on the order in which the information
 *          is put in the file.It works fine if it isn't modified,
 *          Since it's an ascii file any one can modify that file.
 *          There were warnings put in the files file not to modify it.
 */
BOOL
GetONPServerName (char *pNameBuff)
{
    char szDestBuff[_MAX_PATH],*pTmp,*pTmp1,*pBuff;
    DWORD   dwSize;
    HFILE hFile;
    OFSTRUCT of;

    MyGetWindowsDirectory(szDestBuff,MAX_PATH);
    lstrcat(szDestBuff,"\\system32\\drivers\\etc\\services");
    hFile = OpenFile (szDestBuff,&of,OF_READWRITE);
    dwSize = GetFileSize((HANDLE)hFile,0) ;
    pBuff = (char*)GlobalAlloc (GPTR,dwSize + 10);
    if (hFile == HFILE_ERROR){
        MessageBox (hWnd,"Couldn't open Services file\r\nServices Info is not updated","Setup Message",MB_OK | MB_ICONEXCLAMATION);
        wUpdateServ = 0;
        return FALSE;
    }
    ReadFile ((HANDLE)hFile,pBuff,dwSize,&dwSize,0);
    _lclose (hFile);
    pTmp = strstr (pBuff,szServSearchStr);
    if(pTmp)
        pTmp = strchr(pTmp,'\n');
    if (pTmp) {
        pTmp++;
        pTmp = strchr(pTmp,'\n');
        if (pTmp){
            pTmp++;
            if (pTmp1 = strstr(pTmp,":NetBoss"))
                *pTmp1 = '\0';
            if (lstrlen(pTmp) < 50){
                lstrcpy(pNameBuff,pTmp);
                return TRUE;
                }
            }
        }
    return FALSE;
}

void InitNetInstall(char* szCommand)
{
    char* pCommand = szCommand;
    char szInstallType[25];

    if ( !strstr (szCommand,"-n")){
        szCommand[0] = '\0';
        return ;
        }
    pCommand += 2; // to get rid if -n from commnad line
    // First argument is machine name
    pCommand = CopyWord(pCommand,szMachine);
    // Second argument is Install Directory name
    pCommand = CopyWord(pCommand,szInstallPath);
    AppendSlash(szInstallPath);
    // Third argument is Windows directory name
    pCommand = CopyWord(pCommand,szWinDir);
    // Fourth argument is Install type
    pCommand = CopyWord(pCommand,szInstallType);
    if ( !lstrcmp(szInstallType,"1"))
        iCustomSetup = 1;
    // Set GlobalVariable for identifying it is network installation.
    bNetInstall = TRUE;
    //Initialize hRootKey Global variable
    RegConnectRegistry(szMachine,HKEY_LOCAL_MACHINE,&hRootKey);
    szCommand[0] = '\0';
}

char* CopyWord(char *pCommand,char *pRecieveBuffer)
{
    int i;
    i = 0;
    while (' ' ==  *pCommand || '\t' == *pCommand || '\r' == *pCommand || '\n' == *pCommand)
        pCommand++;
    while (TRUE){
            pRecieveBuffer[i] = *pCommand;
            pCommand++;
            i++;
            if (*pCommand == ' ' || *pCommand == '\t' || *pCommand == '\0')
                break;
            }
        pRecieveBuffer[i] = '\0'; // Null terminater
    return pCommand;
}

// This function sets all variables used for install option
// to TRUE.
void SetAllOptions()
{

        wCopyRunTimeFiles = 1;
        wCopyDevFiles = 1;
        wInstServices = services[0].wFlag = 1;
        wInstSerial = 1;
        wUpdateServ = 0;		// DO_SERVICE : -> 1
        wUpdatePrinter = 1;
        wUpdatePath = 1;

        services[1].wFlag = DriverInfo[0].wFlag = wInstWincom;
        services[2].wFlag = DriverInfo[1].wFlag = wInstSerial;
        wSetFonts = 1;
}

void MyGetWindowsDirectory(char* pRecieveBuff,int iSize)
{
    if (bNetInstall)
        lstrcpy(pRecieveBuff,szWinDir);
    else
        GetWindowsDirectory(pRecieveBuff,iSize);

}

void CreateProgGroupOnRemote()
{
    DWORD dwIdInst = 0;
//  NDDESHAREINFO nddeShare, *pShare;
    HCONV hConv;
    HSZ hszService,hszTopic;
    char szString[75];
    char szExecString[1000] = "\0";
    DWORD i = 0,j = 0;
    // The following code is commented but not deleted coz, incase we decide
    // to create Share via Setup, instead of ddeshare app.

#ifdef SHARESETUP
    uint = NDdeShareGetInfo(NULL,"$1",2,NULL,0,&i,&j);
    if (NDDE_NO_ERROR != uint)
        NDdeGetErrorString(uint,szExecString,1000);
    i = 0;j = 0;
    pShare = (NDDESHAREINFO*)szExecString;
    uint = NDdeShareEnum(NULL,0,(LPBYTE)szExecString,1000,&i,&j);
    if (NDDE_NO_ERROR != uint)
        NDdeGetErrorString(uint,szExecString,1000);
    i =0;j=0;
    uint = NDdeShareGetInfo("\\\\PRAV_PC","Chat$",2,szExecString,1000,&i,&j);
    if (NDDE_NO_ERROR != uint)
        NDdeGetErrorString(uint,szExecString,1000);

    nddeShare.lRevision = 1;
    nddeShare.lpszShareName = "progman$";
    nddeShare.lShareType = SHARE_TYPE_OLD |SHARE_TYPE_NEW |SHARE_TYPE_STATIC;
    nddeShare.lpszAppTopicList = "PROGMAN|PROGMAN\0PROGMAN|PROGMAN\0PROGMAN|PROGMAN\0\0";//<PROGMAN|PROGMAN>\0<PROGMAN|PROGMAN>\0\0";
    nddeShare.fSharedFlag = NDDE_SHARE_GENERIC_ALL ;
    nddeShare.fService = TRUE;
    nddeShare.fStartAppFlag = FALSE;
    nddeShare.qModifyId[0] = 0;
    nddeShare.qModifyId[1] = 0;
    nddeShare.cNumItems =0;
    nddeShare.lpszItemList = "";
    pShare->lpszShareName = "Progman$";
    pShare->lpszAppTopicList = "\0PROGMAN|PROGMAN\0\0";
    uint= NDdeShareAdd("\\\\PPNT-VAI",2,NULL,pShare,1000);
    if (NDDE_NO_ERROR != uint )
        NDdeGetErrorString(uint,szExecString,100);
    uint= NDdeShareAdd("\\\\PPNT-VAI",2,NULL,&nddeShare,100);
    if (NDDE_NO_ERROR != uint )
        NDdeGetErrorString(uint,szExecString,100);
#endif

    DdeInitialize(&dwIdInst,DdeCallback,APPCMD_CLIENTONLY,0);
    wsprintf(szString,"%s\\NDDE$",szMachine);
    hszService = DdeCreateStringHandle(dwIdInst,szString,CP_WINANSI);
    hszTopic = DdeCreateStringHandle(dwIdInst,"Progman$",CP_WINANSI);

    hConv = DdeConnect(dwIdInst,hszService,hszTopic,NULL);
    wsprintf(szExecString,"[CreateGroup(\"PPNT\")]\0");
    //Create program group
    // wait for two minutes, if couldn't do it in 2min then forget it.
    DdeClientTransaction(szExecString,lstrlen(szExecString),hConv,0L,0,
                XTYP_EXECUTE,120000,NULL);
    // Create program item
    DdeFreeStringHandle(dwIdInst,hszService);
    DdeFreeStringHandle(dwIdInst,hszTopic);
    DdeDisconnect(hConv);
    DdeUninitialize(dwIdInst);
}
HDDEDATA CALLBACK DdeCallback(uType, uFmt, hconv, hsz1, hsz2, hdata, dwData1,dwData2)

UINT uType; /* transaction type */
UINT uFmt;  /* clipboard data format    */
HCONV hconv;    /* handle of the conversation   */
HSZ hsz1;   /* handle of a string   */
HSZ hsz2;   /* handle of a string   */
HDDEDATA hdata; /* handle of a global memory object */
DWORD dwData1;  /* transaction-specific data    */
DWORD dwData2;  /* transaction-specific data    */

{
    HDDEDATA hData = 0;
    return hData;
}
// UpdateFontsInRegistry
//  This funciton update registry with new fonts.
//
BOOL UpdateFontsInRegistry(char ch, int i)
{
    HKEY hKey;
    char szBuff[50];
    //char szMsgBuff[150];
    DWORD dwType;
    DWORD dwSize = 50;
    LONG lRetVal;

    lRetVal = RegOpenKey(hRootKey,"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",&hKey);
    if ('i' == ch) {
        if (ERROR_SUCCESS == lRetVal) {
            lRetVal = RegQueryValueEx   (hKey,szFontNames[i],0,&dwType,szBuff,&dwSize);
            if(dwSize && (ERROR_SUCCESS == lRetVal)) {
              // PSR #667 - request to silently update fonts if they already
              // exist.  Fix: #ifdef the messagebox code, just force the RegCloseKey.
              #if 0
                wsprintf(szMsgBuff,"The %s fonts are already installed \r\nDo you want to replace them ?",szFontNames[i]);
                if (IDOK != MessageBox(hWnd,szMsgBuff,"Setup Message",MB_ICONQUESTION |MB_OKCANCEL)) {
              #endif
                    RegCloseKey(hKey);
                    return FALSE;
                    // if bbacking out #667 fix, uncomment this close curly.
                    // }
                }
            lRetVal = RegSetValueEx(hKey,szFontNames[i],0,REG_SZ,szFonts[i],lstrlen(szFonts[i]) + 1);
            }
        }
    else    // uninstall
        RegDeleteValue(hKey,szFontNames[i]);
    RegCloseKey(hKey);
    if (ERROR_SUCCESS == lRetVal)
        return TRUE;
    return FALSE;
}


/* GetRealPath
 * This routine checks if the path contains either 'Z' or 'Y' as drive and get's the
 * remote drive name and creates a new path based on that.
 */

char* GetRealPath(char* pNewPath, char* pPath)
{
    char ch[10];
    lstrcpy(pNewPath,pPath);
    if (bNetInstall){
        char szRemoteName[50];
        if (bNetInstall){
            DWORD dwSize =50;
            ch[0]= *pNewPath;
            ch[1]= ':';
            ch[2]= '\0';
            WNetGetConnection(ch,szRemoteName,&dwSize);
            CharUpperBuff(szRemoteName,lstrlen(szRemoteName));
            if (strstr(szRemoteName,"C$"))
                pNewPath[0] = 'C';
            else
                pNewPath[0] = 'D';
            }
        }
    return pNewPath;
}
/* CopySharedDLL - This function copies the shared dll like  Ctl3d32.dll
 * This function checks for the version information of file existing
 * on the system before over writing
 * Shared dll's should be installed in winnt\\system32 directory
 */
void CopySharedDLL()
{
    char szDestBuff[_MAX_PATH], szSrcBuff[_MAX_PATH],*pDest,*pSrcVersionInfo;
    int i,j;
    DWORD dwSize;
    UINT uint;
    int ival;
    char *pDestFileVer,*pSrcFileVer,*pDestVersionInfo;
    OFSTRUCT of;

    GetWindowsDirectory(szDestBuff,MAX_PATH);
    pDest = szDestBuff + lstrlen(szDestBuff);
    lstrcpy(szSrcBuff,szInstallPath);
    lstrcat(szSrcBuff,"ctl3d32.dll");
    j = sizeof (szSharedDLLs) / sizeof (char*);
    for (i = 0; i < j; i++){
        lstrcat(szDestBuff,szSharedDLLs[i]);
        if (HFILE_ERROR != OpenFile(szDestBuff,&of,OF_EXIST)){
            dwSize = GetFileVersionInfoSize(szDestBuff,0);
            pDestVersionInfo = alloca(dwSize);
            GetFileVersionInfo(szDestBuff,0,dwSize,pDestVersionInfo);
            ival = VerQueryValue(pDestVersionInfo,"\\StringFileInfo\\040904E4\\FileVersion",
                            (void**)&pDestFileVer,&uint);
            dwSize = GetFileVersionInfoSize(szSrcBuff,0);
            pSrcVersionInfo = alloca(dwSize);
            GetFileVersionInfo(szSrcBuff,0,dwSize,pSrcVersionInfo);
            ival = VerQueryValue(pSrcVersionInfo,"\\StringFileInfo\\040904E4\\FileVersion",
                            (void**)&pSrcFileVer,&uint);
            if (lstrcmp(pSrcFileVer,pDestFileVer) <=0)
                return;
            }
        CopyAFile (szSrcBuff,szDestBuff,cmoOverwrite,0);
    }

}

void DumpServicesFlags ( void ) 
{
    char szBuf[1024], szT1[8], szT2[8], szT3[8], szT4[8];

    wsprintf ( szBuf, "Global flags: wInstAuxPrint=%d, wInstRemServer=%d, wInstRDateInitTab=%d, wInstRDateAt=%d", 
        wInstAuxPrint, wInstRemServer, wInstRDateInitTab, wInstRDateAt ) ;
    OutputDebugString ( szBuf ) ;

    CbGetListItem ( "CheckItemsIn", 1, szT1, 1024);
    CbGetListItem ( "CheckItemsIn", 2, szT2, 1024);
    CbGetListItem ( "CheckItemsIn", 3, szT3, 1024); 
    CbGetListItem ( "CheckItemsIn", 4, szT4 , 1024);


    wsprintf ( szBuf, "CheckItemsIn: wInstAuxPrint=%s, wInstRemServer=%s, wInstRDateInitTab=%s, wInstRDateAt=%s", 
        szT1, szT2, szT3, szT4 ) ;
    
    OutputDebugString ( szBuf ) ;    
}    

void DumpGlobalFlags ( void )
{
    char szBuf[2048];

    wsprintf ( szBuf, "Global flags:\n"
                      "wCopyRunTimeFiles=%d, wCopyDevFiles=%d, wInstServices=%d, wInstWincom=%d,\n"
                      "wInstSerial=%d, wUpdatePath=%d, wUpdateServ=%d, wUpdatePrinter=%d, wSetFonts=%d,\n"
                      "wInstAuxPrint=%d, wInstRemServer=%d, wInstRDateInitTab=%d, wInstRDateAt=%d, wRemoveDirTree=%d", 
                        
                       wCopyRunTimeFiles, wCopyDevFiles, wInstServices, wInstWincom,
                       wInstSerial, wUpdatePath, wUpdateServ, wUpdatePrinter, wSetFonts,
                       wInstAuxPrint, wInstRemServer, wInstRDateInitTab, wInstRDateAt, 
                       wRemoveDirTree ) ;

    OutputDebugString ( szBuf ) ;
}


void UpdateInitTabFile ( void ) 
{
    char szRDateBuff[MAXLEN];
    char szIniLine[MAXLEN];
    
    // Get the path to rdate.exe
    Assert (( szONPServerName != NULL ) && (szInstallPath != NULL));
    wsprintf ( szRDateBuff, "%s\\bin\\rdate %s\n", szInstallPath, szONPServerName ) ;

    // Make changes to the [NAMES] section as necessary.
    GetPrivateProfileString ( "NAMES", "rdate", DEFAULT_STR, szIniLine, MAXLEN, INITTAB_FILE ) ;

    if ( strcmp ( DEFAULT_STR, szIniLine ) == 0 )
    {
        WritePrivateProfileString ( "NAMES", "rdate", " ", INITTAB_FILE ) ;
    }

    // Make changes to the [rdate] section as necessary
    WritePrivateProfileString ( "RDATE", "Command", szRDateBuff, INITTAB_FILE ) ;
}


void UninstallInitTabFile ( void ) 
{
    // Remove the RDATE key from the NAMES section of the INITTAB file
    WritePrivateProfileString ( "NAMES", "rdate", NULL, INITTAB_FILE ) ;

    // Remove the command name and its key from the RDATE section 
    WritePrivateProfileString ( "RDATE", "Command", NULL, INITTAB_FILE ) ;
}


void UninstallRDateAt ( void )
{ 
    CHAR szBuff[256], szFileName[256];
    char *szTempEnv, *pchTemp;
    BYTE *pbFile, *pbFileCur;
    int iRet, nBytes=256, nID=0, nLoop=0;
    HANDLE hfTemp;
    
    // Get the path to the temp directory, and if one doesn't exist, create the file on c: root

    if (( szTempEnv = getenv ( "temp" )) == NULL )
    {
        if ( (szTempEnv = getenv ( "tmp" )) == NULL )    
        {
            szTempEnv = malloc ( 10 * sizeof ( char ) );
            strcpy ( szTempEnv, "C:" ) ; 
        }
    }
    
    // Assert that we got SOME path in the variable.
    Assert (szTempEnv != NULL);

    wsprintf ( szFileName, "%s\\_RDateAt.TMP", szTempEnv ) ;
    wsprintf ( szBuff, "AT > %s", szFileName ) ;

    iRet = system ( szBuff ) ;

    if ( iRet == -1 ) 
    {
        MessageBox ( GetActiveWindow(), "Error creating the AT job table for RDate\nRun `at' and then `at # /d' by hand", 
                        "Setup Message", MB_OK|MB_ICONEXCLAMATION ) ;
        return;
    }

    hfTemp = CreateFile ( szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
    if (hfTemp == INVALID_HANDLE_VALUE)
    {
        iRet = GetLastError();
				
		wsprintf ( szBuff, "Couldn't open temp file %s for RDate AT uninstall\nCall returned %d\nRun `at' and then `at # /d' by hand", szFileName, GetLastError() ) ;
        MessageBox ( GetActiveWindow(), szBuff, "Setup Message", MB_OK|MB_ICONEXCLAMATION ) ;
        return;
    }

    nBytes = GetFileSize ( hfTemp, NULL ) ;

    pbFile = malloc ( nBytes ) ;

    iRet = (INT) ReadFile ( hfTemp, pbFile, nBytes, &nBytes, NULL );
    if ( iRet == (INT) FALSE ) 
    {
        GetLastError () ;    
    }

    // Now, look for the string associated with the RDATE section, 
    // then back up to get the id #, then delete it.
    //
    // Note that this code deletes any scheduled "rdate" command, in case
    // a user had set up many of them.
    pbFileCur = pbFile;
    while ( (pchTemp=strstr ((char *)pbFileCur, "Each M T W Th F S Su    12:01 AM      rdate")) != NULL ) 
    {
        nLoop++;

        pchTemp -= 8;

        sscanf ( pchTemp, "%d", &nID ) ;

        // Set the temp pointer ahead of the inital setting, so that the next search 
        // gets the next hit in the file.
        pbFileCur = pchTemp + 10;
        
        wsprintf ( szBuff, "AT %d /d", nID ) ;
        iRet = _spawnlp ( _P_DETACH, "AT.EXE", szBuff, NULL ) ;

        if ( iRet == -1 ) 
        {
            MessageBox ( GetActiveWindow(), "Error deleting the AT job for RDate\nRun `at' and then `at # /d' by hand", 
                            "Setup Message", MB_OK|MB_ICONEXCLAMATION ) ;
            return;
        }
        
    }

    free ( pbFile ) ;

    if (nLoop < 1 )
    {
        MessageBox ( GetActiveWindow(), "RDate <onpserver> command not found in AT command table\nRun `at' and then `at # /d' by hand", 
                        "Setup Message", MB_OK | MB_ICONEXCLAMATION ) ;
        return;
    }

} 



void AppendOverSizeFiles()
{
    OFSTRUCT of;
    HFILE hInfFile,hFile;
    DWORD dwSize;
    long offset;
    int iMax = 256;
    char *pSection,*pFileName,*pOriName,*pLine;
    char szSrcFileName[_MAX_PATH],szDestFileName[_MAX_PATH],szLine[256];
    char szLastDest[_MAX_PATH] = "\0";
    FILE* pFile;
    char* pBuff,*pTmp;
    FILETIME filetime;

    // Open .inf file
	wsprintf(szSrcFileName,"%s%s",szInstallPath,pInfFileName);
    hInfFile = OpenFile(szSrcFileName,&of,OF_READ);
    if (HFILE_ERROR == hInfFile)
        return;
    // go to place where Over size file section is present.
    dwSize = GetFileSize((HANDLE)hInfFile,NULL);
    pBuff = GlobalAlloc(GPTR,dwSize + 2);
    if (!pBuff)
        return;
    ReadFile((HANDLE)hInfFile,pBuff,dwSize,&dwSize,NULL);
    CloseHandle((HANDLE)hInfFile);
    CharUpperBuff(pBuff,dwSize);
    pTmp = strstr(pBuff,"OVERSIZE FILES");
    if (!pTmp) {
        GlobalFree(pBuff);
        return;
        }
    offset =(long) (pTmp - pBuff);
    pFile = fopen(szSrcFileName,"r");
    if (pFile == NULL)
       return;
    fseek(pFile,offset,SEEK_SET);
    // read lines
    iMax = sizeof(szLine);
    fgets(szLine,iMax,pFile);
    while(fgets(szLine,iMax,pFile)) {
        pLine = StripOfLeadingSpaces(szLine);
        if (!*pLine || '#' == *pLine)
            continue;
        if( '[' == *pLine)
            break;
        // Get pointers to section,filename, original filename.
        if(!GetMagicPointers(pLine,&pSection,&pFileName,&pOriName))
            continue;
        // Check if the file is installed ,ideally based on section name
		// but for now check if the file exists at the specified location,if not 
		// just continue with next file.
        wsprintf(szSrcFileName,"%s%s",szInstallPath,pFileName);
		hFile = OpenFile(szSrcFileName,&of,OF_EXIST);
        if (HFILE_ERROR == hFile)
            continue;

        wsprintf(szDestFileName,"%s%s",szInstallPath,pOriName);
        // Check if the destination file is not same as the last one,
        // if it's not then delete it .
        if (lstrcmpi(szLastDest,szDestFileName)){
            hFile = OpenFile(szDestFileName,&of,OF_CREATE);
            CloseHandle((HANDLE)hFile);
            }

        lstrcpy(szLastDest,szDestFileName);
        // Append the file
        FixFileName(szSrcFileName);
        FixFileName(szDestFileName);
        CopyAFile (szSrcFileName,szDestFileName,cmoAppend,1);
        GetFileTimeStamp(szSrcFileName,&filetime);
        SetFileTimeStamp(szDestFileName,&filetime);
        // delete the file
        DeleteFile(szSrcFileName);
        }
    fclose  (pFile);
}

BOOL GetMagicPointers(char* pLine,char** pSection,char** pFileName,char** pOriName)
{
    char *pTmp;
    if(!pLine)
        return FALSE;
    *pSection = pLine;
    *pFileName = GetNextWord(pLine);
    if (!*pFileName)
        return FALSE;
    *pOriName = GetNextWord(*pFileName);
    if (!*pOriName)
        return FALSE;
    pTmp = strchr(*pOriName,'\n');
    if (!pTmp)
        pTmp = strchr(*pOriName,'\r');
    if(pTmp)
        *pTmp = '\0';
    return TRUE;
}

char* GetNextWord(char* pLine)
{
    char* pTmp;
    if (!(int)(pTmp = strchr(pLine,',')))
        return pTmp;
    *pTmp++ = '\0';
    pTmp = StripOfLeadingSpaces(pTmp);
    return pTmp;
}

void FixFileName(char* pString)
{
    while (*pString) {
        if ('/' == *pString)
            *pString = '\\';
        pString++;
        }

}

void GetFileTimeStamp(char* pFileName,FILETIME* pFileTime)
{
    HANDLE hFile;
    OFSTRUCT of;
    hFile = (HANDLE)OpenFile(pFileName,&of,OF_READ);
    if (HFILE_ERROR == (HFILE) hFile)
        return;
    GetFileTime(hFile,NULL,NULL,pFileTime);
    CloseHandle(hFile);
}

void SetFileTimeStamp(char* pFileName,FILETIME* pFileTime)
{
    
    HANDLE hFile;
    OFSTRUCT of;
    hFile = (HANDLE)OpenFile(pFileName,&of,OF_READWRITE);
    if (HFILE_ERROR == (HFILE) hFile)
        return;
    SetFileTime(hFile,NULL,pFileTime,pFileTime);
    CloseHandle(hFile);
    
}


void Usage ( void ) 
{
	MessageBox ( NULL,"Mosaic OA Setup\n\n"
					  "Usage: SETUP [-u] [-U] [-?]\n"
	                  "   -u : Uninstall Mosaic OA files\n"
					  "   -U,-? : Display this usage message\n", "Setup Usage", MB_OK | MB_ICONEXCLAMATION) ;
}


//
// bCopyAnyFiles(void) 
// 
// Return TRUE if any of the global w* flags have been set, (i.e. if the user checked ANY
// of the checkboxes for files to copy.
//
BOOL bCopyAnyFiles ( void ) 
{
	// Why are the wUpdatePath and wSetFonts commented?
	// We copy the shared/common file section for any situation in which files need
	// to be copied off floppy diskettes.  We don't need to copy the shared stuff for
	// path setting and fonts install.
    return (wCopyRunTimeFiles || wCopyDevFiles || wInstServices || wInstWincom || 
        wInstSerial || /*wUpdatePath ||*/ wUpdateServ || wUpdatePrinter || /*wSetFonts ||*/
        wInstAuxPrint ||  wInstRDateAt ||  wInstRDateInitTab ||  wInstRemServer );
}


//
// bCompleteUninstall ( void ) 
// 
// Return TRUE if all of the file sections are being deleted (implemented as: return
// TRUE if all the file section related items in the moasetup.ini file are FALSE.
//
BOOL bCompleteUninstall ( void ) 
{
	// Why are the wUpdatePath and wSetFonts commented?
	// We copy the shared/common file section for any situation in which files need
	// to be copied off floppy diskettes.  We don't need to copy the shared stuff for
	// path setting and fonts install.
    return (bIsInstalled("Runtime Files") &&
    		bIsInstalled("Development Environment") && 
			bIsInstalled("Serial Device Sharing") && 
			bIsInstalled("Auxprint and Wincom") && 
			bIsInstalled("Fonts") && 
			bIsInstalled("Remote COM Server") );
}


///////////////
//
//	FUNCTION:
//		BOOL bRemoveTree ( char * szDir ) 
//
//	PURPOSE:
//		Worker function to recurse through a directory tree
//		and delete the non-empty nodes of a tree.
//
//	COMMENTS:
//		Notes, etc.
//
//	INPUT:
//		szDir is the string describing the current working
//		directory.  It's mainly used just for debugging output;
//		the function could easily take no parameters.
//
//	OUTPUT:
//		Returns TRUE if the current directory is empty and can be 
//		deleted.  Returns FALSE if files were found in the directory
//		or some other error occurred.
//
//	REVISION HISTORY:
//		8/24/94		Patl	Created from scratch
//		8/24/94		Patl	Modified from console output to Windows, 
//							plugged into moa setup.
//		9/14/94		Patl	Removed section which checked for existing files in the current tree.
//							We now just call RemoveDir, and if it fails, we don't care, and leave
//							that part of the tree there.
//
//  (C) Copyright 1989-1994 by Olivetti North America
///////////////

BOOL bRemoveTree ( char * szDir )
{
	HANDLE hSearch;
	int iRet;
	WIN32_FIND_DATA ffd;

	hSearch = FindFirstDirectory ( "*", &ffd ) ;
	if ( hSearch == (HANDLE)-1 )
	{
		// No subdirectory found, go back up
		SetCurrentDirectory ( ".." );
		return TRUE;
	}

	for ( ;; ) 
	{
		SetCurrentDirectory ( ffd.cFileName ) ;

		iRet = bRemoveTree ( ffd.cFileName ) ;
		if ( iRet == TRUE ) 
		{	
			RemoveDirectory ( ffd.cFileName ) ;
		}

		if ( FindNextDirectory ( hSearch, &ffd ) == FALSE )
		{
			SetCurrentDirectory ( ".." ) ;
			break;
		}
	}  
	
	FindClose (hSearch);
	return TRUE;
}


/****************************************************************************
*
*    FUNCTION: FindFirstDirectory(LPTSTR, LPWIN32_FIND_DATA)
*
*    This function comes from the WALK sample project in MSVC 2.0.
* 
*    PURPOSE: finds the first directory in the current working directory
*
*    COMMENTS:
*
*       This function is called by Walk() each time a new subdirectory is
*       entered.  Since only directory entries are of interest, this call
*       provides a simple means to bypass non-directory filenames.
*
*    INPUT: lpszSearchFile -> "*"
*           lpffd - pointer to the file find data structure of type
*                   WIN32_FIND_DATA
*
*    OUTPUT: Returns a file handle if a directory is found
*            Returns a -1 if no directory is found        
*
****************************************************************************/

HANDLE FindFirstDirectory(LPTSTR lpszSearchFile, LPWIN32_FIND_DATA lpffd)
{
  BOOL     bRC;
  DWORD    dwRC;
  HANDLE   hSearch;
  int      iRC;

  hSearch=FindFirstFile(lpszSearchFile,lpffd);
  if (hSearch == (HANDLE) -1)
     return (hSearch);
  for(;;)
     {
     dwRC=GetFileAttributes(lpffd->cFileName);
     if (dwRC & FILE_ATTRIBUTE_DIRECTORY)
        {
        iRC=strcmp(lpffd->cFileName,".");
        if (iRC)
           {
           iRC=strcmp(lpffd->cFileName,"..");
           if (iRC)
              return (hSearch);
           }
        }
     bRC=FindNextFile(hSearch,lpffd);
     if (bRC == FALSE)
        {
        FindClose(hSearch);
        return ((HANDLE) -1);
        }
     }
}



///////////////
//
//	FUNCTION:
//		bFindRealFile ( void )
//
//	PURPOSE:
//		This function searches for any real filenames ("real" meaning not 
//		a directory) 
//  	
//
//	COMMENTS:
//		Notes, etc.
//
//	INPUT:
//		None
//
//	OUTPUT:
//		returns TRUE if any file was found, or FALSE if not.
//
//	REVISION HISTORY:
//		08/22/1994		Patl	Created from scratch, based on FindFirstFile
//								from the MSVC Win32 WALK sample.
//
//  (C) Copyright 1989-1994 by Olivetti North America
///////////////

BOOL bFindRealFile ( void ) 
{
	BOOL     bRC;
  	DWORD    dwRC;
	HANDLE   hSearch;
	WIN32_FIND_DATA ffd;

	hSearch=FindFirstFile("*",&ffd);
  
	if (hSearch == (HANDLE) -1)
	{
     	return FALSE;
	}

  	while ( TRUE ) 
  	{
     	dwRC=GetFileAttributes(ffd.cFileName);
     	if (!(dwRC & FILE_ATTRIBUTE_DIRECTORY))
        {
			// It's a file, so return true.
           	return TRUE;
       	}
     	
     	bRC=FindNextFile(hSearch,&ffd);
     	
     	if (bRC == FALSE)
        {
        	FindClose(hSearch);
        	return (FALSE);
        }
 	} // end while (TRUE)
}


/****************************************************************************
*
*    FUNCTION: FindNextDirectory(LPTSTR, LPWIN32_FIND_DATA)
*
*    PURPOSE: finds the next directory in the current working directory
*
*    COMMENTS:
*
*       This function is called by Walk() each time a new subdirectory is
*       entered.  Since only directory entries are of interest, this call
*       provides a simple means to bypass non-directory filenames.
*
*    INPUT: lpszSearchFile -> "*"
*           lpffd - pointer to the file find data structure of type
*                   WIN32_FIND_DATA
*
*    OUTPUT: Returns a file handle if a directory is found
*            Returns a #-1 NOT!# FALSE if no directory is found        
*						^^ Incorrect documentation from sample code. (PL)
*
****************************************************************************/

BOOL FindNextDirectory(HANDLE hSearch, LPWIN32_FIND_DATA lpffd)
{
  	BOOL     bRC;
  	DWORD    dwRC;

  for(;;)
     {
     bRC=FindNextFile(hSearch,lpffd);
     if (bRC == FALSE)
        return (FALSE);
     dwRC=GetFileAttributes(lpffd->cFileName);
     if (dwRC & FILE_ATTRIBUTE_DIRECTORY)
        return (TRUE);
     }
}

/****************************************************************************
*
*    FUNCTION: IsInterruptTwoOK
*
*    PURPOSE: Checks if the network card is configured to use interrupt 2
*				
*
*    COMMENTS:
*			Wincom is configured to use interrupt 2.Ideally this function should 
*			check if the interrupt 2 is free to use.Usually network card is configured
*			to use interrupt 2, so this function right now checks only for net card
*			configuration.
*       
*
*
*    OUTPUT: Returns FALSE  if the interrupt is in use by net card else TRUE
*
****************************************************************************/
BOOL IsInterruptTwoOK(void)
{
	HKEY hKey = 0;
    long lRetValue;
	char szServiceBuff[256] = "\0";
	long lSize = 256;
	DWORD dwType; 
	char szBuff[2 * _MAX_PATH];
	DWORD dwInterrupt;

	if (!wInstWincom)
		return TRUE;

    if (bUnInstall)
        return TRUE;

	// Get the service name which is responsible for
	// holding IP address.
	lRetValue = RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\1",&hKey);	
	if (ERROR_SUCCESS != lRetValue)
		// In Windows NT 3.1 we can find the info in following key.
		lRetValue = RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\01",&hKey);	
	if (ERROR_SUCCESS != lRetValue)
		// There is no network card present in the machine.
		return TRUE;

	lRetValue = RegQueryValueEx(hKey,"ServiceName",NULL,&dwType,szServiceBuff,&lSize);
	if (ERROR_SUCCESS != lRetValue)
		ErrorHandler("Setup failed to find out the interrupt of network card.\r\n Verify that it is not set to interrupt 2.");		
	RegCloseKey(hKey);
	wsprintf(szBuff,"System\\CurrentControlSet\\Services\\%s\\Parameters",szServiceBuff);
	lRetValue = RegOpenKey(HKEY_LOCAL_MACHINE,szBuff,&hKey);	
	if (ERROR_SUCCESS != lRetValue)
		ErrorHandler("Setup failed to find out the interrupt of network card.\r\n Verify that it is not set to interrupt 2.");
	lSize = sizeof(DWORD);
	lRetValue = RegQueryValueEx(hKey,"InterruptNumber",NULL,&dwType,(char*)&dwInterrupt,&lSize);
	if (ERROR_SUCCESS != lRetValue)
		lRetValue = RegQueryValueEx(hKey,"INTERRUPT",NULL,&dwType,(char*)&dwInterrupt,&lSize);
	if (ERROR_SUCCESS != lRetValue)
		ErrorHandler("Setup failed to find out the interrupt of network card.\r\n Verify that it is not set to interrupt 2.");
	RegCloseKey(hKey);
	// if the interrupt is set to 2 then it is error.
	if (dwInterrupt == 2)
    	return FALSE;
	else
		return TRUE;
}

void ErrorHandler(char* szBuff)
{
	MessageBox(GetActiveWindow(),szBuff,"Setup Message",MB_OK|MB_ICONINFORMATION);
}


///////////////
//
//	FUNCTION:
//		bSplitFileName ( char * )
//
//	PURPOSE:
//		Determines whether the current file name reflects that
// 		the file is a large file, split into pieces to fit onto
// 		floppy diskettes.
//
//	COMMENTS:
//		The current naming convention for split file names is
// 		to split FOO.DLL  into FOO.D0L, FOO.D1L, FOO.D2L, ...
//
//		If you have a large file which is NOT a DLL, then this function
// 		should change to reflect the candidate names.  You may want to 
// 		implement a finite-state machine for names such as EXE, DLL, TXT, 
// 		etc.
//
//	INPUT:
//		The file name which is a candidate for a split file name
//
//	OUTPUT:
//		Returns TRUE if the filename matches the candidate 
//		descriptions, or FALSE if it does not.
//
//	REVISION HISTORY:
//		09/20/1994		Patl	Created from scratch
//
//  (C) Copyright 1989-1994 by Olivetti North America
///////////////

BOOL bSplitFileName ( CHAR szFileName[] )
{
	INT nNameLength;

	nNameLength = strlen ( szFileName ) ;

	// The shortest filename we could see here would be a.d3l, for example.
	if (nNameLength < 5)
		return FALSE;

	// The file name is a split name if the extension is of the form NAME.[d|D][0..9][l|L].
	if ( (szFileName[nNameLength-4] == '.') && 
		 ((szFileName [nNameLength-3] == 'd') ||  (szFileName [nNameLength-3] == 'D')) && 
		 (isdigit ( szFileName[nNameLength-2] )) &&
	     (( szFileName [nNameLength-1] == 'l') || (szFileName [nNameLength-1] == 'L')) )
		return TRUE;
	else
		return FALSE;
}


///////////////
//
//	FUNCTION:
//		szReturnUnsplitName ( char * );
//
//	PURPOSE:
//		Returns the filename le name reflects that
// 		the file is a large file, split into pieces to fit onto
// 		floppy diskettes.
//
//	COMMENTS:
//		Assumes the input filename has been determined to be
// 		a split filename, so all the function must do is update
//		the file extension.
//
//	INPUT:
//		The file name which is a split file name
//
//	OUTPUT:
//		Returns a string specifying the filename of the 
//		final (unsplit, glued) file.
//
//		Returns NULL if an error occurred.
//
//	REVISION HISTORY:
//		09/20/1994		Patl	Created from scratch
//
//  (C) Copyright 1989-1994 by Olivetti North America
///////////////

CHAR *szReturnUnsplitName ( CHAR szFileName[] )
{
	INT nNameLength;
	CHAR *szNewName;
	
	nNameLength = strlen ( szFileName ) ;

	// The shortest filename we could see here would be a.d3l, for example.
	if (nNameLength < 5)
		return NULL;

	szNewName = (CHAR *) malloc ( sizeof (CHAR) * (nNameLength + 1) );

	if (szNewName != NULL)
	{
		lstrcpy ( szNewName, szFileName ) ;
		szNewName[nNameLength-2] = 'L';
		return szNewName;
	}		 
}




///////////////
//
//	FUNCTION:
//		bZerothSplitFile ( char * )
//
//	PURPOSE:
//		Determines whether the current file name is the first (indexed off 0)
// 		split file name.  E.g. that it's foo.d0l, not foo.d3l
//
//	COMMENTS:
//		The current naming convention for split file names is
// 		to split FOO.DLL  into FOO.D0L, FOO.D1L, FOO.D2L, ...
//
//	INPUT:
//		The file name which is a split file name
//
//	OUTPUT:
//		Returns TRUE if the filename is .d0l, FALSE if it does not.
//
//	REVISION HISTORY:
//		09/20/1994		Patl	Created from scratch
//
//  (C) Copyright 1989-1994 by Olivetti North America
///////////////

BOOL bZerothSplitFile ( CHAR szFileName[] )
{
	INT nNameLength;

	nNameLength = strlen ( szFileName ) ;

	// The shortest filename we could see here would be a.d3l, for example.
	if (nNameLength < 5)
		return FALSE;

	// The file name is a split name if the extension is of the form NAME.[d|D][0..9][l|L].
	if ( szFileName[nNameLength-2] == '0')
	    return TRUE;
	else
		return FALSE;
}



VOID SaveSetupOptions(VOID)
{
	CHAR szPreviousPath[MAX_PATH]="", szBuff[MAX_PATH]="";

	// Consider the case in which the user has set up a few items, then 
	// runs setup at a later time to pick up previously unused options.  
	// We need to use the following heuristics to cover this case:
	// - if the previous install directory does not exist (first time installation)
	//	 then write all the options.
	// - if the previous install directory does exist but is different from 
	//   the current directory, then write all the options and warn the user
	//	 that there is another installation out there in <path>.
	// - if the previous installation directory exists and is the same as 
	//   the one we're now installing to, then simply write the "YES"s, and 
	//   overwrite the existing settings.

	GetSetupDirectory ( "Runtime and Development Tools", szPreviousPath, MAX_PATH ) ;

 	if ((strlen (szPreviousPath) == 0) || (bUnInstall==TRUE))
	{
		bWriteSetupDirectory ( "Runtime and Development Tools", szInstallPath ) ;
		bWriteSetupDirectory ( "mOA Projects", szMoaProjDir ) ;
	
		bWriteSetupOption ( "Runtime and Development Tools", "Runtime Files", wCopyRunTimeFiles?"YES":"NO" ) ;
		bWriteSetupOption ( "Runtime and Development Tools", "Development Environment", wCopyDevFiles?"YES":"NO" ) ;
		bWriteSetupOption ( "Runtime and Development Tools", "Serial Device Sharing", wInstSerial?"YES":"NO" ) ;
		bWriteSetupOption ( "Runtime and Development Tools", "Fonts", wSetFonts?"YES":"NO" ) ;
		bWriteSetupOption ( "Runtime and Development Tools", "Auxprint and Wincom", wInstWincom?"YES":"NO" ) ;
		bWriteSetupOption ( "Runtime and Development Tools", "Remote COM Server", wInstRemServer?"YES":"NO" ) ;
	}
	else	// the previous directory exists
	{		
		if (strcmp ( szInstallPath, szPreviousPath) == 0)
		{
			// We're overwriting the existing directory

			if (wCopyRunTimeFiles)
				bWriteSetupOption ( "Runtime and Development Tools", "Runtime Files", "YES" ) ;

			if (wCopyDevFiles)
				bWriteSetupOption ( "Runtime and Development Tools", "Development Environment", "YES" ) ;

			if (wInstSerial)
				bWriteSetupOption ( "Runtime and Development Tools", "Serial Device Sharing", "YES" ) ;

			if (wSetFonts)
				bWriteSetupOption ( "Runtime and Development Tools", "Fonts", "YES" ) ;

			if (wInstWincom)
				bWriteSetupOption ( "Runtime and Development Tools", "Auxprint and Wincom", "YES" ) ;

			if (wInstRemServer)
				bWriteSetupOption ( "Runtime and Development Tools", "Remote COM Server", "YES" ) ;
		}
		else	// we're installing into a new directory
		{
			wsprintf ( szBuff, "Warning!  There is a previous installation of Mosaic OA "
							   "in the %s directory.  You may want to run SETUP -U to "
							   "Uninstall the previous installation.\n\n Continue this setup?", szPreviousPath ) ;
			
			if ( IDNO == MessageBox ( NULL, szBuff, "Setup Message", MB_ICONEXCLAMATION | MB_YESNO ) )				  
			{
				AskQuit();
			}

			bWriteSetupDirectory ( "Runtime and Development Tools", szInstallPath ) ;
			bWriteSetupDirectory ( "mOA Projects", szMoaProjDir ) ;

			bWriteSetupOption ( "Runtime and Development Tools", "Runtime Files", wCopyRunTimeFiles?"YES":"NO" ) ;
			bWriteSetupOption ( "Runtime and Development Tools", "Development Environment", wCopyDevFiles?"YES":"NO" ) ;
			bWriteSetupOption ( "Runtime and Development Tools", "Serial Device Sharing", wInstSerial?"YES":"NO" ) ;
			bWriteSetupOption ( "Runtime and Development Tools", "Fonts", wSetFonts?"YES":"NO" ) ;
			bWriteSetupOption ( "Runtime and Development Tools", "Auxprint and Wincom", wInstWincom?"YES":"NO" ) ;
			bWriteSetupOption ( "Runtime and Development Tools", "Remote COM Server", wInstRemServer?"YES":"NO" ) ;
		}
	}
}


///////////////
//
//	FUNCTION:
//		bIsInstalled( LPCSTR szSection )
//
//	PURPOSE:
//		Determines whether or not a particular sectoin of files was previously
//		installed.  This is accomplished by a pair of checks:
//			- Check that the setup option associated with this section was 
//			  selected and stored to the .ini file/registry.
//			- Check that a sample file from that section exists where it should.
//
//	COMMENTS:
//		Care must be taken in this function to ensure that the "sample file"
//		is one which will always exist.
//
//	INPUT:
//		szSection - a string naming the setup section, e.g. "Development Files"
//
//	OUTPUT:
//		Returns TRUE if both tests pass and we think this section was installed
//		Returns FALSE otherwise.
//
///////////////
BOOL bIsInstalled ( LPCSTR szSection )
{
 	BOOL bRet = FALSE;
	CHAR szSetting[16];
	CHAR szFile[MAX_PATH];

	// This section should be changed to an case/switch integer comparison on key IDs 
	// for speed's sake
	
	if ( strcmp ( szSection, "Runtime Files" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Runtime Files", szSetting, 16 ) ;
		wsprintf ( szFile, "%sbin\\rt.exe", szInstallPath ) ;
	}
#if 0
	else if ( strcmp ( szSection, "Runtime SLIM Files" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Runtime Files", szSetting, 16 ) ;
		wsprintf ( szFile, "%sbin_slim\\rt.exe", szInstallPath ) ;
	}
	else if ( strcmp ( szSection, "Runtime FAT Files" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Runtime Files", szSetting, 16 ) ;
		wsprintf ( szFile, "%sbin_fat\\rt.exe", szInstallPath ) ;
	}
#endif
	else if ( strcmp ( szSection, "Development Environment" ) == 0 )
	{ 
		GetSetupOption ( "Runtime and Development Tools", "Development Environment", szSetting, 16 ) ;
		wsprintf ( szFile, "%sbin\\ibae.exe", szInstallPath ) ;
	}
	else if ( strcmp ( szSection, "Serial Device Sharing" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Serial Device Sharing", szSetting, 16 ) ;
		wsprintf ( szFile, "%ssrc\\driver\\files\\serial.sys", szInstallPath ) ;
	}
	else if ( strcmp ( szSection, "Fonts" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Fonts", szSetting, 16 ) ;
		wsprintf ( szFile, "%sfonts\\isc_br1.fon", szInstallPath ) ;
	}
	else if ( strcmp ( szSection, "Auxprint and Wincom" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Auxprint and Wincom", szSetting, 16 ) ;
		wsprintf ( szFile, "%ssrc\\driver\\files\\wincom.sys", szInstallPath ) ;
	}
	else if ( strcmp ( szSection, "Remote COM Server" ) == 0 )
	{
		GetSetupOption ( "Runtime and Development Tools", "Remote COM Server", szSetting, 16 ) ;
		wsprintf ( szFile, "%sbin\\remsrvr.exe", szInstallPath ) ;
	}
	else
	{ 
		return FALSE;
	}

	return ( (strcmp(szSetting,"YES")==0) && (bFileExists ( szFile )) );	
 }


BOOL CreateBatchFiles()
{
	HANDLE hf;
	CHAR szBuffer[2048];
	SECURITY_ATTRIBUTES sa;
	DWORD dwBW, len;
	BOOL bSuccess=TRUE;

 	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle=FALSE;

	wsprintf ( szBuffer, "%sbin\\USESLIM.CMD", szInstallPath ) ;
	hf = CreateFile( szBuffer, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)0 ) ;
	if ( hf != INVALID_HANDLE_VALUE )
	{
		wsprintf ( szBuffer, "REM use the slim (release) version of the mOA binaries\n\n"
							 "Pause Please exit from any mOA application which you are currently running and press a key to continue\n"
							 "copy %sbin_slim\\*.* %sbin\n", szInstallPath, szInstallPath ) ;
		len = lstrlen ( szBuffer ) ;
		WriteFile ( hf, szBuffer, len, &dwBW, NULL );
		if (dwBW != len)
			bSuccess=FALSE;

		CloseHandle ( hf );
	}
	else
	{
		DWORD err = GetLastError();
		bSuccess=FALSE;
	}

	wsprintf ( szBuffer, "%sbin\\USEFAT.CMD", szInstallPath ) ;
	hf = CreateFile( szBuffer, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)0 ) ;
	if ( hf != INVALID_HANDLE_VALUE )
	{
		wsprintf ( szBuffer, "REM use the fat (debug) version of the mOA binaries\n\n"
  		       		 		 "Pause Please exit from any mOA application which you are currently running and press a key to continue\n"
							  "copy %sbin_fat\\*.* %sbin\n", szInstallPath, szInstallPath ) ;	
		len = lstrlen ( szBuffer ) ;
		WriteFile ( hf, szBuffer, len, &dwBW, NULL );
		if (dwBW != len)
			bSuccess=FALSE;

		CloseHandle ( hf );
	}
	else
	{	
		DWORD err = GetLastError();
		bSuccess=FALSE;
	}

 	return bSuccess;
}

// This function copies the printer drivers to the directory where
// Windows NT will look for drivers.It will update the mosaic OA
// supplied printer drivers, even if there is no printer created
// which uses  these drives,This shouldn't be a probelm.

BOOL CopyPrinterDLLs()
{
	char *pSrcDir = "src\\driver\\files\\";
	char *pDestDir = "\\system32\\spool\\drivers\\w32x86\\1\\";
	char szSrcBuff[_MAX_PATH];
	char szDestBuff[_MAX_PATH];
	OFSTRUCT ofstruct;
	int i,j;

	i = sizeof(szPrinterDLLs) / sizeof (char*);
	for (j = 0; j < i; j++) {
		wsprintf (szSrcBuff,"%s%s%s",szInstallPath,pSrcDir ,szPrinterDLLs[j]);
		if (HFILE_ERROR != OpenFile(szSrcBuff,&ofstruct,OF_EXIST)) {
			GetWindowsDirectory (szDestBuff,MAX_PATH);
			lstrcat (szDestBuff,pDestDir);
			lstrcat (szDestBuff,szPrinterDLLs[j]);
			CopyAFile(szSrcBuff,szDestBuff,cmoOverwrite,0);
			}
		}
	return TRUE;

}

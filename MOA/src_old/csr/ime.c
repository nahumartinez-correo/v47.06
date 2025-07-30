/*$
** MODULENAME - ime.c
**
** $Name: R1V4C13 $
** $Revision: 1.8 $
** $Date: 1999/05/14 15:52:09 $
**
** Synopsis:
**   Input Method Editor support for Asian languages
**
**   Updated to work with NT 4.0/95.  Keystroks in RT's
**   TEfields are are now posted to the OS, and the OS
**   composes the correct WM_CHAR and WM_DEADCHAR.
**
**  (c) Copyright by Olsy-na, 1996-1998
**        All rights reserved.
*/

#include	"pch.h"
#include	<typedefs.h>
#include	<rtdef.h>
#include	<dataman.h>
#include	<csrdef.h>
#include	<ofbdefs.h>
#include	<rtprotos.h>
#include	<convers.h>
#include	<defines.h>
#include	<nls.h>
#include	<ofbwords.h>
#include	<textedit.h>


// To compile and link IME support:
// 
// 1)  READ the RT System Reference Manual section: LOCALIZING MOSAIC OA
// 2)  Replace moa\src\include\ime.h with dke\h\ime.h.
// 3)  Be sure dke\h\winuser.h is used.
// 4)  Link with dke\lib\i386\user32.lib, not msvc\lib\user32.lib.
// 5)  Link with imm32.lib

//#define IME_SUPPORT

#ifdef IME_SUPPORT
// These includes are commented out so errors are not generated
// when updating dependancies.  They must be included when
// compiling IME support.
//#include	<ime.h>
//#include	<imm.h>
//#include	<dke\imep.h>
//#include	<dke\winnls.h>
//#include	<dke\winnls32.h>
#define REMAP_NUMERIC_PAD // remaps the Numeric Pad '.' key to '000'
#define DBSC_MODE		1
#define ENGLISH_MODE	2

static HWND 	hWndCurrentInputField;

//
// INTERNAL FUNCTIONS
//
static LRESULT IMEfunctions(WPARAM wParam, LPIMESTRUCT lpImeStruc);
static BOOL SetConversionStatus( HWND hWnd, int nMode );
static int MakeCompositionChar( WPARAM wParam );
static BOOL CheckDbcsAlphNumChar( DWORD TestChar );

//
// EXTERNAL FUNCTIONS
//
extern void key_event(int);

#endif

//
// EXPORTED FUNCTIONS
//
LRESULT CALLBACK IMEWndProc(HWND hwnd,UINT message,WPARAM uParam,LPARAM lParam);


// ======================================================
// FUNCTION: IMEWndProc(HWND, UINT, WPARAM, LPARAM)
// PURPOSE:  IME Processes messages
//
// return code : 1 - RT will process the message
//               0 - message has be processed
//
// If some IME windows messages are not processed
// by the main window, the IME status window will not appear.
// So these messages should be checked and sent to the main window.
// ======================================================
LRESULT CALLBACK
IMEWndProc(HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
#ifdef IME_SUPPORT
	static BOOL FirstTimeFocus = TRUE;
    int 		keycode = 0;
    int 		i;
	TERec		*tr; // the textedit field	
#endif

	switch (message) {

// The WM_QUERYENDSESSION and WM_ENDSESSION messages are now
// handled in RT.dll.
//
// When a WM_QUERYENDSESSION message is received, the OFB routine
// Common:QueryEndSession is executed if it exists.  To cancel the
// shutdown then set Retval to FALSE (0) and execute a RETURN RETVAL.

#ifdef IME_SUPPORT
	  case WM_COMMAND:
		if( (HIWORD(uParam) ) == EN_SETFOCUS )
		{
			tr = (TERec *)(((struct RT_TEFIELD *)Currentdlp->obj)->terec);
			hWndCurrentInputField = tr -> hTEWnd;

			FirstTimeFocus = FALSE; // The application is initialized

			if( Currentfield->flags.autoIME )
				SetConversionStatus( hWndCurrentInputField, DBSC_MODE );
			else 
				SetConversionStatus( hWndCurrentInputField, ENGLISH_MODE );
		} else if( ( HIWORD(uParam) ) == EN_KILLFOCUS )
		{
			SetConversionStatus( hWndCurrentInputField, ENGLISH_MODE );
		}
		return( 1 );

	  case WM_KEYDOWN:
		switch( uParam ) {
		  case 0x0000001B:
			//
			// ESC key, reset IME status
			//
			SetConversionStatus( hwnd, ENGLISH_MODE );
			break;

#ifdef REMAP_NUMERIC_PAD
		  case 0x0000006E:
			//
			// Num Lock On ( '.' key )
			//
			if (( HIWORD(lParam) == 0x0053) || (HIWORD(lParam) == 0x4053)) {
				for( i = 0; i < 3; i++ )
					key_event( (int)0x30 ); /* print 000 3 times */
				return( 0 );
			}
			break;

		  case VK_DELETE:
			//
			// Num Lock Off ( 'del' key )
			//
			if (( HIWORD(lParam) == 0x0053) || (HIWORD(lParam) == 0x4053)) {
				//
				// Assign '.' key( DBCS '.' ) on DBSC input field.
				//
				if( Currentfield->flags.autoIME ){
					key_event( (int)0xaea3 );
				} else { 
					for( i = 0; i < 3; i++ )
						key_event( (int)0x30 ); /* print 000 3 times */
				}
			} else
				key_event( (int)0x197 );  // RT_DELETE
			return( 0 );

		  case VK_END:
			//
			// assign 'end' key to clear field
			//
			if((HIWORD(lParam) == 0x014F) || (HIWORD(lParam) == 0x414F)) {
				SendMessage( hWndCurrentInputField, EM_SETSEL, 
							(WPARAM)0, (LPARAM)-1 );
				key_event( (int)0x193 ); /* RT_BACKSPACE */
				return( 0 );
			}
			if((HIWORD(lParam) == 0x004f) || (HIWORD(lParam) == 0x404f)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb1a3 );
					return( 0 );
				}
			}
			break;

		  case VK_INSERT:
			//
			// remap NumLock key for DBSC Input field
			//
			if((HIWORD(lParam) == 0x0052) || (HIWORD(lParam) == 0x4052)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb0a3 );
					return( 0 );
				}
			}
			break;

		  case VK_DOWN:
			//
			// NUM_2
			//
			if((HIWORD(lParam) == 0x0050) || (HIWORD(lParam) == 0x4050)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb2a3 );
					return( 0 );
				}
			}
			break;

		  case VK_NEXT:
			//
			// NUM_3
			//
			if((HIWORD(lParam) == 0x0051) || (HIWORD(lParam) == 0x4051)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb3a3 );
					return( 0 );
				}
			}
			break;

		  case VK_LEFT:
			//
			// NUM_4
			//
			if((HIWORD(lParam) == 0x004b) || (HIWORD(lParam) == 0x404b)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb4a3 );
					return( 0 );
				}
			}
			break;

		  case VK_CLEAR:
			//
			// NUM_5
			//
			if((HIWORD(lParam) == 0x004c) || (HIWORD(lParam) == 0x404c)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb5a3 );
					return( 0 );
				}
			}
			break;

		  case VK_RIGHT:
			//
			// NUM_6
			//
			if((HIWORD(lParam) == 0x004d) || (HIWORD(lParam) == 0x404d)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb6a3 );
					return( 0 );
				}
			}
			break;

		  case VK_HOME:
			//
			// NUM_7
			//
			if((HIWORD(lParam) == 0x0047) || (HIWORD(lParam) == 0x4047)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb7a3 );
					return( 0 );
				}
			}
			break;

		  case VK_UP:
			//
			// NUM_8
			//
			if((HIWORD(lParam) == 0x0048) || (HIWORD(lParam) == 0x4048)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb8a3 );
					return( 0 );
				}
			}
			break;

		  case VK_PRIOR:
			//
			// NUM_9
			//
			if((HIWORD(lParam) == 0x0049) || (HIWORD(lParam) == 0x4049)) {
				if( Currentfield->flags.autoIME ) {
					key_event( (int)0xb9a3 );
					return( 0 );
				}
			}
			break;

		  default:
			break;
		} // end of case WM_KEYDOWN:
		break;
#endif
	  case WM_IME_KEYDOWN:
		//
		// Lock the IME status window so it will not change
		// on non-DBSC input field
		//
		{
			HIMC hIMC;
			DWORD fdwConversion, fdwSentence;

			if( !Currentfield->flags.autoIME ) {

				hIMC = ImmGetContext( hwnd );
				ImmGetConversionStatus( hIMC, &fdwConversion, &fdwSentence );
				if( fdwConversion == IME_CMODE_NATIVE ) {
					SetConversionStatus( hwnd, ENGLISH_MODE );
				}
				ImmReleaseContext( hwnd, hIMC );
			}
		}
		return( 0 );

	  case WM_IME_SETCONTEXT:
		// Messages: WM_IME_SETCONTEXT, WM_IME_NOTIFY
		// The TRrec handle value, Currentdlp, is not available until
		// RT(RT.DLL) establishes the current input field information.
		//
		if(!FirstTimeFocus ) {
			SendMessage(hWndCurrentInputField, WM_IME_SETCONTEXT,
						uParam, lParam );
			return(0);
		}
		break;

	  case WM_IME_NOTIFY:
		if(!FirstTimeFocus ) {
			SendMessage(hWndCurrentInputField, WM_IME_NOTIFY,
						uParam, lParam );
			return(0);
		}
		break;

	  case WM_IME_COMPOSITION:
		if( lParam & GCS_RESULTSTR ) {
			//
			// Prevent '+' gabage input when 'Back Tab' key is pressed.
			// ( KCI assigned 'Back Tab' key as a 'NUM_+' key )
			//
			if( uParam == 0x0000A3AB ) {
				return ( 0 );
			}
			/*
			 * DBCS alpha-numeric character.
			 */
			if( CheckDbcsAlphNumChar( uParam ) == TRUE ) {
				keycode = MakeCompositionChar( uParam );				  
				key_event( keycode );
			} else {
				SendMessage( hWndCurrentInputField, WM_IME_COMPOSITION,
							uParam, lParam );
				Currentfield->numchars = (short)SendMessage(hWndCurrentInputField,
															WM_GETTEXTLENGTH,0,0);
				SendMessage(hWndCurrentInputField, WM_GETTEXT, 
							Currentfield->numchars+1, 
							(long)Currentfield->storage );
				Currentfield->storage[Currentfield->numchars] = '\0';
			}
		} else {
			SendMessage(hWndCurrentInputField, WM_IME_COMPOSITION,
						uParam, lParam );
		}
		return(0);
		
	  default:
		break;
#endif
	}
	return (1);
}

#ifdef IME_SUPPORT
// These routines were needed for NT 3.51.  They should not
// be needed for NT4.0.
// ======================================================
//  SetConversionStatus()
// ======================================================
BOOL
SetConversionStatus( HWND hWnd, int nMode )
{
	HIMC hIMC;
	DWORD fdwConversion, fdwSentence;

	if( hIMC = ImmGetContext( hWnd ) )
	{
		ImmGetConversionStatus( hIMC, &fdwConversion, &fdwSentence );

		switch( nMode )	{
		  case DBSC_MODE:
			fdwConversion |= IME_CMODE_NATIVE;
			fdwConversion |= IME_CMODE_FULLSHAPE;
			break;
			
		  case ENGLISH_MODE:
			fdwConversion &= IME_CMODE_ALPHANUMERIC;
			break;
		}
		ImmSetConversionStatus( hIMC, fdwConversion, fdwSentence );
		ImmReleaseContext( hWnd, hIMC );
	}
	else
		return( FALSE );
	return( TRUE );
}

// ======================================================
// MakeCompositionChar()
// ======================================================
int MakeCompositionChar( WPARAM wParam )
{

	unsigned char DbcsLoByte, DbcsHiByte;
	int ResultChar;

	DbcsLoByte = (unsigned)( LOBYTE( (WORD)wParam ) );
	DbcsHiByte = (unsigned)( HIBYTE( (WORD)wParam ) );
	
	ResultChar = (int)( (WORD)( (DbcsLoByte << 8) | DbcsHiByte ) );

	return( ResultChar );
}

// ======================================================
// CheckDbcsAlphNumChar()
//
// Filtering DBCS alpha-numeric characters.
// If return value is true, key_event function will be used.
// For trouble shooting about omitting data.
// ======================================================
BOOL
CheckDbcsAlphNumChar( DWORD TestChar )
{
	switch( TestChar )	{
	  case 0x0000a2a6: /* '`'key value */
	  case 0x0000a1a1: /* DBCS space */
		return( TRUE );
		
	  case 0x0000a3ac:
		break;

	  default:
		if( (TestChar >= 0xa3a1) && (TestChar <=0xa3fd) )
			return( TRUE );
		break;
	}
	return (FALSE);
}

#endif


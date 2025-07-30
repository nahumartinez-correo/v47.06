/* $Revision: 1.3 $ $Date: 1995/06/27 16:45:32 $
 * $Locker: $
 */

//---------------------------------------------------------------------------
//
//  UBR.H       UBR/M functions, types and definitions
//
//              Version 2.0 Patch 1
//
//              Laboratorio Software - Olivetti Ricerca S.c.p.A.
//              Carsoli (AQ) - Italy
//
//  Created:
//  Modified:   16.03.95
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Peripherals codes
#define BADGEREADER      2
#define KIOSK_KEYBOARD   4
#define ALARMS          11
#define SERIALPRINTER   14

//---------------------------------------------------------------------------
//  LOCK and UNLOCK return codes
#define USABLEDRIVER     0
#define WRONGDRIVER    (-1)
#define BUSYDRIVER       1

//---------------------------------------------------------------------------
//  COMMANDS CODES

// Common
#define PUOPEN           0


// Badge Reader
#define ENABINS          3
#define ENABCCN          4
#define DISABIN          5
#define READT1          70
#define READT2          71
#define READT3          72
#define NW_READT1      102
#define NW_READT2      103
#define NW_READT3      104
#define WRITET3          9
#define EJECTCC         10
#define TAKECC          11
#define BGTSSW          12
#define BGGETDT         13
#define BGGETST         14
#define EJECTST         17
#define WRITET1         19
#define WRITET2         20

#define NOWAIT_READ	32

// Alarms
#define ALRMTS           5

// Keyboard
#define KEYBHIT          5
#define RELDEV           4
#define SELDEV           3

// Serial printer
#define PRINTST          3
#define CUTST            4
#define GETSTST          5
#define CKRCTR           6
#define GETINTST        20

#define SETDOTFEED      40
#define SETDEFFONT      41
#define LDGRAPHPAGE     42
#define LDFONT          43
#define PRINTBC         44
#define PRINTGT         45
#define PRINTGP         46
#define LDLOGO          47

#define SETVMODE        48
#define SETHMODE        49
#define SETFONTTYPE     50
#define LINEFEED        51
#define STEPPER         52
#define SETVTAB         53
#define SETFONT         54
#define PRINTLOGO       55
#define PAPERCUT        56


//---------------------------------------------------------------------------
//  Parameters codes
#define NO_WAIT          0
#define WAIT             1

#define NOTLOCKED        0
#define LOCKED           1


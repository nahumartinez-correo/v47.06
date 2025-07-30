/* $Revision: 1 $ $Date: 10/31/95 11:40a $
 * $Locker: $
 */

//---------------------------------------------------------------------------
//
//  UBRMSPL.H
// 
//  Sequencers - Protocol Layer Interface Include File
//---------------------------------------------------------------------------

// size of trans / receiv. buffers
#define SIZETXRXBUFFER          256
#define SIZETXPRINTBUFFER       0xFA0


// spl messages
#define UM_STARTX               WM_USER
#define UM_ENDTX                (WM_USER + 1)

// common internal reply codes
#define IRC_MEMERROR            0x01    // 21.86 with Message Box
#define IRC_INTERNALERROR       0x05    // 21.86 without Message Box
#define IRC_EMPTYBUFFER         0x31    // 21.84
#define IRC_BADPARMS            0x31    // 21.84
#define IRC_TIMEOUT             0x60    // 21.02
#define IRC_RXTIMEOUT           0x60    // 21.02
#define IRC_BADCRC              0x61    // 21.06
#define IRC_LINEERROR           0x62    // 21.07
#define IRC_SWOVERFLOW          0x63    // 21.85
#define IRC_NAK                 0x64    // 21.09
#define IRC_CHANNELNOTOPEN      0x65    // 21.01

// enquiry
#define STX         0x02
#define ETX         0x03
#define EOT         0x04
#define ENQ         0x05
#define ACK         0x06
#define NAK         0x15
#define BEL         0x07
#define LD          0x7F
#define TR          LD
#define DLE         0x10
#define CR          0x0D

#define DLE                     0x10
#define ENQ                     0x05
#define WUP                     0x12

// possible enquiry returns
#define DC1                     0x11
#define DC2                     0x12
#define DC3                     0x13
#define DC4                     0x14

// transmission types
#define TT_TXRX                 0
#define TT_TX                   1
#define TT_RX                   2
#define TT_POLL                 3
#define TT_TXNOCTRL             4
#define TT_SINC                 5
#define TT_TXRXLONGTOUT         6

// internal wError flag
#define EC_EXPLICIT              0xFF

// internal code for alarm set class
#define EC_ALARMCLASS            0x01

/*
typedef struct tagTRDATA {
    BYTE    bType;          // transmission type
    BYTE    bChannel;       // channel
    LPBYTE  lpbTx;          // transmission buffer pointer
    WORD    wcTx;           // transmission bytes number
    LPBYTE  lpbRx;          // reception buffer pointer
    WORD    wcRx;           // reception buffer size
    LPBYTE  lpbSx;	    // protocol response
    WORD    wcSx;	    // protocol response count
    HWND    hwndSequencer;  // sequencer window handle 
} TRDATA, *LPTRDATA;
*/

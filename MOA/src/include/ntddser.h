/*++ BUILD Version: 0001    // Increment this if a change has global effects

Copyright (c) 1990-1993  Microsoft Corporation

Module Name:

    ntddser.h

Abstract:

    This is the include file that defines all constants and types for
    accessing the Serial device.

Author:

    Steve Wood (stevewo) 27-May-1990

Revision History:

--*/


//
// NtDeviceIoControlFile IoControlCode values for this device.
//

#define IOCTL_SERIAL_SET_BAUD_RATE      CTL_CODE(FILE_DEVICE_SERIAL_PORT, 1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_QUEUE_SIZE     CTL_CODE(FILE_DEVICE_SERIAL_PORT, 2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_LINE_CONTROL   CTL_CODE(FILE_DEVICE_SERIAL_PORT, 3,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_BREAK_ON       CTL_CODE(FILE_DEVICE_SERIAL_PORT, 4,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_BREAK_OFF      CTL_CODE(FILE_DEVICE_SERIAL_PORT, 5,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_IMMEDIATE_CHAR     CTL_CODE(FILE_DEVICE_SERIAL_PORT, 6,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_TIMEOUTS       CTL_CODE(FILE_DEVICE_SERIAL_PORT, 7,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_TIMEOUTS       CTL_CODE(FILE_DEVICE_SERIAL_PORT, 8,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_DTR            CTL_CODE(FILE_DEVICE_SERIAL_PORT, 9,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_CLR_DTR            CTL_CODE(FILE_DEVICE_SERIAL_PORT,10,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_RESET_DEVICE       CTL_CODE(FILE_DEVICE_SERIAL_PORT,11,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_RTS            CTL_CODE(FILE_DEVICE_SERIAL_PORT,12,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_CLR_RTS            CTL_CODE(FILE_DEVICE_SERIAL_PORT,13,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_XOFF           CTL_CODE(FILE_DEVICE_SERIAL_PORT,14,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_XON            CTL_CODE(FILE_DEVICE_SERIAL_PORT,15,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_WAIT_MASK      CTL_CODE(FILE_DEVICE_SERIAL_PORT,16,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_WAIT_MASK      CTL_CODE(FILE_DEVICE_SERIAL_PORT,17,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_WAIT_ON_MASK       CTL_CODE(FILE_DEVICE_SERIAL_PORT,18,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_PURGE              CTL_CODE(FILE_DEVICE_SERIAL_PORT,19,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_BAUD_RATE      CTL_CODE(FILE_DEVICE_SERIAL_PORT,20,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_LINE_CONTROL   CTL_CODE(FILE_DEVICE_SERIAL_PORT,21,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_CHARS          CTL_CODE(FILE_DEVICE_SERIAL_PORT,22,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_CHARS          CTL_CODE(FILE_DEVICE_SERIAL_PORT,23,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_HANDFLOW       CTL_CODE(FILE_DEVICE_SERIAL_PORT,24,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_SET_HANDFLOW       CTL_CODE(FILE_DEVICE_SERIAL_PORT,25,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_MODEMSTATUS    CTL_CODE(FILE_DEVICE_SERIAL_PORT,26,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_COMMSTATUS     CTL_CODE(FILE_DEVICE_SERIAL_PORT,27,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_XOFF_COUNTER       CTL_CODE(FILE_DEVICE_SERIAL_PORT,28,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_PROPERTIES     CTL_CODE(FILE_DEVICE_SERIAL_PORT,29,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SERIAL_GET_DTRRTS         CTL_CODE(FILE_DEVICE_SERIAL_PORT,30,METHOD_BUFFERED,FILE_ANY_ACCESS)


// begin_winioctl

#define IOCTL_SERIAL_LSRMST_INSERT      CTL_CODE(FILE_DEVICE_SERIAL_PORT,31,METHOD_BUFFERED,FILE_ANY_ACCESS)

// end_winioctl

//
// NtDeviceIoControlFile InputBuffer/OutputBuffer record structures for
// this device.
//

//
// This structure used to set line parameters.
//

typedef struct _SERIAL_LINE_CONTROL {
    UCHAR StopBits;
    UCHAR Parity;
    UCHAR WordLength;
    } SERIAL_LINE_CONTROL,*PSERIAL_LINE_CONTROL;

typedef struct _SERIAL_TIMEOUTS {
    ULONG ReadIntervalTimeout;
    ULONG ReadTotalTimeoutMultiplier;
    ULONG ReadTotalTimeoutConstant;
    ULONG WriteTotalTimeoutMultiplier;
    ULONG WriteTotalTimeoutConstant;
    } SERIAL_TIMEOUTS,*PSERIAL_TIMEOUTS;

//
// This structure used to resize the input/output buffers.
// An error code will be returned if the size exceeds the
// drivers capacity.  The driver reserves the right to
// allocate a larger buffer.
//

typedef struct _SERIAL_QUEUE_SIZE {
    ULONG InSize;
    ULONG OutSize;
    } SERIAL_QUEUE_SIZE,*PSERIAL_QUEUE_SIZE;


//
// This structure used by set baud rate
//

typedef struct _SERIAL_BAUD_RATE {
    ULONG BaudRate;
    } SERIAL_BAUD_RATE,*PSERIAL_BAUD_RATE;

//
// Defines the bitmask that the driver can used to notify
// app of various changes in the state of the UART.
//

#define SERIAL_EV_RXCHAR           0x0001  // Any Character received
#define SERIAL_EV_RXFLAG           0x0002  // Received certain character
#define SERIAL_EV_TXEMPTY          0x0004  // Transmitt Queue Empty
#define SERIAL_EV_CTS              0x0008  // CTS changed state
#define SERIAL_EV_DSR              0x0010  // DSR changed state
#define SERIAL_EV_RLSD             0x0020  // RLSD changed state
#define SERIAL_EV_BREAK            0x0040  // BREAK received
#define SERIAL_EV_ERR              0x0080  // Line status error occurred
#define SERIAL_EV_RING             0x0100  // Ring signal detected
#define SERIAL_EV_PERR             0x0200  // Printer error occured
#define SERIAL_EV_RX80FULL         0x0400  // Receive buffer is 80 percent full
#define SERIAL_EV_EVENT1           0x0800  // Provider specific event 1
#define SERIAL_EV_EVENT2           0x1000  // Provider specific event 2

//
// A longword is used to send down a mask that
// instructs the driver what to purge.
//
// SERIAL_PURGE_TXABORT - Implies the current and all pending writes.
// SERIAL_PURGE_RXABORT - Implies the current and all pending reads.
// SERIAL_PURGE_TXCLEAR - Implies the transmit buffer if exists
// SERIAL_PURGE_RXCLEAR - Implies the receive buffer if exists.
//

#define SERIAL_PURGE_TXABORT 0x00000001
#define SERIAL_PURGE_RXABORT 0x00000002
#define SERIAL_PURGE_TXCLEAR 0x00000004
#define SERIAL_PURGE_RXCLEAR 0x00000008

//
// Communication defines
//

#define STOP_BIT_1      0
#define STOP_BITS_1_5   1
#define STOP_BITS_2     2

#define NO_PARITY        0
#define ODD_PARITY       1
#define EVEN_PARITY      2
#define MARK_PARITY      3
#define SPACE_PARITY     4


//
// This structure is used to set and retrieve the special characters
// used by the nt serial driver.
//
// Note that the driver will return an error if xonchar == xoffchar.
//

typedef struct _SERIAL_CHARS {
    UCHAR EofChar;
    UCHAR ErrorChar;
    UCHAR BreakChar;
    UCHAR EventChar;
    UCHAR XonChar;
    UCHAR XoffChar;
    } SERIAL_CHARS,*PSERIAL_CHARS;

//
// This structure is used to contain the flow control
// and handshaking setup.
//
// A reasonably precise explaination of how they all
// work can be found in the OS/2 tech references.
//
// For Xon/Xofflimit:
//
// When there are more characters then
//
// (typeaheadbuffersize - xofflimit)
//
// in the typeahead buffer then the driver will perform all flow
// control that the app has enabled so that the sender will (hopefully)
// stop sending characters.
//
// When there are less than xonlimit number of characters in the
// typeahead buffer the driver will perform all flow control that
// the app has enabled so that the sender will hopefully start sending
// characters again.
//
// It should be noted that if Xoff character is sent then the
// driver will also stop transmitting any more characters.  This is to
// provide support for those systems that take any character that
// follows an Xoff as an implied Xon.
//

typedef struct _SERIAL_HANDFLOW {
    ULONG ControlHandShake;
    ULONG FlowReplace;
    LONG XonLimit;
    LONG XoffLimit;
    } SERIAL_HANDFLOW,*PSERIAL_HANDFLOW;

#define SERIAL_DTR_MASK           ((ULONG)0x03)
#define SERIAL_DTR_CONTROL        ((ULONG)0x01)
#define SERIAL_DTR_HANDSHAKE      ((ULONG)0x02)
#define SERIAL_CTS_HANDSHAKE      ((ULONG)0x08)
#define SERIAL_DSR_HANDSHAKE      ((ULONG)0x10)
#define SERIAL_DCD_HANDSHAKE      ((ULONG)0x20)
#define SERIAL_OUT_HANDSHAKEMASK  ((ULONG)0x38)
#define SERIAL_DSR_SENSITIVITY    ((ULONG)0x40)
#define SERIAL_ERROR_ABORT        ((ULONG)0x80000000)
#define SERIAL_CONTROL_INVALID    ((ULONG)0x7fffff84)
#define SERIAL_AUTO_TRANSMIT      ((ULONG)0x01)
#define SERIAL_AUTO_RECEIVE       ((ULONG)0x02)
#define SERIAL_ERROR_CHAR         ((ULONG)0x04)
#define SERIAL_NULL_STRIPPING     ((ULONG)0x08)
#define SERIAL_BREAK_CHAR         ((ULONG)0x10)
#define SERIAL_RTS_MASK           ((ULONG)0xc0)
#define SERIAL_RTS_CONTROL        ((ULONG)0x40)
#define SERIAL_RTS_HANDSHAKE      ((ULONG)0x80)
#define SERIAL_TRANSMIT_TOGGLE    ((ULONG)0xc0)
#define SERIAL_XOFF_CONTINUE      ((ULONG)0x80000000)
#define SERIAL_FLOW_INVALID       ((ULONG)0x7fffff20)

//
// These are the following reasons that the device could be holding.
//
#define SERIAL_TX_WAITING_FOR_CTS      ((ULONG)0x00000001)
#define SERIAL_TX_WAITING_FOR_DSR      ((ULONG)0x00000002)
#define SERIAL_TX_WAITING_FOR_DCD      ((ULONG)0x00000004)
#define SERIAL_TX_WAITING_FOR_XON      ((ULONG)0x00000008)
#define SERIAL_TX_WAITING_XOFF_SENT    ((ULONG)0x00000010)
#define SERIAL_TX_WAITING_ON_BREAK     ((ULONG)0x00000020)
#define SERIAL_RX_WAITING_FOR_DSR      ((ULONG)0x00000040)

//
// These are the error values that can be returned by the
// driver.
//
#define SERIAL_ERROR_BREAK             ((ULONG)0x00000001)
#define SERIAL_ERROR_FRAMING           ((ULONG)0x00000002)
#define SERIAL_ERROR_OVERRUN           ((ULONG)0x00000004)
#define SERIAL_ERROR_QUEUEOVERRUN      ((ULONG)0x00000008)
#define SERIAL_ERROR_PARITY            ((ULONG)0x00000010)


//
// This structure is used to get the current error and
// general status of the driver.
//

typedef struct _SERIAL_STATUS {
    ULONG Errors;
    ULONG HoldReasons;
    ULONG AmountInInQueue;
    ULONG AmountInOutQueue;
    BOOLEAN EofReceived;
    BOOLEAN WaitForImmediate;
    } SERIAL_STATUS,*PSERIAL_STATUS;

//
// This structure is used for XOFF counter ioctl.  The xoff ioctl
// is used to support those subsystems that feel the need to emulate
// the serial chip in software.
//
// It has the following semantics:
//
// This io request is placed into the normal device write
// queue.  That is, it will be queued behind any writes
// already given to the driver.
//
// When this request becomes the current request, the character
// specified in the field XoffChar will be sent, subject to
// all other flow control already defined.
//
// Immediately upon sending the character the driver will
// perform the following actions.
//
// A timer will be initiated that will expire after the
// number of milliseconds in the Timeout field of the
// SERIAL_XOFF_COUNTER structure.
//
// The driver will initialize a counter to the value specified
// in the Counter field of the SERIAL_XOFF_RECORD.  The driver
// will decrement this counter whenever a character is received.
//
// This request will then be held by the driver.  It will
// actually complete under the following circumstances:
//
// 1) If there is another "write" request behind it in the queue.
//    The "xoff" request will be completed with the informational status
//    STATUS_SERIAL_MORE_WRITES.  The Information field of the
//    IOSTATUS block will be set to 0.
//
//    Note: By write request we mean another SERIAL_XOFF_COUNTER
//    request, or a simple write request.  If the only subsequent
//    request is a flush request, the driver WILL NOT automatically
//    complete the SERIAL_XOFF_COUNTER request.  NOTE: Transmit
//    immediate requests DO NOT count as a normal write, and therefore
//    would not cause a counter request to complete.
//
// 2) The timer expires.  The driver will complete the request
//    with the informational status STATUS_SERIAL_COUNTER_TIMEOUT.
//    The Information field of the IOSTATUS of the request will be set to 0.
//
// 3) The driver maintained counter goes to zero.  (By implication,
//    at least "Counter" number of characters have been received.)
//    The request will be completed with a successful status
//    of STATUS_SUCCESS.  The Information field of the
//    IOSTATUS of the request will be set to 0.
//
// 4) This is really a degenerate case of "1" above.  The request
//    is started and no request follow it on the queue.  However
//    at some point, before "2" or "3" above occur, another "write"
//    request is started.  This will cause the completion actions
//    stated in "1" to occur.
//
// NOTE: This request being issued WILL NOT cause the normal flow
//       control code of the driver to be invoked.
//
// NOTE: This request has no interaction with the IOCTL_SERIAL_WAIT_ON_MASK
//       request.  An application CAN NOT wait via the above ^^^^^^ ioctl
//       on the counter going to zero.  The application must synchronize
//       with the particular IOCTL_SERIAL_XOFF_COUNTER request.
//
// NOTE: The Timeout value equal to zero would cause the counter
//       to NEVER timeout.  The only way that such a request could
//       be killed at that point would be issue another write, or
//       to purge the WRITE queue.
//

typedef struct _SERIAL_XOFF_COUNTER {
    ULONG Timeout; // Zero based.  In milliseconds
    LONG Counter; // Must be greater than zero.
    UCHAR XoffChar;
    } SERIAL_XOFF_COUNTER,*PSERIAL_XOFF_COUNTER;

//
// The following structure (and defines) are passed back by
// the serial driver in response to the get properties ioctl.
//

#define SERIAL_SP_SERIALCOMM         ((ULONG)0x00000001)

//
// Provider subtypes
//
#define SERIAL_SP_UNSPECIFIED       ((ULONG)0x00000000)
#define SERIAL_SP_RS232             ((ULONG)0x00000001)
#define SERIAL_SP_PARALLEL          ((ULONG)0x00000002)
#define SERIAL_SP_RS422             ((ULONG)0x00000003)
#define SERIAL_SP_RS423             ((ULONG)0x00000004)
#define SERIAL_SP_RS449             ((ULONG)0x00000005)
#define SERIAL_SP_FAX               ((ULONG)0x00000021)
#define SERIAL_SP_SCANNER           ((ULONG)0x00000022)
#define SERIAL_SP_BRIDGE            ((ULONG)0x00000100)
#define SERIAL_SP_LAT               ((ULONG)0x00000101)
#define SERIAL_SP_TELNET            ((ULONG)0x00000102)
#define SERIAL_SP_X25               ((ULONG)0x00000103)

//
// Provider capabilities flags.
//

#define SERIAL_PCF_DTRDSR        ((ULONG)0x0001)
#define SERIAL_PCF_RTSCTS        ((ULONG)0x0002)
#define SERIAL_PCF_CD            ((ULONG)0x0004)
#define SERIAL_PCF_PARITY_CHECK  ((ULONG)0x0008)
#define SERIAL_PCF_XONXOFF       ((ULONG)0x0010)
#define SERIAL_PCF_SETXCHAR      ((ULONG)0x0020)
#define SERIAL_PCF_TOTALTIMEOUTS ((ULONG)0x0040)
#define SERIAL_PCF_INTTIMEOUTS   ((ULONG)0x0080)
#define SERIAL_PCF_SPECIALCHARS  ((ULONG)0x0100)
#define SERIAL_PCF_16BITMODE     ((ULONG)0x0200)

//
// Comm provider settable parameters.
//

#define SERIAL_SP_PARITY         ((ULONG)0x0001)
#define SERIAL_SP_BAUD           ((ULONG)0x0002)
#define SERIAL_SP_DATABITS       ((ULONG)0x0004)
#define SERIAL_SP_STOPBITS       ((ULONG)0x0008)
#define SERIAL_SP_HANDSHAKING    ((ULONG)0x0010)
#define SERIAL_SP_PARITY_CHECK   ((ULONG)0x0020)
#define SERIAL_SP_CARRIER_DETECT ((ULONG)0x0040)

//
// Settable baud rates in the provider.
//

#define SERIAL_BAUD_075          ((ULONG)0x00000001)
#define SERIAL_BAUD_110          ((ULONG)0x00000002)
#define SERIAL_BAUD_134_5        ((ULONG)0x00000004)
#define SERIAL_BAUD_150          ((ULONG)0x00000008)
#define SERIAL_BAUD_300          ((ULONG)0x00000010)
#define SERIAL_BAUD_600          ((ULONG)0x00000020)
#define SERIAL_BAUD_1200         ((ULONG)0x00000040)
#define SERIAL_BAUD_1800         ((ULONG)0x00000080)
#define SERIAL_BAUD_2400         ((ULONG)0x00000100)
#define SERIAL_BAUD_4800         ((ULONG)0x00000200)
#define SERIAL_BAUD_7200         ((ULONG)0x00000400)
#define SERIAL_BAUD_9600         ((ULONG)0x00000800)
#define SERIAL_BAUD_14400        ((ULONG)0x00001000)
#define SERIAL_BAUD_19200        ((ULONG)0x00002000)
#define SERIAL_BAUD_38400        ((ULONG)0x00004000)
#define SERIAL_BAUD_56K          ((ULONG)0x00008000)
#define SERIAL_BAUD_128K         ((ULONG)0x00010000)
#define SERIAL_BAUD_115200       ((ULONG)0x00020000)
#define SERIAL_BAUD_57600        ((ULONG)0x00040000)
#define SERIAL_BAUD_USER         ((ULONG)0x10000000)

//
// Settable Data Bits
//

#define SERIAL_DATABITS_5        ((USHORT)0x0001)
#define SERIAL_DATABITS_6        ((USHORT)0x0002)
#define SERIAL_DATABITS_7        ((USHORT)0x0004)
#define SERIAL_DATABITS_8        ((USHORT)0x0008)
#define SERIAL_DATABITS_16       ((USHORT)0x0010)
#define SERIAL_DATABITS_16X      ((USHORT)0x0020)

//
// Settable Stop and Parity bits.
//

#define SERIAL_STOPBITS_10       ((USHORT)0x0001)
#define SERIAL_STOPBITS_15       ((USHORT)0x0002)
#define SERIAL_STOPBITS_20       ((USHORT)0x0004)
#define SERIAL_PARITY_NONE       ((USHORT)0x0100)
#define SERIAL_PARITY_ODD        ((USHORT)0x0200)
#define SERIAL_PARITY_EVEN       ((USHORT)0x0400)
#define SERIAL_PARITY_MARK       ((USHORT)0x0800)
#define SERIAL_PARITY_SPACE      ((USHORT)0x1000)

typedef struct _SERIAL_COMMPROP {
    USHORT PacketLength;
    USHORT PacketVersion;
    ULONG ServiceMask;
    ULONG Reserved1;
    ULONG MaxTxQueue;
    ULONG MaxRxQueue;
    ULONG MaxBaud;
    ULONG ProvSubType;
    ULONG ProvCapabilities;
    ULONG SettableParams;
    ULONG SettableBaud;
    USHORT SettableData;
    USHORT SettableStopParity;
    ULONG CurrentTxQueue;
    ULONG CurrentRxQueue;
    ULONG ProvSpec1;
    ULONG ProvSpec2;
    WCHAR ProvChar[1];
} SERIAL_COMMPROP,*PSERIAL_COMMPROP;

//
// Define masks for the rs-232 input and output.
//

#define SERIAL_DTR_STATE         ((ULONG)0x00000001)
#define SERIAL_RTS_STATE         ((ULONG)0x00000002)
#define SERIAL_CTS_STATE         ((ULONG)0x00000010)
#define SERIAL_DSR_STATE         ((ULONG)0x00000020)
#define SERIAL_RI_STATE          ((ULONG)0x00000040)
#define SERIAL_DCD_STATE         ((ULONG)0x00000080)


// begin_winioctl

//
// The following values follow the escape designator in the
// data stream if the LSRMST_INSERT mode has been turned on.
//
//#define SERIAL_LSRMST_ESCAPE     ((UCHAR)0x00)

//
// Following this value is the contents of the line status
// register, and then the character in the RX hardware when
// the line status register was encountered.
//
//#define SERIAL_LSRMST_LSR_DATA   ((UCHAR)0x01)

//
// Following this value is the contents of the line status
// register.  No error character follows
//
//#define SERIAL_LSRMST_LSR_NODATA ((UCHAR)0x02)

//
// Following this value is the contents of the modem status
// register.
//
//#define SERIAL_LSRMST_MST        ((UCHAR)0x03)

// end_winioctl

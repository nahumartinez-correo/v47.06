/******************************************************************************
**	Filename: 	scanner.h
**	Purpose:	This is the scanner include file.
**	Author:		Mike Niquette
**	Environment:	Portable code, orginally designed for MAC, DOS, 
**			and MSWindows.
**	Library:	None.
**	History:	Creation Date:  08/29/1988
**
**	(c) Copyright Hewlett-Packard Company, 1988.
**	All rights are reserved. Copying or other reproduction of
**	this program except for archival purposes is prohibited
**	without the prior written consent of Hewlett-Packard Company.
**
**			  RESTRICTED RIGHTS LEGEND
**
**	Use, duplication, or disclosure by the Government
**	is subject to restrictions as set forth in
**	paragraph (b) (3) (B) of the Rights in Technical
**	Data and Computer Software clause in
**	DAR 7-104.9(a).
**
**	HEWLETT-PACKARD COMPANY
**	Greeley, Colorado
******************************************************************************/
#ifndef	__SCANNER__
#define	__SCANNER__

/******************************************************************************
**                              IMPORTANT!!!                                 **
**                                                                           **
**      Uncomment the line which describes the environment you are using     **
**                                                                           **
******************************************************************************/
/*#define __PC__*/		/* PC computers */
/*#define __MSW__*/             /* MS Windows */
/*#define __FarProc__*/         /* 32 bit pointers to procedures */
/*#define __FarData__*/         /* 32 bit pointers to data */

/*#define __MAC__		/* Macintosh computers */
/*#define __MPW__*/             /* MacIntosh Programmers Workshop */
/*#define __LIGHTSPEED__*/      /* LightSpeed C */

#ifndef __FarProc__
#define FP
#else
#define FP     far
#endif
#ifndef __FarData__
#define FD
#define HD
#else
#define FD     far
#define HD     huge
#endif

typedef char			INT8;
typedef unsigned char		UINT8;
typedef short int               INT16;
typedef unsigned short int      UINT16;
typedef long			INT32;
typedef unsigned long		UINT32;
typedef float                   FLOAT32;
typedef double                  FLOAT64;
typedef char FD *		PINT8;
typedef unsigned char FD *      PUINT8;
typedef short int FD * 		PINT16;
typedef unsigned short int FD * PUINT16;
typedef long FD *		PINT32;
typedef unsigned long FD *	PUINT32;
typedef float FD * 		PFLOAT32;
typedef double FD *		PFLOAT64;

typedef struct
{
	INT16	Units;			
	INT16	X;
	INT16	Y;
	INT16	Xextent;
	INT16	Yextent;
}	SCANWINDOW;
typedef SCANWINDOW FD *	PSCANWINDOW;

typedef struct
{
	INT16	Model;
	INT16	ADF;
	INT16	WidthPixels;
	INT16	WidthBytes;
	INT16	Lines;
	INT32	Size;
}	SCANINFO;
typedef SCANINFO FD *	PSCANINFO;

typedef struct
{
	INT16	DataType;
	INT16	DitherPattern;
	INT16	Intensity;
	INT16	Contrast;
	INT16	Negative;
	INT16	Mirror;
	INT16	AutoBack;
	INT16	Xresolution;
	INT16	Yresolution;
	INT16	Xscale;
	INT16	Yscale;
	INT16	PixelBits;
	SCANINFO	Info;
	SCANWINDOW	Window;
	INT16	Reserved[16];
} SCANSTATE;
typedef SCANSTATE FD *	PSCANSTATE;


/* SCL maximum lengths */
#define MAXCOMMANDSIZE		10
#define MAXINQUIRESIZE		10
#define MAXRESPONSESIZE		20
#define MAXNUMBERSIZE		10


/* Scanner Command Constants */
#define OFF			0	/* Scanner Option State */
#define ON			1

#define THRESHOLDED 		0	/* Data Types */
#define WHITE			1
#define BLACK			2
#define DITHERED		3
#define GRAYSCALE		4

#define USER_DEFINED		-1	/* Dither Patterns */
#define COARSE_FATTING		0
#define FINE_FATTING		1
#define BAYER			2
#define VERTICAL_LINE		3

#define DEFAULT			0	/* Tone Maps */

#define DITHER 			0	/* Download/Upload Types */
#define TONEMAP			1

#define PIXEL			0	/* Scanner Window Types */
#define DECIPOINT		1

#define FIRST			1	/* Scan Data Transfer Descriptor */
#define NEXT			0

#define LO			'L'	/* Scanner Inquire Type */
#define NOM			'R'
#define HI			'H'


/* Miscellaneous Scanner Constants */
#define HP9190A			0	/* Scanner Model Numbers */
#define HP9195A			1 

#define VALUECHAR		'V'	/* SCL Termination Character */
#define STRINGCHAR		'W'
#define NULLCHAR		'N'
#define ALLCHAR			-1
#define MACBLIND		ALLCHAR
#define MACNOBLIND		-2

#define HIINQUIRE		'H'	/* maximum allowable value */
#define HIRESPONSE		'g'	/* response to H inquire */
#define LOINQUIRE		'L'	/* minimum allowable value */
#define LORESPONSE		'k'	/* response to L inquire */
#define NOMINQUIRE		'R'	/* current value */
#define NOMRESPONSE 		'p'	/* response to R inquire */
#define UPLOADINQUIRE		'U'	/* upload command */
#define UPLOADRESPONSE		't'	/* response to U upload command */
#define VALUEINQUIRE		'E'	/* non-parameter value */
#define VALUERESPONSE		'd'	/* response to E inquire */

/* Scanner library function returns and error codes */
#define OKAY			0	/* No Error */

#define NOACCESS		1	/* Communication Error */
#define BADACCESS		2
#define BADCOMMAND		3
#define BADRESPONSE 		4
#define NULLRESPONSE		5

#define BADCMDFORMAT		6	/* Scanner Error */
#define ILLEGALCMD		7
#define ILLEGALWND		8
#define PARAMERROR		9
#define SCALEERROR		10
#define DITHERERROR 		11
#define TONEMAPERROR		12
#define LAMPERROR		13
#define ADFJAM			14	
#define HOMEMISSING 		15
#define SCANERROR		16


/* Scanner commands */
#define RESET_SCANNER		0
#define CLEAR_ERRORS		1
#define SCAN			2
#define UPLOAD 			3
#define X_RESOLUTION		4
#define Y_RESOLUTION		5
#define X_SCALE			6
#define Y_SCALE			7
#define X_START_DECIPOINT	8
#define Y_START_DECIPOINT	9
#define X_EXTENT_DECIPOINT	10
#define Y_EXTENT_DECIPOINT	11
#define X_START_PIXEL		12
#define Y_START_PIXEL		13
#define X_EXTENT_PIXEL		14
#define Y_EXTENT_PIXEL		15
#define INTENSITY		16
#define CONTRAST		17
#define NEGATIVE		18
#define MIRROR 			19
#define AUTO_BACKGROUND		20
#define DATA_TYPE		21
#define BITS_PER_PIXEL		22
#define DITHER_PATTERN		23 
#define TONE_MAP		24
#define DOWNLOAD_TYPE		25
#define DOWNLOAD		26
#define BAR_Y_POSITION		27
#define TEST_LAMP		28
#define TEST_SCANNER		29
#define ADF_AVAILABLE		30
#define MAX_ERROR_DEPTH		31
#define ERROR_DEPTH 		32
#define CURRENT_ERROR		33
#define OLDEST_ERROR		34
#define PIXELS_PER_LINE		35
#define BYTES_PER_LINE		36
#define LINES_PER_SCAN		37
#define ADF_READY		38
#define PIXELS_PER_INCH		39
#define	LO_STRING		40
#define MODEL_NUMBER		40
#define DATE_CODE		41
#define STATUS_STRING		42
#define	HI_STRING		42
#define LAST_SCL		43			


/* High level scanner command macros */
#define SetAutoBackground(x)	SendCommand(AUTO_BACKGROUND, x)
#define SetContrast(x)		SendCommand(CONTRAST, x)
#define SetDataType(x)		SendCommand(DATA_TYPE, x)
#define SetDitherPattern(x)	SendCommand(DITHER_PATTERN, x)
#define SetIntensity(x)		SendCommand(INTENSITY, x)
#define SetLamp(x)		SendCommand(TEST_LAMP, x)
#define SetMirror(x)		SendCommand(MIRROR, x)
#define SetNegative(x)		SendCommand(NEGATIVE, x)
#define SetPixelBits(x)		SendCommand(BITS_PER_PIXEL, x)
#define SetPosition(x)		SendCommand(BAR_Y_POSITION, x)
#define SetToneMap(x)		SendCommand(TONE_MAP, x)

/* Hi Level Scanner entry points in scanner.c */
INT16 CommandInquire(UINT16, INT16, PINT16);
INT16 Dither(INT16);
INT16 Download(INT16, PUINT8);	
INT16 GetScanInfo(PSCANINFO);	
INT16 GetScanState(PSCANSTATE, UINT16, UINT16);
INT16 GrayScale(INT16);
INT16 ResetScanner();
INT16 Scan(INT16, PUINT8, INT32, PINT32);
INT16 SetDecipointWindow(INT16, INT16, INT16, INT16);
INT16 SetPixelWindow(INT16, INT16, INT16, INT16);
INT16 SetResolution(INT16, INT16);
INT16 SetScale(INT16, INT16);
INT16 SetScanState(PSCANSTATE, UINT16);
INT16 SetScanWindow(PSCANWINDOW, UINT16);
INT16 Threshold();
INT16 Upload(INT16, PUINT8, INT16, PINT16);

/* SCL command interpreter entry points in scl.c */
INT16 BuildCommand(PUINT8, UINT16, INT16);
INT16 BuildInquire(PUINT8, UINT16, UINT16);
INT16 InquireModel(PINT16);
INT16 InquireNumber(UINT16, UINT16, PINT16);
INT16 InquireOldestError();
INT16 InquireString(UINT16, PUINT8, INT16, PINT16);  
INT16 ReceiveResponse(PUINT8, PINT16, INT16);
INT16 SendCommand(UINT16, INT16);

/* Low level I/O entry points in dosscan.c and macscan.c */
INT16 CheckIO();
INT32 RecFromScanner(PUINT8, INT32, INT16);
INT32 SendToScanner(PUINT8, INT32);
#endif

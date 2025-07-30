/*
** Equates for the Control Manager.
*/

/*
** Standard Control types.
*/

#define pushButProc 0		/* Simple button. */
#define checkBoxProc 1		/* Check box. */
#define radioButProc 2		/* Radio button. */
#define defButProc 3		/* Outlined simple button, for default. */
#define useWFont 8		/* Add to above to use current font (not 0). */
#define scrollBarProc 16	/* Scroll bar. */

/*
** Part codes of the standard controls.
*/

#define inButton 10		/* Simple button. */
#define inCheckBox 11		/* Check box or radio button. */
#define inUpButton 20		/* Up arrow of a scroll bar. */
#define inDownButton 21		/* Down arrow of a scroll bar. */
#define inPageUp 22		/* Page up region of scroll bar. */
#define inPageDown 23		/* Page down region of scroll bar. */
#define inThumb 129		/* Thumb of a scroll bar. */

/*
** Values for visible argument to NewControl. 
*/

#define hideControl 0		/* Create hidden control. */
#define showControl 1		/* Create visible control. */

#ifndef CONTROLS_SOURCE
typedef void *ControlID;
#else

/*
** NOTE!  Don't use these values yourself because I reserve the right to
** dick with these definitions myself with no notice.  If this breaks
** something of yours, too bad for you.  You shouldn't depend on the
** internals of something like this anyway.  
*/

/*
** Internal codes for myControl.
*/

#define drawCntl 0	/* Draw the control (or part of it). */
#define testCntl 1	/* Test where mouse button was pressed. */
#define calcCRgns 2	/* Calculate control's regions. */
#define initCntl 3	/* Do any additional control initialization. */
#define dispCntl 4	/* Take any additional disposal actions. */
#define posCntl 5	/* Reposition control's indicator & update it. */
#define thumbCntl 6	/* Calculate parameters for dragging indicator. */
#define dragCntl 7	/* Drag control (or its indicator). */
#define autoTrack 8	/* Execute control's action procedure. */

/*
** Low-level structures.
*/

struct ControlRecord {
    struct ControlRecord *nextControl; /* Next control in list. */
    short contrlOwner;		/* Control's window. */
    Rect contrlRect;		/* Enclosing Rectangle. */
    Point contrlText;		/* Title's starting position. */
    unsigned char contrlVis;	/* TRUE if visible. */
    unsigned char contrlHilite;	/* Highlight state. */
    short contrlValue;		/* Control's current setting. */
    short contrlValue2;		/* Control's setting #2 (for scroll bar). */
    short contrlMin;		/* Control's minimum setting. */
    short contrlMax;		/* Control's maximum setting. */
    short contrlDefProc;	/* Control's type. */
    void *contrlData;		/* Data used internally. */
    void (*contrlAction)();	/* Action procedure. */
    short contrlFont;		/* Control's font for title. */
    short contrlSize;		/* Font size for control. */
    short contrlFColor;		/* Foreground color for control (optional). */
    short contrlBColor;		/* Background color for control (optional). */
    short titleLen;		/* Control's title length. */
    char *contrlTitle;		/* Control's title. */
    long refCon;		/* Reference value. */
    long (*cdefproc)();		/* Pointer to control definition procedure. */
};

typedef struct ControlRecord ControlRecord;
typedef ControlRecord *ControlID;


#endif /* CONTROLS_SOURCE */

#if !defined(lint) || defined(_lint)
/*****************************************************/
/****** Control Manager's available functions. *******/
/*****************************************************/

void InitControls();
void SetCDefProc(long (*cdefproc)());
ControlID NewControl(short theWindow, Rect *boundsRect, char *title, 
			  short visible, short value, short min, short max,
			  short procID, long refCon);
void DisposeControl(ControlID theControl);
void DisposeControlNoErase(ControlID theControl);
void KillControls(register short theWindow);
void SetCTitle(ControlID theControl, char *title);
void HideControl(register ControlID theControl);
void HideControlNoInval(register ControlID theControl);
void ShowControl(ControlID theControl);
void DrawControls(short theWindow);
void DrawControl(ControlID theControl);
void HiliteControl(ControlID theControl, short hiliteState);
short FindControl(Point *thePoint, short theWindow, 
		  ControlID *whichControl);
short TrackControl(ControlID theControl, Point *thePoint,
		   void (*actionProc)());
short TestControl(ControlID theControl, Point *thePoint);
void MoveControl(ControlID theControl, short newh, short newv);
void ShiftControl(register ControlID theControl, register short newh, register short newv);
void SizeControl(ControlID theControl, short width, short height);
void SetCtlValue(ControlID theControl, short theValue);
void SetCtlValues(ControlID theControl, short theValue, short theValue2);
void Set4CtlVals(ControlID theControl, short minValue, short maxValue,
		short theValue, short theValue2);
short IsCtlActive(register ControlID theControl);
short GetCtlValue(register ControlID theControl);
short GetCtlValue2(register ControlID theControl);
void SetCtlMin(ControlID theControl, short minValue);
short GetCtlMin(register ControlID theControl);
void SetCtlMax(ControlID theControl, short maxValue);
short GetCtlMax(register ControlID theControl);
void SetCtlAction(register ControlID theContrl, register
		void (*actionProc)(ControlID theControl, short partCode));
void (*GetCtlAction(register ControlID theContrl))
	(ControlID theControl, short partCode);
void SetCtlScrollRate(register ControlID theContrl, long ticks);
long GetCtlScrollRate(register ControlID theContrl);
void SetCRefCon(register ControlID theControl, register long data);
long GetCRefCon(register ControlID theControl);
/* Prototype for a control definition procedure. */
long myControl(short varCode, ControlID theControl, short message, long param);

#else /* lint */

void InitControls();
void SetCDefProc();
ControlID NewControl();
void DisposeControl();
void DisposeControlNoErase();
void KillControls();
void SetCTitle();
void HideControl();
void HideControlNoInval();
void ShowControl();
void DrawControls();
void DrawControl();
void HiliteControl();
short FindControl();
short TrackControl();
short TestControl();
void MoveControl();
void SizeControl();
void SetCtlValue();
void SetCtlValues();
void Set4CtlVals();
short IsCtlActive();
short GetCtlValue();
short GetCtlValue2();
void SetCtlMin();
short GetCtlMin();
void SetCtlMax();
short GetCtlMax();
void SetCtlAction();
void (*GetCtlAction())();
void SetCtlScrollRate();
long GetCtlScrollRate();
void SetCRefCon();
long GetCRefCon();
/* Prototype for a control definition procedure. */
long myControl();

#endif /* lint */

/*
** header for PCX (PC Paint) pictures
*/

struct pcx {
	unsigned char Manufacturer;
	unsigned char Version;
	unsigned char Encoding;
	unsigned char BitsPerPixel;
	unsigned short MinX;			/* Note: Intel Byte Order */
	unsigned short MinY;			/* Note: Intel Byte Order */
	unsigned short MaxX;			/* Note: Intel Byte Order */
	unsigned short MaxY;			/* Note: Intel Byte Order */
	unsigned short HorzRes;			/* Note: Intel Byte Order */
	unsigned short VertRes;			/* Note: Intel Byte Order */
	unsigned char ColourMap[16][3];
	unsigned char Reserved1;
	unsigned char Planes;			/* number of colour planes */
	unsigned char RowBytes;
	unsigned char Reserved2;
	unsigned char Padding[60];		/* pad to 128 bytes */
};

/*
** DECSRIPTION      DEFINITION
** Manufacturer     10 == PC-Paintbrush PCX
** Version          0 == 2.5
**                  2 == 2.8 with palette info
**                  3 == 2.8 without palette info
**                  5 == 3.0 with palette info
** Encoding         1 == .PCX run-length encoding
** ColourMap        R, G, B for each of 16 colours
**                  The colour Map used is:
**                  0, 0, 0 
**                  255, 255, 255
**                  0, 170, 0   
**                  0, 170, 170  
**                  170, 0, 0
**                  170, 0, 170
**                  170, 170, 0
**                  170, 170, 170
**                  85,  85,  85 
**                  85,  85,  255 
**                  85, 255, 85
**                  85, 255, 255
**                  255,  85,  85
**                  255,  85, 255 
**                  255, 255,  85
**                  255, 255, 255
*/

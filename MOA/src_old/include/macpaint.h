/*
** header for MacPaint Files
*/

struct MacPaint {
	long version;			/* version number (default = 2) */
	Pattern patterns[38];	/* pattern array */
	char future[204];		/* future expansion */
};
typedef struct MacPaint MacPaint;

#define RESOURCE "PNTG"

struct MacPaint2 {
	short desc_length;		/* length of description */
	char desc[63];			/* description (Pascal String) */
	char resource[4];		/* resource fork name */
	char data[4];			/* data fork name */
	char padding[55];
	struct MacPaint orig_header;
};
typedef struct MacPaint2 MacPaint2;

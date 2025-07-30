/*
** Logitech Scanman i/o controls
*/
#define	SMIOC ('S'<<8)
#define SMGETINFO (SMIOC|1) 	/* Get Scanman Information */
#define SMGETSCAN (SMIOC|2)		/* Get Scanman Scan Options */
#define SMSETSCAN (SMIOC|3)		/* Set Scanman Scan Options */

/*
** Structure for SMGETINFO ioctl
*/
struct sminfo {
	short lost_lines;		/* lines lost during last scan */
	short lost_bytes;		/* bytes lost during last scan */
	short changed;			/* resolution changed */
	short resolution;		/* current resolution */
	short rowBytes;			/* max bytes per line at current resolution */
};

/*
** Structure for SM(G/S)ETOPT ioctl
*/
struct smopt {
	unsigned short lines;		/* lines for the next scan */
	unsigned char rowBytes;		/* bytes per line for the next scan */
	unsigned char timeout;		/* scan timeout in ticks */
};

/*
** Resolution defines
*/
#define	SCANRES_NODEVICE	(-1)
#define SCANRES_200_SINGLE	(0)
#define SCANRES_100_MULTI	(1)
#define	SCANRES_200_MULTI	(2)
#define SCANRES_300_MULTI	(3)
#define	SCANRES_400_MULTI	(4)

/*
** timeout stuff
*/
#define SM_TICKLEN			(0.13)			/* in seconds */
#define SM_DEF_TIMEOUT		(38)			/* just under 5 seconds */

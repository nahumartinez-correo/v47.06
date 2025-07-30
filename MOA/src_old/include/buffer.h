/*	Buffer definitions	*/

#ifdef DNIX
#define	MAXL3OVHD	5	/* Maximum protocol header size (Serverprot requires at least 5) */
#else
#define MAXL3OVHD	1
#endif
#define	MAXL3PSC	10	/* x25 log code for packet size */
#define	MAXOOBDATA	32	/* Maximum out of band data length */


#define FLAG_M		0x1		/* X25 M indicator */
#define FLAG_Q		0x2		/* X25 Q indicator */

struct	buffer	{
	int		(*doneproc)();	/* This must be the first field in the buffer struct */
	struct	buffer	*b_next ;
	short	b_flags ;
	short	b_start ;
	short	b_size ;
	unsigned char	b_data[MAXL3OVHD] ;
} ;

struct	buffer	*getebuf();

enum	EV	{
	EV_DOWN,
	EV_FAIL,
	EV_UP,
} ;

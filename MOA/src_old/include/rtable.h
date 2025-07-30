/********************************************************************/
/*  rtable.h	definitions for the runtime CDS interface			*/
/********************************************************************/

#ifndef RTABLE_H
#define RTABLE_H

/* order for selects */
#define	RT_ASCENDING	'A'
#define RT_DESCENDING	'D'

/* select.state bit masks */
#define	MULTIREC		(0x01)
#define	NWTRUE			(0x02)

/* The following structure definitions are used for the CDS verb set.	*/
/* These may be used for custom c source routine which invoke CDS    	*/
/* functions using RT's table handling.                             	*/

typedef struct	RTCRITERIA
{
	byte			ltag;			/* cds object tag value 			*/
	byte 			llen;			/* left qualifier name length		*/
	byte 			*lname;			/* left qualifier name 				*/
	byte			ano;			/* appl no 							*/
	index 			ino;			/* index no							*/
	byte			op;				/* relational operator			   	*/
	byte			rtag;			/* application object tag value 	*/
	byte 			*rname;			/* right qualifier name 			*/
	byte			rano;			/* appl no 							*/
	index 			rino;			/* index no							*/
	index 			length;			/* right qualifier name length or   */
									/* data value length 				*/
	byte			*data;			/* data value 						*/
	char			sign;			/* leading sign char 				*/
} rtcriteria;

typedef struct RTABLELIST
{
	reclen	length;					/* length of table list */
	char	table_name[CLTBLEN];	/* table list buffer, supports single */
									/* table only                         */
} rtablelist;

	/* function prototypes for table operations */

	XDLL int	rtaddfield(register cdsselect *tptr, register field *source,
						byte ano,index ino, field *as);

	byte	*extractfields(register cdsselect *tptr, byte *pclproc,
							rtobj **cargs);
	byte	*appendfields(register cdsselect *tptr, byte *pclproc,
							rtobj **cargs);
	short	recordcount(register cdsselect	*tptr);
	short	setrecord(register cdsselect *tptr, register int count);
	byte	*buildcriteria(register cdsselect *tptr, byte *pclproc,
							rtobj **cargs);
	byte	*fieldcriteria(register cdsselect *tptr, byte *pclproc,
							rtobj **cargs);
	int		listcriteria(register cdsselect *tptr, byte ano, index ino,
							rtobj **cargs);
	void	rtgetallfields(byte *rp, fieldv *fv);
	void 	rtgetfield(register fieldv *fv, register field *fp);
	void	multidisplay(int, register cdsselect *tptr, byte ano, index ino);
	char	*cdsretry(register cdsselect *tptr, byte locktype);
	int		rtclwait(register cdsselect *tptr);
	int		rtgetarray(register fieldv *fv, register short row,
	   		           register short col);
	int 	rtaddarray(cdsselect *tptr, field *fp, byte ano, index ino,
	   		           long rowcol, byte appl_id, short field_id);
	int		rtdereference(register byte *rp,register void **prev_position,
	  		           register fieldv *search_fld);

#endif

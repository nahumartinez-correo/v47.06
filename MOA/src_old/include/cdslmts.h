/********************************************************************
*  cdsmts.h	definitions for the CDS interface
********************************************************************/

#ifndef CDSLIMITS_H
#define CDSLIMITS_H	
#ifndef _WINBASE_
# include <winbase.h>	// for MAX_COMPUTERNAME_LENGTH
#endif

/* general defines */
#define CDS_PATTERN_ESCAPE_CHAR '\\'	 // a single back-slash
#define	MAX_INDEX_NO	29		/* max of 30 index trees */
#define	MAX_KEY_SIZE	128
/* MAX_KEY_FIELDS has a limit of 32 (it uses a bit map of an int) */
#define	MAX_KEY_FIELDS	12
#define MAX_REQUIRED_FIELDS 255
#define	ALTIDXLEN	(8)		/* max length of index name */
#define	TBLEN		(16)	/* max length of table name */
#define	CNTXTLEN	(20)	/* max length of context name */
#define USER_NAME_LEN 21		/* max length of a user(passwd) name */
#define MACHINE_NAME_LEN MAX_COMPUTERNAME_LENGTH+5	/* max length of a machine name 
								(including one domain name)*/
#ifndef MAX_CDS_HOST
// allow some room for domain extention.  May not be enough in extreme cases
#define		MAX_CDS_HOST	(size_t) MACHINE_NAME_LEN + 30
#endif
#define MAX_CDS_PROJECT_NAME	30
#define	MAX_NUMBER_OF_TABLES	255
#define MAX_ROW_SIZE	32768
#define SERVERIDLEN 8	/* max length for server id name */
#if (SERVERIDLEN > USER_NAME_LEN - 1)
#error THE_SERVERIDLEN > USER_NAME_LEN - 1
#endif
#define CLTBLEN	(SERVERIDLEN +1+TBLEN) /* [server_name:]table_name (client side) */
#define MAX_PARTICIPANTS 12  /* maximum number of participants
                                in a distributed transaction */

#endif

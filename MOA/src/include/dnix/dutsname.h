/*$
**	Structure returned by D-NIX uname
*/

	struct dutsname {
		char		sysname[9];
		char		nodename[9];
		char		release[9];
		char		version[9];
		unsigned short	sysorigin;	/* original supplier of Dnix system */
		unsigned short	sysoem;		/* OEM for this system */
		unsigned long	sysserial;	/* Serial number for this system */
		unsigned long	nodeidh;	/* Numerical nodename */
		unsigned long	nodeidl;
		char		machine[9];		/* Type of computter */
		char		osname[17];		/* File name of booted os */
		unsigned short	clocktics;	/* Number of clock ticks/second */
		};

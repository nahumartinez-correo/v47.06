// clccb.  common stuff between clconn.h and x/cmxbuf.h
	unsigned short updates; 
	short	interface_version;  // that server is presenting
	clenv	*env;	   // point back to owner
	char cds_host[MAX_CDS_HOST+1];
	SOCKET	sock;
	char	svname[16+MAX_CDS_HOST+MAX_PROJ_NAME+MAX_REV_LEVEL+SERVERIDLEN];
			/* name of client's server */
	char	server_id[SERVERIDLEN + 1];
	unsigned short	cmbufID;	/* buffer identity for find_n */
	unsigned short	cmbufgoodsize;	/* calculated good size */
	short push_level;
	va_list argp;
	int	rec_count;	 // records count used only in clwait()
	int rqid;
	bool	resilient;
	byte Cds_No_Wait;
 	char Last_read_canceled;
	short use_count;
	const fdes	*ftab;
	int pid;


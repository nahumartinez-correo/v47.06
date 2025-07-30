typedef struct PROJMAN {
	char			projname[MAX_PROJ_NAME + 1];	/* project name			*/
	char			path[MAX_PATH_NAME+ 1];	/* path to data dictionary dir	*/
	int				indexfd;			/* file descriptor for index file	*/
	int				datafd;				/* file descriptor for data file	*/
	short			openflag;			/* TRUE if open, FALSE if not		*/
	short			mode;				/* MAINTAIN, RUN, RUN_UNLOCKED		*/
	BTC				itembtc;			/* btcommo for item number index	*/
	BTC				namebtc;			/* btcommo for name index			*/
	itemidx			*itemindx;			/* pointer to item indexes			*/
	applman			*applindx;			/* pointer to appl indexes			*/
	appl0rec		appl0;				/* header info stored in appl 0		*/
	} projman;

extern	projman			dd[MAX_OPEN_DICTS];

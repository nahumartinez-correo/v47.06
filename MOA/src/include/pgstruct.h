struct pgstruct {
   long   topbloc;     /* top of block record location. */
   long   endbloc;     /* end of block record location. */
   long   tmplng1;     /* */
   long   tmplng2;     /* */
   char  *topbkey;     /* key */
   char  *endbkey;     /* key */
};
#define SZ_PGSTRUCT sizeof(struct pgstruct)


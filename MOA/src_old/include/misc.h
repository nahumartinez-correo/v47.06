/* -------------------------------------------------------------------- */
/*  Function templates (prototypes) for things in the 'misc' directory  */
/* -------------------------------------------------------------------- */

#ifndef MISC_H
#define MISC_H

			/* Find the first occurrence of string t in string s. */
	XDLL char	*strindex(char	*s, char *t);
	XDLL char	*crypt(register const char *, register const char *t);

	// ---------------
	//  fn.c
	// ---------------
	XDLL char	*GetFieldName(int, field *);
	XDLL char	*GetFieldNameByNum(int, byte, index);

	/* ----------- */
	/*  hexdump.c  */
	/* ----------- */

	XDLL void	hexdump(FILE *fh, char *title, int offset, void *from,
					short len, short size);
	XDLL void	hexdmp(HANDLE fh, char *title, int offset, void *from,
					short len, short size);

    XDLL void	prnt(HANDLE outFH, const char *format, ...);  // WriteFile() 

	/* ------------- */
	/*  pclcommon.c  */
	/* ------------- */

	XDLL int		optype(byte ch);
#ifndef FUNCPROTOS_H
	XDLL int		tsize(register byte *s);
#endif
	XDLL void	separate_names(byte *ap, byte *np, byte *s);
	XDLL int 	validnumber(byte *bp, field *fp);
	XDLL char	*make_filename(char *fname);
	XDLL char	*tempdir(void);
	XDLL void	nap(int wait);
	XDLL char	*fld_typename(int);

	// ------------------
	//  MSGNAME.C
	// ------------------

	char		*win_msg_name(long);

	// ------------------
	//  VERINFO.C
	// ------------------

	void	PrintRelInfo(LPTSTR FileName);
	LPTSTR  moaVerMsg(char *space, LPTSTR filename); // returns message same as printed by PrintRelInfo()
	LPVOID	moaLoadVerStruct(LPTSTR FileName);
	char 	*moaGetVerString(LPVOID VerStruct, char *StrName);
	void	moaFreeVerStruct(LPVOID VerStruct);

#endif

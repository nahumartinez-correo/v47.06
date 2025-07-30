// ================================================================
//  Function templates (prototypes) used in RT, etc.  The XDLL
//  macro exposes routines & data to external DLLs which want to
//  access stuff within the RT.DLL
// ================================================================

#ifndef FUNCPROTOS_H
#define FUNCPROTOS_H

#ifndef TYPEDEFS_H
#	include <typedefs.h>
#endif
#ifndef RT_H
#	include <rt.h>
#endif
#ifndef DECMATH_H
#	include <decmath.h>
#endif

#ifndef SWERROR_H
#	include <swerror.h>
#endif
#ifndef DSPLIST_H
#	include <dsplist.h>
#endif
#ifndef SCRLIST_H
#	include <scrlist.h>
#endif
#ifndef _STDIO_H
#	include <stdio.h>
#endif
#ifndef _WINSOCKAPI_
#include <winsock.h>
#endif

	XDLL int	tsize(register byte *);

// --------------------------
// pcl/pcl2xxx.c
// --------------------------
	XDLL field	*rt_get_fp(register byte *, register short *, rtobj **cargs);
	XDLL int	pcl2decwork(register byte *from, register decwork *to, rtobj **cargs);
	XDLL int	pcl2field(register byte *from, register field *to, rtobj **cargs);
	XDLL int	pcl2int(register byte *from, register int *to, rtobj **cargs);
	XDLL int	pcl2str(register byte *from, register char *to,
						register short maxlen, rtobj **cargs);
	byte		*eval_expr(register byte *nbp, int *result, rtobj **cargs);

// --------------------------
// pcl/rtobj2xxx.c
// --------------------------
	XDLL int	rtobj2int(register rtobj *thing, register int *to);
	XDLL int	rtobj2float(register rtobj *thing, register double *to);
	XDLL int	rtobj2decwork(register rtobj *thing, register decwork *to);
	XDLL int	rtobj2str(register rtobj *thing, register char *to,
				register int len);
	XDLL int	rtobj2char(register rtobj *thing, register char *to); 

// --------------------------
// rt/editfill.c
// --------------------------
	XDLL int	editlength(field *);
	XDLL int	editstring(char *, int, char *, char *, int, int, char);
	XDLL int	editfill(char *, void *, field *, char);

	int 		edit_check_valid(char *, int, int, int);
	void		edit_errfill(char *, int);
	int 		editsuffixlength(field *);
	void		edit_pad(char *,char, int,char *,char *,char *,char *,int);
	int 		edit_cursorloc(field *, int, int);
	void		edit_shift_date(field *, char *, int);
	int 		editdatechar(field *, int);
	int 		isspacestr(register char *p);
	int 		edit_de_edit(register char *src, register char *dest,
					register char *pattern, register short selstart,
					register short selend, register short calc_mode,
					register short datatype, register field *fp);

// --------------------------
// csr/datecsr.c
// --------------------------
	XDLL int	date_subtract(char *, char *);
	XDLL int	year_of(char *);
	XDLL int	day_of(char *);
	XDLL int	day_of_week(char *);
	XDLL int	valid_date(char *, char *);
	XDLL void	make_date_string(char *, int, int, int);
	XDLL int	date_add(char *, int, char, char *);
	XDLL int	date_compare(char *, char *);
	XDLL int	date_convert(char *, int, char *, char *, char *);
	XDLL int	yeartype(int);
	XDLL void	date_set_error(char *target);
	XDLL int	month_of(char *);
	XDLL int	day_of_year(char *);
	XDLL int	f_valid_date(int, rtobj **);
	XDLL int	f_day_of_week(int, rtobj **);
	XDLL int	day_name(int, rtobj **);
	XDLL int	month_name(int, rtobj **);
	XDLL int	f_day_of(int, rtobj **);
	XDLL int	f_month_of(int, rtobj **);
	XDLL int	f_year_of(int, rtobj **);
	XDLL int	f_date_subtract(int, rtobj **);
	XDLL int	f_date_add(int, rtobj **);
	XDLL int	f_date_compare(int, rtobj **);
	XDLL int	f_date_convert(int, rtobj **);
	XDLL int	f_day_of_year(int, rtobj **);
	XDLL int	last_day(int, rtobj **);
	XDLL int	date_init();
	XDLL long	days_to(char *);
	XDLL int	firstday(int);

	void		set_csr_string(char *);
	char		**bld_cal(int, int);

// --------------------------
// io.c
// --------------------------
	XDLL void	display_internal_error(char *);

	void		clear_screen(void);
	int		spy_on(char *spyee);
	int		spy_off(char *spyee);
	int		killspyserver(void);
	int		rtserverlisten();
	void		spyreaddone(HWND hwnd,SOCKET sock,int len,int flags,void *userp);
	void		spylistendone(HWND hwnd,SOCKET sock,int len,int flags,void *userp);
	void		settitle(char *str);
	int		getuserkey(bool rflag);
	void		invoke_localprint(void);
	void		ungetuserkey(int ch);
	int 		PCLredraw(void);
	int		status_update(void);
	int		get_string(dlist *dlptr,char *str,short size);
	dlist		*put_title(dlist *dlu,short y,short boxtype,char *title,
							void (*sub)(dlist *));
	XDLL void	show_cursor(short x,short y);
	void		show_cursor_full(short x,short y);
	XDLL void	hide_cursor(void);
#if 0
	int		copykeys(EventRecord *ep);
#endif
	void		badinput(void);
	void		warningbell(void);
	void		errorbell(void);
	int 		copyvideo(char *system, char *title);
	void		copyoff(register int id);
	int		xlatefkey(int ch);
	int		rt_find_keyval(int key,short startat);
	void		pcl_exec_error(byte obj, byte ano, short ino);
	int		getfromkeybuff(void);
	void		addtokeybuff(int ch);
	void		zoom_it(void);
	void		purge_typeahead(void);
	void		set_physicalsize(char *s);
	void		set_titlebar(char *s);
	void		window_init(char *whichsys);
#ifdef _WINUSER_
	void		check_maxsize(WINDOWPOS *winfo);
#endif
	int 		rt_get_nextkey(void);
	int		appl_alt_key(int keycode);

// --------------------------
// initstmt.c
// --------------------------
	int		do_init_statement(byte *NBP, rtobj **cargs);
	void		rt_output_timers(FILE *fptr, register char *cp, register int num,
							register long *times, register short is);
	void		rt_stop_timer(register int val);
	void		rt_start_timer(register int val);

// --------------------------
// debug.c 
// --------------------------
	void	df_ind(int *ip);
	void	dump_field(field *fp);
	void	dump_fldlist(void);
	char 	*makeobj(short pre);
	void	debug_dl(struct DLIST *head);
	void	dumpscreenlist(struct SLIST *slp);
	void	dumptran(struct TRANREC *tlp);
	void	dump_fld_stacks(void);
	void	dump_navlist(struct NAVREC *nlp);
	void	dump_gl(struct LIST *glp);
	void	dump_hex(void);
	void	dump_timers(void);
	void	show_menuname(void);
	void	show_funkeydefs(void);
	void	print_fkeydef(short i);
	char	*get_object_name(byte object,byte *op);
	void	dump_flags(field *fp,short size,int flags);
	void	debug_somelist(void);
	void	checkRAM(void);
	void	df_dumparea(field *fp, byte *loc);
	void	dump_misc_info(void);
	void	dump_field_test(field *fp);
	char	*dc_getname(register field *fp, register byte *ano,index *ino);

// --------------------------
//  rt.c 
// --------------------------
	XDLL BOOL	ToolsInit(HINSTANCE, const char *);
	XDLL BOOL	ToolsReInit(const char *);
	XDLL void	ToolsClose(void);
	XDLL BOOL	ToolsOpenRTwindow(void);
	XDLL void	ToolsCloseRTwindow(void);
	XDLL void	ToolsClearRTsGlobals(void);

	void	post_cleanup(void);
	void	initialize(int argc, char *argv[]);
	void	reset_exit(void);
	void	math_error(void);
	bool	init_field(byte appl, char *name, char *from);
	void	setup_keyboard_input(void);
	void	setup_common_fields(void);
	void	setup_hardware_type(void);
	int 	rtcdsinit(char *sv_name, short retry);
	void	init_errfyi(void);
	void	onechar_setup(void);

// --------------------------
//  pclsubs.c   
// --------------------------
	XDLL void	refreshfield(byte ano, index ino, field *desiredfp);
	XDLL int	rt_collect(byte ano, index ino);
	XDLL void	rt_undisplay(byte ano, index ino);
	XDLL bool	rt_display(byte ano, index ino);
	XDLL int	rt_combofield(int, int, field *, pcldef *, unsigned long, 
							pcldef *, picheader *, int);
	XDLL int	rt_comboverify(unsigned long tag);
	XDLL int	rt_combopick(unsigned long tag, rtcontrol *crp);

	void	CallStack_reset(void);
	void	CallStack_dump(void);
	void	CallStack_add(pcldef *cp);
	void	CallStack_remove(pcldef *cp);

	byte	*do_dprint(byte *bp, rtobj **cargs);
	byte	*do_fprint(byte *bp, rtobj **cargs);
	int 	dorefresh(byte *bp, rtobj **cargs);
	void	do_settimer(register short whichtimer, register short function,
						int seconds, byte object, byte ano, index ino,
						long own_id);
	void	keydef_clear(void);
	void	keydef_pop(void);
	void	keydef_push(void);
	void	keydef_reset(void);
	int 	lookupstmt(byte *bp, rtobj **cargs);
	char	*make_stringbuff(void);
	struct TRANREC *make_tran(void);
	byte	*navigateback(byte **ptr, byte **inuse, byte **PPaddress);
	void	pcl_doreset(byte rs_type);
	void	plug_at_comm_ptr(byte type, char value);
	void	plug_comm_ptr(byte type, char *value);
	void	prefill(register field *fp);
	void	purge_all_screens(void);
	void	remove_tran(struct TRANREC *tp, short flag);
	int 	rt_comboverify(unsigned long);
	int 	rt_combofield(int,int,field *,pcldef *,unsigned long,pcldef *,
						picheader *, int);
	void 	rt_dl_free(dlist *head);
	void	rt_list_init(void);
	int 	rt_redraw(byte *bp, rtobj **cargs);
	void	*setup_utsname(void);
	void	stack_clear(field *fp);
	void	stack_pop(field *fp);
	void	stack_push(field *fp);
	void	stack_reset(field *fp);
	long	start_interval(void);
	void	stop_interval(long tv, char *s);
	int 	tran_finish(short resetflag);
	int 	tran_reset_all(void);
	int 	tran_start(short *tano, short *tino);
	void	check_timers(register byte func);
	void	reset_timers(register byte func);
/*	void	dump_timers(void); see duplicate above */
	struct	RT_TEFIELD * make_mlfield(field *fp, RECT *rp, int addbox);

// --------------------------
//  pcl.c       
// --------------------------
	XDLL bool	pcl(int flags, byte pcltype, byte application, 
						index pclnum, pcldef *method2, rtobj **argv);
	XDLL bool	async_pcl(int flags, byte pcltype, byte application, 
						index pclnum, pcldef *method2, rtobj **argv);
	byte 		*rt_get_dd_ptr(byte *NBP, void **ptr, rtobj **cargs, 
						byte *type, int may_be_indirect);
	byte		*rt_get_obj_nums(byte *NBP, byte *ano, index *ino, 
						rtobj **cargs, byte ddtype);

// --------------------------
//  fldsubs.c   
// --------------------------
	XDLL void	add_field_rec(field *fp);
	XDLL void	clr_fld_list(field **flp);
	XDLL void	clr_fld_explicit(field *fp);
	XDLL void	dup_fld_explicit(field *fp);
	XDLL void	dup_fld_storage(field *fp);
	XDLL void	sav_fld_list(field **flp);
	XDLL void	sav_fld_storage(field *fp);
	XDLL void	undup_fld_list(void);

	void		dup_fld_list(void);
	void		clr_fld_dlist(dlist *dlp);

// --------------------------
//  supover.c   
// --------------------------
	void	supervisor_override(char *tosystem, short demo);

// --------------------------
//  singlestep.c
// --------------------------
	void 	singlestep(byte *base, byte *ip, byte pcltype, rtobj **cargs);
	void 	show_OFB_source(register byte *base, register byte *stmtptr);
	int  	getkey(void);
	void    checkwatch(byte *ip, byte pcltype);
	void    set_watch_field(short numchars, field *wfield, void *data,
                              byte watchtype, char *watchvalue);

// --------------------------
//  ramalloc.c  
// --------------------------
	BOOL	ramalloc(void);

// --------------------------
//  mouse.c     
// --------------------------
	XDLL void	drawTEfield(dlist *dlptr);
	XDLL void	alter_button(rtcontrol *crp, short what, field *fp);
	XDLL void	drawpushbutton(dlist *dlptr);

	void		rt_mouse_events(int message, int keyflags, int x, int y);
	void		rt_plot_icon(RECT *where, void *obj);
	int 		rt_add_title(short inside, RECT *where, char *title);
	void		drawcheckbox(dlist *dlptr);
	void		drawradiobutton(dlist *dlptr);
	void		drawreliefbutton(dlist *dlptr);
	void		drawiconbutton(dlist *dlptr);
	void		drawvoidbutton(dlist *dlptr);
	void		redrawcontrol(dlist *dlptr);
	void		drawTEtext(dlist *dlptr);
	void		render_control(register rtcontrol *crp, register dlist *dlptr,
						int flag);
	void 		mouse_scroll(HWND hwnd, short pc);
	void 		allcontrols(int onoff);
	void 		checkTEstorage(void);
	void 		do_alter_ctrl (unsigned long tag, int fc, int bc, int dfc, 
						int dbc, int attr, int down, int hidden, int disabled);
	void 		calc_control_size(rtcontrol *crp, short font, short face, 
						short size, char *text, short *width, short *height);
	int 		redraw_ctrl(unsigned long tag);
	void 		inform_TE_change(field *fp);

// --------------------------
//  menus.c     
// --------------------------
	int 		menu(byte mtype, byte ano, index ino, char *mname,
						char *startstr, field *fp);
	int 		pmenu(byte ano, unsigned short ino, char *mname);
	XDLL int	rt_listexec(int argc, rtobj *argv[]);
	
// --------------------------
//  errorbox.c  
// --------------------------
	XDLL void	rterrmsg(short which, short x, short y, short width,
						short height, short boxtype, short font,
						short face, short size,
						short forecolor, short backcolor,
						short keyaction, char *line1);
	/* VARARGS 1 */
	XDLL void	rterrorbox(char *proto, ...);
	XDLL void	unshow_fyi(void);
	XDLL void	rterrmsg_via_list(short which, struct COMPLIST *lp,
								char *text, short keyflag);

	XDLL void	unshow_err(void);

// --------------------------
//  manual.c    
// --------------------------
	XDLL int	manual(int argc, rtobj *argv[]);

	int 		man_init_fields(int argc, rtobj *argv[]);

// --------------------------
//  localprint.c
// --------------------------
	void	localprint(void);

// --------------------------
//  expr.c      
// --------------------------
	int 	determinetype(byte *t, rtobj **cargs);
	byte	*dobinop(byte op, byte *left, byte *right, rtobj **cargs);
	byte	*douniop(byte op, byte *left, rtobj **cargs);
	int 	mystrcmp(char *s, char *t);

// --------------------------
//  execprt.c
// --------------------------
	XDLL int print_package(int flags, byte obj, byte doc_applno,
			       index doc_itemno, char *logicaldev,
			       char *prtprompt, int (*prtfunc)(), int (*dmfunc)(), docdef *pdoc);
	XDLL void printformatting(void *op, short objtype, const char *xep, short expwidth, 
							  byte justedit, char *resultbuf, int bufsize); 
	int 	PrtDrawBitmap (byte ano, index x, HDC hdc, int left, int top,int right, int bottom);
	int 	PrtDrawIcon (byte ano, index x, HDC hdc, int left, int top, int right, int bottom);
	int 	PrtGetBitmapSize(byte ano, index x, SIZE *psize);

	XDLL void rt_nop_verb_msg(char *);
	int  	addprtbuff(short ctype, short cmnd, short dtype, short dlen,
						byte *data);
	int  	exec_print(byte **nbp, int flags, rtobj **cargs);
	int  	printit(int flag);
	int  	printer_open(char *logicalprinter, char *documentname);
	void  	printer_close(void);
	int  	rt_signal(int id, int rpar);
	int  	safe_sex(int flags);
	int  	wnread(short track, char *path, short len, short allowkeys,
							int timeoutval, field *fp);
	int  	wnwrite(short track, char *path, short len, char *str);
#ifdef _WINGDI_
	void	SelectPrintFont(LOGFONT *lp);
#endif
	HDC 	GetPrintDC(void);
	void	stopprinttimer(void);
	int 	GetSizeFromWidthHeight(int width, int height, int slop);
	SIZE	GetWidthHeightFromSize(int page_size);

// --------------------------
//  demo.c      
// --------------------------
	int 	demo_mode(char *);

// --------------------------
//  datacoll.c  
// --------------------------
	XDLL void 	drawfield(dlist *dlptr);

	int  		datacollect(short startfield, short errortype);
	char 		*makefieldtext(dlist *dlptr);
	void  		setfont(dlist *dlptr);
	int  		dovalidation(field *fp, dlist *dcfield);
	int  		do_vals(dlist *dlp);
	void 		field_focus(short, slist *, dlist *);
	void  		copy_pick(field *fp, dlist *dcfield);
	byte		fld_boxtype(field *fp);
	int 		doduringentry(register field *fp,register int ch);
	bool		dc_set_te_text(dlist *dlp, field *fp,
					void *tr, short flags, RECT *bounds, short redraw);
	field		*fp_from_dlp(dlist *);

// --------------------------
//  rtrawkey.c  
// --------------------------
	bool	check_reset(void);
	XDLL void	key_event(int keycode);
	int		key_fold(register int ch);
	int		rawkey(int wid, bool rflag);
	void	sync_write(void);
	void	control_mouse_events(int);
	void	remove_watchcursor_completely(void);
	void	set_delayed_watchcursor(void);
	void	setcursor(void);
	void	clear_mouse_messages(void);
	void	clear_key_messages(void);
	void	flopcursor(void);
	void	undo_watchcursor(void);
	void	do_watchcursor(void);


// --------------------------
//  execdde.c  
// --------------------------
	int 	exec_dde(byte *bp, rtobj **cargs);
	
// --------------------------
//  exec_cds.c  
// --------------------------
	int 	exec_cds(int flags, byte *bp, pcldef *cp, byte pcltype, 
						rtobj **cargs);
	int 	parse_arg(void);
	
// --------------------------
//  help.c      
// --------------------------
	int 	help(short default_action);

// --------------------------
//  execdraw.c 
// --------------------------
	int 	exec_draw(byte **nbp, int flags, rtobj **cargs);
	void 	rt_boundrect(dlist *dlp, short xp, short yp, short len);

// --------------------------
//  exec_comm.c 
// --------------------------
	int 		exec_comm(byte *bp, rtobj **cargs);
	static int	comm_stuff_field(char *from, byte datatype, int length, 
								field *fp);
	byte		*comm_get_pcl_ptr(byte *bp, pcldef **pcl_ptr, rtobj **cargs); 

// --------------------------
//  calculator.c
// --------------------------
	int 	calculator(short x, short y, short border, short calc_mode,
						field *tapefield, int tapeload, int tapesave);
	
// --------------------------
//  cal.c       
// --------------------------
	int 	cal(void);
	int 	cal_opt(int x, int y, int cal_border);
	void	cal_reverse(dlist *cursor);

// --------------------------
//  rtwindow.c  
// --------------------------
	XDLL int	fieldfromscrap(field *fp);
	XDLL int	fieldtoscrap(field *fp);
	XDLL void	rtwdw_resize(short width, short height);
	XDLL int	do_PCLredraw(void);
	XDLL HDC	getdc(void);
	XDLL void	releasedc(void);
	XDLL long	get_RTs_wndprocaddr(void);
	XDLL void	ToolsInitColor(void);
	XDLL void	setdc(HDC hdc);

 	void		GetPersonality(char *ininame);
	void 		rt_clearwindow(void);
	int 		openwindow(char *windowname, int flag);
	int 		winnt_keycode(int vkey, int flags);
	int 		SetRGBColor(short idx, short red, short green, short blue,
							short realize);
	void 		timerproc(WPARAM uParam, LPARAM lParam);
	void 		Bell(void);
	void 		scrap_add_cr(char *buff, int maxlen);
	void 		scrap_remove_cr(char *buff);
	HANDLE 		CreateRTWindowDIB();
	HANDLE 		DisplayRTWindowDIB (HANDLE hDIB, char *szWndTitle);
	LRESULT CALLBACK CopyVideoWndProc(HWND hChildWnd, UINT message,
             					   WPARAM wParam, LPARAM lParam);
	bool		ctrl_is_active(rtcontrol *crp);
	void		ControlProc(dlist *ctldlp, UINT message, WPARAM uParam, 
							LPARAM lParam);


// --------------------------
//  rtfonts.c   
// --------------------------
#ifdef _WINGDI_
	XDLL void	getfont(TEXTMETRIC *newtm);
#endif

	void		getunscaledfont(TEXTMETRIC *newtm);
	void 		InitFontStuff(void);
	void 		TermFonts(void);
	HFONT 		selectfont(void);
	HFONT		fontfromcache(void);
	long 		scaletext(long width, int type);
	void 		XlateText(BYTE *is, BYTE *os);
	short 		rt_drawtext(short x, short y, BYTE *str, short len);

// --------------------------
//  windoman.c  
// --------------------------
	XDLL long	StringLen(char *string);
	XDLL void	ShowControl(HWND hwnd);
	XDLL void	HideControl(HWND hwnd);
	XDLL void	HiliteControl(HWND hwnd, int setting);
	XDLL int	GetCtlValue(HWND hwnd);
	XDLL void	TextFont(short fontno);
	XDLL void	TextFace(short faceno);
	XDLL void	TextSize(short size);
	XDLL void	TextMode(short mode);
	XDLL void	ForeColor(short forecolor);
	XDLL void	BackColor(short backcolor);

	int 		LocalTextWidth(int font,int face,int size,char *string,int len);
	int 		LocalCharWidth(int font, int face, int size, char chr);
	int 		StringWidth(char *str);
	int 		LocalStringWidth(int font, int face, int size, char *string);
	long		LocalTextWidHit(short font, short face, short size,
								char *str, short len, short where);
	void		BlockMove(void *source, void *dest, unsigned int nbytes);
	unsigned long HiWord(long number);
	unsigned long LoWord(long number);
	void		ClipRectSect(RECT *rect);
	void		SetClip(HRGN region);
	HRGN		LocalGetClip(void);
	void		ClipRect(RECT *rect);
	void		DrawControl(HWND hwnd);
	void		ValidRect(RECT *theRect);
	void		SetWTitle(HWND hwnd, char *newtitle);
	void		PenMode(int mode);
	void		PenSize(short width, short height);
	void		PenPat(short penpat);
	void		PenNormal(void);
	void		HideControlNoInval(HWND hwnd);
	void		SetCtlValue(HWND hwnd, int setting);
	void		InvalRect(RECT *rp);
	int		SectRect(RECT *src1, RECT *src2, RECT *dstRect);
	void		EraseRect(RECT *rp);
	void		ScrollRect(RECT *rect, short dh, short dv, int update);
	int			IsCtlActive(int cid);

// --------------------------
// rt/winmenu.c
// --------------------------
	byte	*winmenu(byte *bp, rtobj **cargs);
	int 	winmenu_keyaction(int keycode);
	int 	winmenu_menu_key(int key);
	void	winmenu_action(long uparam, byte *bp);

// --------------------------
// global custom csr routinew		
// --------------------------
	void	do_winmenu(long);

// --------------------------
// execcds functions
// --------------------------
	int 	transform_cdserror(int error);

// --------------------------
//  misc functions 
// --------------------------
	XDLL int	pcldecompile(int pass, byte *buffer, byte *address[]);
	XDLL int 	tool_decomp_expr(byte *passed_ip, char *resultbuff);

	int  		val_string(byte type);
	int  		val_int(byte value);
	int 		pdmstmt(byte *,char **,short);
	int 		depdm(byte *,int);
//	void 		cmstats_print(const void *);
	int 		exec_crt(byte *bp, rtobj **cargs);
	char 		*getflag_name(byte *dp, short fbv);
	char		*xlate_filename(char *fname);	// exported function
	bool		required_files();				// exported function

	int 		init_fwdref_objects();
	void		*get_fwdref(byte object);
	void		free_fwdref_objects();

	XDLL char 	*dayname(int day);
	XDLL char 	*monthname(int month);
	XDLL char	*getSubString(register char *buff);
	XDLL char	*getSubStrStart(register char *buff);
	XDLL int 	getSubStrLen(register char *buff);

// --------------------------
//  execdll.c  
// --------------------------
	int 		exec_dll(byte *bp, rtobj **cargs);
	int 		exec_declare(byte *bp, rtobj **cargs);


// --------------------------
//  control.c  
// --------------------------
	void	redrawn_by_Windows(dlist *);
	LRESULT CALLBACK combobox_msg(HWND,UINT,WPARAM,LPARAM);

// --------------------------
//  errorbox.c  
// --------------------------
	int 		msgbox(byte *, rtobj **cargs);
	XDLL int	ofb_msgbox(HWND, char *, char *, int);

// --------------------------
//  winmenu.c  
// --------------------------
	void		delete_pdmenu(pdmnode *pdmx);
	pdmnode		*append_pdm(byte *bp,byte ano,index ino,int position,
						long menutype,HMENU	hmenu,rtobj **cargs);
	void		delete_pdm(pdmnode *pdmx);
	pdmnode		*PDM_find_by_hpdmenu(HMENU hmenu);
	HMENU		set_hmenu(HMENU hmenu);
	void		pdm_init();

// --------------------------
//  nls/nls.c  
// --------------------------
	HANDLE		NLSInit(LPTSTR resdllname,BOOL append);
	LPTSTR		moaLoadString(WORD resID, LPTSTR lpBuffer, int nSize);
	LRESULT 	CALLBACK IME_WndProc(HWND, UINT, WPARAM, LPARAM);

#endif

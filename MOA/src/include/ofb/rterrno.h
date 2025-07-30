//********************************************************************
//                                                                    
//  Symbolic error codes, used in the RunTime code.  The error     
//  messages themselves are stored in "/usr/lib/pplus/error_####". 
//                                                                    
//********************************************************************

#ifndef RTERRNO_H
#define RTERRNO_H

							 // general error messages in 0-99	
#define	ERR_INFO		0	 // error severity levels used in	
#define	ERR_WARN		1	 // error routine					
#define	ERR_FATAL		2


#define	MISSINGOPERAND	200	 // PCL proc related errors 
#define	TOOMANYOPERANDS 201	 // Too many operands for this stmt 
#define	BADPCLVERB		202	 // Unrecognized PCL verb 
#define	UNIMPLEMENTED	203	 // Unimplemented PCL statement 
#define	BADBYTEVAL		204	 // Operand value out of range (0-255) 
#define	NOCOMPILE 		205	 // PCL proc compiled with error 
#define BADTIMEVAL		206	 // Bad TIME value 
#define BADRETVAL		207	 // Bad RETURN value 
#define	MISSINGWIDTH	208	 // PRINT statement has CENTER & no width arg 
#define	BADSTRINGDEF	209	 // Syntax error in STRING definition 
#define ILLEGALX  		210	 // video pixel out of range (0-639) 
#define ILLEGALY  		211	 // video pixel out of range (0-479) 
#define BADATTRIBUTE	212	 // bad attribute (not 0-255) 
#define PCLSTACKOFLOW	213	 // local variable stack overflow 
#define LOCALBUFOFLOW	214	 // local variable buffer overflow 
#define LOCALNOOPEN		215	 // ADD of local variable attempted 
							 // without prior call to OPEN locals 
#define LOCALSNOBUFF	216	 // can't malloc storage for local variables 
#define	PCLBADCCON		217	 // illegal character constant 
#define	PCLBADSCON		218	 // illegal string constant 
#define	PCLBADXCON		219	 // illegal hex constant 
#define	PCLBADICON		220	 // illegal decimal constant 
#define	PCL2MANYLABELS	221	 // too many labels in PCL proc 
#define	PCLUNDEFLABEL	222	 // undefined PCL label 
#define	PCLMULTDEFLAB	223	 // multiply defined label in PCL proc 
#define PCLUNDEFOPND	224	 // undefined operand (not field or DD object) 
#define PCLMOVETARGET	225	 // illegal target for MOVE instr 
#define BADASSIGNOP		226	 // = found in expression, should be == 
#define PCLBADON		227	 // missing target statement in ON stmt 
#define PCL2FEWPAR		228	 // too few right parentheses 
#define PCL2MANYPAR		229	 // too many right parentheses 
#define PCLBADEXPR      230	 // badly formed expression 
#define PCLIFSYNTAX		231	 // syntax error in IF stmt 
#define PCLBADOP		232	 // Illegal operator in expression 
#define UNKNOWNPCL		233	 // undefined PCL proc 
#define PCLOVERFLOW		234	 // PCL printer buffer overflow 
#define BADFONT			235	 // Bad font 
#define BADBOXTYPE		236	 // Bad boxtype 
#define BADDCSCREEN		237	 // Bad datacollect screen 
#define PCLPRTERROR		238	 // Error writing to PRINTER device 
#define PCLBADLET		239	 // Badly formed LET statement 
#define SLISTFAILURE	240	 // Can't create screenlist (out of RAM?) 
#define UNDEFLOCAL		241	 // Undefined local variable 
#define JNLBUFFOFLOW	242	 // Journal buffer overflow 
#define BADOBJECT   	243	 // Incorrect object type (bad operand) 
#define UNDEFOBJECT   	244	 // Undefined object 
#define BADCSR          245	 // Undefined C subroutine reference 
#define BADLETTARGET	246	 // Bad left hand side of LET statement 
#define PCLPRTOPEN      247	 // Can't open printer device 
#define CURRTRANERROR	248	 // CURRENTTRAN without .NAME suffix 
#define BADVALOBJECT	249	 // bad validation object 
#define DUPVALOBJECT	250	 // duplicate validation object 
#define BADTESTFLAG		251	 // badly formed TESTFLAG expression 
#define BADFLAGBITNAME	252	 // bad flagfield or bitflag name 
#define BADSUBSTR		253	 // bad format on SUBSTR 
#define BADSUBSTROBJ	254	 // illegal SUBSTR target 
#define BADSUBSTROFFSET	255	 // illegal SUBSTR offset 
#define BADSUBSTRLEN	256	 // illegal SUBSTR length 
#define NOCURRENTFIELD	257	 // No current field defined 
#define BADPRINTKEYWORD 258	 // bad keyword on PRINT stmt 
#define BADPRINTSTMT    259	 // extra operand or missing EDIT 
#define ILLEGALEDIT     260	 // Illegal EDIT on non-field 
#define BADPRINTEDIT	261	 // Illegal EDIT object type 
#define BADFUNCKEYNAME	262	 // bad function key name in KEYDEF 
#define BADSUBSTRFLD  	263	 // Field in SUBSTR is not ASCII type 
#define FILLER03        264	 // 
#define BADENDTRAN		265	 // ENDTRAN without preceeding STARTTRAN 
#define PCLBADFCON		266	 // Illegal floating point constant 
#define BADMOVESOURCE	267	 // Illegal source on MOVE stmt 
#define BADMOVESTMT  	268	 // Illegal MOVE (inconsistant FLD types?) 
#define ZEROLENFIELD	269	 // field has zero length 
#define WRONGOPERANDS  	270	 // wrong number of operands 
#define BADCOMMPTR     	271	 // unknown comm pointer 
#define BADCOMMSTMT    	272	 // bad syntax in comm stmt 
#define COMMPTRREF		273	 // bad comm pointer name 
#define MISSINGENDIF	274	 // un-ELSE'd or un-ENDIF'd IF 
#define BADCOMMHDR		275	 // bad COMMHDR function call 
#define BADLENGTH		276	 // bad LENGTH value in COMM stmt 
#define BADCOMMSTATUS	277	 // bad status operand in COMM statement 
#define BADCOMMSENSE	278	 // bad sense data in COMM statement 
#define BADRESOURCE 	279	 // bad resource name in COMM statement 
#define BADPUTTARGET	280	 // bad target for PUT statement 
#define PCLTOOLONG  	281	 // source statement is too long 
#define BADSPYNAME		282	 // invalid node-name in SPY statement 
#define MISSINGPROC		283	 // Missing or invalid PROC statement 
#define BADSUFFIX  		284	 // Invalid .suffix on fieldname 
#define MISSINGCOMMA	285	 // Missing comma in SUBSTR expression 
#define MISSINGENDLOOP	286	 // Missing ENDLOOP 
#define NOTDECIPOINT  	287	 // Operand must be decipoint (*999) 
#define BADENDLOOP    	288	 // ENDLOOP encountered without matching LOOP 
#define INCOMPLETEIF  	289	 // LOOP contains incomplete IF statement 
#define INCOMPLETELOOP 	290	 // IF contains incomplete LOOP statement 
#define BADCONTINUE    	291	 // CONTINUE encountered outside LOOP 
#define IFNESTLEVEL    	292	 // Too many nested IF statements 
#define BADELSE        	293	 // ELSE/ENDIF/ELSEIF statement w/no matching IF
#define NODEFINE       	294	 // Referenced but undefined PCL proc 
#define BADAUX1			295	 // Open failure trying to get auxprt node 
#define BADAUX2			296	 // Read failure trying to get auxprt node 
#define BADINDREF		297	 // FIELD is not of type IND (indirect) 
#define BADDRAWFLAG		298	 // cannot DRAWFIELD on a flags field 
#define CHARTINITIALIZED 299 // Previously initialized chart is not completed 
// ==========================================================================
#define	ISDECIPOINT   	300	 // Operand cannot be given in decipoint 
#define	BADFROMTO     	301	 // Illegal or misplaced FROM or TO keyword 
#define	DUPPROCSTMT   	302	 // Illegal duplicate PROC/... stmt 
#define	BADSIGNATURE  	303	 // Illegal signature file name 
#define BADICONREF 		304	 // bad ICON reference 
#define BADBITREF  		305	 // bad BITMAP reference 
#define BADPRTSTMT		306	 // illegal PRT statement 
#define BADBYTE   		307	 // Illegal byte value (must be 0-255) 
#define USEINUSE  		308	 // Illegal USE within USE'd proc 
#define TOOMANYARGS		309	 // too many replacement args in DPRINT/FPRINT 
#define TOOFEWARGS		310	 // too few replacement args in DPRINT/FPRINT 
#define BADKEYACTION	311	 // bad KEYACTION value (not 0-3) 
#define BADINIT     	312	 // undefined INIT value (1st operand) 
#define DUPDYNVALOBJ	313	 // 2nd attempt to attach dynamic val object 
#define BADDISPLAYFIELD	314	 // Illegal FIELD type for DRAW FIELD 
#define BADNUMERIC     	315	 // Illegal (non-numeric) operand value 
#define NOTASCIITYPE   	316	 // Field must be of type ASCII 
#define MOVECONVERSION 	317	 // Conversion error during MOVE statement 
#define SWITCHNESTLEVEL	318	 // Too many nested SWITCH statements 
#define PCLBADSWITCH   	319	 // Syntax error in SWITCH statement 
#define MISSINGENDSWITCH 320 // Missing ENDSWITCH statement 
#define BADENDSWITCH   	321	 // ENDSWITCH encountered w/o matching SWITCH 
#define BADBREAK       	322	 // BREAK encountered outside LOOP or SWITCH 
#define BADFIELDTYPE   	323	 // Field type not allowed in this context 
#define BADCASEARG     	324	 // Bad argument on CASE statement 
#define BADSUBFOLLOW   	325	 // Illegal character following subscript 
#define SYNTAXINNAME   	326	 // bad syntax in DD object name reference 
#define BADSUBSCRIPT   	327	 // Illegal subscript reference 
#define MISSINGNAME    	328	 // 3-char object type not followed by object 
							 // name 
#define BADDISPFLD      329	 // Illegal field type for display 
#define NONFIELDARRAY   330	 // Non-FIELD object cannot be an array 
#define NOTARRAYFIELD   331	 // Subscripted FIELD not defined as an array 
#define NOTNORMALFIELD  332	 // Array FIELD referenced without subscripts 
#define SUBSCRIPTS1NOT2 333	 // Illegal 2nd subscript on singly dimensioned field 
#define SUBSCRIPTS2NOT1 334	 // Missing 2nd subscript on doubly dimensioned field 
#define BADSTRINGOP     335	 // Illegal operator for STRING arguments 
#define BADINTOP        336	 // Illegal operator for INTEGER arguments 
#define BADFIXEDOP      337	 // Illegal operator for FIXED arguments 
#define BADFLAGLST		338	 // badly formed flagbit definition LIST 
#define RT_TNLEVEL_HIGH 339	 // too many ENDTRANs or RESET TRANs 
#define RT_TNLEVEL_LOW  340	 // missing ENDTRAN or RESET TRAN 
#define BADRECORDFLDTYPE  341 // field type must be bulk 
#define BADRECORDOPEN   342	 // unable to open file "SAMPLE.SND" 
#define BADRECORDREAD   343	 // unable to read file "SAMPLE.SND" 
#define BADDDESTMT		344	 // error decompiling DDE stmt 
#define BADKEYWORD		345	 // error compiling DLL stmt 
#define	NEGDCPT			346	 // Illegal negative decipoints
#define	BADFWDREF		347	 // Illegal forward referenced object
#define	SCON_TOOLONG	348	 // Illegal string constant, exceeds 255 chars
#define	OFBTRACE    	349	 // %s trace %s:%s %c

#define MISSINGSUB 		350	 // Missing or invalid SUB statement 
#define MOADIRDNE		351	 // MOADIR not defined or directory DNE
#define PROJDIRUND		352	 // MOAPROJ not defined
#define PROJDIRDNE		353	 // MOAPROJ project directory DNE
#define MOAPROJTOOLONG	354	 // MOA project path name too long
#define MOAKEYUND		355	 // MOA register key not found

#define MISPLACEDLOCAL	370	 // LOCALs must appear before any executable stmts
#define LOCAL_BAD_GROUP	371	 // Illegal group number for LOCALs (internal)
#define LOCAL_MALLOC_FAILED	372	 // Malloc failed to obtain storage for LOCALs
#define LOCAL_NO_LIST	373	 // No LOCALs allocated for group %d
#define LOCAL_BAD_ITEMNO	374	 // Attempt to reference undefined LOCAL item number %d
#define MISSINGLIKE		375	 // Missing or misplaced LIKE keyword in LOCAL statement
#define TOOMANYLOCALS	376	 // Too many LOCAL variables in SUBROUTINE
#define	BADLOCALNAME	377  // Redefinition of formal parameter
#define	BADLOCALNAME2	378  // Redefinition of local field

#define NOSOURCEPROC	380	 // No Get_Src_Func procedure defined in pclcompile()

#define	BADFIELDNAME	390	 // FIELD related error group 
#define	BADPICKLISTNUM	391	 // bad picklist header ref in FIELD 
#define	TOOMANYFIELDS 	392	 // too many FIELDs in this transaction 
#define	BADFIELDREF   	393	 // Illegal FIELD reference 
#define	BADSETTIMER   	394	 // Illegal SETTIMER function 
#define	BADSTTIMEVAL   	395	 // Illegal TIMEOFDAY value 
#define	BADSECVAL    	396	 // Illegal IDLE/INTERVAL value 
#define	BADDRAWSTMT    	397	 // Illegal DRAW statement type 
#define	BADJUSTIFY    	398	 // BAD justification arg for RELIEFBUTTON 
#define	BADTAG	    	399	 // BAD TAG arg 
// ==========================================================================
#define BADDSCNAME		400	 // DESCRIPTOR error messages group 
#define BADPATNUM		401	 // Undefined edit pattern number 
#define MISSINGLIST		402	 // Missing LIST statement 
#define INVALIDLIST		403	 // LIST contains other than MEMBERs 
#define	VERBNOTPCL		404	 // verb not allowed in PCL compile	
#define	VERBNOTDOC		405	 // verb not allowed in DOC compile	
#define	VERBNOTSCR		406	 // verb not allowed in SCR compile	
#define	VERBNOTLST		407	 // verb not allowed in LST compile	
#define	MISSINGDOC		408	 // Missing DOCUMENT statement		
#define	MISSINGSCREEN	409	 // Missing SCREEN statement			
#define BADSCREEN		410	 // Bad screen reference 
#define BADLISTOBJ		411	 // Bad object in MEMBER of LIST 
#define	VERBNOTOFB		412	 // verb not allowed in OFB compile	

#define INCOMPLETEWHILE 413	 // Missing or misplaced END WHILE statement
#define WHILENESTLEVEL  414	 // Too many nested WHILE statements
#define WHILESYNTAX     415	 // Syntax error in WHILE statement
#define BADWHILE        416	 // END WHILE without matching WHILE statement
#define BADARGLIST      417	 // Missing or invalid argument list
#define IFINWHILE       418	 // Incomplete IF statement within WHILE statement
#define DOINWHILE       419	 // Incomplete DO statement within WHILE statement
#define MISSINGENDWHILE 420	 // Missing END WHILE statement
#define OFBRTNTOOBIG	421	 // Compiled OFB RTN exceeds 32K
// ==========================================================================
#define	BADMENUNAME		500	 // Undefined menu name in PCL proc 
#define BADMENUINDEX	501	 // Undefined menu index in PCL proc 
#define NOMENUITEMS		502	 // Menu encountered with no items 
#define BADLISTPTR		503	 // NULL list pointer in Menu HEADER 
#define MISSINGMSI		504	 // missing menu selection item 
#define BADPICKLIST		505	 // bad picklist reference 
#define	NOTITLEINHEADER 506	 // No title found when appl # and item given 
#define	BADMENUTYPE		507	 // Invalid menu type; must be 'P' or 'T' 
#define	EDIT_INVALID	508	 // edit pattern contains inval chars for field
#define	EDIT_TOOSHORT	509	 // edit pattern not long enough for field  
#define	EDIT_NO_SIGN	510	 // neg value can't be displayed with 
							 // non-negative edit	
#define EDIT_BAD_TYPE	511	 // bad field type for the given edit pattern 
#define EDIT_BAD_DATE	512	 // invalid date can't be edited	
#define	EDIT_DOUBLE_DEC	513	 // pattern has multiple DecimalInsertion chars
#define	LS_FAIL_ARGTYPE	514	 // Parameter must be a list or indirect field 
#define	LS_NOLIST		515	 // Parameter must be a list 
#define	LS_NOIND		516	 // Parameter must be a indirect field 
#define	LS_NOPARMLIST	517	 // 1st element of 1st member must be exec list
#define	LS_NOPCL		518	 // 1st element of 1st member must be exec rtn 
#define	LS_MISMATCH		519	 // Exec & parm lists must have same # members
#define	LS_FIELDERR		520	 // Exec list MEMBERs must begin with field 
#define	LS_NOFIELD		521	 // Parameter field not in data dictionary 
#define	LS_BADDATA		522	 // PCL token points to incorrect data type 
#define	LS_BADARGTYPE	523	 // Invalid data for indirect field 
#define	LS_BADARGDATA	524	 // Invalid data in argument list 

#define	NOMENUFLAGS     525	 // Missing FLD:common:menu_flags 
#define	NOMENU2MENU     526	 // menu selection ref's undefined menu 
#define	NOMENUBODY      527	 // menu selection ref's undefined menu body 
#define	NOKEYFILTERINHEADER 528 // No key_filter found in picheader 
// ==========================================================================
#define	APPLNOTFOUND		600 // application couldn't be located in file	
#define	BADAPPLNAME			601 // application name null or doesn't exist	
#define	BADAPPLNO			602 // application number <min or >max app #	
#define	BADCSRNO			603 // CSR number incorrect						
#define	BADDICTNO			604 // dict number <min or >max dict no			
#define	BADITEMNAME			605 // Item name null, too long, or nonexistant 
#define	BADITEMNO			606 // Item number <min or >max item no.		
#define	BADITEMPTR			607 // Item pointer null or incorrect			
#define	BTDATAOPENERROR		608 // Can't find Data Dictionary for this proj 
#define	BTIDXINITERROR		609 // Can't initialize btree index file		
#define	BTIDXOPENERROR		610 // Can't open btree index file				
#define	BTINITERROR			611 // Can't initialize btree					
#define	BTREEERROR			612 // Error calling cbtree						
#define	BTSEEKVERROR		613 // btseekv error							
#define	BTSIZEERROR			614 // btsize incorrect							
#define	CSRNOTFOUND			615 // CSR couldn't be recalled					
#define	DISKREADERROR		616 // Error reading disk file					
#define	DISKSEEKERROR		617 // Error during disk file seek				
#define	DISKWRITEERROR		618 // Error writing disk file					
#define	FREEFAILED			619 // Error releasing memory buffer			
#define	INSERTFAILED		620 // Couldn't insert btree record				
#define	ITEMNOTDELETED		621 // Couldn't delete item from btree			
#define	ITEMNOTFOUND		622 // Couldn't find requested item				
#define	ITEMNOTINFILE		623 // Couldn't recall requested item			
#define	ITEMNOTPRESENT		624 // Item not present							
#define	MALLOCFAILED		625 // Error allocating memory buffer			
#define	NODISKADDRESS		626 // Item's disk address not correct			
#define	NOLISTFORHEADER		627 // No data list as specified by header		
#define	NOSPACEFORITEM		628 // No additional items may be added			
#define	SELECTFAILED		629 // Couldn't recall record from btree		
#define	ZEROLENGTH			630 // Length can't be zero						
#define	BUFFERNOTVALID		631 // Null or incorrect buffer pointer			
#define	DICTALREADYOPEN		632 // Attempted dictopen when dict already open
#define	PUTNOTALLOWED		633 // Attempted ddput when not in MAINTAIN mode
#define	APPLALREADYEXISTS	634 // Can't add application that already exists
#define	CANTOPENSYSCFIG		635 // Can't open the system configuration file	
#define	SYSCFIGREADERROR	636 // Error reading system configuration file	
#define	SYSCFIGWRITEERROR	637 // Error writing system configuration file	
#define	BADPROJECTNAME		638 // Project name too long or empty			
#define	BADREVLEVEL			639 // Rev level too long or empty				
#define	CANTFINDCFIGREC		640 // Can't recall requested configuration rec	
#define	CANTCHANGEDIR		641 // Can't change to data dictionary dir		
#define	CANTLOADCONFIG		642 // Can't load system configuration file		
#define	MAINTNOTALLOWED		643 // Maintenance not allowed on this project	
#define	CANTGETCWD			644 // Can't get name of current working dir	
#define	CANTREADSYSCFIG		645 // Can't read system config file			
#define	OBSOLETESYSCFIG		646 // Incompatible system configuration file	
#define	CANTCREATEQUICKFILE	647 // The quick item file couldn't be opened	
#define	OBSOLETEDICTIONARY	648 // Incompatible data dictionary				
#define	DICTLOCKED			649 // dictionary locked by another user		
#define	ILLEGALNAME			650 // name isn't legal							
#define	ERRPOPEN			651 // Error opening pipe						
#define	ERRPWRITE			652 // Error writing to pipe					
#define	ITEMALREADYEXISTS	653 // Attempted rename to an existing name		
#define	BADDDHANDLE			654 // invalid dictionary handle				
#define	APPLDELETED			655 // Application has been deleted				
#define	ITEMDELETED			656 // object has been deleted					
#define XREFBADKEY          657 // XREF key pointer is null                 
#define XRNOPOINTER         658 // XREF return buffer pointer is null       
#define XRBTREADTYPE        659 // XREF read type not GETFRST or GETNXT     
#define XREFALREADYOPEN     660 // Attempted xropen when XREF already open  
#define XRPUTNOTALLOWED     661 // Attempted XREF write not in MAINTAIN mode
#define XREFLOCKED          662 // The XREF is locked by another user       
#define	BADREGEXPR			663 // invalid regular expression for getlist	
#define DDLISTACTIVE		664 // LISTINIT attempt when list already active
#define DDLISTNOTACTIVE		665 // LISTCONT or LISTSTOP attempted when no	
							 // list is active							
#define	DDNOTANARRAY		666 // ddaget on an non-array field				
#define	DDBADSUBSCRIPT		667 // row/col request for col only array		
							 // or col only request for col/row array	
#define	DDARRAYOUTOFRANGE	668 // row or column too high					
#define DDCBTREEVERSION		669 // file's CBTREE version bad 
#define DDREVMSG                670 // Dictionary revision =%d 
#define DDSWREVMSG              671 // Software revision =%d 
#define DDSIGNALFAIL            672
#define DDUNEXPECTED_SIGNAL     673
#define DDCOMPLETING_UPDATE     674
#define DDCLEANINGUP            675
#define DDCFIGREAD              676
#define DDCFIGUSING             677
#define DDCFIG_INCOMPATABLE     678
#define DDLOCKED                679
#define DDQCK_INCOMPATIBLE      680
#define DDITEM_DELETED          681
#define DDBADARRAYFLD1          682
#define DDBADARRAYFLD2          683
#define INVALIDWORKINGDIR		684 // working directory not valid	
#define CSRDLLNOTLOADED         685
#define CSRDLLNOPROCADDRESS     686
#define DDUSERSTOP			    687
// ==========================================================================
#define OUTOFMEMORY			700 // Additional memory not available 			
#define	MEMOVERWRITTEN		701 // Allocated buffer has been overwritten	
#define UNDEFAPPL			702 // Undefined application name 

// CRT related error messages 
#define ERRCRTSTMT    	703 // bad syntax in crt stmt 
#define ERRCRTROW    	704 // bad row in crt stmt 
#define ERRCRTCOL    	705 // bad column in crt stmt 
#define ERRCRTLEN    	706 // bad length in crt stmt 
#define ERRCRTTLEFT    	707 // bad left trim in crt stmt 
#define ERRCRTTRIGHT   	708 // bad right trim in crt stmt 
#define ERRCRTRC    	709 // bad row and column in crt stmt 
#define ERRCRTFILL    	710 // bad fill character in crt stmt 
#define ERRCRTATTR    	711 // bad attribute definition in crt stmt 
#define ERRCRTOPRND    	712 // bad operand for function in crt stmt 
#define ERRCRTNLEN    	713 // length not allowed in crt stmt 
#define ERRCRTKEY    	714 // invlaid key definition in crt stmt 
#define ERRNOCOMM   	715 // no host communications installed 
#define ERRNOTCRTRES   	716 // current resource not a crt type resource 
#define ERRINVALDEST   	717 // invalid destination for crt get stmt 
#define ERRCRTTOUT   	718 // bad trim out in crt stmt 

// COMM related error messages 
#define ERRCOMTLEFT		730 // bad left trim in comm statement 
#define ERRCOMTRIGHT	731 // bad right trim in comm statement 
#define ERRCOMPAT		732 // bad binary pattern in comm statement 
#define ERRCOMBADTYPE	733 // invalid COMM data type for field type 
#define ERRCOMBADFLDT	734 // invalid field type in COMM GET or PUT 
#define ERRCOMARG4TYPE  735 // invalid argument for data type in COMM stmt 
#define ERRCOMBADARGS   736 // invalid argument combination in COMM stmt 
#define ERRCOMUTYPE     737 // invalid USER_TYPE in COMM stmt 
#define ERRNOLENGTH		738 // length required in COMM GET with non ASCII fld 
#define ERRCOMTOUT		739 // bad trim out in comm statement 

// 750 - 799 reserved for match errors 
#define ERRMINMATCH		750 // used in catastrophe.c, not really an error 
#define ERR_REG_NO_PREV_REGEXP		750
#define ERR_REG_BAD_ESC_CHAR		751
#define ERR_REG_MISSING_SQBRKT		752
#define ERR_REG_TOO_MANY_PARENS		753
#define ERR_REG_NULL_PAT_PARENS		754
#define ERR_REG_UNMATCH_PARENS		755
#define ERR_REG_NULL_PAT_WORD		756
#define ERR_REG_EMPTY_CLOSURE		757
#define ERR_REG_ILLEGAL_CLOSURE		758
#define ERR_REG_CYCLICAL_REF		759
#define ERR_REG_UNDETERM_REF		760
#define ERR_REG_ILLEGAL_BIT_NUM		761
#define ERR_REG_MISS_EQ_IN_BTST		762
#define ERR_REG_ILLEGAL_BIN_NUM		763
#define ERR_BAD_REG_EXPR_DFA		764
#define ERRMAXMATCH		799 // used in catastrophe.c, not really an error 

// 800 - 899 reserverd for cds client errors 

// 900 - 969 reserverd for run-time cds verb errors 

// 970-999 compiler error messages for cds 


#define	RTM_NOINITONLY	1600 // missing PCL:common:initialization_only 
#define	RTM_NOSTARTING	1601 // missing PCL:common:starting_point      
#define	RTM_NORESETKEY	1602 // missing PCL:common:reset_key           

#define RTM_NOSPYSELF	1610 // spy_on(%s) failed, can't spy on one's self 
#define RTM_SPYFAILED	1611 // spy_on(%s) failed, errno=%d 
#define RTM_SPYOFFFAILED 1612 // spy_off(%s) failed, errno=%d 

// ----------------------------------------------------------------
// OFB (Olivetti Financial BASIC) error messages in range 3000-3199
// ----------------------------------------------------------------

#define BADOFBVERSION	3001 // Illegal OFB version ID

// CSR error messages in range 5000 - 5199 

#define	CSR_EXPECT_FLD				5000
#define	CSR_EXPECT_IND				5001
#define	CSR_EXPECT_LSTIND			5002
#define	CSR_NO_LST					5003
#define	CSR_BAD_CONTEXT				5004
#define	CSR_BAD_FLAGFLD				5005
#define	CSR_BAD_INT_OBJ				5006
#define	CSR_BAD_TOTAL_LIST_HDR		5007
#define	CSR_BAD_TL_MEMBER			5008
#define	CSR_BAD_TL_APPEND			5009

// -------------------------------------------------- 
// RT runtime ERROR message text in range 7000 - 7999 
// -------------------------------------------------- 

#define	ERR_MAG_TRK_SEL_FAILED            7000  // Mag track select failed 
#define	ERR_WNREAD_IO_FAILED              7001  // WNREAD I/O failed 
#define	ERR_MAG_WRITE_FAILED              7002  // mag write failed 
#define	ERR_OPEN_FAILED                   7003  // open failed to device 
#define	ERR_AUX_OPEN_FAILED               7004  // aux open failed 
#define	ERR_NO_LOGICAL_DEVICE             7005  // no logical dev assignment
#define	ERR_PRTR_WRITE_FAILED             7006  // printer write failed 

#define	WINMENU_ERROR			          7007
#define	UNABLE_TO_ACCESS_WINHELP          7008

// ------------------------------------------------ 
// RT FYI runtime message text in range 8000 - 8999 
// ------------------------------------------------ 

#define	FYI_AUX_PRINT                     8000  // aux print prompt 
#define	FYI_PRINT_TERMINATED_BY_OPERATOR  8001 
#define	FYI_DEVICE_BUSY                   8002 

// ------------------------------------------------ 
// RT DDE runtime message text in range 9000 - 9199 
// ------------------------------------------------ 

#define	DDE_1                     	9000
#define DDE_BAD_CONVHANDLE          9001
#define DDE_NO_CALLBACKFUNC         9002
#define DDE_MEM_ALLOC_FAIL          9003
#define DDE_STRING_HANDLE_FAIL      9004
#define DDE_UNKNOWN_CONVHANDLE      9005
#define DDE_EXCEED_MAX_SERV_TOPICS  9006
#define DDE_QUERYSTRING_FAILED		9007	//added LCC 1/17/96

// ------------------------------------------------ 
// RT DDE runtime message text in range 9200 - 9299 
// ------------------------------------------------ 

#define DLL_FUNC_NOTDECLARED		9200
#define DLL_INVALID_NUM_OF_PARAM	9201
#define DLL_INVALID_FIELD			9202
#define DLL_DIFFERENT_DATA_TYPE		9203
#define DLL_FAILED_TO_LOAD_LIB		9204
#define DLL_FAILED_TO_GET_ADDRESS	9205
#define DLL_INVALID_VARARGS         9206
#define DLL_FUNC_ALREADY_DECLARED	9207
#endif

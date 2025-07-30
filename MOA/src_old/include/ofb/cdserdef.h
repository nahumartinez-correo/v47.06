#ifndef CDSERDEF_H
#define CDSERDEF_H
/* error code definitions */

#define CDS_OK                  800
#define CDS_SCAN_CONTINUE       800
#define CDS_RECORD_NOT_FOUND  801 /* Find did not get rec this time   */
#define CDS_CANCELED_BY_RESET 802 /* canceled by user intervention    */
#define CDS_LOCK_FAILURE      803 /* Lock held by another process       */

#define CDS_INSERT_FAILURE      804
#define CDS_BAD_LOG_ADDRESS     805
#define CDS_FUTURE_TIME			806   
#define CDS_GET_FAILURE			807
#define CDS_AGG_BAD_TYPE		808		/* invalid type for aggregate */
#define CDS_CONVERT_FAILURE		809
#define CDS_OVERFLOW			810
#define CDS_CONTEXT_RESET	811	/* current position invalid */
#define CDS_FINDLAST_FAILURE    812
#define CDS_PARTNER_ACTIVE      813
#define CDS_MAXIMUM_TABLES		814
#define CDS_BAD_VALUE			815
#define CDS_ROLLED_BACK         816    /* Rolled back during 2 phase commit */
#define CDS_SYNTAX_ERROR        817    /* Criteria rules contained a syntax*/
                                       /* error.                           */
#define CDS_BIT_AND             818    /* tried bit and on non int type */
#define CDS_NO_TABLE          819 /* named table not found */
#define CDS_NO_SUCH_FILE  CDS_NO_TABLE /* The specified file does not exist*/
                                       /* in the CDS control tables.       */
#define CDS_FILE_OPEN_FAILURE   820    /* Could not open specified file    */
#define CDS_KEY_BUFFER_INVALID  821
#define CDS_SEEK_ERROR          822    /* Could not seek to record location*/
#define CDS_REQUEST_ERROR		823    /* client side only */
#define CDS_MEMORY_FAILURE      824    /* Memory allocation failure        */

#define CDS_CLIENT_SET_UP_TWICE 825        /* only happens durring debugging */
#define CDS_CLIENT_INCOMPATIBLE 826        /* incompatible version numbers */


#define CDS_NOT_YET             827        /* function not ready yet */

#define CDS_IN_PHASE_2_COMMIT	828		/* busy in second phase */
#define CDS_UPDATEFLD_NOT_FOUND 829     /* Could not find field to update  */
#define CDS_DELETE_FAILURE      830
#define CDS_CONTEXT_HELD        831
#define CDS_INVALID_KEY         832
#define CDS_GETKEY_FAILURE      833
#define CDS_NOT_LOCKED          834
#define CDS_WRITE_ERROR         835
#define CDS_DUPLICATE_KEY     	836
#define CDS_KEY_BUFFER_TYPE     837
#define CDS_KEY_FIELD_TOO_LONG  838
#define CDS_BAD_RESPONSE_DT		839			/* bad data type for projection response */
#define CDS_OP_NOT_ALLOWED  	840        /* operation not allowed on this table */
#define CDS_BAD_FIELD_CONTEXT_NAME 841	/* bad context name in criteria */
#define CDS_JOIN_NOT_INDEXED	842		/* left side field of join not indexed */
#define CDS_BAD_NUMERIC_VALUE   843     /* non-digits in numeric field */
#define CDS_SHADOW_LOG_OLD		844     /* partner's log is out of date */
#define CDS_NO_CRITERIA_FIELDS  845        /* criteria doesn't mention a field */
#define CDS_CONNECTION_FAILED   846        /* Failed to connect to service */
#define CDS_KEY_FIELD_2_FIELD   847        /* criteria key field compare to fld */
#define CDS_CONVERSION          848        /* criteria field conversion error */
#define CDS_NO_SCAN_START       849        /* must start scan with findfirst or findlast */
#define CDS_CANT_CHANGE_KEY     850        /* can't change key when delete not allowed */
#define CDS_CANNOT_READ         851        /* can't read requested record */
#define CDS_EOF_ERROR           852        /* can't read size requested (eof encountered) */
#define CDS_CONTEXT_NAME_USED   853        /* context name in use with another table */
#define CDS_BAD_ORDER           854        /* incorrect order by spec */
#define CDS_UPDATEFLD_TYPE      855     /* buffer field type != rec field type */
#define CDS_BAD_COMPARE         856     /* buffer field type not numveric  */
#define CDS_ZERO_KEY_LENGTH     857     /* buffer key field has length zero  */
#define CDS_TIME_OUT            858     /* EREQAB from partner  */
#define CDS_NULL_CONTEXT_PTR    859     /* context is nil ptr  */
#define CDS_BAD_CONTEXT_PTR     860     /* context ptr not valid */
#define CDS_BAD_APPL_NAME	    861     /* application name not valid */
#define CDS_BAD_FIELD_NAME	    862     /* application:field name not valid */
#define CDS_BAD_DDGET_FIELD		863     /* application:field name not valid */
#define CDS_BAD_TABLE_NAME		864		/* null table name or too long */
#define CDS_TABLE_EXISTS      	865 /* duplicate table name on create table */
#define CDS_DEREF               866     /* application:field not in record */
#define CDS_BAD_INDEX_LIST		867		/* null or too short */
#define CDS_BAD_PARAMETER		868		/* bad parameter passed */
#define CDS_LINK_ERROR          869     /* rename failed */
#define CDS_UNLINK_ERROR        870     /* file unlink (remove) failed */
#define CDS_NOT_ALLOWED         871     /* direct update of catalog not OK */
#define CDS_ALREADY_IN_LUW      872     /* Can't start 2nd LUW without intervening commit or rollback */
#define CDS_NOT_IN_LUW          873     /* Can't commit what is not begun */
#define CDS_END_OF_FILE         874     /* Obvious                          */
#define CDS_BAD_LUW_FLAG		875
#define CDS_BAD_LOCK_LEVEL      876
#define CDS_ONLY_ONE_SCAN       877		/* only one scan at a time allowed */
#define	CDS_INDEX_EXISTS		878		/* named index already exists on table */
#define	CDS_MAX_INDEXES			879		/* Max number of indexes exceeded */
#define CDS_NAME_TOO_LONG		880		/* name longer than allowed */
#define CDS_NULL_PARMS			881		/* parm must not be null */
#define	CDS_BAD_INDEX_NAME		882		/* named index does not exist */
#define	CDS_KEY_TOO_BIG			883		/* key declaration is too large */
#define	CDS_TOO_MANY_KEYFIELDS	884		/* key has too many fields */
#define	CDS_REPEATED_KEY_FIELD	885		/* field occurs multiple times in key */
#define CDS_FLD_DD_INCOMPATIBLE 886		/* field dictionary at client not same as at server */
#define CDS_NO_LOG_RESET_IN_LUW 887		/* can't reset log during and LUW, username parm */
#define CDS_SHADOW_WAITING      888		/* client side msg only */
#define CDS_SHADOW_VERIFY       889		/* primary side msg only. prime list length, shadow list length */
#define CDS_ECHO_SERV_OPEN      890		/* primary side can't open echo server*/
#define CDS_BE_PRIMARY          891		/* primary side not up to date*/
#define CDS_MUST_SYNC           892		/* after one failure, primary side has to sync with partner before serving other clients*/
#define CDS_NET_ACCESS          893		/* Can't see other machine */
#define CDS_BAD_OPEN            894		/* Can't open file */
#define CDS_NO_SHADOW			895     /* Can't access shadow directory */
#define CDS_BAD_SHADOW_NAME		896     /* name does't match current shadow */
#define CDS_SWITCHED_ROLES		897
#define CDS_REPLICATED			898		/* files copied to shadow machine */
#define CDS_LOG_READ  			899		/* can't read log at displacement */


/* 900-969 error defines for run-time cds verb set */
#define BADMEMBERFORMAT		900	/* Invalid member format */
#define	BADFONTINDEX		901	/* Invalid font idx for multi-record display */
#define BADCTNAME			902	/* Bad table or context name */
#define	BADANDBITFIELD		903	/* Incorrect field for "and bit" operation */
#define	BADMULTILINECNT		904	/* Too many records for multi-record display */
#define	CONTEXTEXISTS		905	/* Select context exists for another table */
#define	NORECORD			906	/* No record in CDS buffer */
#define	NOTSUPPORTED		907	/* Verb or verb feature not supported */
#define	BADELEMENTCOUNT		908	/* Incorrect number of elements */
#define	BADLYFORMEDVERB		909	/* invalid format for compiled verb */
#define	NAMETOOLONG			910	/* table or context name too long */
#define	BADJOINTABLELIST	911	/* no join tables defined */
#define	BADASTYPE			912	/* AS field type not same as source field */
#define	NOBUFFSIZE			913	/* buffer size not specified */
#define	BADARRAYASFLD		914	/* Array AS field with different element # */
#define	CONTEXTNOTFOUND		915	/* Select context does not exist */
#define	COUNTTOOBIG			916	/* multi-record index too large */
#define	BADTYPE				917	/* invalid object type */
#define	POSINTONLY			918	/* positive integers only */
#define	NOFIELD				919	/* field does not exist in DD */
#define	NOSELECT			920	/* no select context defined */
#define	ERRORPCL			921	/* error calling pcl for multi-display */
#define	NOCSR				922	/* csr fn does not exist */
#define	DISPLAYTOOLONG		923	/* formatted display line too long */
#define	BADEXPR				924	/* badly formed expression for array fld idx */
#define	NOHEADER			925	/* No header found */
#define	NOCDSLOCKRETRY		926	/* No CDSLOCKRETRY descriptor */
#define	BADCDSLOCKRETRY		927	/* Cannot convert string to integer */
#define	NOCDSLOCKSLEEP		928	/* No CDSLOCKSLEEP descriptor */
#define	BADCDSLOCKSLEEP		929	/* Cannot convert string to integer */
#define	NOTMULTIRECBUF		930	/* Not a multi-record record buffer */
#define	NOLIST				931	/* List not found in data dictionary */
#define NORTCONTEXT			932 /* No RT table context defined */
#define	BADITEMCOUNT		933	/* Bad item count based on window length/font */
#define BUFFOFLOW			934	/* table context buffer overflow */
#define	NOCSRSTRINGFLD		935	/* No common:csr_string field defined */
#define CDS_TABLE_LOCK_FAIL	936
#define CDS_TBL_UNLOCK_FAIL 937
#define CDS_FAIL_COMPRESS   938
#define	BADTITLESTR   		939	/* Bad title in multidisplay */
#define	BADFINDBUF          940 /* Find buffer no longer valid */
#define	BADFNCODE   		941	/* Invalid function code for cds wait context */
#define	NOTNW		   		942	/* No-wait cds fn not issued for context */
#define	NOITEMIDX			943	/* Field item referenced but not defined */	
#define	TBLNAMETOOLONG		944	/* Table name is too long */
#define	CNTXTNAMETOOLONG	945	/* Context name is too long */
#define	CDS_NOT_LINKED		946 /* CDS not included in this version of rt */
#define	IDXNAMETOOLONG		947	/* Alternate index name is too long */
#define	NO_LOST_CDS_CONNECT_PCL	948	/* Can't find req'd PCL routine */
									/* common:lost_cds_connection */
#define	MORE_RECORDS		949	/* Find_n returned fewer records than */
								/* requested because the LAN buffer was full */
#define	BADSRVRNAME			950	/* Invalid server name */
#define	BADARRAYFIELD		951	/* Not array field or array size not equal */
#define	BADINTOTYPE			952	/* INTO field type not same as record field */
#define	ARRAYFLDNOTINRECORD   953 /* Array field not in record */
#define BADARRAYFLDDIMENSION  954 /* Bad array element dimension for rec fld */
#define MEMBERNOTARRAYFLD     955 /* Asked for array fld but found element */
#define	FLDNOTINRECORD        956 /* Field not in record or array fld larger */
                                  /* then record array field */
#define	RECORDHASMOREELEMENTS 957 /* Record array field has more elements */
                                  /* then reocrd array field */
#define	RECORDFIELDLARGER     958 /* Record field data longer than RT field */

/* 970-999 compiler error messages for cds */
#define BADCDSSTMT		970		/* bad syntax in cds stmt */
#define BADCDSKEYWORD	971		/* cds keyword not recognized	*/
#define BADCDSFILENAME	972		/* invalid type for cds file name	*/
#define	BADPROJECTION	973		/* invalid projection list */
#define	BADCDSCONTEXTNAME	974	/* context name not valid			*/
#define	BADCDSSORT		975		/* bad keyword where sort direction expected*/
#define EXPECTINGFIELD	976		/* wrong data type where field expected	*/
#define POSITIVEONLY	977		/* Negative numbers not allowed	*/
#define	BADCDSLUW		978		/* LUW not usable						*/
#define	BADCDSTABLENAME	979		/* unusable data type for table name	*/
#define	BADCDSWHERE		980		/* Invalid where clause source type */
#define	BADINFOFIELD	981		/* Invalid type for lockholder info field */
#define	BADCDSIDXNAME	982		/* Table index name not valid			*/
#define	BADCDSWAIT		983		/* NOWAIT parm not usable				*/
#define	BADCDSRESET		984		/* NORESET parm not usable				*/
#define	BADCDSPARM		985		/* Invalid parm or wrong number ov parms */
#define BADINTOAS		986		/* Expecting keyword: INTO or AS */
#define BADSERVERNAME	987		/* bad data type for server name object */
#define BADAGGREGATEFN	988		/* bad aggregate function declaration */
#define	BADCDSALL		989		/* invalid use of all keyword */
#define	MISSINGFROM		990		/* Expecting FROM keyword or bad proj list */ 
#define	BADPRESERVE		991		/* preserve must be assigned to first table */
								/* in join list                             */
#define	BADALIASNAME	992		/* expecting alias name to follow as keyword */ 
#define	BADARRAYASFIELD	993		/* ARRAY keyword missing or used */
								/* inappropriately */
#define	BADARRAYFLDIDX	994		/* Can't reference array element when ARRAY */
								/* keyword is specified */
#define	BADSEGMENTMOD	995		/* Invalid segment modifier */
#define NO_CDS_OOB_PCL  996     /* missing Cds_out_of_band PCL routine */
#define ERR_LOCALFLDAPPEND	997	// attempt to append fld as local fld 
#define	BADPATH			998		// invalid or missing path

#define CDS_NO_SERVER			1800	/* Client is not connected to a server */
#define CDS_1801				1801
#define CDS_1802				1802
#define CDS_1803				1803
#define CDS_1804				1804
#define CDS_NO_CONNECTION		1805
#define CDS_1806				1806
#define CDS_1807				1807
#define CDS_1808				1808
#define CDS_1809				1809
#define CDS_CANT_PACK			1810
#define CDS_BUFFER_OVERLAP		1811
#define CDS_1812				1812
#define CDS_1813				1813
#define CDS_1814				1814
#define CDS_1815				1815
#define CDS_1816				1816
#define CDS_1817				1817
#define CDS_1818				1818
#define CDS_1819				1819
#define CDS_1820				1820
#define CDS_1821				1821
#define CDS_1822				1822
#define CDS_1823				1823
#define CDS_1824				1824
#define CDS_1825				1825
#define CDS_1826				1826
#define CDS_1827				1827
#define CDS_1828				1828
#define CDS_1829				1829
#define CDS_1830				1830
#define CDS_1831				1831
#define CDS_1832				1832
#define CDS_1833				1833
#define CDS_RECIEVE_ERROR		1834
#define CDS_NO_JOIN_DEPENDENCY	1835
#define CDS_NO_INDEPENDENT_CONTEXT 1836
#define CDS_NO_DDL_IN_LUW          1837
#define CDS_AGGREGATE_CONVERSION   1838
#define CDS_NO_MULTISERVER_JOIN   1839
#define CDS_SECOND_SHADOW_CLIENT  1840
#define CDS_PREV_LUW_NOT_COMPLETE 1841
#define CDS_PROJECTION_LIST_SHORT 1842
#define CDS_BAD_UNIQUE_CODE       1843
#define CDS_BAD_DATATYPE          1844
#define CDS_BAD_DIMENSION         1845
#define CDS_DOMAIN_BOUNDARY       1846
#define CDS_BAD_UPDATE_BUFFER     1847
#define CDS_BAD_INDEX_MODIFIER    1848
#define CDS_BAD_SERIAL_MODIFIER   1849
#define CDS_BAD_SERIAL_TYPE       1850
#define CDS_BAD_MINIMUM_SIZE      1851
#define CDS_BUFFER_TOO_BIG        1852
#define CDS_PROJECTION_NOT_UNIQUE 1853
#define CDS_LOST_ECHO             1854
#define CDS_BAD_FORMAT            1855
#define CDS_FOUND_NOT_ARRAY 	  1856	/* found scalar when searching for member or array */
#define CDS_MEMBER_SEARCH_DIMENSION 1857  /* found array has different dimension than search argument */
#define CDS_ASKED_FOR_ARRAY_FOUND_MEMBER 1858 /* asked for whole array, found single element */
#define CDS_SHADOW_TOO_SLOW_AT_RESET 1869
#define CDS_EL_SEEK_ERR 1870
#define CDS_EL_STDERR 1871
#define CDS_EL_CANT_REOPEN_ECHO 1872
#define CDS_CANT_CHDIR 1873
#define CDS_CANT_MKDIR 1874
#define CDS_EL_CANT_OPEN_LOG 1875
#define CDS_EL_DIR_NAME_MISSING 1876
#define CDS_CANT_STAT_DIR 1877
#define CDS_SHARED_MEM_CREATE_ERR 1878
#define CDS_SHARED_MEM_ATTACH_ERR 1879
#define CDS_PIPE_WRITE_ERR 1880
#define CDS_PIPE_READ_ERR 1881
#define CDS_LSEEK_ERR 1882
#define CDS_MSG_TOO_SMALL 1883
#define CDS_BAD_RL_ACTION 1884
#define CDS_BAD_SEEK_ADDR 1885
#define CDS_ALONE 1886
#define CDS_DICTIONARY_CLOSED 1887
#define CDS_BUFFER_EMPTY 1888
#define CDS_WRITE_LOG_PIPE 1889
#define CDS_READ_LOG_PIPE 1890
#define CDS_BAD_LOG_FILE_SIZE 1891
#define CDS_CANT_WRITE_LOG 1892
#define CDS_FLUSH_FAILED 1893
#define CDS_LOG_COPY_FAILED 1894
#define CDS_OUT_OF_BAND 1895
/* 1896 is used */
#define CDS_PARTNER_TIME_OUT 1897
#define CDS_SIGNAL_FAILED 1898
#define CDS_CHECK_SUM_MISMATCH 1899
#define CDS_BAD_PIPE_REPLY 1900
#define CDS_CANT_OPEN_RDONLY 1901
#define CDS_CANT_READ_LAST_ECHO 1902
#define CDS_ECHO_LOG_SHORTER 1903
#define CDS_ECHO_DIFFERS 1904
#define CDS_SHADOW_SHUTTING_DOWN 1905
#define CDS_NOT_RESILIENT 1906
#define CDS_RL_OPEN_WRITE 1907
#define CDS_RL_OPEN_READ  1908
#define CDS_ECHO_DIFFERS_BY 1909
#define CDS_ECHO_PRIME_FAILED 1910
#define CDS_BAD_ECHO_BATCH 1911
#define CDS_ECHO_TOO_LONG  1912
#define CDS_ALREADY_THERE  1913
#define CDS_BAD_RUN_MODE   1914
#define CDS_NEGOTIATING    1915
#define CDS_USAGE          1916
#endif

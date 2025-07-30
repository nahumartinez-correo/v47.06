/*********************************************************************
*
*  Module :  csmsgdef.h
*
*
*  Notes: All defines for csmsg.h structure elements are contained
*		within this header file.
*		These defines are isolated from the structure defines
*		to allow for ease of use in the P+ 'rt' environemnt.
*
**********************************************************************/


/*eject*/

#define SRVR_LOCATE		0x0001	/* used for finding srvr in IP */
#define SRVR_LOCATION	0x0002	/* rsp to SRVR locate */

#define CS_VERSION   102         /* Version of PPCS msgs support   */
                                  /* 0100 = 1.00 Initial Version    */

#define CSL_NAME     25           /* ASCII Resource Name Length    */
#define CSL_NDNM      9           /* utsname nodename Length       */

#define PPCOMMON	"PPcommon"		/* used for configuration */
/*eject*/
/*********************************************************************
*  CSS_HDR common msg header element defines
*********************************************************************/

/*  element:  CSS_HDR.mq */
#define FLAG_M		0x1			/* Netman More Flag       (X.25) */
#define FLAG_Q		0x2			/* Netman Qualified Flag  (X.25) */

/*  element:  CSS_HDR.fc  */
#define CS_ALLOCATE		'Z'		/* ALLOCATE */
#define CS_DEALLOCATE	'Y'		/* DEALLOCATE */
#define CS_DATA			'X'		/* DATA */
#define CS_ERROR		'W'		/* ERROR */
#define CS_CONFIRM		'V'		/* CONFIRM */
#define CS_STATUS		'U'		/* STATUS */
#define CS_ESTATUS		'T'		/* EXTENDED STATUS */
#define CS_COMM			'S'		/* COMM */
#define CS_SRVR			'R'		/* SRVR */
#define CS_IDENTIFY		'Q'		/* IDENTIFY */

/* these defines match waht is in commdef.h do not change */
#define CS_CRTDATA		'a'		/* used for crt to appl */
#define CS_CRTEXIT		'b'		
#define CS_CRTHELP		'c'
#define CS_CRTAPI		'd'		
#define CS_CRTENQ		'e'		

/*********************************************************************
*  CSS_HDRE msg header extension element defines
*********************************************************************/

/*  element:  CSS_HDRE.req_rsp  */
#define CS_REQ			'A'		/* Request */
#define CS_RSP			'B'		/* Response */


/*eject*/
/*********************************************************************
*  CSS_ALLOCATE structure element defines
*********************************************************************/

/*  element:  CSS_ALLOCATE.class  */
#define CS_CL_MSW		0x0001	/* PPLUS to PPLUS messages */
#define CS_CL_HOST		0x0002
#define CS_CL_CFG		0x0004	/* Configuration changes */
#define CS_CL_NBMSGS	0x0008	/* Internal Netboss to Netboss messages */
#define CS_CL_LALERTS	0x0010	/* Network Management Local alerts */
#define CS_CL_GALERTS	0x0020	/* Network Management Gateway alerts */
#define CS_CL_NMREQ		0x0040	/* Network Management requests */
#define CS_CL_SNAFUHA	0x0080	/* Host appl violates protocol */
#define CS_CL_FUIP		0x0100	/* FU'd IP client */

/*eject*/
/*********************************************************************
*  CSS_DATA structure element defines
*********************************************************************/

/*  element:  CSS_DATA.category */
#define CS_N_DATA		'A'		/* normal data */
#define CS_ALCUNSO		'B'		/* ALC UNSO data (ALC ONLY) */
#define CS_SSCP			'C'		/* SSCP message  (SNA ONLY) */
#define CS_PPLUS_DATA	'D'		/* PPLUS message switch msg */
#define CS_BRDCST		'E'		/* PPLUS Broadcast */
#define	CS_P8_DATA		'F'		/* p8 data */
#define CS_SD_DATA		'G'		/* Serial Device Data */
#define CS_RAW_DATA		'R'		/* used for forced raw send no header */
/*  element:  CSS_DATA.format  */
#define CS_UNFORMATTED	'A'		/* Unformatted data */
#define CS_FMH			'B'		/* SNA Function Managment Header */

/* element:  CSS_DATA.msg_pos */
#define CS_FIRST		'A'		/* first msg */
#define CS_MIDDLE		'B'		/* middle msg */
#define CS_LAST			'C'		/* last msg */
#define CS_ONLY			'D'		/* only msg */

/* element:  CSS_DATA.brkt */
#define CS_BB			CS_FIRST	/* Begin bracket */
#define CS_IB			CS_MIDDLE	/* In Bracket */
#define CS_EB			CS_LAST		/* End Bracket */
#define CS_BE			CS_ONLY		/* Begin and End Bracket */
#define CS_NO			'E'			/* No BRACKETS */


/*eject*/
/*********************************************************************
*  CSS_ERROR msg structure element defines                           *
*********************************************************************/

/* element:  CSS_ERROR.req  */
	/* same as CSS_HDR.fc defines */ 

/*eject*/
/*********************************************************************
*  IDENTIFY msg structure element defines
*********************************************************************/

/*  element:  CSS_IDENTIFY_RSP.srvr_class  */
	/* same as ESTATUS rsctype defines */

/*  element:  CSS_IDENTIFY_RSP.resil_type  */
#define CS_ACT_SRV		'A'		/* ACTIVE Server */
#define CS_BKU_SRV		'B'		/* BACKUP Server */
#define CS_NON			'C'		/* NON-Resilient Server */

/*  element:  CSS_IDENTIFY_RSP.resil_cfg  */
#define CS_PRI			'A'		/* PRIMARY Server */
#define CS_AUX			'B'		/* AUXILIARY Server */
#define CS_NON			'C'		/* NON-Resilient Server */

/*eject*/
/*********************************************************************
*  ESTATUS & STATUS msg structure element defines                              *
*********************************************************************/

/*  element:  req  */
/* #define CS_DEALLOCATE	'Y'	/* DEALLOCATE */
#define CS_UPDATE		'Q'		/* Unsolicited STATUS UPDATE */

/*  element: sesstat  */
#define CS_DOWN			'A'		/* Down */
								/* SNA: LU not active */
								/* Non-SNA: No Poll for grp/addr */
#define CS_UP			'B'		/* Up */
								/* SNA: LU active and bound */
								/* Non-SNA: Poll for grp/addr */
#define CS_SSCP			'C'		/* Control Point Active */
								/* SNA: LU active and not bound  */
								/* Non-SNA:  NOT APPLICABLE */
#define CS_ACTIVE		'D'		/* No Poll for assigned net addr */
								/* Non-SNA: No Poll for net addr */
								/* SNA: *** NOT APPLICABLE *******/
#define	CS_CSDN			'E'	    /* Comm Srvr down Local status only */
#define CS_CRNA	'F'		/* host Resource pool empty */

/*  element:  lnkstat  */
	/* CS_DOWN, CS_UP, CS_ACTIVE, CS_CSDN from sesstat */

/*  element:  CSS_ESTATUS.rsctype */
#define CS_ACI		0x40	/* P+ ACI Server */
#define CS_SRV		0x41	/* Server */
#define CS_CLNT		0x42	/* Client */
#define CS_CNM		0x43	/* Comm Network Management */
#define CS_SNA		0x44	/* SNA Comm */
#define CS_X25		0x45	/* X.25 Comm */
#define CS_BSC		0x46	/* Bisync Comm */
#define CS_BUR		0x47	/* Burroughs Comm */
#define CS_UNI		0x48	/* Uniscope Comm */
#define CS_NCR		0x49	/* NCR Comm */
#define CS_ALC		0x4A	/* Airlines Line Control */
#define CS_SDS		0x4B	/* Serial Desvice Server */
#define CS_MSW		0x4C	/* Message Switch Server */
#define CS_CMN		0x4E	/* Common resource requesti */
#define CS_P8		0x4F	/* P8 for ACI compatibility */
#define CS_PPLUS	0x50	/* P+ for ACI compatibility */
#define CS_NBS		0x51	/* NetBoss Server */
#define CS_LINE		0x52	/* SNA LINE resource */
#define CS_PU		0x53	/* SNA PU resource */
#define CS_LU		0x54	/* SNA LU resource */
#define CS_SES		0x55	/* SNA Session resource */
#define CS_POOL		0x56	/* SNA POOL resource */
#define CS_DTE		0x57	/* X25 DTE resource */
#define CS_TELNET	0x58	/* TELNET session */


/*  element:  CSS_ESTATUS.srvrtype  */
#define CS_PRI		'A'		/* Primary Server */
#define CS_AUX		'B'		/* Auxiliary Server */ 
#define CS_NON		'C'		/* NONRESILIENT Server */ 

/*  element:  CSS_ESTATUS.pooled  */
#define CS_TRUE		1
#define CS_FALSE	0

/*eject*/
/*********************************************************************
*  Common COMM msg structure element defines                         * 
*********************************************************************/

/*  element:  CSS_COMM_RSP.req  */
#define CS_LINK_SWITCH	0x4E41		/* Link Switch */
									/* Client  ==> Comm */
#define CS_LINK_INQ		0x4E42		/* Link Inquiry */
									/* Client  ==> Comm */
#define CS_ACT_COMM		0x4E43		/* Activate comm */
									/* Client  ==> Comm */
#define CS_DEACT_COMM	0x4E44		/* Deactivate comm */
									/* Client  ==> Comm */
#define CS_INQ_COMM		0x4E45		/* Comm status inquiry */
									/* Client  ==> Comm */
#define CS_GET_LINKSTATS 0x4E46		/* get link stats */
#define CS_SET_LINKSTATS 0x4E47		/* set link stats */
#define CS_CANCEL		0x4E48		/* Host Canceled Msg In Progress */
									/* Client <==  Host */
#define CS_STOP_COMM	0x4E49		/* stop comm line */
#define CS_START_COMM	0x4E4A		/* start comm line */
#define CS_DSP_COM		0x4E4D		/* Display Comm Resources */
#define CS_LEED_STATUS	0x4E4E		/* Display Lead Status */
	
/*
   These requests are put in here for use with Z80 NON SNA comm 
   they may be changed or removed as native comm servers are written
   for NON SNA protocols
*/
#define CS_SET_RR		0x4E70		/* Z80 NON SNA comm req */
#define CS_SET_RNR		0x4E71		/* Z80 NON SNA comm req */
#define CS_ENABLE_DEV	0x4E72		/* Z80 NON SNA comm req */
#define CS_DISABLE_DEV	0x4E73		/* Z80 NON SNA comm req */
#define CS_TERM_ADDRESS	0x4E74		/* Z80 NON SNA comm req */

/*  element:  CSS_COMM_RSP.rc  */
#define CS_NO_SUPPORT	'A'		/* Function Not Supported */
#define CS_CHANNEL_A	'B'		/* Link Channel A */
#define CS_CHANNEL_B	'C'		/* Link Channel B */
#define CS_COMM_ACTIVE	'D'		/* Comm is active */
#define CS_COMM_INACTIVE 'E'	/* Comm is inactive */
#define CS_TOGGLELINK	'F'		/* toggle link */
#define	CS_PRILINK		CS_CHANNEL_A	
#define CS_AUXLINK		CS_CHANNEL_B


/*eject*/
/*********************************************************************
*  CNM   COMM msg structure element defines 
*********************************************************************/

/*  element:  CSS_CNMCOMM.req[0] & CSS_CNMCOMM.req[1]  */
#define CS_CNM_RTMCTL	0x4341		/* CNM Response Time Control */
									/* CNM Appl ==> Client     (REQ) */
#define CS_CNM_RTMRD	0x4342		/* CNM Response Time Read Data */
									/* CNM Appl ==> Client     (REQ) */
									/* CNM Appl <== Client     (RSP) */
#define CS_CNM_RTMRDCL	0x4343		/* CNM Response Time Read & Clear*/
									/* CNM Appl ==> Client     (REQ) */
									/* CNM Appl <== Client     (RSP) */
#define CS_CNM_RTMDDATA	0x4344		/* CNM Response Time DEALLOC-DATA*/
									/* CNM Appl <== Client     (REQ) */
#define CS_CNM_RTMODATA	0x4345		/* CNM Response Time OVERFLO-DATA*/
									/* CNM Appl <== Client     (REQ) */
#define CS_CNM_LALERT	0x4346		/* Local alert */
									/* CNM Appl <== Client     (REQ) */
#define CS_CNM_GALERT	0x4347		/* Gateway alert */
									/* CNM Appl <== Client     (REQ) */
#define CS_CNM_UNB_RSP_NOTIFY	0x4348		/* Notify server it's OK to send +RSP to UNBIND */
									/* CNM Appl ==> server     (REQ) */

/*  element:  CSS_CNMCOMM_RTM.measure_type  */
#define RTM_1STCHAR		0x01		/* stop measure on receipt 1st char */
#define RTM_UNLOCK		0x02		/* stop measure on unlock of kybd */
#define RTM_CD_EB		0x03		/* stop measure when CD | EB */
#define RTM_LSTCHAR		0x04		/* stop measure on receipt last char */

/*  element:  CSS_CNMCOMM_RTM.unit */
#define RTM_100MS		0x00		/* 100 millisecond */

/*eject*/
/*********************************************************************
*  SNA   COMM msg structure element defines 
*********************************************************************/

/*  element:  CSS_SNACOMM.req[0] & CSS_SNACOMM.req[1] */
#define CS_SNA_RSHUTD		0x4441		/* SNA RSHUTD */
										/* Client  ==> Host Appl */
#define CS_SNA_CANCEL		0x4442		/* SNA CANCEL */
										/* Client  ==> Host Appl */
										/** See CS_CANCEL also */
#define CS_SNA_LUSTAT		0x4443		/* SNA LUSTAT */
										/* Client <==> Host Appl */
#define CS_SNA_SSCP_LUSTAT	0x4444		/* SNA LUSTAT */
										/* Client <==> Host SSCP */
#define CS_SNA_SIGNAL		0x4445		/* SNA SIG */
										/* Client <==> Host Appl */
#define CS_SNA_BIND_PARMS	0x4446		/* SNA BIND Parameters */
										/* Client <==  Host Appl */
#define CS_SNA_BID			0x4447		/* SNA BID request */
										/* Client <== Host Appl */
#define CS_SNA_CHASE		0x4448		/* SNA CHASE request */
										/* Client <== Host Appl */
#define CS_SNA_QEC			0x4449		/* SNA QEC request */
										/* Client <== Host Appl */
#define CS_SNA_SHUTD		0x444A		/* SNA SHUTD request */
										/* Client <== Host Appl */
#define CS_SNA_RELQ			0x444B		/* SNA RELQ request */
										/* Client <== Host Appl */
#define	CS_SNA_NEG_RSP		0x444C		/* SNA Negative rsp */
										/* Client <== Host Appl */
#define CSL_BIND_RSP		26

/*eject*/
/*********************************************************************
*  X25   COMM msg structure element defines 
*********************************************************************/

/*  element:  CSS_X25COMM.req[0] & CSS_X25COMM.req[1]  */
/* reserved                0x4541  */
#define CS_X25_DTECLOSE		0x4542		/* Close Remote ISC DTE circut */
#define CS_X25_STATION		0x4543		/* Station Id inquiry */

/*********************************************************************
*  ALC   COMM msg structure element defines
*********************************************************************/

#define	CS_ALC_STOP_POLL	 0x4A41		/* stop poll 1980 terminals */
#define	CS_ALC_INSERT_POLL	 0x4A42		/* insert poll list 1980 term */
#define	CS_ALC_SYSNA_MSG	 0x4A43		/* Deliver the SysNA msg */
#define	CS_ALC_NEG_RSP		 0x4A44		/* Nak ALC message */
#define	CS_ALC_SYSNA_REQ	 0x4A45		/* Request the SysNA msg */
#define	CS_ALC_GET_STATS	 0x4A46		/* Get ALC Error Statistics */
#define	CS_ALC_CLR_STATS	 0x4A47		/* Clear ALC Error Statistics */
#define	CS_ALC_GET_CTRS		 0x4A48		/* Get ALC Rcv & Xmt msg counts */
#define	CS_ALC_CLR_CTRS		 0x4A49		/* Clear ALC Rcv & Xmt msg counts */

/*eject*/
/*********************************************************************
*  BSC   COMM msg structure element defines
*********************************************************************/

#define CS_BSC_STATUS		0x4641		/* Bisync Status request */
#define CS_BSC_TEST			0x4642		/* Bisync Test request */


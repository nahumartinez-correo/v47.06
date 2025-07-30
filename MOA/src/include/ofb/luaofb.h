/*

// These are masks to be used to check the RH bits that have come in on the
// message.  Some of these can be set on a SEND comment on each
//
//  RCV means you can look at the flag on messages received from host
//  SEND means you can set this flag for messages sent to host

// These are the bit masks that come into the lua_event OFB routine
// as the Pflags parameter
*/
#define LUA_EXPEDITED_FLOW 0x01000000	/* RCV-SEND send on expedited flow */
#define LUA_SSCP_FLOW 0x02000000		/* RCV-SEND send on SSCP flow */
#define LUA_EMULATOR 0x04000000			/* Session is a CRT emulator */
#define LUA_RRI		0x00800000			/* RCV      request-response indicator */
#define LUA_RUC		0x00600000			/* RCV      RU category */
	#define  LUA_FMD  0x00000000			/* FMD data RU */
	#define  LUA_NC	 0x00200000				/* Network Control RU	*/
	#define  LUA_DFC  0x00400000			/* Data flow control RU */
	#define  LUA_SC   0x00600000			/* Session Control RU */
#define LUA_FI		0x00080000			/* RCV-SEND Format indicator */
#define LUA_SDI		0x00040000			/* RCV      Sense data included */
#define LUA_BCI		0x00020000			/* RCV-SEND Begin Chain */
#define LUA_ECI		0x00010000			/* RCV-SEND End Chain */

#define LUA_DR1I	0x00008000			/* RCV      Definite response type 1 */
#define LUA_DR2I	0x00002000			/* RCV      Defintie response type 2 */
#define LUA_ERI		0x00001000			/* RCV      Exception response */
#define LUA_QRI		0x00000200			/* RCV      Queued response  */
#define LUA_PI		0x00000100			/* RCV      Pacing indicator */

#define LUA_BBI		0x00000080			/* RCV-SEND Begin Bracket */
#define LUA_EBI		0x00000040			/* RCV-SEND End Bracket */
#define LUA_CDI		0x00000020			/* RCV-SEND Change Direction */
#define LUA_CSI		0x00000008			/* RCV-SEND Code Selection */
#define LUA_EDI		0x00000004			/* RCV-SEND Enciphered Data */
#define LUA_PDI		0x00000002			/* RCV-SEND Padded Data  */

/*
// These flags will work for 99% of case on transmit
// They are hard coded to the bit definitons because OFB is not a real language
*/
/*#define LUA_LU_XMIT	(LUA_BCI|LUA_ECI|LUA_CDI|LUA_BBI|LUA_FMD) */
#define LUA_LU_XMIT		0x000300A0
/*#define LUA_SSCP_XMIT	(LUA_BCI|LUA_ECI|LUA_FMD|LUA_SSCP_FLOW) */
#define LUA_SSCP_XMIT	0x02030000


/*****************************************************************************/
/* LUA MESSAGE TYPES                                                         */
/*****************************************************************************/
/*
// These are the defines that will come into the lua_event OFB routine as 
// the Pmsgtype parameter
*/
/* Session Status notifciation messages */
#define LUA_STATUS_UP		0x50		
#define LUA_STATUS_SSCP		0x51
#define LUA_STATUS_DOWN		0x52
#define LUA_STATUS_CSDN		0x53
/* used for NOTIFY mode with ONA 3270 */
#define LUA_MESSAGE_TYPE_CRT_DATA	0x54		
#define LUA_MESSAGE_TYPE_CRT_EXIT	0x55
#define LUA_MESSAGE_TYPE_CRT_HELP	0x56

#ifndef LUA_CH_INCLUDED

#define LUA_MESSAGE_TYPE_LU_DATA          0x01
#define LUA_MESSAGE_TYPE_SSCP_DATA        0x11
#define LUA_MESSAGE_TYPE_RSP              0x02
#define LUA_MESSAGE_TYPE_BID              0xC8
#define LUA_MESSAGE_TYPE_BIND             0x31
#define LUA_MESSAGE_TYPE_BIS              0x70
#define LUA_MESSAGE_TYPE_CANCEL           0x83
#define LUA_MESSAGE_TYPE_CHASE            0x84
#define LUA_MESSAGE_TYPE_LUSTAT_LU        0x04
#define LUA_MESSAGE_TYPE_LUSTAT_SSCP      0x14
#define LUA_MESSAGE_TYPE_QC               0x81
#define LUA_MESSAGE_TYPE_QEC              0x80
#define LUA_MESSAGE_TYPE_RELQ             0x82
#define LUA_MESSAGE_TYPE_RTR              0x05
#define LUA_MESSAGE_TYPE_SBI              0x71
#define LUA_MESSAGE_TYPE_SIGNAL           0xC9
#define LUA_MESSAGE_TYPE_UNBIND			  0x32



/*****************************************************************************/
/* LUA PRIMARY RETURN CODES                                                  */
/*****************************************************************************/
#define LUA_OK                            0x0000
#define LUA_PARAMETER_CHECK               0x0001
#define LUA_STATE_CHECK                   0x0002
#define LUA_SESSION_FAILURE               0x000F
#define LUA_UNSUCCESSFUL                  0x0014
#define LUA_NEGATIVE_RSP                  0x0018
#define LUA_CANCELLED                     0x0021
#define LUA_IN_PROGRESS                   0x0030
#define LUA_STATUS                        0x0040
#define LUA_COMM_SUBSYSTEM_ABENDED        0xF003
#define LUA_COMM_SUBSYSTEM_NOT_LOADED     0xF004
#define LUA_INVALID_VERB_SEGMENT          0xF008
#define LUA_UNEXPECTED_DOS_ERROR          0xF011
#define LUA_STACK_TOO_SMALL               0xF015
#define LUA_INVALID_VERB                  0xFFFF

/*****************************************************************************/
/* LUA SECONDARY RETURN CODES                                                */
/*****************************************************************************/
#define LUA_SEC_RC_OK                     0x00000000
#define LUA_INVALID_LUNAME                0x00000001
#define LUA_BAD_SESSION_ID                0x00000002
#define LUA_DATA_TRUNCATED                0x00000003
#define LUA_BAD_DATA_PTR                  0x00000004
#define LUA_DATA_SEG_LENGTH_ERROR         0x00000005
#define LUA_RESERVED_FIELD_NOT_ZERO       0x00000006
#define LUA_INVALID_POST_HANDLE           0x00000007
#define LUA_PURGED                        0x0000000C
#define LUA_BID_VERB_SEG_ERROR            0x0000000F
#define LUA_NO_PREVIOUS_BID_ENABLED       0x00000010
#define LUA_NO_DATA                       0x00000011
#define LUA_BID_ALREADY_ENABLED           0x00000012
#define LUA_VERB_RECORD_SPANS_SEGMENTS    0x00000013
#define LUA_INVALID_FLOW                  0x00000014
#define LUA_NOT_ACTIVE                    0x00000015
#define LUA_VERB_LENGTH_INVALID           0x00000016
#define LUA_REQUIRED_FIELD_MISSING        0x00000019
#define LUA_READY                         0x00000030
#define LUA_NOT_READY                     0x00000031
#define LUA_INIT_COMPLETE                 0x00000032
#define LUA_SESSION_END_REQUESTED         0x00000033
#define LUA_NO_SLI_SESSION                0x00000034
#define LUA_SESSION_ALREADY_OPEN          0x00000035
#define LUA_INVALID_OPEN_INIT_TYPE        0x00000036
#define LUA_INVALID_OPEN_DATA             0x00000037
#define LUA_UNEXPECTED_SNA_SEQUENCE       0x00000038
#define LUA_NEG_RSP_FROM_BIND_ROUTINE     0x00000039
#define LUA_NEG_RSP_FROM_CRV_ROUTINE      0x0000003A
#define LUA_NEG_RSP_FROM_STSN_ROUTINE     0x0000003B
#define LUA_CRV_ROUTINE_REQUIRED          0x0000003C
#define LUA_STSN_ROUTINE_REQUIRED         0x0000003D
#define LUA_INVALID_OPEN_ROUTINE_TYPE     0x0000003E
#define LUA_MAX_NUMBER_OF_SENDS           0x0000003F
#define LUA_SEND_ON_FLOW_PENDING          0x00000040
#define LUA_INVALID_MESSAGE_TYPE          0x00000041
#define LUA_RECEIVE_ON_FLOW_PENDING       0x00000042
#define LUA_DATA_LENGTH_ERROR             0x00000043
#define LUA_CLOSE_PENDING                 0x00000044
#define LUA_NEGATIVE_RSP_CHASE            0x00000046
#define LUA_NEGATIVE_RSP_SHUTC            0x00000047
#define LUA_NEGATIVE_RSP_RSHUTD           0x00000048
#define LUA_NO_RECEIVE_TO_PURGE           0x0000004A
#define LUA_CANCEL_COMMAND_RECEIVED       0x0000004D
#define LUA_RUI_WRITE_FAILURE             0x0000004E
#define LUA_SLI_BID_PENDING               0x00000051
#define LUA_SLI_PURGE_PENDING             0x00000052
#define LUA_PROCEDURE_ERROR               0x00000053
#define LUA_INVALID_SLI_ENCR_OPTION       0x00000054
#define LUA_RECEIVED_UNBIND               0x00000055
#define LUA_SLI_LOGIC_ERROR               0x0000007F
#define LUA_TERMINATED                    0x00000080
#define LUA_NO_RUI_SESSION                0x00000081
#define LUA_DUPLICATE_RUI_INIT            0x00000082
#define LUA_INVALID_PROCESS               0x00000083
#define LUA_API_MODE_CHANGE               0x00000085
#define LUA_COMMAND_COUNT_ERROR           0x00000087
#define LUA_NO_READ_TO_PURGE              0x00000088
#define LUA_MULTIPLE_WRITE_FLOWS          0x00000089
#define LUA_DUPLICATE_READ_FLOW           0x0000008A
#define LUA_DUPLICATE_WRITE_FLOW          0x0000008B
#define LUA_LINK_NOT_STARTED              0x0000008C
#define LUA_INVALID_ADAPTER               0x0000008D
#define LUA_ENCR_DECR_LOAD_ERROR          0x0000008E
#define LUA_ENCR_DECR_PROC_ERROR          0x0000008F
#define LUA_NEG_NOTIFY_RSP                0x000000BE
#define LUA_RUI_LOGIC_ERROR               0x000000BF
#define LUA_LU_INOPERATIVE                0x000000FF

/*****************************************************************************/
/* THE FOLLOWING SECONDARY RETURN CODES ARE SNA SENSE CODES                  */
/*****************************************************************************/
#define LUA_NON_UNIQ_ID                   0xC0001001
#define LUA_NON_UNIQ_NAU_AD               0xC0001002
#define LUA_INV_NAU_ADDR                  0xC0002001
#define LUA_INV_ADPT_NUM                  0xC0002002

#define LUA_RESOURCE_NOT_AVAILABLE        0x08010000
#define LUA_SESSION_LIMIT_EXCEEDED        0x08050000
#define LUA_SLU_SESSION_LIMIT_EXCEEDED    0x0805000A
#define LUA_MODE_INCONSISTENCY            0x08090000
#define LUA_BRACKET_RACE_ERROR            0x080B0000
#define LUA_INSUFFICIENT_RESOURCES        0x08120000
#define LUA_BB_REJECT_NO_RTR              0x08130000
#define LUA_BB_REJECT_RTR                 0x08140000
#define LUA_RECEIVER_IN_TRANSMIT_MODE     0x081B0000
#define LUA_REQUEST_NOT_EXECUTABLE        0x081C0000
#define LUA_INVALID_SESSION_PARAMETERS    0x08210000
#define LUA_UNIT_OF_WORK_ABORTED          0x08240000
#define LUA_FM_FUNCTION_NOT_SUPPORTED     0x08260000
#define LUA_LU_COMPONENT_DISCONNECTED     0x08310000
#define LUA_INVALID_PARAMETER_FLAGS       0x08330000
#define LUA_INVALID_PARAMETER             0x08350000
#define LUA_NEGOTIABLE_BIND_ERROR         0x08350001
#define LUA_BIND_FM_PROFILE_ERROR         0x08350002
#define LUA_BIND_TS_PROFILE_ERROR         0x08350003
#define LUA_BIND_LU_TYPE_ERROR            0x0835000E
#define LUA_CRYPTOGRAPHY_INOPERATIVE      0x08480000
#define LUA_REQ_RESOURCES_NOT_AVAIL       0x084B0000
#define LUA_SSCP_LU_SESSION_NOT_ACTIVE    0x08570000
#define LUA_SYNC_EVENT_RESPONSE           0x08670000
#define LUA_REC_CORR_TABLE_FULL           0x08780001
#define LUA_SEND_CORR_TABLE_FULL          0x08780002
#define LUA_SESSION_SERVICE_PATH_ERROR    0x087D0000

#define LUA_RU_DATA_ERROR                 0x10010000
#define LUA_RU_LENGTH_ERROR               0x10020000
#define LUA_FUNCTION_NOT_SUPPORTED        0x10030000
#define LUA_HDX_BRACKET_STATE_ERROR       0x10050121
#define LUA_RESPONSE_ALREADY_SENT         0x10050122
#define LUA_EXR_SENSE_INCORRECT           0x10050123
#define LUA_RESPONSE_OUT_OF_ORDER         0x10050124
#define LUA_CHASE_RESPONSE_REQUIRED       0x10050125
#define LUA_CATEGORY_NOT_SUPPORTED        0x10070000

#define LUA_INCORRECT_SEQUENCE_NUMBER     0x20010000
#define LUA_CHAINING_ERROR                0x20020000
#define LUA_BRACKET                       0x20030000
#define LUA_DIRECTION                     0x20040000
#define LUA_DATA_TRAFFIC_RESET            0x20050000
#define LUA_DATA_TRAFFIC_QUIESCED         0x20060000
#define LUA_DATA_TRAFFIC_NOT_RESET        0x20070000
#define LUA_NO_BEGIN_BRACKET              0x20080000
#define LUA_SC_PROTOCOL_VIOLATION         0x20090000
#define LUA_IMMEDIATE_REQ_MODE_ERROR      0x200A0000
#define LUA_QUEUED_RESPONSE_ERROR         0x200B0000
#define LUA_ERP_SYNC_EVENT_ERROR          0x200C0000
#define LUA_RSP_BEFORE_SENDING_REQ        0x200D0000
#define LUA_RSP_CORRELATION_ERROR         0x200E0000
#define LUA_RSP_PROTOCOL_ERROR            0x200F0000

#define LUA_INVALID_SC_OR_NC_RH           0x40010000
#define LUA_BB_NOT_ALLOWED                0x40030000
#define LUA_EB_NOT_ALLOWED                0x40040000
#define LUA_EXCEPTION_RSP_NOT_ALLOWED     0x40060000
#define LUA_DEFINITE_RSP_NOT_ALLOWED      0x40070000
#define LUA_PACING_NOT_SUPPORTED          0x40080000
#define LUA_CD_NOT_ALLOWED                0x40090000
#define LUA_NO_RESPONSE_NOT_ALLOWED       0x400A0000
#define LUA_CHAINING_NOT_SUPPORTED        0x400B0000
#define LUA_BRACKETS_NOT_SUPPORTED        0x400C0000
#define LUA_CD_NOT_SUPPORTED              0x400D0000
#define LUA_INCORRECT_USE_OF_FI           0x400F0000
#define LUA_ALTERNATE_CODE_NOT_SUPPORT    0x40100000
#define LUA_INCORRECT_RU_CATEGORY         0x40110000
#define LUA_INCORRECT_REQUEST_CODE        0x40120000
#define LUA_INCORRECT_SPEC_OF_SDI_RTI     0x40130000
#define LUA_INCORRECT_DR1I_DR2I_ERI       0x40140000
#define LUA_INCORRECT_USE_OF_QRI          0x40150000
#define LUA_INCORRECT_USE_OF_EDI          0x40160000
#define LUA_INCORRECT_USE_OF_PDI          0x40170000

#define LUA_NAU_INOPERATIVE               0x80030000
#define LUA_NO_SESSION                    0x80050000

#endif

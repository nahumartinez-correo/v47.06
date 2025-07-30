
/* THIS FILE CONTAINS DEFINES RELATING TO COEXISTENCE */


/* The following defines correspond to the Pinnacle 8 application */
/* which is to run in coexistence with Pinnacle Plus application. */
/* Some of the information can be found in the BCGEN file.        */

#define P8_AUTHORITY_BYTE		1
#define P8_PASSWORD_LEN			8
#define P8_PKBRNUM_LEN			2
#define P8_PKBRTLRNUM_LEN		3
#define P8_PKSUPNUM_LEN			1
#define P8_PKTLRNUM_LEN			1

#define P8_MAX_PAD_LEN	7		/* p8 messages use the 7 bytes after   */
														/* service request code and before the */
														/* the actual message.                 */

/* The following defines correspond to the Pinnacle 8 service requests */

#define P8_WORKSTATION_MSG		"10"
#define P8_BR_SUPV_MSG				"11"
#define P8_BROADCAST_MSG			"12"
#define P8_COPY_VIDEO					"13"
#define P8_SUPV_OVR_REQ				"15"
#define P8_AUX_PRINT_REQ			"16"
#define P8_WS_CONC_MODE				"1A" 
#define P8_WS_HOST_MODE				"1B" 
#define P8_ALL_CONC_MODE			"1C" 
#define P8_ALL_HOST_MODE			"1D" 
#define P8_RESET_BR_SUPV			"1F" 
#define P8_CONC_REBOOT				"80" 
#define P8_CONC_OPEN					"81" 
#define P8_CONC_CLOSED				"82" 


/* The following defines are the responses to P8 messages */

#define P8_OK					"01"
#define P8_NOTOK			"03"
#define P8_ERR_CODE		"00"

#define P8_NO_MSG_IND		"*"

/* The following defines correspond to the Pinnacle 8 request codes */

#define P8_OPEN_SUP_PASSWD_FILE		"41"
#define P8_SCAN_FILE				"11"
#define P8_CLOSE_FILE				"15"
#define P8_BRSUP_SIGNON				"14"
#define P8_BRSUP_SIGNOFF			"16"
#define P8_OVR_GRANT				"01"
#define P8_OVR_HOLD					"02"
#define P8_OVR_DENY					"03"
#define P8_COPY_VIDEO_PATH			"/usr/users/pplus/bin/p8v"		

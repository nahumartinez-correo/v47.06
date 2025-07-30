/****x* Pendientes de agrupación/DEF:include:keys
* NOMBRE
*   DEF:include:keys
* DESCRIPCION
*   Comportamiento del objeto (obligatorio)
* ENTRADAS
*   Parámetros de entrada de la rutina, sean argumentos o campos que tiene en cuenta
* SALIDAS
*   Campos modificados como resultado o returns cuyos valores sean no triviales
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

/****************************************************************/
/*																*/
/* Header files contain #defines for use by the pre-processor	*/
/* with "pc" and the import utilities.  The #defines have the	*/
/* basic form "#define <name> <value>".  There are three basic	*/
/* categories of header files, distinguished by whether the		*/
/* <name>s and/or <value>s in the file are standard or custom.	*/
/*																*/
/*   file name		use					<name>s		<value>s	*/
/*   ----------		------------------	--------	--------	*/
/*   xxx_std.h		standard defines	standard	standard	*/
/*   xxx_talr.h		tailoring std fcns	standard	custom		*/
/*   xxx_cust.h		custom use			custom		custom		*/
/*																*/
/****************************************************************/

/*--------------------------------------------------------------*/
/* Global Keymapping Tailoring									*/
/*--------------------------------------------------------------*/

/* Function Keys */
#define KEY_SYS_ADMIN		F1
#define KEY_HELP			F1
#define KEY_CRT				F2
#define KEY_CIF				SF2
#define KEY_SPEC_TRAN		F3
#define KEY_PICK_HELP		F4
#define KEY_MAN_HELP		SF4
#define KEY_INDEX_HELP		AF4
#define KEY_DUP_RECEIPT		F5
#define KEY_CALCULATIONS	F6
#define KEY_WS_LOCK		F7
#define KEY_OVERRIDE		F8
#define KEY_OFFICE_SUP		F9
#define KEY_DEMO			AF9
#define KEY_CLR_MESSAGE		SF9
#define KEY_DUP_TRAN		F10
#define KEY_DUP_FIELD		F10
#define KEY_CALCULATOR		F11
#define KEY_CALENDAR		SF11
#define KEY_SEND			F12
/* Teller functions */
/* Se cambia la tecla F3 por F19 (Bloq Num) y F4 por F21 (*) - 14/3/96 - AD
#define KEY_TRAN_MENU		F3
#define KEY_TLR_ACTIV		F4  */
#define KEY_TRAN_MENU		F19
#define KEY_TLR_ACTIV		F21
#define KEY_LIST			F6
/*   Se comenta para usar F22   #define KEY_CORRECTION		F9  */
#define KEY_CORRECTION		F22
/* FGB (reingenieria) */
#define MNU_MODE_KEY			F9

/* New Accounts functions */
#define KEY_MAIN_MENU		F19
#define KEY_SESSION			F20
#define KEY_OPEN_ACCT		F21
#define KEY_SAVE_REC		F22

/* Navigation Keys for the AT keyboard */
#define KEY_UP_AT				F29
#define KEY_LEFT_AT				F31
#define KEY_DOWN_AT				F32
#define KEY_RIGHT_AT			F33
#define KEY_BACKSPACE_AT		F15
#define KEY_TAB_AT				F23
#define KEY_BACKTAB_AT			SF23
#define KEY_CLRFIELD_AT			F24
#define KEY_CLRALL_AT			SF24
#define KEY_ENTER_AT			F28
#define KEY_KEYPADENTER_AT		F30
#define KEY_INSERT_AT			F16
#define KEY_HOME_AT				F17
#define KEY_END_AT				F25
#define KEY_SEND_AT				F12
#define KEY_PAGEUP_AT			F18
#define KEY_PAGEDOWN_AT			F26
#define KEY_PRINTSCREEN_AT		F13

/* Navigation Keys for the 4900 keyboard */
#define KEY_LEFT_4900			F29
#define KEY_RIGHT_4900			F31
#define KEY_BACKSPACE_4900		F19
#define KEY_TAB_4900			F25
#define KEY_BACKTAB_4900		SF25
#define KEY_CLRFIELD_4900		F32
#define KEY_CLRALL_4900			F20
#define KEY_ENTER_4900			F28
#define KEY_KEYPADENTER_4900	F30
#define KEY_HOME_4900			F21
#define KEY_END_4900			F22
#define KEY_SEND_4900			F16
#define KEY_PAGEUP_4900			F13
#define KEY_PAGEDOWN_4900		F14
#define KEY_PRINTSCREEN_4900	F24

/* Office Support keys */
#define KEY_NEXT_AT				KEY_PAGEDOWN_AT
#define KEY_PREV_AT				KEY_PAGEUP_AT
#define KEY_NEXT_4900			KEY_PAGEDOWN_4900
#define KEY_PREV_4900			KEY_PAGEUP_4900

/* Bond Redemption keys */
#define KEY_COPY_AT				F22
#define KEY_COPY_4900			F27

/* Calendar/calculator keys */
#define KEY_PLUS_AT				F27
#define KEY_MINUS_AT			F22
#define KEY_MULT_AT				F21
#define KEY_DIV_AT				F20
#define KEY_CALC_DUP_AT			F19
#define KEY_PRINT_CLNDR_AT		AF13
#define KEY_PRINT_TAPE_AT		AF13
#define KEY_SAVE_TAPE_AT		F1
#define KEY_LOAD_TAPE_AT		F2

#define KEY_PLUS_4900			F30
#define KEY_MINUS_4900			F27
#define KEY_MULT_4900			F29
#define KEY_DIV_4900			F26
#define KEY_CALC_DUP_4900		F21
#define KEY_PRINT_CLNDR_4900	AF24
#define KEY_PRINT_TAPE_4900		AF24
#define KEY_SAVE_TAPE_4900		F1
#define KEY_LOAD_TAPE_4900		F2

/*******/

/****x* Pendientes de agrupación/DEF:include:NA_DT
* NOMBRE
*   DEF:include:NA_DT
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
/* prod con */
#define BUTterm         CB0
#define BUTrisk         CB1
#define BUTselect       CB2

/* sales */
#define BUTsales_open           SB0
#define BUTsales_next           SB2
#define BUTsales_prev           SB3

#define DS_OPEN_SCR             8
#define SA_OPEN_SCR             3
#define CA_OPEN_SCR             2
#define RP_OPEN_SCR             2
#define SD_OPEN_SCR             2
#define OD_OPEN_SCR                     1
#define ATM_OPEN_SCR            1
#define DIR_DEP_OPEN_SCR        1

/* OPEN */
#define TRANSMIT                SB0

#define DEL                   SB0
#define UPD                    SB1
#define SAV                    SB2

#define BUTprofile_state        CB0

#define BUTaccount_type     CB0
#define BUTser_chrg_code    CB1
#define BUToverdraft_code   CB2
#define BUTstmt_dispos      CB3
#define BUTfunds_type_src   CB4
#define BUTinterest_code    CB6
#define BUTexempt_status    CB7
#define BUTacct_ownership   CB8

#define BUTckbook_style     CB0
#define BUTcheck_design     CB1
#define BUTcover                    CB2
#define BUTquantity                 CB3

#define BUTdsv_account_type     CB0
#define BUTdsv_funds_type_src   CB1
#define BUTdsv_stmt_dispos      CB3
#define BUTdsv_acct_ownership   CB4
#define BUTdsv_exempt_status    CB5

#define BUTdd_state                             CB0

#define BUTpax_freq                             CB0
#define BUTpax_tran_code                CB1

#define BUTatm_offset                   CB0
#define BUTatm_repl                             CB1
#define BUTatm_reis                             CB2

#define DS_OPEN_NAME1           1
#define DS_OPEN_NAME2           6
#define DS_OPEN_NAME3           7
#define DS_OPEN_OPEN            2
#define DS_OPEN_ORDER           3
#define DS_OPEN_DOC                     4

#define SA_OPEN_NAME1           1
#define SA_OPEN_NAME2           6
#define SA_OPEN_NAME3           7
#define SA_OPEN_OPEN            2
#define SA_OPEN_DOC                     3

#define RP_OPEN_NAME1           1
#define RP_OPEN_NAME2           6
#define RP_OPEN_NAME3           7
#define RP_OPEN_OPEN            2
#define RP_OPEN_DOC                     4

#define CA_OPEN_NAME1           1
#define CA_OPEN_NAME2           6
#define CA_OPEN_NAME3           7
#define CA_OPEN_OPEN            2
#define CA_OPEN_DOC                     3

#define SD_OPEN_NAME1           1
#define SD_OPEN_NAME2           6
#define SD_OPEN_NAME3           7
#define SD_OPEN_OPEN            2
#define SD_OPEN_DOC                     3

#define ATM_OPEN_NAME1          1
#define ATM_OPEN_OPEN           2
#define ATM_OPEN_DOC            3

#define PAX_OPEN_NAME1          1
#define PAX_OPEN_OPEN           2
#define PAX_OPEN_DOC            3

#define DD_OPEN_NAME1           1
#define DD_OPEN_OPEN            2
#define DD_OPEN_DOC                     3

/* CIF screen */
#define CIF_RECALL              1
#define CIF_PROFILE_SEL         2
#define CIF_PROFILE_SESS        3
#define CIF_PROFILE_CREATE      4
#define CIF_PROFILE_MAINT       5
#define CIF_PROFILE_DELETE      6
#define CIF_PROFILE_SORT        7
#define CIF_SORT_DISPLAY        8

/* PBD screen */
#define PBD_PROFILE_CREATE      1
#define PBD_PROFILE_MAINT       2
#define PBD_PROFILE_DELETE      3
#define PBD_RECALL              4
#define PBD_PROFILE_SORT        5
#define PBD_SORT_DISPLAY        6

#define BUTdt_profile_state     CB0

/* Report Screens */
#define RPT_ANL_IND_DET                 1
#define RPT_ANL_SUM_BRN                         2
#define RPT_ANL_SUM_IND                         3
#define RPT_TRK_BRN_SUM                         4
#define RPT_TRK_DISP_DET                        5
#define RPT_TRK_IND_SUM                         6

#define BUTrpt_display                          SB0
#define BUTrpt_print                            SB1
#define BUTrpt_graph                            SB2

/*******/

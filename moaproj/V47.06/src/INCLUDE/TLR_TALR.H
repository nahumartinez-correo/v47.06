/****x* Pendientes de agrupación/DEF:include:tlr_talr
* NOMBRE
*   DEF:include:tlr_talr
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
/* Main Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name of custom teller keys routine */
#define TLR_MAIN_KEYS			KEYScustom_tlr
/* specify name of custom teller open day routine */
#define TLR_OPEN_DAY_RTN		TRANopen_day
/*--------------------------------------------------------------*/
/* Menu Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name for the teller (activities) key menu */
#define TLR_MENU_TELLER			teller_actv
/* specify title for the teller (activities) key menu */
#define TLR_MENU_TTL_TELLER		"Teller Activities"
/* specify name for the (teller) tran key menu */
#define TLR_MENU_TRAN			teller_tran
/* specify title for the (teller) tran key menu */
#define TLR_MENU_TTL_TRAN		"Teller Transactions"
/* specify unique titles for up to 4 speed key menus */
#define TLR_MENU_TTL_SPEED1		"Bank Speed Keys"
#define TLR_MENU_TTL_SPEED2		"Thrift Speed Keys"
/* specify menu object names for up to 4 speed key menus */
#define TLR_MENU_SPEED1			speed_key1
#define TLR_MENU_SPEED2			speed_key2
/*--------------------------------------------------------------*/
/* Cash Control Module Tailoring								*/
/*--------------------------------------------------------------*/
/* specify whether operator has option (ask) when linking to ctr */
#define TLR_CCTL_10KASK			1		/* 0=inform, 1=ask */
/*--------------------------------------------------------------*/
/* Duplicate (Field/Tran/Receipt) Module Tailoring				*/
/*--------------------------------------------------------------*/
/* autosend corrected tran (idle or journal scan) */
#define TLR_DUPS_CORSEND		1		/* 0=no, 1=yes */
/* allow correction for current tran (during data collect) */
#define TLR_DUPS_CORCUR			1		/* 0=no, 1=yes */
/*--------------------------------------------------------------*/
/* Journal Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name of custom journal header routine */
#define TLR_JNL_HEADER			JNLcustom_header
/* specify name of custom journal/totals resync decision routine */
#define TLR_JNL_RESYNC			JNLcustom_resync
/* specify name of custom journal scan format routine */
#define	TLR_JSCN_FORMAT			JSCNcustom_format
/* specify wait or nowait for journal CDS operations */
#define TLR_JNL_WAIT			0		/* 0=nowait, 1=wait */
/* specify prefilled direction for jnl scans ("Backward","Forward") */
#define TLR_JSCN_PREDIR			"Atras"
/* specify prefilled output for jnl scans ("Quick","Print","Display") */
#define TLR_JSCN_PREOUT			"Rapido"
/*--------------------------------------------------------------*/
/* Totals Module Tailoring										*/
/*--------------------------------------------------------------*/
/* specify name of custom totals clear routine */
#define TLR_TOTS_CLEAR			TOTScustom_clear
/* specify wait or nowait for totals CDS operations */
#define TLR_TOTS_WAIT			0		/* 0=nowait, 1=wait */
/*--------------------------------------------------------------*/
/* Passbook Module Tailoring									*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/* Alignment table for passbook format 1.						*/
/*--------------------------------------------------------------*/
/* The following parameters are defined in decipoints */
#define TLR_PSBK1_LSPACE		120		/* line spacing */
#define TLR_PSBK1_HEIGHT		4905	/* passbook height */
#define TLR_PSBK1_WIDTH			4050	/* passbook width */
#define TLR_PSBK1_FIRST_LINE	585		/* line one vertical offset */
/* The following parameters are defined in lines */
#define TLR_PSBK1_LN_BEF_CF		14		/* line # before centerfold */
#define TLR_PSBK1_CF_SKIP		3		/* centerfold skip */
#define TLR_PSBK1_LN_AFT_CF		15		/* line # after centerfold */
#define TLR_PSBK1_LAST_LN		30		/* last line # */
/*--------------------------------------------------------------*/
/* Alignment table for passbook format 2.						*/
/*--------------------------------------------------------------*/
/* The following parameters are defined in decipoints */
#define TLR_PSBK2_LSPACE		144		/* line spacing */
#define TLR_PSBK2_HEIGHT		7920	/* passbook height */
#define TLR_PSBK2_WIDTH			6120	/* passbook width */
#define TLR_PSBK2_FIRST_LINE	1440	/* line one vertical offset */
/* The following parameters are defined in lines */
#define TLR_PSBK2_LN_BEF_CF		15		/* line # before centerfold */
#define TLR_PSBK2_CF_SKIP		5		/* centerfold skip */
#define TLR_PSBK2_LN_AFT_CF		21		/* line # after centerfold */
#define TLR_PSBK2_LAST_LN		40		/* last line # */

/*******/

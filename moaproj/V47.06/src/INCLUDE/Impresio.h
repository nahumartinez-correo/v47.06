/****x* Pendientes de agrupación/DEF:include:Impresio
* NOMBRE
*   DEF:include:Impresio
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
#define	PAGE_IRP        24
#define 	PAGE_FACTURAB   60
#define    PAGE_FACTURA_CUERPO    21
#define    PAGE_FACTURA    24
#define 	REMITO          "IRP"
#define 	TRIBNOIDENT      "SNC"
#define 	REGIMSIMP        "RM"
#define 	FACTURAA        "RI"
#define    FACTURAA2       "RNI"
#define 	TICKFACTB       "EXE"
#define 	TICKFACTB2      "NR"
#define 	TICKFACTB3      "CF"
#define    CONSUMIDORFINAL "CF"
// Tipo de Factura
#define    FACTURA_A       1
#define    FACTURA_B       2
#define    IRP			    3
#define    TICKET	        4
// Distintas Familias
#define    SERVICIO        1
#define    OPCIONAL        2
#define    DESCUENTO       3
#define    MERCHA	        4
#define    TESORERIA       5
#define    INGRVARIOS      6
//Carina - 22/10/99 Se incluye descripcion de una nueva familia
#define    ACTVARIAS       7
#define    BONIF_PROD        9
#define    BONIF_CLIMED        10
#define    BONIF_SUR_POSTAL        17
#define    SERVGIROS       "11"
#define    OPCGIROS        "12"
#define    TASAGIROS       "13"
#define    ENCINTVIRTUAL_ADUANERO 	14
#define    ENCINTVIRTUAL_ENTREGA 	15
#define    HAY_REGISTROS	 1
#define    NO_HAY_REGISTROS 0
#define    CTACTE           1
#define    OCASIONAL        2
#define    TOPE_JERARQUIA   9999
#define    CTRLD			 \x04
#define    SETEODOC         "\x1bc0\x04"
#define    JUSTIFICOIZQ     "\x1ba0"
#define    LINEANUEVA       "\n"
#define    SALTODEPAGINA    "\x0c"
#define    CORTE_TICKET     "\ec0\x02"
#define    LINE_GUIONES     "--------------------------------------\n"
#define    SETEODOCGALI     "\x1bc0\x02"
//Agregado para Tira Auditora
#define    SETEODEFAULT     "\x1bc0\x03"
#define    TAB              0x1E
#define    LINENUEVAJOUR    "\n\x1e\n"
#define    SETEOJOU         "\x1bc0\x01"
#define 	RESET_DOCPRT 	"\x1b\x40\x0d\x0a"
#define 	RESET_DOCPRTSF1 	"\x1b\x40"
//Se Agrego para la Autoimpresion
#define    TOTAL_LINEA_FACTURA  50

// [PAW] 22/04/99 --> Agregado para Nota de Crédito.
#define FACTFIELD			"F"
#define LASER				"L"
#define DOCPR				"D"
#define DIRECTAS			"D"
#define REVERSAS			"R"
#define FACTURAS			"F"
#define NOTACRED 			"N"
#define ACRECENTAM			"A"
#define INGRESOBRUT		"B"
#define FINSESION			"S"
#define PENDIENTE			"P"
#define CAPTURA			"c"
#define IMPRESION			"i"
#define NULO				"_"
#define TERMINO			"T"
#define VERDAD				 0
#define FALSO				-1
#define SIN_FORM			 0
#define DIRREV				 1
#define PRESUP				 0
/*Agregado por Gabriel*/
#define NUEVOMEDIOPAGO		"M"
#define JNLMEDIOPAGO		"J"
#define IMPUESTOSELLADO	"I"
#define LONGITUDDESC       57
#define LIMITEINFSERVICIO	25
#define LIMITEINFDESTINO	10
// [PAW] --> Tamaño de buffer de impresión.
// [PAW] --> Cambia la impresión (antes a un campo, ahora a un buffer)
#define TAMBUFFER			400000
#define PRINTER_CMD "\x1B\x3D\x01"
#define LINEASMAX                    20
#define CLI_INT 			3
#define CLIUP 			4
#define BON_VENTAS_UP		"V"
#define BARRAPORCENT 	1
#define BARRAPROGRES 	2
#define BARRACOMPLETA1 	"�����������������������������������������������������������������������"
#define BARRAOK 		0
#define E_LONGTITULO 	-1
#define E_TOTALPASOS 	-2
#define COMPCONBARRA 1
#define MAX_VEC_INGBR	20

#define true 1
#define false 0
#define CLRJNLMEDPAGO		"M"
#define JNLMEDPAGO			"J"
#define INTVENTAUP			"U"

#define CON_FORM_ANT 1
#define CON_FORM_DES 2
#define CON_FORM 3

#define    CLIMED    5		/*CLIENTE MEDIO*/

#define	COMP_IMPOSICION  5
#define	PAGE_CIMPO        27

#define	UP_REC_PP      18 /*Cantidad de Ups (para el listado de Recibidas) Por Pagina, a lo ancho*/
#define	PROD_PP        35 /*Cantidad de Prod (para el listado de Recibidas) Por Pagina, a los largo*/
#define    PIETICKET2      "esta excluido de emitir comprobantes por\n"
#define    PIETICKET3	   "RG 1415 Anexo I punto A Inc. B\n"
#define    PIE1            "esta excluido de emitir comprobantes por\n"
#define    PIE2            "RG 1415 Anexo I punto A Inc. B\n"
#define BON_LOCAL		"L"
#define COMBOSERV				16
#define	PAGE_RSPCM        28

#define	CHEQUE_DIFERIDO        02
#define	CHEQUE_DIA 		       03
#define	CHEQUE_CANC        	   15
#define	TARJETA			       06
#define	CTACTE_HOST		       03
#define    UP_HOST    1
#define    CHEQUES_LG    4

#define BARRACOMPLETA2 	"##############################################################################################################################################"
/*******/

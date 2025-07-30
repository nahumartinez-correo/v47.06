#include "drv.h"
#include "ofbdefs.h"
#include "keys.h"
#include "presupuesto.h"
#include "impresio.h"
#include "tesoro.h"
#include "csr.h"
#include "pickdrv.h"
#include "presupuesto.h"
#include "giros.h"
#include "hcommstd.h"
#include "Sap.h"
#include "sucuvirt.h"
#include "hcommstd.h"
#include "commdef.h"
#include "descuento.h"
#include "bonif.h"
#include "cdserdef.h"
#include "reporte.h"
#undef TOTAL
#define SETEODOC         "\x1bc0\x04"
#define NEWLINE          "\n"
#define MENSAJES_CIERRE  "SI"
#define MAYORISTAS 8
#define MONEDA_SF1_PROV_GENERICA	"9"
#define STATUS_UP_INTERMEDIO 97
#define STATUS_UP_ANULADO 100
#define ESPECIE_SERV 1
#define ESPECIE_PROD 2
#define PRODUCTO_SERV 3
#define PRODUCTOCC_SERV 4
#define SPACES " "
#define VALUE_CHANGED	"C"
#define CANT_MED_PAGOS 6
#define PICKOP_SEARCH 3
#define TODOS_LOS_CLIENTES 9 
#define SERVICIO_NO_PESABLE -1
#define PICKOP_SORT 4
#define TIPO_ESPECIES_VS "TIPO_ESPECIES_VALORIZADAS"
#define TIPO_PRODUCTOS_VS "TIPO_PRODUCTOS_VARIOS"
#define TIPO_ENTREGA_DE_PZA "TIPO_ENTREGA_DE_PIEZAS"
#define REST_REG "REGIONES:"
#define REST_PRV "PROVINCIAS:"
#define REST_SUC "SUCURSALES:"
#define REST_CLI "CLIENTES:"
#define REST_UP "UP:"
#define CANCELAR 2
#define SIGUIENTE 0
#define PREVIO 1
#define SEPARADOR_DEFAULT "\x09"
#define REST_PRD "PRODUCTOS:"
#define REST_ESP "ESPECIES:"
#define DESCTO_SERVICIO "SERVICIOS:"
#define APROBADA 0
#define PROCESO_NO_ENCONTRADO 1
#define PARAMETROS_INCORRECTOS 2
#define PROCESO_CORRIO_CON_ERRORES 3
#define PROCESO_BLOQUEADO 4
#define ESPERANDO_MENSAJE -1
#define SIN_RESPUESTA_DEL_HOST -2

#include "base.h"
#include "cdserdef.h"
#include "gsp.h"
#include "ofbdefs.h"
#include "impresio.h"
#include "csr.h"
#include "drv.h"
#include "giros.h"
#include "pickdrv.h"
#include "Sap.h"
#include "tesoro.h"
#include "hcommstd.h"
#include "commdef.h"
#include "bonif.h"
#include "presupuesto.h"
#define INGRESO 1
#define EGRESO  2
#define ESPECIE_SERV 1
#define ESPECIE_PROD 2
#define PRODUCTO_SERV 3
#define TIPO_ENTREGA_DE_PZA "TIPO_ENTREGA_DE_PIEZAS" 
#define PICKOP_SEARCH 3
#define REST_REG "REGIONES:"
#define REST_PRV "PROVINCIAS:"
#define REST_SUC "SUCURSALES:"
#define REST_CLI "CLIENTES:"
#define REST_UP "UP:"
#define CODBAR_FAC_A "01"
#define CODBAR_FAC_B "06"
#define CODBAR_NC_A "03"
#define CODBAR_NC_B "08"
#define DESCTO_SERVICIO "SERVICIOS:"

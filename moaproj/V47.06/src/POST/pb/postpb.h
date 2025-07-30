#include "drv.h"
#include "base.h"
#include "cdserdef.h"
#include "gsp.h"
#include "ofbdefs.h"
#include "csr.h"
#include "presupuesto.h"
#include "pickdrv.h"
#include "Impresio.h"
#include "giros.h"
#include "sap.h"
#include "tesoro.h"
#include "descuento.h"
#include "bonif.h"
#include "hcommstd.h"
#define REEMP_MAX_MPAG 6
#define PICKOP_SEARCH 3
#define PICKOP_SORT 4
#define REST_REG "REGIONES:"
#define REST_PRV "PROVINCIAS:"
#define REST_SUC "SUCURSALES:"
#define REST_CLI "CLIENTES:"
#define REST_UP "UP:"
#define CANCELAR 2
#define SIGUIENTE 0
#define PREVIO 1
#define REST_PRD "PRODUCTOS:"
#define REST_ESP "ESPECIES:"
#define DESCTO_SERVICIO "SERVICIOS:"

#include "drv.h"
#include <ofbdefs.h>
#include "hcommstd.h"
#include "csr.h"
#include "tesoro.h"
#include "giros.h"
#include "sucuvirt.h"
#include "impresio.h"
#include "pickdrv.h"
#define NTRAMOS         8
#define RPT_ORIGINAL            0
#define RPT_COPIA               1
#define LONG_ARCH               80 /* Longitud maxima de un nombre de archivo */
#define BORRAR_LUEGO
#define TDRV_PRELOG     1               /* 0=no prelog, 1=prelog */
#define VENTA  78
#define COMPRA 76
#define ENT_INVERSA "E"
#define SAL_INVERSA "S"

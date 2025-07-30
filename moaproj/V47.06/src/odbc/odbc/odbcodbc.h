#include "pickdrv.h"
#include "csr.h"
#include "hcommstd.h"
#include "ofbdefs.h"
#include "drv.h"
#include "dberrs.h"
#include "pickdrv.h"
#include "giros.h"
;#define PL_GETITEM	              0x8000
#define SQL_AUTOCOMMIT                 102
#define SQL_AUTOCOMMIT_OFF           0 //UL
#define SQL_AUTOCOMMIT_ON            1 //UL
#define SQL_AUTOCOMMIT_DEFAULT          SQL_AUTOCOMMIT_ON

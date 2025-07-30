/****x* Pendientes de agrupación/DEF:includes:cb
* NOMBRE
*   DEF:includes:cb
* DESCRIPCION
* 	These defines are used to tailor parts of the     
* 	Credit Bureau  system. There are also .h files    
* 	in the different sub directories which can tailor 
* 	different function which are less global than     
* 	these.                                            
*      .                                            
* 	0 = DO NOT INSTALL THIS FUNCTION                  
* 	1 = INSTALL THIS FUNCTION                         
* IMPACTOS
*   Otros objetos que deberán ser analizados al modificar el objeto actual
* REFERENCIADO POR
*   Objetos que llaman al objeto documentado (obligatorio)
* DOCUMENTACION
*   Documentación relevante que deberá ser actualizada por la modificación de la rutina
* SOURCE
*/

#include "base.h"
#include "color.h"

/********************************************/
/* LOGIN ID'S         - Sets the size       */
/*      of the login_id operator number     */
/* branch number                            */
/********************************************/
#define LOGIN_ID_SIZE 8

#define SPEED_KEY 1
#define QUICK_KEY 1

#include "field.h"

/*******/

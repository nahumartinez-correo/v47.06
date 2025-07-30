/*$
** MODULENAME - cdscsr.c
**
**	$Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
**	$Revision: 1.11 $
**	$Date: 1997/01/10 22:04:56 $
**
**	Synopsis:
**		Load Dynamic Link Library for DataBase
**
**	External Function List:
**
**	Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
**
*/


#include "pch.h"

#include	"ppdefs.h"
#include	"defines.h"
#include	"typedefs.h"
#include	"rtdef.h"
#include	"cdsfuncs.h"
#include 	"svmode.h"
#include 	"clcdsfn.h"
#include	"csrdef.h"
#include	"rtprotos.h"
#include	"convers.h"
#include	"misc.h"
#include "cdscsr.h"

void rtAllocCDSEnv(void);

XDLL int
pingCDS(int argc, rtobj *argv[]) 
{
	int argn=0;
	int retries;
	char service_name_suffix[128];
	
	if (CdsEnv == NULL)
		rtAllocCDSEnv();
	if (argc == 0)
		return(cdsPing(CdsEnv,NULL,0));
	if (argc == 1)
		if (rtobj2int(argv[argn], &retries) == RT_ERROR)
			retries=0;
		else
			return(cdsPing(CdsEnv, NULL,retries));

	if (argc > 2)
		return (CSR_FAIL_ARGCOUNT);
	if (rtobj2str(argv[argn++], service_name_suffix, sizeof(service_name_suffix)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	// if there was only one argument and it
	// made it to this point, use the default for retries
	// otherwise, get the retries from the 2nd argument
	if (argc == 2) {
		if (rtobj2int(argv[argn++], &retries) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	}
	return(cdsPing(CdsEnv, service_name_suffix, retries));
}

// CSR:copyTableDef(source_table_name, target_table_name)
XDLL int
copyTableDef(int argc, rtobj *argv[]) 
{
	int argn=0;
	char source_table[CLTBLEN+1], target_table[CLTBLEN+1];
	

	if (argc !=2)
		return (CSR_FAIL_ARGCOUNT);
	if (rtobj2str(argv[argn++], source_table, sizeof(source_table)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	if (rtobj2str(argv[argn++], target_table, sizeof(target_table)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);
	return(Cds_Copy_Table_Def(CdsEnv, source_table, target_table));
}

XDLL int
stopCDS(int argc, rtobj *argv[]) 
{
	char service_name[CDS_MAX_SERVICE_NAME];
	if (argc != 1)
		return (CSR_FAIL_ARGCOUNT);
	if (rtobj2str(argv[0], service_name, sizeof(service_name)) == RT_ERROR)
		return(CSR_FAIL_ARGTYPE);


	return(cdsStop(CdsEnv, service_name));
}

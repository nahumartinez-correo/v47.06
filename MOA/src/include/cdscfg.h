#define _CDSCFG_H
#ifndef CDSLIMITS_H
#include <cdslmts.h>
#endif
typedef struct cds_config {
	char primary[MACHINE_NAME_LEN];
	char shadow[MACHINE_NAME_LEN];
	char tbl_path[OFS_MAXPATHNAME];
	char log_path[OFS_MAXPATHNAME];
} cds_config;

boolean cmconfig(cds_config *cfg, char *project,char *service_name);

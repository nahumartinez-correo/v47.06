#ifndef _CMGETENV_H
#define _CMGETENV_H
extern char *cmgetenv(const char *var_name, const char *env_file_name);
extern char *cmenvfile_name(const char *project, const char *service_name);

/* environmental variable names */
#define CDS_SERVICE_NAME	"CDS_SERVICE_NAME"
#define PRIMARY_NP		"CDS_PRIMARY_NP"
#define SHADOW_NP 		"CDS_SHADOW_NP"
#define CDS_DIRECTORY		"CDS_DIRECTORY"
#define CDSYNC_OUT_DIR		"CDSYNC_OUT_DIR"
#define CDSYNC_OUT		"CDSYNC_OUT"
#define CDS_NKILL_TIME		"CDS_NKILL_TIME"
#define CDS_BACKUP_TIME		"CDS_BACKUP_TIME"
#define CDS_RECOVERY_LOG	"CDS_RECOVERY_LOG"

#endif

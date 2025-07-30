#ifndef CDSDRIVE_H
#define CDSDRIVE_H
#ifndef CDSLIMITS_H
#include <cdslmts.h>
#endif

typedef struct drivecb {
		char user[USER_NAME_LEN];
		char machine[MACHINE_NAME_LEN];
		int  process_id;
		int  fd;
} drivecb;

global_proc drivecb *find_dcb(char *user, char *machine, int process_id);

#endif

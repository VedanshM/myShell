#include "prompt.h"
#include "format.h"
#include "sysinfo.h"
#include <stdio.h>

int last_exec_status = 0;

void prompt() {
#ifdef DEBUG
	fprintf(stderr, "[%d]\n", last_exec_status);
#endif
	printf("%s" GRN_COL "<%s@%s: " BLU_COL "%s" GRN_COL " >>" RST_COL,
		   (last_exec_status == 0) ? GRN_COL ":')" : RED_COL ":'(",
		   username, hostname, intidir_to_tilda(get_pwd()));
}
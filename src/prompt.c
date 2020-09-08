#include "prompt.h"
#include "format.h"
#include "sysinfo.h"
#include <stdio.h>

void prompt() {
	printf(GRN_COL "<%s@%s: " BLU_COL "%s" GRN_COL " >>" RST_COL,
		   username, hostname, conv_initdir(get_pwd()));
}
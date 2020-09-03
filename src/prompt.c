#include "prompt.h"
#include "format.h"
#include "sysinfo.h"
#include <stdio.h>

void prompt() {
	printf("\033[1;34m"); //sets to blue
	printf("<%s@%s: %s >>", username, hostname, conv_initdir(get_pwd()));
	printf("\033[0m"); // reset
}
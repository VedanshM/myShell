#include "prompt.h"
#include "sysinfo.h"
#include <stdio.h>

void prompt() {
	printf("\033[1;34m"); //sets to blue
	printf("<%s@%s: ### ~>", username, hostname);
	printf("\033[0m"); // reset
}
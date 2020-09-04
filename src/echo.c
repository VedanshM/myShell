#include "echo.h"
#include <stdio.h>

int echo(char **args, int argc) {
	//args to be freed by caller func
	for (int i = 1; i < argc; i++) {
		printf("%s ", args[i]);
	}
	printf("\n");
	return 0;
}
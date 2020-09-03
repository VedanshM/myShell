#include "echo.h"
#include <stdio.h>

int echo(char **args, int n) {
	//args to be freed by caller func
	for (int i = 0; i < n; i++) {
		printf("%s ", args[i]);
	}
	printf("\n");
	return 0;
}
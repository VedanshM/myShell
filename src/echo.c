#include "echo.h"
#include <stdio.h>

int echo(command *cmd) {
	//args to be freed by caller func
	for (int i = 1; i < cmd->argc; i++) {
		printf("%s ", cmd->args[i]);
	}
	printf("\n");
	return 0;
}
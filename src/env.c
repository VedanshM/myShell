#include "env.h"
#include <stdio.h>
#include <stdlib.h>

int set_env(command *cmd) {
	if (cmd->argc != 3) {
		fprintf(stderr, "setenv requires exactly two args\n");
		return 1;
	}
	int ret = setenv(cmd->args[1], cmd->args[2], 1);
	if (ret < 0) {
		perror("error in setenv");
	}
	return ret == 0 ? 0 : 1;
}

int unset_env(command *cmd) {
	if (cmd->argc != 2) {
		fprintf(stderr, "unsetenv requires exactly 1 args\n");
		return 1;
	}
	int ret = unsetenv(cmd->args[1]);
	if (ret < 0) {
		perror("error in unsetenv");
	}
	return ret == 0 ? 0 : 1;
}

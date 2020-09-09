#include "quit.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int quit(command *cmd) {
	if (cmd->argc != 1) {
		fprintf(stderr, "quit shall have no args\n");
	}
	lastSetup();
	exit(0);
}
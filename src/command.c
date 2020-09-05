#include "command.h"
#include "format.h"
#include <stdlib.h>
#include <string.h>

command *create_command(char *s) {
	command *cmd = malloc(sizeof(command));
	cmd->argc = 1 + rem_extra_spaces(s);
	cmd->args = calloc(1 + cmd->argc, sizeof(char *));

	char *saveptr;
	char *c = strtok_r(s, " ", &saveptr);
	for (int i = 0; i < cmd->argc; i++) {
		cmd->args[i] = strdup(c); //free at bottom
		c = strtok_r(NULL, " ", &saveptr);
	}
	return cmd;
}

void destory_command(command *cmd) {
	for (int i = 0; i < cmd->argc; i++)
		free(cmd->args[i]);
	free(cmd->args);
	free(cmd);
}
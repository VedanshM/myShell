#include "command.h"
#include "format.h"
#include <stdlib.h>
#include <string.h>

command *create_command(char *s) {
	if (is_blankcmd(s))
		return NULL;
	command *cmd = malloc(sizeof(command));
	cmd->argc = 1 + rem_extra_spaces(s);
	cmd->args = calloc(1 + cmd->argc, sizeof(char *));

	char *saveptr;
	char *c = strtok_r(s, " ", &saveptr);
	for (int i = 0; i < cmd->argc; i++) {
		cmd->args[i] = strdup(c); //free at bottom
		c = strtok_r(NULL, " ", &saveptr);
	}
	char *lastarg = cmd->args[cmd->argc - 1];
	if (strcmp(lastarg, "&") == 0) {
		cmd->is_bg = 1;
		free(lastarg);
		cmd->args[cmd->argc - 1] = NULL;
		cmd->argc--;
		cmd->args = realloc(cmd->args, cmd->argc + 1);
	} else {
		int n = strlen(lastarg);
		if (lastarg[n - 1] == '&') {
			cmd->is_bg = 1;
			cmd->args[cmd->argc - 1] = realloc(cmd->args[cmd->argc - 1], n);
			cmd->args[cmd->argc - 1][n - 1] = 0;
		} else
			cmd->is_bg = 0;
	}
	return cmd;
}

void destory_command(command *cmd) {
	for (int i = 0; i < cmd->argc; i++)
		free(cmd->args[i]);
	free(cmd->args);
	free(cmd);
}
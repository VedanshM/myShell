#include "readexec.h"
#include "cd.h"
#include "echo.h"
#include "format.h"
#include "ls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *shellBuiltins[BUILTIN_CNT] = {
	"echo",
	"cd",
	"pwd",
	"ls",
	/*
	"pinfo" */
};
builtin_func_t builtin_funcs[BUILTIN_CNT] = {
	echo,
	cd,
	pwd,
	ls,
};

int read_n_exec() {
	const char *delim = ";";
	char *buf = NULL, *saveptr;
	size_t bufsz = 0;
	int n = getline(&buf, &bufsz, stdin); //freed at bottom
	if (n < 0)
		exit(0);
	buf[n - 1] = 0;
	char *command = strtok_r(buf, delim, &saveptr);
	while (command) {
		execute(command);
		command = strtok_r(NULL, delim, &saveptr);
	}
	free(buf);
	return 0;
}

int execute(char *s) {
	// s to be freed by caller
	command *cmd = create_command(s);
	int i, ret_val = 0;
	for (i = 0; i < BUILTIN_CNT; i++) {
		if (strcmp(shellBuiltins[i], cmd->args[0]) == 0) {
			ret_val = builtin_funcs[i](cmd);
			break;
		}
	}
	if (i == BUILTIN_CNT) {
	}
	destory_command(cmd);
	return ret_val;
}

int execute_child(char **args, int argc) {
}
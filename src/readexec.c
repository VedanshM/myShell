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

int execute(char *cmd) {
	// cmd to be freed by caller
	char *saveptr;
	int wcnt = 1 + rem_extra_spaces(cmd);
	char **words = calloc(1 + wcnt, sizeof(char *)); //free at bottom
	// last pointer is should be NULL

	char *c = strtok_r(cmd, " ", &saveptr);
	for (int i = 0; i < wcnt; i++) {
		words[i] = calloc(strlen(c), sizeof(c[0])); //free at bottom
		strcat(words[i], c);
		c = strtok_r(NULL, " ", &saveptr);
	}
	int i, ret_val = 0;
	for (i = 0; i < BUILTIN_CNT; i++) {
		if (strcmp(shellBuiltins[i], words[0]) == 0) {
			ret_val = builtin_funcs[i](words, wcnt);
			break;
		}
	}
	if (i == BUILTIN_CNT) {
		}
	for (int i = 0; i < wcnt; i++)
		free(words[i]);
	free(words);
	return ret_val;
}
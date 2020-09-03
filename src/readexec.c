#include "readexec.h"
#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *shellBuiltins[BUILTIN_CNT] = {
	"echo"
	/* "pwd",
	"cd",
	"ls",
	"pinfo" */
};
builtin_func_t builtin_funcs[BUILTIN_CNT] = {echo};

int read_n_exec() {
	const char *delim = ";";
	char *buf = NULL, *saveptr;
	size_t bufsz = 0;
	getline(&buf, &bufsz, stdin); //freed at bottom
	char *command = strtok_r(buf, delim, &saveptr);
	while (command) {
		exec(command);
		command = strtok_r(NULL, delim, &saveptr);
	}
	free(buf);
	return 0;
}

int exec(char *cmd) {
	// cmd to be freed by caller
	char *saveptr;
	int wcnt = 1 + rem_extra_spaces(cmd);
	char **words = calloc(wcnt, sizeof(char *)); //freed at bottom
	char *c = strtok_r(cmd, " ", &saveptr);
	for (int i = 0; i < wcnt; i++) {
		words[i] = calloc(strlen(c), sizeof(c[0])); //freed at bottom
		strcat(words[i], c);
		c = strtok_r(NULL, " ", &saveptr);
	}
	int i, ret_val = 0;
	for (i = 0; i < BUILTIN_CNT; i++) {
		if (strcmp(shellBuiltins[i], words[0]) == 0) {
			ret_val = builtin_funcs[i](words + 1, wcnt - 1);
			break;
		}
	}
	if (i == BUILTIN_CNT) {
		//TODO
	}
	for (int i = 0; i < wcnt; i++)
		free(words[i]);
	free(words);
	return ret_val;
}
#include "builtins.h"
#include <string.h>

#define BUILTIN_CNT 8
typedef int (*builtin_func_t)(command *cmd);

const char *shellBuiltins[BUILTIN_CNT] = {
	"echo",
	"cd",
	"pwd",
	"ls",
	"pinfo",
	"history",
	"nightswatch",
	"quit",
};
builtin_func_t builtin_funcs[BUILTIN_CNT] = {
	echo,
	cd,
	pwd,
	ls,
	pinfo,
	history,
	nightswatch,
	quit,
};

int exec_builtin(command *cmd) {
	for (int i = 0; i < BUILTIN_CNT; i++) {
		if (strcmp(shellBuiltins[i], cmd->args[0]) == 0) {
			builtin_funcs[i](cmd);
			return 1;
		}
	}
	return 0;
}
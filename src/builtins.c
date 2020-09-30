#include "builtins.h"
#include <string.h>

#define BUILTIN_CNT 15
typedef int (*builtin_func_t)(command *cmd);

const char *shellBuiltins[BUILTIN_CNT] = {
	"echo",
	"cd",
	"pwd",
	"ls",
	"pinfo",
	"history",
	"nightswatch",
	"setenv",
	"unsetenv",
	"jobs",
	"kjob",
	"fg",
	"bg",
	"overkill",
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
	set_env,
	unset_env,
	jobs,
	kjob,
	fg,
	bg,
	overkill,
	quit,
};

int exec_builtin(command *cmd) {
	for (int i = 0; i < BUILTIN_CNT; i++) {
		if (strcmp(shellBuiltins[i], cmd->args[0]) == 0) {
			int ret = builtin_funcs[i](cmd);
			return (ret == 0) ? 0 : 1;
		}
	}
	return -1;
}
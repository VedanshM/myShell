#include "readexec.h"
#include "bgprocess.h"
#include "cd.h"
#include "echo.h"
#include "format.h"
#include "history.h"
#include "ls.h"
#include "pinfo.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char *shellBuiltins[BUILTIN_CNT] = {
	"echo",
	"cd",
	"pwd",
	"ls",
	"pinfo",
};
builtin_func_t builtin_funcs[BUILTIN_CNT] = {
	echo,
	cd,
	pwd,
	ls,
	pinfo,
};

int read_n_exec() {
	const char *delim = ";";
	char *buf = NULL, *saveptr;
	size_t bufsz = 0;
	int n = getline(&buf, &bufsz, stdin); //freed at bottom
	if (n < 0)
		return 1;
	pushHist(buf);
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
		execute_child(cmd);
	}
	destory_command(cmd);
	return ret_val;
}

int execute_child(command *cmd) {
	int forkpid = fork();
	if (forkpid < 0) { //error in fork no child created
		fprintf(stderr, "Couldn't execute %s\n", cmd->args[0]);
		perror("error");
	} else if (forkpid == 0) { //in child
		setpgid(0, 0);
		if (execvp(cmd->args[0], cmd->args) == -1) {
			fprintf(stderr, "%s : command not found\n%s : ", cmd->args[0], cmd->args[0]);
			perror("");
			exit(1);
		}
	} else { // in parent process
		if (!cmd->is_bg) {
			signal(SIGTTIN, SIG_IGN); // to prevent defalut behaviouur of killing process
			signal(SIGTTOU, SIG_IGN);
			tcsetpgrp(0, forkpid); //giving control to child
			int status;
			waitpid(forkpid, &status, WUNTRACED); //waiting for child to finish
			tcsetpgrp(0, getpgrp());			  // taking control back
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);

		} else {
			make_bg(cmd, forkpid);
		}
	}
	return 0;
}
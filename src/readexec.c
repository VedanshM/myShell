#include "readexec.h"
#include "builtins.h"
#include "format.h"
#include "jobs.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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
		execute_semicolon_splits(command);
		command = strtok_r(NULL, delim, &saveptr);
	}
	free(buf);
	return 0;
}

int execute_semicolon_splits(char *s) {
	char **piped_segments;
	int segcnt = split_by_pipes(s, &piped_segments);

	int orgSTDIN = dup(STDIN_FILENO), orgSTDOUT = dup(STDOUT_FILENO);

	int pipe_fds[2];
	for (int i = 0; i < segcnt; i++) {
		//stdin set properly only tweek stdout
		if (i != segcnt - 1) {
			pipe(pipe_fds);
			dup2(pipe_fds[1], STDOUT_FILENO);
			close(pipe_fds[1]); //open only in stdout now
			execute_pipe_splits(piped_segments[i]);
			dup2(pipe_fds[0], STDIN_FILENO);
			close(pipe_fds[0]); //open only in stdin now

		} else {
			dup2(orgSTDOUT, STDOUT_FILENO);
			execute_pipe_splits(piped_segments[i]);
		}
	}
	destroy_pipesplits(piped_segments, segcnt);
	dup2(orgSTDIN, STDIN_FILENO);
	dup2(orgSTDOUT, STDOUT_FILENO);
	close(orgSTDIN);  //open only in stdin now
	close(orgSTDOUT); //open only in stdout now
	return 0;
}

int execute_pipe_splits(char *s) {
	// s to be freed by caller
	command *cmd = create_command(s);
	if (!cmd) { // only spaces provided
		return 0;
	}

	int oldfd[2];
	setRedirection(cmd, oldfd);
	if (!exec_builtin(cmd)) {
		execute_child(cmd);
	}
	resetRedirection(oldfd);
	destory_command(cmd);
	return 0;
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
			make_bg_job(cmd, forkpid);
		}
	}
	return 0;
}
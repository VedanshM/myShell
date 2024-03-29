#include "readexec.h"
#include "builtins.h"
#include "format.h"
#include "jobs.h"
#include "prompt.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int read_n_exec() {
#ifdef DEBUG
	fprintf(stderr, "[entered read_n_exec]\n");
#endif
	const char *delim = ";";
	char *buf = NULL, *saveptr;
	size_t bufsz = 0;
	int n = getline(&buf, &bufsz, stdin); //freed at bottom
	if (n < 0) {
		free(buf);
		return 1;
	}
	pushHist(buf);
	buf[n - 1] = 0;
	char *command = strtok_r(buf, delim, &saveptr);
	while (command) {
		last_exec_status = execute_semicolon_splits(command);
		command = strtok_r(NULL, delim, &saveptr);
	}
	free(buf);
#ifdef DEBUG
	fprintf(stderr, "[exiting read_n_exec]\n");
#endif
	return 0;
}

int execute_semicolon_splits(char *inp) {
#ifdef DEBUG
	fprintf(stderr, "[entered execute_semicolon_splits]\n");
#endif

	char *dp = strdup(inp);
	char *s = inp;
	int ex_next = 1, ret = 0;
	char *e;
	while (ex_next) {
		char *eand = strchr(s, '@');
		char *eor = strchr(s, '$');
		if (!eand)
			e = eor;
		else if (!eor)
			e = eand;
		else
			e = (eand - s < eor - s) ? eand : eor;
		if (!e)
			break;
		char c = *e;
		*e = 0;
		int ret = execute_one_term(s);
		if (c == '@')
			ex_next = (ret == 0) ? 1 : 0;
		else
			ex_next = (ret == 0) ? 0 : 1;
		s = e + 1;
	}
	if (ex_next)
		ret = execute_one_term(s);
	free(dp);
#ifdef DEBUG
	fprintf(stderr, "[exiting execute_semicolon_splits: %d]\n", ret);
#endif
	return ret;
}

int execute_one_term(char *s) {
#ifdef DEBUG
	fprintf(stderr, "[entered execute_one_term]\n");
#endif

	char **piped_segments;
	int segcnt = split_by_pipes(s, &piped_segments);

	int orgSTDIN = dup(STDIN_FILENO), orgSTDOUT = dup(STDOUT_FILENO);

	int pipe_fds[2], ret = 0;
	for (int i = 0; i < segcnt; i++) {
		//stdin set properly only tweek stdout
		if (i != segcnt - 1) {
			pipe(pipe_fds);
			dup2(pipe_fds[1], STDOUT_FILENO);
			close(pipe_fds[1]); //open only in stdout now
			ret = execute_pipe_splits(piped_segments[i]);
			dup2(pipe_fds[0], STDIN_FILENO);
			close(pipe_fds[0]); //open only in stdin now

		} else {
			dup2(orgSTDOUT, STDOUT_FILENO);
			ret = execute_pipe_splits(piped_segments[i]);
		}
	}
	destroy_pipesplits(piped_segments, segcnt);
	dup2(orgSTDIN, STDIN_FILENO);
	dup2(orgSTDOUT, STDOUT_FILENO);
	close(orgSTDIN);  //open only in stdin now
	close(orgSTDOUT); //open only in stdout now
#ifdef DEBUG
	fprintf(stderr, "[exiting execute_one_term: %d]\n", ret);
#endif
	return ret;
}

int execute_pipe_splits(char *s) {
	// s to be freed by caller
#ifdef DEBUG
	fprintf(stderr, "[entered execute_pipe_splits]\n");
#endif

	command *cmd = create_command(s);
	if (!cmd) { // only spaces provided
		return 0;
	}

	int oldfd[2];
	setRedirection(cmd, oldfd);
	int ret = 0;
	if ((ret = exec_builtin(cmd)) == -1) {
		ret = execute_child(cmd);
	}
	resetRedirection(oldfd);
	destory_command(cmd);
#ifdef DEBUG
	fprintf(stderr, "[exiting execute_pipe_splits with code: %d]\n", ret);
#endif

	return ret;
}

int execute_child(command *cmd) {
#ifdef DEBUG
	fprintf(stderr, "[entered exec_child]\n");
#endif
	int ret = 0;
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
		setpgid(forkpid, forkpid);
		if (cmd->is_bg) {
			add_bg_joblist(forkpid, cmd->args[0]);
			ret = 0;
		} else {
			ret = wait_for_pid(forkpid);
		}
	}
#ifdef DEBUG
	fprintf(stderr, "[exiting exec_child]\n");
#endif

	return ret;
}

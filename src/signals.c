#include "signals.h"
#include "format.h"
#include "jobs.h"
#include "prompt.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void init_sig_setup() {
	signal(SIGCHLD, remove_bg_job_handler);
	signal(SIGTSTP, tstp_handler);
	signal(SIGINT, int_handler);
}

void remove_bg_job_handler(int sig) {
	int status;
	pid_t deadpid = waitpid(-1, &status, WNOHANG);
	if (deadpid > 0) {
		job_t *jb = remove_bg_joblist(deadpid);
		if (!jb)
			return;
		printf("\r" CLEAR_AFTER);
		fflush(stdout);
		fprintf(stderr, "%s with pid %d exited %s\n", jb->commandName, deadpid,
				((WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) ? "normally" : "abnormally"));
		prompt();
		fflush(stderr);
		fflush(stdout);
		free(jb);
		return;
	}
}

void int_handler(int sig) {
	write(STDOUT_FILENO, "\n", 1);
	prompt();
	fflush(stdout);
}

void tstp_handler(int sig) {
	write(STDOUT_FILENO, "\n", 1);
	prompt();
	fflush(stdout);
}

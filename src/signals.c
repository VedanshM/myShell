#include "signals.h"
#include "format.h"
#include "jobs.h"
#include "prompt.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
				((WIFEXITED(status) ? "normally" : "abnormally")));
		prompt();
		fflush(stderr);
		fflush(stdout);
		free(jb);
		return;
	}
}

void init_sig_setup() {
	signal(SIGCHLD, remove_bg_job_handler);
}
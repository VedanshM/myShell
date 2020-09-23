#include "jobs.h"
#include "command.h"
#include "format.h"
#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

job_t *job_list;
ssize_t joblist_size = 0, joblist_bufsize = 0;

void make_bg_job(command *cmd, pid_t pid) {
	/** 
     *  the process is supposed to already be blocked from stdin
     *  this function only does to add this in a dynamic array
     *  for future refrence
     */
	if (!job_list || joblist_bufsize == 0) {
		job_list = realloc(job_list, 2 * sizeof(job_t));
		joblist_bufsize = 2;
	} else if (joblist_size == joblist_bufsize) {
		job_list = realloc(job_list, 2 * joblist_bufsize);
		joblist_bufsize *= 2;
	}
	job_list[joblist_size++] = (job_t){.commandName = strdup(cmd->args[0]), .pid = pid};
}

void remove_bg_job_handler(int sig) {
	int status;
	pid_t deadpid = waitpid(-1, &status, WNOHANG);
	if (deadpid > 0) {
		char *cmdname;
		for (int i = 0; i < joblist_size; i++) {
			if (deadpid == job_list[i].pid) {
				cmdname = job_list[i].commandName;
				while (i + 1 < joblist_size) {
					job_list[i] = job_list[i + 1];
					i++;
				}
				joblist_size--;
				printf("\r" CLEAR_AFTER);
				fflush(stdout);
				fprintf(stderr, "%s with pid %d exited %s\n", cmdname, deadpid,
						((WIFEXITED(status) ? "normally" : "abnormally")));
				prompt();
				fflush(stderr);
				fflush(stdout);
				free(cmdname);
				return;
			}
		}
	}
}

#include "jobs.h"
#include "command.h"
#include "format.h"
#include "prompt.h"
#include "sysinfo.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

job_t *job_list;
ssize_t joblist_len = 0, joblist_buflen = 0;

int add_bg_joblist(command *cmd, pid_t pid) {
	/** 
     *  the process is supposed to already be blocked from stdin
     *  this function only does to add this in a dynamic array
     *  for future refrence
     */
#ifdef DEBUG
	fprintf(stderr, "[entered add_bg_joblist]\n");
#endif

	if (!job_list || joblist_buflen == 0) {
		job_list = realloc(job_list, 2 * sizeof(job_t));
		joblist_buflen = 2;
	} else if (joblist_len == joblist_buflen) {
		job_list = realloc(job_list, 2 * joblist_buflen * sizeof(job_t));
		joblist_buflen *= 2;
	}
	job_list[joblist_len++] = (job_t){.commandName = strdup(cmd->args[0]), .pid = pid};
#ifdef DEBUG
	fprintf(stderr, "[exiting add_bg_joblist]\n");
#endif

	return 0;
}

int wait_for_pid(pid_t pid) {
	//here pid is a child which is in separate proc grp with gid = its pid
	signal(SIGTTIN, SIG_IGN); // to prevent defalut behaviour of killing process
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(STDIN_FILENO, pid); //giving control to child
	int status;
	waitpid(pid, &status, WUNTRACED);	//waiting for child to stop or terminate
	tcsetpgrp(STDIN_FILENO, getpgrp()); // taking control back
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	return 0;
}

void remove_bg_job_handler(int sig) {
	int status;
	pid_t deadpid = waitpid(-1, &status, WNOHANG);
	if (deadpid > 0) {
		char *cmdname;
		for (int i = 0; i < joblist_len; i++) {
			if (deadpid == job_list[i].pid) {
				cmdname = job_list[i].commandName;
				memmove(job_list + i, job_list + i + 1, sizeof(job_t) * (joblist_len - 1 - i));
				joblist_len--;
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

int jobs(command *cmd) {
	if (cmd->argc != 1) {
		fprintf(stderr, "jobs command takes no arguments\n");
		return -1;
	}
	char tmp[PATHMAX];
	for (int i = 0; i < joblist_len; i++) {
		int pid = job_list[i].pid;
		sprintf(tmp, "/proc/%d/status", pid);
		int fd = open(tmp, O_RDONLY);
		if (!fd) {
			fprintf(stderr, "couldnt read info for pid: %d\n", pid);
			perror("error");
			continue;
		}
		read(fd, tmp, 64);
		tmp[64] = 0;
		char state[16], st;
		sscanf(strstr(tmp, "State:") + 7, "%c", &st);
		switch (st) {
		case 'R':
			strcpy(state, "running");
			break;
		case 'S':
			strcpy(state, "sleeping");
			break;
		case 'D':
			strcpy(state, "disk sleep");
			break;
		case 'T':
			strcpy(state, "stopped");
			break;
		case 'Z':
			strcpy(state, "zombie");
			break;
		case 'X':
			strcpy(state, "dead");
			break;
		}
		printf("[%d] %s %s [%d]\n", i + 1, state, job_list[i].commandName, pid);
	}
	return 0;
}

int kjob(command *cmd) {
	if (cmd->argc != 3) {
		fprintf(stderr, "kjob takes only 2 inputs\n");
		return -1;
	}
	int jobno = atoi(cmd->args[1]) - 1,
		signo = atoi(cmd->args[2]);
	if (jobno >= joblist_len || jobno < 0) {
		fprintf(stderr, "no command with job number %d\n", jobno);
		return -1;
	}
	if (signo >= 32 || signo < 0) {
		fprintf(stderr, "no such signal exists\n");
		return -1;
	}
	if (kill(job_list[jobno].pid, signo) != 0) {
		perror("error in kjob");
		return -1;
	}
	return 0;
}

int fg(command *cmd) {
	if (cmd->argc != 2) {
		fprintf(stderr, "fg requires exactly one argument.\n");
		return -1;
	}
	int jobno = atoi(cmd->args[1]) - 1;
	if (jobno >= joblist_len || jobno < 0) {
		fprintf(stderr, "no command with job number %d\n", jobno);
		return -1;
	}
	if (kill(job_list[jobno].pid, SIGCONT) != 0) {
		perror("could not set process running");
		return -1;
	}
	return wait_for_pid(job_list[jobno].pid);
}

int bg(command *cmd) {
	if (cmd->argc != 2) {
		fprintf(stderr, "bg requires exactlty one argument\n");
		return -1;
	}
	int jobno = atoi(cmd->args[1]) - 1;
	if (jobno >= joblist_len || jobno < 0) {
		fprintf(stderr, "no command with job number %d\n", jobno);
		return -1;
	}
	if (kill(job_list[jobno].pid, SIGCONT) != 0) {
		perror("could not set process running");
		return -1;
	}
	return 0;
}

#include "jobs.h"
#include "command.h"
#include "format.h"
#include "prompt.h"
#include "signals.h"
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

int add_bg_joblist(pid_t pid, const char *proc_name) {
	/** 
     *  the process is supposed to already be blocked from stdin
     *  this function only does to add this in a dynamic array
     *  for future refrence
     */
#ifdef DEBUG
	fprintf(stderr, "[entered add_bg_joblist]\n");
#endif

	char tmp[PATHMAX];
	sprintf(tmp, "/proc/%d/status", pid);
	int fd = open(tmp, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "process %d: status file could not be read", pid);
		perror("error in adding to job list");
		return -1;
	}

	if (!job_list || !joblist_buflen) {
		job_list = realloc(job_list, 2 * sizeof(job_t));
		joblist_buflen = 2;
	} else if (joblist_len == joblist_buflen) {
		job_list = realloc(job_list, 2 * joblist_buflen * sizeof(job_t));
		joblist_buflen *= 2;
	}

	job_list[joblist_len] = (job_t){.pid = pid};
	if (!proc_name) {
		lseek(fd, strlen("Name:"), SEEK_SET);
		read(fd, tmp, 32);
		sscanf(tmp, "%s", job_list[joblist_len].commandName);
		//acc to linux standards scanned name will fit in array
	} else
		strcpy(job_list[joblist_len].commandName, proc_name);

	joblist_len++;
#ifdef DEBUG
	fprintf(stderr, "[exiting add_bg_joblist]\n");
#endif
	return 0;
}

job_t *remove_bg_joblist(pid_t pid) {
	if (!job_list)
		return NULL;
	for (int i = 0; i < joblist_len; i++) {
		if (pid == job_list[i].pid) {
			job_t *ptr = malloc(sizeof(job_t));
			*ptr = job_list[i]; // commandname still needs to be freed
			memmove(job_list + i, job_list + i + 1, sizeof(job_t) * (joblist_len - 1 - i));
			joblist_len--;
			return ptr;
		}
	}
	return NULL;
}

int wait_for_pid(pid_t pid) {
	//here pid is a child which is in separate proc grp with gid = its pid
	signal(SIGTTIN, SIG_IGN); // to prevent defalut behaviour of killing process
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(STDIN_FILENO, pid); //giving control to child
	int status;
	int ret = waitpid(pid, &status, WUNTRACED); //waiting for child to stop or terminate
	tcsetpgrp(STDIN_FILENO, getpgrp());			// taking control back
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);

	if (ret == -1)
		return -1;

	if (WIFSTOPPED(status))
		add_bg_joblist(pid, NULL);

	return 0;
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
		char *ptr = strstr(tmp, "State:");
		if (!ptr) {
			fprintf(stderr, "process %d not found\n.", pid);
			continue;
		}
		sscanf(ptr + 7, "%c", &st);
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
	job_t *ptr = remove_bg_joblist(job_list[jobno].pid);
	int pid = ptr->pid;
	free(ptr);
	if (kill(pid, SIGCONT) != 0) {
		perror("could not set process running");
		return -1;
	}
	return wait_for_pid(pid);
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

int overkill(command *cmd) {
	if (cmd->argc != 1) {
		fprintf(stderr, "Incorrect usage of overkill: no arguments should be provided.\n");
		return -1;
	}
	for (int i = joblist_len - 1; i >= 0; i--) {
		kill(job_list[i].pid, SIGKILL);
		remove_bg_job_handler(0); // signal handler idk why not executed this automatically in some cases
	}
	return 0;
}

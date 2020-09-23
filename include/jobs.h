#ifndef JOBS_H
#define JOBS_H
#include "command.h"
#include <sys/types.h>

typedef struct job_t {
	char *commandName;
	pid_t pid;
} job_t;

void make_bg_job(command *cmd, pid_t pid);
void remove_bg_job_handler(int sig);

#endif
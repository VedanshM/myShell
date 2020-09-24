#ifndef JOBS_H
#define JOBS_H
#include "command.h"
#include <sys/types.h>

typedef struct job_t {
	char *commandName;
	pid_t pid;
} job_t;

int wait_for_pid(pid_t pid);
// for foreground running of child proces pauses the shell
int add_bg_joblist(command *cmd, pid_t pid);
job_t *remove_bg_joblist(pid_t pid);

int jobs(command *cmd);
int kjob(command *cmd);
int fg(command *cmd);
int bg(command *cmd);
int overkill(command *cmd);

#endif
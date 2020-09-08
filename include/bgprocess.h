#ifndef BGP_H
#define BGP_H
#include "command.h"
#include <sys/types.h>

typedef struct process_t {
	char *commandName;
	pid_t pid;
} process_t;

void make_bg(command *cmd, pid_t pid);
void remove_bg(int sig);

#endif
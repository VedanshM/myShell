#include "bgprocess.h"
#include "command.h"
#include "format.h"
#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

process_t *bgProcessList;
ssize_t bglist_size = 0, bglistbuf_size = 0;

void make_bg(command *cmd, pid_t pid) {
	/** 
     *  the process is supposed to already be blocked from stdin
     *  this function only does to add this in a dynamic array
     *  for future refrence
     */
	if (!bgProcessList || bglistbuf_size == 0) {
		bgProcessList = realloc(bgProcessList, 2 * sizeof(process_t));
		bglistbuf_size = 2;
	} else if (bglist_size == bglistbuf_size) {
		bgProcessList = realloc(bgProcessList, 2 * bglistbuf_size);
		bglistbuf_size *= 2;
	}
	bgProcessList[bglist_size++] = (process_t){.commandName = strdup(cmd->args[0]), .pid = pid};
}

void remove_bg(int sig) {
	int status;
	pid_t deadpid = waitpid(-1, &status, WNOHANG);
	if (deadpid > 0) {
		char *cmdname;
		for (int i = 0; i < bglist_size; i++) {
			if (deadpid == bgProcessList[i].pid) {
				cmdname = bgProcessList[i].commandName;
				while (i + 1 < bglist_size) {
					bgProcessList[i] = bgProcessList[i + 1];
					i++;
				}
				bglist_size--;
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

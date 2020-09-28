#include "cd.h"
#include "format.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cd(command *cmd) {
	static char last_wd[PATHMAX] = {0};
	if (last_wd[0] == '\0')
		strcpy(last_wd, get_pwd());
	if (cmd->argc != 1 && cmd->argc != 2) {
		printf("cd should have only 1 or 0 args\n");
		return -1;
	}
	char *destdir;
	int ret = 0, echo = 0;
	if (cmd->argc == 1) {
		destdir = strdup(initdir);
	} else if (cmd->argc == 2) {
		if (strcmp(cmd->args[1], "-") == 0) {
			echo = 1;
			destdir = strdup(last_wd);
		} else
			destdir = process_path(cmd->args[1], 0);
	}
	char tmp[PATHMAX];
	strcpy(tmp, get_pwd());
	if (chdir(destdir) == -1) {
		fprintf(stderr, "Directory provided :%s \n", destdir);
		perror("Error in cd");
		ret = -1;
	} else {
		strcpy(last_wd, tmp);
		if (echo)
			printf("%s\n", intidir_to_tilda(destdir));
	}
	free(destdir);
	return ret;
}

int pwd(command *cmd) {
	printf("%s\n", get_pwd());
	if (cmd->argc != 1) {
		fprintf(stderr, "pwd should have no args");
	}
	return 0;
}

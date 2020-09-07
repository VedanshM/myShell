#include "pinfo.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int pinfo(command *cmd) {
	if (cmd->argc > 2) {
		fprintf(stderr, "pinfo takes only 1 or 0 arg.\n");
		return 1;
	}
	pid_t pid = (cmd->argc == 2) ? atoi(cmd->args[1]) : getpid();
	printf("pid -- %d\n", pid);
	char *tmp = malloc(PATHMAX * sizeof(char));
	sprintf(tmp, "/proc/%d/status", pid);
	FILE *stfil = fopen(tmp, "r");
	if (!stfil) {
		fprintf(stderr, "couldnt read info for pid: %d\n", pid);
		perror("error");
		return 1;
	}
	char line[100], disp[PATHMAX] = {0};
	int stfg = 0, memfg = 0;
	// flags for are they shown
	while (fgets(line, 100, stfil) && !(stfg && memfg)) {
		if (strncmp(line, "State:", 6) == 0) {
			char *p = line + 7;
			sscanf(p, "%s", disp);
			printf("Process Status -- %s\n", disp);
			stfg = 1;
		} else if (strncmp(line, "VmSize:", 7) == 0) {
			char *p = line + 8;
			sscanf(p, "%s", disp);
			printf("memory -- %s\n", disp);
			memfg = 1;
		}
	}
	fclose(stfil);
	sprintf(tmp, "/proc/%d/exe", pid);
	int n = readlink(tmp, disp, PATHMAX);
	disp[n] = 0;
	printf("Executable Path -- %s\n", disp);
	free(tmp);
	return 0;
}
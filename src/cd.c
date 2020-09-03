#include "cd.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cd(char **args, int n) {
	if (n != 0 && n != 1) {
		printf("cd should have only 1 or 0 args\n");
		return -1;
	}
	if (n == 1) {
		char *dir = calloc(PATHMAX, sizeof(char));
		if (args[0][0] == '~') {
			strcpy(dir, initdir);
			int i = strlen(dir);
			dir[i] = '/';
			i++;
			strcpy(dir + i, args[0] + 1);
		} else
			strcpy(dir, args[0]);

		if (chdir(dir) == -1) {
			fprintf(stderr, "Directory provided :%s \n", dir);
			perror("Error in cd");
			free(dir);
			return -1;
		}
		free(dir);

	} else {
		if (chdir(initdir) == -1) {
			fprintf(stderr, "Directory provided :%s \n", initdir);
			perror("Error in cd");
			return -1;
		}
	}

	return 0;
}

int pwd(char **args, int n) {
	printf("%s\n", get_pwd());
	if (n != 0) {
		fprintf(stderr, "pwd should have no args");
	}
	return 0;
}

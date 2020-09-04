#include "cd.h"
#include "format.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cd(char **args, int argc) {
	if (argc != 1 && argc != 2) {
		printf("cd should have only 1 or 0 args\n");
		return -1;
	}
	if (argc == 2) {
		char *dir = process_path(args[1], 0);

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

int pwd(char **args, int argc) {
	printf("%s\n", get_pwd());
	if (argc != 1) {
		fprintf(stderr, "pwd should have no args");
	}
	return 0;
}
#include "cd.h"
#include "sysinfo.h"
#include <stdio.h>
#include <unistd.h>

int cd(char **args, int n) {
	if (n != 0 && n != 1) {
		printf("cd should have only 1 or 0 args\n");
		return -1;
	}
	char *dir;
	if (n == 1)
		dir = args[0];
	else
		dir = initdir;
	if (chdir(dir) == -1) {
		fprintf(stderr, "Directory provided :%s \n", args[0]);
		perror("Error in cd");
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

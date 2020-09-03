#include "cd.h"
#include <stdio.h>
#include <unistd.h>

int cd(char **args, int n) {
	if (n != 1) {
		printf("Only 1 arg to cd allowed..\n");
		return -1;
	}
	if (chdir(args[0]) == -1) {
		fprintf(stderr, "Directory provided :%s \n", args[0]);
		perror("Error in cd");
	}
	return 0;
}

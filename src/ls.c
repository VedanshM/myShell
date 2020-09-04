#include "ls.h"
#include "format.h"
#include "sysinfo.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int filter_hidden(const struct dirent *fil) {
	if (fil->d_name[0] == '.')
		return 0;
	return 1;
}

int ls(char **args, int argc) {
	static char opts[] = "la";
	int l = 0, a = 0;
	char c;
	optind = 0;
	while ((c = getopt(argc + 1, args - 1, opts)) > 0) {
		switch (c) {
		case 'l':
			l = 1;
			break;
		case 'a':
			a = 1;
			break;
		default:
			break;
		}
	}
	if (optind - argc == 1) {
		ls_dir(".", l, a);
		printf("\n");
		return 0;
	}

	for (int i = optind - 1; i < argc; i++) {
		ls_indv(args[i], l, a);
		printf("\n");
	}
	return 0;
}

int ls_indv(char *arg, int l, int a) {
	char *path = process_path(arg, 0);
	struct stat st;
	stat(path, &st);
	if (S_ISDIR(st.st_mode)) {
		ls_dir(path, l, a);
	}
	free(path);
	return 0;
}

// int ls_fil(char *fil, char *dir, int l, int a) {
// }

int ls_dir(char *dirpath, int l, int a) {
	printf("%s:\n", dirpath);
	DIR *dir = opendir(dirpath);
	if (dir == NULL) {
		fprintf(stderr, "Error in ls of: %s", dirpath);
		perror("error:");
		return -1;
	}
	closedir(dir);
	struct dirent **entries = NULL;
	int nof = scandir(dirpath, &entries, ((!a) ? filter_hidden : NULL), alphasort); //hidden malloc
	if (nof == -1) {
		fprintf(stderr, "Error in scaning of: %s", dirpath);
		perror("error:");
		return -1;
	}
	if (l) {

	} else {
		for (int i = 0; i < nof; i++) {
			printf("%s ", entries[i]->d_name);
		}
	}
	return 0;
}

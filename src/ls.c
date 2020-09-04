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

int l, a, echoDir;

int filter_hidden(const struct dirent *fil) {
	if (fil->d_name[0] == '.')
		return 0;
	return 1;
}

int ls(char **args, int argc) {
	static char opts[] = "la";
	l = 0, a = 0;
	char c;
	optind = 1;
	while ((c = getopt(argc, args, opts)) > 0) {
		switch (c) {
		case 'l':
			l = 1;
			break;
		case 'a':
			a = 1;
			break;
		default:
			c = getopt(argc, args, opts);
			break;
		}
	}
	int no_of_paths = argc - optind;
	echoDir = (no_of_paths > 1) ? 1 : 0;

	if (no_of_paths == 0) {
		ls_dir(".");
		printf("\n");
		return 0;
	}

	for (int i = optind - 1; i < argc; i++) {
		ls_indv(args[i]);
		printf("\n");
	}
	return 0;
}

int ls_indv(char *arg) {
	char *path = process_path(arg, 0);
	struct stat st;
	stat(path, &st);
	if (S_ISDIR(st.st_mode)) {
		ls_dir(path);
	}
	free(path);
	return 0;
}

// int ls_fil(char *fil, char *dir, int l, int a) {
// }

int ls_dir(char *dirpath) {
	if (echoDir)
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
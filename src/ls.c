#include "ls.h"
#include "format.h"
#include "sysinfo.h"
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int l, a, echoDir;

int ls(command *cmd) {
	int argc = cmd->argc;
	char **args = cmd->args;
	l = 0, a = 0;
	int isOpt[argc];
	isOpt[0] = 1;
	int no_of_paths = 0;
	for (int i = 1; i < argc; i++) {
		if (args[i][0] == '-') {
			isOpt[i] = 1;
			for (int j = 0; args[i][j]; j++) {
				if (args[i][j] == 'l')
					l = 1;
				else if (args[i][j] == 'a')
					a = 1;
			}
		} else {
			isOpt[i] = 0;
			no_of_paths++;
		}
	}
	echoDir = (no_of_paths > 1) ? 1 : 0;

	if (no_of_paths == 0) {
		ls_dir(".");
		printf("\n");
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		if (isOpt[i])
			continue;
		ls_indv(args[i]);
		printf("\n");
	}
	return 0;
}

int ls_indv(char *arg) {
	char *path = process_path(arg, 0);
	struct stat st;
	stat(path, &st);
	int ret = 0;
	if (S_ISDIR(st.st_mode)) {
		ret = ls_dir(path);
	} else {
		ret = ls_file(path);
	}
	free(path);
	return ret;
}

int ls_file(char *fil) { //can handle dirs too for -l compatibilty
	char *tmp = realpath(fil, NULL);
	if (tmp == NULL) {
		fprintf(stderr, "Couldn't ls: %s\n", fil);
		perror("error");
		return -1;
	}
	if (l == 0) {
		printf("%s ", basename(fil));
	} else {
		struct stat st;
		stat(tmp, &st);
		char permsymbols[] = "rwx";
		char perms[11] = "-"
						 "---"
						 "---"
						 "---";
		int linkCnt = st.st_nlink;
		// char *owner :=getpwuid(st.st_uid)->pw_name,
		// char *grp := getgrgid(st.st_gid)->gr_name;
		size_t fsize = st.st_size;
		char tm[32];
		ctime_r(&st.st_mtime, tm);
		tm[16] = 0;
		//basename
		int mask[3][3] = {
			{S_IRUSR, S_IWUSR, S_IXUSR},
			{S_IRGRP, S_IWGRP, S_IXGRP},
			{S_IROTH, S_IWOTH, S_IXOTH}};
		if (S_ISDIR(st.st_mode))
			perms[0] = 'd';
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (st.st_mode & mask[i][j])
					perms[1 + 3 * i + j] = permsymbols[j];
		printf("%s %4d %s %s %10ld %s    %s\n",
			   perms,
			   linkCnt,
			   getpwuid(st.st_uid)->pw_name,
			   getgrgid(st.st_gid)->gr_name,
			   fsize,
			   tm + 4,
			   basename(fil));
	}
	free(tmp);
	return 0;
}

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
	char fullpath[PATHMAX] = {0};
	strcat(strcpy(fullpath, dirpath), "/");
	int n = strlen(fullpath);
	if (l) {
		size_t tot_blocks = 0;
		for (int i = 0; i < nof; i++)
			tot_blocks += fileBlocks(strcpy(fullpath + n, entries[i]->d_name) - n);
		tot_blocks /= 2;
		printf("total %ld \n", tot_blocks);
	}

	for (int i = 0; i < nof; i++) {
		ls_file(strcpy(fullpath + n, entries[i]->d_name) - n);
		free(entries[i]);
	}
	free(entries);
	return 0;
}

int filter_hidden(const struct dirent *fil) {
	if (fil->d_name[0] == '.')
		return 0;
	return 1;
}

size_t fileBlocks(char *fil) {
	char *tmp = process_path(fil, 1);
	struct stat st;
	if (stat(tmp, &st) == -1) {
		fprintf(stderr, "can't access : %s\n", fil);
		perror("error");
		free(tmp);
		return 0;
	}
	free(tmp);
	return st.st_blocks;
}

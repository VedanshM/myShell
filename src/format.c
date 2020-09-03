#include "format.h"
#include "sysinfo.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rem_extra_spaces(char *s) {
	int x = 0;
	int spcnt = 0;
	for (int i = 0; s[i]; i++) {
		if (s[i] == '\t')
			s[i] = ' ';
		if (s[i] != ' ' || (x > 0 && s[x - 1] != ' ')) {
			s[x++] = s[i];
			if (s[x - 1] == ' ')
				spcnt++;
		}
	}
	if (s[x - 1] != ' ')
		x++;
	else
		spcnt--;
	s[x - 1] = 0;
	return spcnt;
}

/* char *abs_path(char *s) {
	char *dir = calloc(PATHMAX, sizeof(char));
	if (s[0] != '/') {
		strcpy(dir, get_pwd());
		dir[strlen(dir)] = '/';
	}
	strcat(dir, s);
	char *abs = realpath(dir, NULL);
	free(dir);
	if (abs == NULL) {
		perror("Error in path provided");
		return NULL;
	}
	return abs;
}
 */
char *conv_initdir(char *s) { // no malloc only redunction in s
	int n = strlen(initdir);
	if (strncmp(s, initdir, n) == 0) {
		int i;
		s[0] = '~';
		for (i = 1; s[i]; i++) {
			s[i] = s[i - 1 + n];
		}
		if (s[i - 1] == '/')
			s[i] = 0;
	}
	return s;
}

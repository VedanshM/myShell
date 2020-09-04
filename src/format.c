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

char *process_path(char *s, int append_pwd) {
	char *path = calloc(PATHMAX, sizeof(char));
	if (s[0] == '/') {
		strcpy(path, s);
	} else if (s[0] == '~') {
		strcpy(path, initdir);
		strcat(path, s + 1);
	} else {
		if (append_pwd) {
			strcpy(path, get_pwd());
			int n = strlen(path);
			path[n] = '/';
			strcat(path, s);
		} else
			strcpy(path, s);
	}
	return path;
}

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

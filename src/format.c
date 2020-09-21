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

char *add_req_spaces(char *s) {
	// s should be a malloced array
	const int speccnt = 4;
	const char *specstr[4] = {
		"&",
		"<",
		">",
		">>",
	};
	int n = strlen(s);
	s = realloc(s, n + 1 + 2 * speccnt);
	char *pos;
	for (int i = 0; i < speccnt; i++) {
		char *findstart = s;
		while ((pos = strstr(findstart, specstr[i]))) {
			if (pos - s > 0 && *(pos - 1) != ' ') {
				memmove(pos + 1, pos, (s + n) - pos);
				*(pos) = ' ';
				n++;
				pos++;
			}
			if (*(pos + 1) && *(pos + 1) != ' ') {
				char *nxt = pos + strlen(specstr[i]);
				memmove(nxt + 1, nxt, (s + n) - nxt);
				*nxt = ' ';
				n++;
			}
			findstart = pos + 1;
		}
	}
	return realloc(s, n + 1);
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

int is_blankcmd(char *s) {
	const char *blanks[5] = {
		"",
		"\n",
		"&",
		"&\n",
		"& \n",
	};
	char *tmp = strdup(s);
	rem_extra_spaces(tmp);
	int ret = 0;
	for (int i = 0; i < 5; i++)
		if (strcmp(tmp, blanks[i]) == 0)
			ret = 1;
	free(tmp);
	return ret;
}

char *intidir_to_tilda(char *s) { // no malloc only redunction in s
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

int countchar(char *s, char c) {
	int cnt = 0;
	while (*s)
		if (*(s++) == c)
			cnt++;
	return cnt;
}

int split_by_pipes(char *inp, char ***pipedCmds) {
	int segment_cnt = countchar(inp, '|') + 1;
	*pipedCmds = malloc(sizeof(char *) * segment_cnt);
	char *s = inp;
	for (int i = 0; i < segment_cnt; i++) {
		char *e = strchr(s, '|');
		if (e)
			*e = 0;
		(*pipedCmds)[i] = s;
		s = e + 1;
	}
	return segment_cnt;
}

void destroy_pipesplits(char **s, int n) {
	// the string pointers point to parts of a single large string
	// hence doesnt need to be deallocated individually
	free(s);
}
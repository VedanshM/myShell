#include "history.h"
#include "sysinfo.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTSIZE 10
#define HISTFILESIZE 20
char HISTFILE[PATHMAX];
char *hist_lines[HISTFILESIZE] = {0};
int line_start = -1, line_end = -1;
/**
 * convention: e is after last valid *
 * s = e = -1 when empty
 * s = e != -1 when full
 */

void pushHist(char *line) {
	//assumes line ends at \n
	char *tmp = strdup(line);
	if (line_start == -1) {
		line_start = 0;
		line_end = 1;
		hist_lines[0] = tmp;
	} else if (line_end == line_start) {
		free(hist_lines[line_start]);
		hist_lines[line_start] = tmp;
		line_start++;
		line_start %= HISTFILESIZE;
		line_end = line_start;
	} else {
		hist_lines[line_end] = tmp;
		line_end++;
		line_end %= HISTFILESIZE;
	}
}

void hist_setup() {
	strcpy(HISTFILE, initdir);
	strcat(strcat(HISTFILE, "/"), ".mysh_history");
	FILE *fil = fopen(HISTFILE, "r");
	if (fil == 0) {
		if (errno != 2)
			perror("history error");
		else {
		}
		return;
	}
	size_t n = 0;
	int i = 0;
	while (i < 20 && getline(&hist_lines[i], &n, fil) > 0) {
		i++;
		n = 0;
	}
	fclose(fil);
	if (i > 0)
		line_start = 0, line_end = i - 1;
}

void hist_dumpback() {
	if (line_start < 0)
		return;
	FILE *fil = fopen(HISTFILE, "w");
	if (fil == 0) {
		perror("history error");
		return;
	}
	int i = line_start;
	do {
		fputs(hist_lines[i], fil);
		free(hist_lines[i]);
		i++;
		i %= HISTFILESIZE;
	} while (i != line_end);
	fclose(fil);
}
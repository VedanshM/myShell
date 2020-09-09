#include "history.h"
#include "format.h"
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

int history(command *cmd) {
	int n = HISTSIZE;
	if (cmd->argc > 2) {
		fprintf(stderr, "Only 1 argument allowed\n");
		return -1;
	} else if (cmd->argc == 2) {
		errno = 0;
		n = strtol(cmd->args[1], NULL, 10);
		if (errno != 0) {
			perror("error in history");
			return -1;
		}
		if (n > HISTFILESIZE) {
			printf("Only %d commands are stored!!\n", HISTFILESIZE);
			n = HISTFILESIZE;
		}
	}
	if (n <= 0 || line_start == -1)
		return 0;
	// now there is atleast one command to be shown
	// from a non empty hist

	int i = line_end - n;
	if (line_start == line_end) {
		if (i < 0)
			i += HISTFILESIZE;
	} else if (i < 0)
		i = 0;
	do {
		printf("%s", hist_lines[i]);
		i++;
		i %= HISTFILESIZE;
	} while (i != line_end);
	return 0;
}

void pushHist(char *line) {
	//assumes line ends at \n
	char *tmp = strdup(line);
	if (is_blankcmd(tmp)) {
		free(tmp);
		return;
	}

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
	while (i < 20) {
		if (getline(&hist_lines[i], &n, fil) <= 0) {
			free(hist_lines[i]);
			break;
		}
		if (is_blankcmd(hist_lines[i]))
			free(hist_lines[i]);
		else
			i++;
		n = 0;
	}
	fclose(fil);
	if (i > 0)
		line_start = 0, line_end = i % HISTFILESIZE;
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
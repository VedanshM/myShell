#include "nightswatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define SUPPORTED_OPT_COUNT 2

int do_io(char *s, int n);
// diplays s and return 0  if quit detected
// also sleeps for n sec while waiting

void interrupt(int n);
void newborn(int n);

int nightswatch(command *cmd) {
	static const char *options[SUPPORTED_OPT_COUNT] = {
		"interrupt",
		"newborn",
	};
	static const void (*optFuncs[SUPPORTED_OPT_COUNT])(int) = {
		interrupt,
		newborn,
	};

	if (cmd->argc != 4) {
		fprintf(stderr, "nightswatch accepts only 4 args.\n\n");
		return 1;
	}
	if (strcmp(cmd->args[1], "-n") != 0) {
		fprintf(stderr, "wrong usage\n\n");
		return 1;
	}
	int n = atoi(cmd->args[2]);
	if (n == 0 && strcmp(cmd->args[2], "0") != 0) {
		fprintf(stderr, "error: %s not a number.\n", cmd->args[2]);
		return 1;
	}

	for (int i = 0; i < SUPPORTED_OPT_COUNT; i++) {
		if (strcmp(cmd->args[3], options[i]) == 0) {
			optFuncs[i](n);
			break;
		}
	}
	return 0;
}

void newborn(int n) {
	char line[256] = {0};
	static const char *load_file = "/proc/loadavg";
	FILE *fil = fopen(load_file, "r");
	if (!fil) {
		perror("error in /proc/loadavg");
		return;
	}
	fclose(fil);
	char *lastproc;
	do {
		fil = fopen(load_file, "r");
		if (!fgets(line, sizeof(line), fil)) {
			fprintf(stderr, "could not read from %s\n", load_file);
			return;
		}
		fclose(fil);
		lastproc = strrchr(line, ' ') + 1;
		*(strchr(line, '\n')) = 0;
	} while (do_io(lastproc, n));
}

void interrupt(int n) {
	char line[256] = {0};
	static const char *intrpt_file = "/proc/interrupts";
	FILE *fil = fopen(intrpt_file, "r");
	if (!fil) {
		perror("error in interrupt");
		return;
	}
	if (!fgets(line, sizeof(line), fil)) {
		fprintf(stderr, "could not read from %s\n", intrpt_file);
		return;
	}
	printf("%s", line);
	fgets(line, sizeof(line), fil);
	int pos = ftell(fil);
	// dump one line
	fclose(fil);
	do {
		fil = fopen(intrpt_file, "r");
		fseek(fil, pos, SEEK_SET);
		fgets(line, sizeof(line), fil);
		fclose(fil);

		//formatting
		char *tmp = strchr(line, '1');
		tmp[0] = tmp[1] = ' ';
		tmp = strchr(line, 'I');
		*tmp = 0;

	} while (do_io(line, n));
}

int do_io(char *s, int n) {
	printf("%s\n", s);

	struct termios attributes;
	tcgetattr(STDIN_FILENO, &attributes);

	int oldflags = attributes.c_lflag;

	// disable canonical mode(no need of enter) and don't echo
	attributes.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	attributes.c_lflag = oldflags;
	int avail = 1;
	sleep(n);
	ioctl(STDIN_FILENO, FIONREAD, &avail);
	//reset state
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);

	while (avail > 0) {
		if (getchar() == 'q') {
			return 0; // no more io
		}
		ioctl(STDIN_FILENO, FIONREAD, &avail);
	}

	return 1;
}
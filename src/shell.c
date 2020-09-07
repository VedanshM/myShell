#include "prompt.h"
#include "readexec.h"
#include "signals.h"
#include "sysinfo.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void initSetup();
void repl();
void lastSetup();

int main() {
	initSetup();
	repl();
	lastSetup();
}

void repl() {
	while (1) {
		prompt();
		if (read_n_exec() != 0)
			return;
	}
}

void initSetup() {
	assign_names();
	strcpy(initdir, get_pwd());
	init_sig_setup();
}

void lastSetup() {
	printf("bye!!");
}
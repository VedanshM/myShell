#include "format.h"
#include "prompt.h"
#include "readexec.h"
#include "signals.h"
#include "sysinfo.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void initSetup();
void lastSetup();

int main() {
	initSetup();
	do {
		prompt();
	} while (read_n_exec() == 0);
	lastSetup();
}

void initSetup() {
	printf("\t\t\t" BLU_COL " >> Welcome to myShell << " RST_COL "\n");
	assign_names();
	strcpy(initdir, get_pwd());
	init_sig_setup();
}

void lastSetup() {
	printf("bye!!");
}
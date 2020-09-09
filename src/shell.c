#include "format.h"
#include "history.h"
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
	strcpy(initdir, get_pwd());
	printf("\t\t\t" BLU_COL " >> Welcome to myShell << " RST_COL "\n");
	assign_names();
	init_sig_setup();
	hist_setup();
}

void lastSetup() {
	printf("\n" BLU_COL "bye!!" RST_COL "\n");
	hist_dumpback();
}
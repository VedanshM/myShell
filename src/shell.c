#include "prompt.h"
#include "readexec.h"
#include "sysinfo.h"
#include <stdio.h>
#include <unistd.h>

void repl() {
	prompt();
	read_n_exec();
}

int main() {
	// constructors
	assign_names();
	while (1)
		repl();

	// destrucotrs
}
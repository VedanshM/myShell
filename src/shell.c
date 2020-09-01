#include "prompt.h"
#include "readexec.h"
#include <stdio.h>
#include <unistd.h>

void repl() {
	prompt();
	read_n_exec();
}

int main() {
	while (1)
		repl();
}
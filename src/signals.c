#include "signals.h"
#include "bgprocess.h"
#include <signal.h>
#include <stdio.h>

void init_sig_setup() {
	struct sigaction sa;
	sa.sa_handler = remove_bg;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		fprintf(stderr, "could not set signal for SIGCHLD ");
		perror("error");
	}
}
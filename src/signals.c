#include "signals.h"
#include "bgprocess.h"
#include <signal.h>

void init_sig_setup() {
	signal(SIGCHLD, remove_bg);
}
#include "signals.h"
#include "jobs.h"
#include <signal.h>

void init_sig_setup() {
	signal(SIGCHLD, remove_bg_job_handler);
}
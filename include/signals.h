#ifndef SIGNALS_H
#define SIGNALS_H

void remove_bg_job_handler(int sig);
void int_handler(int sig);
void tstp_handler(int sig);

void init_sig_setup();

#endif
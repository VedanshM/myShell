#ifndef READEXEC_H
#define READEXEC_H

#include "command.h"

int read_n_exec();
//scans a line from stdin and divides into commands and execs them using other funcs

int execute_semicolon_splits(char *inp);
//consider s to start and end by ';'

int execute_one_term(char *s);

int execute_pipe_splits(char *s);
// assumes s to be bounded by '|'
// fds for stdin and stdout sould be set by calling func
// accepts a command and execs it

int execute_child(command *cmd);
// accepts a null terminated list of strings
// and execs it in childprocess

#endif
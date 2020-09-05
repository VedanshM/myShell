#ifndef READEXEC_H
#define READEXEC_H

#include "command.h"

#define BUILTIN_CNT 4
typedef int (*builtin_func_t)(command *cmd);

extern const char *shellBuiltins[BUILTIN_CNT];
extern builtin_func_t builtin_funcs[BUILTIN_CNT];

int read_n_exec();
//scans a line from stdin and divides into commands and execs them

int execute(char *s);
// accepts a command and execs it

int execute_child(command *cmd);
// accepts a null terminated list of strings
// and execs it in childprocess

#endif
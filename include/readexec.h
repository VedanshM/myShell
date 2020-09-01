#ifndef READEXEC_H
#define READEXEC_H
#include "echo.h"

#define BUILTIN_CNT 1
typedef int (*builtin_func_t)(char **, int);

extern const char *shellBuiltins[BUILTIN_CNT];
extern builtin_func_t builtin_funcs[BUILTIN_CNT];

int read_n_exec();
int exec(char *s);
int rem_extra_spaces(char *s);

#endif
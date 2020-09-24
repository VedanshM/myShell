#ifndef BUILTINS_H
#define BUILTINS_H

#include "cd.h"
#include "echo.h"
#include "env.h"
#include "history.h"
#include "jobs.h"
#include "ls.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "quit.h"

int exec_builtin(command *cmd);
/**
 * returns 1 is successful, 0 is no such builtin
 * TODO: -1 for found builtin by failure
 */

#endif
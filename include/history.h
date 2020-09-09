#ifndef HISTORY_H
#define HISTORY_H
#include "readexec.h"

int history(command *cmd);
void pushHist(char *line);
void hist_setup();
void hist_dumpback();

#endif
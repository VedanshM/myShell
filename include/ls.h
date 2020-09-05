#ifndef LS_H
#define LS_H
#include "command.h"
#include <dirent.h>

int filter_hidden(const struct dirent *fil);
size_t fileBlocks(char *fil);

int ls(command *cmd);
int ls_indv(char *fil);
int ls_dir(char *dirpath);
int ls_file(char *fileBase);

#endif
#ifndef LS_H
#define LS_H
#include <dirent.h>

int filter_hidden(const struct dirent *fil);
int ls(char **args, int argc);
int ls_indv(char *fil);
int ls_dir(char *dirpath);
#endif
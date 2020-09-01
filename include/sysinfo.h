#ifndef SYSINFO_H
#define SYSINFO_H

extern char username[1024];
extern char hostname[1024];

char *get_pwd();
void assign_names();

#endif
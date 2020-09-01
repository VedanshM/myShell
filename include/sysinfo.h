#ifndef SYSINFO_H
#define SYSINFO_H

#define PATHMAX 4097
extern char username[1024];
extern char hostname[1024];
extern char initdir[PATHMAX];

char *get_pwd();
void assign_names();
void parse_dir(char *dir);

#endif
#include "sysinfo.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char username[1024] = {0};
char hostname[1024] = {0};
char initdir[PATHMAX] = {0};

void assign_names() {
	strcpy(username, getlogin());
	gethostname(hostname, sizeof(hostname));
}

char *get_pwd() { // no free to be used outside this func :)
	static char dir[PATHMAX] = {0};
	char *c = getcwd(NULL, 0);
	strcpy(dir, c);
	free(c);
	return dir;
}
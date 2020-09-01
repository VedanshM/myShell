#include "sysinfo.h"
#include <string.h>
#include <unistd.h>

char username[1024] = {0};
char hostname[1024] = {0};

void assign_names() {
	strcpy(username, getlogin());
	gethostname(hostname, sizeof(hostname));
}
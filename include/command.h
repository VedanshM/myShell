#ifndef COMMAND_H
#define COMMAND_H

typedef struct command {
	char **args; // * to 1+argc pointers, extra 1 for NULL
	int argc;

	char is_bg;
	int in_fd;
	int out_fd;
} command;

command *create_command(char *s);
void destory_command(command *cmd);
//frees member memories and cmd itself

int setRedirection(command *cmd, int old_files[2]);
int resetRedirection(int org_fd[2]);

#endif

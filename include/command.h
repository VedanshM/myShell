#ifndef COMMAND_H
#define COMMAND_H

typedef struct command {
	char **args;
	int argc;
} command;

command *create_command(char *s);
void destory_command(command *cmd);
//frees member memories and cmd itself

#endif

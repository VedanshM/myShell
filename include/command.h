#ifndef COMMAND_H
#define COMMAND_H

typedef struct command {
	char **args; // * to 1+argc pointers, extra 1 for NULL
	int argc;

	char is_bg;
} command;

command *create_command(char *s);
void destory_command(command *cmd);
//frees member memories and cmd itself

#endif

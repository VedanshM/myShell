#include "command.h"
#include "format.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

command *create_command(char *inp) {
	if (is_blankcmd(inp))
		return NULL;
	command *cmd = malloc(sizeof(command));
	char *s = strdup(inp);
	s = add_req_spaces(s);
	cmd->argc = 1 + rem_extra_spaces(s);
	cmd->args = calloc(1 + cmd->argc, sizeof(char *));

	char *saveptr;
	char *c = strtok_r(s, " ", &saveptr);
	for (int i = 0; i < cmd->argc; i++) {
		cmd->args[i] = strdup(c); //free at bottom
		c = strtok_r(NULL, " ", &saveptr);
	}
	cmd->in_fd = -1;
	cmd->out_fd = -1;
	if (strcmp(cmd->args[cmd->argc - 1], "&") == 0) {
		cmd->is_bg = 1;
		free(cmd->args[cmd->argc - 1]);
		cmd->args[cmd->argc - 1] = NULL;
		cmd->argc--;
		cmd->args = realloc(cmd->args, 1 + cmd->argc);
	} else
		cmd->is_bg = 0;
	for (int i = 0; i < cmd->argc - 1; /* update in loop */) {
		int fd;
		if (strcmp(cmd->args[i], ">>") == 0) {
			fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0) {
				perror("could not set redirection 'append'");
				destory_command(cmd);
				return NULL;
			} else {
				cmd->out_fd = fd;
			}
			free(cmd->args[i]);
			free(cmd->args[i + 1]);
			memmove(cmd->args + i, cmd->args + i + 2, (cmd->argc - i - 1) * sizeof(char *));
			cmd->argc -= 2;
			cmd->args = realloc(cmd->args, (cmd->argc + 1) * sizeof(char *));
		} else if (strcmp(cmd->args[i], ">") == 0) {
			fd = open(cmd->args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd < 0) {
				perror("could not set redirection 'out'");
				destory_command(cmd);
				return NULL;
			} else {
				cmd->out_fd = fd;
			}
			free(cmd->args[i]);
			free(cmd->args[i + 1]);
			memmove(cmd->args + i, cmd->args + i + 2, (cmd->argc - i - 1) * sizeof(char *));
			cmd->argc -= 2;
			cmd->args = realloc(cmd->args, (cmd->argc + 1) * sizeof(char *));

		} else if (strcmp(cmd->args[i], "<") == 0) {
			fd = open(cmd->args[i + 1], O_RDONLY);
			if (fd < 0) {
				perror("could not set redirection 'in'");
				destory_command(cmd);
				return NULL;

			} else {
				cmd->in_fd = fd;
			}
			free(cmd->args[i]);
			free(cmd->args[i + 1]);
			memmove(cmd->args + i, cmd->args + i + 2, (cmd->argc - i - 1) * sizeof(char *));
			cmd->argc -= 2;
			cmd->args = realloc(cmd->args, (cmd->argc + 1) * sizeof(char *));
		} else
			i++;
	}
	if (cmd->in_fd < 0)
		cmd->in_fd = dup(STDIN_FILENO);
	if (cmd->out_fd < 0)
		cmd->out_fd = dup(STDOUT_FILENO);

	return cmd;
}

void destory_command(command *cmd) {
	for (int i = 0; i < cmd->argc; i++)
		free(cmd->args[i]);
	free(cmd->args);
	free(cmd);
}

int setRedirection(command *cmd, int old_files[2]) {
	old_files[0] = dup(STDIN_FILENO);
	old_files[1] = dup(STDOUT_FILENO);
	if (dup2(cmd->in_fd, STDIN_FILENO) < 0) {
		perror("could not set stdin");
	}
	if (dup2(cmd->out_fd, STDOUT_FILENO) < 0) {
		perror("could not set stdout");
	}
	close(cmd->in_fd);
	close(cmd->out_fd);
	return 0;
}

int resetRedirection(int org_fd[2]) {
	if (dup2(org_fd[0], STDIN_FILENO) < 0) {
		perror("couldnt reset redirection");
	}
	if (dup2(org_fd[1], STDOUT_FILENO) < 0) {
		perror("couldnt reset redirection");
	}
	close(org_fd[0]);
	close(org_fd[1]);
	return 0;
}
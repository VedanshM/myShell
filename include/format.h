#ifndef FORMAT_H
#define FORMAT_H

int rem_extra_spaces(char *s);
//remove multiplespaces, tabs and extra leading/trainling spaces

char *process_path(char *s, int append_pwd);
// returns a path(using malloc) to handle cases of home, abs and pwd paths

char *conv_initdir(char *s);
//converts initdir to ~

#endif
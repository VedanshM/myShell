#ifndef FORMAT_H
#define FORMAT_H

int rem_extra_spaces(char *s);
//remove multiplespaces, tabs and extra leading/trainling spaces

char *abs_path(char *s);
// return a malloced string with abs path pointed by s[]

char *conv_initdir(char *s);
//converts initdir to ~

#endif
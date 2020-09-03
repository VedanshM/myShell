#ifndef FORMAT_H
#define FORMAT_H

int rem_extra_spaces(char *s);
//remove multiplespaces, tabs and extra leading/trainling spaces

char *process_path(char *s);
// return a malloced string with abs path pointed by s[]

#endif
#ifndef FORMAT_H
#define FORMAT_H

//   Colour escape codes
#define BLK_COL "\033[30;1m"
#define RED_COL "\033[31;1m"
#define GRN_COL "\033[32;1m"
#define YLW_COL "\033[33;1m"
#define BLU_COL "\033[34;1m"
#define MGN_COL "\033[35;1m"
#define CYN_COL "\033[36;1m"
#define WHT_COL "\033[37;1m"
#define RST_COL "\033[0m"

#define CLEAR_AFTER "\033[K"

int rem_extra_spaces(char *s);
//remove multiplespaces, tabs and extra leading/trainling spaces

char *process_path(char *s, int append_pwd);
// returns a path(using malloc) to handle cases of home, abs and pwd paths

char *conv_initdir(char *s);
//converts initdir to ~

#endif
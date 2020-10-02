# myShell

A simple shell written in basic C language and syscalls for linux OS only.

## Build and Usage

+ **Build** :  
  Just run `make` to build the executable binary, i.e. `mysh`.

+ **Run**:  
  run `./mysh` to use myShell.

> History can be found in `.mysh_history` file, in the directory from where `mysh` was executed.

## Shell features

+ redirection: redirects of stdin and stdout are supported by using `>`, `>` and `>>`
+ pipes: piping stdout of one program to stdin of other is supproted using `|`.
+ logical command chaining: support for logical AND (`@`) and logical OR (`$`) while chaining the commands. Evaluation will be from left to right.
+ last exec status: One can see the success or failure of the last run command in prompt as smileys.
+ Can handle signals like `SIGINT​` and `SIGTSTP` from terminal, to produce expected results.
+ myShell provides some basic shell builtins which are as follows:  
  
  + `quit` : just quits the shell. (recommended way of leaving the shell).
  + Environment vars: One can change environment variables using builtins `setenv var [value]` and `unsetenv var`.
  + Background jobs handling: various builtins are there to handle jobs, which are shell's child processes running in backgorung:
    + `jobs`: displays a list of current jobs along with job number and running status.
    + `kjob [jobno] [signo]` : send signal `signo` to job `jobno`.
    + `fg [jobno]`: bring job `jobno` to foreground and set it running if stopped.
    + `bg [jobno]`: set job `jobno` to running if stopped earlier.
    + `overkill`: kills all background processes at once.
  + `cd [path]`: change current working directory to `path`. If no arg provided then it `cd` to the initial directory of shell. `-` argumets sends to last working directory.
  + `pwd`: displays path of current working directory.
  + `echo [args]...` : Just prints its args in stdout as it is.
  + `ls [-al] [FILE]...` : list contents and information of directories and files. Supports `-a` and `-l` options from bash equivalent.
  + `pinfo [PID]` : prints information about given `pid`. if no argument provided then pid of the running shell.
  + `history [n]` : Displays last n commands from history. New history will not be saved if exited through CTRL+C on shell, use `quit`.
  + `nightswatch -n [sec] [interrupt|newborn]` : Similar to watch from bash but supports only 2 commands, interrupt and newborn. To stop just press q (no enter required).  
  [**Note**: after pressing q the exit can take up to `sec` seconds to perform].
+ Supports executable files in PATH set and can also run them in background by using `&`.

### Code files description

  > All .c files have corresponding .h files, in `inlcude/` folder which contain relevent declarations.  

+ builtins.c : contains functions to execute any bultin command.
+ cd.c : builtins `cd` and `pwd` implemented in this.
+ command.c: Functios useful for command's text processing.
+ echo.c : `echo` implementation.
+ env.c: `setenv` and `unsetenv` implementation.
+ format.c: useful for string formatting.
+ history.c : history related implementations.
+ jobs.c: `jobs`, `overkill`, `fg`,`bg` and other job related implentations.
+ ls.c : `ls` implementation.
+ nightswatch.c : `nightswatch` implementtaion.
+ pinfo.c : `pinfo` implementation.
+ prompt.c : func to display command prompt.
+ quit.c : `quit` implementation.
+ readexec.c: reads a line and executes constituent command(s).
+ shell.c : contains `main()` and shell's REPL loop.
+ signal.c: for handling various signals.
+ sysinfo.c: contains basic system details like username etc. and other system related utils.

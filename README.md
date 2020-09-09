# myShell

A simple shell written in basic C language and syscalls for linux OS only.

## Build and Usage

+ **Build** :  
  Just run `make` to build the executable binary, i.e. `mysh`.

+ **Run**:  
  run `./mysh` to use myShell.

> History can be found in `.mysh_history` file, in the directory from where `mysh` was executed.

## Shell features

+ myShell provides some basic shell builtins which are as follows:  
  
  + `quit` : just quits the shell. (recommended way of leaving the shell).
  + `cd [path]`: change current working directory to `path`. If no arg provided then it `cd` to the initial directory of shell.
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

+ bgprocess.c : Functions related to making a process background.
+ builtins.c : contains functions to execute any bultin command.
+ cd.c : builtins `cd` and `pwd` implemented in this.
+ command.c: Functios useful for command's text processing.
+ echo.c : `echo` implementation.
+ format.c: useful for string formatting.
+ history.c : history related implementations.
+ ls.c : `ls` implementation.
+ nightswatch.c : `nightswatch` implementtaion.
+ pinfo.c : `pinfo` implementation.
+ prompt.c : func to display command prompt.
+ quit.c : `quit` implementation.
+ readexec.c: reads a line and executes constituent command(s).
+ shell.c : contains `main()` and shell's REPL loop.
+ signal.c: for handling various signals from  background processes.
+ sysinfo.c: contains basic system details like username etc. and other system related utils.

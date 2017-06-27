# command-line-interpreter-Shell-
implement a command line interpreter


#Description : 
A Unix shell is a command-line interpreter that provides a traditional user interface for the Unix
operating system and for Unix-like systems. The shell can run in two modes: interactive and batch.
In the shell interactive mode, you start the shell program, which displays a prompt (e.g. Shell>)
and the user of the shell types commands at the prompt. Your shell program executes each of these
commands and terminates when the user enters the exit command at the prompt. In the shell batch
mode, you start the shell by calling your shell program and specifying a batch file to execute the
commands included in it. This batch file contains the list of commands (on separate lines) that the
user wants to execute. In batch mode, you should not display a prompt, however, you will need to
echo each line you read from the batch file (print it) before executing it. This feature in your program
is to help debugging and testing your code. Your shell terminates when the end of the batch file is
reached or the user types Ctrl-D.


# how to compile and run the code :

- Open the terminal and go to the project directory .
- write ./ shell if you want to run interactive mode
or ./ shell fle . txt if you want to run batch mode .
Or use Makefle .

#include "shcore.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/wait.h>
#include <errno.h>


//Checks if its a built in command (cd, pwd, which, die, exit) returns 1 if it is and 0 otherwise
int is_a_builtin(Commands *commands) {
    
}

//actual logic for the built in commands 
int builtin_executor(Commands *command) {
  
}


//for wildcard *
void wildcard() {
  
}

//redirection logic (use dup2())
static void redirection() {
   
}

//sets up the pipeline between 2 processes
static void pipeline() {

}


//executes external commands using fork() and execv() (like ls, cat, echo, etc), also calls the redirction and pipeline functions
int command_executor(Commands *command) {
    
}

//frees all allocated memory from the struct and everything else
void free_commands(Commands *command) {

}

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
#include <glob.h>


//Checks if its a built in command (cd, pwd, which, die, exit) returns 1 if it is and 0 otherwise
int is_a_builtin(Commands *commands) {
    
}

//actual logic for the built in commands 
int builtin_executor(Commands *command) {
  
}


//for wildcard *
void wildcard(Commands *commands, const char *token) {
    glob_t res;

    if (glob(token, GLOB_NOCHECK, NULL, &res) != 0) {
        commands->args[commands->argc++] = strdup(token);
    }

    size_t unchanged_argc = commands->argc;

    for (size_t i = 0; i < res.gl_pathc; i++) {
        const char *slash = strchr(res.gl_pathv[i], "/");
        const char *file;

        if (slash) {
            file = slash + 1;
        } else {
            file = res.gl_pathv[i];
        }

        if (file[0] == "." && token[0] != ".") {
            continue;
        }

        commands->args[commands->argc++] = strdup(res.gl_pathv[i]);
    }

    if (commands->argc == unchanged_argc) {
        commands->args[commands->argc++] = strdup(token);
    }

    globfree(&res);
  
}

//redirection logic (use dup2())
static void redirection(Commands *commands) {
    if (commands->input) {
        int fd;
        if ((fd = open(commands->input, O_RDONLY, 0)) < 0) {
            perror("Error: couldn't open input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (commands->output) {
        int fd;
        if ((fd = open(commands->output, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
            perror("Error: couldn't open output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
   
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

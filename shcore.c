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

int cd(Commands *commands);
int pwd(Commands *commands);
int which(Commands *commands);
int exit_shell();
int die();
int num_builtins();


char *builtin_str[] = {
    "cd",
    "pwd",
    "which",
    "exit",
    "die"
  };

  int (*builtin_func[]) (Commands *) = {
    &cd,
    &pwd,
    &which,
    &exit_shell,
    &die
  };


//Checks if its a built in command (cd, pwd, which, die, exit) returns 1 if it is and 0 otherwise
int is_a_builtin(Commands *commands) {
    int builtin=0;

    for(int i = 0; i < num_builtins();i++){
        if(strcmp(commands->args[0],builtin_str[i])==0){
            builtin=1;
        }
    }

    return builtin;
}

int num_builtins(){
    return sizeof(builtin_str)/sizeof(char*);
}

//actual logic for the built in commands 
int builtin_executor(Commands *commands) {
  if(commands->args[0]==NULL){
    printf("empty command");
    return 1;
  }
  for (int i = 0; i < num_builtins(); i++) {
    if (strcmp(commands->args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(commands);
    }
  }
  return 1;
}

int cd(Commands *commands){
    char *path = commands->args[1];
    if (commands->argc != 2) {
           fprintf(stderr, "Cd: expected 1 argument\n");
           return 1;
       }
    if (path==NULL) {
        path=getenv("HOME");
        if (path==NULL) {
            return 1;
        }
    }
    if (chdir(path)!=0) {
        perror("Error: cd failed");
        return 1;
    }
    return 0;
}
int pwd(Commands *commands){
    size_t dirsize=pathconf(".", _PC_PATH_MAX);
    char *cwd = malloc(dirsize);
    if(getcwd(cwd,dirsize) == NULL){
        printf("pwd failed");
        free(cwd);
        return 1;
    }
    printf("%s\n",cwd);
    free(cwd);
    return 0;
}

int which(Commands *commands){
    if (commands->args[1] == NULL) {
        printf("Error: missing command name\n");
        return 1;
    }
    const char *directories[] = {"/usr/local/bin", "/usr/bin", "/bin"}; 

    for (int i = 0; i < 3; i++) {
        char fullpath[1024];
        strcpy(fullpath, directories[i]);
        strcat(fullpath, "/");
        strcat(fullpath, commands->args[1]);

        if (access(fullpath, F_OK) == 0) {
            printf("%s\n",fullpath);
            return 0;
        }
    }
    return 1;
}

int exit_shell(){
    printf("Exiting, Goodbye\n");
    exit(0);
}

int die(Commands *commands){
    for (int i = 1; i < commands->argc; i++) fprintf(stderr, "%s ", commands->args[i]);
        fprintf(stderr, "\n");
        exit(1);
}

//for wildcard *
void wildcard(Commands *commands, const char *token) {
    glob_t res;

    if (glob(token, GLOB_NOCHECK, NULL, &res) != 0) {
        commands->args[commands->argc++] = strdup(token);
    }

    size_t unchanged_argc = commands->argc;

    for (size_t i = 0; i < res.gl_pathc; i++) {
        const char *slash = strchr(res.gl_pathv[i], '/');
        const char *file;

        if (slash) {
            file = slash + 1;
        } else {
            file = res.gl_pathv[i];
        }

        if (file[0] == '.' && token[0] != '.') {
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
        if ((fd = open(commands->output, O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
            perror("Error: couldn't open output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
   
}

//for the external commands (ls, echo, cat, etc )
void external_commands(Commands *commands) {
    if (strchr(commands->args[0], '/')) {
        if (execv(commands->args[0], commands->args) == -1) {
            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
    } else {
        const char *directories[] = {"/usr/local/bin", "/usr/bin", "/bin"}; 

        for (int i = 0; i < 3; i++) {
            char fullpath[1024];
            strcpy(fullpath, directories[i]);
            strcat(fullpath, "/");
            strcat(fullpath, commands->args[0]);

            //fprintf(stderr, "Trying: %s\n", fullpath);
            execv(fullpath, commands->args);
        }
        perror("Error: execv failed");
        exit(EXIT_FAILURE);
    }
}

//sets up the pipeline between 2 processes
static int pipeline(Commands *leftchild, Commands *rightchild) {
    int p[2];
    if (pipe(p) == -1) {
        perror("Error: pipe failed");
        exit(EXIT_FAILURE);
    }

    //left command
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Error: fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        close(p[1]);
        redirection(leftchild);
        external_commands(leftchild);
        exit(EXIT_FAILURE);
    }

    //right command
    pid_t pid2;
    pid2 = fork();
    if (pid2 < 0) {
        perror("Error: fork failed");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        close(p[1]);
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        redirection(rightchild);
        external_commands(rightchild);
        exit(EXIT_FAILURE);
    }

    //close parent
    close(p[0]);
    close(p[1]);
    int status;
    waitpid(pid2, &status, 0);
    waitpid(pid, NULL, 0);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        return EXIT_FAILURE;  
    }

}

//executes external commands by calling external_commands function, also calls the redirction and pipeline functions
int command_executor(Commands *commands) {
    //fprintf(stderr, "command_executor: got command '%s'\n", commands->args[0]);
    if (!commands || commands->argc == 0) {
        return EXIT_FAILURE;
    }

    if (commands->pipe && commands->next) {
        return pipeline(commands, commands->next);
    }

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Error: pipe failed");
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        redirection(commands);
        external_commands(commands);
        exit(EXIT_FAILURE); // only runs if execv fails!
    }

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        return EXIT_FAILURE;  
    }
    
}

//frees all allocated memory from the struct and everything else
void free_commands(Commands *commands) {
    if (!commands) {
        return;
    }  
    
    for (int i = 0; i < commands->argc; i++) {
        free(commands->args[i]);
    }

    free(commands->args);

    if (commands->input) {
        free(commands->input);
    }
    if (commands->output) {
        free(commands->output);
    }

    if (commands->next) {
        free_commands(commands->next);
    }

    free(commands);

}

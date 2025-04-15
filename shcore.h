#ifndef SHCORE_H
#define SHCORE_H

typedef enum {
    AND, OR, NONE
} Conditionals;

typedef struct Commands {
    char **args;
    int argc;
    char *input;
    char *output;
    int pipe;
    Conditionals condition;
    struct Commands *next;
} Commands;

int is_a_builtin(Commands *commands);
int builtin_executor(Commands *commands);

void wildcard();
int command_executor(Commands *command);
void free_commands(Commands *command);

extern int batch_mode_flag;


#endif

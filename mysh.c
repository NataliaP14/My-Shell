#include "shcore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX_LINE 2048


static int status = 0;
int batch_mode_flag = 0;


typedef struct {
    int fd;
    int pos;
    int length;
    char buffer[BUFFER_SIZE];
    int newline;
    int close;

} Input;

//initialization for the input struct
static void initialize(Input *input, int fd) {
   input->fd = fd;
   input->pos = 0;
   input->length = 0;
   input->newline = 0;
   input->close = 0;

}

//reads character from input buffer
static char char_reader(Input *input) {
    if (input->pos >= input->length) {
        ssize_t bytes = read(input->fd, input->buffer, BUFFER_SIZE);
        if (bytes <= 0) {
            input->close = 1;
            return '\0';
        }
        input->length = bytes;
        input->pos = 0;
    }

    return input->buffer[input->pos++];
}

//gets symbol like < > and | as input handles whitespace, etc
static char *tokenizer(Input *input) {
    char token[BUFFER_SIZE];
    int i = 0;
    char current;

    while (1) {
        current = char_reader(input);
        if (input->close) return NULL;

        if (current == '\n') {
            input->newline = 1;
            token[0] = '\n';
            token[1] = '\0';
            return strdup(token);
        }
        if (isspace(current)) continue;

        if (current == '<' || current == '>' || current == '|') {
            token[0] = current;
            token[1] = '\0';
            return strdup(token);
        }
        break;
    }

    input->newline = 0;

    while (current != '\0' && !isspace(current) && current != '<' && current != '>' && current != '|' && current != '\n') {
        token[i++] = current;
        current = char_reader(input);
    }

    if (current != '\0') {
        if (current == '\n') {
            input->newline = 1;
        } else {
            input->pos--;
        }
    }

    token[i] = '\0';
    return strdup(token);
}

//reads entire line and builds it from the tokens, stops reading when complete command is done
static char *line_reader(Input *input) {
    static char line[MAX_LINE];
    int i = 0;
    line[0] = '\0';

    while (1) {
        char *token = tokenizer(input);
        if (!token)  {
            break;
        }

        if (token[0] == '#' && token[1] == '\0') {
            free(token);
            while (!input->newline && !input->close) {
                char *comment = tokenizer(input);
                if (comment) {
                    free(comment);
                }
            }
            break;
        }

        int length = strlen(token);
        if (i + length + 2 >= MAX_LINE) {
            free(token);
            break;
        }

        strcpy(&line[i], token);
        i += length;
        line[i++] = ' ';
        line[i] = '\0';

        if (input->newline) {
            free(token);
            break;
        }

        free(token);
    }

    if (i == 0 && input->close) return NULL;
    if (i == 0) return strdup("");
    return strdup(line);
}

char *condition_handler(Commands *commands, char *token) {
    if (token != NULL && strcmp(token, "and") == 0) {
        commands->condition = AND;
        return strtok(NULL, " \t\n");
    } else if (token != NULL && strcmp(token, "or") == 0) {
        commands->condition = OR;
        return strtok(NULL, " \t\n");
    } else {
        commands->condition = NONE;
        return token;
    }
}

void print_debug(Commands *commands) {
    while (commands) {
        printf("\nParsed command: %s\n", commands->args[0]);
        for (int i = 1; i < commands->argc; i++) {
            printf("Arg %d: %s\n", i, commands->args[i]);
        }
        if (commands->input) printf("Input redirection: %s\n", commands->input);
        if (commands->output) printf("Output redirection: %s\n", commands->output);
        if (commands->pipe) printf("This is a piped command\n");

        if (commands->condition == AND) printf("Condition: AND\n");
        else if (commands->condition == OR) printf("Condition: OR\n");
        else printf("Condition: NONE\n");

        commands = commands->next;
    }
}

static void flush(Input *input) {
    while (!input->newline && !input->close) {
        char c = char_reader(input);
        if (c == '\n') {
            input->newline = 1; 
            break;
        }
    }
    input->newline = 0;

}

//parses command into the struct, and extracts any arguments entered from the user (pipes, conditions, redirection, wildcards)
Commands *command_parser(char *line, Input *input) {
    if (line == NULL) {
        return NULL;
    }

    Commands *commands = calloc(1, sizeof(Commands));
    if (commands == NULL) {
        return NULL;
    }

    commands->args = malloc(sizeof(char *) * 128);
    if(commands->args == NULL) {
        free(commands);
        return NULL;
    }
    commands->argc = 0;

    char *token = strtok(line, " \t\n");
    // and - or
    token = condition_handler(commands, token);

    while (token != NULL) {
        //wilcards
        if (strchr(token, '*') != NULL) {
            wildcard(commands, token);

        } 
        //redirction
        else if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0) {
            char *file = strtok(NULL, " \t\n");

            if (!file) {
                fprintf(stderr, "Error: missing or invalid filename after '%s'\n", token);
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            if (token[0] == '<') {
                if (commands->input != NULL) {
                    fprintf(stderr, "Error: multiple input redirections\n");
                    if (input != NULL) { flush(input); }
                    free_commands(commands);
                    return NULL;
                }
                commands->input = strdup(file);
            } else {
                if (commands->output != NULL) {
                    fprintf(stderr, "Error: multiple output redirections\n");
                    if (input != NULL) { flush(input); }
                    free_commands(commands);
                    return NULL;
                }
                commands->output = strdup(file);
            }
        }

        //piping
        else if (strcmp(token, "|") == 0) {
            if (commands->pipe) {
                fprintf(stderr, "Error: only one pipe allowed\n");
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            if (commands->argc == 0) {
                fprintf(stderr, "Error: missing command before pipe\n");
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            commands->pipe = 1;

            token = strtok(NULL, "");
            if (!token || strlen(token) == 0) {
                fprintf(stderr, "Error: missing command after pipe\n");
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            if (strchr(token, '|')) {
                fprintf(stderr, "Error: only one pipe allowed\n");
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            commands->next = command_parser(token, NULL);
            if (!commands->next) {
                fprintf(stderr, "Error: invalid pipeline target\n");
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            if (commands->next->input || commands->next->output) {
                fprintf(stderr, "Error: redirection not allowed in pipeline\n");
                if (input != NULL) { flush(input); }
                free_commands(commands);
                return NULL;
            }

            break;
        }
        //other args that are valid
        else {
            if ((token[0] == '"' && token[strlen(token) - 1] == '"') ||
                (token[0] == '\'' && token[strlen(token) - 1] == '\'')) {
                    char *clean = strndup(token + 1, strlen(token) - 2);
                    commands->args[commands->argc++] = clean;
            } else {
                commands->args[commands->argc++] = strdup(token);
            }
        }


        token = strtok(NULL, " \t\n");
    }

    commands->args[commands->argc] = NULL;

    if (commands->argc == 0) {
        free_commands(commands);
        return NULL;
    }

    return commands;
}

static void interactive_mode() {
    Input input; initialize(&input, STDIN_FILENO);
    printf("Welcome to my shell!\n");

    while (1) {
        char *line = NULL;
        printf("mysh> ");
        fflush(stdout);
        line = line_reader(&input);
        
        if (!line) {
            printf("Exiting, Goodbye\n");
            return;
        }

        if (line[0] == '\0') {
            free(line);
            continue; 
        }

        Commands *commands = command_parser(line, &input);
        free(line);  

        if (!commands) {
            continue;  
        }

        //print_debug(commands);

        int prev_failed = (status != 0);
        int prev_succeeded = (status == 0);

        if ((commands->condition == AND && !prev_succeeded) || (commands->condition == OR && !prev_failed)) {
            free_commands(commands);
            continue;
        }

        if (is_a_builtin(commands)) {
            status = builtin_executor(commands);
            free_commands(commands);
            continue;
        }

        status = command_executor(commands);
        free_commands(commands);
    }

    printf("Exiting, Goodbye\n");

}

static void batch_mode(int fd) {
    Input input; initialize(&input, fd);

    while (1) {
        char *line = line_reader(&input);

        if (!line) {
            break;
        }

        Commands *commands = command_parser(line, &input);
        free(line);
        if (!commands) {
            continue;
        }

        int prev_failed = (status != 0);
        int prev_succeeded = (status == 0);

        if ((commands->condition == AND && !prev_succeeded) || (commands->condition == OR && !prev_failed)) {
            free_commands(commands);
            continue;
        }

        if (is_a_builtin(commands)) {
            status = builtin_executor(commands);
            free_commands(commands);
            continue;
        }

        status = command_executor(commands);
        free_commands(commands);
        
    }
}

int main(int argc, char *argv[]) {
    FILE *file = NULL;

    if (argc == 1) {
        if (isatty(STDIN_FILENO)) {
            batch_mode_flag = 0;
            interactive_mode();
        } else {
            batch_mode_flag = 1;
            batch_mode(STDIN_FILENO);
        }
    } else if (argc == 2) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("Error: can't open file");
            exit(EXIT_FAILURE);
        }
        batch_mode_flag = 1;
        batch_mode(fileno(file));
        fclose(file);
    } else {
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}

#include "shcore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX_LINE 2048

static int status = 0;

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
    int i = 0; char current;

    while (1) {
        current = char_reader(input);
        if (input->close) {
            return NULL;
        }
        if (current == '\n') {
            input->newline = 1;
            token[0] = '\n';
            token[1] = '\0';
            return strdup(token);
        }
        if (!isspace(current)) {
            break;
        }
    }

    input->newline = 0;

    while (current != '\0' && !isspace(current) && current != '<' && current != '>' && current != '|' && current != '\n') {
        token[i++] = current;
        current = char_reader(input);

        if (isspace(current) || current == '<' || current == '>' || current == '|' || current == '\n') {
            input->pos--;
            break;
        }
    }

    if (current == '<' || current == '>' || current == '|') {
        token[0] = current;
        token[1] = '\0';
        return strdup(token);
    }

    token[i] = '\0';
    return strdup(token);
}

//reads entire line and builds it from the tokens, stops reading when complete command is done
static char *line_reader(Input *input) {
    static char line[MAX_LINE];
    line[0] = '\0';

    char *token;
    while ((token = tokenizer(input)) != NULL) {
        if (token[0] == '#' && token[1] == '\0') {
            while (!input->newline && !input->close) {
                free(token);
                token = tokenizer(input);
            }
            free(token);
            break;
        }

        strcat(line, token);
        strcat(line, " ");
        int end = input->newline;
        free(token);
        if (end) {
            break;
        }
    }

    if (strlen(line) == 0 && input->close) {
        return NULL;
    }
    return line;
}

//parses command into the struct, and extracts any arguments entered from the user (pipes, conditions, redirection)
Commands *command_parser(char *line) {
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

    if (token != NULL && strcmp(token, "and") == 0) {
        commands->condition = AND;
        token = strtok(NULL, " \t\n");
    } else if (token != NULL && strcmp(token, "or") == 0) {
        commands->condition = OR;
        token = strtok(NULL, " \t\n");
    } else {
        commands->condition = NONE;
    }

    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                commands->input = strdup(token);
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                commands->output = strdup(token);
            }
        } else if (strcmp(token, "|") == 0) {
            commands->pipe = 1;
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                commands->next = command_parser(token);
            }
            break;
        } else {
            if (strchr(token, '*') != NULL) {
                wildcard(commands, token);
            } else {
                commands->args[commands->argc] = strdup(token);
                commands->argc++;
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

}

static void batch_mode(int fd) {

}

int main(int argc, char *argv[]) {
    FILE *file = NULL;

    if (argc < 2) {
        exit(EXIT_FAILURE);
    }

    //batch mode
    if (argc == 2) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("Error: cant open file");
            exit(EXIT_FAILURE);
        }
        batch_mode(fileno(file));
        fclose(file);

    } else {
        if (isatty(STDIN_FILENO)) {
            interactive_mode();
        } else {
            batch_mode(STDIN_FILENO);
        }
    }

    
    return EXIT_SUCCESS;
}

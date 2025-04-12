#include "shcore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX_LINE 2048

int status = 0;

//initialization for the tokenizr
static void initialize(int fd) {
   
}

//reads character from input buffer
static char char_reader() {
    
}

//gets symbol like < > and | as input handles whitespace, etc
static char *tokenizer() {
    
}

//reads entire line and builds it from the tokens, stops reading when complete command is done
static char *line_reader() {
    
}

//parses command into the struct, and extracts any arguments entered from the user (pipes, conditions, redirection)
Commands *command_parser() {
    
}

int main(int argc, char *argv[]) {
    
    return EXIT_SUCCESS;
}

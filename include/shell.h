#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_INPUT     1024
#define MAX_ARGS      64
#define MAX_PIPES     16
#define HISTORY_SIZE  100

typedef struct {
    char *args[MAX_ARGS];
    char *input_file;
    char *output_file;
    int   append;
    int   background;
} Command;

typedef struct {
    Command commands[MAX_PIPES];
    int     num_commands;
} Pipeline;

void print_prompt(void);
void handle_sigint(int sig);

int  parse_input(char *input, Pipeline *pipeline);
void free_pipeline(Pipeline *pipeline);

int  is_builtin(char *cmd);
int  run_builtin(Command *cmd);

void execute_pipeline(Pipeline *pipeline);
void execute_single(Command *cmd);

void history_add(const char *line);
void history_print(void);
char *history_get(int index);

#endif
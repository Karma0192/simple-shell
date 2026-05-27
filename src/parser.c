#include "shell.h"

static void init_command(Command *cmd)
{
    for (int i = 0; i < MAX_ARGS; i++)
        cmd->args[i] = NULL;
    cmd->input_file  = NULL;
    cmd->output_file = NULL;
    cmd->append      = 0;
    cmd->background  = 0;
}

int parse_input(char *input, Pipeline *pipeline)
{
    pipeline->num_commands = 0;

    char *pipe_segments[MAX_PIPES];
    int   num_segments = 0;

    char *input_copy = strdup(input);
    if (!input_copy) {
        perror("strdup");
        return -1;
    }

    char *seg = strtok(input_copy, "|");
    while (seg && num_segments < MAX_PIPES) {
        pipe_segments[num_segments++] = seg;
        seg = strtok(NULL, "|");
    }

    for (int i = 0; i < num_segments; i++) {
        Command *cmd = &pipeline->commands[i];
        init_command(cmd);

        char *seg_copy = strdup(pipe_segments[i]);
        if (!seg_copy) {
            perror("strdup");
            free(input_copy);
            return -1;
        }

        char *token = strtok(seg_copy, " \t");
        int   argc  = 0;

        while (token) {
            if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " \t");
                if (token)
                    cmd->input_file = strdup(token);
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " \t");
                if (token) {
                    cmd->output_file = strdup(token);
                    cmd->append      = 0;
                }
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(NULL, " \t");
                if (token) {
                    cmd->output_file = strdup(token);
                    cmd->append      = 1;
                }
            } else if (strcmp(token, "&") == 0) {
                cmd->background = 1;
            } else {
                if (argc < MAX_ARGS - 1)
                    cmd->args[argc++] = strdup(token);
            }
            token = strtok(NULL, " \t");
        }

        cmd->args[argc] = NULL;
        free(seg_copy);
        pipeline->num_commands++;
    }

    free(input_copy);
    return 0;
}

void free_pipeline(Pipeline *pipeline)
{
    for (int i = 0; i < pipeline->num_commands; i++) {
        Command *cmd = &pipeline->commands[i];
        for (int j = 0; cmd->args[j]; j++) {
            free(cmd->args[j]);
            cmd->args[j] = NULL;
        }
        if (cmd->input_file)  { free(cmd->input_file);  cmd->input_file  = NULL; }
        if (cmd->output_file) { free(cmd->output_file); cmd->output_file = NULL; }
    }
    pipeline->num_commands = 0;
}
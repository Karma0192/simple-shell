#include "shell.h"

static char *history[HISTORY_SIZE];
static int   history_count = 0;

void history_add(const char *line)
{
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(line);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++)
            history[i - 1] = history[i];
        history[HISTORY_SIZE - 1] = strdup(line);
    }
}

void history_print(void)
{
    for (int i = 0; i < history_count; i++)
        printf("  %d  %s\n", i + 1, history[i]);
}

char *history_get(int index)
{
    if (index < 1 || index > history_count)
        return NULL;
    return history[index - 1];
}

int is_builtin(char *cmd)
{
    if (!cmd) return 0;
    return (strcmp(cmd, "cd")      == 0 ||
            strcmp(cmd, "exit")    == 0 ||
            strcmp(cmd, "help")    == 0 ||
            strcmp(cmd, "history") == 0);
}

int run_builtin(Command *cmd)
{
    if (strcmp(cmd->args[0], "cd") == 0) {
        static char oldpwd[1024] = "";
        char cwd[1024];
        char *path = cmd->args[1];

        getcwd(cwd, sizeof(cwd));

        if (!path) {
            path = getenv("HOME");
        } else if (strcmp(path, "-") == 0) {
            if (strlen(oldpwd) == 0) {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return 0;
            }
            printf("%s\n", oldpwd);
            path = oldpwd;
        }

        if (chdir(path) < 0)
            perror("cd");
        else
            strcpy(oldpwd, cwd);

        return 0;
    }

    if (strcmp(cmd->args[0], "exit") == 0) {
        int code = 0;
        if (cmd->args[1])
            code = atoi(cmd->args[1]);
        printf("Goodbye!\n");
        exit(code);
    }

    if (strcmp(cmd->args[0], "history") == 0) {
        history_print();
        return 0;
    }

    if (strcmp(cmd->args[0], "help") == 0) {
        printf("\n  mysh — a simple Unix shell\n\n");
        printf("  Built-in commands:\n");
        printf("    cd [dir]     Change directory (default: HOME)\n");
        printf("    history      Show command history\n");
        printf("    help         Show this help message\n");
        printf("    exit [code]  Exit the shell\n\n");
        printf("  Supported features:\n");
        printf("    cmd | cmd    Piping between commands\n");
        printf("    cmd > file   Redirect output to file\n");
        printf("    cmd >> file  Append output to file\n");
        printf("    cmd < file   Redirect input from file\n");
        printf("    cmd &        Run command in background\n\n");
        return 0;
    }

    return -1;
}
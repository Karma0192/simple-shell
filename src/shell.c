#include "shell.h"

void handle_sigint(int sig)
{
    (void)sig;
    printf("\n");
    print_prompt();
    fflush(stdout);
}

void print_prompt(void)
{
    char host[256];
    char cwd[1024];

    gethostname(host, sizeof(host));
    getcwd(cwd, sizeof(cwd));

    char *user = getenv("USER");
    if (!user) user = "user";

    printf("\033[1;32m%s@%s\033[0m:\033[1;34m%s\033[0m$ ", user, host, cwd);
    fflush(stdout);
}

static void setup_redirections(Command *cmd)
{
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            perror("open input");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd    = open(cmd->output_file, flags, 0644);
        if (fd < 0) {
            perror("open output");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void execute_single(Command *cmd)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        setup_redirections(cmd);
        execvp(cmd->args[0], cmd->args);
        fprintf(stderr, "mysh: command not found: %s\n", cmd->args[0]);
        exit(127);
    }

    if (!cmd->background)
        waitpid(pid, NULL, 0);
    else
        printf("[bg] pid %d\n", pid);
}

void execute_pipeline(Pipeline *pipeline)
{
    if (pipeline->num_commands == 1) {
        execute_single(&pipeline->commands[0]);
        return;
    }

    int   num_cmds = pipeline->num_commands;
    int   pipes[MAX_PIPES - 1][2];
    pid_t pids[MAX_PIPES];

    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return;
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            return;
        }

        if (pids[i] == 0) {
            signal(SIGINT, SIG_DFL);

            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);

            if (i < num_cmds - 1)
                dup2(pipes[i][1], STDOUT_FILENO);

            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            setup_redirections(&pipeline->commands[i]);
            execvp(pipeline->commands[i].args[0], pipeline->commands[i].args);
            fprintf(stderr, "mysh: command not found: %s\n", pipeline->commands[i].args[0]);
            exit(127);
        }
    }

    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_cmds; i++)
        waitpid(pids[i], NULL, 0);
}
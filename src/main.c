#include "shell.h"

int main(void)
{
    char     input[MAX_INPUT];
    Pipeline pipeline;

    signal(SIGINT, handle_sigint);

    while (1) {
        print_prompt();

        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        history_add(input);

        if (parse_input(input, &pipeline) < 0)
            continue;

        if (pipeline.num_commands == 0)
            continue;

        if (is_builtin(pipeline.commands[0].args[0]))
            run_builtin(&pipeline.commands[0]);
        else
            execute_pipeline(&pipeline);

        free_pipeline(&pipeline);
    }

    return 0;
}
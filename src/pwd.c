#include "minishell.h"

void pwd(t_cmd *cmd)
{
    char cwd[4096];
    (void)cmd;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd");
    }
}

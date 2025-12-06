#include "minishell.h"

int process_heredoc(t_cmd *cmd, t_env *env)
{
    int     fd[2];
    char    *line;
    char    *expanded;
    int     i;

    i = 0;
    while (i < cmd->heredoc_count)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            while (--i >= 0)
                if (cmd->heredoc_fds[i] != -1)
                    close(cmd->heredoc_fds[i]);
            return (-1);
        }
        while (1)
        {
            line = readline("> ");
            if (!line)
                break;

            if (ft_strcmp(line, cmd->heredoc_delims[i]) == 0)
            {
                free(line);
                break;
            }

            if (cmd->heredoc_expands[i] == 1)
                expanded = expand_str(line, env);
            else
                expanded = ft_strdup(line);

            if (!expanded)
                expanded = ft_strdup("");

            write(fd[1], expanded, ft_strlen(expanded));
            write(fd[1], "\n", 1);

            free(line);
            free(expanded);
        }

        close(fd[1]);

        if (cmd->heredoc_fds[i] != -1)
            close(cmd->heredoc_fds[i]);

        cmd->heredoc_fds[i] = fd[0];
        i++;
    }
    return (0);
}

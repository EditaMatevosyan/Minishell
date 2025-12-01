#include "minishell.h"

int	process_heredoc(t_cmd *cmd, t_env *env)
{
	int	fd[2];
	char *line;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	signal(SIGINT, SIG_DFL);            //default behavior of the Ctrl+C -> termonate the program

	while (1)
	{
		line = readline("> ");         //after the delimiter we want to get input
		if (!line)       //when we press ctrl d to stop heredoc input , readline automatically returns NULL
			break;
		if (ft_strcmp(line, cmd->heredoc_delim) == 0)    //when it sees the delimiter again, breaks
        {
            free(line);
            break;
        }
	}
}
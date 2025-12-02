#include "minishell.h"

int	process_heredoc(t_cmd *cmd, t_env *env)
{
	int		fd[2];
	char	*line;
	char	*expanded;

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
	if(cmd->heredoc_expand == 1)
		expanded = expand_str(line, env);
	else
		expanded = ft_strdup(line);
	write(fd[1], expanded, ft_strlen(expanded));
	write(fd[1], "\n", 1);
	free(line);
    free(expanded);
	}
	close(fd[1]);       //closing the write end becasue we are not going to use it again
	cmd->heredoc_fd = fd[0];       //the command will read the input from here
	return (0);
}
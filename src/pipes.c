/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:47 by romargar          #+#    #+#             */
/*   Updated: 2025/12/11 12:29:03 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_cmd	*cmd_list)
{
	int	count;
	t_cmd	*cur;
	
	count = 0;
	cur = cmd_list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

int	**create_pipes(int n)
{
	int		**fds;
	int		i;

	if (n <= 1)
        return (NULL);
	fds = malloc(sizeof(int *) * (n - 1));
	if (!fds)
		return (NULL);
	i = 0;
	while (i < n - 1)
	{
		fds[i] = malloc(sizeof(int) * 2);      //allocate for each pipe array fd[2]
		if (!fds[i])
			return (NULL);
		if (pipe(fds[i]) == -1)
			return (NULL);
		i++;
	}
	return (fds);
}

static void	free_pipes(int **fds, int n)
{
	int	i;

	if (!fds)
		return ;
	i = 0;
	while (i < n - 1)
	{
		if (fds[i])
			free(fds[i]);
		i++;
	}
	free(fds);
}

void	close_fds(int	**fds, int n)
{
	int	i;
	
	i = 0;
	while (i < n - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

void	setup_fds(t_cmd *cmds, int i, int n, int **fds)
{
	int	last_heredoc;

	if (i > 0)       //if its not the first command, change the stdin/stdout
		dup2(fds[i - 1][0], STDIN_FILENO);
	if (i < n - 1)         //if its not the last command
        dup2(fds[i][1], STDOUT_FILENO);
	if (cmds->heredoc_count > 0)
	{
    	last_heredoc = cmds->heredoc_count - 1;
    	if (cmds->heredoc_fds &&
        cmds->heredoc_fds[last_heredoc] != -1)
    	{
        	dup2(cmds->heredoc_fds[last_heredoc], STDIN_FILENO);
        	for (int k = 0; k < cmds->heredoc_count; k++)
    		{
        		if (cmds->heredoc_fds[k] != -1)
            		close(cmds->heredoc_fds[k]);
    		}
    	}
	}
	close_fds(fds, n);
	change_stdin(cmds);
    change_stdout(cmds);
}

int process_all_heredocs(t_cmd *cmd_list, t_minishell *ms)
{
    t_cmd *cur = cmd_list;

    while (cur)
    {
        if (cur->heredoc_count > 0)
        {
            if (process_heredoc(cur, ms->env) == -1)
			{
                return (-1);
			}
        }
        cur = cur->next;
    }
    return 0;
}

int fork_and_execute(t_cmd *cmd_list, t_minishell *ms, int **fds, int n)
{
    t_cmd *cur = cmd_list;
    int i = 0;
    pid_t pid;
    char **envp;
    char *path;

    while (cur)
    {
        pid = fork();
        if (pid < 0)
            return -1;

        if (pid == 0)
        {
            setup_fds(cur, i, n, fds);

            // ------------built-in logic here-------------

            envp = env_list_to_array(ms->env);
            path = get_full_path(cur, ms->env);
            if (!path)
            {
                fprintf(stderr, "minishell: %s: command not found\n", cur->argv[0]);
                free_env_array(envp);
                close(STDIN_FILENO);
                close(STDOUT_FILENO);
                close(STDERR_FILENO);
                cleanup(cur, ms);
                exit(127);
            }
            execve(path, cur->argv, envp);
            perror("execve");
            free_env_array(envp);
            free(path);
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            cleanup(cur, ms);
            exit(126);
        }
        cur = cur->next;
        i++;
    }
    return 0;
}

void close_all_heredoc_fds(t_cmd *cmd_list)
{
    t_cmd *cur = cmd_list;
    int j;

    while (cur)
    {
        if (cur->heredoc_fds)
        {
            j = 0;
            while (j < cur->heredoc_count)
            {
                if (cur->heredoc_fds[j] != -1)
                {
                    close(cur->heredoc_fds[j]);
                    cur->heredoc_fds[j] = -1;
                }
                j++;
            }
        }
        cur = cur->next;
    }
}

int execute_pipeline(t_cmd *cmd_list, t_minishell *ms)
{
    int n;
    int **fds;
	int	i;
	t_cmd *cur;

    n = count_commands(cmd_list);
    fds = create_pipes(n);
    if (n > 1 && !fds)
        return (-1);
    if (process_all_heredocs(cmd_list, ms) == -1)
	{
        if (fds)
		{
			close_fds(fds, n);
            free_pipes(fds, n);
		}
        close_all_heredoc_fds(cmd_list);
    	return (-1);
	}
	if (fork_and_execute(cmd_list, ms, fds, n) == -1)
	{
        if (fds)
		{
			close_fds(fds, n);
            free_pipes(fds, n);
		}
        close_all_heredoc_fds(cmd_list);
    	return (-1);
	}
	close_fds(fds, n);
	if (fds)
    {
		free_pipes(fds, n);
        fds = NULL;
    }
	i = 0;
	while (i < n - 1)
	{
		i++;
	}
	cur = cmd_list;
    close_all_heredoc_fds(cmd_list);
    n = count_commands(cmd_list);
    while (n > 0)
    {
        wait(NULL);
        n--;
    }
    return (0);
}

#include "minishell.h"

extern int g_exit_status;            //it is defined in another .c file, and i want to use it in this .c file, thats why it is written extern

int is_builtin(t_cmd *cmd)
{
    if (!cmd->argv || !cmd->argv[0])
	{
        return (0);
	}
	
    return (!ft_strcmp(cmd->argv[0], "echo")          //returns 0 if built-in, 1 if no
        || !ft_strcmp(cmd->argv[0], "cd")
        || !ft_strcmp(cmd->argv[0], "pwd")
        || !ft_strcmp(cmd->argv[0], "export")
        || !ft_strcmp(cmd->argv[0], "unset")
        || !ft_strcmp(cmd->argv[0], "env")
        || !ft_strcmp(cmd->argv[0], "exit"));
}

// 



char **env_list_to_array(t_env *env)
{
    int len = 0;
    t_env *tmp = env;
    while (tmp)
    {
        len++;
        tmp = tmp->next;
    }

    char **arr = malloc(sizeof(char *) * (len + 1));
    if (!arr)
        return NULL;

    tmp = env;
    for (int i = 0; i < len; i++)
    {
        arr[i] = ft_strjoin(tmp->var, "=");
        char *tmp2 = arr[i];
        arr[i] = ft_strjoin(tmp2, tmp->value ? tmp->value : "");
        free(tmp2);
        tmp = tmp->next;
    }
    arr[len] = NULL;
    return arr;
}


//------------TEMP-----------
void	execute_command(t_cmd *cmd, t_minishell *shell)
{
	pid_t	pid;
	char	*path;
	int status;
	int	last_heredoc;

	if (!cmd->argv || !cmd->argv[0])
    	return ;
	if (cmd->heredoc_count > 0)
    {
        if (process_heredoc(cmd, shell->env) == -1)
            return;
    }
	if (is_builtin(cmd))
	{
		status = exec_builtin(cmd, &shell->env, shell);
		shell->exit_status = (unsigned char)status;
		return ;
	}
	char **envp_array = env_list_to_array(shell->env);
	if (!envp_array)
	{
		perror("malloc");
		shell->exit_status = 1;
		exit(1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->exit_status = 1;
		return;
	}
	if (pid == 0)
	{
		change_stdin(cmd);
		change_stdout(cmd);
		//debug
		printf("heredoc count: %d\n",cmd->heredoc_count);
		if (cmd->heredoc_count > 0)
		{
    		last_heredoc = cmd->heredoc_count - 1;
    		if (cmd->heredoc_fds &&
        	cmd->heredoc_fds[last_heredoc] != -1)
    		{
        		dup2(cmd->heredoc_fds[last_heredoc], STDIN_FILENO);
        		for (int k = 0; k < cmd->heredoc_count; k++)
    			{
        			if (cmd->heredoc_fds[k] != -1)
            			close(cmd->heredoc_fds[k]);
    			}
    		}
		}
		path = get_full_path(cmd, shell->env);
		if (!path)
		{
			printf("minishell: the path is not found\n");
			exit(127);                      //means command not found, childs memory is cleaned automatically by the OS, so no need to free anything
		}
		execve(path, cmd->argv, envp_array);
        perror("execve");
        exit(126);               //means command found but cannot be executed
	}
	else
	{
		waitpid(pid, &g_exit_status, 0);          //writes the exit status of the childs termination in g_exit_status

		if (WIFEXITED(g_exit_status))           //if the child exited normally or was it killed by a signal
			g_exit_status = WEXITSTATUS(g_exit_status);        //takes only the exit status from the encoded version
	}
}
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

	if (!cmd->argv || !cmd->argv[0])
    	return ;

	// printf("Executing command: %s\n", cmd->argv[0]);   //for debugging
	//-----built-in execution here
	if (is_builtin(cmd))
	{
		// printf("Executing built-in command: %s\n", cmd->argv[0]);   //for debugging
		g_exit_status = exec_builtin(cmd, &shell->env);
		return ;
	}
	
	char **envp_array = env_list_to_array(shell->env);
	if (!envp_array)
	{
		perror("malloc");
		exit(1);
	}
	//execve(path, cmd->argv, envp_array);

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		change_stdin(cmd);
		change_stdout(cmd);
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
		/*
		waitpid not just writes the exit status of the process, but it is something encoded that shows:

		Did the child exit normally (exit() or return from main)?

		Did it terminate because of a signal?

		If it exited normally, what exit code did it return?
		*/

		if (WIFEXITED(g_exit_status))           //if the child exited normally or was it killed by a signal
			g_exit_status = WEXITSTATUS(g_exit_status);        //takes only the exit status from the encoded version
	}
}
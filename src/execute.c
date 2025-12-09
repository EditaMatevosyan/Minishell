#include "minishell.h"

extern int g_exit_status;

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

void free_env_array(char **envp)
{
    int i = 0;
    if (!envp)
        return;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}


void execute_command(t_cmd *cmd, t_minishell *shell)
{
    pid_t pid;
    char *path;
    int last_heredoc;
    int status;
	struct stat st;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

    if (cmd->heredoc_count > 0)
    {
        if (process_heredoc(cmd, shell->env) == -1)
            return;
    }
    if (is_builtin(cmd))
    {
        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);

        change_stdin(cmd);
        change_stdout(cmd);

        shell->saved_stdin = &saved_stdin;
        shell->saved_stdout = &saved_stdout;
        status = exec_builtin(cmd, &shell->env, shell);
        shell->exit_status = (unsigned char)status;

        // Restore FDs
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return;
    }

    // External commands
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

        if (cmd->heredoc_count > 0)
        {
            last_heredoc = cmd->heredoc_count - 1;
            if (cmd->heredoc_fds && cmd->heredoc_fds[last_heredoc] != -1)
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
// Case 1: path was not found at all
		if (!path)
		{
    		printf("minishell: %s: command not found\n", cmd->argv[0]);
    		free_env_array(envp_array);
    		exit(127);
		}

		if (stat(path, &st) == -1 /*|| !S_ISREG(st.st_mode)*/)
		{
			// File does not exist
    		fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
    		free(path);
    		free_env_array(envp_array);
    		exit(127);
		}
		if (S_ISDIR(st.st_mode))
		{
			if (ft_strchr(cmd->argv[0], '/'))
			{
				fprintf(stderr, "minishell: %s: Is a directory\n", path);
				free(path);
				free_env_array(envp_array);
				exit(126);
			}
			fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
			free(path);
			free_env_array(envp_array);
			exit(127);
		}
		if (!S_ISREG(st.st_mode))
		{
    		fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
    		free(path);
    		free_env_array(envp_array);
    		exit(127);
		}
		if (access(path, X_OK) != 0)
		{
    		fprintf(stderr, "minishell: %s: Permission denied\n", cmd->argv[0]);
    		free(path);
    		free_env_array(envp_array);
    		exit(126);
		}
		execve(path, cmd->argv, envp_array);
		perror("minishell"); // should never happen
		free(path);
		free_env_array(envp_array);
		exit(126);
	}
    else
    {
        waitpid(pid, &g_exit_status, 0);
		free_env_array(envp_array);
        if (WIFEXITED(g_exit_status))
            g_exit_status = WEXITSTATUS(g_exit_status);
    }
}

#include "minishell.h"

extern int g_exit_status;

void	cleanup(t_cmd *cmd, t_minishell *shell)
{
    free_cmd_list(&cmd);
    free_tokens(&shell->tokens);
    free_env(shell->env);
    free(shell);
	return ;
}

void setup_child_io(t_cmd *cmd)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    // Apply declared redirections first
    change_stdin(cmd);
    change_stdout(cmd);

    // Then ensure heredoc takes precedence for stdin
    if (cmd->heredoc_count > 0 && cmd->heredoc_fds)
    {
        int last = cmd->heredoc_count - 1;
        if (cmd->heredoc_fds[last] != -1)
            dup2(cmd->heredoc_fds[last], STDIN_FILENO);

        // Close all heredoc fds to avoid leaks
        int k = 0;
        while (k < cmd->heredoc_count)
        {
            if (cmd->heredoc_fds[k] != -1)
                close(cmd->heredoc_fds[k]);
            k++;
        }
    }
}


void validate_and_exec(t_cmd *cmd, t_minishell *shell, char **envp_array)
{
    struct stat st;
    char *path = get_full_path(cmd, shell->env);

    if (!path)
    {
        printf("minishell: %s: command not found\n", cmd->argv[0]);
        free_env_array(envp_array);
        cleanup(cmd, shell);
        exit(127);
    }
    if (stat(path, &st) == -1)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
        free(path); free_env_array(envp_array); cleanup(cmd, shell);
        exit(127);
    }
    if (S_ISDIR(st.st_mode))
    {
        if (ft_strchr(cmd->argv[0], '/'))
        {
            fprintf(stderr, "minishell: %s: Is a directory\n", path);
            free(path); free_env_array(envp_array); cleanup(cmd, shell);
            exit(126);
        }
        fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
        free(path); free_env_array(envp_array); cleanup(cmd, shell);
        exit(127);
    }
    if (!S_ISREG(st.st_mode))
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
        free(path); free_env_array(envp_array); cleanup(cmd, shell);
        exit(127);
    }
    if (access(path, X_OK) != 0)
    {
        fprintf(stderr, "minishell: %s: Permission denied\n", cmd->argv[0]);
        free(path); free_env_array(envp_array); cleanup(cmd, shell);
        exit(126);
    }

    execve(path, cmd->argv, envp_array);
    free(path); // only reached if execve fails
}



void child_process(t_cmd *cmd, t_minishell *shell, char **envp_array)
{
    setup_child_io(cmd);
    validate_and_exec(cmd, shell, envp_array);
    perror("minishell");
    free_env_array(envp_array);
    cleanup(cmd, shell);
    exit(126);
}


void parent_process(pid_t pid, char **envp_array)
{
    int status = 0;

    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)
            continue;
        perror("waitpid");
        status = 1 << 8;
        break;
    }
    free_env_array(envp_array);

    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (sig == SIGQUIT)
            printf("Quit (core dumped)\n");
        g_exit_status = 128 + sig;
    }
    else if (WIFEXITED(status))
    {
        g_exit_status = WEXITSTATUS(status);
    }
    setup_sigreadline_handlers();
}

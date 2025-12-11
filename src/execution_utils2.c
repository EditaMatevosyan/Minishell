#include "minishell.h"

extern int g_exit_status;

// Close everything except stdin/stdout/stderr so valgrind doesn't report
// stray descriptors inherited from the IDE/terminal.
void	close_stray_fds(void)
{
	struct rlimit	rl;
	int				max_fd;
	int				fd;

	if (getrlimit(RLIMIT_NOFILE, &rl) == -1 || rl.rlim_cur == RLIM_INFINITY)
		max_fd = 1024;
	else
		max_fd = (int)rl.rlim_cur;
	fd = 3;
	while (fd < max_fd)
	{
		close(fd);
		fd++;
	}
}

void	cleanup(t_cmd *cmd_list, t_minishell *shell)
{
    free_cmd_list(&cmd_list);
    free_tokens(&shell->tokens);
    free_env(shell->env);
    free(shell);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
	return ;
}

void setup_child_io(t_cmd *cmd)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    if (change_stdin(cmd) == -1 || change_stdout(cmd) == -1)
        exit(g_exit_status);

    if (cmd->heredoc_count > 0 && cmd->heredoc_fds)
    {
        int last = cmd->heredoc_count - 1;
        if (cmd->heredoc_fds[last] != -1)
            dup2(cmd->heredoc_fds[last], STDIN_FILENO);

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
    int has_slash = (ft_strchr(cmd->argv[0], '/') != NULL);

    if (!path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
        free_env_array(envp_array);
        cleanup(cmd, shell);
        exit(127);
    }
    if (stat(path, &st) == -1)
    {
        if (has_slash)
            fprintf(stderr, "minishell: %s: %s\n", path, strerror(errno));
        else
            fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
        free(path);
        free_env_array(envp_array);
        cleanup(cmd, shell);
        exit(127);
    }
    if (S_ISDIR(st.st_mode))
    {
        if (ft_strchr(cmd->argv[0], '/'))
        {
            fprintf(stderr, "minishell: %s: Is a directory\n", path);
            free(path);
            free_env_array(envp_array);
            cleanup(cmd, shell);
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
    cleanup(cmd, shell);
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

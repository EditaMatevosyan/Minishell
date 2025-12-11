#include "minishell.h"

extern int g_exit_status;

int handle_heredoc(t_cmd *cmd, t_minishell *shell)
{
    if (process_heredoc(cmd, shell->env) == -1)
        return -1;
    return 0;
}

void execute_builtin_helper(t_cmd *cmd, t_minishell *shell)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    int redir_failed = 0;
    shell->saved_stdin = &saved_stdin;
    shell->saved_stdout = &saved_stdout;

    if (change_stdin(cmd) == -1 || change_stdout(cmd) == -1)
        redir_failed = 1;
    if (!redir_failed && cmd->heredoc_count > 0 && cmd->heredoc_fds)
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

    if (redir_failed)
    {
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        shell->exit_status = g_exit_status;
        return;
    }

    int status = exec_builtin(cmd, &shell->env, shell);
    shell->exit_status = (unsigned char)status;
    g_exit_status = shell->exit_status;
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    shell->saved_stdin = NULL;
    shell->saved_stdout = NULL;
}


char **prepare_env(t_minishell *shell)
{
    char **envp_array = env_list_to_array(shell->env);
    if (!envp_array)
    {
        perror("malloc");
        shell->exit_status = 1;
        exit(1);
    }
    return envp_array;
}

void execute_command(t_cmd *cmd, t_minishell *shell)
{
    pid_t pid;
    char **envp_array;
	int k;

    if (!cmd)
        return;

        //for the case < in > out
    if (!cmd->argv || !cmd->argv[0])
    {
        int fd;

        if (cmd->infile)
        {
            fd = open_infile(cmd);
            if (fd < 0)
                return ;
            close(fd);
        }
        if (cmd->outfile)
        {
            fd = open_outfile(cmd);
            if (fd < 0)
                return ;
            close(fd);
        }
        return;
    }
    if (cmd->heredoc_count > 0 && handle_heredoc(cmd, shell) == -1)
        return;
    if (is_builtin(cmd))
        return execute_builtin_helper(cmd, shell);

    envp_array = prepare_env(shell);
    setup_sigexecute_handlers();
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        shell->exit_status = 1;
        return;
    }
    if (pid == 0)
        child_process(cmd, shell, envp_array);
    else
	{
        k = 0;
        while (k < cmd->heredoc_count)
        {
            if (cmd->heredoc_fds[k] != -1)
                close(cmd->heredoc_fds[k]);
            k++;
        }
        parent_process(pid, envp_array);
	}
}

int **create_pipes(int n)
{
    int **fds;
    int i;

    if (n <= 1)
        return (NULL);
    fds = malloc(sizeof(int *) * (n - 1));
    if (!fds)
        return (NULL);
    i = 0;
    while (i < n - 1)
    {
        fds[i] = malloc(sizeof(int) * 2);
        if (!fds[i] || pipe(fds[i]) == -1)
            return (NULL);
        i++;
    }
    return (fds);
}



void close_pipes(int **fds, int n)
{
    int i;

    if (!fds)
        return ;
    i = 0;
    while (i < n - 1)
    {
        close(fds[i][0]);
        close(fds[i][1]);
        i++;
    }
}



void setup_child_fds(t_cmd *cmd, int i, int n, int **fds, t_minishell *ms)
{
    if (i > 0)
        dup2(fds[i - 1][0], STDIN_FILENO);
    if (i < n - 1)
        dup2(fds[i][1], STDOUT_FILENO);
    close_pipes(fds, n);

    if (cmd->has_heredoc)
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
    }
    change_stdin(cmd);
    change_stdout(cmd);
}



int fork_and_execute(t_cmd *cmd_list, t_minishell *ms, int **fds, int n)
{
    t_cmd *cur;
    int i;
    pid_t pid;

    cur = cmd_list;
    i = 0;
    while (cur)
    {
        pid = fork();
        if (pid < 0)
            return (-1);
        if (pid == 0)
        {
            setup_child_fds(cur, i, n, fds, ms);
            if (is_builtin(cur))
                exit(exec_builtin(cur, &ms->env, ms));
            else
            {
                char **envp = env_list_to_array(ms->env);
                char *path = get_full_path(cur, ms->env);
                if (!path)
                    exit(127);
                execve(path, cur->argv, envp);
                exit(126);
            }
        }
        cur = cur->next;
        i++;
    }
    return (0);
}



int execute_pipeline(t_cmd *cmd_list, t_minishell *ms)
{
    int n;
    int **fds;

    n = count_commands(cmd_list);
    fds = create_pipes(n);
    if (n > 1 && !fds)
        return (-1);
    if (fork_and_execute(cmd_list, ms, fds, n) == -1)
        return (-1);

    close_pipes(fds, n);
    for (int i = 0; i < n - 1; i++)
        free(fds[i]);
    free(fds);

    while (n-- > 0)
        wait(NULL);

    return (0);
}

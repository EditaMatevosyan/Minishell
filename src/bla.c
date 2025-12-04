void prepare_all_heredocs(t_cmd *cmds, t_minishell *ms)
{
    t_cmd *c = cmds;

    while (c)
    {
        process_heredocs_for_command(c, ms);
        c = c->next;
    }
}




void process_heredocs_for_command(t_cmd *cmd, t_minishell *ms)
{
    t_list  *node = cmd->redir;
    t_redir *r;
    int     p[2];
    pid_t   pid;
    int     status;

    while (node)
    {
        r = node->content;

        if (r->type == TOKEN_HEREDOC)
        {
            if (pipe(p) == -1)
                fatal_error("pipe");

            pid = fork();
            if (pid < 0)
                fatal_error("fork");

            if (pid == 0)
            {
                close(p[0]);          // child writes to heredoc pipe

                // Read user input until delimiter, write into pipe
                run_heredoc_reader(r->content, p[1], ms);

                exit(0);
            }

            // Parent:
            close(p[1]);              // parent keeps ONLY read-end
            waitpid(pid, &status, 0);

            r->heredoc_fd = p[0];     // store read-end
        }

        node = node->next;
    }
}



void run_heredoc_reader(char *delim, int write_fd, t_minishell *ms)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;

        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break;
        }

        write(write_fd, line, strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }

    close(write_fd);
}

void apply_redirections(t_cmd *cmd)
{
    t_list *node = cmd->redir;
    t_redir *r;
    int last_heredoc_fd = -1;

    // 1) detect last heredoc
    while (node)
    {
        r = node->content;
        if (r->type == TOKEN_HEREDOC)
            last_heredoc_fd = r->heredoc_fd;
        node = node->next;
    }

    // 2) if heredoc exists → override stdin BEFORE pipes
    if (last_heredoc_fd != -1)
        dup2(last_heredoc_fd, STDIN_FILENO);

    // 3) apply <, >, >>
    apply_normal_redirs(cmd); // you already have this
}



execute_pipeline()
{
    prepare_all_heredocs(cmds, ms);   // create pipes and fill them

    create_pipes(fds, n);

    fork loop:
        if child:
            apply_redirections(cmd);  // heredoc first
            setup_pipes();
            execve();
}



// Execute pipeline
int execute_pipeline(t_cmd *cmd_list, t_minishell *ms)
{
    int n = count_commands(cmd_list);
    int **fds = create_pipes(n);
    if (n > 1 && !fds)
        return -1;

    // process all heredocs in parent BEFORE forking
    if (process_all_heredocs(cmd_list, ms) == -1)
        return -1;

    if (fork_and_execute(cmd_list, ms, fds, n) == -1)
        return -1;

    // parent closes all pipes
    close_fds(fds, n);
    for (int i = 0; i < n - 1; i++)
        free(fds[i]);
    free(fds);

    // parent closes heredoc fds
    t_cmd *cur = cmd_list;
    while (cur)
    {
        if (cur->has_heredoc)
            close(cur->heredoc_fd);
        cur = cur->next;
    }

    // wait for all children
    for (int i = 0; i < n; i++)
        wait(NULL);

    return 0;
}
cmd->heredoc_fds = malloc(sizeof(int) * total_heredocs);
for (j = 0; j < total_heredocs; j++)
    cmd->heredoc_fds[j] = -1;



	if (cmds->heredoc_count > 0)
{
    last_heredoc = cmds->heredoc_count - 1;
    if (cmds->heredoc_fds[last_heredoc] >= 0)
        dup2(cmds->heredoc_fds[last_heredoc], STDIN_FILENO);
}

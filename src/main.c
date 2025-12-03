/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:26:34 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/03 10:11:48 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

t_minishell	*minishell_init(char **env)
{
	t_minishell	*shell;

	(void)env;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->tokens = NULL;
	shell->env = copy_env(env);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->fd_in = 0;
	shell->fd_out = 1;
	shell->fd_heredoc = 0;
    shell->exit_status = 0;
	return (shell);
}

static char *read_input(void)
{
	char *input;

	input = readline("minishell> ");
	return (input);
}

static void process_input(t_minishell *ms, char *input)
{
    int rc;
    t_cmd *cmds;

    if (!*input)
        return;
    add_history(input);
    rc = tokenize_input(ms, input);
    if (rc == -1)
        return;

    if (syntax_check(ms->tokens))
    {
        free_tokens(&ms->tokens);
        return;
    }
    if (expand_tokens(ms->tokens, ms->env) == -1)
    {
        free_tokens(&ms->tokens);
        return;
    }

    cmds = parse_tokens(ms->tokens, ms->env);       //this is the head of the liked list
    if (!cmds)
    {
        free_tokens(&ms->tokens);
        return;
    }
	t_cmd	*tmp = cmds;      //this is a temporary pointer not to lose the head of the linked list
	while (tmp)
	{
		if (tmp->has_heredoc == 1)
		{
			if (process_heredoc(tmp, ms->env) == -1)
			{
				free_cmd_list(&cmds);
				free_tokens(&ms->tokens);
				return;
			}
		}
		tmp = tmp->next;
	}
    // TEMP: debug
    print_cmd_list(cmds);
    execute_command(cmds, ms);

	//TEMP: for printing environment variables
	//print_env(ms);

    free_cmd_list(&cmds);
    free_tokens(&ms->tokens);
}

void print_cmd_list(t_cmd *cmd)
{
    int i = 1;
    while (cmd)
    {
        printf("=== CMD %d ===\n", i++);
        for (int j = 0; cmd->argv && cmd->argv[j]; j++)
            printf("argv[%d]: %s\n", j, cmd->argv[j]);
        if (cmd->infile)
            printf("infile: %s\n", cmd->infile);
        if (cmd->outfile)
            printf("outfile: %s (append=%d)\n", cmd->outfile, cmd->append);
        cmd = cmd->next;
    }
}

void minishell_loop(t_minishell *ms)
{
    char *input;

    while (1)
    {
        input = read_input();
        if (!input)
            break;
        process_input(ms, input);
        free(input);
    }
}


int main(int argc, char **argv, char **env)
{
    t_minishell	*shell;

	(void)argv;
	if (argc != 1)
		return (1);
	
	shell = minishell_init(env);
	minishell_loop(shell);
	free_tokens(&shell->tokens);
	free_env(shell->env);
    free(shell);
	return (0);
}

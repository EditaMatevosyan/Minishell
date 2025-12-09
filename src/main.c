
/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosie <rosie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:26:34 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/06 15:11:20 by rosie            ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	{
		free_tokens(&ms->tokens);
        return ;
	}

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
    cmds = parse_tokens(ms->tokens, ms->env);       //this is the head of the linked list
    if (!cmds)
    {
        free_tokens(&ms->tokens);
        return;
    }
	
	if (count_commands(cmds) == 1)
		execute_command(cmds, ms);
	else
		execute_pipeline(cmds, ms);
    free_cmd_list(&cmds);
    //free_tokens(&ms->tokens);
}

// void minishell_loop(t_minishell *ms)
// {
//     char *input;

//     while (1)
//     {
//         input = read_input();
//         if (!input)
//             break;
//         process_input(ms, input);
//         free(input);
//     }
// }


int main(int argc, char **argv, char **env)
{
    t_minishell	*shell;
    char        *input;

	(void)argv;
	if (argc != 1)
		return (1);
	
	while (1)
    {
        input = read_input();
        if (!input)
            break;
        shell = minishell_init(env);
        process_input(shell, input);
        free(input);
        free_tokens(&shell->tokens);
        free_env(shell->env);
        free(shell);
    }
	return (0);
}

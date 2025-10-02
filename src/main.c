/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:26:34 by edmatevo          #+#    #+#             */
/*   Updated: 2025/09/30 20:18:43 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*minishell_init(char **env)
{
	t_minishell	*shell;

	(void)env;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->tokens = NULL;
	// shell->env = env_copy(envp);
	// if (!shell->env)
	// {
	// 	free(shell);
	// 	return (NULL);
	// }
	shell->fd_in = 0;
	shell->fd_out = 1;
	shell->fd_heredoc = 0;
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

    if (!*input)
        return;
    add_history(input);
    rc = tokenize_input(ms, input);
    if (rc == -1)
    {
        free_tokens(&ms->tokens);
        return;
    }
    /* TODO expand + parse + exec. Debug for now: */
    t_token *tmp = ms->tokens;
    while (tmp)
    {
        printf("TOKEN: [%s] type=%d\n",
               tmp->value, tmp->type);
        tmp = tmp->next;
    }
    free_tokens(&ms->tokens);
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
    t_minishell	shell;

	(void)argv;
	if (argc != 1)
		return (1);
	
	shell = *minishell_init(env);
	minishell_loop(&shell);
	free_tokens(&shell.tokens);
	// free_env(shell.env);
	return (0);
}

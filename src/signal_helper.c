/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:40:24 by romargar          #+#    #+#             */
/*   Updated: 2025/12/09 18:24:33 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//CTRL-C
void    sigint_readline_handler(int sig)      //what should happen if the signal occurs
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();                // tell readline we're on new line
    rl_replace_line("", 0);          // clears whatever the user was typing
    rl_redisplay();                  // redraw prompt
}

//when there is no readline, it is the execution phase
void    sigint_handler_for_execution(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
}

void setup_sigreadline_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_readline_handler;
    sigemptyset(&sa.sa_mask);    //no additional signals are blocked
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Ignore Ctrl-\ (SIGQUIT)
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_sigexecute_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler_for_execution;
    sigemptyset(&sa.sa_mask);    //no additional signals are blocked
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Ignore Ctrl-\ (SIGQUIT)
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

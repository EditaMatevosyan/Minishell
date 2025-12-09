/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:32:41 by romargar          #+#    #+#             */
/*   Updated: 2025/12/09 16:33:04 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
✅ 1. CTRL-C (SIGINT) — WHAT BASH DOES
At the prompt (when waiting for user input)

If the user presses Ctrl-C while typing a command:

✔ Kill the current input line
✔ Print a newline
✔ Show a fresh prompt
✔ Do not exit the shell
✔ Set $? (your g_exit_status) to 1
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:19:18 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/21 15:22:08 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type
{
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_APPEND,
    T_HEREDOC
} t_token_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
    int expand; // 1 if the token should be expanded, 0 otherwise
    struct s_token *next;
} t_token;

typedef struct s_cmd
{
    char **argv;
    char *infile;
    char *outfile;
    int append;
    struct s_cmd *next;
} t_cmd;

typedef struct s_minishell
{
    char **env;
    t_token *tokens;
    int fd_in;
    int fd_out;
    int fd_heredoc;
} t_minishell;

t_minishell	*minishell_init(char **env);
t_token *new_token(char *value, t_token_type type, int expand);
void add_token(t_token **list, t_token *new);
int is_special(char c);
char *ft_strdup(char *s, int n);
int tokenize_input(t_minishell *shell, char *input);
int handle_quotes(t_minishell *shell, char *input, int i);
void free_tokens(t_token **list);

char	*extract_operator(char *input, int *i);
char	*extract_quoted(char *input, int *i, int *expand);
char	*extract_word(char *input, int *i);

int is_space(char c);
int is_quote(char c);
int is_operator(char c);
int find_closing_quote(char *input, int start, char quote);
void minishell_loop(t_minishell *ms);

int ft_strcpy(char *dst, const char *src);
int ft_strcat(char *dst, const char *src);  
int	ft_strlen(const char *s);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:19:18 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/27 19:28:39 by edmatevo         ###   ########.fr       */
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
    int quoted; // 0 if unquoted, 1 if double-quoted, 2 if single-quoted
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

typedef struct s_env
{
    char *var;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_minishell
{
    t_env *env;
    t_token *tokens;
    int fd_in;
    int fd_out;
    int fd_heredoc;
} t_minishell;

extern int g_exit_status;

t_minishell	*minishell_init(char **env);
t_token *new_token(char *value, t_token_type type, int expand, int quoted);
void add_token(t_token **list, t_token *new);
int is_special(char c);
char *ft_strndup(char *s, int n);
char *str_join_free(char *s1, char *s2);
char *ft_strdup(char *s);
int tokenize_input(t_minishell *shell, char *input);
int handle_quotes(t_minishell *shell, char *input, int i);
void free_tokens(t_token **list);

char	*extract_operator(char *input, int *i);
char	*extract_quoted(char *input, int *i, int *expand, int *quoted);
char	*extract_word(char *input, int *i);

int is_space(char c);
int is_quote(char c);
int is_operator(char c);
int find_closing_quote(char *input, int start, char quote);
void minishell_loop(t_minishell *ms);

int ft_strcpy(char *dst, const char *src);
int ft_strcat(char *dst, const char *src);  
size_t	ft_strlen(const char *s);
int ft_strcmp(char *s1, char *s2);
int ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);
void print_cmd_list(t_cmd *cmd);
char *ft_strchr(char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int ft_isalnum(int c);
char	*ft_itoa(int n);

char *get_env_value(t_env *env, char *key);
t_env *env_new(char *var, char *value);
void env_add_back(t_env **env, t_env *new);
t_env *env_init(char **envp);
char *expand_str(char *str, t_env *env);
int expand_tokens(t_token *tok, t_env *env);
void free_cmd(t_cmd *cmd);
void free_cmd_list(t_cmd **cmd_list);
t_env *copy_env(char **envp);
void free_env(t_env *env);
t_cmd *parse_command(t_token **cur, t_env *env);
t_cmd *parse_tokens(t_token *tokens, t_env *env);

#endif
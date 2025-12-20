/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:19:18 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 17:28:52 by romargar         ###   ########.fr       */
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
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h> 
#include <sys/resource.h>
#include <string.h>
#include <errno.h>

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
    int expand; 
    int quoted;
    int glued;
    struct s_token *next;
} t_token;

typedef struct s_cmd
{
    char	**argv;
    char	*infile;
    char	*outfile;
    int		append;
	int		heredoc_count;
	char	**heredoc_delims;
	int		*heredoc_fds;
	int		*heredoc_expands;
    int     invalid_redir;
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
    char *input;
    int fd_in;
    int fd_out;
    int fd_heredoc;
    int exit_status;
    int *saved_stdin;
    int *saved_stdout;
	int	in_pipeline;
} t_minishell;

typedef struct s_exec_ctx
{
	t_cmd		*cmd;
	t_minishell	*shell;
	char		**envp_array;
}	t_exec_ctx;

typedef struct s_exec_err
{
	char	*name;
	char	*msg;
	char	*to_free;
	int		code;
}	t_exec_err;

typedef struct s_exec_ctx_2
{
	t_cmd		*cmd_list;
	t_minishell	*ms;
	int			**fds;
	int			n;
	pid_t		*pids;
}	t_exec_ctx_2;

typedef struct s_heredoc_ctx
{
	t_cmd	*cmd;
	t_env	*env;
	int		idx;
	int		fd_out;
}	t_heredoc_ctx;

extern int g_exit_status;

t_minishell	*minishell_init(char **env);
t_token *new_token(char *value, t_token_type type, int expand, int quoted);
void add_token(t_token **list, t_token *new);
int is_special(char c);
char *ft_strndup(char *s, int n);
char *str_join_free(char *s1, char *s2);
char *ft_strdup(const char *s);
int tokenize_input(t_minishell *shell, char **input);
int handle_quotes(t_minishell *shell, char *input, int i);
void free_tokens(t_token **list);

char	*extract_operator(char *input, int *i);
char	*extract_quoted(char **input, int *i, int *expand, int *quoted);
char	*extract_word(char *input, int *i);
int	handle_operator_token(t_minishell *shell, char *input, int *i);

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
char	*ft_strjoin2(const char *s1, const char *s2);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void print_cmd_list(t_cmd *cmd);
char *ft_strchr(char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int ft_isalnum(int c);
char	*ft_itoa(int n);
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
const char *skip_spaces(const char *str);
int acc_digits(const char **ps, int sign, long long *acc_out);
int ft_atoll(const char *s, long long *out);
void cleanup_and_exit(t_minishell *shell, int status);

char *get_env_value(t_env *env, char *key);
int	set_env_value(t_env **env, const char *key, const char *val);
int	append_env_value(t_env **env, const char *key, const char *suffix);
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
int	open_infile(t_cmd *cmd);
int	open_outfile(t_cmd *cmd);
int	change_stdin(t_cmd *cmd);
int	change_stdout(t_cmd *cmd);
void	print_env(t_minishell *shell);
int ft_strcpy_without_null_termination(char *dst, const char *src);
char	*join_path(char *dir, char *command);
void	free_split(char **str);
char	*get_full_path(t_cmd *cmd, t_env *env);
int is_builtin(t_cmd *cmd);
int	word_count(char const *s, char c);
char	*allocate_word(char *s, int len);
void	free_memory(char **str, int count);
int	split_string(char **arr, char *s, char c);
char	**ft_split(char const *s, char c);
void	execute_command(t_cmd *cmd, t_minishell *shell);

int syntax_check(t_token *tok);

int exec_builtin(t_cmd *cmd, t_env **env, t_minishell *shell);
int echo(t_cmd *cmd);
int pwd(t_cmd *cmd);
int	process_heredoc(t_cmd *cmd, t_env *env);
int env_print(t_env *env, char **envp);
void	builtin_exit(t_cmd *cmd, t_minishell *shell);
void builtin_cd(t_cmd *cmd, t_minishell *shell);
int   cd_too_many_args(char **av, t_minishell *sh);
char *cd_dup_home(t_minishell *sh);                 
char *cd_expand_tilde(const char *arg, t_minishell *sh); 
char *cd_resolve_path(char **av, t_minishell *sh);   
int   cd_set_oldpwd(t_minishell *sh);                
int   cd_update_pwd(t_minishell *sh); 
void	builtin_export(t_cmd *cmd, t_minishell *shell);
void	export_print(t_env *env);
void	sort_env_by_key(char **arr);
char	**env_list_to_array_for_export(t_env *env);
void	env_add_if_missing(t_env **env, char *key);
void	export_ident_error(const char *arg, t_minishell *sh);
void	ft_putnstr_fd(char *s, int n, int fd);
void	builtin_unset(t_cmd *cmd, t_minishell *shell);
int is_valid_identifier(char *str);
int	count_commands(t_cmd	*cmd_list);
int		execute_pipeline(t_cmd *cmd_list, t_minishell *ms);
int		count_args(t_token *tok);
void	setup_fds(t_cmd *cmds, int i, int n, int **fds);
int		**create_pipes(int n);
void	close_fds(int	**fds, int n);
char	**env_list_to_array(t_env *env);
int		process_all_heredocs(t_cmd *cmd_list, t_minishell *ms);
int	count_heredocs(t_token *tok);
void free_env_array(char **envp);
void    sigint_readline_handler(int sig);
void    sigint_handler_for_execution(int sig);
void setup_sigreadline_handlers(void);
void setup_sigexecute_handlers(void);
void	cleanup(t_cmd *cmd_list, t_minishell *shell);
void setup_child_io(t_cmd *cmd);
void validate_and_exec(t_cmd *cmd, t_minishell *shell, char **envp_array);
void parent_process(pid_t pid, char **envp_array);
void close_stray_fds(void);
void	exec_error_msg_free(t_exec_ctx *ctx, t_exec_err *err);
void	exec_error_errno(t_exec_ctx *ctx, char *path, int code);
void	exec_stat_or_notfound(t_exec_ctx *ctx, char *path, int has_slash,
		struct stat *st);
int	setup_builtin_redirs(t_cmd *cmd, t_minishell *shell,
		int *saved_stdin, int *saved_stdout);
void	apply_builtin_heredoc(t_cmd *cmd);
void	restore_std_fds(t_minishell *shell, int saved_stdin,
		int saved_stdout);
void	handle_builtin_redir_failure(t_minishell *shell, int saved_stdin,
		int saved_stdout);
int	handle_invalid_redirs(t_cmd *cmd, t_minishell *shell);
int	try_open_infile(t_cmd *cmd, t_minishell *shell);
int	try_open_outfile(t_cmd *cmd, t_minishell *shell);
int	handle_empty_argv(t_cmd *cmd, t_minishell *shell);
void	close_heredoc_fds(t_cmd *cmd);
void	execution_after_forking(t_cmd *cmd, t_minishell *shell);
char	**prepare_env(t_minishell *shell);
int	handle_heredoc(t_cmd *cmd, t_minishell *shell);
void	execute_builtin_helper(t_cmd *cmd, t_minishell *shell);
void	free_pipes(int **fds, int n);
void	setup_child_signals(void);
void	setup_pipe_fds(int i, int n, int **fds);
void	setup_heredoc_fds(t_cmd *cmds);
void	finalize_stdio_or_exit(t_cmd *cmds, int **fds, int n);
void	exit_child(t_exec_ctx_2 *ctx, int code);
void	exec_external_or_exit(t_cmd *cur, t_exec_ctx_2 *ctx);
void	child_process_2(t_cmd *cur, int i, t_exec_ctx_2 *ctx);
int	fork_and_execute(t_exec_ctx_2 *ctx);
int	prepare_pipeline(t_exec_ctx_2 *ctx);
void	cleanup_pipeline_fds(t_exec_ctx_2 *ctx);
int	wait_pipeline(t_exec_ctx_2 *ctx);
void	init_exec_ctx_2(t_exec_ctx_2 *ctx, t_cmd *cmd_list, t_minishell *ms);
void	update_exit_status(t_minishell *ms, int last_status);
int	run_pipeline(t_exec_ctx_2 *ctx);
void	child_process(t_cmd *cmd, t_minishell *shell, char **envp_array);
void	close_all_heredoc_fds(t_cmd *cmd_list);


#endif

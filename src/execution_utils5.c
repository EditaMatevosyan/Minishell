#include "minishell.h"

extern int	g_exit_status;

void	exec_error_msg_free(t_exec_ctx *ctx, t_exec_err *err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(err->name, 2);
	ft_putstr_fd(err->msg, 2);
	if (err->to_free)
		free(err->to_free);
	free_env_array(ctx->envp_array);
	cleanup(ctx->cmd, ctx->shell);
	exit(err->code);
}

void	exec_error_errno(t_exec_ctx *ctx, char *path, int code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(path);
	free_env_array(ctx->envp_array);
	cleanup(ctx->cmd, ctx->shell);
	exit(code);
}

void	exec_stat_or_notfound(t_exec_ctx *ctx, char *path, int has_slash,
		struct stat *st)
{
	t_exec_err	err;

	if (stat(path, st) == -1)
	{
		if (has_slash)
			exec_error_errno(ctx, path, 127);
		err.name = ctx->cmd->argv[0];
		err.msg = ": command not found\n";
		err.to_free = path;
		err.code = 127;
		exec_error_msg_free(ctx, &err);
	}
}
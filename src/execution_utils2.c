/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:35:38 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 17:21:18 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_check_dir(t_exec_ctx *ctx, char *path, int has_slash,
		struct stat *st)
{
	t_exec_err	err;

	if (!S_ISDIR(st->st_mode))
		return ;
	if (has_slash)
	{
		err.name = path;
		err.msg = ": Is a directory\n";
		err.to_free = path;
		err.code = 126;
		exec_error_msg_free(ctx, &err);
	}
	err.name = ctx->cmd->argv[0];
	err.msg = ": command not found\n";
	err.to_free = path;
	err.code = 127;
	exec_error_msg_free(ctx, &err);
}

static void	exec_check_regular(t_exec_ctx *ctx, char *path, struct stat *st)
{
	t_exec_err	err;

	if (!S_ISREG(st->st_mode))
	{
		err.name = ctx->cmd->argv[0];
		err.msg = ": command not found\n";
		err.to_free = path;
		err.code = 127;
		exec_error_msg_free(ctx, &err);
	}
	if (access(path, X_OK) != 0)
	{
		err.name = ctx->cmd->argv[0];
		err.msg = ": Permission denied\n";
		err.to_free = path;
		err.code = 126;
		exec_error_msg_free(ctx, &err);
	}
}

static void	exec_path_checks(t_exec_ctx *ctx, char *path, int has_slash)
{
	struct stat	st;

	exec_stat_or_notfound(ctx, path, has_slash, &st);
	exec_check_dir(ctx, path, has_slash, &st);
	exec_check_regular(ctx, path, &st);
}

void	validate_and_exec(t_cmd *cmd, t_minishell *shell, char **envp_array)
{
	t_exec_ctx	ctx;
	char		*path;
	int			has_slash;
	t_exec_err	err;

	ctx.cmd = cmd;
	ctx.shell = shell;
	ctx.envp_array = envp_array;
	path = get_full_path(cmd, shell->env);
	has_slash = (ft_strchr(cmd->argv[0], '/') != NULL);
	if (!path)
	{
		err.name = cmd->argv[0];
		err.msg = ": command not found\n";
		err.to_free = NULL;
		err.code = 127;
		exec_error_msg_free(&ctx, &err);
	}
	exec_path_checks(&ctx, path, has_slash);
	execve(path, cmd->argv, envp_array);
	cleanup(cmd, shell);
	free(path);
}

void	child_process(t_cmd *cmd, t_minishell *shell, char **envp_array)
{
	setup_child_io(cmd);
	validate_and_exec(cmd, shell, envp_array);
	perror("minishell");
	free_env_array(envp_array);
	cleanup(cmd, shell);
	exit(126);
}

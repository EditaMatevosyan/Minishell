/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:13:03 by romargar          #+#    #+#             */
/*   Updated: 2025/12/11 17:24:16 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//open the infile(the file with which the stdin should be replaced) and take its file descriptor

int	open_infile(t_cmd *cmd)
{
	int	fd;

	if (!cmd->infile)
		return (-1);
	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
	{
		perror(cmd->infile);
		g_exit_status = 1;
		return (-1);
	}
	return (fd);
}

int open_outfile(t_cmd *cmd)
{
    int fd;

    if (!cmd->outfile)
        return (-1);

    if (cmd->append)
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror(cmd->outfile);
        g_exit_status = 1;
        return (-1);
    }

    return fd;
}

int	change_stdin(t_cmd *cmd)
{
	int fd_in;

	if (!cmd->infile)
		return (0);

	fd_in = open_infile(cmd);
	if (fd_in < 0)
	{
		return (-1);
	}
	if (dup2(fd_in, STDIN_FILENO) < 0)
	{
    	perror("dup2");
        close(fd_in);
        g_exit_status = 1;
    	return (-1);
	}
	close(fd_in);
	return (0);
}



int	change_stdout(t_cmd *cmd)
{
	int fd_out;

	if (!cmd->outfile)
		return (0);

	fd_out = open_outfile(cmd);
	if (fd_out < 0)
	{
		return (-1);
	}
	if (dup2(fd_out, STDOUT_FILENO) < 0)
	{
    	perror("dup2");
        close(fd_out);
        g_exit_status = 1;
    	return (-1);
	}
	close(fd_out);
	return (0);
}

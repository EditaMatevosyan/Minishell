/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:13:03 by romargar          #+#    #+#             */
/*   Updated: 2025/12/11 15:51:51 by romargar         ###   ########.fr       */
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
    {
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
    else
    {
		//debug
		printf("enters the outfile ...");
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    if (fd < 0)
    {
        perror(cmd->outfile);
        g_exit_status = 1;
        return (-1);
    }

    return fd;
}

void	change_stdin(t_cmd *cmd)
{
	int fd_in;

	if (!cmd->infile)
		return ;
	fd_in = open_infile(cmd);
	if (fd_in < 0)
	{
		return ;
	}
	if (dup2(fd_in, STDIN_FILENO) < 0)                //"Make STDIN_FILENO (standard input) point to the same file as fd_in."
	{
    	perror("dup2");
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    	exit(1);           //exit the child process
	}
	close(fd_in);
}

void	change_stdout(t_cmd *cmd)
{
	int fd_out;

	if (!cmd->outfile)
		return ;
	fd_out = open_outfile(cmd);
	if (fd_out < 0)
		return ;
	if (dup2(fd_out, STDOUT_FILENO) < 0)
	{
    	perror("dup2");
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    	exit(1);
	}
	close(fd_out);
}
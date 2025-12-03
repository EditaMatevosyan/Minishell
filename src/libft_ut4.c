#include "minishell.h"

void ft_putstr_fd(char *s, int fd)
{
    if (!s)
        return;
    while (*s)
    {
        write(fd, s, 1);
        s++;
    }
}

const char *skip_spaces(const char *str)
{
    while (*str && is_space(*str))
        str++;
    return str;
}

int get_sign(const char **str)
{
    const char	*s;
	int			sign;

	s = *str;
	sign = 1;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	*str = s;
	return (sign);
}

int ft_atoll(const char *s, long long *out)
{
    long long    acc;
    int         sign;

    if(!s || !out)
        return (0);
    s = skip_spaces(s);
    sign = get_sign(&s);
    if (!acc_digits(&s, sign, &acc))
        return (0);
    s = skip_spaces(s);
    if (*s != '\0')
        return (0);
    if(sign == 1)
        *out = acc;
    else
        *out = acc * sign;
    return (1);
}

void cleanup_and_exit(t_minishell *shell, int status)
{
    // 1. Free environment
    if (shell->env)
        free_env(shell->env);

    // if (shell->cmd)
    //     free_cmd_list(shell->cmd);

    rl_clear_history();

    if (shell->fd_in != -1 && shell->fd_in != 0)
        close(shell->fd_in);
    if (shell->fd_out != -1 && shell->fd_out != 1)
        close(shell->fd_out);
    if (shell->fd_heredoc != -1)
        close(shell->fd_heredoc);

    exit(status);
}

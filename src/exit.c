#include "minishell.h" 

int acc_digits(const char **ps, int sign, long long *acc_out)
{
	const char	*s;
	long long	acc;
	int			d;
	int			have;

	s = *ps;
	acc = 0;
	have = 0;
	while (*s >= '0' && *s <= '9')
	{
		d = *s - '0';
		if (sign == 1 && acc > (LLONG_MAX - d) / 10)
			return (0);
		if (sign == -1 && -acc < (LLONG_MIN + d) / 10)
			return (0);
		acc = acc * 10 + d;
		s++;
		have = 1;
	}
	if (!have)
		return (0);
	*ps = s;
	*acc_out = acc;
	return (1);
}

static void	ms_print_exit(void)
{
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
}

static void	ms_exit_numerr(t_minishell *shell, const char *arg)
{
	ms_print_exit();
	ft_putstr_fd("minishell: Exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": Numeric argument required\n", 2);
	cleanup_and_exit(shell, 255);
}

void	builtin_exit(t_cmd *cmd, t_minishell *shell)
{
	char		**av;
	long long	val;

	av = cmd->argv;
	if (!av || !av[0])
		return ;
	if (!av[1])
	{
		ms_print_exit();
		cleanup_and_exit(shell, (unsigned char)shell->exit_status);
	}
	if (!ft_atoll(av[1], &val))
		ms_exit_numerr(shell, av[1]);
	if (av[2])
	{
		ft_putstr_fd("minishell: Exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	ms_print_exit();
	cleanup_and_exit(shell, (unsigned char)val);
}

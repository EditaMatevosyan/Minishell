#include "minishell.h"

void pwd(t_cmd *cmd)
{
    char cwd[4096];
    (void)cmd;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd");
    }
}

void builtin_cd(t_cmd *cmd, t_minishell *shell)
{
    char	**av;
	char	buf[PATH_MAX];
	char	*pwd;

	av = cmd->argv;
	if (!av[1] || av[2])
	{
		ft_putstr_fd("minishell: cd: wrong number of arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (chdir(av[1]) == -1)
	{
		perror("minishell: cd");
		shell->exit_status = 1;
		return ;
	}
	pwd = getcwd(buf, sizeof(buf));
	if (!pwd || set_env_value(&shell->env, "PWD", pwd))
	{
        if (!pwd)
			perror("minishell: cd: getcwd");
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = 0;
}


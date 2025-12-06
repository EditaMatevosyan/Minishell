#include "minishell.h"

void	ft_putnstr_fd(char *s, int n, int fd)
{
	write(fd, s, n);
}

void	env_add_if_missing(t_env **env, char *key)
{
	if (!get_env_value(*env, key))
		env_add_back(env, env_new(key, ""));
}

void sort_env(char **arr)
{
    int i;
    int j;
    char *temp;

    i = 0;
    while (arr[i])
    {
        j = i + 1;
        while (arr[j])
        {
            if (ft_strcmp(arr[i], arr[j]) > 0)
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
            j++;
        }
        i++;
    }
}

void export_print(t_env *env)
{
    char	**arr;
	int		i;
	char	*eq;

	arr = env_list_to_array(env);
	if (!arr)
		return ;
	sort_env(arr);
	i = 0;
	while (arr[i])
	{
		eq = ft_strchr(arr[i], '=');
		if (!eq)
			ft_putstr_fd(arr[i], 1);
		else
		{
			ft_putnstr_fd(arr[i], eq - arr[i] + 1, 1); // print KEY=
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(eq + 1, 1); // value
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	int		i;
	char	*arg;
	char	*eq;

	if (!cmd->argv[1])
		return (export_print(shell->env), (void)(shell->exit_status = 0));
	i = 1;
	while (cmd->argv[i])
	{
		arg = cmd->argv[i];
		eq = ft_strchr(arg, '=');
		if (!eq && !is_valid_identifier(arg))
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2),
			shell->exit_status = 1;
		else if (!eq) // KEY only
			env_add_if_missing(&shell->env, arg);
		else if (!is_valid_identifier(ft_substr(arg, 0, eq - arg)))
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2),
			shell->exit_status = 1;
		else
			set_env_value(&shell->env,
				ft_substr(arg, 0, eq - arg), eq + 1);
		i++;
	}
}

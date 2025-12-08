#include "minishell.h"

void env_print(t_env *env, char **envp)
{
	t_env	*current;

	ft_putstr_fd("[BUILTIN ENV CALLED]\n", 2);
	if (envp[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return;
	}
	current = env;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->var, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
}
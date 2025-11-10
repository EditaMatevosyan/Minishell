#include "minishell.h"

void	print_env(t_minishell *shell)
{
	t_env *tmp = shell->env;
	while (tmp)
	{
    	if (ft_strcmp(tmp->var, "PATH") == 0)
    	{
        	printf("PATH = %s\n", tmp->value);
        	break;
    	}
    	tmp = tmp->next;
	}
}
#include "minishell.h"

void	ft_putnstr_fd(char *s, int n, int fd)
{
	write(fd, s, n);
}

static void export_ident_error(const char *arg, t_minishell *sh)
{
    ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd((char *)arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
    sh->exit_status = 1;
}

void	env_add_if_missing(t_env **env, char *key)
{
	if (!get_env_value(*env, key))
		env_add_back(env, env_new(key, NULL));
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

static void print_escaped(const char *s)
{
    while (*s)
    {
        if (*s == '\"' || *s == '\\' || *s == '$')
            ft_putchar_fd('\\', 1);
        ft_putchar_fd(*s++, 1);
    }
}

static int key_cmp(const char *a, const char *b)
{
    int i = 0;

    while (a[i] && a[i] != '=' && b[i] && b[i] != '=')
    {
        if (a[i] != b[i])
            return ((unsigned char)a[i] - (unsigned char)b[i]);
        i++;
    }
    if ((a[i] == '\0' || a[i] == '=') && (b[i] == '\0' || b[i] == '='))
        return (0);
    if (a[i] == '\0' || a[i] == '=')
        return (-1);
    return (1);
}

static void sort_env_by_key(char **arr)
{
    int   i;
    int   j;
    char *tmp;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
    {
        j = i + 1;
        while (arr[j])
        {
            if (key_cmp(arr[i], arr[j]) > 0)
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

static void	free_str_array(char **arr, int count)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	**env_list_to_array_for_export(t_env *env)
{
	int		len;
	int		i;
	t_env	*tmp;
	char	**arr;

	len = 0;
	tmp = env;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->value == NULL)
			arr[i] = ft_strdup(tmp->var);
		else
		{
			char *with_eq = ft_strjoin(tmp->var, "=");
			if (!with_eq)
				return (free_str_array(arr, i), NULL);
			arr[i] = ft_strjoin(with_eq, tmp->value);
			free(with_eq);
			if (!arr[i])
				return (free_str_array(arr, i), NULL);
		}
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

void export_print(t_env *env)
{
    char **arr;
    int   i;
    char  *eq;

    arr = env_list_to_array_for_export(env);
    if (!arr)
        return ;
    sort_env_by_key(arr); 
    i = 0;
    while (arr[i])
    {
        ft_putstr_fd("declare -x ", 1);
        eq = ft_strchr(arr[i], '=');
        if (!eq)                        
            ft_putstr_fd(arr[i], 1);
        else
        {
            ft_putnstr_fd(arr[i], (int)(eq - arr[i] + 1), 1); 
            ft_putstr_fd("\"", 1);
            print_escaped(eq + 1);
            ft_putstr_fd("\"", 1);
        }
        ft_putstr_fd("\n", 1);
        free(arr[i]);
        i++;
    }
    free(arr);
}

void    builtin_export(t_cmd *cmd, t_minishell *sh)
{
    int   i;
    char  *arg;
    char  *eq;
    char  *key;

    if (!cmd->argv[1])
        return (export_print(sh->env), (void)(sh->exit_status = 0));
    i = 1;
    while (cmd->argv[i])
    {
        arg = cmd->argv[i];
        eq = ft_strchr(arg, '=');

        if (!eq) 
        {
            if (!is_valid_identifier(arg))
                export_ident_error(arg, sh);
            else
                env_add_if_missing(&sh->env, arg); 
        }
        else 
        {
            key = ft_substr(arg, 0, eq - arg);
            if (!key)
                return ((void)(sh->exit_status = 1));
            if (!is_valid_identifier(key))
                export_ident_error(arg, sh);
            else
                set_env_value(&sh->env, key, eq + 1);
            free(key);
        }
        i++;
    }
}
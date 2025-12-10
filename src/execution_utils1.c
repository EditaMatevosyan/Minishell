#include "minishell.h"

extern int g_exit_status;            //it is defined in another .c file, and i want to use it in this .c file, thats why it is written extern

//we need to join the directory and the command name, then check if that path exists or not

char	*join_path(char *dir, char *command)
{
	char *full_path;
	int	full_path_len;

	full_path_len = ft_strlen(dir) + ft_strlen(command) + 2;        //for null termination + '/'
	full_path = malloc(sizeof(char) * full_path_len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	return (full_path);
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}


char	*get_full_path(t_cmd *cmd, t_env *env)
{
	char	*path_value;
	char	**directories;
	int		i;
	char	*full_path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
    	return NULL;
	if (strchr(cmd->argv[0], '/'))            //the path can be absolute or relative for example ./a.out ../....
        return strdup(cmd->argv[0]);
	path_value = get_env_value(env, "PATH");
	if (!path_value)
		return (NULL);
	directories = ft_split(path_value, ':');
	if (!directories)
		return (NULL);
	i = 0;
	while (directories[i])
	{
		full_path = join_path(directories[i], cmd->argv[0]);
		//we need to check if the file is executable
		if (full_path && access(full_path, X_OK) == 0)           //X_OK means executable file
		{
			free_split(directories);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(directories);
    return NULL;
}

char **env_list_to_array(t_env *env)
{
    int len = 0;
    t_env *tmp = env;
    while (tmp)
    {
        len++;
        tmp = tmp->next;
    }

    char **arr = malloc(sizeof(char *) * (len + 1));
    if (!arr)
        return NULL;

    tmp = env;
    for (int i = 0; i < len; i++)
    {
        arr[i] = ft_strjoin(tmp->var, "=");
        char *tmp2 = arr[i];
        arr[i] = ft_strjoin(tmp2, tmp->value ? tmp->value : "");
        free(tmp2);
        tmp = tmp->next;
    }
    arr[len] = NULL;
    return arr;
}

void free_env_array(char **envp)
{
    int i = 0;
    if (!envp)
        return;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}
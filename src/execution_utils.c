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
	//cmd->argv[0]  is the actual command name
	/*
	PATH = /home/rosie/.vscode-server/bin/7d842fb85a0275a4a8e4d7e040d2625abbf7f084/bin/remote-cli:
	/home/rosie/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:
	/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Program Files/Common Files/Oracle/Java/javapath:
	/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:
	/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/Tools/Binn/:
	/mnt/c/Program Files/dotnet/:/mnt/c/Program Files (x86)/Microsoft SQL Server/160/Tools/Binn/:
	/mnt/c/Program Files/Microsoft SQL Server/160/Tools/Binn/:/mnt/c/Program Files/Microsoft SQL Server/160/DTS/Binn/:
	/mnt/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit/:/mnt/c/Program Files/Git/cmd:
	/mnt/c/Program Files/MySQL/MySQL Shell 8.0/bin/:/mnt/c/Users/rosie/AppData/Local/Programs/Python/Launcher/:
	/mnt/c/Users/rosie/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/rosie/AppData/Local/Programs/Microsoft VS Code/bin:
	/mnt/c/Users/rosie/.dotnet/tools:/mnt/c/Users/rosie/AppData/Local/JetBrains/Toolbox/scripts:/snap/bin:/home/rosie/.local/bin:
	/home/rosie/.venv/bin:/home/rosie/.venv/bin:/home/rosie/.venv/bin:/home/rosie/.local/bin:/home/rosie/.local/bin:/home/rosie/.venv/bin:
	/home/rosie/.venv/bin:/home/rosie/.venv/bin
	*/
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
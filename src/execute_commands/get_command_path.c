#include "../../minishell.h"

char	*get_env_path(t_env *env)
{
	t_env	*temp;

	if (!env)
		return (NULL);
	temp = env;
	while (temp)
	{
		if (!ft_strcmp(temp->key, "PATH"))
			return (ft_strdup(temp->value));
		temp = temp->next;
	}
	return (NULL);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*next;
	char		*start;

	if (!str && (!next || *next == '\0'))
		return (NULL);
	if (str)
		next = str;
	start = next;
	while (*next && !ft_strchr(delim, *next))
		next++;
	if (*next)
		*next++ = '\0';
	return (start);
}

static char	*search_cmd_path(char *path_env, char *cmd)
{
	char	*full_path;
	char	*path;
	char	*dir;
	size_t	len;

	if (!cmd)
		return (NULL);
	path = ft_strdup(path_env);
	dir = ft_strtok(path, ":");
	while (dir)
	{
		len = ft_strlen(dir) + ft_strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
			return (NULL);
		snprintf(full_path, len, "%s/%s", dir, cmd);
		if (!access(full_path, F_OK))
			return (free(path), full_path);
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

char	*get_command_path(char *cmd, t_env *env)
{
	char	*local_path;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, F_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	local_path = get_env_path(env);
	if (!local_path)
		return (NULL);
	full_path = search_cmd_path(local_path, cmd);
	if (!full_path)
	{
		free(local_path);
		return (NULL);
	}
	free(local_path);
	return (full_path);
}

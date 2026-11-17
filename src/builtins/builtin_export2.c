#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	add_var(t_env **env, char *key, char *value, t_env *var)
{
	if (var)
	{
		if (var->value)
			free(var->value);
		if (value)
			var->value = ft_strdup(value);
		else
			var->value = ft_strdup("");
	}
	else if (!var && !value)
		env_add_back(env, key, "", 0);
	else
		env_add_back(env, key, value, 0);
	free(key);
	if (value)
		free(value);
}

int	add_to_env_list(t_env **env, char *key, char *value)
{
	t_env	*var_exists;

	if (!is_valid_identifier(key))
	{
		printf("export: %s: not a valid identifier\n", key);
		return (1);
	}
	var_exists = get_env_var(*env, key);
	value = ft_strtrim(value, " \t\n");
	add_var(env, key, value, var_exists);
	return (0);
}

int	add_non_equal_var(t_env **env, char *key, char *value)
{
	if (!is_valid_identifier(key))
	{
		printf("export: %s: not a valid identifier\n", key);
		return (1);
	}
	if (!get_env_var(*env, key))
		env_add_back(env, key, value, 0);
	free(key);
	return (0);
}

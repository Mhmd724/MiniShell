#include "../../minishell.h"

t_env	*create_env_node(char *key, char *value, int hidden)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	if (!key)
		new_node->key = NULL;
	else
		new_node->key = ft_strdup(key);
	if (!value)
	{
		new_node->value = NULL;
		new_node->assigned = 0;
	}
	else
	{
		new_node->value = ft_strdup(value);
		new_node->assigned = 1;
	}
	new_node->hidden = hidden;
	new_node->next = NULL;
	new_node->two_point = 0;
	return (new_node);
}

void	env_add_back(t_env **env, char *key, char *value, int hidden)
{
	t_env	*node;
	t_env	*tmp;

	node = create_env_node(key, value, hidden);
	tmp = *env;
	if (!node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

char	*get_env(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

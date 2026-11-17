#include "../../minishell.h"

int	check_option(const char *str)
{
	return (str && str[0] == '-' && ft_strlen(str) > 1);
}

int	valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(str[0]) && str[0] != '_'))
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

void	remove_env_var(t_env **env, const char *key)
{
	t_env	*to_del;
	t_env	*tmp;

	if (!env || !*env || !key)
		return ;
	if (!ft_strcmp((*env)->key, key))
	{
		to_del = *env;
		*env = to_del->next;
	}
	else
	{
		tmp = *env;
		while (tmp->next && ft_strcmp(tmp->next->key, key))
			tmp = tmp->next;
		if (!tmp->next)
			return ;
		to_del = tmp->next;
		tmp->next = to_del->next;
	}
	free(to_del->key);
	free(to_del->value);
	free(to_del);
}

int	builtin_unset(t_tools *tools, t_parser *parser, t_env **env)
{
	t_lexer	*temp;
	int		exit_status;

	(void)tools;
	exit_status = 0;
	temp = parser->tokens->next;
	if (!temp)
		return (0);
	if (check_option(temp->str))
	{
		printf("unset: invalid option '%s'\n", temp->str);
		return (2);
	}
	while (temp && temp->token == TOKEN_WORD)
	{
		if (!valid_identifier(temp->str))
		{
			exit_status = 1;
			printf("`%s`: not a valid identifier\n", temp->str);
		}
		else
			remove_env_var(env, temp->str);
		temp = temp->next;
	}
	return (exit_status);
}

#include "../../minishell.h"

void	print_export_list(t_env *env)
{
	t_env	*temp;

	if (!env)
		return ;
	temp = env;
	while (temp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(temp->key, STDOUT_FILENO);
		if (temp->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(temp->value, STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		temp = temp->next;
	}
}

int	handle_key_value(t_env **env, const char *var)
{
	char	*key;
	char	*value;
	char	*eq;
	int		ret;

	eq = ft_strchr(var, '=');
	if (eq == var)
	{
		print_cmd_error("export", (char *)var, "not a valid identifier");
		return (1);
	}
	if (!eq)
		ret = add_non_equal_var(env, ft_strdup(var), NULL);
	else
	{
		key = ft_strndup(var, eq - var);
		value = eq + 1;
		ret = add_to_env_list(env, key, value);
	}
	return (ret);
}

int	builtin_export(t_tools *tools, t_parser *parser, t_env **env)
{
	t_lexer	*temp;
	int		ret;

	(void)tools;
	temp = parser->tokens->next;
	if (!temp)
	{
		print_export_list(*env);
		return (0);
	}
	ret = 0;
	while (temp)
	{
		if (handle_key_value(env, temp->str))
			ret = 1;
		temp = temp->next;
	}
	return (ret);
}

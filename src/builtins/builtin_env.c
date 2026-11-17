#include "../../minishell.h"

int	builtin_env(t_tools *tools, t_parser *parser, t_env **env)
{
	t_env	*temp;

	(void)tools;
	(void)parser;
	if (!env)
	{
		write(2, "env: No environment variables to display\n", 41);
		return (0);
	}
	temp = *env;
	while (temp)
	{
		if (temp->hidden == 0 && temp->value)
		{
			ft_putstr_fd(temp->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(temp->value, STDOUT_FILENO);
		}
		temp = temp->next;
	}
	return (0);
}

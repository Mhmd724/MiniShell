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
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	return (0);
}

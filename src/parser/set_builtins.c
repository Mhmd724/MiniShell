#include "../../minishell.h"

void	set_one_builtin(t_parser *parser)
{
	static const char		*builtin_names[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};
	static t_builtin_func	builtin_funcs[] = {
		builtin_echo, builtin_cd, builtin_pwd, builtin_export,
		builtin_unset, builtin_env, builtin_exit
	};
	int						i;

	i = 0;
	while (i < 7)
	{
		if (!ft_strcmp(parser->tokens->str, builtin_names[i]))
		{
			parser->builtin = builtin_funcs[i];
			return ;
		}
		i++;
	}
	parser->builtin = NULL;
}

void	set_builtins(t_parser *parser)
{
	t_parser	*temp;

	if (!parser)
		return ;
	temp = parser;
	while (temp)
	{
		if (temp->tokens)
			set_one_builtin(temp);
		temp = temp->next;
	}
}

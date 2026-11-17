#include "../../minishell.h"

int	count_arguments(t_lexer *tokens)
{
	int		count;
	t_lexer	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp && tmp->token != TOKEN_PIPE && tmp->token != TOKEN_EOF)
	{
		if (tmp->token == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

int	fill_args(char **args, t_lexer *tokens, int count)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp && i < count)
	{
		if (tmp->token == TOKEN_WORD)
		{
			args[i] = ft_strdup(tmp->str);
			if (!args[i])
			{
				while (i > 0)
					free(args[--i]);
				return (0);
			}
			i++;
		}
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (1);
}

char	**build_args(t_lexer *tokens)
{
	char	**args;
	int		count_args;

	count_args = count_arguments(tokens);
	args = (char **)malloc(sizeof(char *) * (count_args + 1));
	if (!args)
		return (NULL);
	if (!fill_args(args, tokens, count_args))
	{
		free(args);
		return (NULL);
	}
	return (args);
}

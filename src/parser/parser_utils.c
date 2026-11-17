#include "../../minishell.h"

int	count_pipes(t_lexer *lexer, unsigned int *tokens_nb)
{
	int	count;

	count = 0;
	while (lexer)
	{
		if (lexer->token == TOKEN_PIPE)
			count++;
		(*tokens_nb)++;
		lexer = lexer->next;
	}
	return (count);
}

int	is_a_special_case(char *str, int *i)
{
	return (str[*i]
		&& (ft_isdigit(str[*i]) || str[*i] == '$'));
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	concat_tokens(t_lexer *current, t_lexer *to_del)
{
	if (!current || !to_del)
		return ;
	if (ft_strlen(current->str) == 1 && current->str[0] == '$'
		&& current->quote_type == NO_QUOTE)
	{
		free(current->str);
		current->str = ft_strdup(current->next->str);
		if (!current->str)
			return ;
	}
	else
		current->str = ft_strjoin(current->str, to_del->str);
}

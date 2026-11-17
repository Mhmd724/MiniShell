#include "../../minishell.h"

int	check_unclosed_quotes(char *str)
{
	int		i;
	char	quote;

	if (!str)
		return (0);
	quote = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote);
}

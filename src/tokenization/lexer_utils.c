#include "../../minishell.h"

int	special_char(char c)
{
	if (c == '|' || c == '>'
		|| c == '<' || c == '\''
		|| c == '\"')
		return (1);
	return (0);
}

void	add_token_increment_i(t_lexer **lexer, const char *str, t_tokens token)
{
	t_lexer	*new_node;

	new_node = create_new_token_node(token, NO_QUOTE, str, 1);
	add_token_node_back(lexer, new_node);
}

t_quote_type	get_quote_type(char quote)
{
	if (quote == '\'')
		return (SINGLE_QUOTES);
	return (DOUBLE_QUOTES);
}

int	get_quote_length(const char *str, int i, char quote)
{
	int	len;

	len = 1;
	while (str[i + len])
	{
		if (quote == '"' && str[i + len] == '\\' && str[i + len + 1] == '"')
		{
			len += 2;
			continue ;
		}
		if (str[i + len] == quote)
			return (len);
		len++;
	}
	return (len);
}

int	get_real_string_length(char *str)
{
	int	len;
	int	i;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '"')
		{
			len++;
			i += 2;
			continue ;
		}
		len++;
		i++;
	}
	return (len);
}

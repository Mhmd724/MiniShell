#include "../../minishell.h"

void	handle_pipe(t_lexer **lexer, int *i)
{
	t_lexer	*new;

	new = create_new_token_node(TOKEN_PIPE, NO_QUOTE, "|", 1);
	add_token_node_back(lexer, new);
	(*i)++;
}

void	handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i] && str[*i + 1] == '<')
	{
		add_token_increment_i(lexer, "<<", TOKEN_HEREDOC);
		(*i) += 2;
	}
	else
	{
		add_token_increment_i(lexer, "<", TOKEN_REDIRECT_IN);
		(*i) += 1;
	}
}

void	handle_out_or_append(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i] && str[*i + 1] == '>')
	{
		add_token_increment_i(lexer, ">>", TOKEN_APPEND);
		(*i) += 2;
	}
	else
	{
		add_token_increment_i(lexer, ">", TOKEN_REDIRECT_OUT);
		(*i) += 1;
	}
}

char	*remove_escaped_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = malloc(get_real_string_length(str) + 1);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '"')
		{
			result[j++] = '"';
			i += 2;
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	handle_quotes(t_lexer **lexer, const char *str, int *i)
{
	t_quote_type	type;
	t_lexer			*new_node;
	int				len;
	char			*sub;
	char			*tmp;

	type = get_quote_type(str[*i]);
	len = get_quote_length(str, *i, str[*i]);
	sub = ft_substr(str, *i + 1, len - 1);
	if (str[*i] == '"')
	{
		tmp = remove_escaped_quotes(sub);
		free(sub);
		sub = tmp;
	}
	new_node = create_new_token_node(TOKEN_WORD, type, sub, 1);
	if ((*i + len + 1) < (int) ft_strlen(str) && !ft_isspace(str[*i
				+ len + 1]) && str[*i + len + 1] != '<' && str[*i + len
			+ 1] != '>' && str[*i + len + 1] != '|')
		new_node->space = 0;
	add_token_node_back(lexer, new_node);
	if (sub)
		free(sub);
	*i += len + 1;
}

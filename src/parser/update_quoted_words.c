#include "../../minishell.h"

void	process_escape_in_double_quotes(t_lexer *current, t_conv *conv)
{
	if (current->str[conv->i + 1]
		&& (current->str[conv->i + 1] == '\\'
			|| current->str[conv->i + 1] == '"'
			|| current->str[conv->i + 1] == '$'
			|| current->str[conv->i + 1] == '`'))
	{
		conv->result[conv->j++] = current->str[conv->i + 1];
		conv->i += 2;
	}
	else
		conv->result[conv->j++] = current->str[conv->i++];
}

void	process_char(t_tools *tools, t_lexer *current, t_conv *conv)
{
	if (current->str && current->str[conv->i] == '$')
		process_dollar(tools, current, conv);
	else if ((current->quote_type == DOUBLE_QUOTES)
		&& current->str[conv->i] == '\\')
		process_escape_in_double_quotes(current, conv);
	else
		conv->result[conv->j++] = current->str[conv->i++];
}

int	get_new_string_length(t_tools *tools, t_lexer *current)
{
	int	len;
	int	i;

	if (!current->str || !current->str[0])
		return (0);
	len = 0;
	i = 0;
	while (current->str[i])
	{
		if (current->str[i] == '$')
			process_dollar_len(tools, current, &i, &len);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*convert_string(t_tools *tools, t_lexer *current)
{
	t_conv	conv;
	int		length;

	length = get_new_string_length(tools, current) + 1;
	conv.result = ft_calloc(length, sizeof(char));
	if (!conv.result)
		return (NULL);
	conv.i = 0;
	conv.j = 0;
	while (current->str[conv.i])
		process_char(tools, current, &conv);
	return (conv.result);
}

void	update_double_quotes(t_tools *tools, t_lexer *current)
{
	char	*str;

	str = convert_string(tools, current);
	if (current->str)
		free(current->str);
	current->str = ft_strdup(str);
	free(str);
}

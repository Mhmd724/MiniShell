#include "../../minishell.h"

void	skip_spaces(const char *str, int *i)
{
	if (!str || !*str)
		return ;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}

void	handle_normal_string(t_lexer **lexer, const char *str, int *i)
{
	char	*sub;
	int		start;
	int		end;
	t_lexer	*new_node;

	start = *i;
	end = start;
	while (str[end] && !special_char(str[end]) && !ft_isspace(str[end]))
		end++;
	sub = ft_substr(str, start, end - start);
	new_node = create_new_token_node(TOKEN_WORD, NO_QUOTE, sub, 1);
	if (str[end] == '"' || str[end] == '\'')
		new_node->space = 0;
	add_token_node_back(lexer, new_node);
	if (sub)
		free(sub);
	*i = end;
}

void	handle_special_char(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i] == '>')
		handle_out_or_append(lexer, str, i);
	else if (str[*i] == '<')
		handle_in_or_heredoc(lexer, str, i);
	else if (str[*i] == '|')
		handle_pipe(lexer, i);
	else if (str[*i] == '\'' || str[*i] == '\"')
		handle_quotes(lexer, str, i);
}

void	process_string(t_lexer **lexer, const char *str, int *i)
{
	if (ft_isspace(str[*i]))
		skip_spaces(str, i);
	else if (special_char(str[*i]))
		handle_special_char(lexer, str, i);
	else
		handle_normal_string(lexer, str, i);
}

void	parse_string(t_lexer **lexer, const char *str)
{
	t_lexer	*eof_node;
	int		i;

	i = 0;
	eof_node = NULL;
	while (i < (int) ft_strlen(str))
		process_string(lexer, str, &i);
	eof_node = create_new_token_node(TOKEN_EOF, NO_QUOTE, NULL, -1);
	add_token_node_back(lexer, eof_node);
	add_index_token(lexer);
}

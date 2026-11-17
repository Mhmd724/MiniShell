#include "../../minishell.h"

void	handle_empty_split(t_lexer **lexer, t_lexer *to_del,
	char *str, char **split)
{
	free_lexer_node(lexer, to_del);
	if (!split || !(*split))
		free_split(split);
	free(str);
}

void	handle_one_string(t_lexer *to_del, char *str, char **split)
{
	if (to_del->str)
		free(to_del->str);
	to_del->str = ft_strdup(str);
	if (!to_del->str)
		return ;
	free_split(split);
	free(str);
}

void	handle_multiple_strings(t_lexer **sublexer, char **split,
	t_lexer *to_del, int count)
{
	int		i;
	int		space;
	t_lexer	*new_node;

	i = 0;
	space = 1;
	while (i < count)
	{
		if (i == count - 1)
			space = to_del->space;
		new_node = create_new_token_node(TOKEN_WORD, NO_QUOTE, split[i], space);
		if (!new_node)
			return ;
		add_token_node_back(sublexer, new_node);
		i++;
	}
}

void	inject_sublexer(t_lexer **lexer, t_lexer *sublexer, t_lexer *to_del)
{
	t_lexer	*temp_lexer;
	t_lexer	*temp_sublexer;

	if (!lexer || !*lexer || !sublexer || !to_del)
		return ;
	temp_sublexer = sublexer;
	while (temp_sublexer && temp_sublexer->next)
		temp_sublexer = temp_sublexer->next;
	if (*lexer == to_del)
		*lexer = sublexer;
	else
	{
		temp_lexer = *lexer;
		while (temp_lexer && temp_lexer->next != to_del)
			temp_lexer = temp_lexer->next;
		if (!temp_lexer)
			return ;
		temp_lexer->next = sublexer;
	}
	temp_sublexer->next = to_del->next;
	if (to_del->str)
		free(to_del->str);
	free(to_del);
}

void	update_no_quotes(t_tools *tools, t_lexer *head_lexer, t_lexer *current)
{
	int		i;
	char	*str;
	char	**split;
	t_lexer	*to_del;
	t_lexer	*sublexer;

	str = convert_string(tools, current);
	to_del = current;
	current = current->next;
	split = ft_split(str, ' ');
	if (!split || !*split)
		return (handle_empty_split(&head_lexer, to_del, str, split), (void)0);
	i = 0;
	while (split[i])
		i++;
	if (i == 1)
		return (handle_one_string(to_del, str, split), (void)0);
	sublexer = NULL;
	handle_multiple_strings(&sublexer, split, to_del, i);
	inject_sublexer(&head_lexer, sublexer, to_del);
	free_split(split);
	free(str);
}

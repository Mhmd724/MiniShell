#include "../../minishell.h"

t_lexer	*create_new_token_node(t_tokens token,
	t_quote_type quotes, const char *str, int space)
{
	t_lexer	*new_node;

	new_node = ft_calloc(1, sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	if (str)
		new_node->str = ft_strdup(str);
	else
		new_node->str = NULL;
	new_node->token = token;
	new_node->quote_type = quotes;
	new_node->space = space;
	new_node->i = -1;
	new_node->next = NULL;
	return (new_node);
}

void	add_token_node_back(t_lexer **lexer, t_lexer *new_node)
{
	t_lexer	*temp;

	if (!*lexer)
	{
		*lexer = new_node;
		return ;
	}
	temp = *lexer;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

void	add_index_token(t_lexer **lexer)
{
	t_lexer	*tmp;
	int		i;

	if (!lexer || !*lexer)
		return ;
	tmp = *lexer;
	i = 0;
	while (tmp)
	{
		tmp->i = i++;
		tmp = tmp->next;
	}
}

void	free_lexer(t_lexer **lexer)
{
	t_lexer	*tmp;
	t_lexer	*next_node;

	if (!lexer || !*lexer)
		return ;
	tmp = *lexer;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
		tmp = next_node;
	}
	*lexer = NULL;
}

void	free_lexer_node(t_lexer **lexer, t_lexer *to_del)
{
	t_lexer	*temp;

	if (!lexer || !*lexer || !to_del)
		return ;
	if (*lexer == to_del)
		*lexer = to_del->next;
	else
	{
		temp = *lexer;
		while (temp && temp->next != to_del)
			temp = temp->next;
		if (!temp)
			return ;
		temp->next = to_del->next;
	}
	if (to_del->str)
		free(to_del->str);
	free(to_del);
	to_del = NULL;
}

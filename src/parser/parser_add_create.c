#include "../../minishell.h"

t_parser	*create_new_parser_node(void)
{
	t_parser	*new_node;

	new_node = malloc(sizeof(t_parser));
	if (!new_node)
		return (NULL);
	new_node->tokens = NULL;
	new_node->builtin = NULL;
	new_node->redirects = NULL;
	new_node->hd_delimiters = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	add_parser_node_back(t_parser **parser)
{
	t_parser	*temp;
	t_parser	*new_node;

	if (!parser)
		return ;
	new_node = create_new_parser_node();
	if (!new_node)
		return ;
	if (!*parser)
	{
		*parser = new_node;
		return ;
	}
	temp = *parser;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	new_node->prev = temp;
}

void	free_parser_list(t_parser **parser)
{
	t_parser	*curr;
	t_parser	*next;

	if (!parser || !*parser)
		return ;
	curr = *parser;
	while (curr)
	{
		next = curr->next;
		if (curr->hd_delimiters)
			free_split(curr->hd_delimiters);
		if (curr->tokens)
			free_lexer(&curr->tokens);
		if (curr->redirects)
			free_lexer(&curr->redirects);
		free(curr);
		curr = next;
	}
	*parser = NULL;
}

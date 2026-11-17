#include "../../minishell.h"

void	fill_sublist(t_lexer **lexer, t_lexer **tmp, unsigned int nb_nodes)
{
	t_lexer			*new_node;
	unsigned int	i;

	i = 0;
	while (i < nb_nodes && (*tmp)->token != TOKEN_EOF)
	{
		new_node = create_new_token_node((*tmp)->token,
				(*tmp)->quote_type, (*tmp)->str, (*tmp)->space);
		if (!new_node)
			return ;
		add_token_node_back(lexer, new_node);
		i++;
		*tmp = (*tmp)->next;
	}
	if (*lexer)
		add_index_token(lexer);
}

t_lexer	*ft_sublist(t_lexer *lexer, t_lexer *start_node, unsigned int nb_nodes)
{
	t_lexer	*temp;
	t_lexer	*sublist;

	if (!lexer || !nb_nodes)
		return (NULL);
	if (!start_node)
		return (NULL);
	temp = start_node;
	sublist = NULL;
	fill_sublist(&sublist, &temp, nb_nodes);
	return (sublist);
}

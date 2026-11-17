#include "../../minishell.h"

void	merge_nodes(t_lexer **lexer, t_lexer *current)
{
	t_lexer	*to_del;

	(void)lexer;
	to_del = current->next;
	if (!to_del)
		return ;
	current->str = ft_strjoin(current->str, to_del->str);
	current->next = to_del->next;
	current->space = to_del->space;
	current->quote_type = SINGLE_QUOTES;
	if (to_del->str)
		free(to_del->str);
	free(to_del);
	to_del = NULL;
}

void	cat_heredoc_word(t_lexer **lexer, t_lexer *current_node)
{
	t_lexer	*lexer_node;

	if (!current_node || current_node->space)
		return ;
	lexer_node = current_node;
	while (lexer_node && lexer_node->next && !lexer_node->space)
		merge_nodes(lexer, lexer_node);
}

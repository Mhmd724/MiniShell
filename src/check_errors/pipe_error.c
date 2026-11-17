#include "../../minishell.h"

int	validate_pipes_and_words(t_parser *head)
{
	t_parser	*current;
	t_lexer		*lexer_node;

	if (head == NULL)
		return (1);
	current = head;
	while (current != NULL)
	{
		if (current->tokens == NULL)
			return (0);
		lexer_node = current->tokens;
		while (lexer_node != NULL)
		{
			if (lexer_node->token == TOKEN_WORD)
				break ;
			lexer_node = lexer_node->next;
		}
		if (lexer_node == NULL && current->next != NULL)
			return (0);
		current = current->next;
	}
	return (1);
}

int	check_pipe_error(t_tools *tools)
{
	if (!validate_pipes_and_words(tools->parser))
	{
		print_syntax_error("|", &(tools->last_exit_status));
		free_tools(tools);
		return (0);
	}
	return (1);
}

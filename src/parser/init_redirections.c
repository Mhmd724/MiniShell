#include "../../minishell.h"

void	add_to_redirects(t_lexer **redirects,
	t_lexer *redirection, t_lexer *redirect_to)
{
	t_lexer	*new_node;

	new_node = create_new_token_node(redirection->token,
			redirection->quote_type, redirection->str, redirection->space);
	if (!new_node)
		return ;
	add_token_node_back(redirects, new_node);
	new_node = create_new_token_node(redirect_to->token,
			redirect_to->quote_type, redirect_to->str, redirect_to->space);
	if (!new_node)
		return ;
	add_token_node_back(redirects, new_node);
}

void	create_redirects_list(t_lexer **redirects, t_lexer **lexer)
{
	t_lexer	*to_del;
	t_lexer	*redirect_to;
	t_lexer	*tmp;

	tmp = *lexer;
	while (tmp)
	{
		if (is_redirection_token(tmp->token))
		{
			to_del = tmp;
			redirect_to = to_del->next;
			add_to_redirects(redirects, to_del, redirect_to);
			if (redirect_to)
				tmp = redirect_to->next;
			else
				tmp = NULL;
			free_lexer_node(lexer, to_del);
			if (redirect_to)
				free_lexer_node(lexer, redirect_to);
		}
		else
			tmp = tmp->next;
	}
}

void	init_redirections(t_parser *parser)
{
	t_parser	*temp;

	if (!parser)
		return ;
	temp = parser;
	while (temp && temp->tokens)
	{
		create_redirects_list(&temp->redirects, &temp->tokens);
		temp = temp->next;
	}
}

#include "../../minishell.h"

void	cat_heredoc_words(t_parser *parser)
{
	t_lexer	*temp_lexer;

	if (!parser || !parser->tokens)
		return ;
	temp_lexer = parser->tokens;
	while (temp_lexer)
	{
		if (temp_lexer->token == TOKEN_HEREDOC)
		{
			cat_heredoc_word(&(parser->tokens), temp_lexer->next);
			if (temp_lexer->next && temp_lexer->next->next)
				temp_lexer = temp_lexer->next->next;
			else
				break ;
			continue ;
		}
		else
			temp_lexer = temp_lexer->next;
	}
	add_index_token(&parser->tokens);
}

void	update_parser_words(t_tools *tools, t_parser **parser)
{
	t_lexer	*tmp;

	if (!parser || !*parser || !(*parser)->tokens)
		return ;
	tmp = (*parser)->tokens;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
		{
			tmp = tmp->next->next;
			continue ;
		}
		else if (tmp->token == TOKEN_WORD)
		{
			if (tmp->quote_type == NO_QUOTE)
				update_no_quotes(tools, (*parser)->tokens, tmp);
			else if (tmp->quote_type == DOUBLE_QUOTES)
				update_double_quotes(tools, tmp);
		}
		tmp = tmp->next;
	}
}

void	cat_words(t_lexer *lexer)
{
	t_lexer	*to_del;
	t_lexer	*temp;

	if (!lexer)
		return ;
	temp = lexer;
	while (temp && temp->next)
	{
		if (temp->space == 0)
		{
			to_del = temp->next;
			concat_tokens(temp, to_del);
			if (temp->next->space)
				temp = temp->next->next;
			free_lexer_node(&lexer, to_del);
		}
		else
			temp = temp->next;
	}
}

void	update_parser_nodes(t_tools *tools)
{
	t_parser	*temp_parser;

	if (!tools->parser)
		return ;
	temp_parser = tools->parser;
	while (temp_parser)
	{
		cat_heredoc_words(temp_parser);
		update_parser_words(tools, &temp_parser);
		cat_words(temp_parser->tokens);
		add_index_token(&(temp_parser)->tokens);
		temp_parser = temp_parser->next;
	}
}

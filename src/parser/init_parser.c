#include "../../minishell.h"

static t_lexer	*find_next_pipe(t_lexer *lexer)
{
	while (lexer && lexer->token != TOKEN_PIPE)
		lexer = lexer->next;
	return (lexer);
}

static void	process_parser_node(t_parser **parser, t_assign *assign)
{
	t_lexer	*pipe;

	pipe = find_next_pipe(assign->lexer);
	if (pipe)
	{
		(*parser)->tokens = ft_sublist(assign->lexer_head,
				assign->start_node,
				pipe->i - assign->start_node->i);
		assign->start_node = pipe->next;
		assign->lexer = pipe->next;
	}
	else
	{
		(*parser)->tokens = ft_sublist(assign->lexer_head,
				assign->start_node,
				assign->nb_nodes - assign->start_node->i);
	}
}

void	assign_tokens_to_parser(t_parser **parser, t_lexer *lexer,
		unsigned int tokens_nb)
{
	t_assign	assign;
	t_parser	*temp;

	temp = *parser;
	assign.lexer = lexer;
	assign.start_node = lexer;
	assign.lexer_head = lexer;
	assign.nb_nodes = tokens_nb;
	while (temp && assign.lexer)
	{
		if (assign.start_node && assign.start_node->token == TOKEN_EOF)
			break ;
		process_parser_node(&temp, &assign);
		temp = temp->next;
	}
}

void	init_parser_nodes(t_parser **parser, t_lexer *lexer, const char *str)
{
	int				pipes_nb;
	unsigned int	tokens_nb;
	int				i;

	if (!str || !str[0])
		return ;
	if (!lexer || lexer->token == TOKEN_EOF)
		return ;
	tokens_nb = 0;
	pipes_nb = count_pipes(lexer, &tokens_nb);
	i = 0;
	while (i++ <= pipes_nb)
		add_parser_node_back(parser);
	assign_tokens_to_parser(parser, lexer, tokens_nb);
}

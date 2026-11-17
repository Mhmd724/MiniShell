#include "../../minishell.h"

static void	fill_hd_array(t_parser *parser)
{
	t_lexer	*redirects;
	int		i;

	redirects = parser->redirects;
	i = 0;
	while (redirects)
	{
		if (redirects->token == TOKEN_HEREDOC && redirects->next)
		{
			parser->hd_delimiters[i] = ft_strdup(redirects->next->str);
			if (!parser->hd_delimiters[i])
				return ;
			i++;
		}
		redirects = redirects->next;
	}
	parser->hd_delimiters[i] = NULL;
}

static void	set_hd_delimiters_in_node(t_parser *parser)
{
	t_lexer	*temp;
	int		heredoc_nb;

	if (!parser || !parser->redirects)
		return ;
	temp = parser->redirects;
	heredoc_nb = 0;
	while (temp)
	{
		if (temp->token == TOKEN_HEREDOC)
			heredoc_nb++;
		temp = temp->next;
	}
	parser->hd_delimiters = ft_calloc(heredoc_nb + 1, sizeof(char *));
	if (!parser->hd_delimiters)
		return ;
	fill_hd_array(parser);
}

void	init_heredoc_delimiters(t_parser *parser)
{
	t_parser	*tmp;

	if (!parser)
		return ;
	tmp = parser;
	while (tmp)
	{
		set_hd_delimiters_in_node(tmp);
		tmp = tmp->next;
	}
}

#include "../../minishell.h"

int	process_redirects_heredoc(t_lexer *redirects, t_tools *tools)
{
	char	*delimiter;
	char	*filename;

	if (!redirects->next || redirects->next->token != TOKEN_WORD)
		return (print_syntax_error("newline", NULL), -1);
	delimiter = redirects->next->str;
	filename = handle_heredoc_filename(tools, &delimiter,
			redirects->next->quote_type);
	if (!filename)
	{
		if (g_signum == SIGINT)
		{
			g_signum = 0;
			return (130);
		}
		return (-1);
	}
	redirects->token = TOKEN_REDIRECT_IN;
	free(redirects->next->str);
	redirects->next->str = filename;
	return (0);
}

int	prepare_redirects_heredoc(t_tools *tools, t_lexer *parse_redirects)
{
	t_lexer	*redirects;
	int		result;

	redirects = parse_redirects;
	while (redirects)
	{
		if (redirects->token == TOKEN_HEREDOC)
		{
			result = process_redirects_heredoc(redirects, tools);
			if (result)
				return (result);
		}
		redirects = redirects->next;
	}
	return (0);
}

int	process_all_heredocs(t_tools *tools)
{
	t_parser	*curr;
	int			result;

	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	curr = tools->parser;
	while (curr)
	{
		result = prepare_redirects_heredoc(tools, curr->redirects);
		if (result)
		{
			set_signals();
			return (result);
		}
		curr = curr->next;
	}
	set_signals();
	return (0);
}

#include "../../minishell.h"

int	get_cmd_len(t_lexer *lexer)
{
	int	count;

	if (!lexer)
		return (0);
	count = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		count++;
		lexer = lexer->next;
	}
	return (count);
}

t_cmd_ctx	*get_cmd(t_lexer *lexer)
{
	t_cmd_ctx	*cmd;
	int			len;
	int			i;

	len = get_cmd_len(lexer) + 1;
	if (!lexer || !len)
		return (NULL);
	cmd = (t_cmd_ctx *)malloc(sizeof(t_cmd_ctx) * (len));
	if (!cmd)
		return (NULL);
	i = 0;
	while (lexer && i < len)
	{
		cmd[i].str = ft_strdup(lexer->str);
		cmd[i].non_quoted = (lexer->quote_type == NO_QUOTE);
		cmd[i].single_quotes = (lexer->quote_type == SINGLE_QUOTES);
		lexer = lexer->next;
		i++;
	}
	cmd[i].str = NULL;
	return (cmd);
}

void	free_args(t_cmd_ctx *args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i].str)
	{
		if (args[i].str)
			free(args[i].str);
		i++;
	}
	free(args);
}

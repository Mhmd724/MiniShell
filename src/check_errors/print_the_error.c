#include "../../minishell.h"

void	print_syntax_error(char *msg, int *status)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd("\'", STDERR_FILENO);
	if (status)
		*status = 2;
}

void	free_tools(t_tools *tools)
{
	free_lexer(&(tools->lexer));
	free_parser_list(&(tools->parser));
	tools = NULL;
}

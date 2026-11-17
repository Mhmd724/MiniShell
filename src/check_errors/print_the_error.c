#include "../../minishell.h"

void	print_syntax_error(char *msg, int *status)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd("\'", STDERR_FILENO);
	if (status)
		*status = 2;
}

void	print_cmd_error(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	free_tools(t_tools *tools)
{
	free_lexer(&(tools->lexer));
	free_parser_list(&(tools->parser));
	tools = NULL;
}

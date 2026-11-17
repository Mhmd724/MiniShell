#include "../../minishell.h"

int	check_exclamation(char *prompt, t_tools *tools)
{
	if (prompt[0] == '!' && prompt[1] == '\0')
	{
		tools->last_exit_status = 2;
		return (free_tools(tools), 0);
	}
	return (1);
}

int	check_colon(char *prompt, t_tools *tools)
{
	if (prompt[0] == ':' && prompt[1] == '\0')
	{
		tools->last_exit_status = 0;
		return (free_tools(tools), 0);
	}
	return (1);
}

int	check_hash(char *prompt, t_tools *tools)
{
	if (prompt[0] == '#' && prompt[1] == '\0')
	{
		tools->last_exit_status = 0;
		return (free_tools(tools), 0);
	}
	return (1);
}

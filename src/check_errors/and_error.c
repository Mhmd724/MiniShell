#include "../../minishell.h"

/*	To check &, &&, &&&, ... (only & in the string)	*/
int	is_only_ands(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] != '&')
			return (0);
		i++;
	}
	return (i);
}

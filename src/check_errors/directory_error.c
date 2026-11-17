#include "../../minishell.h"

/*
	This function is specifically used to detect if the user
	is trying to “run” or execute a directory as a command.
	If prompt is a directory-like path (e.g., /, /.., /.),
	the shell shouldn’t try to execute it.
*/
int	is_directory_path(char *str)
{
	int	i;
	int	j;

	if (!str || str[0] != '/')
		return (0);
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == '/')
			i++;
		if (!str[i])
			break ;
		j = i;
		while (str[j] && str[j] != '/')
			j++;
		if (j - i != 1 && j - i != 2)
			return (0);
		if (j - i == 1 && str[i] != '.')
			return (0);
		if (j - i == 2 && (str[i] != '.' || str[i + 1] != '.'))
			return (0);
		i = j;
	}
	return (1);
}

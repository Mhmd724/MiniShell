#include "../../minishell.h"

int	print_char(char c)
{
	if (c == '\\')
		return (ft_putchar_fd('\\', STDOUT_FILENO), 1);
	if (c == 'n')
		return (ft_putchar_fd('\n', STDOUT_FILENO), 1);
	if (c == 't')
		return (ft_putchar_fd('\t', STDOUT_FILENO), 1);
	return (0);
}

int	get_len(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
			i += 2;
		else
			i++;
		count++;
	}
	return (count);
}

void	handle_escape_unquoted(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		len;

	len = get_len(str) + 1;
	new_str = malloc(len);
	if (!new_str)
		return ;
	i = 0;
	j = 0;
	while (j < len)
	{
		if (str[i] == '\\' && str[i + 1])
		{
			new_str[j++] = str[i + 1];
			i += 2;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	handle_esc_quoted(new_str);
	free(new_str);
}

void	handle_esc_quoted(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			if (print_char(str[i + 1]))
				i += 2;
			else
				ft_putchar_fd(str[i++], STDOUT_FILENO);
		}
		else
			ft_putchar_fd(str[i++], STDOUT_FILENO);
	}
}

void	print_with_escape(char *str, int unquoted)
{
	if (unquoted)
		handle_escape_unquoted(str);
	else
		handle_esc_quoted(str);
}

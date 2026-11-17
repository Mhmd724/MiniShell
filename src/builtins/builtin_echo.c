#include "../../minishell.h"

int	flags_ok(const char c, int *new_line, int *escape)
{
	if (c == 'n' || c == 'e' || c == 'E')
	{
		if (c == 'n')
			*new_line = 0;
		else if (c == 'e')
			*escape = 1;
		else if (c == 'E')
			*escape = 0;
		return (1);
	}
	return (0);
}

int	handle_echo_flags(t_cmd_ctx *args, int *new_line, int *escape)
{
	int	i;
	int	j;

	*new_line = 1;
	*escape = 0;
	i = 0;
	while (args[i].str && args[i].str[0] == '-'
		&& args[i].str[1])
	{
		j = 1;
		while (args[i].str[j])
		{
			if (!flags_ok(args[i].str[j], new_line, escape))
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

void	print_no_escape(const char *str, int unquoted)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (unquoted)
		{
			if (str[i] == '\\' && str[i + 1])
			{
				printf("%c", str[i + 1]);
				i += 2;
			}
			else
				printf("%c", str[i++]);
		}
		else
			printf("%c", str[i++]);
	}
}

void	printf_arguments(t_cmd_ctx *args, int escape, int *i)
{
	if (escape)
	{
		print_with_escape(args[*i].str, args[*i].non_quoted);
		if (args[*i + 1].str)
			printf(" ");
		(*i)++;
		return ;
	}
	if (!args[*i].single_quotes)
		print_no_escape(args[*i].str, args[*i].non_quoted);
	else
		printf("%s", args[*i].str);
	if (args[*i + 1].str)
		printf(" ");
	(*i)++;
}

int	builtin_echo(t_tools *tools, t_parser *parser, t_env **env)
{
	t_cmd_ctx	*args;
	int			new_line;
	int			escape;
	int			i;

	(void)tools;
	(void)env;
	args = get_cmd(parser->tokens->next);
	if (!args || !args[0].str)
	{
		free_args(args);
		printf("\n");
		return (0);
	}
	i = handle_echo_flags(args, &new_line, &escape);
	while (args[i].str)
		printf_arguments(args, escape, &i);
	if (new_line)
		printf("\n");
	return (free_args(args), 0);
}

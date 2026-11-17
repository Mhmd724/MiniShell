#include "../../minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '-')
	{
		if (!str[i + 1])
			return (0);
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_cmp(char *a1, char *a2)
{
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(a1);
	len2 = ft_strlen(a2);
	if (len1 < len2)
		return (-1);
	else if (len1 > len2)
		return (1);
	else
		return (ft_strncmp(a1, a2, len1));
}

int	check_overflow(char *str)
{
	if (str[0] == '-')
	{
		if (ft_cmp("-9223372036854775808", str) >= 0)
			return (1);
		else
			return (0);
	}
	else
	{
		if (ft_cmp("9223372036854775807", str) >= 0)
			return (1);
		else
			return (0);
	}
}

int	print_error(char *arg)
{
	print_cmd_error("exit", arg, "numeric argument required");
	return (2);
}

int	builtin_exit(t_tools *tools, t_parser *parser, t_env **env)
{
	t_cmd_ctx	*arg;
	int			exit_status;

	(void)env;
	arg = get_cmd(parser->tokens);
	if (!arg || !arg[0].str)
		exit(0);
	if (!arg[1].str)
		exit(tools->last_exit_status);
	if (is_numeric(arg[1].str))
	{
		if (arg[2].str)
			return (print_cmd_error("exit", NULL, "too many arguments"),
				free_args(arg), 1);
		if (!check_overflow(arg[1].str))
			exit_status = print_error(arg[1].str);
		else
			exit_status = ft_atoi(arg[1].str);
	}
	else
		exit_status = print_error(arg[1].str);
	free_args(arg);
	exit(exit_status);
}

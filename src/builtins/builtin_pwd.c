#include "../../minishell.h"

void	ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i])
		i++;
	j = 0;
	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
}

t_env	*fetch_pwd(t_env *env)
{
	t_env	*pwd;

	pwd = get_env_var(env, "PWD");
	if (!pwd->value)
		print_cmd_error("pwd", NULL, "PWD not set");
	return (pwd);
}

int	print_pwd(t_env *env)
{
	t_env	*pwd;
	int		nbr;
	int		i;

	if (!env)
		return (1);
	nbr = env->two_point;
	if (nbr < 0)
		nbr = 0;
	pwd = fetch_pwd(env);
	if (!pwd)
		return (1);
	i = 0;
	while (i < nbr)
	{
		ft_strcat(pwd->value, "/..");
		i++;
	}
	ft_putendl_fd(pwd->value, STDOUT_FILENO);
	return (0);
}

int	builtin_pwd(t_tools *tools, t_parser *parser, t_env **env)
{
	char	cwd[PATH_MAX];

	(void)tools;
	(void)parser;
	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	else
	{
		if (!env)
		{
			print_cmd_error("pwd", NULL, "environment list is empty");
			return (1);
		}
		return (print_pwd(*env));
	}
}

#include "../../minishell.h"

// ANSI color codes
#define BOLD  "\001\033[1m\002"
#define CYAN  "\001\033[36m\002"
#define GREEN "\001\033[32m\002"
#define RESET "\001\033[0m\002"

char	*build_colored_prompt(const char *path)
{
	char	*part1;
	char	*part2;
	char	*full;

	part1 = ft_strjoin(GREEN BOLD
			"minishell:" RESET " ", CYAN BOLD);
	if (!part1)
		return (NULL);
	part2 = ft_strjoin(part1, path);
	free(part1);
	if (!part2)
		return (NULL);
	full = ft_strjoin(part2, RESET BOLD "$ " RESET);
	free(part2);
	return (full);
}

char	*get_input(t_tools *tools)
{
	t_env	*tmp;
	char	*home;
	char	*path;
	char	*prompt;
	char	*ret;

	tmp = tools->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "HOME") == 0)
			home = "~";
		if (ft_strcmp(tmp->key, "PWD") == 0)
			path = tmp->value;
		tmp = tmp->next;
	}
	if (path)
		prompt = build_colored_prompt(path);
	else
		prompt = build_colored_prompt(home);
	if (!prompt)
		return (NULL);
	ret = readline(prompt);
	free(prompt);
	return (ret);
}

// char    *get_input(t_tools *tools)
// {
//     (void)tools;
//     return (readline("minishell >> "));
// }

int	ft_isspacee(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (0);
	return (1);
}

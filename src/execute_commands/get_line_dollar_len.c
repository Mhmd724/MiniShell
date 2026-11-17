#include "../../minishell.h"

void	line_question_mark_len(t_tools *tools, int *i, size_t *len)
{
	char	*str;

	str = ft_itoa(tools->last_exit_status);
	*len += ft_strlen(str);
	free(str);
	(*i)++;
}

void	line_special_case_len(char *line, int *i, size_t *len)
{
	char	*str;
	int		j;

	if (line[*i] && ft_isdigit(line[*i]))
	{
		(*i)++;
		j = 0;
		while (line[*i + j] && ft_isalnum(line[*i + j]))
			j++;
		*len += j;
	}
	else if (line[*i] && line[*i] == '$')
	{
		str = ft_itoa((int)get_pid());
		if (!str)
			return ;
		*len += ft_strlen(str);
		free(str);
		(*i)++;
	}
}

void	get_line_dollar_len(t_tools *tools, char *line,
	int *i, size_t *len)
{
	char	*sub;
	t_env	*env_var;
	int		j;

	if (line[++(*i)] && line[*i] == '?')
		return (line_question_mark_len(tools, i, len), (void)0);
	if (is_a_special_case(line, i))
		return (line_special_case_len(line, i, len), (void)0);
	j = 0;
	while (line[*i + j] && (ft_isalnum(line[*i + j])
			|| line[*i + j] == '_'))
		j++;
	sub = ft_substr(line, *i, j);
	if (!sub || !*sub)
	{
		free(sub);
		return ;
	}
	env_var = get_env_var(tools->env, sub);
	if (env_var && env_var->value)
		*len += ft_strlen(env_var->value);
	(free(sub), *i += j);
}

int	get_line_length(t_tools *tools, char *line)
{
	size_t	len;
	int		i;

	if (!line || !line[0])
		return (0);
	len = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			get_line_dollar_len(tools, line, &i, &len);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

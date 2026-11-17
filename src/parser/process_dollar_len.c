#include "../../minishell.h"

void	handle_question_mark_len(t_tools *tools, int *i, int *len)
{
	char	*str;

	str = ft_itoa(tools->last_exit_status);
	*len += ft_strlen(str);
	free(str);
	(*i)++;
}

void	handle_special_case_len(t_lexer *current, int *i, int *len)
{
	char	*str;
	int		j;

	if (current->str[*i] && ft_isdigit(current->str[*i]))
	{
		(*i)++;
		j = 0;
		while (current->str[*i + j] && ft_isalnum(current->str[*i + j]))
			j++;
		*len += j;
	}
	else if (current->str[*i] && current->str[*i] == '$')
	{
		str = ft_itoa((int)get_pid());
		if (!str)
			return ;
		*len += ft_strlen(str);
		free(str);
		(*i)++;
	}
}

void	process_dollar_len(t_tools *tools, t_lexer *current,
	int *i, int *len)
{
	char	*sub;
	t_env	*env_var;
	int		j;

	if (current->str[++(*i)] && current->str[*i] == '?')
		return (handle_question_mark_len(tools, i, len), (void)0);
	if (is_a_special_case(current->str, i))
		return (handle_special_case_len(current, i, len), (void)0);
	j = 0;
	while (current->str[*i + j] && (ft_isalnum(current->str[*i + j])
			|| current->str[*i + j] == '_'))
		j++;
	sub = ft_substr(current->str, *i, j);
	if (!sub || !*sub)
	{
		if (current->space || current->quote_type == DOUBLE_QUOTES)
			*len += 1;
		free(sub);
		return ;
	}
	env_var = get_env_var(tools->env, sub);
	if (env_var && env_var->value)
		*len += ft_strlen(env_var->value);
	(free(sub), *i += j);
}

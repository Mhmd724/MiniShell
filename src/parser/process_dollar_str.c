#include "../../minishell.h"

void	handle_question_mark_str(t_tools *tools, int size, t_conv *conv)
{
	char	*str;

	str = ft_itoa(tools->last_exit_status);
	ft_strlcat(conv->result, str, size);
	free(str);
	conv->i++;
}

void	handle_special_case_str(t_lexer *current, t_conv *conv,
	int size)
{
	char	*sub;
	int		j;

	if (current->str[conv->i] && ft_isdigit(current->str[conv->i]))
	{
		conv->i++;
		j = 0;
		while (current->str[conv->i + j]
			&& ft_isalnum(current->str[conv->i + j]))
			j++;
		sub = ft_substr(current->str, conv->i, j);
		if (sub)
			ft_strlcat(conv->result, sub, size);
		conv->i += j;
		free(sub);
		return ;
	}
	if (current->str[conv->i] && current->str[conv->i] == '$')
	{
		sub = ft_itoa((int)get_pid());
		ft_strlcat(conv->result, sub, size);
		conv->i++;
		free(sub);
	}
}

void	handle_empty_sub(t_lexer *current, t_conv *conv,
	char **sub, int size)
{
	if (current->space || current->quote_type == DOUBLE_QUOTES)
		ft_strlcat(conv->result, "$", size);
	free(*sub);
}

void	handle_dollar_sign_str(t_tools *tools, t_lexer *current,
	t_conv *conv)
{
	int		size;
	int		j;
	char	*sub;
	t_env	*env_var;

	conv->i++;
	size = get_new_string_length(tools, current) + 1;
	if (current->str[conv->i] && current->str[conv->i] == '?')
		return (handle_question_mark_str(tools, size, conv), (void)0);
	if (is_a_special_case(current->str, &conv->i))
		return (handle_special_case_str(current, conv, size), (void)0);
	j = 0;
	while (current->str[conv->i + j] && (ft_isalnum(current->str[
					conv->i + j]) || current->str[conv->i + j] == '_'))
		j++;
	sub = ft_substr(current->str, conv->i, j);
	if (!sub || !*sub)
		return (handle_empty_sub(current, conv, &sub, size), (void)0);
	env_var = get_env_var(tools->env, sub);
	if (env_var && env_var->value)
		ft_strlcat(conv->result, env_var->value, size);
	conv->i += j;
	free(sub);
}

void	process_dollar(t_tools *tools, t_lexer *current, t_conv *conv)
{
	handle_dollar_sign_str(tools, current, conv);
	conv->j = ft_strlen(conv->result);
}

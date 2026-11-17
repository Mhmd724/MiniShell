#include "../../minishell.h"

void	expand_exit_status(t_tools *tools, t_exp_ctx *ctx)
{
	char	*str;
	size_t	len;
	size_t	k;

	str = ft_itoa(tools->last_exit_status);
	if (!str)
		return ;
	len = ft_strlen(str);
	k = 0;
	while (k < len)
		ctx->expanded[ctx->j++] = str[k++];
	ctx->i++;
	free(str);
}

void	expand_dollar_nb(t_exp_ctx *ctx)
{
	char	pid_str[12];
	size_t	k;

	snprintf(pid_str, sizeof(pid_str), "%d", getpid());
	k = 0;
	while (pid_str[k])
		ctx->expanded[ctx->j++] = pid_str[k++];
	ctx->i++;
}

void	expand_env_variable(t_exp_ctx *ctx)
{
	char	*sub;
	t_env	*env;
	int		start;

	start = ctx->i;
	while (ft_isalnum(ctx->line[ctx->i])
		|| ctx->line[ctx->i] == '_')
		ctx->i++;
	sub = ft_substr(ctx->line, start, ctx->i - start);
	if (!sub || !*sub)
	{
		free(sub);
		return ;
	}
	env = get_env_var(ctx->env, sub);
	free(sub);
	if (!env || !env->value)
		return ;
	start = 0;
	while (env->value[start])
		ctx->expanded[ctx->j++] = env->value[start++];
}

void	process_expansion(t_exp_ctx *exp_ctx, t_tools *tools)
{
	if (exp_ctx->line[exp_ctx->i])
	{
		if (exp_ctx->line[exp_ctx->i] == '$')
		{
			exp_ctx->i++;
			if (exp_ctx->line[exp_ctx->i] == '?')
				expand_exit_status(tools, exp_ctx);
			else if (exp_ctx->line[exp_ctx->i] == '$')
				expand_dollar_nb(exp_ctx);
			else if (ft_isalnum(exp_ctx->line[exp_ctx->i])
				|| exp_ctx->line[exp_ctx->i] == '_')
				expand_env_variable(exp_ctx);
		}
		else
			exp_ctx->expanded[exp_ctx->j++] = exp_ctx->line[exp_ctx->i++];
	}
	exp_ctx->expanded[exp_ctx->j] = '\0';
}

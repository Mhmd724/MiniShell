#include "../../minishell.h"

static	int	heredoc_setup(t_heredoc_ctx *ctx, char **delimiter,
	t_quote_type quote_type)
{
	ctx->stdin_copy = dup(STDIN_FILENO);
	if (ctx->stdin_copy == -1)
		return (perror("minishell"), -1);
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	g_signum = 0;
	if (!delimiter || !delimiter[0])
		return (close(ctx->stdin_copy), reset_signals(), -1);
	ctx->delimiter = ft_strtrim(delimiter[0], " \t\n");
	if (!ctx->delimiter)
	{
		perror("minishell");
		return (close(ctx->stdin_copy), reset_signals(), -1);
	}
	ctx->tmp_fd = mkstemp(ctx->template);
	if (ctx->tmp_fd == -1)
	{
		perror("minishell");
		free(ctx->delimiter);
		return (close(ctx->stdin_copy), reset_signals(), -1);
	}
	ctx->expanded_variable = (quote_type == NO_QUOTE);
	return (0);
}

static char	*heredoc_cleanup(t_heredoc_ctx *ctx)
{
	dup2(ctx->stdin_copy, STDIN_FILENO);
	close(ctx->stdin_copy);
	close(ctx->tmp_fd);
	free(ctx->delimiter);
	reset_signals();
	if (g_signum == SIGINT)
	{
		unlink(ctx->template);
		return (NULL);
	}
	return (ft_strdup(ctx->template));
}

char	*handle_heredoc_filename(t_tools *tools, char **delimiter,
	t_quote_type quote_type)
{
	t_heredoc_ctx	context;
	int				length;

	length = ft_strlen("/tmp/myminishell_heredoccXXXXXX") + 1;
	ft_strlcpy(context.template, "/tmp/myminishell_heredoccXXXXXX", length);
	if (heredoc_setup(&context, delimiter, quote_type))
		return (NULL);
	if (heredoc_loop(tools, &context))
		return (NULL);
	return (heredoc_cleanup(&context));
}

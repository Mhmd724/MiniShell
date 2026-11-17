#include "../../minishell.h"

int	handle_sigint(t_heredoc_ctx *ctx, char *line)
{
	dup2(ctx->stdin_copy, STDIN_FILENO);
	close(ctx->stdin_copy);
	close(ctx->tmp_fd);
	if (line)
		free (line);
	free(ctx->delimiter);
	unlink(ctx->template);
	reset_signals();
	return (-1);
}

void	print_heredoc_error(char *delimiter)
{
	char	*nb;

	nb = ft_itoa(g_line_nb);
	ft_putstr_fd("-minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited at line ", 2);
	ft_putstr_fd(nb, 2);
	ft_putstr_fd(" by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	free(nb);
}

char	*expand_heredoc_line(t_tools *tools, char *line)
{
	t_exp_ctx	exp_ctx;
	int			size;

	size = get_line_length(tools, line) + 1;
	exp_ctx.expanded = (char *)malloc(sizeof(char) * size);
	if (!exp_ctx.expanded)
		return (NULL);
	exp_ctx.line = line;
	exp_ctx.i = 0;
	exp_ctx.j = 0;
	exp_ctx.env = tools->env;
	while (exp_ctx.line[exp_ctx.i])
		process_expansion(&exp_ctx, tools);
	return (exp_ctx.expanded);
}

int	process_heredoc_line(t_tools *tools, t_heredoc_ctx *ctx, char *line)
{
	char	*expanded_line;

	if (!ft_strcmp(ctx->delimiter, line))
		return (free(line), 1);
	if (ctx->expanded_variable)
	{
		expanded_line = expand_heredoc_line(tools, line);
		ft_putendl_fd(expanded_line, ctx->tmp_fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, ctx->tmp_fd);
	free(line);
	return (0);
}

int	heredoc_loop(t_tools *tools, t_heredoc_ctx *ctx)
{
	char	*line;
	int		count;

	count = 0;
	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
			return (handle_sigint(ctx, line));
		if (!line)
		{
			print_heredoc_error(ctx->delimiter);
			g_line_nb += count;
			break ;
		}
		if (process_heredoc_line(tools, ctx, line))
			break ;
		count++;
	}
	return (0);
}

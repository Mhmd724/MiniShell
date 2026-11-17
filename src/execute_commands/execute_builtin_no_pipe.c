#include "../../minishell.h"

int	redirect_fd_for_token(int fd, t_tokens token)
{
	if (token == TOKEN_REDIRECT_IN || token == TOKEN_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			return (-1);
		}
	}
	return (0);
}

int	open_fd_for_redirections(t_tools *tools, t_lexer *current)
{
	char	*filename;

	(void)tools;
	if (!current->next || current->next->token != TOKEN_WORD)
		return (write(2, "syntax error near unexpected token\n", 35), -1);
	filename = current->next->str;
	if (!filename)
		return (write(2, "error processing redirection\n", 30), -1);
	if (current->token == TOKEN_REDIRECT_IN)
		return (open(filename, O_RDONLY));
	if (current->token == TOKEN_REDIRECT_OUT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (current->token == TOKEN_APPEND)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (write(2, "syntax error near unexpected token\n", 35), -1);
}

int	setup_redirections_for_builtin(t_tools *tools, t_lexer *redirects)
{
	t_lexer	*curr;
	int		fd;

	curr = redirects;
	while (curr)
	{
		fd = open_fd_for_redirections(tools, curr);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(curr->next->str);
			return (-1);
		}
		if (redirect_fd_for_token(fd, curr->token) == -1)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		curr = curr->next->next;
	}
	return (0);
}

int	execute_builtin_no_pipe(t_tools *tools, t_parser *parser)
{
	int	stdin_copy;
	int	stdout_copy;
	int	stderr_copy;
	int	exit_status;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	stderr_copy = dup(STDERR_FILENO);
	if (parser->redirects && setup_redirections_for_builtin(
			tools, parser->redirects))
		exit_status = 1;
	else
		exit_status = parser->builtin(tools, parser, &tools->env);
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdout_copy);
	dup2(stderr_copy, STDERR_FILENO);
	close(stderr_copy);
	return (exit_status);
}

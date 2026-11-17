#include "../../minishell.h"

int	open_file(t_lexer *curr, const char *file)
{
	if (curr->token == TOKEN_REDIRECT_IN)
		return (open(file, O_RDONLY));
	if (curr->token == TOKEN_REDIRECT_OUT)
		return (open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (curr->token == TOKEN_APPEND)
		return (open(file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	print_syntax_error("newline", NULL);
	return (-1);
}

int	get_redirection_fd(t_lexer *curr)
{
	char	*filename;
	int		fd;

	if (!curr->next || curr->next->token != TOKEN_WORD)
	{
		print_syntax_error("newline", NULL);
		return (-1);
	}
	filename = curr->next->str;
	if (!filename)
	{
		ft_putendl_fd("minishell: error processing redirection", 2);
		return (-1);
	}
	fd = open_file(curr, filename);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(filename);
		return (-1);
	}
	return (fd);
}

int	setup_redirection_child(t_lexer *redirects)
{
	t_lexer	*curr;
	int		fd;

	if (!redirects || !redirects->next)
		return (1);
	curr = redirects;
	while (curr)
	{
		fd = get_redirection_fd(curr);
		if (fd == -1)
			return (-1);
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

void	error_cmd_not_found(const char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 21);
	exit(127);
}

void	exec_child_command(t_tools *tools, t_parser *curr)
{
	char	*path;
	char	**args;
	char	**envp;

	if (curr->redirects && setup_redirection_child
		(curr->redirects))
		exit(EXIT_FAILURE);
	if (curr->builtin)
		exit (curr->builtin(tools, curr, &tools->env));
	if (!curr->tokens || !curr->tokens->str)
		exit(0);
	path = get_command_path(curr->tokens->str, tools->env);
	if (!path)
		error_cmd_not_found(curr->tokens->str);
	args = build_args(curr->tokens);
	if (!args)
		(free(path), exit(EXIT_FAILURE));
	envp = copy_envp_to_execve(tools->env);
	execve(path, args, envp);
	perror("minishell");
	free(path);
	free_split(args);
	exit(EXIT_FAILURE);
}

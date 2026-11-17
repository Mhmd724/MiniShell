#include "../../minishell.h"

int	get_parser_nodes_nb(t_parser *parser)
{
	t_parser	*temp;
	int			len;

	temp = parser;
	len = 0;
	while (temp)
	{
		len++;
		temp = temp->next;
	}
	return (len);
}

int	handle_no_pipe(t_tools *tools, t_parser *parser, char **envp)
{
	pid_t	pid;
	int		exit_status;

	(void)envp;
	if (process_all_heredocs(tools))
		return (130);
	if (parser->builtin)
		return (execute_builtin_no_pipe(tools, parser));
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			return (1);
		}
		else if (pid == 0)
			child_process_no_pipe(tools, parser);
		else
		{
			reset_signals_ok();
			exit_status = wait_for_child(pid);
		}
	}
	return (exit_status);
}

void	free_pipes(int pipes_nb, int **pipes)
{
	int	i;

	i = 0;
	while (i < pipes_nb)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	handle_multiple_pipes(t_tools *tools, int pipes_nb)
{
	const int	nb_cmds = pipes_nb + 1;
	pid_t		*child_pids;
	int			**pipes;
	int			exit_status;

	if (process_all_heredocs(tools))
		return (130);
	if (setup_multiple_pipes(nb_cmds, &child_pids, &pipes))
		return (-1);
	if (fork_all_commands(tools, nb_cmds, child_pids, pipes))
		return (-1);
	if (nb_cmds > 1)
		close_pipes(pipes_nb, pipes);
	exit_status = wait_for_children(child_pids, nb_cmds);
	if (nb_cmds > 1)
		free_pipes(pipes_nb, pipes);
	free(child_pids);
	return (exit_status);
}

int	execute_commands(t_tools *tools, char **envp)
{
	t_parser	*parser;
	int			pipe_nb;
	int			exit_status;

	parser = tools->parser;
	pipe_nb = get_parser_nodes_nb(tools->parser);
	if (!pipe_nb)
		return (0);
	pipe_nb -= 1;
	set_execution_signals();
	if (!pipe_nb)
		exit_status = handle_no_pipe(tools, parser, envp);
	else
		exit_status = handle_multiple_pipes(tools, pipe_nb);
	set_signals();
	return (exit_status);
}

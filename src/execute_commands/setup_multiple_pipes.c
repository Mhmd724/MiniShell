#include "../../minishell.h"

void	close_parent_pipe_ends(int i, int nb_pipes, int **pipes)
{
	if (i > 0)
		close(pipes[i - 1][0]);
	if (i < nb_pipes)
		close(pipes[i][1]);
}

int	fork_all_commands(t_tools *tools, int nb_cmds,
	pid_t *child_pids, int **pipes)
{
	t_parser	*temp;
	int			i;

	temp = tools->parser;
	i = 0;
	while (temp)
	{
		child_pids[i] = fork();
		if (child_pids[i] == -1)
			return (-1);
		if (child_pids[i] == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			setup_child_pipes(i, nb_cmds - 1, pipes);
			exec_child_command(tools, temp);
		}
		else if (nb_cmds > 1)
			close_parent_pipe_ends(i, nb_cmds - 1, pipes);
		i++;
		temp = temp->next;
	}
	return (0);
}

int	**allocate_pipes(int nb_pipes)
{
	int	**pipes;
	int	i;

	pipes = (int **)malloc(sizeof(int *) * nb_pipes);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < nb_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) < 0)
		{
			perror("minishell: pipe");
			while (i-- > 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

int	setup_multiple_pipes(int nb_cmds, pid_t **child_pids, int ***pipes)
{
	int	nb_pipes;

	nb_pipes = nb_cmds - 1;
	*child_pids = (pid_t *)malloc(sizeof(pid_t) * nb_cmds);
	if (!child_pids)
		return (-1);
	if (nb_pipes <= 0)
		pipes = NULL;
	else
	{
		*pipes = allocate_pipes(nb_pipes);
		if (!*pipes)
			return (free(child_pids), -1);
	}
	return (0);
}

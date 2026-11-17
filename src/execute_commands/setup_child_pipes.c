#include "../../minishell.h"

void	close_pipes(int nb_pipes, int **pipes)
{
	int	i;

	i = 0;
	while (i < nb_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	setup_child_pipes(int i, int nb_pipes, int **pipes)
{
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
	if (i < nb_pipes)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
	close_pipes(nb_pipes, pipes);
}

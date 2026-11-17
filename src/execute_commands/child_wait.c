#include "../../minishell.h"

int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	wait_for_children(pid_t *child_pids, int nb_cmds)
{
	int		i;
	int		status;
	int		exit_status;
	pid_t	pid;

	i = 0;
	reset_signals_ok();
	while (i < nb_cmds)
	{
		pid = waitpid(child_pids[i], &status, 0);
		if (pid == child_pids[nb_cmds - 1])
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (exit_status);
}

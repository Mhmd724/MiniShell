#include "../../minishell.h"

/*
    Ctrl + C	SIGINT	Interrupt (kills or stops the current foreground process)
    Ctrl + \	SIGQUIT	Quit (kills the process and may dump core)
*/

void	handle_heredoc_signal(int sig)
{
	g_signum = sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	handle_execution_signals(int sig)
{
	g_signum = sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
}

void	handle_ctrl_c(int sig)
{
	g_signum = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

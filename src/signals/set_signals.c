#include "../../minishell.h"

void	set_signals(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	set_execution_signals(void)
{
	signal(SIGINT, handle_execution_signals);
	signal(SIGQUIT, handle_execution_signals);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	g_signum = 0;
}

void	reset_signals_ok(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	check_signal_if_recieved(int *status)
{
	if (g_signum == SIGINT)
	{
		*status = 130;
		g_signum = 0;
	}
	else if (g_signum == SIGQUIT)
	{
		*status = 131;
		g_signum = 0;
	}
}

#include "../../minishell.h"

void	set_init(int ac, char *av[], int *status, t_env **env)
{
	(void)av;
	if (ac > 1)
	{
		ft_putendl_fd("minishell: Too many arguments", STDERR_FILENO);
		exit(1);
	}
	*status = 0;
	*env = NULL;
	g_signum = 0;
	set_signals();
}

void	initialize_environment(t_env **env)
{
	char	cwd[1024];

	if (getenv("PWD"))
		return ;
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(env, "PWD", cwd, 0);
	else
		perror("minishell: getcwd");
	set_env_var(env, "OLDPWD", NULL, 1);
	set_env_var(env, "SHLVL", "1", 0);
}

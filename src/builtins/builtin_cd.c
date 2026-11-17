#include "../../minishell.h"

int	cd_home(t_env *env)
{
	char	*home;
	char	*pwd;

	pwd = get_env(env, "PWD");
	home = get_env(env, "HOME");
	if (!home)
	{
		write(2, "cd: HOME not set\n", 17);
		return (1);
	}
	if (chdir(home))
	{
		perror("cd");
		free(home);
		free(pwd);
		return (1);
	}
	set_env_var(&env, "OLDPWD", pwd, 0);
	set_env_var(&env, "PWD", home, 0);
	free(home);
	free(pwd);
	return (0);
}

int	cd_oldpwd(t_env *env)
{
	char	*pwd;
	char	*oldpwd;

	pwd = get_env(env, "PWD");
	oldpwd = get_env(env, "OLDPWD");
	if (!oldpwd)
	{
		write(2, "cd: OLDPWD not set\n", 19);
		return (1);
	}
	if (chdir(oldpwd))
	{
		perror("cd");
		free(pwd);
		free(oldpwd);
		return (1);
	}
	set_env_var(&env, "OLDPWD", pwd, 0);
	set_env_var(&env, "PWD", oldpwd, 0);
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	free(pwd);
	free(oldpwd);
	return (0);
}

int	cd_path(char *path, t_env *env)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = get_env(env, "PWD");
	if (chdir(path))
	{
		perror("cd");
		env->two_point += 1;
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		env->two_point += 1;
		perror("cd: error getting current directory");
		free(old_pwd);
		return (1);
	}
	env->two_point = 0;
	set_env_var(&env, "OLDPWD", old_pwd, 0);
	set_env_var(&env, "PWD", new_pwd, 0);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	process_cd_args(t_cmd_ctx *args, t_env *env)
{
	if (!args[0].str)
		return (cd_home(env));
	if (args[1].str)
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	if (!ft_strcmp(args[0].str, "~"))
		return (cd_home(env));
	if (!ft_strcmp(args[0].str, "-"))
		return (cd_oldpwd(env));
	return (cd_path(args[0].str, env));
}

int	builtin_cd(t_tools *tools, t_parser *parser, t_env **env)
{
	t_cmd_ctx	*cmd;
	int			ret;

	(void)tools;
	cmd = get_cmd(parser->tokens->next);
	if (!cmd)
		return (cd_home(*env));
	ret = process_cd_args(cmd, *env);
	free_args(cmd);
	return (ret);
}

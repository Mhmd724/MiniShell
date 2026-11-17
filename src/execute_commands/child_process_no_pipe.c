#include "../../minishell.h"

int	get_env_len(t_env *env)
{
	t_env	*tmp;
	int		len;

	if (!env)
		return (0);
	tmp = env;
	len = 0;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

char	**copy_envp_to_execve(t_env *env)
{
	char	**res;
	int		len;
	int		i;

	len = get_env_len(env);
	res = (char **)malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (env)
	{
		res[i] = ft_strjoin(ft_strdup(env->key), "=");
		res[i] = ft_strjoin(res[i], env->value);
		env = env->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}

void	execute_external_command(t_tools *tools, t_parser *parser)
{
	char	*path;
	char	**args;
	char	**local_envp;

	path = get_command_path(parser->tokens->str, tools->env);
	if (!path)
	{
		print_cmd_error(parser->tokens->str, NULL, "command not found");
		exit(127);
	}
	args = build_args(parser->tokens);
	if (!args)
	{
		free(path);
		exit(EXIT_FAILURE);
	}
	local_envp = copy_envp_to_execve(tools->env);
	execve(path, args, local_envp);
	perror("minishell");
	free(path);
	free_split(args);
	free_split(local_envp);
	exit(EXIT_FAILURE);
}

/* Validates tokens: if the first token is missing or empty,
   it advances until a TOKEN_WORD is found. */
void	validate_and_update_tokens(t_parser *parser)
{
	t_lexer	*tmp;

	if (!parser->tokens || !parser->tokens->str)
	{
		tmp = parser->tokens;
		while (tmp && tmp->token != TOKEN_WORD)
			tmp = tmp->next;
		if (!tmp || !tmp->str)
			exit(0);
		parser->tokens = tmp;
	}
}

void	child_process_no_pipe(t_tools *tools, t_parser *parser)
{
	signal(SIGQUIT, SIG_DFL);
	if (parser->redirects && setup_redirections_for_builtin
		(tools, parser->redirects))
		exit(EXIT_FAILURE);
	validate_and_update_tokens(parser);
	execute_external_command(tools, parser);
}

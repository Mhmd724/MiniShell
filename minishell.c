#include "minishell.h"

int	g_signum;
int	g_line_nb;

void	initialize_shell(int ac, char **av, char **envp, t_tools *tools)
{
	art();
	set_init(ac, av, &tools->last_exit_status, &tools->env);
	copy_envp(&tools->env, envp);
	initialize_environment(&tools->env);
}

static void	cleanup_shell(t_tools *tools)
{
	free_env(&tools->env);
	rl_clear_history();
}

static int	prepare_and_execute(t_tools *tools, char *str, char **envp)
{
	(void)envp;
	tools->lexer = NULL;
	tools->parser = NULL;
	parse_string(&tools->lexer, str);
	init_parser_nodes(&tools->parser, tools->lexer, str);
	if (!check_initial_errors(str, tools))
	{
		free(str);
		free_lexer(&tools->lexer);
		return (0);
	}
	free(str);
	free_lexer(&tools->lexer);
	update_parser_nodes(tools);
	init_redirections(tools->parser);
	init_heredoc_delimiters(tools->parser);
	set_builtins(tools->parser);
	return (1);
}

static void	process_input(t_tools *tools, char **envp)
{
	char	*input;
	char	*cmd;

	input = get_input(tools);
	if (!input)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		cleanup_shell(tools);
		exit(tools->last_exit_status);
	}
	check_signal_if_recieved(&tools->last_exit_status);
	if (!ft_isspacee(input))
		add_history(input);
	cmd = ft_strdup(input);
	free(input);
	if (prepare_and_execute(tools, cmd, envp))
		tools->last_exit_status = execute_commands(tools, envp);
	free_parser_list(&tools->parser);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_tools	tools;

	g_line_nb = 1;
	initialize_shell(argc, argv, envp, &tools);
	while (1)
	{
		process_input(&tools, envp);
		g_line_nb++;
	}
	cleanup_shell(&tools);
	return (0);
}

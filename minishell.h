#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>
# include "libft/inc/libft.h"
# include "libft/inc/ft_printf.h"

struct		s_parser;
struct		s_env;
struct		s_tools;
typedef int	(*t_builtin_func)(struct s_tools *, struct s_parser *,	\
				struct s_env **);

typedef enum e_tokens
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_SEMICOLON,
	TOKEN_EOF
}	t_tokens;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTES,
	DOUBLE_QUOTES,
	QUOTE_ERROR
}	t_quote_type;

typedef struct s_lexer
{
	t_tokens		token;
	t_quote_type	quote_type;
	char			*str;
	int				i;
	int				space;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_parser
{
	t_lexer			*tokens;
	t_builtin_func	builtin;
	t_lexer			*redirects;
	char			**hd_delimiters;
	struct s_parser	*next;
	struct s_parser	*prev;
}	t_parser;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				hidden;
	int				assigned;
	struct s_env	*next;
	int				two_point;
}	t_env;

typedef struct s_tools
{
	int			last_exit_status;
	t_lexer		*lexer;
	t_parser	*parser;
	t_env		*env;
}	t_tools;

typedef struct s_assign
{
	t_lexer			*lexer;
	t_lexer			*start_node;
	t_lexer			*lexer_head;
	unsigned int	nb_nodes;
}	t_assign;

typedef struct s_conv
{
	char	*result;
	int		i;
	int		j;
}	t_conv;

typedef struct s_heredoc_ctx
{
	int		stdin_copy;
	int		tmp_fd;
	char	*delimiter;
	char	template[32];
	int		expanded_variable;
}	t_heredoc_ctx;

typedef struct s_exp_ctx
{
	char		*expanded;
	const char	*line;
	size_t		i;
	size_t		j;
	t_env		*env;
}	t_exp_ctx;

typedef struct s_cmd_ctx
{
	char	*str;
	int		non_quoted;
	int		single_quotes;
}	t_cmd_ctx;

extern int	g_signum;
extern int	g_line_nb;

/*  art.c   */
void			art(void);
/*  Initialization  */
/*  init_shell.c  */
void			set_init(int ac, char *av[], int *status, t_env **env);
void			initialize_environment(t_env **env);
/*  env.c   */
void			copy_envp(t_env **env, char **envp);
void			set_env_var(t_env **env, char *key, char *value, int hidden);
t_env			*get_env_var(t_env *env, char *key);
void			update_shlvl(t_env **env, int hidden);
void			free_env(t_env **env);
/*  add_env.c   */
t_env			*create_env_node(char *key, char *value, int hidden);
void			env_add_back(t_env **env, char *key, char *value, int hidden);
/*  signals */
/*  set_signals.c   */
void			set_signals(void);
void			set_execution_signals(void);
void			reset_signals(void);
void			check_signal_if_recieved(int *status);
void			reset_signals_ok(void);
/*  handle_signals  */
void			handle_ctrl_c(int sig);
void			handle_heredoc_signal(int sig);
void			handle_execution_signals(int sig);
/*  utils   */
/*  utils1.c    */
char			*get_input(t_tools *tools);
int				ft_isspacee(char *str);
void			free_split(char **split);
/*  get_pid.c   */
pid_t			get_pid(void);
void			fill_ptr_substr(char **ptr, char *buffer, pid_t *pid);
/*  tokenization    */
/*  handle_char.c   */
void			handle_pipe(t_lexer **lexer, int *i);
void			handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i);
void			handle_out_or_append(t_lexer **lexer, const char *str, int *i);
void			handle_quotes(t_lexer **lexer, const char *str, int *i);
char			*remove_escaped_quotes(char *str);
/*  lexer_add_create.c  */
t_lexer			*create_new_token_node(t_tokens token,
					t_quote_type quotes, const char *str, int space);
void			add_token_node_back(t_lexer **lexer, t_lexer *new_node);
void			add_index_token(t_lexer **lexer);
void			free_lexer(t_lexer **lexer);
void			free_lexer_node(t_lexer **lexer, t_lexer *to_del);
/*  lexer_utils.c   */
void			add_token_increment_i(t_lexer **lexer, const char *str,
					t_tokens token);
int				special_char(char c);
int				get_real_string_length(char *str);
int				get_quote_length(const char *str, int i, char quote);
t_quote_type	get_quote_type(char quote);
/*  parser_string.c */
void			parse_string(t_lexer **lexer, const char *str);
void			process_string(t_lexer **lexer, const char *str, int *i);
void			handle_special_char(t_lexer **lexer, const char *str, int *i);
void			handle_normal_string(t_lexer **lexer, const char *str, int *i);
void			skip_spaces(const char *str, int *i);
/*  parser  */
/*  init_parser.c   */
void			init_parser_nodes(t_parser **parser, t_lexer *lexer,
					const char *str);
void			assign_tokens_to_parser(t_parser **parser, t_lexer *lexer,
					unsigned int tokens_nb);
/*  cat_heredoc_word.c  */
void			cat_heredoc_word(t_lexer **lexer, t_lexer *current_node);
void			merge_nodes(t_lexer **lexer, t_lexer *current);
/*  parser_utils.c  */
void			free_split(char **split);
void			concat_tokens(t_lexer *current, t_lexer *to_del);
int				count_pipes(t_lexer *lexer, unsigned int *tokens_nb);
int				is_a_special_case(char *str, int *i);
/*  parser_add_create.c   */
t_parser		*create_new_parser_node(void);
void			add_parser_node_back(t_parser **parser);
void			free_parser_list(t_parser **parser);
/*  sublist_lexer.c */
t_lexer			*ft_sublist(t_lexer *lexer, t_lexer *start_node,
					unsigned int nb_nodes);
void			fill_sublist(t_lexer **lexer, t_lexer **tmp,
					unsigned int nb_nodes);
/*  process_dollar_len.c    */
void			process_dollar_len(t_tools *tools, t_lexer *current,
					int *i, int *len);
void			handle_special_case_len(t_lexer *current, int *i, int *len);
void			handle_question_mark_len(t_tools *tools, int *i, int *len);
/*  process_dollar_str.c    */
void			process_dollar(t_tools *tools, t_lexer *current, t_conv *conv);
void			handle_dollar_sign_str(t_tools *tools, t_lexer *current,
					t_conv *conv);
void			handle_empty_sub(t_lexer *current, t_conv *conv,
					char **sub, int size);
void			handle_special_case_str(t_lexer *current, t_conv *conv,
					int size);
void			handle_question_mark_str(t_tools *tools, int size,
					t_conv *conv);
/*  updating_parser.c */
void			update_parser_nodes(t_tools *tools);
void			update_parser_words(t_tools *tools, t_parser **parser);
void			cat_heredoc_words(t_parser *parser);
void			cat_words(t_lexer *lexer);
/*     update_quoted_words.c  */
char			*convert_string(t_tools *tools, t_lexer *current);
int				get_new_string_length(t_tools *tools, t_lexer *current);
void			update_double_quotes(t_tools *tools, t_lexer *current);
void			process_char(t_tools *tools, t_lexer *current, t_conv *conv);
void			process_escape_in_double_quotes(t_lexer *current, t_conv *conv);
/*  update_unquoted_words.c */
void			update_no_quotes(t_tools *tools, t_lexer *head_lexer,
					t_lexer *current);
void			inject_sublexer(t_lexer **lexer, t_lexer *sublexer,
					t_lexer *to_del);
void			handle_multiple_strings(t_lexer **sublexer, char **split,
					t_lexer *to_del, int count);
void			handle_one_string(t_lexer *to_del, char *str, char **split);
void			handle_empty_split(t_lexer **lexer, t_lexer *to_del,
					char *str, char **split);
/*  init_redirections.c */
void			init_redirections(t_parser *parser);
void			create_redirects_list(t_lexer **redirects, t_lexer **lexer);
void			add_to_redirects(t_lexer **redirects,
					t_lexer *redirection, t_lexer *redirect_to);
/*  init_hd_delimiters.c    */
void			init_heredoc_delimiters(t_parser *parser);
//  static void    set_hd_delimiters_in_node(t_parser *parser);
//  static void    fill_hd_array(t_parser *parser);
/*  set_builtins.c  */
void			set_builtins(t_parser *parser);
void			set_one_builtin(t_parser *parser);
/*  checking_errors */
/*  and_error.c */
int				is_only_ands(char *str);
/*  check_errors.c   */
int				check_initial_errors(char *prompt, t_tools *tools);
//  static int	check_redirection_error(t_tools *tools)
//  static int	check_and_error(char *prompt, t_tools *tools)
//  static int	check_directory_error(char *prompt, t_tools *tools)
//  static int	check_quote_error(char *prompt, t_tools *tools)
/*  directory_error.c   */
int				is_directory_path(char *str);
/*  pipe_error.c    */
int				check_pipe_error(t_tools *tools);
int				validate_pipes_and_words(t_parser *head);
/*  print_the_error.c   */
void			print_syntax_error(char *msg, int *status);
void			print_cmd_error(char *cmd, char *arg, char *msg);
void			free_tools(t_tools *tools);
/*  quote_error.c   */
int				check_unclosed_quotes(char *str);
/*  redirection_error.c */
int				is_redirection_token(t_tokens token);
//  static int	handle_redirect_in(t_lexer *current)
//  static int	check_invalid_token(t_lexer *current)
//  static int	check_redirections(t_lexer *lexer)
int				check_parser_redirections(t_parser *parser);
/*  skipping_errors.c    */
int				check_hash(char *prompt, t_tools *tools);
int				check_colon(char *prompt, t_tools *tools);
int				check_exclamation(char *prompt, t_tools *tools);
/*  execute_commands    */
/*  execute_1   */
int				execute_commands(t_tools *tools, char **envp);
int				handle_no_pipe(t_tools *tools, t_parser *parser, char **envp);
int				handle_multiple_pipes(t_tools *tools, int pipes_nb);
void			free_pipes(int pipes_nb, int **pipes);
int				get_parser_nodes_nb(t_parser *parser);
/*  get_line_dollar_len */
int				get_line_length(t_tools *tools, char *line);
void			get_line_dollar_len(t_tools *tools, char *line,
					int *i, size_t *len);
void			line_special_case_len(char *line, int *i, size_t *len);
void			line_question_mark_len(t_tools *tools, int *i, size_t *len);
/*  handle_heredoc_filename */
char			*handle_heredoc_filename(t_tools *tools, char **delimiter,
					t_quote_type quote_type);
// static char *heredoc_cleanup(t_heredoc_ctx *ctx);
// static int  heredoc_setup(t_heredoc_ctx *ctx, char **delimiter,
//     t_quote_type quote_type);
/*  heredoc_loop    */
int				heredoc_loop(t_tools *tools, t_heredoc_ctx *ctx);
int				process_heredoc_line(t_tools *tools, t_heredoc_ctx *ctx,
					char *line);
char			*expand_heredoc_line(t_tools *tools, char *line);
void			print_heredoc_error(char *delimiter);
int				handle_sigint(t_heredoc_ctx *ctx, char *line);
/*  line_dollar_str */
void			process_expansion(t_exp_ctx *exp_ctx, t_tools *tools);
void			expand_env_variable(t_exp_ctx *ctx);
void			expand_dollar_nb(t_exp_ctx *ctx);
void			expand_exit_status(t_tools *tools, t_exp_ctx *ctx);
/*  process_heredoc */
int				process_all_heredocs(t_tools *tools);
int				prepare_redirects_heredoc(t_tools *tools,
					t_lexer *parse_redirects);
int				process_redirects_heredoc(t_lexer *curr, t_tools *tools);
/*  child_process_no_pipe   */
void			child_process_no_pipe(t_tools *tools, t_parser *parser);
void			validate_and_update_tokens(t_parser *parser);
void			execute_external_command(t_tools *tools, t_parser *parser);
char			**copy_envp_to_execve(t_env *env);
int				get_env_len(t_env *env);
/*  execute_builtin_no_pipe.c   */
int				execute_builtin_no_pipe(t_tools *tools, t_parser *parser);
int				setup_redirections_for_builtin(t_tools *tools,
					t_lexer *redirects);
int				open_fd_for_redirections(t_tools *tools, t_lexer *current);
int				redirect_fd_for_token(int fd, t_tokens token);
/*  child_wait.c    */
int				wait_for_child(pid_t pid);
int				wait_for_children(pid_t *child_pids, int nb_cmds);
/*  setup_multiple_pipes.c  */
int				setup_multiple_pipes(int nb_cmds, pid_t **child_pids,
					int ***pipes);
int				**allocate_pipes(int nb_pipes);
int				fork_all_commands(t_tools *tools, int nb_cmds,
					pid_t *child_pids, int **pipes);
void			close_parent_pipe_ends(int i, int nb_pipes, int **pipes);
/*  setup_child_pipes.c */
void			setup_child_pipes(int i, int nb_pipes, int **pipes);
void			close_pipes(int nb_pipes, int **pipes);
/*  exec_child_command.c    */
void			exec_child_command(t_tools *tools, t_parser *curr);
void			error_cmd_not_found(const char *cmd);
int				setup_redirection_child(t_lexer *redirects);
int				get_redirection_fd(t_lexer *curr);
int				open_file(t_lexer *curr, const char *file);
/*  get_command_path.c  */
char			*get_command_path(char *cmd, t_env *env);
char			*ft_strtok(char *str, const char *delim);
char			*get_env_path(t_env *env);
// static char	*search_cmd_path(char *path_env, char *cmd);
/*  build_args.c    */
char			**build_args(t_lexer *tokens);
int				fill_args(char **args, t_lexer *tokens, int count);
int				count_arguments(t_lexer *tokens);
/*  builtins    */
/*  get_cmd */
void			free_args(t_cmd_ctx *args);
t_cmd_ctx		*get_cmd(t_lexer *lexer);
int				get_cmd_len(t_lexer *lexer);
/*  builtin_echo    */
int				builtin_echo(t_tools *tools, t_parser *parser, t_env **env);
int				builtin_env(t_tools *tools, t_parser *parser, t_env **env);
int				builtin_cd(t_tools *tools, t_parser *parser, t_env **env);
char			*get_env(t_env *env, char *key);
int				builtin_pwd(t_tools *tools, t_parser *parser, t_env **env);
int				builtin_exit(t_tools *tools, t_parser *parser, t_env **env);
int				builtin_unset(t_tools *tools, t_parser *parser, t_env **env);
int				builtin_export(t_tools *tools, t_parser *parser, t_env **env);
/*  builtin_echo2.c */
void			print_with_escape(char *str, int unquoted);
void			handle_esc_quoted(char *str);
int				print_char(char c);
/*  builtin_export2.c   */
int				add_to_env_list(t_env **env, char *key, char *value);
int				add_non_equal_var(t_env **env, char *key, char *value);

#endif
NAME = minishell

LIBFT = libft

LIBFT_LIB = libft/libft.a

SRCS =  minishell.c								\
		art.c									\
		src/initialization/init_shell.c			\
		src/initialization/add_env.c			\
		src/initialization/env.c				\
		src/signals/set_signals.c				\
		src/signals/handle_signals.c			\
		src/utils/utils1.c						\
		src/utils/get_pid.c						\
		src/tokenization/lexer_add_create.c		\
		src/tokenization/handle_char.c			\
		src/tokenization/lexer_utils.c			\
		src/tokenization/parse_string.c			\
		src/parser/init_parser.c				\
		src/parser/parser_add_create.c			\
		src/parser/parser_utils.c				\
		src/parser/sublist_lexer.c				\
		src/parser/cat_heredoc_words.c			\
		src/parser/updating_parser.c			\
		src/parser/process_dollar_len.c			\
		src/parser/process_dollar_str.c			\
		src/parser/update_quoted_words.c		\
		src/parser/update_unquoted_words.c		\
		src/parser/init_redirections.c			\
		src/parser/init_hd_delimiters.c			\
		src/parser/set_builtins.c				\
		src/check_errors/check_errors.c			\
		src/check_errors/and_error.c			\
		src/check_errors/directory_error.c		\
		src/check_errors/pipe_error.c			\
		src/check_errors/print_the_error.c		\
		src/check_errors/quote_error.c			\
		src/check_errors/redirection_error.c	\
		src/check_errors/skipping_errors.c		\
		src/execute_commands/execute_1.c		\
		src/execute_commands/get_line_dollar_len.c	\
		src/execute_commands/handle_heredoc_filename.c	\
		src/execute_commands/heredoc_loop.c		\
		src/execute_commands/line_dollar_str.c	\
		src/execute_commands/process_heredoc.c	\
		src/execute_commands/child_process_no_pipe.c	\
		src/execute_commands/child_wait.c		\
		src/execute_commands/execute_builtin_no_pipe.c	\
		src/execute_commands/setup_multiple_pipes.c		\
		src/execute_commands/setup_child_pipes.c		\
		src/execute_commands/exec_child_command.c		\
		src/execute_commands/get_command_path.c	\
		src/execute_commands/build_args.c		\
		src/builtins/get_cmd.c					\
		src/builtins/builtin_echo.c				\
		src/builtins/builtin_echo2.c			\
		src/builtins/builtin_env.c				\
		src/builtins/builtin_cd.c				\
		src/builtins/builtin_pwd.c				\
		src/builtins/builtin_unset.c			\
		src/builtins/builtin_export.c			\
		src/builtins/builtin_export2.c			\
		src/builtins/builtin_exit.c				\
		
OBJDIR = objs
OBJCS = $(SRCS:%.c=$(OBJDIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAG = -lreadline

all: $(NAME)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJCS)
	make -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJCS) $(READLINE_FLAG) $(LIBFT_LIB)
	@printf "\n\033[35m"
	@printf " __  __ _       _     _          _ _ _ _ _ _ _ _ \n"
	@printf "|  \\/  (_)     (_)   | |        | | | | | | | | |\n"
	@printf "| \\  / |_ _ __  _ ___| |__   ___| | | | | | | | |\n"
	@printf "| |\\/| | | '_ \\| / __| '_ \\ / _ \\ | | | | | | | |\n"
	@printf "| |  | | | | | | \\__ \\ | | |  __/ | | | | | | | |\n"
	@printf "|_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|_|_|_|_|_|_|\n"
	@printf "\033[0m\n"
	@printf "\033[1;36m"
	@printf "╔════════════════════════════════╗\n"
	@printf "║  🚀 Minishell Compiled!        ║\n"
	@printf "║  🖥 Type './minishell' to run   ║\n"
	@printf "╚════════════════════════════════╝\n"
	@printf "\033[0m\n"

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT) clean
	rm -f $(OBJCS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(LIBFT_LIB)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
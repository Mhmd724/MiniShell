# Minishell

A bash-like Unix shell written in C, built as part of the **42 Beirut** curriculum.

It reads commands from the user, splits them into tokens, parses them into commands, and executes them, with support for pipes, redirections, heredocs, environment variables, quoting rules and signals, all following bash's behavior.

```
minishell: ~/projects$ echo "Hello $USER" | tr a-z A-Z > out.txt
minishell: ~/projects$ cat out.txt
HELLO MOHARAJLI
```

## Features

- **Interactive prompt** with command history (GNU Readline)
- **Executing programs** by absolute/relative path or by searching `PATH`
- **Pipes** `|`, chaining any number of commands
- **Redirections**
  - `<` input from a file
  - `>` output to a file (truncate)
  - `>>` output to a file (append)
  - `<<` heredoc, with variable expansion (disabled when the delimiter is quoted)
- **Quotes**
  - `'single quotes'` prevent all interpretation
  - `"double quotes"` prevent interpretation except for `$`
- **Environment variables**: `$VAR` expansion and `$?` (exit status of the last command)
- **Signals**
  - `Ctrl-C` shows a new prompt on a new line
  - `Ctrl-D` exits the shell
  - `Ctrl-\` does nothing at the prompt
- **Syntax error detection** for unclosed quotes and misplaced pipes or redirections

### Builtins

| Command  | Description                              |
|----------|------------------------------------------|
| `echo`   | Print arguments (supports `-n`)          |
| `cd`     | Change directory (relative/absolute path, `~`, `-`) |
| `pwd`    | Print the current directory              |
| `export` | Set environment variables                |
| `unset`  | Remove environment variables             |
| `env`    | Print the environment                    |
| `exit`   | Exit the shell with an optional status   |

## Getting started

### Requirements

- Linux (or WSL)
- `cc` / `gcc` and `make`
- GNU Readline library

```bash
sudo apt install build-essential libreadline-dev
```

### Build and run

```bash
git clone https://github.com/Mhmd724/MiniShell.git
cd MiniShell
make
./minishell
```

Other Makefile rules: `make clean` (remove object files), `make fclean` (remove objects and binary), `make re` (rebuild).

## How it works

Each input line goes through four stages:

1. **Tokenization** (`src/tokenization/`): the line is split into tokens (words, pipes, redirection operators), tracking which parts are quoted.
2. **Parsing** (`src/parser/`): tokens are grouped into one command node per pipe segment. Variables are expanded, quotes are removed, and redirections, heredoc delimiters and builtins are attached to each command.
3. **Error checking** (`src/check_errors/`): syntax errors are detected before anything runs, with bash-style error messages.
4. **Execution** (`src/execute_commands/`): heredocs are collected first, then pipes are created and each command runs in a child process via `fork` + `execve`. A builtin with no pipe runs directly in the shell process, so `cd`, `export` and `exit` affect the shell itself.

```
.
├── minishell.c / minishell.h   # main loop and shared definitions
├── src/
│   ├── initialization/         # environment copy, SHLVL
│   ├── tokenization/           # lexer
│   ├── parser/                 # command nodes, expansion, quotes
│   ├── check_errors/           # syntax validation
│   ├── execute_commands/       # pipes, redirections, heredocs, execve
│   ├── builtins/               # echo, cd, pwd, export, unset, env, exit
│   ├── signals/                # Ctrl-C / Ctrl-\ handling
│   └── utils/
└── libft/                      # my own C standard library functions
```

## What I learned

- How a shell works under the hood: processes (`fork`, `execve`, `waitpid`), file descriptors (`pipe`, `dup2`) and signal handling
- Writing a lexer and parser by hand, and handling bash's quoting and expansion rules
- Managing memory carefully in a long-running C program (checked with Valgrind)

## Author

**Mohamad Harajli** ([@Mhmd724](https://github.com/Mhmd724)), 42 Beirut
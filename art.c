#include "minishell.h"

void	art(void)
{
	printf("\33[1;36m%s\033[0m\n", " __  __ _       _     _          _ _ ");
	printf("\33[1;36m%s\033[0m\n", "|  \\/  (_)_ __ (_)___| |__   ___| | |");
	printf("\33[1;36m%s\033[0m\n", "| |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |");
	printf("\33[1;36m%s\033[0m\n", "| |  | | | | | | \\__ \\ | | |  __/ | |");
	printf("\33[1;36m%s\033[0m\n", "|_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|");
	printf("\33[1;36m%s\033[0m\n", "                                      ");
	printf("\33[1;37m%s\033[0m\n", "           Made by Mohamad Harajly     ");
	printf("\33[1;32m%s", "\"No leaks allowed — ");
	printf("my shell is watertight!\"\033[0m\n\n");
}

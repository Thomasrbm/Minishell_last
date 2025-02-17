/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_readline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 09:16:01 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 11:18:55 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	check_redir_end(char *cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i])
		i++;
	while (i > 0 && (cmd[i - 1] == ' ' || cmd[i - 1] == '\t'))
		i--;
	if (i > 0 && (cmd[i - 1] == '<' || cmd[i - 1] == '>'))
	{
		write(2, "minishell:  error near unexpected token `newline'\n", 55);
		// write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		return (2);
	}
	return (0);
}


// Updated get_readline function in get_readline.c
// Updated get_readline() in get_readline.c

char	*get_readline(t_shell *shell)
{
	char	*input;
	char	*trim;
	int		ret;

	input = readline("\033[1;32mminishell > \033[0m");
	if (!input)
		return (NULL);
	trim = ft_strtrim(input, " \t\n");
	if (!trim)
		return (input);
	ret = check_redir_end(trim);
	if (ret == 2)
	{
		shell->exit_code = 2;
		add_history(input);
		free(trim);
		free(input);
		return (ft_strdup(""));
	}
	if (*trim)
		add_history(input);
	free(trim);
	return (input);
}


void	get_sigint(t_shell *shell)
{
	shell->exit_code = 130;
	g_signal = 0;
}
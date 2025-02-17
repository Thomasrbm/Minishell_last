/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 01:26:26 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 05:22:40 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_cmds_split(const char *str, char sep)
{
	int	word_count;

	word_count = 0;
	while (*str)
	{
		while (*str && *str == sep)
			str++;
		if (*str)
		{
			word_count++;
			while (*str && *str != sep)
			{
				if (*str == '\'' || *str == '"')
					str = skip_quote(str);
				else
					str++;
			}
		}
	}
	return (word_count);
}

static char	*ft_get_word(const char **str_ptr, char sep)
{
	const char	*current;
	const char	*word_start;
	int			word_length;
	char		*word;

	current = *str_ptr;
	word_start = current;
	while (*current && *current != sep)
	{
		if (*current == '\'' || *current == '"')
			current = skip_quote(current);
		else
			current++;
	}
	word_length = current - word_start;
	word = malloc(word_length + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, word_start, word_length + 1);
	*str_ptr = current;
	return (word);
}

/* split_loop:
   - Loops through words and extracts them.
   - Returns NULL if an error occurs, otherwise cmds_split.
*/
static char	**split_loop(const char **ptr, char sep, char **cmds_split,
		int word_count)
{
	int	i;

	i = 0;
	while (i < word_count)
	{
		while (**ptr && **ptr == sep)
			(*ptr)++;
		cmds_split[i] = ft_get_word(ptr, sep);
		if (!cmds_split[i])
		{
			free_tab(cmds_split);
			return (NULL);
		}
		i++;
	}
	cmds_split[i] = NULL;
	return (cmds_split);
}

/* split_cmd:
   - Splits the command string using 'sep'.
   - Calls split_loop to process words.
*/
static char **split_cmd(char *cmd, char sep)
{
    const char  *ptr;
    int         word_count;
    char        **cmds_split;

    ptr = cmd;
    word_count = count_cmds_split(ptr, sep);
    if (word_count < 0)
        return (NULL);
    cmds_split = ft_calloc(word_count + 1, sizeof(char *)); // +1 for NULL
    if (!cmds_split)
        return (NULL);
    return (split_loop(&ptr, sep, cmds_split, word_count));
}

// Ensure ft_split_with_quote initializes shell->cmd correctly
t_shell ft_split_with_quote(char *cmd, char sep, t_shell shell)
{
    char **splitted;
    int i;

    if (!cmd || *cmd == '\0') {
        shell.cmd = ft_calloc(1, sizeof(char *));
        return (shell);
    }

    splitted = split_cmd(cmd, sep);
    if (!splitted) {
        shell.cmd = ft_calloc(1, sizeof(char *));
        return (shell);
    }

    // Garantir la terminaison NULL
    i = 0;
    while (splitted[i])
        i++;
    splitted = realloc(splitted, sizeof(char *) * (i + 1));
    splitted[i] = NULL;

    shell.cmd = splitted;
    return (shell);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 01:19:57 by throbert          #+#    #+#             */
/*   Updated: 2025/02/14 13:03:27 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

const char	*skip_quote(const char *str)
{
	char	quote;

	if (*str != '\'' && *str != '"')
		return (str);
	quote = *str;
	str++;
	while (*str && *str != quote)
		str++;
	if (*str)
		str++;
	return (str);
}

int	is_quote_closed(const char *str)
{
	int	single;
	int	double_;
	int	i;

	single = 0;
	double_ = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !double_)
			single = !single;
		else if (str[i] == '"' && !single)
			double_ = !double_;
		i++;
	}
	return (!single && !double_);
}

char	*handle_dquote(char *cmd_quote)
{
	int		len;
	int		i;
	int		j;
	char	*cmd_unquote;

	len = ft_strlen(cmd_quote);
	if (cmd_quote[0] != '"' || cmd_quote[len - 1] != '"')
		return (ft_strdup(cmd_quote));
	cmd_unquote = malloc(len - 1);
	if (!cmd_unquote)
		return (NULL);
	i = 1;
	j = 0;
	while (i < len - 1)
	{
		cmd_unquote[j] = cmd_quote[i];
		i++;
		j++;
	}
	cmd_unquote[j] = '\0';
	return (cmd_unquote);
}

// cmd_unquote[0] = 1; // MARK 1  line 94
//
//
// Handle single quotes
// mark the begining of '  .... '  with invisible  1 before 1rst quote
// result = 1'...'
char	*handle_squote(char *cmd_quote)
{
	int		len;
	int		i;
	int		j;
	char	*cmd_unquote;

	len = ft_strlen(cmd_quote);
	if (cmd_quote[0] != '\'' || cmd_quote[len - 1] != '\'')
		return (ft_strdup(cmd_quote));
	cmd_unquote = malloc(len);
	if (!cmd_unquote)
		return (NULL);
	cmd_unquote[0] = 1;
	i = 1;
	j = 1;
	while (i < len - 1)
	{
		cmd_unquote[j] = cmd_quote[i];
		i++;
		j++;
	}
	cmd_unquote[j] = '\0';
	return (cmd_unquote);
}

char	**handle_quote(t_shell *shell)
{
	int		i;
	int		size;
	char	**new_cmd;

	if (!shell->cmd)
		return (NULL);
	size = tab_size(shell->cmd);
	new_cmd = malloc(sizeof(char *) * (size + 1));
	if (!new_cmd)
		return (NULL);
	i = 0;
	while (shell->cmd[i])
	{
		new_cmd[i] = process_quote(ft_strdup(shell->cmd[i]), shell);
		i++;
	}
	new_cmd[i] = NULL;
	return (new_cmd);
}

// si dernier char =  ou  > <

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:02:48 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 15:52:03 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_exit_status(t_shell shell)
{
	char	*exit_str;

	exit_str = ft_itoa(shell.exit_code);
	return (exit_str);
}

char	*handle_dollar(char *s, int *i, t_shell shell)
{
	char	*tmp;

	tmp = NULL;
	if (s[*i + 1] == '?')
	{
		tmp = handle_exit_status(shell);
		(*i) += 2;
	}
	else
	{
		(*i)++;
		if (!s[*i] || (!ft_isalnum(s[*i]) && s[*i] != '_'))
			tmp = ft_strdup("$");
		else
			tmp = process_dollar(s, i, shell);
	}
	return (tmp);
}

char	*handle_normal_char(char *s, int *i)
{
	char	*tmp;

	tmp = ft_substr(s, *i, 1);
	(*i)++;
	return (tmp);
}

char	*expand_dollar(char *s, t_shell shell)
{
	char	*res;
	char	*tmp;
	char	*val;
	int		i;
	int		start;

	res = ft_strdup("");
	tmp = NULL;
	val = NULL;
	i = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == '$')
		{
			i++;
			if (s[i] && s[i] == '?')
			{
				tmp = ft_itoa(shell.exit_code);
				val = ft_strjoin(res, tmp);
				free(res);
				free(tmp);
				res = val;
				i++;
			}
			else
			{
				start = i;
				while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
					i++;
				tmp = ft_substr(s, start, i - start);
				val = get_env_value(tmp, shell);
				free(tmp);
				tmp = ft_strjoin(res, val);
				free(res);
				free(val);
				res = tmp;
			}
		}
		else
		{
			tmp = ft_substr(s, i, 1);
			val = ft_strjoin(res, tmp);
			free(res);
			free(tmp);
			res = val;
			i++;
		}
	}
	return (res);
}

char	*process_dollar(char *s, int *i, t_shell shell)
{
	int		start;
	char	*tmp;
	char	*res;

	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	tmp = ft_substr(s, start, *i - start);
	res = get_env_value(tmp, shell);
	free(tmp);
	return (res);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 06:31:28 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 08:00:14 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	mal_len(char **tab, char delim)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	len = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			if (tab[i][j] == delim)
				len += 2;
			len++;
			j++;
		}
		i++;
	}
	len += 2;
	return (len);
}

static void	append_delim(char *res, int *r, char delim)
{
	if (*r > 0 && res[*r - 1] != ' ')
		res[(*r)++] = ' ';
	res[(*r)++] = delim;
	res[(*r)++] = ' ';
}

static void	fill_res(char *res, char **tab, char delim, int *r)
{
	int	i;
	int	j;

	i = 0;
	while (tab[i])
	{
		j = -1;
		while (tab[i][++j])
		{
			if (tab[i][j] == delim)
				append_delim(res, r, delim);
			else
				res[(*r)++] = tab[i][j];
		}
		if (tab[i + 1] && res[*r - 1] != ' ')
			res[(*r)++] = ' ';
		i++;
	}
}

// reparse the args to get space between separator and arg
// ex : cat -e<test -> cat -e < test
char	*parse_cmd(char **tab, char delim)
{
	int		r;
	char	*res;

	r = 0;
	res = malloc(sizeof(char) * mal_len(tab, delim));
	if (!res)
		return (NULL);
	fill_res(res, tab, delim, &r);
	res[r - (r > 0 && res[r - 1] == ' ')] = '\0';
	return (res);
}

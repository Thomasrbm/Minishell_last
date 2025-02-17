/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:03:17 by throbert          #+#    #+#             */
/*   Updated: 2025/02/15 21:08:35 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_remaining(char *pattern)
{
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

int	handle_char_match(char *patt, char *str, int *p, int *s)
{
	if (patt[*p] == str[*s])
	{
		(*p)++;
		(*s)++;
		return (1);
	}
	return (0);
}

int	handle_star(char *pattern, t_wildcard *wc)
{
	if (pattern[wc->p] == '*')
	{
		wc->star = wc->p;
		wc->match = wc->s;
		wc->p++;
		return (1);
	}
	return (0);
}

int	handle_backtrack(t_wildcard *wc)
{
	if (wc->star != -1)
	{
		wc->p = wc->star + 1;
		wc->match++;
		wc->s = wc->match;
		return (1);
	}
	return (0);
}

int	wildcard_match(char *pattern, char *str, t_wildcard *wc)
{
	wc->s = 0;
	wc->p = 0;
	wc->star = -1;
	wc->match = 0;
	while (str[wc->s])
	{
		if (handle_char_match(pattern, str, &wc->p, &wc->s))
			continue ;
		if (handle_star(pattern, wc))
			continue ;
		if (handle_backtrack(wc))
			continue ;
		return (0);
	}
	return (check_remaining(&pattern[wc->p]));
}

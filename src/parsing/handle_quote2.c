/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 08:10:36 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 16:29:29 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*join;
	size_t	l1;
	size_t	l2;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	join = malloc(l1 + l2 + 1);
	if (!join)
		return (NULL);
	ft_strlcpy(join, s1, l1 + 1);
	ft_strlcpy(join + l1, s2, l2 + 1);
	free(s1);
	return (join);
}

char	*process_end_quote(char *input, t_shell *shell, t_quote q)
{
	q.seg = ft_substr(input, q.start, q.i - q.start);
	if (q.quote == '\'')
		q.tmp = q.seg;
	else
		q.tmp = expand_dollar(q.seg, *shell);
	q.new_res = ft_strjoin(q.res, q.tmp);
	free(q.res);
	free(q.seg);
	if (q.tmp != q.seg)
		free(q.tmp);
	/* free(input) à enlever ici */
	return (q.new_res);
}

// 	if (q->tmp != q->seg) // Ne libère que si expansionvoid	process_quote_block(t_quote *q, char *input, t_shell *shell)
void	process_quote_block(t_quote *q, char *input, t_shell *shell)
{
	q->seg = ft_substr(input, q->start, q->i - q->start);
	if (q->quote == '"')
		q->tmp = expand_dollar(q->seg, *shell);
	else
		q->tmp = q->seg;
	q->new_res = ft_strjoin(q->res, q->tmp);
	free(q->res);
	if (q->tmp != q->seg)
		free(q->tmp);
	free(q->seg);
	q->res = q->new_res;
	if (q->quote == input[q->i])
		q->quote = 0;
	else
		q->quote = input[q->i];
	q->start = q->i + 1;
}

// char	*process_quote(char *input, t_shell *shell)
// {
// 	t_quote	q;

// 	ft_memset(&q, 0, sizeof(t_quote));
// 	q.res = ft_strdup("");
// 	q.start = 0;
// 	while (input[q.i])
// 	{
// 		if ((input[q.i] == '\'' || input[q.i] == '"') && (q.quote == 0
// 				|| input[q.i] == q.quote))
// 			process_quote_block(&q, input, shell);
// 		q.i++;
// 	}
// 	return (process_end_quote(input, shell, q));
// }

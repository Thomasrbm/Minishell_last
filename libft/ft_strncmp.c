/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:49:45 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 00:21:47 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strcmp(const char *s1, const char *s2)
{
    size_t a = 0;

    while (s1[a] && s2[a] && s1[a] == s2[a])
        a++;

    return ((unsigned char)s1[a] - (unsigned char)s2[a]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	a;

	a = 0;
	if (n == 0)
		return (0);
	while (s1[a] && s2[a] && a < n - 1 && s1[a] == s2[a])
		a++;
	return ((unsigned char )s1[a] - (unsigned char )s2[a]);
}
// int main(void)
// {
// 	char str[] = "test";
// 	char str2[] = "";
//	printf("%d\n", ft_strncmp(str, str2,06));
//	printf("%d\n", strncmp(str, str2, 0));
//  return 0;
// }

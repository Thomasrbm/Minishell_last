/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 19:30:15 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 09:45:56 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// edit the value that we want
// value is the value that want to change
// new_one is the value that will be the new one.
char	**edit_env_value(t_shell *shell, char *value, char *new_one)
{
	char	*tmp;
	char	*new_value;
	int		i;

	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], value, ft_strlen(value)))
		{
			tmp = ft_strjoin(value, "=");
			new_value = ft_strjoin(tmp, new_one);
			free(shell->env[i]);
			shell->env[i] = new_value;
			free(tmp);
			break ;
		}
		i++;
	}
	if (shell->env[i] == NULL)
		return (NULL);
	return (shell->env);
}

// get the value that we want in the env of shell->env
char	*get_env(t_shell *shell, char *value)
{
	char	**tmp;
	char	*result;
	int		i;

	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], value, ft_strlen(value)))
		{
			tmp = ft_split(shell->env[i], '=');
			tmp[2] = NULL;
			result = ft_strdup(tmp[1]);
			break ;
		}
		i++;
	}
	if (shell->env[i] == NULL)
	{
		free(tmp);
		return (NULL);
	}
	free_tab(tmp);
	return (result);
}

// reproduce env
int	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (!shell->env)
	{
		ft_putstr_fd("No env available.\n", 2);
		shell->exit_code = 1;
		return (shell->exit_code);
	}
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	shell->exit_code = 0;
	return (shell->exit_code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 19:08:10 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 10:30:56 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get all the args after unset, they are the values to be deleted
// from the env.
char	**get_unset_value(t_shell *shell)
{
	char	**get_args;
	int		i;
	int		j;

	i = 1;
	j = 0;
	get_args = calloc(tab_size(shell->cmd), sizeof(char *));
	while (shell->cmd[i])
	{
		get_args[j] = ft_strdup(shell->cmd[i]);
		i++;
		j++;
	}
	get_args[j] = NULL;
	return (get_args);
}

// Update env by removing the value at the specified position
static char	**update_env(t_shell *shell, int pos)
{
	char	**new_env;
	int		i;
	int		j;
	int		size;

	size = tab_size(shell->env);
	new_env = calloc(size, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (shell->env[j])
	{
		if (j != pos)
		{
			new_env[i] = shell->env[j];
			i++;
		}
		else
			free(shell->env[j]);
		j++;
	}
	new_env[i] = NULL;
	free(shell->env);
	return (new_env);
}

// Unset all the values after "unset"
t_shell	*ft_unset(t_shell *shell)
{
	char	**get_args;
	int		i;
	int		j;

	i = 0;
	shell->exit_code = 0;
	if (!shell->cmd[1])
		return (shell);
	get_args = get_unset_value(shell);
	while (get_args[i])
	{
		j = 0;
		while (shell->env[j])
		{
			if (!ft_strncmp(get_args[i], shell->env[j], ft_strlen(get_env(shell, get_args[i]))))
			{
				shell->env = update_env(shell, j);
				break ;
			}
			j++;
		}
		i++;
	}
	free_tab(get_args);
	return (shell);
}


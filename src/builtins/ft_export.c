/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 20:25:48 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 10:32:45 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// if there is no arg with export , print the env with declare -x
// followed by value.
static void	no_arg_export(t_shell *shell)
{
	int		i;
	char	*tmp;

	i = 0;
	while (shell->env[i] != NULL)
	{
		tmp = ft_strjoin("declare -x ", shell->env[i]);
		ft_printf("%s\n", tmp);
		free(tmp);
		i++;
	}
	return ;
}

// change the value of the arg to the new one or create if it dont exist.
static char	*new_value(t_shell *shell, char **get_args, int i, int j)
{
	char	**get_value;

	get_value = ft_split(get_args[i], '=');
	if (!get_value || !get_value[1])
	{
		free_tab(get_value);
		return (shell->env[j]);
	}
	free_tab(get_value);
	free(shell->env[j]);
	return (get_args[i]);
}

// if the value does not exist , we create it in a new env and return it.
static char	**add_value(t_shell *shell, char **get_args, int i)
{
	char	**new_env;
	int		j;

	j = 0;
	if (!ft_strchr(get_args[i], '='))
	{
		free(get_args[i]);
		return (shell->env);
	}
	new_env = calloc(tab_size(shell->env) + 2, sizeof(char *));
	while (shell->env[j])
	{
		new_env[j] = ft_strdup(shell->env[j]);
		j++;
	}
	new_env[j] = get_args[i];
	new_env[j + 1] = NULL;
	return (new_env);
}

// export all value to the shell->env.
static t_shell	*export_value(t_shell *shell, char **get_args)
{
	char	**finder;
	int		i;
	int		j;

	i = 0;
	while (get_args[i])
	{
		j = 0;
		finder = ft_split(get_args[i], '=');
		while (shell->env[j])
		{
			if (!ft_strncmp(finder[0], shell->env[j], ft_strlen(finder[0])) &&
				shell->env[j][ft_strlen(finder[0])] == '=')
			{
				shell->env[j] = new_value(shell, get_args, i, j);
				break ;
			}
			j++;
		}
		if (shell->env[j] == NULL)
			shell->env = add_value(shell, get_args, i);
		free_tab(finder);
		i++;
	}
	return (shell);
}


// export function main. create value or add it in the env if it does not exist.
t_shell	*ft_export(t_shell *shell)
{
	char	**get_args;

	shell->exit_code = 0;
	if (!shell->cmd[1])
	{
		no_arg_export(shell);
		return (shell);
	}
	get_args = get_unset_value(shell);
	shell = export_value(shell, get_args);
	free(get_args);
	return (shell);
}

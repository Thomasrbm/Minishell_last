/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:51:59 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 12:05:36 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void free_all(t_shell *shell, char **cmd_files, t_ast *ast)
{
	free_tab(ast->cmd);
	if (cmd_files)
		free_tab(cmd_files);
	free_tab(shell->cmd_flag);
	free_tab(shell->cmd);
	free_tab(shell->env);
	free(shell);
	free(ast);
	exit(1);
}

void	binary_path(t_shell *shell, t_ast *ast)
{
	if (shell->cmd_flag[0] == NULL)
	{
		free_tab(shell->cmd_flag);
		free_tab(ast->cmd);
		free(ast);
		exit(EXIT_FAILURE);
	}
	if (strchr(shell->cmd_flag[0], '/'))
	{
		if (access(shell->cmd_flag[0], X_OK) == 0)
		{
			if (execve(shell->cmd_flag[0], shell->cmd_flag, shell->env) == -1)
			{
				free_tab(shell->cmd_flag);
				ft_putstr_fd("execve failed\n", 2);
				free_tab(ast->cmd);
				free(ast);
				exit(EXIT_FAILURE);
			}
		}
	}
}

void	exec_path(t_shell *shell, t_ast *ast)
{
	int		i;
	char	**cmd_files;

	i = 0;
	cmd_files = split_path(shell->cmd_flag, shell->env);
	if (!cmd_files || !cmd_files[0])
		free_all(shell, cmd_files, ast);
	while (cmd_files[i] != NULL)
	{
		if (access(cmd_files[i], X_OK) == 0)
		{
			if (execve(cmd_files[i], shell->cmd_flag, shell->env) == -1)
				free_all(shell, cmd_files, ast);
		}
		i++;
	}
	ft_putstr_fd(shell->cmd_flag[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_all(shell, cmd_files, ast);
}

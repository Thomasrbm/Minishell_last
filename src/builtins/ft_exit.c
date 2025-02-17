/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:55:53 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 09:49:46 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void free_exit(t_shell *shell, t_ast *ast)
{
	if (shell->cmd_flag)
		free_tab(shell->cmd_flag);
	if 	(shell->cmd)
		free_tab(shell->cmd);
	if (shell->env)
		free_tab(shell->env);
	if (ast->cmd)
		free_tab(ast->cmd);
	if (ast)
		free(ast);
	free(shell);
}

int	ft_exit(t_shell *shell, t_ast *ast)
{
	long int	exit_status;
	int			i;

	i = 0;
	ft_putstr_fd("exit\n", 2);
	if (!shell->cmd[1])
	{
		free_exit(shell, ast);
		exit(0);
	}
	while (shell->cmd[1][i] != '\0')
	{
		if (!ft_isdigit(shell->cmd[1][i]))
		{
			ft_putstr_fd(("Minishell: exit: "), 2);
			ft_putstr_fd(shell->cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			free_exit(shell, ast);
			exit(2);
		}
		i++;
	}
	exit_status = ft_atol(shell->cmd[1]);
	free_exit(shell, ast);
	exit(exit_status);
}

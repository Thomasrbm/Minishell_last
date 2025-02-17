/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:51:23 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 14:03:11 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	t_shell	*open_file(t_shell *shell, int file_pos)
{
	shell->fd = open(shell->cmd[file_pos], O_RDONLY);
	if (shell->fd < 0)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(shell->cmd[file_pos], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->exit_code = 1;
		free_tab(shell->cmd_flag);
		return(shell);
	}
	if (access(shell->cmd[file_pos], R_OK) == -1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(shell->cmd[file_pos], 2);
		ft_putstr_fd(": permission denied\n", 2);
		shell->exit_code = 1;
		free_tab(shell->cmd_flag);
	}
	return(shell);
}

static t_shell	*input_exec(t_shell *shell, int fd, t_ast *ast)
{
	pid_t	pid;
	int		status;
	pid = fork();
	if (pid == -1)
		return (shell);
	if (pid == 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		binary_path(shell, ast);
		exec_path(shell, ast);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd);
	}
	shell->exit_code = status;
	return (shell);
}

//  $? >das 

static t_shell	*get_flag(t_shell *shell, int i, int file_pos, t_ast *ast)
{
	int		j;

	j = 0;
	shell->cmd_flag = calloc(tab_size(shell->cmd + 1), sizeof(char *));
	while (shell->cmd[i] && ft_strcmp(shell->cmd[i], "<"))
	{
		shell->cmd_flag[j] = ft_strdup(shell->cmd[i]);
		i++;
		j++;
	}
	shell->cmd_flag[j] = NULL;
	shell = open_file(shell, file_pos);
	if (shell->exit_code == 1)
		return(shell);
	shell = input_exec(shell, shell->fd, ast);
	free_tab(shell->cmd_flag);
	shell->cmd_flag = NULL; // SINON CA LEAK AVEC   $? > das + exit 
	return(shell);
}

int	redirect_input(t_shell *shell, t_ast *ast)
{
	int		i;
	int		file_pos;
	char	*token;

	i = 0;
	token = parse_cmd(shell->cmd, '<');
	free_tab(shell->cmd);
	shell->cmd = ft_split(token, ' ');
	free(token);
	while (ft_strcmp(shell->cmd[i], "<"))
		i++;
	file_pos = i + 1;
	if (i == 0)
		i = 2;
	else
		i = 0;
	shell->exit_code = 0;
	shell = get_flag(shell, i, file_pos, ast);
	return (shell->exit_code);
}

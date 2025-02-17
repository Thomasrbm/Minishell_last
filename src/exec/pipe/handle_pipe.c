/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:00:44 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/16 03:01:16 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**split_cmd_flag(t_shell *shell, int i, char *c)
{
	int		j;
	char	*tmp;
	char	*tmp2;

	j = 0;
	shell->cmd2 = malloc(sizeof(char *) * (tab_size(shell->cmd)));
	while (shell->cmd[i])
	{
		if (!ft_strncmp(shell->cmd[i], c, ft_strlen(shell->cmd[i])))
			i++;
		tmp = ft_strdup(shell->cmd[i]);
		i++;
		while (shell->cmd[i] && ft_strncmp(shell->cmd[i], c, ft_strlen(shell->cmd[i])))
		{
			tmp2 = ft_strjoin(tmp, " ");
			free(tmp);
			tmp = ft_strjoin(tmp2, shell->cmd[i]);
			free(tmp2);
			i++;
		}
		shell->cmd2[j] = tmp;
		j++;
	}
	shell->cmd2[j] = NULL;
	return (shell->cmd2);
}

static pid_t process_pipe(t_shell *shell, int i, int prev_pipefd[])
{
	pid_t   pid;
	char    **cmd_split;
	int     next_pipefd[2];

	if (i < tab_size(shell->cmd2) - 1)
	{
		if (pipe(next_pipefd) == -1)
			exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);

	if (pid == 0)
	{
		// Child process: Handle input from previous command
		if (i > 0)
		{
			dup2(prev_pipefd[0], STDIN_FILENO);
			close(prev_pipefd[0]);
			close(prev_pipefd[1]);
		}

		// Handle output to next command
		if (i < tab_size(shell->cmd2) - 1)
		{
			close(next_pipefd[0]);
			dup2(next_pipefd[1], STDOUT_FILENO);
			close(next_pipefd[1]);
		}

		cmd_split = ft_split(shell->cmd2[i], ' ');
		absolute_path_ms(cmd_split, shell->env, shell);
		exit(shell->exit_code); // Make sure to propagate exit code
	}
	else
	{
		// Parent process: Close unused pipe ends
		if (i > 0)
		{
			close(prev_pipefd[0]);
			close(prev_pipefd[1]);
		}
		if (i < tab_size(shell->cmd2) - 1)
		{
			prev_pipefd[0] = next_pipefd[0];
			prev_pipefd[1] = next_pipefd[1];
		}
	}
	return (pid);
}

t_shell *pipe_handler(t_shell *shell)
{
	int     i;
	int     prev_pipefd[2];
	pid_t   *wait_pid;
	int     status;

	i = -1;
	prev_pipefd[0] = -1;
	prev_pipefd[1] = -1;
	shell->cmd2 = split_cmd_flag(shell, 0, "|");
	wait_pid = malloc(sizeof(pid_t) * tab_size(shell->cmd2));

	while (++i < tab_size(shell->cmd2))
	{
		wait_pid[i] = process_pipe(shell, i, prev_pipefd);
	}

	// Close any remaining pipe ends in parent
	if (prev_pipefd[0] != -1)
		close(prev_pipefd[0]);
	if (prev_pipefd[1] != -1)
		close(prev_pipefd[1]);

	// Wait for all child processes
	i = -1;
	while (++i < tab_size(shell->cmd2))
		waitpid(wait_pid[i], &status, 0);

	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);

	free(wait_pid);
	return (shell);
}

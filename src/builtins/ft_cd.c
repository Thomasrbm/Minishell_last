/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:33:55 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/17 06:07:38 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// handle "cd" or "cd --" case wich return the cd to /home/USER_VALUE
static t_shell	*return_home(t_shell *shell, char *pwd)
{
	char	*home;

	home = get_env(shell, "HOME");
	if (!home)
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		shell->exit_code = 1;
		return (shell);
	}
	if ((access(home, X_OK)) == -1)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		ft_putstr_fd(home, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->exit_code = 1;
		free(home);
		return (shell);
	}
	chdir(home);
	free(home);
	shell->env = edit_env_value(shell, "OLDPWD", pwd);
	getcwd(pwd, 5000);
	shell->env = edit_env_value(shell, "PWD", pwd);
	shell->exit_code = 0;
	return (shell);
}

// handle "cd ~" case wich return the cd to /home/USER_VALUE
// this one is different because it work even with HOME unset
static t_shell	*return_home_2(t_shell *shell, char *pwd)
{
	char	*home;

	home = get_env(shell, "HOME");
	if (home)
	{
		if ((access(home, X_OK)) == -1)
		{
			free(home);
			ft_putstr_fd("Minishell: cd: ", 2);
			ft_putstr_fd(home, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			shell->exit_code = 1;
			return (shell);
		}
	}
	free(home);
	chdir(shell->home);
	shell->env = edit_env_value(shell, "OLDPWD", pwd);
	getcwd(pwd, 5000);
	shell->env = edit_env_value(shell, "PWD", pwd);
	shell->exit_code = 0;
	return (shell);
}

// change the cd to the new one .
static t_shell	*change_directory(t_shell *shell, char *pwd)
{
	if ((access(shell->cmd[1], X_OK)) == -1)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		ft_putstr_fd(shell->cmd[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->exit_code = 1;
		return (shell);
	}
	chdir(shell->cmd[1]);
	shell->env = edit_env_value(shell, "OLDPWD", pwd);
	getcwd(pwd, 5000);
	shell->env = edit_env_value(shell, "PWD", pwd);
	shell->exit_code = 0;
	return (shell);
}

// handle "cd -" case wich change directory to OLDPWD
static t_shell	*go_to_old_pwd(t_shell *shell, char *pwd)
{
	char	*old_pwd;

	old_pwd = get_env(shell, "OLDPWD");
	if (!old_pwd)
	{
		ft_putstr_fd("Minishell: cd: OLDPWD not set\n", 2);
		shell->exit_code = 1;
		return (shell);
	}
	chdir(old_pwd);
	shell->env = edit_env_value(shell, "OLDPWD", pwd);
	getcwd(pwd, 5000);
	shell->env = edit_env_value(shell, "PWD", pwd);
	shell->exit_code = 0;
	free(old_pwd);
	return (shell);
}

// cd function main
t_shell	*ft_cd(t_shell *shell)
{
	char	pwd[5000];

	getcwd(pwd, 5000);
	if (!shell->cmd[1] || !ft_strcmp(shell->cmd[1], "--"))
		return (return_home(shell, pwd));
	else if (ft_strchr(shell->cmd[1], '~'))
		return (return_home_2(shell, pwd));
	else if (ft_strchr(shell->cmd[1], '-'))
		return (go_to_old_pwd(shell, pwd));
	else
		return (change_directory(shell, pwd));
	ft_putstr_fd("Minishell: cd: ", 2);
	ft_putstr_fd(shell->cmd[1], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	shell->exit_code = 1;
	return (shell);
}

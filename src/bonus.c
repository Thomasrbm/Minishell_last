/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:42:00 by me                #+#    #+#             */
/*   Updated: 2025/02/17 01:54:57 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/*
** Nos fonctions locales, renommées en static pour éviter les conflits
*/
#include "minishell.h"

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == CMD_NODE && node->cmd)
		free_tab(node->cmd);
	free(node);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_tab(shell->env);
	free_tab(shell->cmd);
	free_tab(shell->cmd2);
	free(shell);
}

static char	**dup_tokens(char **t, int start, int end)
{
	int		i;
	int		len;
	char	**new_t;

	len = 0;
	while (t[len])
		len++;
	if (end == -1)
		end = len - 1;
	if (start > end || start >= len || end >= len)
		return (NULL);
	new_t = malloc(sizeof(char *) * (end - start + 2));
	if (!new_t)
		return (NULL);
	i = start;
	while (i <= end)
	{
		new_t[i - start] = ft_strdup(t[i]);
		i++;
	}
	new_t[i - start] = NULL;
	return (new_t);
}

static char	**ft_tabdup(char **tab)
{
	int		i;
	int		len;
	char	**new_tab;

	if (!tab)
		return (NULL);
	i = 0;
	len = 0;
	while (tab[len])
		len++;
	new_tab = malloc(sizeof(char *) * (len + 1));
	if (!new_tab)
		return (NULL);
	while (i < len)
	{
		new_tab[i] = ft_strdup(tab[i]);
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

static int		parse_factor(char **str, t_shell *shell);

static int	parse_expr(char **str, t_shell *shell)
{
	int	ret;

	ret = parse_factor(str, shell);
	while (**str && **str != ')')
	{
		while (**str == ' ')
			(*str)++;
		if (!**str || **str == ')')
			break ;
		if (((*str)[0] == '&' && (*str)[1] == '&') || ((*str)[0] == '|'
				&& (*str)[1] == '|'))
			(*str) += 2;
		ret = parse_factor(str, shell);
	}
	return (ret);
}

static int	parse_factor(char **str, t_shell *shell)
{
	char	**arr;
	int		i;

	(void)shell;
	i = 0;
	while (**str == ' ')
		(*str)++;
	if (**str == '(')
	{
		(*str)++;
		(void)parse_expr(str, shell);
		if (**str == ')')
			(*str)++;
		return (0);
	}
	arr = ft_split(*str, ' ');
	while ((*str)[i] && (*str)[i] != ' ' && (*str)[i] != ')')
		i++;
	(*str) += i;
	free_tab(arr);
	return (0);
}

void	error_message(char **cmd)
{
	if (!cmd || !cmd[0] || cmd[0][0] == '\0')
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return ;
	}

	if (ft_isspace(cmd[0][0])) // Vérifie si la commande commence par un espace
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return ;
	}

	// Gestion des chemins invalides
	if (cmd[0][0] == '/' || ft_strncmp(cmd[0], "./", 2) == 0 ||
	    (ft_strchr(cmd[0], '/') && ft_strncmp(cmd[0], "./", 2) != 0))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
}



// void	error_message(char **cmd)
// {
// 	if (!cmd || !cmd[0])
// 	{
// 		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
// 		return ;
// 	}
// 	/* Si la commande est un chemin absolu,
// 	   ou un chemin relatif commençant par "./",
// 	   ou contient un slash ailleurs (mais n'est pas "./...") */
// 	if (cmd[0][0] == '/' ||
// 	    ft_strncmp(cmd[0], "./", 2) == 0 ||
// 	    (ft_strchr(cmd[0], '/') != NULL && ft_strncmp(cmd[0], "./", 2) != 0))
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd[0], STDERR_FILENO);
// 		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
// 	}
// 	else
// 	{
// 		ft_putstr_fd(cmd[0], STDERR_FILENO);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 	}
// }


int	exec_builtin(char **cmd, t_shell *shell, t_ast *ast)
{
	if (!cmd || !cmd[0])
		return (shell->exit_code = 127, 127);
	if (ft_strcmp(cmd[0], "cd") == 0)
		shell = ft_cd(shell);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		shell->exit_code = ft_pwd();
	else if (ft_strcmp(cmd[0], "export") == 0)
		shell = ft_export(shell);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		shell = ft_unset(shell);
	else if (ft_strcmp(cmd[0], "env") == 0)
		shell->exit_code = ft_env(shell);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		shell = ft_echo(shell->cmd, shell);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		ft_exit(shell, ast);
	else
		return (shell->exit_code = 127);
	return (shell->exit_code);
}

char	*get_env_value(const char *var, t_shell shell)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (shell.env[i])
	{
		if (!ft_strncmp(shell.env[i], var, len) && shell.env[i][len] == '=')
			return (ft_strdup(shell.env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

static int	find_split_pos(char **tokens, int *op_type)
{
	int	j;
	int	paren;
	int	pos;

	j = 0;
	paren = 0;
	pos = -1;
	*op_type = 0;
	while (tokens[j])
	{
		if (!ft_strcmp(tokens[j], "("))
			paren++;
		else if (!ft_strcmp(tokens[j], ")"))
			paren--;
		else if (paren == 0)
		{
			if (!ft_strcmp(tokens[j], "&&"))
			{
				pos = j;
				*op_type = AND_NODE;
			}
			else if (!ft_strcmp(tokens[j], "||"))
			{
				pos = j;
				*op_type = OR_NODE;
			}
		}
		j++;
	}
	return (pos);
}

static t_ast	*build_ast_operator(char **t, int sp, int op_type)
{
	t_ast	*node;
	char	**left;
	char	**right;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	left = dup_tokens(t, 0, sp - 1);
	right = dup_tokens(t, sp + 1, -1);
	if (!left || !right)
	{
		free(node);
		free_tab(left);
		free_tab(right);
		return (NULL);
	}
	node->type = (op_type == AND_NODE) ? AND_NODE : OR_NODE;
	node->cmd = NULL;
	node->left = build_ast(left);
	node->right = build_ast(right);
	free_tab(left);
	free_tab(right);
	if (!node->left || !node->right)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

static t_ast	*build_ast_cmd(char **tokens)
{
	t_ast	*node;

	if (!tokens || !*tokens)
		return (NULL);
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = CMD_NODE;
	node->left = NULL;
	node->right = NULL;
	node->cmd = ft_tabdup(tokens);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

t_ast	*build_ast(char **tokens)
{
	int		split_pos;
	int		op_type;
	t_ast	*node;

	if (!tokens || !*tokens)
		return (NULL);
	split_pos = find_split_pos(tokens, &op_type);
	if (split_pos != -1)
		node = build_ast_operator(tokens, split_pos, op_type);
	else
		node = build_ast_cmd(tokens);
	return (node);
}

char	*ft_strjoin_free_both(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

static int	local_exec(char **cmd, t_shell *sh)
{
	if (access(cmd[0], X_OK) == 0)
		execve(cmd[0], cmd, sh->env);
	error_message(cmd);
	sh->exit_code = 127;
	return (127);
}

static int	path_exec2(char **cmd, char **ps, t_shell *sh, int j)
{
	char	*fp;

	fp = ft_strjoin(ps[j], "/");
	if (!fp)
		return (127);
	fp = ft_strjoin_free_both(fp, ft_strdup(cmd[0]));
	if (fp && access(fp, X_OK) == 0)
	{
		execve(fp, cmd, sh->env);
		free(fp);
		free_tab(ps);
		error_message(cmd);
		error_message(cmd);
		sh->exit_code = 127;
		exit(127);
	}
	free(fp);
	return (-1);
}

static int	path_exec(char **cmd, t_shell *sh, char *pv)
{
	char	**ps;
	int		j;
	int		ret;

	ps = ft_split(pv, ':');
	free(pv);
	if (!ps)
	{
		error_message(cmd);
		sh->exit_code = 127;
		return (127);
	}
	j = 0;
	while (ps[j])
	{
		ret = path_exec2(cmd, ps, sh, j);
		if (ret != -1)
		{
			free_tab(ps);
			return (ret);
		}
		j++;
	}
	free_tab(ps);
	error_message(cmd);
	sh->exit_code = 127;
	return (127);
}
char **split_env_path(char **cmd, char **env)
{
    char *pathvar = NULL;
    char **dirs = NULL;
    char **res = NULL;
    char *tmp = NULL;
    int i = 0;

    // Chercher la variable PATH dans l'environnement
    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            pathvar = env[i] + 5;
            break;
        }
        i++;
    }

    // Si PATH n'est pas trouvé, retourner NULL
    if (!pathvar)
        return (NULL);

    // Spliter le chemin en répertoires
    dirs = ft_split(pathvar, ':');
    if (!dirs)
        return (NULL);

    // Compter les éléments dans dirs
    i = 0;
    while (dirs[i])
        i++;

    // Allouer le tableau de résultats
    res = malloc(sizeof(char *) * (i + 1));
    if (!res)
    {
        free_tab(dirs);
        return (NULL);
    }

    // Pour chaque répertoire, construire le chemin complet
    i = 0;
    while (dirs[i])
    {
        tmp = ft_strjoin(dirs[i], "/");
        if (!tmp)
        {
            free_tab(dirs);
            free_tab(res);
            return (NULL);
        }
        res[i] = ft_strjoin_free_both(tmp, ft_strdup(cmd[0]));
        if (!res[i])
        {
            free_tab(dirs);
            free_tab(res);
            return (NULL);
        }
        i++;
    }
    res[i] = NULL;
    free_tab(dirs);
    return (res);
}



int	check_first(char *argv, char **env)
{
	char	**cmd;
	char	**cmd_files;
	int		i;

	cmd = ft_split(argv, ' ');
	cmd_files = split_env_path(cmd, env);
	i = 0;
	while (cmd_files[i])
	{
		if (access(cmd_files[i], X_OK) == 0)
		{
			free_tab(cmd);
			free_tab(cmd_files);
			return (0);
		}
		i++;
	}
	free_tab(cmd);
	free_tab(cmd_files);
	return (1);
}


int	exec_cmd_external(char **cmd, t_shell *sh)
{
	char	*pv;
	char	*value;

	if (!cmd || !cmd[0])
		return (127);
	if (cmd[0][0] == '$')
	{
		value = get_env_value(cmd[0] + 1, *sh);
		if (value && value[0])
		{
			ft_putendl_fd(value, STDOUT_FILENO);
			free(value);
		}
		else if (value)
			free(value);
		return (0);
	}
	if (ft_strchr(cmd[0], '/'))
		return (local_exec(cmd, sh));
	if (check_first(cmd[0], sh->env) != 0)
	{
		error_message(cmd);
		sh->exit_code = 127;
		return (127);
	}
	pv = get_env_value("PATH", *sh);
	if (!pv || !pv[0])
	{
		error_message(cmd);
		sh->exit_code = 127;
		if (pv)
			free(pv);
		return (127);
	}
	return (path_exec(cmd, sh, pv));
}



int tab_size(char **tab)
{
	int i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}


int exec_ast(t_ast *ast, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*value;

	if (!ast)
		return (1);
	if (ast->type == CMD_NODE)
	{
		if (!ast->cmd || !ast->cmd[0])
			return (1);
		if (!ft_strcmp(ast->cmd[0], "/"))
		{
			error_message(ast->cmd);
			free_tab(ast->cmd);
			ast->cmd = NULL;
			return (127);
		}
		if (ast->cmd[0][0] == '$')
		{
			value = get_env_value(ast->cmd[0] + 1, *shell);
			if (value && value[0])
				ft_putendl_fd(value, STDOUT_FILENO);
			free(value);
			free_tab(ast->cmd);
			ast->cmd = NULL;
			return (0);
		}
		if (is_builtin(shell)->check == 0)
			return (exec_builtin(ast->cmd, shell, ast));
		if (is_redirection(shell)->check == 0)
			return (exec_redirection(shell, ast));
		if (check_single_cmd(shell))
			return (shell->exit_code);
		if (ast->cmd[1] && !ft_strchr(ast->cmd[0], '/') &&
			check_first(ast->cmd[0], shell->env))
		{
			error_message(ast->cmd);
			shell->exit_code = 127;
			free_tab(ast->cmd);
			ast->cmd = NULL;
			return (127);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return (127);
		}
		if (pid == 0)
		{
			setup_signals(0);
			status = exec_cmd_external(ast->cmd, shell);
			exit(status);
		}
		return (wait_ignore(pid));
	}
	status = exec_ast(ast->left, shell);
	if ((ast->type == AND_NODE && status == 0) ||
	    (ast->type != AND_NODE && status != 0))
		return (exec_ast(ast->right, shell));
	return (status);
}

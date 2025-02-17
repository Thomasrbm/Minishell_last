/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:17:42 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 16:54:29 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_quote(char *input, t_shell *shell)
{
	t_quote	q;

	if (!input || !*input) // Ajouter une vérification pour les entrées vides
		return (ft_strdup(""));

	q.i = 0;
	q.start = 0;
	q.res = ft_strdup("");
	q.quote = 0;
	while (input[q.i])
	{
		// Traite les quotes une fois détectées
		if ((input[q.i] == '\'' || input[q.i] == '"') && (q.quote == 0
				|| input[q.i] == q.quote))
			process_quote_block(&q, input, shell);
		q.i++;
	}
	return (process_end_quote(input, shell, q));
}


void process_wildcards(t_shell *shell)
{
	char **expanded_cmd;
	int i;

	expanded_cmd = expand_wildcards(shell->cmd);
	free_tab(shell->cmd);
	shell->cmd = expanded_cmd;
	
	if (shell->cmd)
	{
		i = 0;
		while (shell->cmd[i]) // Ensure loop stops at NULL
			i++;
		char **temp = realloc(shell->cmd, (i + 1) * sizeof(char *));
		if (temp)
		{
			shell->cmd = temp;
			shell->cmd[i] = NULL; // Ensure NULL termination
		}
	}
}

char	*process_quote_if_closed(char *input, t_shell *shell)
{
	char	*res;

	// Si l'entrée est vide ou incorrecte, on retourne une chaîne vide.
	if (!input || !*input || !is_quote_closed(input))
		return (ft_strdup(""));  // Retourne une chaîne vide valide

	// Traite normalement les quotes
	res = process_quote(input, shell);
	return (res);
}

char *remove_quotes(const char *str)
{
    if (!str)  // Si la chaîne est NULL, retourne NULL
        return (NULL);
    char *clean;
    int i, j;
    int in_single, in_double;

    clean = malloc(ft_strlen(str) + 1);  // Allocation mémoire
    if (!clean)  // Vérification d'allocation
        return (NULL);

    i = 0;
    j = 0;
    in_single = 0;
    in_double = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        else
            clean[j++] = str[i];
        i++;
    }
    clean[j] = '\0';
    return (clean);
}




// static char	*get_processed_input(char *inp, t_shell *shell)
// {
// 	char	*proc;
// 	int		i;

// 	if (inp[0] == '$')
// 	{
// 		if (inp[1] == '?' && inp[2] == '\0')
// 			proc = ft_itoa(shell->exit_code);
// 		else
// 		{
// 			i = 1;
// 			while (inp[i] && (ft_isalnum(inp[i]) || inp[i] == '_'))
// 				i++;
// 			if (inp[i] == '\0')
// 			{
// 				proc = get_env_value(inp + 1, *shell);
// 				if (!proc)
// 					proc = ft_strdup("");
// 			}
// 			else
// 				proc = process_quote_if_closed(inp, shell);
// 		}
// 	}
// 	else
// 		proc = process_quote_if_closed(inp, shell);
// 	return (proc);
// }

int	check_single_cmd(t_shell *shell)
{
	if (shell->cmd && shell->cmd[0] && !shell->cmd[1])
	{
		if (ft_strchr(shell->cmd[0], '/'))
		{
			if (access(shell->cmd[0], X_OK) != 0)
			{
				error_message(shell->cmd);
				shell->exit_code = 127;
				shell->cmd = free_tab(shell->cmd);
				return (1);
			}
		}
		else if (check_first(shell->cmd[0], shell->env))
		{
			error_message(shell->cmd);
			shell->exit_code = 127;
			shell->cmd = free_tab(shell->cmd);
			return (1);
		}
	}
	return (0);
}



int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	only_quotes(const char *str)
{
	int	i;
	int	in_single;
	int	in_double;
	int	has_non_space;

	if (!str || *str == '\0')
		return (0);

	i = 0;
	in_single = 0;
	in_double = 0;
	has_non_space = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (!ft_isspace(str[i]))
			has_non_space = 1;
		i++;
	}
	// Retourne vrai si la chaîne est entourée de quotes fermées et ne contient que des espaces
	return (!in_single && !in_double && !has_non_space);
}


static int	handle_cycle(t_shell *shell)
{
	char	*inp;
	char	*proc;
	char	*clean_cmd;
	t_ast	*ast;

	inp = get_readline(shell);
	if (!inp)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		return (-1);
	}
	if (g_signal == SIGINT)
		get_sigint(shell);

	// Traiter les quotes avant d'expander
	proc = process_quote_if_closed(inp, shell);
	free(inp);

	// Si l'entrée est vide ou a des quotes fermées sans contenu, on la ignore
	if (!proc || !is_quote_closed(proc) || only_quotes(proc))
	{
		free(proc);
		return (0);
	}

	// On enlève les quotes internes et on traite la commande
	clean_cmd = remove_quotes(proc);
	free(proc);

	// Vérifier si l'allocation de shell->cmd a réussi
	shell->cmd = free_tab(shell->cmd);
	shell->cmd = malloc(sizeof(char *) * 2);
	if (!shell->cmd)  // Vérification d'allocation
	{
		free(clean_cmd);
		return (0);  // Retourne tôt si échec d'allocation
	}
	shell->cmd[0] = clean_cmd;
	shell->cmd[1] = NULL;

	if (shell->cmd[0])
	{
		ast = build_ast(shell->cmd);
		if (ast)
		{
			shell->exit_code = exec_ast(ast, shell);
			free_ast(ast);
		}
	}
	shell->cmd = free_tab(shell->cmd);
	return (0);
}



// '        '


//  juste saut de ligne 

int shell_loop(t_shell *shell)
{
    int exit_code;

    while (1)
    {
        shell->fd = 0;
        exit_code = handle_cycle(shell);
        if (exit_code == -1)
            break;
    }
    clear_history();
    free_tab(shell->env);
    exit_code = shell->exit_code; // Sauvegarde le exit code
    free(shell);
    return (exit_code);
}


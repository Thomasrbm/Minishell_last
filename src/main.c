/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 00:56:09 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 17:01:53 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Dans main.c

t_shell	*is_redirection(t_shell *s)
{
	if (!s || !s->cmd || !s->cmd[0])
		return (s);
	if (ft_tabchr(s->cmd, '<') || ft_tabchr(s->cmd, '>'))
		s->check = 0;
	else
		s->check = 1;
	return (s);
}

t_shell	*is_builtin(t_shell *s)
{
	if (!s || !s->cmd || !s->cmd[0])
		return (s);
	if (!ft_strcmp("cd", s->cmd[0]) || !ft_strcmp("pwd", s->cmd[0])
		|| !ft_strcmp("export", s->cmd[0]) || !ft_strcmp("unset", s->cmd[0])
		|| !ft_strcmp("env", s->cmd[0]) || !ft_strcmp("echo", s->cmd[0])
		|| !ft_strcmp("exit", s->cmd[0]))
		s->check = 0;
	else
		s->check = 1;
	return (s);
}
/* main.c */
int    exec_redirection(t_shell *shell, t_ast *ast)
{
    if (ft_tabchr(shell->cmd, '<'))
	{
        shell->exit_code = redirect_input(shell, ast);
	}
    else if (ft_tabchr(shell->cmd, '>'))
        shell->exit_code = redirect_output(shell, ast);
	else
	{
		error_message(ast->cmd);
		shell->exit_code = 127;
	}
    return (shell->exit_code);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		i;
	int		env_count;
	int		exit_code;

	(void)argv;
	if (argc != 1)
		return (1);
	setup_signals(1);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	env_count = tab_size(env);
	shell->env = malloc(sizeof(char *) * (env_count + 1));
	if (!shell->env)
		return (free(shell), 1);
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		i++;
	}
	shell->env[i] = NULL;
	shell->home = getenv("HOME");
	shell->exit_code = 0;
	shell->cmd = NULL;
	shell->cmd2 = NULL;
	shell->cmd_flag = NULL;
	exit_code = shell_loop(shell);
	return (exit_code);
}


// RAJOUTER LE CTRL + C et direct au exit  avec getsint  

// false&&echo d


// minishell > $ DOIT ECRIRE 
// $: command not found

// $HOME (seul) LEAK  +  PWD ( fork en balle si / )


// throbert@c2r6p5:~/projets/minishell$ '        '
//         : command not found
// throbert@c2r6p5:~/projets/minishell$ 
















//  $? > $?
// pas le bon exit code


//  $? > das
// pas le bon exit code 



// < Makefile cat >  Makefile  leak  invalid read



// les exit negatif ne sont pas pris en compte


//  bash                
// throbert@c2r6p5:~/projets/minishell$ exit 5 asd
// exit
// bash: exit: too many arguments
// throbert@c2r6p5:~/projets/minishell$ $?
// 1: command not found
// throbert@c2r6p5:~/projets/minishell$ 
// exit
// → ~/projets/minishell ./minishell
// minishell > exit 5 asd
// exit
// → ~/projets/minishell 


//  ./minishell
// minishell > exit 555555555555555555555555555555555555555555555555555555555555555555555555555555
// exit
// → ~/projets/minishell $?         
// zsh: command not found: 227
// → ~/projets/minishell bash
// throbert@c2r6p5:~/projets/minishell$ exit 555555555555555555555555555555555555555555555555555555555555555555555555555555
// exit
// bash: exit: 555555555555555555555555555555555555555555555555555555555555555555555555555555: numeric argument required
// → ~/projets/minishell $?         
// zsh: command not found: 2
// → ~/projets/minishell 



// IL FAUT QUE EXPORT (seul) trie en ordr alpha


// export = CRASH 


// bash 
// throbert@c2r6p5:~/projets/minishell$ export =d
// bash: export: `=d': not a valid identifier
// throbert@c2r6p5:~/projets/minishell$ 

// export s=s 
// export s= 
// env : s doit = (vide) pas NULL


// export sans = (ssssssssssssssssssssssssssssssssssssss)  , n est pas dans le env mais s affiche dans export(sans rien)


// throbert@c2r6p5:~/projets/minishell$ export 5=asd
// bash: export: `5=asd': not a valid identifier



// env -i valgrind ./minishell  CRASH pck env -i desactive les variables d environnement

// unset l crash

// unset PATH fonctionne pas  + leak a ctrl + d



// ls >> test > tt  ( test n est pas crer) + nom de fihier icorect (tt )


// < Makefile < asd cat leak + fonctionne pas




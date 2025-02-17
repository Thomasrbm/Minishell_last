/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 09:12:37 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 13:37:29 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// signal can be from user : ctrl+ c etc, from system : segfault,
// or from another process (kill -9 PID)
// 
// volatile = modifiable only by the signal
// sig_atomic_t = safe for asynchronous function sig_handler (for global)
//
// The sig handler may be called at any time, so it's safer to use a
// sig_atomic variable (written in a single operation on every system).
// Using int could pose problems if modified asynchronously.
volatile sig_atomic_t	g_signal = 0;

// sig_handler is called by default by the kernel. Here we modify it.
//
//
// Asynchronous: can be called anytime even if not in the current order.
//
// it come from signal.h: parameters cannot be modified, but we can change
// inside.
//
// This function is automatically called when a signal is received.
// Inside we recover the signal and store it in the global variable.
//
// If we receive SIGINT (ctrl+c), we jump a line, clear the prompt if typing
// set a new line before us, and redisplay the prompt.
void	sig_handler(int signum)
{
	g_signal = signum;
	if (g_signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

// This is not a function from signal.h.
// We initialize signals at the beginning of main. in parent mode
// 
// and in access path in child mode
//
// struct sigaction from signal.h:
// int sigaction(int signum, const struct sigaction *act,
//               struct sigaction *old_act);
//
// sigaction gives a new action to a signal: signum (SIGINT, etc),
// act (new action) and struc where stock old_action (NULL if not needed).
//
// sigemptyset clears the mask of signals to block (none are blocked).
//
// sa.sa_flags: configuration flags. SA_RESTART restarts syscalls.
// SIG_IGN is used to ignore signals.
//
// Disable ctrl+\.
//
//
// struct sa ; 
// sa_handler : pointeur sur sig_handler
// sa_mask :  signaux bloqués durant le handler
// sa_flags : options de comportement (ex : SA_RESTART)
// + 2 autre useless ici
void	setup_signals(int mode)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa)); // Initialize all fields to zero
	sigemptyset(&sa.sa_mask); // sige = pour vider + sa mask pour les mask
	if (mode == 1) // Mode parent (Minishell)
	{
		sa.sa_flags = SA_RESTART; // pour pas quit en ctrl c si trop spam
		sa.sa_handler = sig_handler;
		sigaction(SIGINT, &sa, NULL);
		sa.sa_handler = SIG_IGN; 
		sigaction(SIGQUIT, &sa, NULL);
		sigaction(SIGTSTP, &sa, NULL);
	}
	else // Mode enfant (execve)
	{
		sa.sa_handler = SIG_DFL; 
		sigaction(SIGQUIT, &sa, NULL);
		sigaction(SIGTSTP, &sa, NULL);
	}
}

// wait_pid_ignore_signals: waits for a child process to finish,
// and ignores ctrl+c and ctrl+\ during the wait.
//
// If a child process is executed, this function waits for its end.
// Ctrl+C and Ctrl+\ are ignored during the wait.
void	wait_pid_ignore_signals(pid_t pid, int *exit_code)
{
	struct sigaction	ignore_sig; // les structure stock la config des signaux
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;

	ignore_sig.sa_handler = SIG_IGN;
	sigemptyset(&ignore_sig.sa_mask);
	ignore_sig.sa_flags = 0; // no flag
	sigaction(SIGINT, &ignore_sig, &old_sigint); // stock sigint orignal et ignore actuel on a deja ignore mais la on ignore pour le minishell
	sigaction(SIGQUIT, &ignore_sig, &old_sigquit); // sinon ctrl + \ quitte le minishell et ctrl backslash aussi
	waitpid(pid, exit_code, WUNTRACED); // waitpid attend la fin du processus, stock exit code, WUNTRACED pour prendre ctrl z en compte
	sigaction(SIGINT, &old_sigint, NULL); // reactive celui du minishell
	sigaction(SIGQUIT, &old_sigquit, NULL);
}

int	wait_ignore(pid_t pid)
{
	int	exit_code;

	wait_pid_ignore_signals(pid, &exit_code); // le & permet de stocker le exit code
	if (WIFSTOPPED(exit_code)) // si ctrl z 
	{
		g_signal = WSTOPSIG(exit_code);
		ft_putstr_fd("\n[1]+  Stopped\n", STDERR_FILENO);
	}
	else if (WIFSIGNALED(exit_code)) // si ctrl c ou ctrl backslash
	{
		g_signal = WTERMSIG(exit_code);
		if (g_signal == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		else if (g_signal == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (WIFEXITED(exit_code))
		return (WEXITSTATUS(exit_code));
	return (128 + g_signal);
}


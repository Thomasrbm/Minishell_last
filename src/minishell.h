/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:24:32 by throbert          #+#    #+#             */
/*   Updated: 2025/02/17 15:04:14 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <dirent.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile __sig_atomic_t	g_signal;

/* Structures */
typedef enum e_ast_type
{
	CMD_NODE,
	AND_NODE,
	OR_NODE
}								t_ast_type;

typedef struct s_ast
{
	t_ast_type					type;
	char						**cmd;
	struct s_ast				*left;
	struct s_ast				*right;
}								t_ast;

typedef struct s_wildcard
{
	int							s;
	int							p;
	int							star;
	int							match;
}								t_wildcard;

typedef struct t_shell
{
	int							exit_code;
	char						**env;
	char						**cmd;
	char						**cmd2;
	int							check;
	char						*home;
	char						**cmd_flag;
	int							fd;
	int							i;
}								t_shell;

typedef struct t_pipe
{
	int							pipefd[2];
	int							child_count;
	int							infile;
	int							outfile;
	pid_t						*wait_pid;
}								t_pipe;

typedef struct s_quote
{
	int							i;
	int							start;
	char						quote;
	char						*res;
	char						*seg;
	char						*tmp;
	char						*new_res;
}								t_quote;

/* builtins */
int								ft_pwd(void);
int								ft_env(t_shell *shell);
char							**edit_env_value(t_shell *shell, char *value,
									char *new_one);
t_shell							*ft_unset(t_shell *shell);
char							**get_unset_value(t_shell *shell);
t_shell							*ft_export(t_shell *shell);
t_shell							*ft_cd(t_shell *shell);
t_shell							*ft_echo(char **cmd, t_shell *shell);

/* Redirections */
int								redirect_input(t_shell *shell, t_ast *ast);
int								redirect_output(t_shell *shell, t_ast *ast);
void							binary_path(t_shell *shell, t_ast *ast);
void							exec_path(t_shell *shell, t_ast *ast);
char							**split_path(char **cmd, char **env);
char							*find_env(char **env);
char							*parse_cmd(char **tab, char delim);

/* Gestion des erreurs */
void							error_message(char **cmd);
char							*ft_strjoin_free(char *s1, char *s2);
int								check_single_cmd(t_shell *shell);

/* Builtins */
t_shell							*is_redirection(t_shell *s);
t_shell							*is_builtin(t_shell *s);
int								exec_redirection(t_shell *shell, t_ast *ast);
int								ft_exit(t_shell *shell, t_ast *ast);

/* Variables d'environnement */
char							*process_dollar(char *s, int *i, t_shell shell);
char							*get_env_value(const char *var, t_shell shell);
char							*find_env_value(t_shell *shell, char *var);
char							*expand_dollar(char *s, t_shell shell);
char							*get_env(t_shell *shell, char *value);
char							**edit_env_value(t_shell *shell, char *value,
									char *new_one);
char							**get_unset_value(t_shell *shell);
char							**split_env_path(char **cmd, char **env);

/* Gestion mémoire */
void							free_struct(t_shell *shell);

/* AST */
t_ast							*build_ast(char **cmd);
int								exec_ast(t_ast *ast, t_shell *shell);
void							free_ast(t_ast *node);
void							get_sigint(t_shell *shell);
void							exec_cmd_ms(char **cmd, char **env,
									t_shell *shell);

/* Commandes et Exécution */
int								absolute_path_ms(char **cmd, char **env,
									t_shell *shell);
void							exec_cmd(char **cmd, char **env, t_pipe pipex);
void							open_error(void);
void							handle_pipe(t_pipe pipex, int argc, char **argv,
									char **env);
void							check_first_arg(char *argv, char **env);
int								pipex(int argc, char **argv, char **env);
int								absolute_path_first(char **cmd);
int								check_first(char *argv, char **env);

/* Gestion des guillemets */
char							**handle_quote(t_shell *shell);
char							*handle_dquote(char *cmd_quote);
char							*handle_squote(char *cmd_quote);
const char						*skip_quote(const char *str);
void							process_quote_block(t_quote *q, char *input,
									t_shell *shell);
char							*process_end_quote(char *input, t_shell *shell,
									t_quote q);
char							*process_quote(char *input, t_shell *shell);
t_shell							ft_split_with_quote(char *str, char sep,
									t_shell shell);
int								is_quote_closed(const char *str);

/* Here Doc */
t_shell							*here_doc_simple(t_shell *shell);
t_shell							*here_doc_to_file(t_shell *shell);
t_pipe							handle_here(char **argv, int argc,
									t_shell *shell, int i);
void							here_doc(char **argv, int pipe_fd[2],
									t_shell *shell, int i);
t_shell							*pipe_handler(t_shell *shell);
void							here_doc_exec(char **argv, int pipe_fd[2],
									t_shell shell);
t_pipe							handle_here_doc(char **argv, int argc,
									t_shell *shell);

/* Wildcards */
int								wildcard_match(char *pattern, char *str,
									t_wildcard *wc);
int								contains_wildcard(char *str);
t_list							*create_node_from_entry(char *entry_name,
									t_list **lst, DIR *dir);
t_list							*expand_arg_list(char *p);

/* Boucle du shell */
int								shell_loop(t_shell *shell);
t_shell							*handle_cmd(t_shell *shell);
int								ft_isspace(char c);

/* gestion de signaux */
int								wait_ignore(pid_t pid);
void							setup_signals(int mode);

/* wildcards */

char							**expand_wildcards(char **cmd);

/* readline */
char							*get_readline(t_shell *shell);

#endif

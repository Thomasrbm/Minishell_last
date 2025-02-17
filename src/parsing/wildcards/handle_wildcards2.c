/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:07:47 by throbert          #+#    #+#             */
/*   Updated: 2025/02/15 21:14:18 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



/* Retourne 1 si la chaîne contient '*', 0 sinon */ // 
int	contains_wildcard(char *str) // Fonction qui détermine si 'str' contient un caractère '*'
{
	int	i; // Indice pour parcourir la chaîne

	i = 0; // Initialisation de l'indice i
	if (!str) // Si 'str' est NULL
		return (0); // Retourne 0 (aucun wildcard)
	while (str[i]) // Tant que l'on n'est pas à la fin de la chaîne
	{
		if (str[i] == '*') // Si le caractère courant est '*'
			return (1); // Retourne 1 (wildcard trouvé)
		i++; // Incrémente l'indice i
	}
	return (0); // Aucun wildcard trouvé, retourne 0
}

/* ************************************************************************** */ // 
/*             EXPANSION D'UN ARGUMENT AVEC LISTE CHAÎNÉE                   */ // 
/* ************************************************************************** */ // 

/* Crée un noeud à partir du nom d'entrée avec gestion d'erreur */ // 
t_list	*create_node_from_entry(char *entry_name, t_list **lst, DIR *dir) // Fonction qui crée un noeud contenant 'entry_name'
{
	t_list	*new_node; // Pointeur vers le nouveau noeud

	new_node = ft_lstnew(ft_strdup(entry_name)); // Crée un nouveau noeud avec une copie de 'entry_name'
	if (!new_node) // Si la création échoue
	{
		ft_lstclear(lst, free); // Libère la liste existante
		closedir(dir); // Ferme le répertoire
	}
	return (new_node); // Retourne le noeud (ou NULL en cas d'erreur)
}



static int	process_entry(char *p, t_list **lst,
	struct dirent *e, DIR *d, t_wildcard *wc)
{
	t_list	*node;

	if (e->d_name[0] == '.' &&
	    (p[0] != '.' || (p[0] == '*' && p[1] != '.')))
		return (0);
	if (wildcard_match(p, e->d_name, wc))
	{
		node = create_node_from_entry(e->d_name, lst, d);
		if (!node)
			return (-1);
		ft_lstadd_back(lst, node);
	}
	return (0);
}

// Entrée : "a*" (wildcard pattern)
// Répertoire courant : {"apple", "alpha", "arcade", "banana", "notes.txt"}
// Sortie attendue : { "apple" -> "alpha" -> "arcade" }
t_list	*expand_arg_list(char *p)
{
	DIR				*dir;
	struct dirent	*e;
	t_list			*lst = NULL;
	t_wildcard		wc = { .star = -1, .match = 0 };

	dir = opendir(".");
	if (!dir)
		return (NULL);
	while ((e = readdir(dir)))
	{
		if (process_entry(p, &lst, e, dir, &wc) < 0)
		{
			closedir(dir);
			return (NULL);
		}
	}
	closedir(dir);
	return (lst);
}

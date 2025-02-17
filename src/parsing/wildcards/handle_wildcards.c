/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:33:19 by throbert          #+#    #+#             */
/*   Updated: 2025/02/16 23:33:17 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Retourne une liste chaînée des fichiers correspondant au motif */ //









/* ************************************************************************** */ // 
/*              TRAITEMENT D'UN ARGUMENT INDIVIDUEL                          */ // 
/* ************************************************************************** */ // 

/* Retourne la liste d'expansion pour un argument donné */ // 

// Entrée : "a*"
// Sortie : { "apple" -> "alpha" -> "arcade" }   GRACE A expand_arg_list
// SINON : { ls }
t_list	*process_argument(char *arg)
{
	t_list	*lst; // Pointeur vers la liste d'expansion

	if (contains_wildcard(arg)) // Si l'argument contient un wildcard
	{
		lst = expand_arg_list(arg); // Obtient la liste d'expansion correspondant à l'argumen
		// lst =   { "apple" -> "alpha" -> "arcade" }
		if (lst)
			return (lst);
		else
			return (ft_lstnew(ft_strdup(arg))); // Sinon, retourne un noeud contenant l'argument original
	}
	else
		return (ft_lstnew(ft_strdup(arg))); // Si aucun wildcard, retourne un noeud avec l'argument
		// return ls, 
}

/* ************************************************************************** */ // 
/*          EXPANSION DES WILDCARDS POUR TOUS LES ARGUMENTS                   */ // 
/* ************************************************************************** */ // 

/* Construit une liste chaînée avec l'expansion des wildcards */ // 

// entreee : // cmd = ["ls", "a*"]
// sortie : // result = {"ls" -> "apple" -> "alpha" -> "arcade"}
t_list	*expand_wildcards_list(char **cmd)
{
	t_list	*result; // Liste résultat globale
	t_list	*arg_lst; // Liste d'expansion pour un argument
	t_list	*tmp_node; //ptr temp pour la liste
	int		i;

	result = NULL;
	i = 0;
	while (cmd[i])
	{
		arg_lst = process_argument(cmd[i]); // Traite l'argument pour obtenir sa liste d'expansion
		tmp_node = arg_lst; // Initialise le pointeur temporaire à la tête de arg_lst
		while (tmp_node) // Parcourt la liste d'expansion
		{
			ft_lstadd_back(&result, ft_lstnew(ft_strdup(tmp_node->content))); // Ajoute une copie du contenu à la liste résultat
			tmp_node = tmp_node->next; // Passe au noeud suivant
		}
		ft_lstclear(&arg_lst, free); // Libère la liste d'expansion temporaire
		i++; // Incrémente l'indice pour le prochain argument
	}
	return (result); // Retourne la liste globale d'expansion
}


/* Convertit une liste chaînée en tableau de chaînes */ // 
// Entrée : { "ls" -> "apple" -> "alpha" -> "arcade" }
// Sortie : ["ls", "apple", "alpha", "arcade", NULL]
char	**lst_to_tab(t_list *lst)
{
	int		size; // Taille de la liste
	char	**tab; // Tableau de chaînes à construire
	int		i; // Indice pour le tableau
	t_list	*tmp; // Pointeur temporaire pour parcourir la liste

	size = ft_lstsize(lst);
	tab = malloc(sizeof(char *) * (size + 1)); // Alloue le tableau avec une case supplémentaire pour le NULL final
	if (!tab)
		return (NULL);
	i = 0;
	tmp = lst; // init a la tête de la liste
	while (tmp)
	{
		tab[i] = ft_strdup(tmp->content); // Duplique noeud dans tab
		i++;
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (tab);
}


/* Expands wildcards in cmd and returns a new array of arguments */ // 

// cmd = ["ls", "a*"]
char	**expand_wildcards(char **cmd)
{
	char	**tab;
	t_list	*lst;

	if (!cmd || !*cmd) // Vérification NULL
		return (NULL);
		
	lst = expand_wildcards_list(cmd);
	if (!lst)
		return (NULL);
		
	tab = lst_to_tab(lst);
	ft_lstclear(&lst, free);
	
	return (tab);
}

// fix le cat
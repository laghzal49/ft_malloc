/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaghzal <tlaghzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 05:59:46 by tlaghzal          #+#    #+#             */
/*   Updated: 2026/03/28 06:13:12 by tlaghzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#include <stdlib.h>

t_list_gc	**get_manger(void)
{
	static t_list_gc	*gc = NULL;

	return (&gc);
}

void	*ft_malloc(size_t size)
{
	t_list_gc	*node;

	node = malloc(sizeof(t_list_gc) + size);
	if (!node)
		return (NULL);
	node->next = *get_manger();
	node->prev = NULL;
	if (*get_manger() != NULL)
		(*get_manger())->prev = node;
	*get_manger() = node;
	return (node + 1);
}

void	free_all(void)
{
	t_list_gc	*curr;
	t_list_gc	*next;

	curr = *get_manger();
	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	*get_manger() = NULL;
}

void	ft_panic(void *ptr)
{
	if (ptr)
		ft_free(ptr);
	free_all();
	exit(1);
}

void	ft_free(void *ptr)
{
	t_list_gc	*node;

	if (!ptr)
		return ;
	node = (t_list_gc *)ptr - 1;
	if (node->prev)
		node->prev->next = node->next;
	else
		*get_manger() = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node);
}

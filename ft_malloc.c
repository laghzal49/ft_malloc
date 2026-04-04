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

pthread_mutex_t	*get_malloc_mutex(void)
{
	static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

	return (&mutex);
}

void	*ft_malloc(size_t size)
{
	t_list_gc	*node;

	pthread_mutex_lock(get_malloc_mutex());
	node = malloc(sizeof(t_list_gc) + size);
	if (!node)
	{
		pthread_mutex_unlock(get_malloc_mutex());
		return (NULL);
	}
	node->next = *get_manger();
	node->prev = NULL;
	if (*get_manger() != NULL)
		(*get_manger())->prev = node;
	*get_manger() = node;
	pthread_mutex_unlock(get_malloc_mutex());
	return (node + 1);
}

void	free_all(void)
{
	t_list_gc	*curr;
	t_list_gc	*next;

	pthread_mutex_lock(get_malloc_mutex());
	curr = *get_manger();
	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	*get_manger() = NULL;
	pthread_mutex_unlock(get_malloc_mutex());
}

void	ft_free(void *ptr)
{
	t_list_gc	*node;

	if (!ptr)
		return ;
	pthread_mutex_lock(get_malloc_mutex());
	node = (t_list_gc *)ptr - 1;
	if (node->prev)
		node->prev->next = node->next;
	else
		*get_manger() = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node);
	pthread_mutex_unlock(get_malloc_mutex());
}

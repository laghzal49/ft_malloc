/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaghzal <tlaghzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 06:00:00 by tlaghzal          #+#    #+#             */
/*   Updated: 2026/03/28 06:16:20 by tlaghzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdlib.h>

typedef struct s_list_gc
{
	struct s_list_gc	*prev;
	struct s_list_gc	*next;
}	t_list_gc;

void		ft_panic(void *ptr);
void		free_all(void);
void		*ft_malloc(size_t size);
t_list_gc	**get_manger(void);
void		ft_free(void *ptr);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaghzal <tlaghzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 06:30:00 by tlaghzal          #+#    #+#             */
/*   Updated: 2026/03/28 06:30:00 by tlaghzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "../ft_malloc.h"

#define NUM_THREADS 5
#define ALLOCS_PER_THREAD 100

typedef struct s_thread_data
{
	int		thread_id;
	int		success;
	int		fail;
}	t_thread_data;

void	*thread_func(void *arg)
{
	t_thread_data	*data;
	void			*ptrs[ALLOCS_PER_THREAD];
	int				i;
	size_t			size;
	size_t			j;

	data = (t_thread_data *)arg;
	data->success = 0;
	data->fail = 0;
	i = 0;
	while (i < ALLOCS_PER_THREAD)
	{
		size = (size_t)(i + 1) * 10;
		ptrs[i] = ft_malloc(size);
		if (!ptrs[i])
		{
			data->fail++;
			i++;
			continue ;
		}
		memset(ptrs[i], data->thread_id, size);
		j = 0;
		while (j < size)
		{
			if (((unsigned char *)ptrs[i])[j] != (unsigned char)data->thread_id)
			{
				data->fail++;
				break ;
			}
			j++;
		}
		if (j == size)
			data->success++;
		if (i % 2 == 0)
		{
			ft_free(ptrs[i]);
			ptrs[i] = NULL;
		}
		i++;
	}
	i = 0;
	while (i < ALLOCS_PER_THREAD)
	{
		if (ptrs[i] != NULL)
		{
			ft_free(ptrs[i]);
			ptrs[i] = NULL;
		}
		i++;
	}
	return (NULL);
}

static void	test_free_all(void)
{
	void	*leak1;
	void	*leak2;

	leak1 = ft_malloc(100);
	leak2 = ft_malloc(200);
	if (leak1)
		memset(leak1, 'A', 100);
	if (leak2)
		memset(leak2, 'B', 200);
	printf("Calling free_all() to clean up remaining allocations...\n");
	free_all();
}

int			main(void)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				total_success;
	int				total_fail;
	int				i;
	int				ret;

	total_success = 0;
	total_fail = 0;
	printf("Starting multi-threaded ft_malloc test ");
	printf("with %d threads...\n", NUM_THREADS);
	i = 0;
	ret = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].thread_id = i;
		if (pthread_create(&threads[i], NULL, thread_func, &thread_data[i]) != 0)
		{
			perror("pthread_create");
			ret = 1;
			break ;
		}
		i++;
	}
	if (ret != 0)
		return (1);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		total_success += thread_data[i].success;
		total_fail += thread_data[i].fail;
		i++;
	}
	printf("Test completed.\n");
	printf("Total allocations verified: %d\n", total_success);
	printf("Total failures: %d\n", total_fail);
	test_free_all();
	if (total_fail == 0)
	{
		printf("\n✅ SUCCESS: All tests passed!\n");
		return (0);
	}
	else
	{
		printf("\n❌ FAILURE: %d tests failed.\n", total_fail);
		return (1);
	}
}
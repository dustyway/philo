/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:36:20 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/07 22:57:50 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	*philosopher_thread(void *data)
{
	t_philo	*ph;
	/* int		status; */

	ph = data;
	printf("hello from philo %d\n", ph->id);
	if (gettimeofday(&ph->lastmeal, NULL) == -1)
		return ((void *)TIME_ERROR);
	/* while (!ph->data->end) */
	/* { */
	/* 	status = philo_think(ph); */
	/* 	if (status != SUCCESS) */
	/* 		return ((void *)(intptr_t)status); */
	/* 	status = philo_eat(ph); */
	/* 	if (status != SUCCESS) */
	/* 		return ((void *)(intptr_t)status); */
	/* 	if (ph->data->end) */
	/* 		return ((void *)SUCCESS); */
	/* 	philo_sleep(ph); */
	/* 	if (status != SUCCESS) */
	/* 		return ((void *)(intptr_t)status); */
	/* } */
	return ((void *)SUCCESS);
}

void	*waiter_thread(void *data)
{
	t_data	*d;
	/* int		i; */

	printf("hello from waiter\n");
	d = (t_data *)data;
	/* while (1) */
	/* { */
	/* 	printf("running waiter\n"); */
	/* 	while ((d->n_eating != 0 || (int) d->eat_queue->size < d->n_philo / 2)) */
	/* 		usleep(42); */
	/* 	while (d->n_eating != d->n_philo / 2) */
	/* 	{ */
	/* 		i = -1; */
	/* 		while (++i < (int) d->eat_queue->size) */
	/* 		{ */
	/* 			printf("try eating"); */
	/* 			if (try_eating(lst_nth(d->eat_queue->front, i)) == SUCCESS) */
	/* 			{ */
	/* 				dequeue_nth(d->eat_queue, i); */
	/* 				break ; */
	/* 			} */
	/* 		} */
	/* 	} */
	/* } */
	return (NULL);
}

int	checker(t_data *d)
{
	int	n_satisfied;
	int	i;
	int elapsed;

	while (1)//n_satisfied != d->n_philo)
	{
		n_satisfied = 0;
		i = -1;
		usleep(10);
		while (++i < d->n_philo)
		{
			pthread_mutex_lock(&d->philos[i].eating_or_check);
			elapsed = time_elapsed_since(d->philos[i].lastmeal);
			if (elapsed > d->tt_die)
			{
				write_message(&d->philos[i], DIED);
				return (SUCCESS);
			}
			pthread_mutex_unlock(&d->philos[i].eating_or_check);
			//if (d->philos[i].n)
		}
	}
	return (SUCCESS);
}

int	start_simulation(t_data *data)
{
	int	i;

	if (gettimeofday(&data->start_simulation, NULL) == -1)
		return (TIME_ERROR);
	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_create(&data->philos[i].thread_id, NULL, philosopher_thread,
				   &data->philos[i]))
			return (THREAD_ERROR);
		i++;
	}
	printf("starting waiter\n");
	if (pthread_create(&data->waiter_id, NULL, waiter_thread, data))
		return (THREAD_ERROR);
	return (0);
	//return (checker(data));
}

void	cleanup_simulation(t_data *data)
{
	int	i;

	i = -1;
	// while (++i < data->n_philo)
	// 	pthread_join(data->philos[i].thread_id, NULL);
	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&data->forks[i].mtx);
	free(data->forks);
	free(data->philos);
	clear_queue(&data->eat_queue);
}

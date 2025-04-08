/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:36:20 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/08 19:08:36 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"


void 	*checker_thread(void *data)
{
    t_philo *ph;
    ph = data;
    printf("hello from checker %d\n", ph->id);
    /* int	n_satisfied; */
	/* int elapsed; */
	/* t_data d; */

	/* d = data; */
	/* while (1)//n_satisfied != d->n_philo) */
	/* { */
	/* 	usleep(42); */
	/* 		pthread_mutex_lock(&d->philos[i].eating_or_check); */
	/* 		elapsed = time_elapsed_since(d->philos[i].lastmeal); */
	/* 		if (elapsed > d->tt_die) */
	/* 		{ */
	/* 			write_message(&d->philos[i], DIED); */
	/* 			return (SUCCESS); */
	/* 		} */
	/* 		pthread_mutex_unlock(&d->philos[i].eating_or_check); */
	/* 		//if (d->philos[i].n) */
	/* } */
	return (NULL);
}

void	*philosopher_thread(void *data)
{
	t_philo	*ph;
	int		status;

	ph = data;
	printf("hello from philo %d\n", ph->id);
	if (gettimeofday(&ph->lastmeal, NULL) == -1)
		return ((void *)TIME_ERROR);
	if (pthread_create(&ph->checker_id, NULL, checker_thread, ph))
	    return ((void *)THREAD_ERROR);

	while (!ph->data->end)
	{
	    printf("hi\n");
	    sleep(1);
		status = philo_think(ph);
		if (status != SUCCESS)
			return ((void *)(intptr_t)status);
		/* status = philo_eat(ph); */
		/* if (status != SUCCESS) */
		/* 	return ((void *)(intptr_t)status); */
		/* if (ph->data->end) */
		/* 	return ((void *)SUCCESS); */
		/* philo_sleep(ph); */
		/* if (status != SUCCESS) */
		/* 	return ((void *)(intptr_t)status); */
	}
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
	return (SUCCESS);
}

void	cleanup_simulation(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo) {
	    pthread_join(data->philos[i].thread_id, NULL);
	    pthread_join(data->philos[i].checker_id, NULL);	    
	}
	pthread_join(data->waiter_id, NULL);
	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&data->forks[i].mtx);
	free(data->forks);
	free(data->philos);
	clear_queue(&data->eat_queue);
}

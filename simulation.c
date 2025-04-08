/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:36:20 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/09 00:16:58 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void unlock_philos(void *ph) {
	t_philo	*philo;
	philo = ph;
	pthread_mutex_unlock(&philo->allow_eating);
	printf("unlocked philo: %d\n", philo->id);
}


void 	*checker_thread(void *data)
{
    t_philo *ph;
    ph = data;
    printf("hello from checker %d\n", ph->id);

	int elapsed;
	while (!ph->data->end)
	{
	    /* printf("running checker\n"); */
	    pthread_mutex_lock(&ph->eating_or_check);
	    elapsed = time_elapsed_since(ph->lastmeal);
	    /* printf("elapsed %d\n", elapsed); */
	    /* printf("end %d\n", ph->data->end); */
	    if (elapsed > ph->data->tt_die)
	    {
		write_message(ph, DIED);
		ph->data->end = 1;
		queue_iter(ph->data->eat_queue, unlock_philos);
		pthread_mutex_unlock(&ph->eating_or_check);
		return ((void *)SUCCESS);
	    }
	    /* printf("after elapsed %d\n", elapsed); */
	    pthread_mutex_unlock(&ph->eating_or_check);
	    usleep(42);
	    /* printf("end2 %d\n", ph->data->end); */
	}
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
	pthread_mutex_lock(&ph->allow_eating);
	while (!ph->data->end)
	{
		status = philo_think(ph);
		if (status != SUCCESS)
			return ((void *)(intptr_t)status);
		status = philo_eat(ph);
		if (status != SUCCESS)
			return ((void *)(intptr_t)status);
		if (ph->data->end)
			return ((void *)SUCCESS);
		philo_sleep(ph);
		if (status != SUCCESS)
			return ((void *)(intptr_t)status);
	}
	return ((void *)SUCCESS);
}

void	*waiter_thread(void *data)
{
	t_data	*d;
	int		i;

	printf("hello from waiter\n");
	d = (t_data *)data;
	while (!d->end)
	{
		printf("running waiter\n");
		while (!d->end && (d->n_eating != 0 || (int) d->eat_queue->size < d->n_philo / 2))
			usleep(42);
		usleep(42);
		while (!d->end && d->n_eating != d->n_philo / 2)
		{
			i = -1;
			while (++i < (int) d->eat_queue->size)
			{
				printf("try eating\n");
				if (try_eating(lst_nth(d->eat_queue->front, i)) == SUCCESS)
				{
					dequeue_nth(d->eat_queue, i);
					break ;
				}
			}
			/* printf("number eating: %d/%d\n", d->n_eating, d->n_philo); */
		}
		printf("served philos\n");
	}
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
		i+=2;
	}
	usleep(42);
	i = 1;
	while (i < data->n_philo)
	{
		if (pthread_create(&data->philos[i].thread_id, NULL, philosopher_thread,
				   &data->philos[i]))
			return (THREAD_ERROR);
		i+=2;
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
	/* if(pthread_mutex_destroy(&data->write_access)) */
	/*     printf("write acces destroy failed\n"); */
	if(pthread_mutex_destroy(&data->data_access))
	    printf("data acces destroy failed\n");
	if(pthread_mutex_destroy(&data->waiter_lock))
	   printf("waiter lock destroy failed\n");
	free(data->forks);
	free(data->philos);
	clear_queue(&data->eat_queue);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:51:37 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/10 23:08:20 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	create_philos(t_data *data, t_philo **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->n_philo);
	if (!*philos)
		return (MALLOC_ERROR);
	i = -1;
	while (++i < data->n_philo)
	{
		(*philos)[i].id = i;
		(*philos)[i].allow_eating = 0;
		(*philos)[i].meals_counter = 0;
		(*philos)[i].data = data;
		(*philos)[i].left_fork = &data->forks[i];
		(*philos)[i].right_fork = &data->forks[(i + 1) % data->n_philo];
		if (pthread_mutex_init(&(*philos)[i].mtx_allow_eating, NULL) ||
		    pthread_mutex_init(&(*philos)[i].mtx_eating_or_check, NULL) )
			return (MUTEX_ERROR);
	}
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	data->n_philo_started = 0;
	data->n_eating = 0;
	data->end = 0;
	init_queue(&data->eat_queue, NULL);
	data->forks = malloc(sizeof(t_fork) * data->n_philo);
	if (!data->forks)
		return (MALLOC_ERROR);
	i = 0;
	while (i < data->n_philo)
	{
		data->forks[i].active = 0;
		if (pthread_mutex_init(&data->forks[i++].mtx, NULL) != 0)
			return (MUTEX_ERROR);
	}
	if (pthread_mutex_init(&data->mtx_write_access, NULL) != 0
		|| pthread_mutex_init(&data->mtx_data_access, NULL) != 0
		|| pthread_mutex_init(&data->mtx_waiter_lock, NULL) != 0)
		return (MUTEX_ERROR);
	i = create_philos(data, &data->philos);
	return (i);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("usage: %s nbr_philosophers time_to_die "
				"time_to_eat time_to_sleep [times_each_philosopher_must_eat]\n",
				argv[0]);
		return (1);
	}
	if (parse_input(&data, argv) || init_data(&data))
	{
		printf("Error while parsing or initialising the data.\n");
		return (1);
	}
	if (start_simulation(&data) != SUCCESS)
	{
		printf("Error starting the simulation.\n");
		return (1);
	}
	cleanup_simulation(&data);
	return (0);
}

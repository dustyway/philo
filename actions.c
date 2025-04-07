/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 22:06:16 by pschneid          #+#    #+#             */
/*   Updated: 2025/03/21 22:14:33 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	philo_sleep(t_philo *ph)
{
	int	status;

	status = write_message(ph, SLEEP);
	if (status != SUCCESS)
		return (status);
	usleep(ph->data->tt_sleep * 1000);
	return (SUCCESS);
}

int	philo_think(t_philo *ph)
{
	return (write_message(ph, THINK));
}

int	try_eating(t_philo *ph)
{
	if (!ph)
		return (NULL_ERROR);
	if (!ph->left_fork->active && !ph->right_fork->active)
	{
		pthread_mutex_lock(&ph->left_fork->mtx);
		ph->left_fork->active = 1;
		write_message(ph, FORK);
		pthread_mutex_lock(&ph->right_fork->mtx);
		ph->right_fork->active = 1;
		write_message(ph, FORK);
		pthread_mutex_unlock(&ph->allow_eating);
		return (SUCCESS);
	}
	return (FORKS_BLOCKED);
}

int	philo_eat(t_philo *ph)
{
	enqueue(ph->data->eat_queue, ph);
	pthread_mutex_lock(&ph->allow_eating);
	pthread_mutex_lock(&ph->eating_or_check);
	pthread_mutex_lock(&ph->data->data_access);
	ph->data->n_eating++;
	pthread_mutex_unlock(&ph->data->data_access);
	if (gettimeofday(&ph->lastmeal, NULL) != 0)
		return (TIME_ERROR);
	pthread_mutex_unlock(&ph->eating_or_check);
	write_message(ph, EAT);
	usleep(ph->data->tt_eat * 1000);
	ph->meals_counter++;
	ph->left_fork->active = 0;
	if (pthread_mutex_unlock(&ph->left_fork->mtx))
		return (MUTEX_ERROR);
	ph->right_fork->active = 0;
	if (pthread_mutex_unlock(&ph->right_fork->mtx))
		return (MUTEX_ERROR);
	pthread_mutex_lock(&ph->data->data_access);
	ph->data->n_eating--;
	pthread_mutex_unlock(&ph->data->data_access);
	return (SUCCESS);
}

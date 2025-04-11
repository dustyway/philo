/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 22:06:16 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/11 21:59:03 by pschneid         ###   ########.fr       */
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
	pthread_mutex_lock(&ph->left_fork->mtx);
	pthread_mutex_lock(&ph->right_fork->mtx);	
	if (!ph->left_fork->is_active && !ph->right_fork->is_active)
	{
		ph->left_fork->is_active = 1;
		pthread_mutex_unlock(&ph->left_fork->mtx);
		write_message(ph, FORK);
		ph->right_fork->is_active = 1;
		pthread_mutex_unlock(&ph->right_fork->mtx);			
		write_message(ph, FORK);
		sync_printf(ph->data, "allow eating: %d\n", ph->id);
		pthread_mutex_lock(&ph->mtx_allow_eating);
		ph->allow_eating = 1;
		pthread_mutex_unlock(&ph->mtx_allow_eating);		
		return (SUCCESS);
	}
	pthread_mutex_unlock(&ph->left_fork->mtx);
	pthread_mutex_unlock(&ph->right_fork->mtx);				
	return (FORKS_BLOCKED);
}

int wait_for_permission(t_philo *ph)
{
    while(1) {
        pthread_mutex_lock(&ph->mtx_allow_eating);
	if (ph->data->end) {
	    pthread_mutex_unlock(&ph->mtx_allow_eating);
	    return (END_SIMULATION);
	}
	if (ph->allow_eating)
	{
	    pthread_mutex_unlock(&ph->mtx_allow_eating);
	    ph->allow_eating = 0;
	    return (SUCCESS);
	}
	pthread_mutex_unlock(&ph->mtx_allow_eating);
	usleep(10);
    }
}


int	philo_eat(t_philo *ph)
{
	enqueue(ph->data->eat_queue, ph);
	sync_printf(ph->data, "enqued, queue size %d\n", (int) queue_size(ph->data->eat_queue));
	if (wait_for_permission(ph) == END_SIMULATION) {
	    sync_printf(ph->data, "returning %d because end\n", ph->id);
	    return (END_SIMULATION);
	}
	sync_printf(ph->data, "allowed eating %d\n", ph->id);
	pthread_mutex_lock(&ph->mtx_eating_or_check);
	pthread_mutex_lock(&ph->data->mtx_data_access);
	ph->data->n_eating++;
	pthread_mutex_unlock(&ph->data->mtx_data_access);
	if (gettimeofday(&ph->lastmeal, NULL) != 0)
		return (TIME_ERROR);
	pthread_mutex_unlock(&ph->mtx_eating_or_check);
	write_message(ph, EAT);
	usleep(ph->data->tt_eat * 1000);
	ph->meals_counter++;
	pthread_mutex_lock(&ph->data->mtx_data_access);
	if (pthread_mutex_lock(&ph->left_fork->mtx) || pthread_mutex_lock(&ph->right_fork->mtx))
		return (MUTEX_ERROR);
	ph->left_fork->is_active = 0;	
	ph->right_fork->is_active = 0;
	if (pthread_mutex_unlock(&ph->left_fork->mtx) || pthread_mutex_unlock(&ph->right_fork->mtx))
		return (MUTEX_ERROR);
	ph->data->n_eating--;
	pthread_mutex_unlock(&ph->data->mtx_data_access);
	return (SUCCESS);
}

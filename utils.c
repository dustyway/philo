/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 22:06:21 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/10 23:11:17 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"
#include <stdarg.h>

uint64_t	time_elapsed_since(struct timeval timeref)
{
	struct timeval	time;
	uint64_t		msref;
	uint64_t		ms;

	gettimeofday(&time, NULL);
	ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	msref = (timeref.tv_sec * 1000) + (timeref.tv_usec / 1000);
	return (ms - msref);
}

int	write_message(t_philo *ph, enum e_actions action)
{
	char		*str;

	if (pthread_mutex_lock(&(ph->data->mtx_write_access)))
		return (MUTEX_ERROR);
	if (action == FORK)
		str = "has taken a fork";
	else if (action == EAT)
		str = "is eating";
	else if (action == SLEEP)
		str = "is sleeping";
	else if (action == THINK)
		str = "is thinking";
	else if (action == DIED)
		str = "died";
	else
		str = "unknown";
	printf("%-6d %d %s\n", (int)time_elapsed_since(ph->data->start_simulation),
		ph->id, str);
	if (action != DIED)
	{
		if (pthread_mutex_unlock(&(ph->data->mtx_write_access)))
			return (MUTEX_ERROR);
	}
	return (SUCCESS);
}


int sync_printf(t_data *data, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    pthread_mutex_lock(&data->mtx_write_access);
    vprintf(format, args);
    pthread_mutex_unlock(&data->mtx_write_access);
    va_end(args);
    return 0;
}


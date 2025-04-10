/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:33:52 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/10 23:23:45 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# include "queue.h"
# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

enum					e_errors
{
	SUCCESS = 0,
	END_SIMULATION,
	FORKS_BLOCKED,
	PHILO_DIED,
	MALLOC_ERROR,
	MUTEX_ERROR,
	TIME_ERROR,
	THREAD_ERROR,
	NULL_ERROR
};

enum					e_actions
{
	FORK = 0,
	EAT,
	SLEEP,
	THINK,
	DIED,
	SATISFIED
};

typedef struct s_data	t_data;

typedef struct s_fork
{
	pthread_mutex_t		mtx;
	int					active;
	void				*resource;
}						t_fork;

typedef struct s_philo
{
	int					id;
	int					meals_counter;
	int		allow_eating;
	pthread_t			thread_id;
    	pthread_t			checker_id;
	struct timeval		lastmeal;
	t_fork				*left_fork;
	t_fork				*right_fork;
	pthread_mutex_t		mtx_allow_eating;
    pthread_mutex_t		mtx_eating_or_check;
	t_data				*data;
}						t_philo;

struct					s_data
{
	int					n_philo;
    int                                         n_philo_started;
	int					tt_die;
	int					tt_eat;
	int					tt_sleep;
	int					n_meals;
	struct timeval		start_simulation;
	volatile int		end;
	int					n_eating;
	pthread_mutex_t		mtx_data_access;
	pthread_mutex_t		mtx_write_access;
	pthread_mutex_t		mtx_waiter_lock;
	t_fork				*forks;
	t_philo				*philos;
	t_queue				*eat_queue;
	pthread_t			waiter_id;
};

int						parse_input(t_data *sd, char **argv);
int						write_message(t_philo *ph, enum e_actions action);
uint64_t				time_elapsed_since(struct timeval timeref);
int						philo_sleep(t_philo *ph);
int						philo_think(t_philo *ph);
int						try_eating(t_philo *ph);
int						philo_eat(t_philo *ph);
int						start_simulation(t_data *data);
void					cleanup_simulation(t_data *data);
void					check_size(t_queue *queue);
void unlock_philos(void *ph);
int sync_printf(t_data *data, const char *format, ...);
#endif

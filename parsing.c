/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:35:55 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/07 20:58:36 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static int	_isspace(int c)
{
	return (('\t' <= c && c <= '\r') || c == 32);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	my_atoi(int *nbr, const char *nptr)
{
	long	n;

	n = 0;
	while (_isspace(*nptr))
		nptr++;
	if (*nptr == '+')
		nptr++;
	else if (*nptr == '-' && nptr++)
		return (1);
	if (!ft_isdigit(*nptr))
		return (1);
	while (ft_isdigit(*nptr))
	{
		n = 10 * n + *nptr++ - '0';
		if (n >= INT_MAX)
			return (1);
	}
	if (*nptr == '\0')
	{
		*nbr = (int)n;
		return (0);
	}
	return (1);
}

int	parse_input(t_data *d, char **argv)
{
	if (my_atoi(&d->n_philo, argv[1]))
		return (1);
	if (my_atoi(&d->tt_die, argv[2]))
		return (1);
	if (my_atoi(&d->tt_eat, argv[3]))
		return (1);
	if (my_atoi(&d->tt_sleep, argv[4]))
		return (1);
	if (argv[5])
	{
		if (my_atoi(&d->n_meals, argv[5]))
			return (1);
	}
	else
		d->n_meals = -1;
	return (0);
}

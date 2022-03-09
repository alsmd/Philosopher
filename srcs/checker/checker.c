/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:44 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/09 11:15:27 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	check_dissatisfaction(t_philo **philo)
{
	int	index;

	if (philo[0]->data->meals_must_eat == -1)
		return (1);
	index = 0;
	while (philo[index])
	{
		pthread_mutex_lock(philo[index]->last_meal_locker);
		if (philo[index]->n_meals < philo[index]->data->meals_must_eat)
		{
			pthread_mutex_unlock(philo[index]->last_meal_locker);
			return (1);
		}
		pthread_mutex_unlock(philo[index]->last_meal_locker);
		index++;
	}
	pthread_mutex_lock(philo[0]->data->end_simulation_lock);
	philo[0]->data->end_simulation = 1;
	pthread_mutex_unlock(philo[0]->data->end_simulation_lock);
	return (0);
}

int	check_is_dead(t_philo *philo)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->last_meal_locker);
	if (get_miliseconds(current_time) - philo->last_meal > \
		philo->data->time_to_die && philo->last_meal != -1)
	{
		pthread_mutex_unlock(philo->last_meal_locker);
		message(DIED, philo);
		pthread_mutex_lock(philo->data->end_simulation_lock);
		philo->data->end_simulation = 1;
		pthread_mutex_unlock(philo->data->end_simulation_lock);
		return (1);
	}
	pthread_mutex_unlock(philo->last_meal_locker);
	return (0);
}

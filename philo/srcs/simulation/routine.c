/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:22 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 15:38:55 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	one_philo_routine(t_philo **philos)
{
	pthread_t		info;

	set_time(&philos[0]->data->start_simulation);
	set_time(&philos[0]->last_meal);
	message(TAKEN_FORK, philos[0]);
	pthread_create(&info, NULL, &death_checker, philos);
	pthread_join(info, NULL);
}

static void	lunch(t_philo *philo)
{
	pthread_mutex_lock(philo->fork);
	pthread_mutex_lock(philo->fork_right);
	message(TAKEN_FORK, philo);
	message(TAKEN_FORK, philo);
	pthread_mutex_lock(philo->last_meal_locker);
	if (philo->data->end_simulation)
	{
		pthread_mutex_unlock(philo->last_meal_locker);
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork);
		return ;
	}
	set_time(&philo->last_meal);
	philo->n_meals += 1;
	pthread_mutex_unlock(philo->last_meal_locker);
	message(EATING, philo);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->fork_right);
	pthread_mutex_unlock(philo->fork);
}

void	first_iteration(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(5000);
	pthread_mutex_lock(philo->last_meal_locker);
	set_time(&philo->last_meal);
	pthread_mutex_unlock(philo->last_meal_locker);
}

void	*lifespan(void *p)
{
	t_philo	*philo;
	int		first;

	first = 1;
	philo = (t_philo *)p;
	while (1)
	{
		if (first)
			first_iteration(philo);
		first = 0;
		if (check_is_dead(philo))
			return (NULL);
		lunch(philo);
		pthread_mutex_lock(philo->data->end_simulation_lock);
		if (philo->data->end_simulation)
			return (unlock(philo->data->end_simulation_lock));
		pthread_mutex_unlock(philo->data->end_simulation_lock);
		if (philo->n_meals == philo->data->meals_must_eat)
			return (NULL);
		message(SLEEPING, philo);
		usleep(philo->data->time_to_sleep * 1000);
		message(THINKING, philo);
	}
	return (NULL);
}

void	*death_checker(void *p)
{
	t_philo	**philo;
	int		index;

	philo = (t_philo **)p;
	while (check_dissatisfaction(philo))
	{
		if (!philo[0]->data->start_simulation)
			continue ;
		index = 0;
		while (philo[index])
		{
			check_is_dead(philo[index]);
			pthread_mutex_lock(philo[0]->data->end_simulation_lock);
			if (philo[0]->data->end_simulation)
				return (unlock(philo[0]->data->end_simulation_lock));
			pthread_mutex_unlock(philo[0]->data->end_simulation_lock);
			usleep(1000);
			index++;
		}
	}
	return (NULL);
}

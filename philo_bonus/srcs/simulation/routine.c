/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:22 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 14:25:09 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	lunch(t_philo *philo)
{
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
	message(TAKEN_FORK, philo);
	message(TAKEN_FORK, philo);
	set_time(&philo->last_meal);
	philo->n_meals += 1;
	message(EATING, philo);
	usleep(philo->data->time_to_eat * 1000);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	first_iteration(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(5000);
	set_time(&philo->last_meal);
}

void	*lifespan(void *p)
{
	pthread_t	info;
	t_philo		*philo;
	int			first;

	first = 1;
	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep(5000);
	pthread_create(&info, NULL, &check_is_dead, philo);
	pthread_detach(info);
	while (1)
	{
		if (first)
			first_iteration(philo);
		first = 0;
		lunch(philo);
		if (philo->n_meals == philo->data->meals_must_eat)
		{
			sem_post(philo->data->philo_is_satisfied);
			philo->n_meals += 1;
		}
		message(SLEEPING, philo);
		usleep(philo->data->time_to_sleep * 1000);
		message(THINKING, philo);
	}
	return (NULL);
}

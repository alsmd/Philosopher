/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:44 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 14:23:35 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	*check_is_dead(void *p)
{
	t_philo	*philo;
	struct timeval	current_time;

	philo = (t_philo *)p;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		if (get_miliseconds(current_time) - philo->last_meal > \
			philo->data->time_to_die && philo->last_meal != -1)
		{
			sem_wait(philo->data->philo_is_dead_log);
			message(DIED, philo);
			sem_post(philo->data->philo_is_dead);
		}
		usleep(1000);
	}
}

void	stop_simulation(t_philo **philos)
{
	int	index;

	index = 0;
	while (philos[index])
	{
		kill(philos[index]->process_id, SIGQUIT);
		index++;
	}
	index = 0;
	while (index < philos[0]->data->n_philo)
	{
		sem_post(philos[0]->data->philo_is_satisfied);
		index++;
	}
	sem_post(philos[0]->data->philo_is_dead);
}

void	check_simulation_end(t_philo **philos)
{
	int	index;
	int	id;

	index = 0;
	id = fork();
	if (id == 0)
	{
		sem_wait(philos[0]->data->philo_is_dead);
		stop_simulation(philos);
		free_simulation(philos);
		exit(0);
	}
	id = fork();
	if (id == 0)
	{
		while (index < philos[0]->data->n_philo)
		{
			sem_wait(philos[0]->data->philo_is_satisfied);
			index++;
		}
		stop_simulation(philos);
		free_simulation(philos);
		exit(0);
	}
	waitpid(id, NULL, 0);
}
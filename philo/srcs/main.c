/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:11:34 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 09:27:32 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	main(int argc, char *argv[])
{
	t_philo	**philos;

	if (argc > 6 || argc < 5)
		return (1);
	if (validate(argv + 1))
	{
		printf("Invalid argument line!\n");
		return (1);
	}
	philos = create_simulation(argv + 1);
	start_simulation(philos);
	free_simulation(philos);
	return (0);
}

void	message(char *action, t_philo *philo)
{
	struct timeval	current_time;
	long int		time;

	pthread_mutex_lock(philo->data->message_lock);
	gettimeofday(&current_time, NULL);
	time = get_miliseconds(current_time) - philo->data->start_simulation;
	pthread_mutex_lock(philo->data->end_simulation_lock);
	if (!philo->data->end_simulation)
		printf("%ld %d %s\n", time, philo->id, action);
	pthread_mutex_unlock(philo->data->end_simulation_lock);
	pthread_mutex_unlock(philo->data->message_lock);
}

void	free_mutex(pthread_mutex_t	*m)
{
	pthread_mutex_destroy(m);
	free(m);
}

void	free_simulation(t_philo **philos)
{
	int	index;

	free_mutex(philos[0]->data->end_simulation_lock);
	free_mutex(philos[0]->data->message_lock);
	free(philos[0]->data);
	index = 0;
	while (philos[index])
	{
		free_mutex(philos[index]->fork);
		free_mutex(philos[index]->is_eating_locker);
		free_mutex(philos[index]->last_meal_locker);
		free(philos[index]);
		index++;
	}
	free(philos);
}

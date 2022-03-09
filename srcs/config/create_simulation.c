/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_simulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:33 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/09 11:20:29 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

t_philo	*create_philo(t_data *data)
{
	static int	id;
	t_philo		*new_philo;
	t_philo		*begin;

	new_philo = ft_calloc(1, sizeof(t_philo));
	new_philo->fork = ft_calloc(1, sizeof(pthread_mutex_t));
	new_philo->last_meal = -1;
	new_philo->last_meal_locker = ft_calloc(1, sizeof(pthread_mutex_t));
	new_philo->is_eating_locker = ft_calloc(1, sizeof(pthread_mutex_t));
	pthread_mutex_init(new_philo->fork, NULL);
	pthread_mutex_init(new_philo->last_meal_locker, NULL);
	pthread_mutex_init(new_philo->is_eating_locker, NULL);
	new_philo->data = data;
	id++;
	new_philo->id = id;
	return (new_philo);
}

void	link_forks(t_philo **philos)
{
	int	index;

	index = 0;
	while (philos[index])
	{
		if (philos[index + 1])
			philos[index]->fork_right = philos[index + 1]->fork;
		else
			philos[index]->fork_right = philos[0]->fork;
		index++;
	}
}

t_data	*create_data(char *argv[])
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	data->meals_must_eat = -1;
	data->message_lock = ft_calloc(1, sizeof(pthread_mutex_t));
	data->end_simulation_lock = ft_calloc(1, sizeof(pthread_mutex_t));
	return (data);
}

t_philo	**create_simulation(char *argv[])
{
	t_philo	**philos;
	int		n;
	t_data	*data;
	int		index;

	index = 0;
	philos = 0;
	n = ft_atoi(argv[0]);
	data = create_data(argv);
	pthread_mutex_init(data->message_lock, NULL);
	pthread_mutex_init(data->end_simulation_lock, NULL);
	if (argv[4])
		data->meals_must_eat = ft_atoi(argv[4]);
	philos = (t_philo **) ft_calloc(n + 1, sizeof(t_philo **));
	while (index < n)
	{
		philos[index] = create_philo(data);
		index++;
	}
	link_forks(philos);
	return (philos);
}

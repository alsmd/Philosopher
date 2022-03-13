/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_simulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:33 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 22:29:59 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

t_philo	*create_philo(t_data *data)
{
	static int	id;
	char		*sem_unic;
	t_philo		*new_philo;

	new_philo = ft_calloc(1, sizeof(t_philo));
	new_philo->last_meal = -1;
	new_philo->data = data;
	sem_unic = ft_itoa(id);
	sem_unlink(sem_unic);
	new_philo->last_meal_sem = sem_open(sem_unic, O_CREAT, 0666, 1);
	free(sem_unic);
	sem_unic = ft_itoa(id * -1);
	sem_unlink(sem_unic);
	new_philo->died_sem = sem_open(sem_unic, O_CREAT, 0666, 1);
	free(sem_unic);
	if (new_philo->last_meal_sem == SEM_FAILED || \
		new_philo->died_sem == SEM_FAILED)
	{
		printf("ERROR CREATING SEM::create_simulation.c!\n");
		return (NULL);
	}
	id++;
	new_philo->id = id;
	return (new_philo);
}

t_data	*create_data(char *argv[])
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	sem_unlink(SEM_FORK_AVAIABLE);
	sem_unlink(SEM_PHILO_DEAD);
	sem_unlink(SEM_PHILO_SATISFIED);
	sem_unlink(SEM_PHILO_DEAD_LOG);
	data->forks = sem_open(SEM_FORK_AVAIABLE, O_CREAT, 0666, ft_atoi(argv[0]));
	data->philo_is_dead = sem_open(SEM_PHILO_DEAD, O_CREAT, 0666, 0);
	data->philo_log = sem_open(SEM_PHILO_DEAD_LOG, O_CREAT, 0666, 1);
	data->philo_is_satisfied = sem_open(SEM_PHILO_SATISFIED, O_CREAT, 0666, 0);
	if (data->forks == SEM_FAILED || data->philo_is_dead == SEM_FAILED \
		|| data->philo_is_satisfied == SEM_FAILED || \
		data->philo_log == SEM_FAILED)
	{
		printf("ERROR CREATING SEM::create_simulation.c!\n");
		return (NULL);
	}
	data->meals_must_eat = -1;
	if (argv[4])
		data->meals_must_eat = ft_atoi(argv[4]);
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
	data->n_philo = n;
	philos = (t_philo **) ft_calloc(n + 1, sizeof(t_philo **));
	while (index < n)
	{
		philos[index] = create_philo(data);
		index++;
	}
	return (philos);
}

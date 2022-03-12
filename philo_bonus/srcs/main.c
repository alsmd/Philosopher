/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:11:34 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 14:22:57 by flda-sil         ###   ########.fr       */
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
	return (0);
}

void	message(char *action, t_philo *philo)
{
	struct timeval	current_time;
	long int		time;

	gettimeofday(&current_time, NULL);
	time = get_miliseconds(current_time) - philo->data->start_simulation;
	printf("%ld %d %s\n", time, philo->id, action);
}

void	free_sem(sem_t	*s)
{
	sem_close(s);
}

void	free_simulation(t_philo **philos)
{
	int	index;
	
	free_sem(philos[0]->data->forks);
	free_sem(philos[0]->data->philo_is_dead_log);
	free_sem(philos[0]->data->philo_is_dead);
	free_sem(philos[0]->data->philo_is_satisfied);
	free(philos[0]->data);
	index = 0;
	while (philos[index])
	{
		free_sem(philos[index]->last_meal_sem);
		free(philos[index]);
		index++;
	}
	free(philos);
}

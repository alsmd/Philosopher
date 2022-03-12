/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:28 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 08:58:29 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	start_simulation(t_philo **philos)
{
	int				index;
	pthread_t		info;

	index = 0;
	if (philos[1] == 0)
	{
		one_philo_routine(philos);
		return ;
	}
	set_time(&philos[0]->data->start_simulation);
	while (philos[index])
	{
		pthread_create(&philos[index]->thread, NULL, &lifespan, philos[index]);
		index++;
	}
	pthread_create(&info, NULL, &death_checker, philos);
	index = 0;
	while (philos[index])
	{
		pthread_join(philos[index]->thread, NULL);
		index++;
	}
	pthread_join(info, NULL);
}

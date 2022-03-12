/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:28 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 14:13:33 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	start_simulation(t_philo **philos)
{
	int				index;

	index = 0;
	set_time(&philos[0]->data->start_simulation);
	while (philos[index])
	{
		philos[index]->process_id = fork();
		if (philos[index]->process_id == 0)
			lifespan(philos[index]);
		index++;
	}
	check_simulation_end(philos);
	free_simulation(philos);
}

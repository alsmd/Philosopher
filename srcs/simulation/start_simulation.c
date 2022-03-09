#include <philo.h>

void	start_simulation(t_philo **philos)
{
	int				index;
	pthread_t		info;

	index = 0;
	if (philos[1] == 0)
		one_philo_routine(philos);
	else
	{
		while (philos[index])
		{
			pthread_create(&philos[index]->thread, NULL, &lifespan, philos[index]);
			index++;
		}
		pthread_create(&info, NULL, &death_checker, philos);
		set_time(&philos[0]->data->start_simulation);
		index = 0;
		while (philos[index])
		{
			pthread_join(philos[index]->thread, NULL);
			index++;
		}
		pthread_join(info, NULL);
	}
	
}

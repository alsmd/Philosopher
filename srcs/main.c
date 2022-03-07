#include <philo.h>

int	main(int argc, char *argv[])
{
	t_philo	**philos;

	if (argc > 6 || argc < 5)
		return (1);
	if (validate(argv + 1))
		return (1);
	philos = create_simulation(argv + 1);
	start_simulation(philos);
	return (0);
}

void	message(char *action, t_philo *philo)
{
	struct	timeval current_time;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->data->message_lock);
	if (!philo->data->end_simulation)
		printf("%ld %d %s\n", get_miliseconds(current_time) - philo->data->start_simulation, philo->id, action);
	pthread_mutex_unlock(philo->data->message_lock);
}
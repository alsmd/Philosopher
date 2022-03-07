#include <philo.h>


int	try_get_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->fork);
	pthread_mutex_lock(philo->fork_right);
	message(TAKEN_FORK, philo);
	return (1);
}

int	start_eating(t_philo *philo)
{
	message(EATING, philo);
	usleep(philo->data->time_to_eat * 1000);
	
	pthread_mutex_lock(philo->last_meal_locker);
	set_time(&philo->last_meal);
	philo->n_meals += 1;
	pthread_mutex_unlock(philo->last_meal_locker);
	pthread_mutex_unlock(philo->fork);
	pthread_mutex_unlock(philo->fork_right);
	if (philo->n_meals == philo->data->meals_must_eat)
		return (1);
	message(SLEEPING, philo);
	usleep(philo->data->time_to_sleep * 1000);
	return (0);
}

void	check_is_dead(t_philo *philo)
{
	struct	timeval current_time;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->last_meal_locker);
	if (philo->last_meal && (get_miliseconds(current_time) - philo->last_meal >= philo->data->time_to_die) && philo->n_meals != philo->data->meals_must_eat)
	{
		pthread_mutex_unlock(philo->last_meal_locker);
		message(DIED, philo);
		pthread_mutex_lock(philo->data->end_simulation_lock);
		philo->data->end_simulation = 1;
		pthread_mutex_unlock(philo->data->end_simulation_lock);
	}
	else
		pthread_mutex_unlock(philo->last_meal_locker);
}

void	*lifespan(void *p)
{
	t_philo	*philo;
	int		n_meals;
	int		first;

	first = 1;
	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep(5000);
	while (1)
	{
		if (first)
		{
			pthread_mutex_lock(philo->last_meal_locker);
			set_time(&philo->last_meal);
			pthread_mutex_unlock(philo->last_meal_locker);
		}
		if (philo->data->end_simulation)
			break;
		first = 0;
		if (!try_get_fork(philo))
			continue ;
		if (start_eating(philo))
			break ;
		if (philo->id % 2 == 0)
			usleep(5000);
		message(THINKING, philo);
	}
}

void	*death_checker(void *p)
{
	t_philo	**philo;
	int		index;

	index = 0;
	philo = (t_philo **)p;
	while (1)
	{
		check_is_dead(philo[index]);
		if (philo[index]->data->end_simulation)
		{
			printf("%d\n", philo[index]->n_meals);
			return (NULL);
		}
		index++;
		if (philo[index] == NULL)
			index = 0;
	}
}
void	start_simulation(t_philo **philos)
{
	int				index;
	pthread_t		info;

	index = 0;
	pthread_create(&info, NULL, &death_checker, philos);
	set_time(&philos[0]->data->start_simulation);
	while (philos[index])
	{
		pthread_create(&philos[index]->thread, NULL, &lifespan, philos[index]);
		index++;
	}
	index = 0;
	while (philos[index])
	{
		pthread_join(philos[index]->thread, NULL);
		index++;
	}
}
